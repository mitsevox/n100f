#include "zFog.h"
#include "xEvent.h"
#include "iCamera.h"

void zFogInit(void* b, void* tasset)
{
    xBaseInit((_zFog*)b, (zFogAsset*)tasset);
    ((_zFog*)b)->eventFunc = zFogEventCB;
    ((_zFog*)b)->tasset = (zFogAsset*)tasset;
    if (((_zFog*)b)->linkCount)
    {
        ((_zFog*)b)->link = (p2LinkAsset*)(((_zFog*)b)->tasset + 1);
    }
    else
    {
        ((_zFog*)b)->link = NULL;
    }
}

void zFogSave(_zFog* ent, xSerial* s)
{
    xBaseSave(ent, s);
}

void zFogLoad(_zFog* ent, xSerial* s)
{
    xBaseLoad(ent, s);
}

S32 zFogEventCB(xBase*, xBase* to, U32 toEvent, F32*, xBase*)
{
    _zFog* t = (_zFog*)to;

    switch (toEvent)
    {
    case eEventOn:
        iFogParams fog;
        fog.type = rwFOGTYPELINEAR;
        fog.start = t->tasset->fogStart;
        fog.stop = t->tasset->fogStop;
        fog.density = t->tasset->fogDensity;
        fog.fogcolor.red = t->tasset->fogColor[0];
        fog.fogcolor.green = t->tasset->fogColor[1];
        fog.fogcolor.blue = t->tasset->fogColor[2];
        fog.fogcolor.alpha = t->tasset->fogColor[3];
        fog.bgcolor.red = t->tasset->bkgndColor[0];
        fog.bgcolor.green = t->tasset->bkgndColor[1];
        fog.bgcolor.blue = t->tasset->bkgndColor[2];
        fog.bgcolor.alpha = t->tasset->bkgndColor[3];
        fog.table = NULL;
        iCameraSetFogParams(&fog, t->tasset->transitionTime);
        break;
    case eEventOff:
        iCameraSetFogParams(NULL, 0.0f);
        break;
    case eEventReset:
        xBaseReset(t, t->tasset);
        break;
    }
    return 1;
}

void zFogUpdate(xBase*, _xScene*, F32)
{
}
