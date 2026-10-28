#ifndef IMODEL_H
#define IMODEL_H

#include <rpworld.h>

#include "xMath3.h"
#include "xVec3.h"

struct iModelTag
{
    xVec3 v;
    U32 matidx;
    F32 wt[4];
};

void iModelTagEval(RpAtomic* model, iModelTag* tag, RwMatrix* mat, xVec3* dest);
U32 iModelTagSetup(iModelTag* tag, RpAtomic* model, F32 x, F32 y, F32 z);
U32 iModelTagIndex(iModelTag* tag, RpAtomic* model, S32 v);
U32 iModelVertEval(RpAtomic* model, U32 index, U32 count, RwMatrix* mat, xVec3* vert, xVec3* dest);
U32 iModelVertCount(RpAtomic* model);
S32 iModelCull(RpAtomic* model, RwMatrix* mat);
void iModelRender(RpAtomic* model, RwMatrix* mat);
void iModelQuatToMat(xQuat* quat, xVec3* tran, RwMatrix* mat);
void iModelAnimMatrices(RpAtomic* model, xQuat* quat, xVec3* tran, RwMatrix* mat);
U32 iModelNumBones(RpAtomic* model);
RpAtomic* iModelFile_RWMultiAtomic(RpAtomic* model);
void iModelUnload(RpAtomic* userdata);
RpAtomic* iModelFileNew(void* buffer, U32 size);
void iModelInit();

#endif
