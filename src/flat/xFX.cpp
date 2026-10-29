#include "xFX.h"

#include "iFX.h"
#include "xMemMgr.h"
#include "xModel.h"
#include "xstransvc.h"
#include "zEntPickup.h"
#include "zFX.h"
#include "zGlobals.h"
#include "zScene.h"
#include "zSurface.h"

#include <math.h>
#include <rpmatfx.h>
#include <rpskin.h>
#include <rwcore.h>
#include <string.h>

// One saved copy of an atomic's original texture coordinates (GameCube only).
struct xFXanimUVCopy
{
    RpAtomic* atomic;
    RwTexCoords* uv;
};

RpLight* MainLight;
static U32 xFXanimUVCopyCount;
U32 xfx_initted;

F32 EnvMapShininess = 1.0f;
F32 xFXanimUVRotMat0[2] = { 1.0f, 0.0f };
F32 xFXanimUVRotMat1[2] = { 0.0f, 1.0f };
F32 xFXanimUVTrans[2] = { 0.0f, 0.0f };
F32 xFXanimUVScale[2] = { 1.0f, 1.0f };
F32 xFXanimUV2PRotMat0[2] = { 1.0f, 0.0f };
F32 xFXanimUV2PRotMat1[2] = { 0.0f, 1.0f };
F32 xFXanimUV2PTrans[2] = { 0.0f, 0.0f };
F32 xFXanimUV2PScale[2] = { 1.0f, 1.0f };

static xFXanimUVCopy xFXanimUVCopyList[32];

static void LightResetFrame(RpLight* light);
static inline void xFXanimUVCreate();
static RpWorldSector* SectorMatFXEnableCB(RpWorldSector* worldSector, void*);
static RpMaterial* MaterialSetEnvMap(RpMaterial* material, void* data);
static RpMaterial* MaterialSetBumpMap(RpMaterial* material, void* data);
static inline RpMaterial* MaterialSetBumpEnvMap(RpMaterial* material, RwTexture* envMap,
                                                F32 envCooef, RwTexture* bumpMap, F32 bumpCooef);
RpMaterial* MaterialSetShininess(RpMaterial* material, void*);
RpAtomic* xFXAtomicEnvMapSetup(RpAtomic* atomic, U32 envmapID, F32 shininess);

static inline F32 isin(F32 x)
{
    return sin(x);
}

static inline F32 icos(F32 x)
{
    return cos(x);
}

// Create the directional light used for environment and bump maps, and the UV animation pipeline.
void xFXInit()
{
    if (!xfx_initted)
    {
        xfx_initted = 1;

        RpLight* light = RpLightCreate(rpLIGHTDIRECTIONAL);

        if (light)
        {
            RwFrame* frame = RwFrameCreate();

            if (frame)
            {
                RpLightSetFrame(light, frame);
                LightResetFrame(light);

                MainLight = light;
            }
            else
            {
                RpLightDestroy(light);
            }
        }

        xFXanimUVCreate();
    }
}

