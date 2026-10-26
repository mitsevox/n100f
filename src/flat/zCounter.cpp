#include "zCounter.h"

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
