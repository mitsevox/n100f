#include <rpworld.h>
#include <dolphin/gx/GXManage.h>
#include <dolphin/os.h>

// Native geometry and world sector data: a GameCube instance entry (the vertex arrays header, the
// display list records and the arrays and lists they point into) streamed as one chunk, with its
// pointers stored as offsets from the start of the data.

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

// The platform word _rpNativeWrite writes after the chunk header.
S32 lbl_8025E7C0 = NATIVE_PLATFORMGCN;

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

// The bytes an instance entry takes in a stream: the chunk header, the platform and the two sizes,
// the vertex arrays header and display list records and the data.
// FAKEMATCH: built from the inline helpers NativeStreamSize and NativeDataSize to get the target's
// instruction and register order; real form believed to be one expression of NATIVE_DATASIZE.
#define NATIVE_STREAMSIZE(_entry, _numMeshes)                                                      \
    NativeStreamSize(sizeof(GCNVertexArrays) + (_numMeshes) * sizeof(GCNDisplayList),             \
                     NativeDataSize(_entry, _numMeshes))

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
    // FAKEMATCH: the display list count lives in a one-member struct so it becomes a compiler temp
    // and the unroller's count copy coalesces into it; real form believed to be a plain U32 count.
    struct { U32 n; } c;
    // FAKEMATCH: the texture pointers are walked through tex, a word pointer over the arrays header
    // (tex[9] is arrays->tex[0]), with t holding the loaded pointer; real form believed to index
    // arrays->tex[i] directly.
    void** tex;
    void* t;

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

    c.n = numMeshes;
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

    // FAKEMATCH: the texture pointers are read through a word pointer over the arrays header
    // (tex[9] is arrays->tex[0]); real form believed to loop over arrays->tex[i].
    tex = (void**)arrays;
    for (i = 0; (t = tex[9]) != NULL && i < 8 && !(arrays->flags & GCNARRAYS_EXTERNALTEX); i++)
    {
        tex[9] = data + (U32)t;
        tex++;
    }

    for (; c.n; c.n--)
    {
        list->list = data + (U32)list->list;
        list++;
    }

    DCFlushRange(NATIVE_ARRAYS(*entryRef), (*entryRef)->size);
    GXInvalidateVtxCache();

    return object;
}

// The stream size of the header part (size plus the chunk header and the three ints) and the data.
// FAKEMATCH: this helper adds into its size parameter so the sum keeps its two steps and the add's
// operand order; real form believed to be the sum written in _rpNativeWrite.
static inline S32 NativeStreamSize(S32 size, S32 dataSize)
{
    size += NATIVE_CHUNKHEADERSIZE + sizeof(S32) * 3;
    size += dataSize;
    return size;
}

// NATIVE_DATASIZE for the size functions.
// FAKEMATCH: the aligned data address is built in d over three statements, the last a no-op cast
// that keeps the alignment as its own instruction before the arrays pointer is formed; real form
// believed to be NATIVE_DATASIZE.
static inline S32 NativeDataSize(RwResEntry* entry, U16 numMeshes)
{
    U32 d;

    d = (U32)entry + sizeof(RwResEntry) + sizeof(GCNVertexArrays) +
        numMeshes * sizeof(GCNDisplayList) + 0x1F;
    d &= ~0x1F;
    d = (U32)(U8*)d;
    return entry->size - (d - (U32)NATIVE_ARRAYS(entry));
}

static RwStream* _rpNativeWrite(RwStream* stream, RwResEntry* entry, RpGameCubeVtxFmt* vtxFmt,
                                U32 numMeshes, S32 numVerts)
{
    GCNDisplayList* list;
    U32 count;
    // FAKEMATCH: the first display list walk keeps its pointer, its count and the end of the
    // display lists in one-member structs (compiler temps), and the pointer is set twice, to give
    // the target's register and instruction order; real form believed to use list, count and a
    // plain NATIVE_ALIGN32 expression for data.
    struct { GCNDisplayList* p; } l;
    struct { U32 e; } x;
    struct { U32 n; } c;
    S32 size;
    U8* data;
    S32 chunkSize;
    GCNVertexArrays* arrays;
    GCNDisplayList* lists;
    S32 headerSize;
    S32 dataSize;
    S32 i;
    // FAKEMATCH: both texture loops walk word pointers over the arrays header (tex[9] is
    // arrays->tex[0]), each with its own pointer and value locals; real form believed to index
    // arrays->tex[i].
    void** tex2;
    void* t2;
    void** tex;
    void* t;

    arrays = NATIVE_ARRAYS(entry);
    lists = (GCNDisplayList*)(arrays + 1);
    l.p = lists;
    c.n = numMeshes;
    // FAKEMATCH: the pointer is set a second time and the display lists' end is built in x; real
    // form believed to set list once and use a plain NATIVE_ALIGN32 expression for data.
    l.p = lists;
    x.e = (U32)lists + numMeshes * sizeof(GCNDisplayList);
    x.e += 0x1F;
    data = (U8*)(x.e & ~0x1F);
    size = sizeof(GCNVertexArrays) + numMeshes * sizeof(GCNDisplayList);
    chunkSize = NativeStreamSize(size, NATIVE_DATASIZE(entry, numMeshes)) - NATIVE_CHUNKHEADERSIZE;

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

    // FAKEMATCH: the texture pointers are read through a word pointer over the arrays header
    // (tex[9] is arrays->tex[0]); real form believed to loop over arrays->tex[i].
    tex = (void**)arrays;
    for (i = 0; (t = tex[9]) != NULL && i < 8 && !(arrays->flags & GCNARRAYS_EXTERNALTEX); i++)
    {
        tex[9] = (void*)((U8*)t - data);
        tex++;
    }

    for (; c.n; c.n--)
    {
        l.p->list = (void*)((U8*)l.p->list - data);
        l.p++;
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

    // FAKEMATCH: count and list are set before the arrays are restored; real form believed to set
    // them just before the display list loop.
    count = numMeshes;
    list = lists;

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

    // FAKEMATCH: the texture pointers are read through a word pointer over the arrays header
    // (tex[9] is arrays->tex[0]); real form believed to loop over arrays->tex[i].
    tex2 = (void**)arrays;
    for (i = 0; (t2 = tex2[9]) != NULL && i < 8 && !(arrays->flags & GCNARRAYS_EXTERNALTEX); i++)
    {
        tex2[9] = data + (U32)t2;
        tex2++;
    }

    for (; count; count--)
    {
        list->list = data + (U32)list->list;
        list++;
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