// Apply each surface's environment and bump maps to the world materials, then give the shiny
// entities (some villains, pickups and the h003 platforms) their environment maps.
void xFX_SceneEnter(RpWorld* world)
{
    S32 i;
    S32 num;

    RpWorldForAllWorldSectors(world, SectorMatFXEnableCB, NULL);

    num = world->matList.numMaterials;

    for (i = 0; i < num; i++)
    {
        xSurface* sp = zSurfaceGetSurface(i);
        zSurfaceProps* pp = (zSurfaceProps*)sp->moprops;

        if (pp && pp->asset)
        {
            zSurfMatFX* fxp = &pp->asset->matfx;

            if (fxp->flags)
            {
                if (fxp->flags == 0x10)
                {
                    fxp->flags |= 0x1;
                }

                RpMaterial* mp = world->matList.materials[i];

                if (mp->texture)
                {
                    gFXSurfaceFlags = fxp->flags;

                    if (fxp->flags & 0x1)
                    {
                        RwTexture* env = (RwTexture*)xSTFindAsset(fxp->envmapID, NULL);

                        if (!env)
                        {
                            continue;
                        }

                        MaterialSetEnvMap(mp, env);
                        RpMatFXMaterialSetEnvMapCoefficient(mp, 0.5f * fxp->shininess);
                    }

                    if (fxp->flags & 0x2)
                    {
                        if (globals.sceneCur->sceneID == 'H002')
                        {
                            continue;
                        }

                        RwTexture* bump = (RwTexture*)xSTFindAsset(fxp->bumpmapID, NULL);

                        static S32 skip_bump = 0;

                        if (!bump)
                        {
                            continue;
                        }

                        if (skip_bump)
                        {
                            continue;
                        }

                        MaterialSetBumpMap(mp, bump);
                        RpMatFXMaterialSetBumpMapCoefficient(mp, fxp->bumpiness);
                    }

                    if (fxp->flags & 0x4)
                    {
                        static S32 skip_bumpenv = 0;

                        RwTexture* env = (RwTexture*)xSTFindAsset(fxp->envmapID, NULL);
                        RwTexture* bump = (RwTexture*)xSTFindAsset(fxp->bumpmapID, NULL);

                        if (!env || !bump)
                        {
                            continue;
                        }

                        if (skip_bumpenv)
                        {
                            continue;
                        }

                        MaterialSetBumpEnvMap(mp, env, fxp->shininess, bump, fxp->bumpiness);
                    }
                }
            }
        }
    }

    zScene* sc = globals.sceneCur;

    for (i = 0; i < sc->num_act_ents; i++)
    {
        _xEnt* ent = sc->act_ents[i];

        if (ent->id == 0xEBB8C925 || ent->id == 0xE8E6A355)
        {
            xFXAtomicEnvMapSetup(ent->model->Data, 0xE891750E, 0.3f);
        }

        if (ent->baseType == 2)
        {
            switch (ent->subType)
            {
            case 1:
                xFXAtomicEnvMapSetup(ent->model->Data, 0xE891750E, 0.3f);
                break;
            case 4:
                break;
            case 7:
                break;
            }
        }

        if (ent->baseType == 4)
        {
            zAssetPickup* p = ((_zEntPickup*)ent)->p;

            if (p->pickupType == 1)
            {
                switch (p->pickupIndex)
                {
                case 5:
                case 6:
                case 8:
                case 9:
                    xFXAtomicEnvMapSetup(ent->model->Data, 0xE891750E, 0.3f);
                    break;
                }
            }
            else if (p->pickupType == 2)
            {
                xFXAtomicEnvMapSetup(ent->model->Data, 0xBBBC909C, 0.5f);
            }
            else if (p->pickupType == 4)
            {
                xFXAtomicEnvMapSetup(ent->model->Data, 0xE891750E, 2.0f);
            }
        }

        if (sc->sceneID == zSceneIDFromName("h003") && ent->baseType == 6)
        {
            xFXAtomicEnvMapSetup(ent->model->Data, 0xE891750E, 2.0f);
        }
    }

    xFXanimUVCopyCount = 0;
}

// Nothing to undo on GameCube when a scene ends.
void xFX_SceneExit(RpWorld*)
{
}

// Advance the water and tree warble effect.
void xFXUpdate(F32 dt)
{
    iFXWarbUpdate(dt);
}

// Point the main light down at 45 degrees on two axes.
static void LightResetFrame(RpLight* light)
{
    RwV3d v1 = { 1, 0, 0 };
    RwV3d v2 = { 0, 1, 0 };

    RwFrame* frame = RpLightGetFrame(light);

    RwFrameRotate(frame, &v1, 45.0f, rwCOMBINEREPLACE);
    RwFrameRotate(frame, &v2, 45.0f, rwCOMBINEPOSTCONCAT);
}

// Turn on material effects for one world sector.
static RpWorldSector* SectorMatFXEnableCB(RpWorldSector* worldSector, void*)
{
    RpMatFXWorldSectorEnableEffects(worldSector);
    return worldSector;
}

// Set an environment-mapped material's reflection strength to EnvMapShininess.
RpMaterial* MaterialSetShininess(RpMaterial* material, void*)
{
    RpMatFXMaterialFlags flags = RpMatFXMaterialGetEffects(material);

    if (flags == rpMATFXEFFECTENVMAP || flags == rpMATFXEFFECTBUMPENVMAP)
    {
        RpMatFXMaterialSetEnvMapCoefficient(material, EnvMapShininess);
    }

    return material;
}

