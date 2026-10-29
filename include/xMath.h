#ifndef XMATH_H
#define XMATH_H

#include "iMath.h"
#include "types.h"

#define xabs(x) iabs(x)

#define PI 3.1415927f
#define ONEEIGHTY 180.0f

#define DEG2RAD(x) ((PI) * (x) / (ONEEIGHTY))

#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#define CLAMP(x, a, b) (MAX((a), MIN((x), (b))))
#define xlerp(a, b, t) ((a) + (t) * ((b) - (a)))
#define SQR(x) ((x) * (x))

F32 xAngleClamp(F32 a);
U32 xMathSolveQuadratic(F32 a, F32 b, F32 c, F32* x1, F32* x2);

#endif
