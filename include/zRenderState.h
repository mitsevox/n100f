#ifndef ZRENDERSTATE_H
#define ZRENDERSTATE_H

#include "types.h"

enum _SDRenderState
{
    SDRS_Unknown,
    SDRS_Default,
    SDRS_OpaqueModels,
    SDRS_AlphaModels,
    SDRS_Bubble,
    SDRS_Projectile,
    SDRS_Font,
    SDRS_HUD,
    SDRS_Particles,
    SDRS_Lightning,
    SDRS_SkidMark,
    SDRS_Tide,
    SDRS_SkyBack,
    SDRS_Total = -1
};

void zRenderStateSetDstBlendMode(S32 xmode);
void zRenderStateSetSrcBlendMode(S32 xmode);
void zRenderStateSetTexture(U32 assetID);
void zRenderState(_SDRenderState next);
void zRenderStateInit();

#endif
