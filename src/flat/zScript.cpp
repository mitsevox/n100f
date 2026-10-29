#include "zScript.h"
#include "zEnt.h"
#include "zScene.h"
#include "zGlobals.h"
#include "xPad.h"

// state: 0 stopped, 1 running, 3 waiting for the start button.
static inline void zScriptExecuteEvents(_zScript* t, F32 start, F32 end)
{
    t->more = 0;

    zScriptEventAsset* a = (zScriptEventAsset*)(t->tasset + 1);
    for (S32 i = 0; i < (S32)t->tasset->eventCount; a++, i++)
    {
        if (a->widget != 0 && a->time >= start)
        {
            if (a->time < end)
            {
                xBase* b = zSceneFindObject(a->widget);
                if (b)
                {
                    zEntEvent(t, 0, b, a->paramEvent, a->param,
                              !a->paramWidget ? NULL : zSceneFindObject(a->paramWidget), 0);
                }
            }
            else
            {
                t->more = 1;
            }
        }
    }
}

void zScriptInit(void* data, void* asset)
{
    _zScript* script = (_zScript*)data;
    zScriptAsset* tasset = (zScriptAsset*)asset;

    xBaseInit(script, tasset);
    script->eventFunc = &zScriptEventCB;
    script->tasset = tasset;

    // The asset's timed events come first; the links follow them.
    if (script->linkCount != 0)
    {
        script->link = (p2LinkAsset*)(script->tasset + 1);
        script->link += tasset->eventCount;
    }
    else
    {
        script->link = NULL;
    }

    script->time = script->tasset->scriptStartTime;
    script->more = 1;
}

void zScriptSave(_zScript* ent, xSerial* s)
{
    xBaseSave(ent, s);
}

void zScriptLoad(_zScript* ent, xSerial* s)
{
    xBaseLoad(ent, s);
}

S32 zScriptEventCB(xBase*, xBase* to, U32 toEvent, F32*, xBase*)
{
    _zScript* t = (_zScript*)to;

    switch (toEvent)
    {
    case 18:
        t->state = 1;
        break;
    case 19:
        if (t->state == 1 || t->state == 3)
        {
            t->state = 0;
        }
        break;
    case 20:
        t->state = 0;
        break;
    case 292:
        t->state = 0;
        t->time = t->tasset->scriptStartTime;
        break;
    case 293:
        if (t->state == 1)
        {
            t->state = 3;
        }
        break;
    case 10:
        xBaseReset(t, t->tasset);
        t->time = t->tasset->scriptStartTime;
        t->more = 1;
        break;
    }

    return 1;
}

void zScriptUpdate(xBase* to, _xScene*, F32 dt)
{
    _zScript* t = (_zScript*)to;

    if (t->state == 1)
    {
        zScriptExecuteEvents(t, t->time, t->time + dt);
        t->time += dt;
        if (!t->more && t->state == 3)
        {
            zEntEvent(t, 20);
        }
    }
    else if (t->state == 3)
    {
        if (globals.pad0 && (globals.pad0->on & 0x40000))
        {
            t->state = 1;
        }
    }
}
