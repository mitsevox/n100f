#include "zSurface.h"
#include "zScene.h"
#include "xstransvc.h"

struct zMaterialMapAsset;

static S32 sMapperCount;
static zMaterialMapAsset* sMapper[1] = {};

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

S32 zSurfaceGetSlickness(const xSurface* surf)
{
    return (S32)(1.0f / surf->friction);
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
