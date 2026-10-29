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

struct _xMat4x3
{
    xVec3 right;
    S32 flags;
    xVec3 up;
    U32 pad1;
    xVec3 at;
    U32 pad2;
    xVec3 pos;
    U32 pad3;
};

struct _xVec4
{
    F32 x;
    F32 y;
    F32 z;
    F32 w;
};

struct _xQuat
{
    xVec3 v;
    F32 s;
};

struct _xSphere
{
    xVec3 center;
    F32 r;
};

struct _xRay3
{
    xVec3 origin;
    xVec3 dir;
    F32 min_t;
    F32 max_t;
    S32 flags;
};

struct _xRot
{
    xVec3 axis;
    F32 angle;
};

struct _xIsect
{
    U32 flags;
    F32 penned;
    F32 contained;
    F32 lapped;
    xVec3 point;
    xVec3 norm;
    F32 dist;
};

typedef struct _xMat3x3 xMat3x3;
typedef struct _xMat4x3 xMat4x3;
typedef struct _xVec4 xVec4;
typedef struct _xQuat xQuat;
typedef struct _xSphere xSphere;
typedef struct _xRot xRot;
typedef struct _xIsect xIsect;

extern const xMat4x3 g_I3;
extern const xVec3 g_O3;
extern const xVec3 g_Y3;
extern const xVec3 g_NY3;

inline void xVec3Init(xVec3* v, F32 _x, F32 _y, F32 _z)
{
    v->x = _x;
    v->y = _y;
    v->z = _z;
}

inline void xVec3Sub(xVec3* o, const xVec3* a, const xVec3* b)
{
    o->x = a->x - b->x;
    o->y = a->y - b->y;
    o->z = a->z - b->z;
}

inline void xVec3Copy(xVec3* o, const xVec3* v)
{
    o->x = v->x;
    o->y = v->y;
    o->z = v->z;
}

inline void xVec3Add(xVec3* o, const xVec3* a, const xVec3* b)
{
    o->x = a->x + b->x;
    o->y = a->y + b->y;
    o->z = a->z + b->z;
}

inline void xVec3SMul(xVec3* o, const xVec3* v, F32 s)
{
    o->x = v->x * s;
    o->y = v->y * s;
    o->z = v->z * s;
}

inline void xVec3SMulBy(xVec3* v, F32 s)
{
    v->x *= s;
    v->y *= s;
    v->z *= s;
}

inline F32 xVec3Length2(const xVec3* vec)
{
    return vec->x * vec->x + vec->y * vec->y + vec->z * vec->z;
}

F32 xVec3Normalize(xVec3* o, const xVec3* v);

inline F32 xVec3Dot(const xVec3* a, const xVec3* b)
{
    return a->x * b->x + a->y * b->y + a->z * b->z;
}

inline void xVec3Cross(xVec3* o, const xVec3* a, const xVec3* b)
{
    xVec3 temp;

    temp.x = a->y * b->z - b->y * a->z;
    temp.y = a->z * b->x - b->z * a->x;
    temp.z = a->x * b->y - b->x * a->y;
    *o = temp;
}

inline void xVec3Lerp(xVec3* o, const xVec3* a, const xVec3* b, F32 t)
{
    xVec3 temp;

    temp.x = a->x + (b->x - a->x) * t;
    temp.y = a->y + (b->y - a->y) * t;
    temp.z = a->z + (b->z - a->z) * t;
    *o = temp;
}

void xMat3x3RotY(xMat3x3* m, F32 t);
void xMat3x3RotC(xMat3x3* m, F32 _x, F32 _y, F32 _z, F32 t);
void xMat3x3Euler(xMat3x3* m, F32 yaw, F32 pitch, F32 roll);
void xMat3x3Euler(xMat3x3* m, xVec3* ypr);
void xMat3x3RMulVec(xVec3* o, const xMat3x3* m, const xVec3* v);
void xMat3x3GetEuler(const xMat3x3* m, xVec3* a);
void xMat3x3LMulVec(xVec3* o, const xMat3x3* m, const xVec3* v);
void xMat3x3LMulRotY(xMat3x3* o, const xMat3x3* m, F32 t);
void xMat3x3RMulRotY(xMat3x3* o, const xMat3x3* m, F32 t);
void xMat4x3MoveLocalRight(xMat4x3* m, F32 mag);
void xMat4x3MoveLocalUp(xMat4x3* m, F32 mag);
void xMat4x3MoveLocalAt(xMat4x3* m, F32 mag);
void xQuatToMat(const xQuat* q, xMat3x3* m);
void xMat4x3Mul(xMat4x3* o, const xMat4x3* a, const xMat4x3* b);
void xMat4x3Rot(xMat4x3* m, const xVec3* a, F32 t, const xVec3* p);
F32 xMat3x3LookVec(xMat3x3* m, const xVec3* at);
void xMat4x3RMul3x3(xMat4x3* o, const xMat3x3* m3x3, const xMat4x3* m);

#endif
