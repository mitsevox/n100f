#ifndef XVEC3INLINES_H
#define XVEC3INLINES_H

#include "xMath3.h"

#include <math.h>
#include "xMathInlines.h"

inline F32 xVec3Length(const xVec3* v)
{
    return xsqrt(v->x * v->x + v->y * v->y + v->z * v->z);
}

#endif
