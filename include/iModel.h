#ifndef IMODEL_H
#define IMODEL_H

#include <rpworld.h>

#include "xVec3.h"

struct iModelTag
{
    xVec3 v;
    U32 matidx;
    F32 wt[4];
};

void iModelTagEval(RpAtomic* model, iModelTag* tag, RwMatrix* mat, xVec3* dest);
void iModelRender(RpAtomic* model, RwMatrix* mat);

#endif