// Environment-map a world material, lit from the camera when the surface asks for it.
static RpMaterial* MaterialSetEnvMap(RpMaterial* material, void* data)
{
    RwTexture* texture;
    RwFrame* frame;

    if (data == NULL)
    {
        return NULL;
    }

    if (material->texture)
    {
        texture = (RwTexture*)data;

        if (texture)
        {
            if (gFXSurfaceFlags & 0x10)
            {
                if (globals.camera.lo_cam)
                {
                    frame = RwCameraGetFrame(globals.camera.lo_cam);
                }
                else
                {
                    frame = RpLightGetFrame(MainLight);
                }
            }
            else
            {
                frame = RpLightGetFrame(MainLight);
            }

            RpMatFXMaterialSetEffects(material, rpMATFXEFFECTENVMAP);
            RpMatFXMaterialSetupEnvMap(material, texture, frame, FALSE, 1.0f);
        }
        else
        {
            RpMatFXMaterialSetEffects(material, rpMATFXEFFECTNULL);
        }
    }

    return material;
}

// Environment-map an entity material. The spec3 map follows the main light; every other map
// follows the camera.
RpMaterial* MaterialSetEnvMap2(RpMaterial* material, void* data)
{
    char* textureName;
    RwTexture* texture;
    RwFrame* frame;

    if (material->texture)
    {
        texture = (RwTexture*)data;
        textureName = texture->name;

        if (rwstrcmp(textureName, "spec3"))
        {
            frame = RwCameraGetFrame(globals.camera.lo_cam);
        }
        else
        {
            frame = RpLightGetFrame(MainLight);
        }

        RpMatFXMaterialSetEffects(material, rpMATFXEFFECTENVMAP);
        RpMatFXMaterialSetupEnvMap(material, texture, frame, FALSE, EnvMapShininess);
    }

    return material;
}

// Set the reflection strength of every material of an atomic.
static inline RpAtomic* AtomicSetShininess(RpAtomic* atomic, void* data)
{
    RpGeometry* geometry = RpAtomicGetGeometry(atomic);

    if (geometry)
    {
        RpGeometryForAllMaterials(geometry, MaterialSetShininess, data);
    }

    return atomic;
}

// Environment-map every material of an atomic with the given texture.
static inline RpAtomic* AtomicSetEnvMap(RpAtomic* atomic, void* data)
{
    RpMatFXAtomicEnableEffects(atomic);

    if (atomic->geometry)
    {
        RpGeometryForAllMaterials(atomic->geometry, MaterialSetEnvMap2, data);
    }

    return atomic;
}

// Environment-map every material of an atomic with the given texture and reflection strength.
RpAtomic* xFXAtomicEnvMapSetup(RpAtomic* atomic, U32 envmapID, F32 shininess)
{
    RwTexture* env = (RwTexture*)xSTFindAsset(envmapID, NULL);

    if (!env)
    {
        return NULL;
    }

    AtomicSetEnvMap(atomic, env);

    F32 tmp = EnvMapShininess;
    EnvMapShininess = shininess;
    AtomicSetShininess(atomic, NULL);
    EnvMapShininess = tmp;

    RpSkin* skin = RpSkinGeometryGetSkin(atomic->geometry);

    if (skin)
    {
        RpSkinAtomicSetType(atomic, rpSKINTYPEMATFX);
    }

    return atomic;
}

// Bump-map a world material, lit from the main light.
static RpMaterial* MaterialSetBumpMap(RpMaterial* material, void* data)
{
    RwFrame* frame;

    if (data == NULL)
    {
        return NULL;
    }
    else if (material->texture)
    {
        if (data)
        {
            frame = RpLightGetFrame(MainLight);
            RpMatFXMaterialSetEffects(material, rpMATFXEFFECTBUMPMAP);
            RpMatFXMaterialSetupBumpMap(material, (RwTexture*)data, frame, 1.0f);
        }
        else
        {
            RpMatFXMaterialSetEffects(material, rpMATFXEFFECTNULL);
        }
    }

    return material;
}

// Bump- and environment-map a world material.
static inline RpMaterial* MaterialSetBumpEnvMap(RpMaterial* material, RwTexture* envMap,
                                                F32 envCooef, RwTexture* bumpMap, F32 bumpCooef)
{
    if (envMap == NULL || bumpMap == NULL)
    {
        return NULL;
    }
    else
    {
        RwFrame* frame;

        RpMatFXMaterialSetEffects(material, rpMATFXEFFECTBUMPENVMAP);

        if (gFXSurfaceFlags & 0x10)
        {
            frame = RwCameraGetFrame(globals.camera.lo_cam);
        }
        else
        {
            frame = RpLightGetFrame(MainLight);
        }

        RpMatFXMaterialSetupEnvMap(material, envMap, frame, TRUE, envCooef);
        RpMatFXMaterialSetupBumpMap(material, bumpMap, RpLightGetFrame(MainLight), bumpCooef);
    }

    return material;
}

