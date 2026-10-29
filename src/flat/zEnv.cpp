#include "zEnv.h"

#include "iEnv.h"
#include "iFX.h"
#include "iLight.h"
#include "xEnv.h"
#include "xSurface.h"
#include "xstransvc.h"
#include "zEntPlayer.h"
#include "zGlobals.h"
#include "zGroup.h"
#include "zLight.h"
#include "zScene.h"
#include "zSurface.h"

#include <rpworld.h>

xSurface* zSurfaceGetSurface(U32 mat_id);
void xEnvLoadBsp(_xEnv* env, const void* data, U32 datasize, S32 dataType);
void zLightResetAll(_xEnv* env);
void zEntPlayer_StoreCheckPoint(F32* pos);
void xCameraPickInitial(_tagxCamFrame* cam, _tagxCam* c);
void iEnvLightingBasics(iEnv* env, p2EnvAsset* easset);
RpMaterial* RpMaterialSetTexture(RpMaterial* material, RwTexture* texture);

// Set up the scene environment: load the world and its collision, effect and camera worlds,
// register the surface mapper and light the world.
void zEnvInit(void* env, void* easset)
{
    zEnvInit((_zEnv*)env, (p2EnvAsset*)easset);
}

// Set up the environment base and load its world, collision and effect BSPs.
void zEnvInit(_zEnv* env, p2EnvAsset* easset)
{
    U32 bufsize;
    void* buf;

    xBaseInit(env, easset);

    env->easset = easset;
    env->eventFunc = zEnvEventCB;

    if (env->linkCount != 0)
    {
        env->link = (p2LinkAsset*)(env->easset + 1);
    }

    xSTAssetCountByType('BSP ');

    buf = xSTFindAsset(easset->bspAssetID, &bufsize);
    if (buf != NULL)
    {
        xEnvLoadBsp(globals.sceneCur->env, buf, bufsize, 0);
    }

    zSurfaceRegisterMapper(easset->bspMapperID);

    if (easset->bspCollisionAssetID != 0)
    {
        buf = xSTFindAsset(easset->bspCollisionAssetID, &bufsize);
        if (buf != NULL)
        {
            xEnvLoadBsp(globals.sceneCur->env, buf, bufsize, 1);
        }
    }

    if (easset->bspFXAssetID != 0)
    {
        buf = xSTFindAsset(easset->bspFXAssetID, &bufsize);
        if (buf != NULL)
        {
            xEnvLoadBsp(globals.sceneCur->env, buf, bufsize, 2);
        }
    }

    if (easset->bspCameraAssetID != 0)
    {
        buf = xSTFindAsset(easset->bspCameraAssetID, &bufsize);
        if (buf != NULL)
        {
            xEnvLoadBsp(globals.sceneCur->env, buf, bufsize, 3);
        }
    }

    if (globals.sceneCur->env != NULL && globals.sceneCur->env->geom != NULL)
    {
        zLightResetAll(globals.sceneCur->env);
        iLightInit(globals.sceneCur->env->geom->world);
    }
}

// Apply the asset's lighting to the scene environment.
void zEnvSetup(_zEnv* env)
{
    xBaseSetup(env);
    iEnvLightingBasics(globals.sceneCur->env->geom, env->easset);
}

// Start the camera at the environment's starting camera and store the player's checkpoint.
void zEnvStartingCamera(_zEnv* env)
{
    _tagxCam* cam = (_tagxCam*)zSceneFindObject(env->easset->startCameraAssetID);

    if (cam != NULL)
    {
        xCameraPickInitial(&globals.camera, cam);
        zEntPlayer_StoreCheckPoint(NULL);
    }
}

// Animated surface textures: each world material whose surface animates its texture shows the
// texture its group currently selects. Then render the environment.
void zEnvRender(_xEnv* env)
{
    RpWorld* world = env->geom->world;
    S32 num = world->matList.numMaterials;

    for (S32 i = 0; i < num; i++)
    {
        xSurface* sp = zSurfaceGetSurface(i);
        zSurfaceProps* pp = (zSurfaceProps*)sp->moprops;

        if (pp != NULL && pp->asset != NULL && (pp->texanim_flags & 0x1))
        {
            RpMaterial* mp = world->matList.materials[i];

            if (mp != NULL)
            {
                _zGroup* g = (_zGroup*)pp->texanim[0].group_ptr;

                if (g != NULL)
                {
                    U32 texid = zGroupGetItem(g, pp->texanim[0].group_idx);
                    RwTexture* texptr = (RwTexture*)xSTFindAsset(texid, NULL);

                    if (texptr != NULL)
                    {
                        RpMaterialSetTexture(mp, texptr);
                    }
                }
            }
        }
    }

    xEnvRender(env);
}

// Save the environment base state.
void zEnvSave(_zEnv* ent, xSerial* s)
{
    xBaseSave(ent, s);
}

// Restore the environment base state.
void zEnvLoad(_zEnv* ent, xSerial* s)
{
    xBaseLoad(ent, s);
}

// Environment events: reset, the screen warble effects and the weather.
S32 zEnvEventCB(xBase*, xBase* to, U32 toEvent, F32* toParam, xBase*)
{
    _zEnv* t = (_zEnv*)to;

    switch (toEvent)
    {
    case 10:
        xBaseReset(t, t->easset);
        break;
    case 0x10A:
        iFXWarbSetParams(1, toParam);
        break;
    case 0x10B:
        iFXWarbSetParams(2, toParam);
        break;
    case 0x122:
        xClimateSetSnow(toParam[0]);
        break;
    case 0x121:
        xClimateSetRain(toParam[0]);
        break;
    }

    return 1;
}
