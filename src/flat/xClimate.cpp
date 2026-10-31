#include "xClimate.h"
#include "xMath3.h"
#include "xString.h"
#include "zParEmitter.h"
#include "zGlobals.h"
#include "zScene.h"
#include "iMath.h"

#include <string.h>

static _tagClimate* sClimate;

static void UpdateRain(_tagClimate* climate, F32 seconds);

// Finds both weather emitters and switches them off until a scene asks for rain or snow.
void xClimateInit(_tagClimate* climate)
{
    climate->rain.strength = 0.0f;
    climate->rain.rain_emitter = zParEmitterFind(xStrHash("PAREMIT_RAIN"));
    climate->rain.rain_emitter->emit_flags &= 0xFE;
    climate->rain.snow_emitter = zParEmitterFind(xStrHash("PAREMIT_SNOW"));
    climate->rain.snow_emitter->emit_flags &= 0xFE;
}

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

// Spawns this frame's rain drops or snow flakes in a square around a point 10 units in front of
// the camera. Rain drops start 10 units above it. Snow flakes start higher and live longer near
// the middle of the square. In R004 five more drops fall at a fixed spot and five on the player.
static void UpdateRain(_tagClimate* climate, F32 seconds)
{
    _zParEmitterCustomSettings info;
    info.custom_flags = 0x20;

    if (climate->rain.rain)
    {
        S32 total_rain_drops = 25.0f * climate->rain.strength;
        if (globals.sceneCur->sceneID == zSceneIDFromName("R004"))
        {
            S32 i;
            for (i = 0; i < 5; i++)
            {
                info.pos.x = 99.9643f;
                info.pos.y = 32.8615f;
                info.pos.z = -177.306f;
                info.pos.x += 25.0f * iurand() - 12.5f;
                info.pos.y += 10.0f;
                info.pos.z += 25.0f * iurand() - 12.5f;
                zParEmitterEmitCustom(climate->rain.rain_emitter, seconds, &info);
            }
            for (i = 0; i < 5; i++)
            {
                info.pos = *(xVec3*)&globals.player.ent.model->Mat->pos;
                info.pos.x += 25.0f * iurand() - 12.5f;
                info.pos.y += 10.0f;
                info.pos.z += 25.0f * iurand() - 12.5f;
                zParEmitterEmitCustom(climate->rain.rain_emitter, seconds, &info);
            }
            for (i = 0; i < total_rain_drops; i++)
            {
                info.pos.x = 10.0f * globals.camera.hi_cam.mat.at.x + globals.camera.hi_cam.mat.pos.x;
                info.pos.y = 10.0f * globals.camera.hi_cam.mat.at.y + globals.camera.hi_cam.mat.pos.y;
                info.pos.z = 10.0f * globals.camera.hi_cam.mat.at.z + globals.camera.hi_cam.mat.pos.z;
                info.pos.x += 25.0f * iurand() - 12.5f;
                info.pos.y += 10.0f;
                info.pos.z += 25.0f * iurand() - 12.5f;
                zParEmitterEmitCustom(climate->rain.rain_emitter, seconds, &info);
            }
            return;
        }

        for (S32 i = 0; i < total_rain_drops; i++)
        {
            info.pos.x = 10.0f * globals.camera.hi_cam.mat.at.x + globals.camera.hi_cam.mat.pos.x;
            info.pos.y = 10.0f * globals.camera.hi_cam.mat.at.y + globals.camera.hi_cam.mat.pos.y;
            info.pos.z = 10.0f * globals.camera.hi_cam.mat.at.z + globals.camera.hi_cam.mat.pos.z;
            info.pos.x += 25.0f * iurand() - 12.5f;
            info.pos.y += 10.0f;
            info.pos.z += 25.0f * iurand() - 12.5f;
            zParEmitterEmitCustom(climate->rain.rain_emitter, seconds, &info);
        }
        return;
    }

    xVec3 fool;
    fool.x = 10.0f * globals.camera.hi_cam.mat.at.x + globals.camera.hi_cam.mat.pos.x;
    fool.y = 10.0f * globals.camera.hi_cam.mat.at.y + globals.camera.hi_cam.mat.pos.y;
    fool.z = 10.0f * globals.camera.hi_cam.mat.at.z + globals.camera.hi_cam.mat.pos.z;
    info.custom_flags |= 1;
    S32 total_snow_flakes = 25.0f * climate->rain.strength;
    for (S32 i = 0; i < total_snow_flakes; i++)
    {
        info.pos = fool;
        info.pos.x += 45.0f * iurand() - 22.5f;
        info.pos.z += 45.0f * iurand() - 22.5f;

        F32 perc = 1.0f - (info.pos.x - fool.x) * (info.pos.z - fool.z) / 506.25f;
        info.pos.y += 5.0f * perc + 5.0f;
        info.life = 2.5f * perc + 2.5f;
        zParEmitterEmitCustom(climate->rain.snow_emitter, seconds, &info);
    }
}

void xClimateUpdate(_tagClimate* climate, F32 seconds)
{
    UpdateRain(climate, seconds);
}

// The wind blows along the ground: its direction scaled by its strength, with no height.
void xClimateGetRealWindDir(_tagClimate* climate, xVec3* vec)
{
    vec->x = climate->wind.dir.x * climate->wind.strength;
    vec->y = 0.0f;
    vec->z = climate->wind.dir.z * climate->wind.strength;
}
