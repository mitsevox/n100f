#include <rpworld.h>
#include <dolphin/gx/GXManage.h>
#include <dolphin/os.h>

// Native geometry and world sector data: a GameCube instance entry (the vertex arrays header, the
// display list records and the arrays and lists they point into) streamed as one chunk, with its
// pointers stored as offsets from the start of the data.

extern S32 lbl_8025E7C0;
extern U16 lbl_8025E800;

void _rxGCResEntryWaitDone(RwResEntry* resEntry);
RpGameCubeVtxFmt* _rpGameCubeVtxFmtGetDefault(void);
RpGameCubeVtxFmt* fn_8015D818(const RpGeometry* geometry);
RpGameCubeVtxFmt* fn_8015D824(const RpWorld* world);

S32 _rpGeometryNativeSize(const RpGeometry* geometry);
RwStream* _rpGeometryNativeWrite(RwStream* stream, const RpGeometry* geometry);
RpGeometry* _rpGeometryNativeRead(RwStream* stream, RpGeometry* geometry);
S32 _rpWorldSectorNativeSize(const RpWorldSector* sector);
RwStream* _rpWorldSectorNativeWrite(RwStream* stream, const RpWorldSector* sector);
RpWorldSector* _rpWorldSectorNativeRead(RwStream* stream, RpWorldSector* sector);

// The platform a native chunk was written for.
#define NATIVE_PLATFORMGCN 6

// A stream chunk header: its type, size and version words.
#define NATIVE_CHUNKHEADERSIZE (sizeof(U32) * 3)

#define NATIVE_ALIGN32(_size) (((_size) + 0x1F) & ~0x1F)

// The vertex arrays header of an instance entry, and the start of its data for numMeshes meshes.
#define NATIVE_ARRAYS(_entry) ((GCNVertexArrays*)((_entry) + 1))
#define NATIVE_DATA(_entry, _numMeshes)                                                            \
    ((U8*)NATIVE_ALIGN32((U32)NATIVE_ARRAYS(_entry) + sizeof(GCNVertexArrays) +                     \
                         (_numMeshes) * sizeof(GCNDisplayList)))

// The bytes an instance entry takes in a stream: the chunk header, the platform and the two sizes,
// the vertex arrays header and display list records and the data.
#define NATIVE_STREAMSIZE(_entry, _numMeshes)                                                      \
    ((_numMeshes) * sizeof(GCNDisplayList) +                                                       \
     (NATIVE_CHUNKHEADERSIZE + sizeof(S32) * 3 + sizeof(GCNVertexArrays)) +                        \
     ((_entry)->size - (NATIVE_DATA(_entry, _numMeshes) - (U8*)NATIVE_ARRAYS(_entry))))

static void* _rpNativeRead(RwStream* stream, void* object, RwResEntry** entryRef, U16 numMeshes,
                           S32 numVerts)
{
    U32 size;
    U32 version;
    S32 platform;
    S32 dataSize;
    S32 headerSize;
    GCNVertexArrays* arrays;
    GCNDisplayList* lists;
    U8* data;
    S32 i;

    if (!RwStreamFindChunk(stream, rwID_STRUCT, &size, &version))
    {
        return NULL;
    }

    if (version < rwLIBRARYBASEVERSION || version > rwLIBRARYCURRENTVERSION)
    {
        RwError err;
        err.pluginID = 2;
        err.errorCode = _rwerror(0x80000004);
        RwErrorSet(&err);
        return NULL;
    }

    if (!RwStreamReadInt32(stream, &platform, sizeof(platform)))
    {
        return NULL;
    }

    if (platform != NATIVE_PLATFORMGCN)
    {
        return NULL;
    }

    if (!RwStreamReadInt32(stream, &headerSize, sizeof(headerSize)))
    {
        return NULL;
    }

    if (!RwStreamReadInt32(stream, &dataSize, sizeof(dataSize)))
    {
        return NULL;
    }

    *entryRef = (RwResEntry*)RwMalloc(NATIVE_ALIGN32(sizeof(RwResEntry) + headerSize) + dataSize +
                                      0x20);

    arrays = NATIVE_ARRAYS(*entryRef);
    if (RwStreamRead(stream, arrays, headerSize) != headerSize)
    {
        return NULL;
    }

    lists = (GCNDisplayList*)(arrays + 1);
    data = (U8*)NATIVE_ALIGN32((U32)lists + numMeshes * sizeof(GCNDisplayList));
    if (RwStreamRead(stream, data, dataSize) != dataSize)
    {
        return NULL;
    }

    (*entryRef)->link.next = NULL;
    (*entryRef)->link.prev = NULL;
    (*entryRef)->owner = object;
    (*entryRef)->size = size;
    (*entryRef)->ownerRef = entryRef;
    (*entryRef)->destroyNotify = _rxGCResEntryWaitDone;

    arrays->syncToken = lbl_8025E800;

    if (arrays->pos && !(arrays->flags & GCNARRAYS_EXTERNALPOS))
    {
        arrays->pos = data + (U32)arrays->pos;
    }

    if (arrays->nrm && !(arrays->flags & GCNARRAYS_EXTERNALNRM))
    {
        arrays->nrm = data + (U32)arrays->nrm;
    }

    if (arrays->clr)
    {
        arrays->clr = data + (U32)arrays->clr;
    }

    for (i = 0; arrays->tex[i] && i < 8 && !(arrays->flags & GCNARRAYS_EXTERNALTEX); i++)
    {
        arrays->tex[i] = data + (U32)arrays->tex[i];
    }

    for (i = numMeshes; i; i--)
    {
        lists->list = data + (U32)lists->list;
        lists++;
    }

    DCFlushRange(NATIVE_ARRAYS(*entryRef), (*entryRef)->size);
    GXInvalidateVtxCache();

    return object;
}

