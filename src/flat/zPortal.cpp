#include "zPortal.h"
#include "zGlobals.h"
#include "zScene.h"
#include "xserializer.h"


static inline void zPortalReset(_zPortal* portal)
{
    xBaseReset(portal, portal->passet);
}

void zPortalInit(void* portal, void* passet)
{
    xBaseInit((_zPortal*)portal, (zPortalAsset*)passet);

    ((_zPortal*)portal)->passet = (zPortalAsset*)passet;
    ((_zPortal*)portal)->eventFunc = zPortalEventCB;

    if (((_zPortal*)portal)->linkCount != 0)
    {
        ((_zPortal*)portal)->link = (p2LinkAsset*)(((_zPortal*)portal)->passet + 1);
    }
}

void zPortalSave(_zPortal* ent, xSerial* s)
{
    xBaseSave(ent, s);
}

void zPortalLoad(_zPortal* ent, xSerial* s)
{
    xBaseLoad(ent, s);
}

S32 zPortalEventCB(xBase*, xBase* to, U32 toEvent, F32*, xBase*)
{
    switch (toEvent)
    {
    case 10:
        zPortalReset((_zPortal*)to);
        break;
    case 16:
        if (globals.player.Health != 0)
        {
            zSceneSwitch((_zPortal*)to);
        }
        break;
    }

    return 1;
}
