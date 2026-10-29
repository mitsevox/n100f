#ifndef XFFX_H
#define XFFX_H

#include "xVec3.h"

struct _xEnt;
struct _xScene;

struct xFFX
{
    U32 flags;
    void (*doEffect)(_xEnt*, _xScene*, F32, void*);
    void* fdata;
    xFFX* next;
};

struct xFFXShakeState
{
    xVec3 disp;
    F32 dur;
    F32 freq;
    F32 tmr;
    F32 alpha;
    F32 lval;
    xFFXShakeState* next;
};

struct xFFXRotMatchState
{
    S32 lgrounded;
    xVec3 lfup;
    xVec3 lfat;
    xVec3 plfat;
    F32 tmr;
    F32 mrate;
    F32 tmatch;
    F32 rrate;
    F32 trelax;
    F32 max_decl;
    xFFXRotMatchState* next;
};

void xFFXApply(_xEnt* ent, _xScene* sc, F32 dt);
S16 xFFXAddEffect(_xEnt* ent, void (*dof)(_xEnt*, _xScene*, F32, void*), void* fd);
xFFX* xFFXAlloc();
void xFFXTurnOn(xFFX* f);
void xFFXTurnOff(xFFX* f);
xFFXShakeState* xFFXShakeAlloc();
xFFXRotMatchState* xFFXRotMatchAlloc();

#endif
