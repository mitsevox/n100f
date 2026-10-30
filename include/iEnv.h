#ifndef IENV_H
#define IENV_H

#include "types.h"

#include <rpworld.h>
#include <rwcore.h>

struct iEnv
{
    RpWorld* world;
    RpWorld* collision;
    RpWorld* fx;
    RpWorld* camera;
    RpLight* light[2];
    RwFrame* light_frame[2];
    S32 memlvl;
};

struct p2EnvAsset;

void iEnvLoad(iEnv* env, const void* data, U32, S32 dataType);
void iEnvFree(iEnv* env);
void iEnvDefaultLighting(iEnv* env);
void iEnvLightingBasics(iEnv* env, p2EnvAsset* asset);
void iEnvRender(iEnv* env);
S32 iEnvBeginRenderFX(iEnv* env);
void iEnvRenderFX(iEnv* env);
void iEnvEndRenderFX(iEnv* env);

#endif
