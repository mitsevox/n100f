#ifndef ZFOG_H
#define ZFOG_H

#include "xBase.h"

struct zFogAsset : p2BaseAsset
{
    U8 bkgndColor[4];
    U8 fogColor[4];
    F32 fogDensity;
    F32 fogStart;
    F32 fogStop;
    F32 transitionTime;
    U8 fogType;
    U8 padFog[3];
};

struct _zFog : xBase
{
    zFogAsset* tasset;
};

S32 zFogEventCB(xBase* from, xBase* to, U32 toEvent, F32* toParam, xBase* b3);
void zFogInit(void* b, void* tasset);

#endif
