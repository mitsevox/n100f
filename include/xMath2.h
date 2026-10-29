#ifndef XMATH2_H
#define XMATH2_H

#include "types.h"

#include <math.h>

struct _xVec2
{
    F32 x;
    F32 y;
};

struct _xRect
{
    F32 x;
    F32 y;
    F32 w;
    F32 h;
};

typedef struct _xVec2 xVec2;
typedef struct _xRect xRect;

inline F32 xVec2Dist(F32 x1, F32 y1, F32 x2, F32 y2)
{
    F32 x = x1 - x2;
    F32 y = y1 - y2;

    return sqrtf(x * x + y * y);
}

inline F32 xVec2Normalize(xVec2* o, const xVec2* v)
{
    F32 len2 = v->x * v->x + v->y * v->y;

    if (len2 == 1.0f)
    {
        if (o != v)
        {
            *o = *v;
        }
        return 1.0f;
    }
    else if (len2 == 0.0f)
    {
        if (o != v)
        {
            *o = *v;
        }
        return 0.0f;
    }
    else
    {
        F32 len = sqrtf(len2);
        F32 inv = 1.0f / len;
        o->x = v->x * inv;
        o->y = v->y * inv;
        return len;
    }
}

#endif
