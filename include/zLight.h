#ifndef ZLIGHT_H
#define ZLIGHT_H

#include "xBase.h"
#include "xMath3.h"
#include "iLight.h"

struct _zVolume;
struct _xEnt;

struct zLightAsset : p2BaseAsset
{
    U8 lightType;
    U8 lightEffect;
    U8 lightPad[2];
    U32 lightFlags;
    F32 lightColor[4];
    xVec3 lightDir;
    F32 lightConeAngle;
    xSphere lightSphere;
    U32 attachID;
};

struct _zLight : xBase
{
    U32 flags;
    zLightAsset* tasset;
    iLight light;
    xBase* attached_to;
    S32 true_idx;
    F32* reg;
    S32 effect_idx;
};

extern void (*sEffectInitFuncs[18])(_zLight*);

void zLightOn(_zLight* zl, S32 on);
void zLightSetVolume(_zVolume* vol);
void zLightRemoveLocalEnv();
void zLightAddLocalEnv();
void zLightAddLocal(_xEnt* ent);
void zLightRemoveLocal();
S32 zLightEventCB(xBase* from, xBase* to, U32 toEvent, F32* toParam, xBase* b);
void zLightDestroyAll();
void zLightResolveLinks();

#endif
