#ifndef XMODEL_H
#define XMODEL_H

#include "types.h"
#include "xMath3.h"
#include "xVec3.h"
#include <rwcore.h>

struct xModelPool;
struct xAnimPlay;
struct RpAtomic;

// Partial: members up to Scale.
struct xModelInstance
{
    xModelInstance* Next;
    xModelInstance* Parent;
    xModelPool* Pool;
    xAnimPlay* Anim;
    RpAtomic* Data;
    void* Object;
    U16 Flags;
    U8 BoneCount;
    U8 BoneIndex;
    U8* BoneRemap;
    RwMatrix* Mat;
    xVec3 Scale;
};

struct xModelPool
{
    xModelPool* Next;
    U32 NumMatrices;
    xModelInstance* List;
};

struct _xRect;
struct RpWorld;

void xModelInit();
void xModelPoolInit(U32 count, U32 numMatrices);
void xModelSetMaterialAlpha(xModelInstance* modelInst, S32 alpha);
void xModelResetMaterial(xModelInstance* modelInst);
void xModelMaterialMul(xModelInstance* modelInst, F32 rm, F32 gm, F32 bm);
void xModelSetMaterialTextureRestore(xModelInstance* modelInst);
void xModelSetMaterialTexture(xModelInstance* modelInst, void* texture);
void xModelRender2D(xModelInstance* modelInst, _xRect* sr, xVec3* from, xVec3* to, F32 fov);
void xModelUpdate(xModelInstance* modelInst, F32 timeDelta);
void xModelEval(xModelInstance* modelInst);
void xModelRender(xModelInstance* modelInst);
xModelInstance* xModelInstanceAlloc(RpAtomic* data, void* object, U16 flags, U8 boneIndex,
                                    U8* boneRemap);
void xModelInstanceFree(xModelInstance* modelInst);
void xModelInstanceAttach(xModelInstance* inst, xModelInstance* parent);
void xModelRenderSingle(xModelInstance* modelInst);
void xModelSetRender2DStyle(U32 renderStyle, F32 alpha);
void xModel_SceneEnter(RpWorld* world);
void xModel_SceneExit(RpWorld* world);

inline xMat4x3* xModelGetFrame(xModelInstance* modelInst)
{
    return (xMat4x3*)modelInst->Mat;
}

#endif
