#ifndef ZSFX_H
#define ZSFX_H

#include "xBase.h"
#include "xVec3.h"

struct zSFXAsset : p2BaseAsset
{
    U16 flagsSFX;
    U16 freq;
    F32 freqm;
    U32 soundAssetID;
    U32 attachID;
    U8 loopCount;
    U8 priority;
    U8 volume;
    U8 pad;
    xVec3 pos;
    F32 range;
    F32 range_falloff;
};

struct zSFX : xBase
{
    zSFXAsset* asset;
    U32 sndID;
};

void zSFXInit(void* t, void* asset);
void zSFXSave(zSFX* ent, xSerial* s);
void zSFXLoad(zSFX* ent, xSerial* s);
S32 zSFXEventCB(xBase*, xBase* to, U32 toEvent, F32* toParam, xBase*);

#endif
