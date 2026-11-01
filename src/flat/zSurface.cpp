#include "zSurface.h"
#include "zScene.h"
#include "xstransvc.h"
#include "xMemMgr.h"
#include "xEnt.h"
#include "zGroup.h"
#include "iMath.h"
#include <math.h>
#include <string.h>

extern "C" U32 lbl_8025E9F8;

struct zMaterialMapEntry
{
    U32 surfaceAssetID;
    U32 materialIndex;
};

struct zMaterialMapAsset
{
    U32 id;
    U32 count;
};

static zSurfaceProps* zsps;
static xSurface sDef_surf;
static zSurfaceProps sDef_surf_props;
static zSurfAsset sDef_surf_asset;

static S32 zSurfaceEventCB(xBase* from, xBase* to, U32 toEvent, F32* toParam, xBase* toParamWidget);

static S32 sMapperCount;
static zMaterialMapAsset* sMapper[1] = {};

static inline void zSurfaceInitDefaultSurface();
void zSurfaceGetName(S32 type, char* buffer);
void zSurfaceInit();
void zSurfaceRegisterMapper(U32 assetId);
void zSurfaceUpdate(xBase* to, _xScene*, F32 dt);
static S32 zSurfaceEventCB(xBase*, xBase* to, U32 toEvent, F32* toParam, xBase* toParamWidget);
void zSurfaceExit();
xSurface* zSurfaceGetSurface(U32 mat_id);
xSurface* zSurfaceGetSurface(const _xCollis* coll);
U32 zSurfaceGetSlide(const xSurface* surf);
U32 zSurfaceGetStep(const xSurface* surf);
F32 zSurfaceGetSlideStartAngle(const xSurface* surf);
F32 zSurfaceGetSlideStopAngle(const xSurface* surf);
U32 zSurfaceGetMatchOrient(const xSurface* surf);
S32 zSurfaceGetDamageType(const xSurface* surf);
void zSurfaceSetup(xSurface* s);
U32 zSurfaceGetDamagePassthrough(const xSurface* surf);
void zSurfaceLoad(xSurface* ent, xSerial* s);
void zSurfaceSave(xSurface* ent, xSerial* s);
F32 zSurfaceGetDamping(const xSurface* surf, F32 min_vel);
U32 zSurfaceGetSticky(const xSurface* surf);
S32 zSurfaceGetSlickness(const xSurface* surf);
F32 zSurfaceGetFriction(const xSurface* surf);
U32 zSurfaceGetStandOn(const xSurface* surf);

void zSurfaceInit()
{
    U32 size;
    U16 nsurfs = xSTAssetCountByType('SURF');

    xSurfaceInit(nsurfs);

    if (nsurfs)
    {
        zsps = (zSurfaceProps*)xMemAlloc(gActiveHeap, nsurfs * sizeof(zSurfaceProps));

        for (U16 i = 0; i < nsurfs; i++)
        {
            xSurface* surf = xSurfaceGetByIdx(i);
            zSurfaceProps* moprops = &zsps[i];
            surf->moprops = moprops;

            zSurfAsset* asset = (zSurfAsset*)xSTFindAssetByType('SURF', i, &size);
            moprops->asset = asset;

            xBaseInit(surf, asset);

            if (surf->linkCount)
            {
                surf->link = (p2LinkAsset*)(asset + 1);
            }
            else
            {
                surf->link = NULL;
            }

            surf->eventFunc = zSurfaceEventCB;
            surf->friction = asset->friction;
            surf->state = asset->on ? 0 : 1;
            moprops->uvfx_flags = asset->uvfx_flags;
            moprops->texanim_flags = asset->texture_anim_flags;

            for (S32 j = 0; j < 2; j++)
            {
                moprops->texanim[j].mode = asset->texture_anim[j].mode;
                moprops->texanim[j].speed = asset->texture_anim[j].speed;
                moprops->texanim[j].group = asset->texture_anim[j].group;
                moprops->texanim[j].group_idx = 0;
                moprops->uvfx[j].mode = asset->uvfx[j].mode;
                if (moprops->uvfx[j].rot <= 360.0f)
                {
                    moprops->uvfx[j].rot = 360.0f;
                }
                if (moprops->uvfx[j].rot >= 0.0f)
                {
                    moprops->uvfx[j].rot = 0.0f;
                }
                moprops->uvfx[j].rot = asset->uvfx[j].rot;
                moprops->uvfx[j].rot_spd = asset->uvfx[j].rot_spd;
                moprops->uvfx[j].trans = asset->uvfx[j].trans;
                moprops->uvfx[j].trans_spd = asset->uvfx[j].trans_spd;
                moprops->uvfx[j].scale = asset->uvfx[j].scale;
                moprops->uvfx[j].scale_spd = asset->uvfx[j].scale_spd;
            }
        }
    }
    else
    {
        zsps = NULL;
    }

    zSurfaceInitDefaultSurface();
}

