#include "zLight.h"
#include "zVolume.h"
#include "zScene.h"
#include "xString.h"
#include "xEvent.h"
#include "xEnt.h"
#include "xMemMgr.h"
#include "xPartition.h"
#include "xShadow.h"
#include "zGame.h"
#include <string.h>

static _zLight* sLight[32];
static S32 sLightTotal;
static _zVolume* sPartitionVolume;
static S32 gNumTemporaryLights;
static _zLight* gTemporaryLights[32];
static _tagPartition sLightPart;
static xVec3 sDefaultShadowVec = { 0.0f, 1.0f, 0.0f };

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

static inline void zLightDestroy(_zLight* zl)
{
    iLightDestroy(&zl->light);
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

void zLightRemoveLocal()
{
    for (S32 i = 0; i < gNumTemporaryLights; i++)
    {
        iLightRemove(&gTemporaryLights[i]->light);
    }
    gNumTemporaryLights = 0;
}

void zLightResetAll(_xEnv* env)
{
    memset(sLight, 0, sizeof(sLight));
    sLightTotal = 0;

    if (sPartitionVolume)
    {
        xPartitionVolume(&sLightPart, sPartitionVolume, 10, 1, 10);
    }
    else
    {
        xPartitionWorld(&sLightPart, env, 10, 1, 10);
    }

    xPartitionDump(&sLightPart, "Lighting");
}

void zLightInit(void* b, void* tasset)
{
    zLightInit((xBase*)b, (zLightAsset*)tasset);
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

// Adds the lights of the partition spaces around the entity, and moves its shadow towards the
// lights that cast shadows on it (or back to straight down when none does).
void zLightAddLocal(_xEnt* ent)
{
    S32 shadow_light_total = 0;
    _zLight* shadow_light[32];
    xVec3 shadow_vec[32];
    F32 shadow_dist[32];
    S32 vx;
    S32 vz;
    S32 xstart;
    S32 zstart;
    S32 xend;
    S32 zend;
    S32 zi;
    S32 xi;

    xVec3* pos = xEntGetPos(ent);
    xVec3 default_light_pos = *pos;
    default_light_pos.y += 1.0f;

    if (!ent->ent_shadow)
    {
        ent->ent_shadow = (_xEntShadow*)xMemAlloc(gActiveHeap, sizeof(_xEntShadow));
        if (ent->ent_shadow)
        {
            ent->ent_shadow->pos = default_light_pos;
            ent->ent_shadow->vec = sDefaultShadowVec;
        }
    }

    xPartitionGetVirtualIdx(&sLightPart, pos, &vx, NULL, &vz);
    xPartitionGetLoop(&sLightPart, 2, &xstart, NULL, &zstart, vx, 0, vz, &xend, NULL, &zend);

    for (zi = zstart; zi <= zend; zi++)
    {
        for (xi = xstart; xi <= xend; xi++)
        {
            _tagPartSpace* space = xPartitionGetSpace(&sLightPart, xPartitionGetTrueIdx(&sLightPart, xi, 0, zi));
            if (space->total <= 0)
            {
                continue;
            }

            _tagPartLink* link = &space->head;
            while ((link = link->next) != NULL)
            {
                _zLight* zlight = (_zLight*)link->data;
                if (!(zlight->flags & 1))
                {
                    continue;
                }

                U32 test_flags = 0;
                switch (ent->collType)
                {
                case 2:
                case 4:
                    test_flags |= 4;
                    break;
                case 8:
                    test_flags |= 1;
                    break;
                case 16:
                    test_flags |= 2;
                    break;
                default:
                    return;
                }

                if (zlight->tasset->lightFlags & test_flags)
                {
                    iLight* light = &zlight->light;
                    F32 x = light->sph.center.x - pos->x;
                    F32 y = light->sph.center.y - pos->y;
                    F32 z = light->sph.center.z - pos->z;
                    if (1.0f + (y * y + x * x + z * z) <= light->radius_sq)
                    {
                        iLightEnv(light, 2);
                        iLightAdd(light);
                        gTemporaryLights[gNumTemporaryLights] = zlight;
                        gNumTemporaryLights++;
                    }
                }

                if (zlight->tasset->lightFlags & 0x10)
                {
                    F32 x = zlight->light.sph.center.x - pos->x;
                    F32 y = zlight->light.sph.center.y - pos->y;
                    F32 z = zlight->light.sph.center.z - pos->z;
                    F32 dist = 1.0f + (y * y + x * x + z * z);
                    if (dist < zlight->light.radius_sq)
                    {
                        shadow_vec[shadow_light_total].x = x;
                        shadow_vec[shadow_light_total].y = y;
                        shadow_vec[shadow_light_total].z = z;
                        shadow_light[shadow_light_total] = zlight;
                        shadow_dist[shadow_light_total] = dist;
                        shadow_light_total++;
                    }
                }
            }
        }
    }

    if (shadow_light_total)
    {
        xVec3 single_light_vec = {};
        xVec3 single_light_pos = {};

        for (S32 i = 0; i < shadow_light_total; i++)
        {
            xVec3Normalize(&shadow_vec[i], &shadow_vec[i]);

            F32 w = 1.0f - shadow_dist[i] / shadow_light[i]->light.radius_sq;
            single_light_vec.x += w * shadow_vec[i].x;
            single_light_vec.y += w * shadow_vec[i].y;
            single_light_vec.z += w * shadow_vec[i].z;
            single_light_pos.x += shadow_light[i]->light.sph.center.x;
            single_light_pos.y += shadow_light[i]->light.sph.center.y;
            single_light_pos.z += shadow_light[i]->light.sph.center.z;
        }

        single_light_pos.x /= shadow_light_total;
        single_light_pos.y /= shadow_light_total;
        single_light_pos.z /= shadow_light_total;

        if (!zGameIsPaused() && ent->ent_shadow)
        {
            _xEntShadow* shadow = ent->ent_shadow;
            shadow->pos.x += 0.1f * (single_light_pos.x - shadow->pos.x);
            shadow->pos.y += 0.1f * (single_light_pos.y - shadow->pos.y);
            shadow->pos.z += 0.1f * (single_light_pos.z - shadow->pos.z);
            shadow = ent->ent_shadow;
            shadow->vec.x += 0.1f * (single_light_vec.x - shadow->vec.x);
            shadow->vec.y += 0.1f * (single_light_vec.y - shadow->vec.y);
            shadow->vec.z += 0.1f * (single_light_vec.z - shadow->vec.z);
        }
    }
    else if (!zGameIsPaused() && ent->ent_shadow)
    {
        _xEntShadow* shadow = ent->ent_shadow;
        shadow->pos.x += 0.01f * (default_light_pos.x - shadow->pos.x);
        shadow->pos.y += 0.01f * (default_light_pos.y - shadow->pos.y);
        shadow->pos.z += 0.01f * (default_light_pos.z - shadow->pos.z);
        shadow = ent->ent_shadow;
        shadow->vec.x += 0.01f * (sDefaultShadowVec.x - shadow->vec.x);
        shadow->vec.y += 0.01f * (sDefaultShadowVec.y - shadow->vec.y);
        shadow->vec.z += 0.01f * (sDefaultShadowVec.z - shadow->vec.z);
    }

    if (ent->ent_shadow)
    {
        xShadowSetLight(&ent->ent_shadow->pos, &ent->ent_shadow->vec, 1.0f);
    }
}

void zLightInit(xBase* b, zLightAsset* tasset)
{
    _zLight* t = (_zLight*)b;

    xBaseInit(b, tasset);
    t->eventFunc = zLightEventCB;
    t->tasset = tasset;

    if (b->linkCount)
    {
        b->link = (p2LinkAsset*)(t->tasset + 1);
    }
    else
    {
        b->link = NULL;
    }

    U32 itype = 1;
    switch (tasset->lightType)
    {
    case 0:
        itype = 1;
        break;
    case 1:
        itype = 2;
        break;
    case 2:
        itype = 1;
        break;
    case 3:
        itype = 1;
        break;
    }

    if (iLightCreate(&t->light, itype))
    {
        sLight[sLightTotal++] = t;

        t->light.sph.center.x = tasset->lightSphere.center.x;
        t->light.sph.center.y = tasset->lightSphere.center.y;
        t->light.sph.center.z = tasset->lightSphere.center.z;
        t->light.sph.r = tasset->lightSphere.r;
        t->light.color.r = tasset->lightColor[0];
        t->light.color.g = tasset->lightColor[1];
        t->light.color.b = tasset->lightColor[2];
        t->light.color.a = tasset->lightColor[3];
        t->light.coneangle = tasset->lightConeAngle;
        t->light.radius_sq = t->light.sph.r * t->light.sph.r;
        iLightModify(&t->light, 31);

        t->flags = 0;
        if (t->tasset->lightFlags & 0x20)
        {
            t->flags |= 1;
        }

        if (t->tasset->lightEffect)
        {
            t->reg = (F32*)xMemAlloc(gActiveHeap, 32);
            zLightEffectSet(t, t->tasset->lightEffect);
        }
        else
        {
            t->reg = NULL;
        }

        t->true_idx = xPartitionInsert(&sLightPart, t, &t->light.sph.center);
    }
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

void zLightUpdate(xBase* to, _xScene*, F32 dt)
{
    _zLight* t = (_zLight*)to;

    if ((t->flags & 1) && t->attached_to)
    {
        xVec3 pos = *xEntGetPos((_xEnt*)t->attached_to);
        pos.y += 1.0f;
        iLightSetPos(&t->light, &pos);
        t->light.sph.center = pos;
        t->true_idx = xPartitionUpdate(&sLightPart, t, t->true_idx, &pos);
    }

    if (sEffectFuncs[t->effect_idx])
    {
        sEffectFuncs[t->effect_idx](t, dt);
    }
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

void zLightSave(_zLight* ent, xSerial* s)
{
    xBaseSave(ent, s);
}

void zLightLoad(_zLight* ent, xSerial* s)
{
    xBaseLoad(ent, s);
}
