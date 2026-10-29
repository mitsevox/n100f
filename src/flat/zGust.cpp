#include "zGust.h"
#include "zVolume.h"
#include "zParEmitter.h"
#include "zScene.h"
#include "iMath.h"
#include "xMath.h"
#include "xBound.h"
#include "xMemMgr.h"
#include "xString.h"
#include "xEvent.h"
#include "xstransvc.h"


static _zGust* gusts;
static U16 ngusts;
static _zParEmitter* sGustDustEmitter;
static _zParEmitter* sGustDebrisEmitter;

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

static inline void zGustSetup(_zGust* g)
{
    g->volume = (_zVolume*)zSceneFindObject(g->asset->volumeID);
    g->fx_volume = (_zVolume*)zSceneFindObject(g->asset->effectID);
}

static inline void zGustInit(_zGust* g, zGustAsset* a)
{
    xBaseInit(g, a);
    g->eventFunc = zGustEventCB;
    g->asset = a;
    g->flags = a->flags;

    if (g->linkCount)
    {
        g->link = (p2LinkAsset*)(g->asset + 1);
    }
    else
    {
        g->link = NULL;
    }

    g->debris_timer = 0.15f;
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

// Blows dust (asset flag 2) or debris around the gust's volume, more for a larger floor area.
static void UpdateGustFX(_zGust* g, F32 seconds)
{
    S32 i;

    if (g->asset->partMod <= 0.0f)
    {
        return;
    }

    g->debris_timer -= seconds;
    if (!(g->debris_timer <= 0.0f))
    {
        return;
    }

    _zVolume* volume = !g->asset->effectID ? g->volume : g->fx_volume;
    zVolumeAsset* asset = volume->asset;

    S32 vol_area = (asset->bound.box.box.upper.x - asset->bound.box.box.lower.x) *
                   (asset->bound.box.box.upper.z - asset->bound.box.box.lower.z);

    if (vol_area > 1000)
    {
        g->debris_timer = 1000000.0f;
        return;
    }

    S32 total_debris = vol_area >> 5;
    if (total_debris > 5)
    {
        total_debris = 5;
    }
    else if (total_debris < 1)
    {
        total_debris = 1;
    }

    g->debris_timer = iurand() * 0.15f + 0.15f;

    _zParEmitter* e = (g->asset->flags & 2) ? sGustDustEmitter : sGustDebrisEmitter;
    if (!e)
    {
        return;
    }

    _zParEmitterCustomSettings info;
    info.custom_flags = 0x61;

    for (i = 0; i < total_debris; i++)
    {
        if (g->asset->effectID == 0)
        {
            asset = g->volume->asset;
            _xBBox* box = &asset->bound.box;

            info.pos = box->box.lower;
            info.pos.x = (box->box.upper.x - box->box.lower.x) * iurand() + info.pos.x;
            info.pos.z = (box->box.upper.z - box->box.lower.z) * iurand() + info.pos.z;
            info.vel.x = 0.0f;
            info.vel.y = 5.0f;
            info.vel.z = 0.0f;
            info.life = (box->box.upper.y - box->box.lower.y) / 5.0f * 2.0f;
        }
        else
        {
            asset = g->fx_volume->asset;
            _xBBox* box = &asset->bound.box;

            info.pos.x = (box->box.upper.x - box->box.lower.x) * iurand() + box->box.lower.x;
            info.pos.y = (box->box.upper.y - box->box.lower.y) * iurand() + box->box.lower.y;
            info.pos.z = (box->box.upper.z - box->box.lower.z) * iurand() + box->box.lower.z;
            info.vel.x = 1.5f * g->asset->vel.x;
            info.vel.y = 1.5f * g->asset->vel.y;
            info.vel.z = 1.5f * g->asset->vel.z;
            info.life = 7.5f * g->asset->fade;
        }

        info.life *= g->asset->partMod;
        zParEmitterEmitCustom(e, 1.0f / 30.0f, &info);
    }
}

S32 zGustEventCB(xBase*, xBase* to, U32 toEvent, F32*, xBase*)
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

void zGustLoad(_zGust* ent, xSerial* s)
{
    xBaseLoad(ent, s);
}

void zGustSave(_zGust* ent, xSerial* s)
{
    xBaseSave(ent, s);
}

// An entity can be in four gusts at once. lerp fades each gust in (0 to 1) and out (-1 to 0) over
// its fade time.
void zGustUpdateEnt(_xEnt* ent, _xScene*, F32 dt, void* gdata)
{
    zGustData* data = (zGustData*)gdata;

    if (!gusts)
    {
        return;
    }

    _xCollis coll;
    coll.flags = 0;

    for (U32 i = 0; i < ngusts; i++)
    {
        if (gusts[i].flags & 1)
        {
            xBoundHitsBound(&ent->bound, &gusts[i].volume->asset->bound, &coll);
            if (coll.flags & 1)
            {
                U32 j;
                U32 minidx;
                for (j = 0; j < 4; j++)
                {
                    if (data->lerp[j] < 2.0f)
                    {
                        minidx = j;
                    }
                    if (data->g[j] == &gusts[i])
                    {
                        break;
                    }
                }

                if (j == 4)
                {
                    data->g[minidx] = &gusts[i];
                    data->lerp[minidx] = 1e-7f;
                }
            }
            else
            {
                for (U32 j = 0; j < 4; j++)
                {
                    if (data->g[j] == &gusts[i] && data->lerp[j] == 1.0f)
                    {
                        data->lerp[j] = -1.0f;
                        break;
                    }
                }
            }
        }
    }

    for (U32 i = 0; i < 4; i++)
    {
        _zGust* g = data->g[i];
        if (g)
        {
            if (g->flags & 1)
            {
                F32 lerpinc = g->asset->fade;
                if (dt >= lerpinc)
                {
                    if (!(data->lerp[i] < 0.0f))
                    {
                        data->lerp[i] = 1.0f;
                        continue;
                    }
                }
                else
                {
                    lerpinc = dt / lerpinc;
                    if (data->lerp[i] >= 0.0f)
                    {
                        data->lerp[i] += lerpinc;
                        if (data->lerp[i] >= 1.0f)
                        {
                            data->lerp[i] = 1.0f;
                        }
                        continue;
                    }
                    else
                    {
                        data->lerp[i] += lerpinc;
                        if (!(data->lerp[i] >= 0.0f))
                        {
                            continue;
                        }
                    }
                }
            }

            data->g[i] = NULL;
            data->lerp[i] = 0.0f;
        }
    }

    data->gust_on = 0;

    for (U32 i = 0; i < 4; i++)
    {
        if (data->g[i])
        {
            data->gust_on = 1;

            xVec3 dpos;
            xVec3SMul(&dpos, &data->g[i]->asset->vel, dt * xabs(data->lerp[i]));
            xVec3AddTo(&ent->frame->mat.pos, &dpos);
        }
    }
}

_zGust* zGustGetGust(U16 n)
{
    if (gusts)
    {
        return &gusts[n];
    }

    return NULL;
}

void zGustSetup()
{
    if (gusts)
    {
        for (U16 i = 0; i < ngusts; i++)
        {
            zGustSetup(&gusts[i]);
        }

        sGustDustEmitter = zParEmitterFind(xStrHash("PAREMIT_GUST_DUST"));
        sGustDebrisEmitter = zParEmitterFind(xStrHash("PAREMIT_GUST_DEBRIS"));
    }
}

void zGustInit()
{
    ngusts = xSTAssetCountByType('GUST');

    if (ngusts)
    {
        gusts = (_zGust*)xMemAlloc(gActiveHeap, ngusts * sizeof(_zGust));
        for (U16 i = 0; i < ngusts; i++)
        {
            U32 size;
            zGustAsset* asset = (zGustAsset*)xSTFindAssetByType('GUST', i, &size);
            zGustInit(&gusts[i], asset);
        }
    }
    else
    {
        gusts = NULL;
    }
}
