#include "iEnv.h"
#include "iCamera.h"
#include "xMemMgr.h"

#include <dolphin/gx.h>

// The GameCube RenderWare driver keeps its current render state in one block; bytes 0x3C and
// 0x3D are the two reference values its alpha-test modes pass to GXSetAlphaCompare.
struct RwGcnRenderStateCache
{
    U8 unk0[0x3C];
    U8 alphaRef0;
    U8 alphaRef1;
    U8 unk3E[2];
};

extern "C" RwGcnRenderStateCache lbl_8025E348;
extern "C" void RwGameCubeSetAlphaCompare(S32 mode);

static U32 sFXBlendEnabled = 1;
static U32 sFXBlendWorldEnabled = 1;
static U8 sFXBlendAlphaRef = 0xFF;
static U8 sFXBlendFlags = 2;

static S32 sBeginDrawFX;
static iEnv* lastEnv;
static U32 sFXBlendAdditive;

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

// Draws the last rendered level's effects world translucent: alpha or additive blending, depth
// tested but not written, with an optional alpha test. The depth states and the driver's alpha
// test are restored afterwards.
void iEnvRenderFXBlend(iEnv*)
{
    iEnv* env;

    if (!sFXBlendEnabled)
    {
        return;
    }

    env = lastEnv;

    if (sFXBlendWorldEnabled && env->fx && globalCamera)
    {
        if (sFXBlendAdditive)
        {
            RwRenderStateSet(rwRENDERSTATESRCBLEND, (void*)rwBLENDSRCALPHA);
            RwRenderStateSet(rwRENDERSTATEDESTBLEND, (void*)rwBLENDONE);
        }
        else
        {
            RwRenderStateSet(rwRENDERSTATESRCBLEND, (void*)rwBLENDSRCALPHA);
            RwRenderStateSet(rwRENDERSTATEDESTBLEND, (void*)rwBLENDINVSRCALPHA);
        }

        RwRenderStateSet(rwRENDERSTATEZTESTENABLE, (void*)TRUE);
        RwRenderStateSet(rwRENDERSTATEZWRITEENABLE, (void*)FALSE);

        if (sFXBlendFlags & 2)
        {
            RwGameCubeSetAlphaCompare(4);
            GXSetAlphaCompare(GX_GEQUAL, 0, GX_AOP_AND, GX_LEQUAL, sFXBlendAlphaRef);
            GXSetZCompLoc(GX_FALSE);
        }
        else
        {
            lbl_8025E348.alphaRef0 = 0;
            lbl_8025E348.alphaRef1 = sFXBlendAlphaRef;
            RwGameCubeSetAlphaCompare(3);
        }

        RpWorldRender(env->fx);

        lbl_8025E348.alphaRef0 = 0;
        lbl_8025E348.alphaRef1 = 0;
        RwGameCubeSetAlphaCompare(1);

        RwRenderStateSet(rwRENDERSTATEZTESTENABLE, (void*)TRUE);
        RwRenderStateSet(rwRENDERSTATEZWRITEENABLE, (void*)TRUE);
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