void zSurfaceRegisterMapper(U32 assetId)
{
    if (sMapperCount >= 1)
    {
        return;
    }
    if (!assetId)
    {
        return;
    }

    sMapper[sMapperCount] = (zMaterialMapAsset*)xSTFindAsset(assetId, NULL);
    if (sMapper[sMapperCount])
    {
        sMapperCount++;
    }
}

void zSurfaceExit()
{
    xSurfaceExit();
    sMapperCount = 0;
}

xSurface* zSurfaceGetSurface(U32 mat_id)
{
    for (S32 map = 0; map < sMapperCount; map++)
    {
        zMaterialMapAsset* mapper = sMapper[map];
        if (mapper)
        {
            for (U16 i = 0; i < mapper->count; i++)
            {
                zMaterialMapEntry* entry = (zMaterialMapEntry*)(mapper + 1) + i;
                if (entry->materialIndex == mat_id)
                {
                    U16 nsurfs = xSurfaceGetNumSurfaces();
                    for (U16 j = 0; j < nsurfs; j++)
                    {
                        xSurface* surf = xSurfaceGetByIdx(j);
                        if (surf->id == entry->surfaceAssetID)
                        {
                            return surf;
                        }
                    }
                }
            }
        }
    }

    return &sDef_surf;
}

xSurface* zSurfaceGetSurface(const _xCollis* coll)
{
    xSurface* surf = NULL;

    if (coll->flags & 1)
    {
        if (coll->optr)
        {
            surf = ((_xEnt*)coll->optr)->surf;
        }
        else
        {
            surf = zSurfaceGetSurface(coll->oid);
        }
    }

    if (surf)
    {
        return surf;
    }

    return &sDef_surf;
}

U32 zSurfaceGetSlide(const xSurface* surf)
{
    if (surf->moprops)
    {
        return ((zSurfaceProps*)surf->moprops)->asset->phys_flags & 1;
    }
    return 0;
}

U32 zSurfaceGetStep(const xSurface* surf)
{
    if (surf->moprops)
    {
        return ((zSurfaceProps*)surf->moprops)->asset->phys_flags & 4;
    }
    return 0;
}

F32 zSurfaceGetSlideStartAngle(const xSurface* surf)
{
    if (surf->moprops)
    {
        return 3.1415927f * ((zSurfaceProps*)surf->moprops)->asset->sld_start / 180.0f;
    }
    return 3.1415927f * 20 / 180.0f;
}

F32 zSurfaceGetSlideStopAngle(const xSurface* surf)
{
    if (surf->moprops)
    {
        return 3.1415927f * ((zSurfaceProps*)surf->moprops)->asset->sld_stop / 180.0f;
    }
    return 3.1415927f * 10 / 180.0f;
}

U32 zSurfaceGetMatchOrient(const xSurface* surf)
{
    if (surf->moprops)
    {
        return ((zSurfaceProps*)surf->moprops)->asset->phys_flags & 2;
    }
    return 0;
}

S32 zSurfaceGetDamageType(const xSurface* surf)
{
    if (surf->moprops)
    {
        return ((zSurfaceProps*)surf->moprops)->asset->game_damage_type;
    }
    return 0;
}

U32 zSurfaceGetDamagePassthrough(const xSurface* surf)
{
    if (surf->moprops)
    {
        return ((zSurfaceProps*)surf->moprops)->asset->game_damage_flags & 1;
    }
    return 0;
}

