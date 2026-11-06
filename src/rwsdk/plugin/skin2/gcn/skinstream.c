#include <rpskin.h>
#include <string.h>

// GameCube skin streaming: the skin data of a native (pre-instanced) geometry, written and read
// as its own chunk after the platform word.

void _rxGCResEntryWaitDone(RwResEntry* resEntry);

// The platform a native chunk was written for, and the skin plugin id. Written for this project.
#define SKINSTREAM_PLATFORMGCN 6
#define SKINSTREAM_PLUGINID 0x116

// Hands back the instanced arrays a skin kept from streaming to the geometry's resource entry,
// which then waits for the GPU before it is freed.
#define SKINSTREAM_RESTOREARRAYS(_geometry, _skin)                                                 \
    if ((_skin)->savedPos)                                                                         \
    {                                                                                              \
        GCNVertexArrays* arrays = (GCNVertexArrays*)((_geometry)->repEntry + 1);                   \
        ((RwResEntry**)arrays->pos)[-1]->ownerRef = NULL;                                          \
        arrays->pos = (_skin)->savedPos;                                                           \
        arrays->nrm = (_skin)->savedNrm;                                                           \
        (_geometry)->repEntry->destroyNotify = _rxGCResEntryWaitDone;                              \
        (_skin)->savedPos = NULL;                                                                  \
        (_skin)->savedNrm = NULL;                                                                  \
    }

// The native chunk size includes its header, platform word and bone count.
S32 _rpSkinGeometryNativeSize(const RpGeometry* geometry)
{
    S32 size = geometry->numVertices * (sizeof(U32) + sizeof(RwMatrixWeights));
    RpSkin* skin = SKINGEOMETRYGETDATA(geometry);

    size += sizeof(U32) * 5;
    size += skin->numBones * sizeof(RwMatrix);

    SKINSTREAM_RESTOREARRAYS(geometry, skin);

    return size;
}

// The platform and bone count precede the indices, weights and matrices.
RwStream* _rpSkinGeometryNativeWrite(RwStream* stream, const RpGeometry* geometry)
{
    S32 platform = SKINSTREAM_PLATFORMGCN;
    RpSkin* skin;
    S32 numVertices;

    if (!_rwStreamWriteVersionedChunkHeader(stream, rwID_STRUCT,
                                            _rpSkinGeometryNativeSize(geometry) - sizeof(U32) * 3,
                                            rwLIBRARYCURRENTVERSION, 0xFFFF))
    {
        return NULL;
    }

    if (!RwStreamWriteInt32(stream, &platform, sizeof(S32)))
    {
        return NULL;
    }

    numVertices = geometry->numVertices;
    skin = SKINGEOMETRYGETDATA(geometry);

    if (!RwStreamWriteInt32(stream, (S32*)&skin->numBones, sizeof(S32)))
    {
        return NULL;
    }

    if (!RwStreamWrite(stream, skin->vertexBoneIndices, numVertices * sizeof(U32)))
    {
        return NULL;
    }

    if (!RwStreamWrite(stream, skin->vertexBoneWeights, numVertices * sizeof(RwMatrixWeights)))
    {
        return NULL;
    }

    if (!RwStreamWrite(stream, skin->skinToBoneMatrices, skin->numBones * sizeof(RwMatrix)))
    {
        return NULL;
    }

    return stream;
}

// The three bone arrays share the streamed payload allocation.
RwStream* _rpSkinGeometryNativeRead(RwStream* stream, RpGeometry* geometry)
{
    U32 version;
    U32 size;
    S32 platform;
    S32 numVertices;
    RpSkin* skin;

    if (!RwStreamFindChunk(stream, rwID_STRUCT, &size, &version))
    {
        return NULL;
    }

    if (version < rwLIBRARYBASEVERSION || version > rwLIBRARYCURRENTVERSION)
    {
        RwError err;
        err.pluginID = SKINSTREAM_PLUGINID;
        err.errorCode = _rwerror(0x80000004);
        RwErrorSet(&err);
        return NULL;
    }

    if (!RwStreamReadInt32(stream, &platform, sizeof(S32)))
    {
        return NULL;
    }

    if (platform != SKINSTREAM_PLATFORMGCN)
    {
        return NULL;
    }

    skin = (RpSkin*)RWSRCGLOBAL(memoryAlloc)(_rpSkinGlobals.freeList);
    memset(skin, 0, sizeof(RpSkin));
    numVertices = geometry->numVertices;
    size -= sizeof(S32) * 2;
    skin->unaligned = RwMalloc(size);
    skin->vertexBoneIndices = (U32*)skin->unaligned;
    skin->vertexBoneWeights = (RwMatrixWeights*)(skin->vertexBoneIndices + numVertices);
    skin->skinToBoneMatrices = (RwMatrix*)(skin->vertexBoneWeights + numVertices);

    if (!RwStreamReadInt32(stream, (S32*)&skin->numBones, sizeof(S32)))
    {
        return NULL;
    }

    if (size != RwStreamRead(stream, skin->unaligned, size))
    {
        return NULL;
    }

    RpSkinGeometrySetSkin(geometry, skin);

    return stream;
}
