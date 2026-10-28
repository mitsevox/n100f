#include "zPortal.h"

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
