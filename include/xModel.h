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

void xModelPoolInit(U32 count, U32 numMatrices);
void xModelEval(xModelInstance* modelInst);
void xModelRender(xModelInstance* modelInst);
xModelInstance* xModelInstanceAlloc(RpAtomic* data, void* object, U16 flags, U8 boneIndex,
                                    U8* boneRemap);
void xModelInstanceFree(xModelInstance* modelInst);
void xModelInstanceAttach(xModelInstance* inst, xModelInstance* parent);

inline xMat4x3* xModelGetFrame(xModelInstance* modelInst)
{
    return (xMat4x3*)modelInst->Mat;
}

#endif
