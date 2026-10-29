#ifndef XMATHINLINES_H
#define XMATHINLINES_H

#include "types.h"

#include <math.h>

inline F32 SQ(F32 x)
{
    return x * x;
}

inline F32 xsqrt(F32 x)
{
    return sqrtf(x);
}

#endif
