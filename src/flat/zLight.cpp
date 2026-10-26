#include "zLight.h"
#include "zVolume.h"
#include "zScene.h"
#include "xString.h"
#include "xEvent.h"

static _zLight* sLight[32];
static S32 sLightTotal;
static _zVolume* sPartitionVolume;
static S32 gNumTemporaryLights;
static _zLight* gTemporaryLights[32];

static inline void zLightEffectSet(_zLight* zlight, S32 idx)
{
    if (zlight->reg)
    {
        zlight->effect_idx = idx;
        if (sEffectInitFuncs[zlight->effect_idx] != NULL)
        {
            sEffectInitFuncs[zlight->effect_idx](zlight);
        }
    }
}

void zLightResolveLinks()
{
    S32 i;
    _zLight* zl;

    for (i = 0; i < sLightTotal; i++)
    {
        zl = sLight[i];
        if (zl->tasset->attachID)
        {
            zl->attached_to = zSceneFindObject(zl->tasset->attachID);
        }
        else
        {
            zl->attached_to = 0;
        }
    }
}

static inline void zLightDestroy(_zLight* zl)
{
    iLightDestroy(&zl->light);
}

void zLightDestroyAll()
{
    S32 total = sLightTotal;

    for (int i = 0; i < total; i++, sLightTotal--)
    {
        zLightDestroy(sLight[i]);
    }
    sLightTotal = 0;
}

static inline void zLightReset(_zLight* zl)
{
    xBaseReset(zl, zl->tasset);
    zl->flags = 0;
    if (zl->tasset->lightFlags & 0x20)
    {
        zl->flags = zl->flags | 1;
    }
    zLightEffectSet(zl, zl->tasset->lightEffect);
}

S32 zLightEventCB(xBase* from, xBase* to, U32 toEvent, F32* toParam, xBase* b)
{
    _zLight* t = (_zLight*)to;
    switch (toEvent)
    {
    case eEventOn:
    {
        t->flags |= 1;
        break;
    }
    case eEventOff:
    {
        t->flags &= 0xfffffffe;
        break;
    }
    case eEventReset:
    {
        zLightReset(t);
        break;
    }
    default:
    {
        break;
    }
    }
    return 1;
}

void zLightAddLocalEnv()
{
    for (int i = 0; i < sLightTotal; i++)
    {
        _zLight* zlight = sLight[i];
        if ((zlight->flags & 1) && (zlight->tasset->lightFlags & 8))
        {
            iLight* light = &zlight->light;
            iLightEnv(light, 1);
            light->hw->inWorld.prev = gLightWorld->directionalLightList.link.prev;
            light->hw->inWorld.next = &gLightWorld->directionalLightList.link;
            gLightWorld->directionalLightList.link.prev->next = &light->hw->inWorld;
            gLightWorld->directionalLightList.link.prev = &light->hw->inWorld;

            gTemporaryLights[gNumTemporaryLights] = zlight;
            gNumTemporaryLights++;
        }
    }
}

void zLightRemoveLocalEnv()
{
    int i;
    const RwLLLink* link;

    for (i = 0; i < gNumTemporaryLights; i++)
    {
        link = gLightWorld->directionalLightList.link.prev;
        link->prev->next = link->next;
        link->next->prev = link->prev;
    }
    gNumTemporaryLights = 0;
}

void zLightSetVolume(_zVolume* vol)
{
    if (!vol)
    {
        sPartitionVolume = 0;
    }
    else
    {
        U32 lp_id = xStrHash("LIGHT_PARTITION");
        if (vol->id == lp_id)
        {
            sPartitionVolume = vol;
        }
    }
}

void zLightOn(_zLight* zl, S32 on)
{
    if (on)
    {
        zl->flags |= 1;
    }
    else
    {
        zl->flags &= 0xfffffffe;
    }
}
