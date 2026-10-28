#include "zCounter.h"
#include "xEvent.h"
#include "zEnt.h"

void zCounterInit(void* b, void* asset)
{
    xBaseInit((_zCounter*)b, (zCounterAsset*)asset);
    ((_zCounter*)b)->eventFunc = zCounterEventCB;
    ((_zCounter*)b)->asset = (zCounterAsset*)asset;
    if (((_zCounter*)b)->linkCount)
    {
        ((_zCounter*)b)->link = (p2LinkAsset*)(((_zCounter*)b)->asset + 1);
    }
    else
    {
        ((_zCounter*)b)->link = 0;
    }
    ((_zCounter*)b)->state = 0;
    ((_zCounter*)b)->count = ((zCounterAsset*)asset)->count;
}

void zCounterReset(xBase* b)
{
    _zCounter* t = (_zCounter*)b;

    xBaseInit(t, t->asset);

    if (t->linkCount)
    {
        t->link = (p2LinkAsset*)(t->asset + 1);
    }
    else
    {
        t->link = 0;
    }

    t->count = t->asset->count;
    t->state = 0;
}

void zCounterSave(_zCounter* ent, xSerial* s)
{
    xBaseSave(ent, s);

    s->Write(ent->state);
    s->Write(ent->count);
}

void zCounterLoad(_zCounter* ent, xSerial* s)
{
    xBaseLoad(ent, s);

    s->Read(&ent->state);
    s->Read(&ent->count);
}

// state 1 locks the count; reaching zero sends eEventExpired to the counter itself.
S32 zCounterEventCB(xBase*, xBase* to, U32 toEvent, F32*, xBase*)
{
    _zCounter* t = (_zCounter*)to;

    switch (toEvent)
    {
    case eEventDecrement:
        if (t->state != 1)
        {
            t->count--;
            if (t->count == 0)
            {
                zEntEvent(t, t, eEventExpired);
            }
        }
        break;
    case eEventIncrement:
        if (t->state != 1)
        {
            t->count++;
            if (t->count == 0)
            {
                zEntEvent(t, t, eEventExpired);
            }
        }
        break;
    case eEventReset:
        t->state = 0;
        t->count = t->asset->count;
        break;
    case eEventExpired:
        t->count = 0;
        break;
    }

    return 1;
}
