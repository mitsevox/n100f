#ifndef XMATH2_H
#define XMATH2_H

#include "types.h"

#include <math.h>

inline F32 xVec2Dist(F32 x1, F32 y1, F32 x2, F32 y2)
{
    F32 x = x1 - x2;
    F32 y = y1 - y2;

    return sqrtf(x * x + y * y);
}

#endif
