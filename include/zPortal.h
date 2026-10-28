#ifndef ZPORTAL_H
#define ZPORTAL_H

#include "types.h"
#include "xBase.h"

struct zPortalAsset : p2BaseAsset
{
    U32 assetCameraID;
    U32 assetMarkerID;
    F32 ang;
    U32 sceneID;
};

struct _zPortal : xBase
{
    zPortalAsset* passet;
};

S32 zPortalEventCB(xBase* from, xBase* to, U32 toEvent, F32* toParam, xBase* toParamWidget);
void zPortalInit(void* portal, void* passet);

#endif
