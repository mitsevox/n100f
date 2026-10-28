#include "zHologram.h"
#include "xEnt.h"
#include "xMemMgr.h"

struct _HoloBurst
{
    F32 r;
    F32 g;
    F32 b;
    F32 total_time;
};

struct _tagHologram
{
    U32 flags;
    _xEnt* ent;
    F32 src_r;
    F32 src_g;
    F32 src_b;
    F32 time_passed;
    S32 cur_burst;
    S32 alpha_max;
};

static const _HoloBurst cHologramColBurst[23] = {
    { 1.0f, 1.0f, 1.0f, 0.0f },
    { -1.0f, -1.0f, 5.0f, 0.01f },
    { 1.0f, 1.0f, 1.0f, 0.0f },
    { -1.0f, -1.0f, 5.0f, 0.01f },
    { 1.0f, 1.0f, 1.0f, 0.0f },
    { -1.0f, -1.0f, 5.0f, 0.01f },
    { 1.0f, 1.0f, 1.0f, 0.0f },
    { -1.0f, -1.0f, 5.0f, 0.01f },
    { 1.0f, 1.0f, 1.0f, 0.0f },
    { -1.0f, -1.0f, 5.0f, 0.01f },
    { 1.0f, 1.0f, 1.0f, 0.0f },
    { -1.0f, -1.0f, 5.0f, 0.01f },
    { 1.0f, 1.0f, 1.0f, 0.0f },
    { -1.0f, -1.0f, 5.0f, 0.01f },
    { 1.0f, 1.0f, 1.0f, 0.0f },
    { -1.0f, -1.0f, 5.0f, 0.01f },
    { 1.0f, 1.0f, 1.0f, 0.0f },
    { -1.0f, -1.0f, 5.0f, 0.03f },
    { 1.0f, 1.0f, 1.0f, 0.33f },
    { -1.0f, -1.0f, 5.0f, 0.01f },
    { 1.0f, 1.0f, 1.0f, 0.33f },
    { -1.0f, -1.0f, 5.0f, 0.01f },
    { 1.0f, 1.0f, 1.0f, 0.33f },
};

static _tagHologram* sHologram[16];

void zHologramInit()
{
    S32 i;

    for (i = 0; i < 16; i++)
    {
        sHologram[i] = NULL;
    }
}

static _tagHologram* GetFreeHologram()
{
    S32 i;

    for (i = 0; i < 16; i++)
    {
        if (sHologram[i])
        {
            if (!(sHologram[i]->flags & 1))
            {
                return sHologram[i];
            }
        }
        else
        {
            sHologram[i] = (_tagHologram*)xMemAlloc(gActiveHeap, sizeof(_tagHologram));
            return sHologram[i];
        }
    }

    return NULL;
}

void zHologramAdd(_xEnt* ent)
{
    _tagHologram* h = GetFreeHologram();

    if (h)
    {
        h->flags |= 1;
        h->ent = ent;
        h->src_r = cHologramColBurst[0].r;
        h->src_g = cHologramColBurst[0].g;
        h->src_b = cHologramColBurst[0].b;
        h->time_passed = 0.0f;
        h->alpha_max = 1;
        h->cur_burst = 1;
    }
}

static void UpdateHologram(_tagHologram* h, F32 seconds)
{
    F32 rset;
    F32 gset;
    F32 bset;
    const _HoloBurst* burst;

    if (h->cur_burst >= 23)
    {
        h->flags &= ~1;
        h->alpha_max = 1;
        return;
    }

    burst = &cHologramColBurst[h->cur_burst];

    h->time_passed += seconds;

    if (h->time_passed >= burst->total_time)
    {
        rset = h->src_r = burst->r;
        gset = h->src_g = burst->g;
        bset = h->src_b = burst->b;
        h->time_passed = 0.0f;
        h->cur_burst++;
    }
    else
    {
        rset = h->time_passed * ((burst->r - h->src_r) / burst->total_time);
        gset = h->time_passed * ((burst->g - h->src_g) / burst->total_time);
        bset = h->time_passed * ((burst->b - h->src_b) / burst->total_time);
        rset += h->src_r;
        gset += h->src_g;
        bset += h->src_b;
    }

    h->alpha_max = !h->alpha_max;

    xEntSetMaterialMul(h->ent, rset, gset, bset);
}

void zHologramUpdate(F32 seconds)
{
    S32 i;

    for (i = 0; i < 16; i++)
    {
        if (sHologram[i] && (sHologram[i]->flags & 1))
        {
            UpdateHologram(sHologram[i], seconds);
        }
    }
}
