#include "zGust.h"
#include "xEvent.h"

static _zGust* gusts;
static U16 ngusts;

static void UpdateGustFX(_zGust* g, F32 seconds);

static inline void zGustTurnOn(_zGust* g)
{
    g->flags |= 1;
    g->debris_timer = 0.15f;
}

static inline void zGustTurnOff(_zGust* g)
{
    g->flags &= ~1;
}

static inline void zGustToggleOn(_zGust* g)
{
    g->flags ^= 1;
    g->debris_timer = 0.15f;
}

static inline void zGustReset(_zGust* g)
{
    xBaseReset(g, g->asset);
    g->flags = g->asset->flags;
}

_zGust* zGustGetGust(U16 n)
{
    if (gusts)
    {
        return &gusts[n];
    }

    return NULL;
}

S32 zGustEventCB(xBase* from, xBase* to, U32 toEvent, F32* toParam, xBase* b)
{
    switch (toEvent)
    {
    case eEventOn:
        zGustTurnOn((_zGust*)to);
        break;

    case eEventOff:
        zGustTurnOff((_zGust*)to);
        break;

    case eEventToggle:
        zGustToggleOn((_zGust*)to);
        break;

    case eEventReset:
        zGustReset((_zGust*)to);
        break;
    }

    return 1;
}

void zGustUpdateFX(F32 seconds)
{
    for (S32 i = 0; i < ngusts; i++)
    {
        _zGust* curr = &gusts[i];
        if (curr->flags & 1)
        {
            UpdateGustFX(curr, seconds);
        }
    }
}
