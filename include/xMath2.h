#ifndef XMATH2_H
#define XMATH2_H

#include "types.h"

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

#ifdef __cplusplus
#include "libc/math.h"

inline void xVec2Normalize(xVec2* o, const xVec2* v)
{
    F32 len2 = v->x * v->x + v->y * v->y;

    if (len2 == 1.0f)
    {
        if (o != v)
        {
            *o = *v;
        }
    }
    else if (len2 == 0.0f)
    {
        if (o != v)
        {
            *o = *v;
        }
    }
    else
    {
        F32 inv = 1.0f / sqrtf(len2);
        o->x = v->x * inv;
        o->y = v->y * inv;
    }
}
#endif

#endif
