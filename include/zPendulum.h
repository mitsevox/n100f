#ifndef ZPENDULUM_H
#define ZPENDULUM_H

#include "zEnt.h"
#include "xEntMotion.h"

struct _zPendulum : _zEnt
{
    _xEntMotion motion;
    F32 lt;
    F32 q1t;
    F32 q3t;
};

struct xSerial;
struct _xScene;
struct _xEntFrame;

void zPendulum_Init(void* pend, void* asset);
void zPendulum_Save(_zPendulum* pend, xSerial* s);
void zPendulum_Load(_zPendulum* pend, xSerial* s);
void zPendulum_Setup(_zPendulum* pend, _xScene* sc);
void zPendulum_Reset(_zPendulum* pend, _xScene* sc);
void zPendulum_Update(_zPendulum* pend, _xScene* sc, F32 dt);
void zPendulum_Move(_zPendulum* pend, _xScene* sc, F32 dt, _xEntFrame* frame);
S32 zPendulumEventCB(xBase* from, xBase* to, U32 toEvent, F32* toParam, xBase* toParamWidget);

#endif
