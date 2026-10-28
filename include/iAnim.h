#ifndef IANIM_H
#define IANIM_H

#include "xMath3.h"

void iAnimInit();
void iAnimEval(void* RawData, F32 time, U32 flags, xVec3* tran, xQuat* quat);

#endif
