#ifndef XMATH_H
#define XMATH_H

#include "iMath.h"
#include "types.h"

#define xabs(x) iabs(x)

F32 xAngleClamp(F32 a);
U32 xMathSolveQuadratic(F32 a, F32 b, F32 c, F32* x1, F32* x2);

#endif
