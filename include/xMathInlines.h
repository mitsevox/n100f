#ifndef XMATHINLINES_H
#define XMATHINLINES_H

#include "types.h"
#include "xVec3.h"

#include <math.h>

inline F32 SQ(F32 x)
{
    return x * x;
}

inline F32 xsqrt(F32 x)
{
    return sqrtf(x);
}

inline F32 xVec3Length(const xVec3* v)
{
    return xsqrt(v->x * v->x + v->y * v->y + v->z * v->z);
}

#endif
