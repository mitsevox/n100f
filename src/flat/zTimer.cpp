#include "zTimer.h"
#include "xEvent.h"
#include "zEnt.h"

static inline void zTimerInit(xBase* b, zTimerAsset* tasset);

void zTimerInit(void* b, void* tasset)
{
    zTimerInit((xBase*)b, (zTimerAsset*)tasset);
}

static inline void zTimerInit(xBase* b, zTimerAsset* tasset)
{
    xBaseInit(b, tasset);

    _zTimer* t = (_zTimer*)b;

    t->eventFunc = zTimerEventCB;
    t->tasset = tasset;

    if (t->linkCount)
    {
        t->link = (p2LinkAsset*)(t->tasset + 1);
    }
    else
    {
        t->link = NULL;
    }

    t->state = 0;
    t->secondsLeft = tasset->seconds;
}

void zTimerSave(_zTimer* ent, xSerial* s)
{
    xBaseSave(ent, s);

    s->Write(ent->state);
    s->Write(ent->secondsLeft);
}

void zTimerLoad(_zTimer* ent, xSerial* s)
{
    xBaseLoad(ent, s);

    s->Read(&ent->state);
    s->Read(&ent->secondsLeft);
}

// state 1 is running; the timer sends eEventExpired to itself when it runs out.
S32 zTimerEventCB(xBase*, xBase* to, U32 toEvent, F32*, xBase*)
{
    _zTimer* t = (_zTimer*)to;

    switch (toEvent)
    {
    case eEventRun:
        t->state = 1;
        break;
    case eEventStop:
        if (t->state != 1)
        {
            t->state = 0;
        }
        break;
    case eEventReset:
        xBaseReset(t, t->tasset);
        t->state = 0;
        t->secondsLeft = t->tasset->seconds;
        break;
    case eEventExpired:
        t->state = 0;
        break;
    }

    return 1;
}

void zTimerUpdate(xBase* to, _xScene*, F32 dt)
{
    _zTimer* t = (_zTimer*)to;

    if (t->state == 1)
    {
        t->secondsLeft -= dt;
        if (t->secondsLeft <= 0.0f)
        {
            zEntEvent(t, t, eEventExpired);
        }
    }
}