static S32 sSetMaterialTextureBufferCount;
static RwTexture* sSetMaterialTextureBuffer[32];

// Save a material's texture and replace it.
static RpMaterial* iModelSetMaterialTextureCallBack(RpMaterial* material, void* data)
{
    sSetMaterialTextureBuffer[sSetMaterialTextureBufferCount++] = material->texture;
    RpMaterialSetTexture(material, (RwTexture*)data);
    return material;
}

// Give a material back the texture saved for it.
static RpMaterial* iModelSetMaterialTextureRestoreCallBack(RpMaterial* material, void*)
{
    RpMaterialSetTexture(material, sSetMaterialTextureBuffer[--sSetMaterialTextureBufferCount]);
    return material;
}

// Swap every material's texture for another, saving the originals.
void iModelSetMaterialTexture(RpAtomic* model, void* texture)
{
    RpGeometryForAllMaterials(model->geometry, iModelSetMaterialTextureCallBack, texture);
}

// Put back the textures saved by iModelSetMaterialTexture.
void iModelSetMaterialTextureRestore(RpAtomic* model)
{
    RpGeometryForAllMaterials(model->geometry, iModelSetMaterialTextureRestoreCallBack, NULL);
}

static S32 sMaterialAlpha;

// Set a material's alpha to sMaterialAlpha.
static RpMaterial* iModelMaterialCallBack(RpMaterial* material, void* data)
{
    RwRGBA new_rw_col = material->color;
    new_rw_col.alpha = sMaterialAlpha;
    material->color = new_rw_col;
    return material;
}

// Set the alpha of every material of a model.
void iModelSetMaterialAlpha(RpAtomic* model, S32 alpha)
{
    RpGeometry* geom = model->geometry;
    geom->flags |= rpGEOMETRYMODULATEMATERIALCOLOR;
    sMaterialAlpha = alpha;
    RpGeometryForAllMaterials(geom, iModelMaterialCallBack, &alpha);
}

static S32 sMaterialTotal;
static RwRGBA sMaterialColor[16];

// Save a material's colour and scale it by the three factors, clamped to 255.
static RpMaterial* iModelMatMultiplyCallBack(RpMaterial* material, void* data)
{
    RwRGBA* rw_col = &sMaterialColor[sMaterialTotal++];
    *rw_col = material->color;

    RwRGBA col = *rw_col;
    F32* mods = (F32*)data;
    F32 tmp;

    tmp = col.red * mods[0];
    if (tmp < 0.0f)
    {
        tmp = 0.0f;
    }
    else if (tmp > 255.0f)
    {
        tmp = 255.0f;
    }
    col.red = tmp;

    tmp = col.green * mods[1];
    if (tmp < 0.0f)
    {
        tmp = 0.0f;
    }
    else if (tmp > 255.0f)
    {
        tmp = 255.0f;
    }
    col.green = tmp;

    tmp = col.blue * mods[2];
    if (tmp < 0.0f)
    {
        tmp = 0.0f;
    }
    else if (tmp > 255.0f)
    {
        tmp = 255.0f;
    }
    col.blue = tmp;

    material->color = col;
    return material;
}

// Give a material back the colour saved for it.
static RpMaterial* iModelMatMultiplyResetCallBack(RpMaterial* material, void*)
{
    material->color = sMaterialColor[sMaterialTotal++];
    return material;
}

// Scale the colour of every material of a model, saving the originals.
void iModelMaterialMul(RpAtomic* model, F32 rm, F32 gm, F32 bm)
{
    F32 cols[3];
    RpGeometry* geom;

    sMaterialTotal = 0;

    geom = model->geometry;
    geom->flags |= rpGEOMETRYMODULATEMATERIALCOLOR;

    cols[0] = rm;
    cols[1] = gm;
    cols[2] = bm;

    RpGeometryForAllMaterials(geom, iModelMatMultiplyCallBack, cols);
}

// Put back the colours saved by iModelMaterialMul.
void iModelResetMaterial(RpAtomic* model)
{
    sMaterialTotal = 0;
    RpGeometryForAllMaterials(model->geometry, iModelMatMultiplyResetCallBack, NULL);
}