static RwStream* _rpNativeWrite(RwStream* stream, RwResEntry* entry, RpGameCubeVtxFmt* vtxFmt,
                                U16 numMeshes, S32 numVerts)
{
    GCNVertexArrays* arrays = NATIVE_ARRAYS(entry);
    GCNDisplayList* lists = (GCNDisplayList*)(arrays + 1);
    S32 headerSize = sizeof(GCNVertexArrays) + numMeshes * sizeof(GCNDisplayList);
    U8* data = (U8*)NATIVE_ALIGN32((U32)lists + numMeshes * sizeof(GCNDisplayList));
    S32 chunkSize = NATIVE_STREAMSIZE(entry, numMeshes) - NATIVE_CHUNKHEADERSIZE;
    S32 dataSize;
    GCNDisplayList* list;
    S32 i;

    if (!vtxFmt)
    {
        vtxFmt = _rpGameCubeVtxFmtGetDefault();
    }

    if (arrays->pos && !(arrays->flags & GCNARRAYS_EXTERNALPOS))
    {
        arrays->pos = (void*)((U8*)arrays->pos - data);
    }

    if (arrays->nrm && !(arrays->flags & GCNARRAYS_EXTERNALNRM))
    {
        arrays->nrm = (void*)((U8*)arrays->nrm - data);
    }

    if (arrays->clr && !(arrays->flags & GCNARRAYS_EXTERNALCLR))
    {
        arrays->clr = (void*)((U8*)arrays->clr - data);
    }

    for (i = 0; arrays->tex[i] && i < 8 && !(arrays->flags & GCNARRAYS_EXTERNALTEX); i++)
    {
        arrays->tex[i] = (void*)((U8*)arrays->tex[i] - data);
    }

    list = lists;
    for (i = numMeshes; i; i--)
    {
        list->list = (void*)((U8*)list->list - data);
        list++;
    }

    if (!_rwStreamWriteVersionedChunkHeader(stream, rwID_STRUCT, chunkSize, rwLIBRARYCURRENTVERSION,
                                            0xFFFF))
    {
        return NULL;
    }

    if (!RwStreamWriteInt32(stream, &lbl_8025E7C0, sizeof(S32)))
    {
        return NULL;
    }

    if (!RwStreamWriteInt32(stream, &headerSize, sizeof(headerSize)))
    {
        return NULL;
    }

    dataSize = entry->size - (data - (U8*)NATIVE_ARRAYS(entry));
    if (!RwStreamWriteInt32(stream, &dataSize, sizeof(dataSize)))
    {
        return NULL;
    }

    if (!RwStreamWrite(stream, arrays, headerSize))
    {
        return NULL;
    }

    if (!RwStreamWrite(stream, data, dataSize))
    {
        return NULL;
    }

    if (arrays->pos && !(arrays->flags & GCNARRAYS_EXTERNALPOS))
    {
        arrays->pos = data + (U32)arrays->pos;
    }

    if (arrays->nrm && !(arrays->flags & GCNARRAYS_EXTERNALNRM))
    {
        arrays->nrm = data + (U32)arrays->nrm;
    }

    if (arrays->clr)
    {
        arrays->clr = data + (U32)arrays->clr;
    }

    for (i = 0; arrays->tex[i] && i < 8 && !(arrays->flags & GCNARRAYS_EXTERNALTEX); i++)
    {
        arrays->tex[i] = data + (U32)arrays->tex[i];
    }

    for (i = numMeshes; i; i--)
    {
        lists->list = data + (U32)lists->list;
        lists++;
    }

    DCFlushRange(NATIVE_ARRAYS(entry), entry->size);
    GXInvalidateVtxCache();

    return stream;
}

S32 _rpGeometryNativeSize(const RpGeometry* geometry)
{
    if ((geometry->flags & rpGEOMETRYNATIVE) && geometry->repEntry)
    {
        return NATIVE_STREAMSIZE(geometry->repEntry, geometry->mesh->numMeshes);
    }

    return 0;
}

RwStream* _rpGeometryNativeWrite(RwStream* stream, const RpGeometry* geometry)
{
    if (geometry->flags & rpGEOMETRYNATIVE)
    {
        _rpNativeWrite(stream, geometry->repEntry, fn_8015D818(geometry),
                       geometry->mesh->numMeshes, geometry->numVertices);
    }

    return stream;
}

RpGeometry* _rpGeometryNativeRead(RwStream* stream, RpGeometry* geometry)
{
    return (RpGeometry*)_rpNativeRead(stream, geometry, &geometry->repEntry,
                                      geometry->mesh->numMeshes, geometry->numVertices);
}

S32 _rpWorldSectorNativeSize(const RpWorldSector* sector)
{
    if ((RpWorldSectorGetWorld(sector)->flags & rpWORLDNATIVE) && sector->repEntry)
    {
        return NATIVE_STREAMSIZE(sector->repEntry, sector->mesh->numMeshes);
    }

    return 0;
}

RwStream* _rpWorldSectorNativeWrite(RwStream* stream, const RpWorldSector* sector)
{
    const RpWorld* world = RpWorldSectorGetWorld(sector);

    if (world->flags & rpWORLDNATIVE)
    {
        S32 numVerts = RpWorldSectorGetNumVertices(sector);

        _rpNativeWrite(stream, sector->repEntry, fn_8015D824(world), sector->mesh->numMeshes,
                       numVerts);
    }

    return stream;
}

RpWorldSector* _rpWorldSectorNativeRead(RwStream* stream, RpWorldSector* sector)
{
    return (RpWorldSector*)_rpNativeRead(stream, sector, &sector->repEntry,
                                         sector->mesh->numMeshes,
                                         RpWorldSectorGetNumVertices(sector));
}
