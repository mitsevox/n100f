#ifndef ZTIMER_H
#define ZTIMER_H

#include "types.h"
#include "xBase.h"

struct zTimerAsset : p2BaseAsset
{
    F32 seconds;
};

struct _zTimer : xBase
{
    zTimerAsset* tasset;
    U8 state;
    U8 pad[3];
    F32 secondsLeft;
};

struct _xScene;

void zTimerInit(void* b, void* tasset);
void zTimerSave(_zTimer* ent, xSerial* s);
void zTimerLoad(_zTimer* ent, xSerial* s);
S32 zTimerEventCB(xBase*, xBase* to, U32 toEvent, F32*, xBase*);
void zTimerUpdate(xBase* to, _xScene*, F32 dt);

#endif
