#ifndef ZCOUNTER_H
#define ZCOUNTER_H

#include "xBase.h"

struct zCounterAsset : p2BaseAsset
{
    S16 count;
};

struct _zCounter : xBase
{
    zCounterAsset* asset;
    S16 count;
    U8 state;
    U8 pad;
};

void zCounterInit(void* b, void* asset);
void zCounterReset(xBase* b);
void zCounterSave(_zCounter* ent, xSerial* s);
void zCounterLoad(_zCounter* ent, xSerial* s);
S32 zCounterEventCB(xBase*, xBase* to, U32 toEvent, F32*, xBase*);

#endif