U32 zSurfaceGetSticky(const xSurface* surf)
{
    if (surf->moprops)
    {
        return ((zSurfaceProps*)surf->moprops)->asset->game_sticky;
    }
    return 0;
}

U32 zSurfaceGetStandOn(const xSurface* surf)
{
    if (surf->moprops)
    {
        return !(((zSurfaceProps*)surf->moprops)->asset->phys_flags & 8);
    }
    return 1;
}

F32 zSurfaceGetFriction(const xSurface* surf)
{
    return surf->friction;
}

S32 zSurfaceGetSlickness(const xSurface* surf)
{
    return (S32)(1.0f / surf->friction);
}

F32 zSurfaceGetDamping(const xSurface* surf, F32 min_vel)
{
    return pow(min_vel, surf->friction);
}

void zSurfaceSave(xSurface* ent, xSerial* s)
{
    xSurfaceSave(ent, s);
}

void zSurfaceLoad(xSurface* ent, xSerial* s)
{
    xSurfaceLoad(ent, s);
}

void zSurfaceSetup(xSurface* s)
{
    zSurfaceProps* pp = (zSurfaceProps*)s->moprops;
    if (!pp)
    {
        return;
    }

    for (S32 i = 0; i < 2; i++)
    {
        pp->texanim[i].group_ptr = NULL;
        if (pp->texanim[i].group)
        {
            pp->texanim[i].group_ptr = zSceneFindObject(pp->texanim[i].group);
        }
    }
}

// uvfx_flags and texanim_flags: bit 0 enables the first effect, bit 1 the second.
void zSurfaceUpdate(xBase* to, _xScene*, F32 dt)
{
    xSurface* t = (xSurface*)to;
    zSurfaceProps* moprops = (zSurfaceProps*)t->moprops;
    S32 j;

    for (j = 0; j < 2; j++)
    {
        if ((j == 0 && (moprops->uvfx_flags & 1)) || (j == 1 && (moprops->uvfx_flags & 2)))
        {
            switch (moprops->uvfx[j].mode)
            {
            case 1:
                moprops->uvfx[j].trans.x = sin((1.0f / 60) * (2.0f * lbl_8025E9F8));
                moprops->uvfx[j].trans.y = sin((1.0f / 60) * (2.0f * lbl_8025E9F8));
                moprops->uvfx[j].scale.x = sin((1.0f / 60) * (2.0f * lbl_8025E9F8));
                moprops->uvfx[j].scale.y = sin((1.0f / 60) * (2.0f * lbl_8025E9F8));
                break;
            default:
            {
                xVec3 d;

                moprops->uvfx[j].rot += moprops->uvfx[j].rot_spd * dt;
                while (moprops->uvfx[j].rot >= 360.0f)
                {
                    moprops->uvfx[j].rot -= 360.0f;
                }
                while (moprops->uvfx[j].rot < 0.0f)
                {
                    moprops->uvfx[j].rot += 360.0f;
                }

                xVec3SMul(&d, &moprops->uvfx[j].trans_spd, dt);
                xVec3Add(&moprops->uvfx[j].trans, &d, &moprops->uvfx[j].trans);
                while (moprops->uvfx[j].trans.x >= 1.0f)
                {
                    moprops->uvfx[j].trans.x -= 1.0f;
                }
                while (moprops->uvfx[j].trans.x < 0.0f)
                {
                    moprops->uvfx[j].trans.x += 1.0f;
                }
                while (moprops->uvfx[j].trans.y >= 1.0f)
                {
                    moprops->uvfx[j].trans.y -= 1.0f;
                }
                while (moprops->uvfx[j].trans.y < 0.0f)
                {
                    moprops->uvfx[j].trans.y += 1.0f;
                }

                xVec3SMul(&d, &moprops->uvfx[j].scale_spd, dt);
                xVec3Add(&moprops->uvfx[j].scale, &d, &moprops->uvfx[j].scale);
                while (moprops->uvfx[j].scale.x >= 8.0f)
                {
                    moprops->uvfx[j].scale.x -= 8.0f;
                }
                while (moprops->uvfx[j].scale.x < 0.0f)
                {
                    moprops->uvfx[j].scale.x += 8.0f;
                }
                while (moprops->uvfx[j].scale.y >= 8.0f)
                {
                    moprops->uvfx[j].scale.y -= 8.0f;
                }
                while (moprops->uvfx[j].scale.y < 0.0f)
                {
                    moprops->uvfx[j].scale.y += 8.0f;
                }
                break;
            }
            }
        }
    }

    for (j = 0; j < 2; j++)
    {
        if ((j == 0 && (moprops->texanim_flags & 1)) || (j == 1 && (moprops->texanim_flags & 2)))
        {
            _zGroup* g = (_zGroup*)moprops->texanim[j].group_ptr;
            if (g)
            {
                S32 max = zGroupGetCount(g);
                if (max > 0)
                {
                    if (moprops->texanim[j].mode == 0)
                    {
                        moprops->texanim[j].frame += moprops->texanim[j].speed * dt;
                        if (moprops->texanim[j].frame >= 1.0f)
                        {
                            moprops->texanim[j].group_idx += (S32)moprops->texanim[j].frame;
                            moprops->texanim[j].frame -= (S32)moprops->texanim[j].frame;
                        }
                    }
                    else if (moprops->texanim[j].mode == 1)
                    {
                        moprops->texanim[j].frame += moprops->texanim[j].speed * dt;
                        if (moprops->texanim[j].frame >= 1.0f)
                        {
                            moprops->texanim[j].group_idx -= (S32)moprops->texanim[j].frame;
                            moprops->texanim[j].frame -= (S32)moprops->texanim[j].frame;
                        }
                    }
                    else if (moprops->texanim[j].mode == 2)
                    {
                        moprops->texanim[j].frame += moprops->texanim[j].speed * dt;
                        if (moprops->texanim[j].frame >= 1.0f)
                        {
                            moprops->texanim[j].group_idx = irand() % max;
                            moprops->texanim[j].frame -= (S32)moprops->texanim[j].frame;
                        }
                    }

                    while (moprops->texanim[j].group_idx >= max)
                    {
                        moprops->texanim[j].group_idx -= max;
                    }
                    while (moprops->texanim[j].group_idx < 0)
                    {
                        moprops->texanim[j].group_idx += max;
                    }
                }
            }
        }
    }
}

