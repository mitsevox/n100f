#ifndef IANIM_H
#define IANIM_H

#include "types.h"
#include "xMath3.h"

extern U8* giAnimScratch;

void iAnimInit();
void iAnimEval(void* RawData, F32 time, U32 flags, xVec3* tran, xQuat* quat);

#endif
