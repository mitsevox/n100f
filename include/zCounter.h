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

void zCounterLoad(_zCounter* ent, xSerial* s);
void zCounterSave(_zCounter* ent, xSerial* s);
void zCounterReset(xBase* b);

#endif