// toParam[3] selects the first (0) or second texture animation.
static S32 zSurfaceEventCB(xBase*, xBase* to, U32 toEvent, F32* toParam, xBase* toParamWidget)
{
    xSurface* t = (xSurface*)to;

    switch (toEvent)
    {
    case 38:
        t->state = 0;
        break;
    case 39:
        t->state = 1;
        break;
    case 15:
        if (t->state == 0)
        {
            t->state = 1;
        }
        else
        {
            t->state = 0;
        }
        break;
    case 10:
        xSurfaceReset(t);
        t->friction = ((zSurfaceProps*)t->moprops)->asset->friction;
        t->state = 0;
        break;
    case 241:
    {
        zSurfaceProps* p = (zSurfaceProps*)t->moprops;
        if (p)
        {
            if (toParam[3] == 0.0f)
            {
                p->texanim_flags |= 1;
            }
            else
            {
                p->texanim_flags |= 2;
            }
        }
        break;
    }
    case 249:
    {
        zSurfaceProps* p = (zSurfaceProps*)t->moprops;
        if (p && (p->texanim_flags & 1))
        {
            if (toParam[3] == 0.0f)
            {
                p->texanim[0].frame += toParam[0];
            }
            else
            {
                p->texanim[1].frame += toParam[0];
            }
        }
        break;
    }
    case 242:
    {
        zSurfaceProps* p = (zSurfaceProps*)t->moprops;
        if (p)
        {
            if (toParam[3] == 0.0f)
            {
                if (p->texanim_flags & 1)
                {
                    p->texanim_flags ^= 1;
                }
            }
            else
            {
                if (p->texanim_flags & 2)
                {
                    p->texanim_flags ^= 2;
                }
            }
        }
        break;
    }
    case 243:
    {
        zSurfaceProps* p = (zSurfaceProps*)t->moprops;
        if (p)
        {
            if (toParam[3] == 0.0f)
            {
                if (p->texanim_flags & 1)
                {
                    if (p->texanim_flags & 1)
                    {
                        p->texanim_flags ^= 1;
                    }
                }
                else
                {
                    p->texanim_flags |= 1;
                }
            }
            else
            {
                if (p->texanim_flags & 2)
                {
                    if (p->texanim_flags & 2)
                    {
                        p->texanim_flags ^= 2;
                    }
                }
                else
                {
                    p->texanim_flags |= 2;
                }
            }
        }
        break;
    }
    case 247:
    {
        zSurfaceProps* p = (zSurfaceProps*)t->moprops;
        if (p)
        {
            if (toParamWidget)
            {
                S32 idx = (toParam[3] == 0.0f) ? 0 : 1;
                p->texanim[idx].group = toParamWidget->id;
                p->texanim[idx].group_ptr = toParamWidget;
            }
            else
            {
                if (toParam[3] == 0.0f)
                {
                    p->texanim[0].group = 0;
                    p->texanim[0].group_ptr = NULL;
                }
                else
                {
                    p->texanim[1].group = 0;
                    p->texanim[1].group_ptr = NULL;
                }
            }
        }
        break;
    }
    case 248:
    {
        zSurfaceProps* p = (zSurfaceProps*)t->moprops;
        if (p)
        {
            if (toParam[3] == 0.0f)
            {
                p->texanim[0].speed = toParam[0];
            }
            else
            {
                p->texanim[1].speed = toParam[0];
            }
        }
        break;
    }
    }

    return 1;
}

