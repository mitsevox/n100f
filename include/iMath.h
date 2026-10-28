#ifndef IMATH_H
#define IMATH_H

#include "types.h"

#define iabs(x) (float)(__fabs((float)x))

void iMathInit();
void iMathExit();
void isrand(U32 seed);
U32 irand();
F32 iurand();

#endif
