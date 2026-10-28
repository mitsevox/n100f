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

void zTimerLoad(_zTimer* ent, xSerial* s);
void zTimerSave(_zTimer* ent, xSerial* s);

#endif
