#include "xClimate.h"
#include "xMath3.h"

#include <string.h>

static _tagClimate* sClimate;

static inline void xClimateVecFromAngle(F32 angleDegrees, xVec3* vec)
{
    xMat3x3 Mat;

    memcpy(&Mat, &g_I3, sizeof(xMat3x3));
    xMat3x3Euler(&Mat, (3.1415927f * angleDegrees) / 180.0f, 0.0f, 0.0f);
    xVec3Init(vec, 0.0f, 0.0f, 1.0f);
    xMat3x3LMulVec(vec, &Mat, vec);
}

void xClimateInitAsset(_tagClimate* climate, p2EnvAsset* easset)
{
    sClimate = climate;
    climate->wind.strength = 0.0f;
    xClimateVecFromAngle(climate->wind.angle, &climate->wind.dir);

    if (easset->climateFlags == 0)
    {
        climate->wind.strength = 0.0f;
        climate->rain.strength = 0.0f;
        return;
    }
    if (easset->climateFlags & 1)
    {
        climate->rain.rain = 1;
        climate->rain.strength = 0.5f * (easset->climateStrengthMax - easset->climateStrengthMin);
        climate->rain.strength += easset->climateStrengthMin;
    }
    else if (easset->climateFlags & 2)
    {
        climate->rain.rain = 0;
        climate->rain.strength = 0.5f * (easset->climateStrengthMax - easset->climateStrengthMin);
        climate->rain.strength += easset->climateStrengthMin;
    }
}

void xClimateSetSnow(F32 stre)
{
    sClimate->rain.rain = 0;
    sClimate->rain.strength = stre;
}

void xClimateSetRain(F32 stre)
{
    sClimate->rain.rain = 1;
    sClimate->rain.strength = stre;
}