static RxPipeline* xFXanimUVPipeline;
RwTexture* xFXanimUV2PTexture;

// Build the render pipeline for UV-animated atomics.
static inline void xFXanimUVCreate()
{
    xFXanimUVPipeline = iFXanimUVCreatePipe();
}

// Set the offset of the first UV layer.
void xFXanimUVSetTranslation(xVec3* trans)
{
    xFXanimUVTrans[0] = trans->x;
    xFXanimUVTrans[1] = trans->y;
}

// Set the scale of the first UV layer.
void xFXanimUVSetScale(xVec3* scale)
{
    xFXanimUVScale[0] = scale->x;
    xFXanimUVScale[1] = scale->y;
}

// Set the rotation of the first UV layer.
void xFXanimUVSetAngle(F32 angle)
{
    F32 s = isin(angle);
    F32 c = icos(angle);

    xFXanimUVRotMat0[0] = c;
    xFXanimUVRotMat0[1] = -s;
    xFXanimUVRotMat1[0] = s;
    xFXanimUVRotMat1[1] = c;
}

// Set the offset of the second UV layer.
void xFXanimUV2PSetTranslation(xVec3* trans)
{
    xFXanimUV2PTrans[0] = trans->x;
    xFXanimUV2PTrans[1] = trans->y;
}

// Set the scale of the second UV layer.
void xFXanimUV2PSetScale(xVec3* scale)
{
    xFXanimUV2PScale[0] = scale->x;
    xFXanimUV2PScale[1] = scale->y;
}

// Set the rotation of the second UV layer.
void xFXanimUV2PSetAngle(F32 angle)
{
    F32 s = isin(angle);
    F32 c = icos(angle);

    xFXanimUV2PRotMat0[0] = c;
    xFXanimUV2PRotMat0[1] = -s;
    xFXanimUV2PRotMat1[0] = s;
    xFXanimUV2PRotMat1[1] = c;
}

// Set the texture of the second UV layer.
void xFXanimUV2PSetTexture(RwTexture* texture)
{
    xFXanimUV2PTexture = texture;
}

// Animate an atomic's texture coordinates on the CPU. The first call for an atomic saves its
// original coordinates; each call rewrites them as scale * (rotation * original) + translation.
RpAtomic* xFXanimUVAtomicSetup(RpAtomic* atomic)
{
    S32 num;
    U32 i;
    RwTexCoords* src;
    RpGeometry* geom;
    RwTexCoords* orig;
    RwTexCoords* dst;
    RwTexCoords* copy;

    if (!atomic)
    {
        return NULL;
    }

    for (i = 0; i < xFXanimUVCopyCount; i++)
    {
        if (xFXanimUVCopyList[i].atomic == atomic)
        {
            break;
        }
    }

    if (i == xFXanimUVCopyCount)
    {
        geom = atomic->geometry;
        num = geom->numVertices;
        orig = geom->texCoords[0];
        copy = (RwTexCoords*)xMemAlloc(gActiveHeap, num * sizeof(RwTexCoords));

        if (copy)
        {
            memcpy(copy, orig, num * 8);
            xFXanimUVCopyList[xFXanimUVCopyCount].atomic = atomic;
            xFXanimUVCopyList[xFXanimUVCopyCount].uv = copy;
            xFXanimUVCopyCount++;
        }
    }

    src = xFXanimUVCopyList[i].uv;

    if (atomic && src)
    {
        RpGeometry* geom = atomic->geometry;

        if (geom)
        {
            dst = geom->texCoords[0];

            if (dst)
            {
                RpGeometryLock(geom, rpGEOMETRYLOCKTEXCOORDS1);

                num = geom->numVertices;

                for (S32 j = 0; j < num; j++)
                {
                    dst->u = xFXanimUVScale[0] *
                                 (xFXanimUVRotMat0[0] * src->u + xFXanimUVRotMat0[1] * src->v) +
                             xFXanimUVTrans[0];
                    dst->v = xFXanimUVScale[1] *
                                 (xFXanimUVRotMat1[0] * src->u + xFXanimUVRotMat1[1] * src->v) +
                             xFXanimUVTrans[1];
                    src++;
                    dst++;
                }

                RpGeometryUnlock(geom);
            }
        }
    }

    return atomic;
}
