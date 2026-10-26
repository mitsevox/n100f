#include "zFog.h"
#include "xEvent.h"
#include "iCamera.h"

void zFogInit(void* b, void* tasset)
{
    xBase* ent = (xBase*)b;

    xBaseInit(ent, (p2BaseAsset*)tasset);
    ent->eventFunc = zFogEventCB;
    ((_zFog*)ent)->tasset = (zFogAsset*)tasset;
    if (ent->linkCount != 0)
    {
        ent->link = (p2LinkAsset*)(((_zFog*)ent)->tasset + 1);
    }
    else
    {
        ent->link = NULL;
    }
}

static inline void zFogReset(_zFog* ent)
{
    xBaseReset((xBase*)ent, (p2BaseAsset*)ent->tasset);
}

S32 zFogEventCB(xBase* from, xBase* to, U32 toEvent, F32* toParam, xBase* b3)
{
    switch (toEvent)
    {
    case eEventOn:
        iFogParams fog;
        fog.type = rwFOGTYPELINEAR;
        zFogAsset* asset = ((_zFog*)to)->tasset;
        fog.start = asset->fogStart;
        fog.stop = asset->fogStop;
        fog.density = asset->fogDensity;
        fog.fogcolor.red = asset->fogColor[0];
        fog.fogcolor.green = asset->fogColor[1];
        fog.fogcolor.blue = asset->fogColor[2];
        fog.fogcolor.alpha = asset->fogColor[3];
        fog.bgcolor.red = asset->bkgndColor[0];
        fog.bgcolor.green = asset->bkgndColor[1];
        fog.bgcolor.blue = asset->bkgndColor[2];
        fog.bgcolor.alpha = asset->bkgndColor[3];
        fog.table = NULL;
        iCameraSetFogParams(&fog, asset->transitionTime);
        break;
    case eEventOff:
        iCameraSetFogParams(NULL, 0.0f);
        break;
    case eEventReset:
        zFogReset((_zFog*)to);
        break;
    }
    return 1;
}
