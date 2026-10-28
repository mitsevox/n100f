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

#ifdef __cplusplus
extern "C" {
#endif

RpSkin* RpSkinGeometryGetSkin(RpGeometry* geometry);
U32 RpSkinGetNumBones(RpSkin* skin);
const RwMatrixWeights* RpSkinGetVertexBoneWeights(RpSkin* skin);
const U32* RpSkinGetVertexBoneIndices(RpSkin* skin);
const RwMatrix* RpSkinGetSkinToBoneMatrices(RpSkin* skin);
RpAtomic* RpSkinAtomicSetHAnimHierarchy(RpAtomic* atomic, RpHAnimHierarchy* hierarchy);

#ifdef __cplusplus
}
#endif

#endif