static inline void zSurfaceInitDefaultSurface()
{
    sDef_surf.friction = 1.0f;
    sDef_surf.state = 0;
    sDef_surf.moprops = &sDef_surf_props;
    sDef_surf_props.asset = &sDef_surf_asset;

    sDef_surf_asset.game_damage_type = 0;
    sDef_surf_asset.game_sticky = 0;
    sDef_surf_asset.game_damage_flags = 0;
    sDef_surf_asset.sld_start = 20;
    sDef_surf_asset.sld_stop = 10;
    sDef_surf_asset.phys_flags = 0;
    sDef_surf_asset.friction = 1.0f;
}

void zSurfaceGetName(S32 type, char* buffer)
{
    *buffer = '\0';

    switch (type)
    {
    case 0:
        strcpy(buffer, "DEFAULT");
        break;
    case 1:
        strcpy(buffer, "TILE");
        break;
    case 2:
        strcpy(buffer, "CARPET");
        break;
    case 3:
        strcpy(buffer, "SHORTGRASS");
        break;
    case 4:
        strcpy(buffer, "LONGGRASS");
        break;
    case 5:
        strcpy(buffer, "GRAVEL");
        break;
    case 6:
        strcpy(buffer, "DIRT");
        break;
    case 7:
        strcpy(buffer, "MUD");
        break;
    case 8:
        strcpy(buffer, "THORNS");
        break;
    case 9:
        strcpy(buffer, "METAL");
        break;
    case 10:
        strcpy(buffer, "SHEETMETAL");
        break;
    case 11:
        strcpy(buffer, "CONCRETE");
        break;
    case 12:
        strcpy(buffer, "MARBLE");
        break;
    case 13:
        strcpy(buffer, "STONE");
        break;
    case 14:
        strcpy(buffer, "WOOD");
        break;
    case 15:
        strcpy(buffer, "SNOW");
        break;
    case 16:
        strcpy(buffer, "ICE");
        break;
    case 17:
        strcpy(buffer, "SHALLOWWATER");
        break;
    case 18:
        strcpy(buffer, "DEEPWATER");
        break;
    case 19:
        strcpy(buffer, "TAR");
        break;
    case 20:
        strcpy(buffer, "SPECIAL1");
        break;
    case 21:
        strcpy(buffer, "DeepAcid");
        break;
    case 22:
        strcpy(buffer, "NONE");
        break;
    }
}
