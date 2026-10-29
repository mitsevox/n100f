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
    _zPortal* p = (_zPortal*)portal;

    xBaseInit(p, (zPortalAsset*)passet);

    p->passet = (zPortalAsset*)passet;
    p->eventFunc = zPortalEventCB;

    if (p->linkCount != 0)
    {
        p->link = (p2LinkAsset*)(p->passet + 1);
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
