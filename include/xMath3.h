#ifndef XMATH3_H
#define XMATH3_H

#include "types.h"
#include "xVec3.h"

struct _xMat3x3
{
    xVec3 right;
    S32 flags;
    xVec3 up;
    U32 pad1;
    xVec3 at;
    U32 pad2;
};

struct _xMat4x3;

typedef struct _xMat3x3 xMat3x3;
typedef struct _xMat4x3 xMat4x3;

extern const xMat4x3 g_I3;

inline void xVec3Init(xVec3* v, F32 _x, F32 _y, F32 _z)
{
    v->x = _x;
    v->y = _y;
    v->z = _z;
}

void xMat3x3Euler(xMat3x3* m, F32 yaw, F32 pitch, F32 roll);
void xMat3x3LMulVec(xVec3* o, const xMat3x3* m, const xVec3* v);

#endif
