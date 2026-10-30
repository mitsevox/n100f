#ifndef XCLIMATE_H
#define XCLIMATE_H

#include "types.h"
#include "xVec3.h"
#include "xBase.h"

struct _zParEmitter;

struct _tagRain
{
    S32 rain;
    F32 strength;
    _zParEmitter* rain_emitter;
    _zParEmitter* snow_emitter;
};

struct _tagWind
{
    F32 strength;
    F32 angle;
    xVec3 dir;
};

struct _tagClimate
{
    _tagRain rain;
    _tagWind wind;
};

struct p2EnvAsset : p2BaseAsset
{
    U32 bspAssetID;
    U32 startCameraAssetID;
    U32 climateFlags;
    F32 climateStrengthMin;
    F32 climateStrengthMax;
    F32 padF1;
    F32 padF2;
    F32 padF3;
    U32 bspCollisionAssetID;
    U32 bspFXAssetID;
    U32 bspCameraAssetID;
    U32 bspMapperID;
    U32 bspMapperCollisionID;
    U32 bspMapperFXID;
};

void xClimateInit(_tagClimate* climate);
void xClimateInitAsset(_tagClimate* climate, p2EnvAsset* easset);
void xClimateUpdate(_tagClimate* climate, F32 seconds);
void xClimateGetRealWindDir(_tagClimate* climate, xVec3* vec);
void xClimateSetSnow(F32 stre);
void xClimateSetRain(F32 stre);

#endif
