#include <rpworld.h>
#include <dolphin/gx/GXManage.h>
#include <dolphin/os.h>

// Native geometry and world sector data: a GameCube instance entry (the vertex arrays header, the
// display list records and the arrays and lists they point into) streamed as one chunk, with its
// pointers stored as offsets from the start of the data.

extern S32 lbl_8025E7C0;
extern volatile U16 lbl_8025E800;

void _rxGCResEntryWaitDone(RwResEntry* resEntry);
RpGameCubeVtxFmt* _rpGameCubeVtxFmtGetDefault(void);

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
    ((U8*)NATIVE_ALIGN32((U32)(_entry) + sizeof(RwResEntry) + sizeof(GCNVertexArrays) +           \
                         (_numMeshes) * sizeof(GCNDisplayList)))

// The bytes of an instance entry's data, after its vertex arrays header and display list records.
#define NATIVE_DATASIZE(_entry, _numMeshes)                                                        \
    ((_entry)->size - (NATIVE_DATA(_entry, _numMeshes) - (U8*)NATIVE_ARRAYS(_entry)))

static void* _rpNativeRead(RwStream* stream, void* object, RwResEntry** entryRef, U32 numMeshes,
                           S32 numVerts)
{
    U32 version;
    U32 size;
    S32 headerSize;
    S32 dataSize;
    S32 platform;
    GCNVertexArrays* arrays;
    U8* data;
    GCNDisplayList* lists;
    S32 i;
    GCNDisplayList* list;
    U32 count;

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

    *entryRef = (RwResEntry*)RwMalloc(0x20 + NATIVE_ALIGN32(sizeof(RwResEntry) + headerSize) +
                                      dataSize);

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

    count = numMeshes;
    list = lists;

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

    for (; count; count--)
    {
        list->list = data + (U32)list->list;
        list++;
    }

    DCFlushRange(NATIVE_ARRAYS(*entryRef), (*entryRef)->size);
    GXInvalidateVtxCache();

    return object;
}

static RwStream* _rpNativeWrite(RwStream* stream, RwResEntry* entry, RpGameCubeVtxFmt* vtxFmt,
                                U32 numMeshes, S32 numVerts)
{
    GCNDisplayList* list;
    U32 count;
    S32 size;
    U8* data;
    S32 chunkSize;
    GCNVertexArrays* arrays;
    GCNDisplayList* lists;
    S32 headerSize;
    S32 dataSize;
    S32 i;

    arrays = NATIVE_ARRAYS(entry);
    lists = (GCNDisplayList*)(arrays + 1);
    list = lists;
    count = numMeshes;
    data = (U8*)NATIVE_ALIGN32((U32)lists + numMeshes * sizeof(GCNDisplayList));
    size = sizeof(GCNVertexArrays) + numMeshes * sizeof(GCNDisplayList);
    chunkSize = size + (NATIVE_CHUNKHEADERSIZE + sizeof(S32) * 3) +
                NATIVE_DATASIZE(entry, numMeshes) - NATIVE_CHUNKHEADERSIZE;

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

    for (; count; count--)
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

    headerSize = sizeof(GCNVertexArrays) + numMeshes * sizeof(GCNDisplayList);
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

    list = lists;
    for (count = numMeshes; count; count--)
    {
        list->list = data + (U32)list->list;
        list++;
    }

    DCFlushRange(NATIVE_ARRAYS(entry), entry->size);
    GXInvalidateVtxCache();

    return stream;
}

// The bytes of an instance entry's data (see NATIVE_DATASIZE).
static inline S32 NativeDataSize(RwResEntry* entry, U32 numMeshes)
{
    return NATIVE_DATASIZE(entry, numMeshes);
}

// The bytes of a native chunk before the data: the chunk header, the platform and the two sizes,
// the vertex arrays header and the display list records.
static inline S32 NativeHeaderSize(U32 numMeshes)
{
    return numMeshes * sizeof(GCNDisplayList) +
           (NATIVE_CHUNKHEADERSIZE + sizeof(S32) * 3 + sizeof(GCNVertexArrays));
}

S32 _rpGeometryNativeSize(const RpGeometry* geometry)
{
    if ((geometry->flags & rpGEOMETRYNATIVE) && geometry->repEntry)
    {
        S32 size;

        size = NativeHeaderSize(geometry->mesh->numMeshes);
        size += NativeDataSize(geometry->repEntry, geometry->mesh->numMeshes);
        return size;
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
        S32 size;

        size = NativeHeaderSize(sector->mesh->numMeshes);
        size += NativeDataSize(sector->repEntry, sector->mesh->numMeshes);
        return size;
    }

    return 0;
}

// Writes a world sector's native instance data when its world is native.
RwStream* _rpWorldSectorNativeWrite(RwStream* stream, const RpWorldSector* sector)
{
    const RpWorld* world = RpWorldSectorGetWorld(sector);

    if (world->flags & rpWORLDNATIVE)
    {
        _rpNativeWrite(stream, sector->repEntry, fn_8015D824(world), sector->mesh->numMeshes,
                       RpWorldSectorGetNumVertices(sector));
    }

    return stream;
}

RpWorldSector* _rpWorldSectorNativeRead(RwStream* stream, RpWorldSector* sector)
{
    return (RpWorldSector*)_rpNativeRead(stream, sector, &sector->repEntry,
                                         sector->mesh->numMeshes,
                                         RpWorldSectorGetNumVertices(sector));
}
