#ifndef XMODEL_H
#define XMODEL_H

#include "types.h"
#include <rwcore.h>

struct xModelPool;
struct xAnimPlay;
struct RpAtomic;

// Partial: members up to Mat.
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
};

void xModelRender(xModelInstance* modelInst);

#endif
