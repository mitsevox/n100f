#ifndef RPSKIN_H
#define RPSKIN_H

#include <rpworld.h>

#include "rphanim.h"

typedef struct RpSkin RpSkin;

typedef struct RwMatrixWeights RwMatrixWeights;
struct RwMatrixWeights
{
    F32 w0;
    F32 w1;
    F32 w2;
    F32 w3;
};

typedef enum RpSkinType
{
    rpNASKINTYPE = 0,
    rpSKINTYPEGENERIC = 1,
    rpSKINTYPEMATFX = 2,
    rpSKINTYPETOON = 3,
    rpSKINTYPEFORCEENUMSIZEINT = 0x7FFFFFFF
} RpSkinType;

// The bone matrices and per-vertex bone indices and weights of a skinned geometry, carved from
// one 16-byte aligned allocation. On GameCube, unk10 and unk14 hold an instanced resource and its
// size until the geometry is next instanced (skinstream.c).
struct RpSkin
{
    U32 numBones;
    RwMatrix* skinToBoneMatrices;
    U32* vertexBoneIndices;
    RwMatrixWeights* vertexBoneWeights;
    void* unk10;
    S32 unk14;
    void* unaligned;
};

// The plugin's state: the plugin offsets, the shared matrix cache, the skin free list and the
// skin pipeline.
typedef struct SkinGlobals SkinGlobals;
struct SkinGlobals
{
    S32 engineOffset;
    S32 atomicOffset;
    S32 geometryOffset;
    RwMatrix* matrixCache;
    void* matrixCacheUnaligned;
    RwFreeList* freeList;
    RwModuleInfo module;
    U8 unk20[4];
    RxPipeline* pipeline;
    U8 unk28[0x10];
};

#define SKINGEOMETRYGETDATA(geometry)                                                              \
    (*(RpSkin**)((U8*)(geometry) + _rpSkinGlobals.geometryOffset))
#define SKINATOMICGETDATA(atomic)                                                                  \
    (*(RpHAnimHierarchy**)((U8*)(atomic) + _rpSkinGlobals.atomicOffset))

#ifdef __cplusplus
extern "C" {
#endif

extern SkinGlobals _rpSkinGlobals;

RpGeometry* _rpSkinInitialize(RpGeometry* geometry);
RpGeometry* _rpSkinDeinitialize(RpGeometry* geometry);
S32 _rpSkinPipelinesCreate(U32 pipes);
S32 _rpSkinPipelinesDestroy(void);
RpAtomic* _rpSkinPipelinesAttach(RpAtomic* atomic, RpSkinType type);
S32 _rpSkinGeometryNativeSize(const RpGeometry* geometry);
RwStream* _rpSkinGeometryNativeWrite(RwStream* stream, const RpGeometry* geometry);
RwStream* _rpSkinGeometryNativeRead(RwStream* stream, RpGeometry* geometry);

S32 RpSkinPluginAttach(void);
RpSkin* RpSkinGeometryGetSkin(RpGeometry* geometry);
RpGeometry* RpSkinGeometrySetSkin(RpGeometry* geometry, RpSkin* skin);
RpSkin* RpSkinDestroy(RpSkin* skin);
U32 RpSkinGetNumBones(RpSkin* skin);
const RwMatrixWeights* RpSkinGetVertexBoneWeights(RpSkin* skin);
const U32* RpSkinGetVertexBoneIndices(RpSkin* skin);
const RwMatrix* RpSkinGetSkinToBoneMatrices(RpSkin* skin);
RpAtomic* RpSkinAtomicSetHAnimHierarchy(RpAtomic* atomic, RpHAnimHierarchy* hierarchy);
RpAtomic* RpSkinAtomicSetType(RpAtomic* atomic, RpSkinType type);

#ifdef __cplusplus
}
#endif

#endif
