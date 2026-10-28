#include "zParSys.h"

#include "xEvent.h"

S32 zParSysEventCB(xBase* from, xBase* to, U32 toEvent, F32* toParam, xBase* toParamWidget)
{
    zParSys* t = (zParSys*)to;

    switch (toEvent)
    {
    case eEventReset:
        xBaseReset(t, t->tasset);

        if (t->group != NULL)
        {
            xParGroupKillAllParticles(t->group);

            // parFlags: bit 0 visible, bit 1 stops aging, bit 2 stops respawning.
            t->visible = t->tasset->parFlags & 0x1;

            xParGroupSetAging(t->group, (((t->tasset->parFlags >> 1) & 0x1) ^ 0x1) & 0xFF);
            xParGroupSetVisibility(t->group, t->visible);
            xParGroupSetBack2Life(t->group, (((t->tasset->parFlags >> 2) & 0x1) ^ 0x1) & 0xFF);
        }
        break;
    case eEventVisible:
        t->visible = 1;
        if (t->group != NULL)
        {
            xParGroupSetVisibility(t->group, t->visible);
        }
        break;
    case eEventInvisible:
        t->visible = 0;
        if (t->group != NULL)
        {
            xParGroupSetVisibility(t->group, t->visible);
        }
        break;
    case eEventOn:
        if (t->group != NULL)
        {
            xParGroupSetActive(t->group, 1);
        }
        break;
    case eEventOff:
        if (t->group != NULL)
        {
            xParGroupSetActive(t->group, 0);
        }
        break;
    }

    return 1;
}

void zParSysExit(zParSys* t)
{
    if (t->group != NULL)
    {
        xParGroupKillAllParticles(t->group);
        xParGroupUnregister(t->group);
    }
}
