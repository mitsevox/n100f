#ifndef ZSURFACE_H
#define ZSURFACE_H

#include "types.h"
#include "xBase.h"
#include "xSurface.h"
#include "xVec3.h"

struct _xCollis;

struct zSurfMatFX
{
    U32 flags;
    U32 bumpmapID;
    U32 envmapID;
    F32 shininess;
    F32 bumpiness;
    U32 dualmapID;
};

struct zSurfColorFX
{
    U16 flags;
    U16 mode;
    F32 speed;
};

struct zSurfTextureAnim
{
    U16 pad;
    U16 mode;
    U32 group;
    F32 speed;
};

struct zSurfUVFX
{
    S32 mode;
    F32 rot;
    F32 rot_spd;
    xVec3 trans;
    xVec3 trans_spd;
    xVec3 scale;
    xVec3 scale_spd;
};

struct zSurfAsset : p2BaseAsset
{
    U8 game_damage_type;
    U8 game_sticky;
    U8 game_damage_flags;
    U8 surf_type;
    U8 phys_pad;
    U8 sld_start;
    U8 sld_stop;
    U8 phys_flags;
    F32 friction;
    zSurfMatFX matfx;
    zSurfColorFX colorfx;
    U32 texture_anim_flags;
    zSurfTextureAnim texture_anim[2];
    U32 uvfx_flags;
    zSurfUVFX uvfx[2];
    U8 on;
    U8 surf_pad[3];
};

struct zSurfacePropTexAnim
{
    U16 mode;
    F32 speed;
    F32 frame;
    U32 group;
    S32 group_idx;
    xBase* group_ptr;
};

struct zSurfacePropUVFX
{
    S32 mode;
    F32 rot;
    F32 rot_spd;
    xVec3 trans;
    xVec3 trans_spd;
    xVec3 scale;
    xVec3 scale_spd;
};

struct zSurfaceProps
{
    zSurfAsset* asset;
    U32 texanim_flags;
    zSurfacePropTexAnim texanim[2];
    U32 uvfx_flags;
    zSurfacePropUVFX uvfx[2];
};

void zSurfaceRegisterMapper(U32 assetId);
void zSurfaceExit();
xSurface* zSurfaceGetSurface(const _xCollis* coll);
U32 zSurfaceGetSlide(const xSurface* surf);
U32 zSurfaceGetStep(const xSurface* surf);
F32 zSurfaceGetSlideStartAngle(const xSurface* surf);
F32 zSurfaceGetSlideStopAngle(const xSurface* surf);
U32 zSurfaceGetMatchOrient(const xSurface* surf);
S32 zSurfaceGetDamageType(const xSurface* surf);
U32 zSurfaceGetDamagePassthrough(const xSurface* surf);
U32 zSurfaceGetSticky(const xSurface* surf);
U32 zSurfaceGetStandOn(const xSurface* surf);
S32 zSurfaceGetSlickness(const xSurface* surf);
void zSurfaceSetup(xSurface* s);
void zSurfaceGetName(S32 type, char* buffer);

#endif
