#include "iEnv.h"
#include "iCamera.h"
#include "xMemMgr.h"

static S32 sBeginDrawFX;
static iEnv* lastEnv;

static void iEnvSetBSP(iEnv* env, S32 envDataType, RpWorld* bsp)
{
    RwSurfaceProperties surf_props;

    if (envDataType == 0)
    {
        env->world = bsp;
    }
    else if (envDataType == 1)
    {
        env->collision = bsp;
    }
    else if (envDataType == 2)
    {
        env->fx = bsp;
    }
    else if (envDataType == 3)
    {
        env->camera = bsp;
    }

    surf_props.ambient = 1.0f;
    surf_props.diffuse = 1.0f;
    surf_props.specular = 0.0f;
    _rpWorldSetSurfaceProperties(bsp, &surf_props);
}

// The data type picks the world slot: 0 render, 1 collision, 2 effects, 3 camera.
void iEnvLoad(iEnv* env, const void* data, U32, S32 dataType)
{
    iEnvSetBSP(env, dataType, (RpWorld*)data);

    if (dataType == 0)
    {
        env->memlvl = xMemGetBase();
    }
}

void iEnvFree(iEnv* env)
{
    _rwFrameSyncDirty();

    RpWorldDestroy(env->world);
    env->world = NULL;

    if (env->fx)
    {
        RpWorldDestroy(env->fx);
        env->fx = NULL;
    }

    if (env->collision)
    {
        RpWorldDestroy(env->collision);
        env->collision = NULL;
    }
}

void iEnvDefaultLighting(iEnv* env)
{
}

void iEnvLightingBasics(iEnv* env, p2EnvAsset* asset)
{
}

void iEnvRender(iEnv* env)
{
    RwRenderStateSet(rwRENDERSTATESRCBLEND, (void*)rwBLENDSRCALPHA);
    RwRenderStateSet(rwRENDERSTATEDESTBLEND, (void*)rwBLENDINVSRCALPHA);
    RpWorldRender(env->world);
    lastEnv = env;
}

// Moves the camera from the last rendered world to its effects world, once per frame.
S32 iEnvBeginRenderFX(iEnv*)
{
    iEnv* env = lastEnv;

    if (env->fx && globalCamera && !sBeginDrawFX)
    {
        RpWorldRemoveCamera(env->world, globalCamera);
        RpWorldAddCamera(env->fx, globalCamera);

        sBeginDrawFX = 1;
    }

    return sBeginDrawFX;
}

void iEnvRenderFX(iEnv*)
{
    iEnv* env = lastEnv;

    if (env->fx && globalCamera)
    {
        RwRenderStateSet(rwRENDERSTATESRCBLEND, (void*)rwBLENDSRCALPHA);
        RwRenderStateSet(rwRENDERSTATEDESTBLEND, (void*)rwBLENDINVSRCALPHA);
        RpWorldRender(env->fx);
    }
}

void iEnvEndRenderFX(iEnv*)
{
    iEnv* env = lastEnv;

    if (env->fx && globalCamera && sBeginDrawFX)
    {
        RpWorldRemoveCamera(env->fx, globalCamera);
        RpWorldAddCamera(env->world, globalCamera);

        sBeginDrawFX = 0;
    }
}
