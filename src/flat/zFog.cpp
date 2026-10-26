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

S32 zFogEventCB(xBase* to, xBase* from, U32 toEvent, F32* toParam, xBase* b3)
{
    switch (toEvent)
    {
    case eEventOn:
        iFogParams fog;
        fog.type = rwFOGTYPELINEAR;
        zFogAsset* fromFog = ((_zFog*)from)->tasset;
        fog.start = fromFog->fogStart;
        fog.stop = fromFog->fogStop;
        fog.density = fromFog->fogDensity;
        fog.fogcolor.red = fromFog->fogColor[0];
        fog.fogcolor.green = fromFog->fogColor[1];
        fog.fogcolor.blue = fromFog->fogColor[2];
        fog.fogcolor.alpha = fromFog->fogColor[3];
        fog.bgcolor.red = fromFog->bkgndColor[0];
        fog.bgcolor.green = fromFog->bkgndColor[1];
        fog.bgcolor.blue = fromFog->bkgndColor[2];
        fog.bgcolor.alpha = fromFog->bkgndColor[3];
        fog.table = NULL;
        iCameraSetFogParams(&fog, fromFog->transitionTime);
        break;
    case eEventOff:
        iCameraSetFogParams(NULL, 0.0f);
        break;
    case eEventReset:
        zFogReset((_zFog*)from);
        break;
    }
    return 1;
}
