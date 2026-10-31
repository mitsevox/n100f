#include "xShadow.h"
#include "xDraw.h"
#include "xMath.h"
#include "xMath3.h"
#include "xMemMgr.h"
#include "xEnt.h"
#include "xEnv.h"
#include "xModel.h"
#include "xScene.h"
#include "iCollide.h"
#include "iModel.h"
#include "iParMgr.h"
#include "zEnt.h"
#include "zGlobals.h"
#include "zMenu.h"
#include "zScene.h"

#include <rwcore.h>
#include <rpworld.h>

#include <math.h>
#include <string.h>

// The shadow pass renders one entity's model, white on clear, into a small camera's raster seen
// from the light, then projects that raster down onto the world triangles and receiving models
// under the entity.

struct RpIntersection
{
    union
    {
        RwSphere sphere;
        U8 pad[0x18];
    } t;
    S32 type;
};

struct RpCollisionTriangle
{
    RwV3d normal;
    RwV3d point;
    S32 index;
    RwV3d* vertices[3];
};

// Shared by ShadowRender and its triangle callback: the projection from world space into the
// shadow raster's 0..1 texture space.
struct _ProjectionParam
{
    RwV3d at;
    RwMatrix invMatrix;
    U8 shadowValue;
    S32 fade;
    U32 numIm3DBatch;
};

extern "C" RwMatrix* RwMatrixInvert(RwMatrix* matrixOut, const RwMatrix* matrixIn);
extern "C" RwMatrix* RwMatrixTranslate(RwMatrix* matrix, const RwV3d* translation,
                                       RwOpCombineType combineOp);
extern "C" F32 RwV3dLength(const RwV3d* in);
extern "C" RpWorld* RpCollisionWorldForAllIntersections(
    RpWorld* world, RpIntersection* intersection,
    RpCollisionTriangle* (*callBack)(RpIntersection*, RpWorldSector*, RpCollisionTriangle*, F32,
                                     void*),
    void* data);
extern "C" void fn_80178A3C(RwRaster* raster);

static S32 ShadowRender(RwCamera* shadowCamera, RwRaster* shadowRaster, RpWorld* world,
                        RpIntersection* shadowZone, F32 shadowFactor, F32 fadeDist);
static RpCollisionTriangle* ShadowRenderTriangleCB(RpIntersection* intersection,
                                                   RpWorldSector* sector,
                                                   RpCollisionTriangle* collTriangle,
                                                   F32 distance, void* data);
void InvertRaster(RwCamera* cam);
S32 Im2DRenderQuad(F32 x1, F32 y1, F32 x2, F32 y2, F32 z, F32 recipCamZ, F32 uvOffset);
void xShadowRenderWorld(xVec3* center, F32 radius);

static xSphere sSphere;
F32 ShadowThreshold = 144.0f;
F32 ShadowZoneRadius = 2.25f;
F32 ShadowStrength = 0.3f;
static F32 SHADOW_BF_DOT;
static F32 SHADOW_BOTH;
static RxObjSpace3DVertex* Im3DBuffer;
static U32 Im3DBufferPos;
static U32 ShadowNumTriangles;
static RwCamera* ShadowCamera;
static RwRaster* ShadowCameraRaster;
static RwRaster* ShadowRenderRaster;
S32 ShadowFade;
static U32 sShadowFlags;
static RwRaster* gc_saveraster;

static inline void ShadowCameraDestroy(RwCamera* shadowCamera)
{
    RwFrame* frame;
    RwRaster* raster;

    if (shadowCamera)
    {
        _rwFrameSyncDirty();
        frame = (RwFrame*)shadowCamera->object.object.parent;
        if (frame)
        {
            _rwObjectHasFrameSetFrame(shadowCamera, NULL);
            RwFrameDestroy(frame);
        }
        raster = shadowCamera->zBuffer;
        if (raster)
        {
            shadowCamera->zBuffer = NULL;
            RwRasterDestroy(raster);
        }
        raster = shadowCamera->frameBuffer;
        if (raster)
        {
            shadowCamera->frameBuffer = NULL;
            RwRasterDestroy(raster);
        }
        RwCameraDestroy(shadowCamera);
    }
}

static inline RwCamera* ShadowCameraCreatePersp(S32 res)
{
    RwCamera* cam;
    RwRaster* zRaster;

    cam = RwCameraCreate();
    if (cam)
    {
        _rwObjectHasFrameSetFrame(cam, RwFrameCreate());
        if (cam->object.object.parent)
        {
            zRaster = RwRasterCreate(res, res, 0, rwRASTERTYPEZBUFFER);
            if (zRaster)
            {
                cam->zBuffer = zRaster;
                RwCameraSetProjection(cam, rwPARALLEL);
                return cam;
            }
        }
    }
    ShadowCameraDestroy(cam);
    return NULL;
}

static inline S32 SetupShadow()
{
    S32 res = 256;

    // Halve the raster until it fits the 640x480 frame buffer.
    while (res > 640 || res > 480)
    {
        res >>= 1;
    }

    ShadowCamera = ShadowCameraCreatePersp(res);
    if (ShadowCamera)
    {
        ShadowCameraRaster = RwRasterCreate(res, res, 0, rwRASTERTYPECAMERATEXTURE);
        if (ShadowCameraRaster)
        {
            ShadowCamera->frameBuffer = ShadowCameraRaster;
        }
    }
    return 1;
}

// Called once from iSystemInit; also makes the raster that saves the frame buffer around the
// shadow pass.
void xShadowCameraCreate()
{
    SetupShadow();
    gc_saveraster = RwRasterCreate(256, 256, 32, rwRASTERTYPETEXTURE | rwRASTERFORMAT8888);
}

// A world triangle takes the shadow only while its dot with the light stays at or below
// SHADOW_BF_DOT; the listed scenes lower it from 0 to -0.5, so steep surfaces drop out. W028
// casts from further away.
void xShadowSetWorld(RpWorld* world)
{
    U32 sceneID = globals.sceneCur->sceneID;

    RpWorldAddCamera(world, ShadowCamera);
    SHADOW_BOTH = 2.0f;
    SHADOW_BF_DOT = 0.0f;
    if (sceneID == zSceneIDFromName("E001") || sceneID == zSceneIDFromName("E002") ||
        sceneID == zSceneIDFromName("F003") || sceneID == zSceneIDFromName("G002") ||
        sceneID == zSceneIDFromName("I020") || sceneID == zSceneIDFromName("I021") ||
        sceneID == zSceneIDFromName("I003") || sceneID == zSceneIDFromName("L011") ||
        sceneID == zSceneIDFromName("O004"))
    {
        SHADOW_BF_DOT = -0.5f;
    }
    ShadowThreshold = 144.0f;
    if (sceneID == zSceneIDFromName("W028"))
    {
        ShadowThreshold = 225.0f;
    }
}

// Aim the shadow camera down in_vec at target_pos.
void xShadowSetLight(xVec3* target_pos, xVec3* in_vec, F32 strength)
{
    xVec3 zvec;
    xMat4x3 matrix;
    RwFrame* camFrame;
    RwMatrix* camMatrix;

    xVec3Normalize(&zvec, in_vec);
    xMat3x3LookVec((xMat3x3*)&matrix, &zvec);
    matrix.pos = *target_pos;
    camFrame = (RwFrame*)ShadowCamera->object.object.parent;
    camMatrix = &camFrame->modelling;
    memcpy(camMatrix, &matrix, sizeof(xMat4x3));
    RwFrameOrthoNormalize(camFrame);
    RwMatrixUpdate(camMatrix);
    RwFrameUpdateObjects(camFrame);
}

// Cast three short rays along the light under the entity. When they reach the world geometry
// before any entity, project the shadow onto the world triangles in a sphere round the hit.
void xShadowRenderWorld(xVec3* center, F32 radius)
{
    RpIntersection shadowZone;
    xSphere zone;
    RwMatrix* camMatrix;
    xRay3 R[3];
    _xCollis crec[3];
    F32 sf[3][2] = { { 0.0f, 0.0f }, { 0.0f, 0.5f }, { 0.0f, -0.5f } };
    U32 hit_env;
    U32 hit_ent;
    F32 ent_dist;
    F32 env_dist;
    S32 i;
    xVec3 hit;
    _xCollis ecrec;

    camMatrix = &((RwFrame*)ShadowCamera->object.object.parent)->modelling;
    zone.center.x = 0.0f;
    zone.center.y = 0.0f;
    zone.center.z = 0.0f;
    sShadowFlags = 0;
    ent_dist = 100.0f;
    env_dist = 100.0f;
    hit_env = 0;
    hit_ent = 0;
    for (i = 0; i < 3; i++)
    {
        R[i].dir = *(xVec3*)&camMatrix->at;
        R[i].origin = *center;
        R[i].origin.x += camMatrix->right.x * sf[i][0];
        R[i].origin.y += camMatrix->right.y * sf[i][0];
        R[i].origin.z += camMatrix->right.z * sf[i][0];
        R[i].origin.x += camMatrix->up.x * sf[i][1];
        R[i].origin.y += camMatrix->up.y * sf[i][1];
        R[i].origin.z += camMatrix->up.z * sf[i][1];
        R[i].min_t = 0.0f;
        R[i].max_t = 10.0f;
        R[i].flags = 0xC00;
        xRayHitsScene(globals.sceneCur, &R[i], &crec[i]);

        hit = R[i].origin;
        hit.x = R[i].dir.x * crec[i].dist + hit.x;
        hit.y = R[i].dir.y * crec[i].dist + hit.y;
        hit.z = R[i].dir.z * crec[i].dist + hit.z;

        if (crec[i].flags & 1)
        {
            if (crec[i].optr)
            {
                hit_ent++;
                xDrawSetColor(255, 0, 0, 255);
                if (crec[i].dist < ent_dist)
                {
                    ent_dist = crec[i].dist;
                }
                iRayHitsEnv(&R[i], globals.sceneCur->env, &ecrec);
                if (ecrec.flags & 1)
                {
                    hit_env++;
                    if (ecrec.dist < env_dist)
                    {
                        env_dist = ecrec.dist;
                        zone.center.x = R[i].dir.x * ecrec.dist + R[i].origin.x;
                        zone.center.y = R[i].dir.y * ecrec.dist + R[i].origin.y;
                        zone.center.z = R[i].dir.z * ecrec.dist + R[i].origin.z;
                    }
                }
            }
            else
            {
                xDrawSetColor(0, 255, 0, 255);
                hit_env++;
                if (crec[i].dist < env_dist)
                {
                    env_dist = crec[i].dist;
                    zone.center = hit;
                }
            }
        }
    }

    if (hit_env && hit_ent && ent_dist > 0.0f && env_dist > 0.0f &&
        (F32)fabs(ent_dist - env_dist) < SHADOW_BOTH)
    {
        sShadowFlags |= 3;
    }
    if (hit_env && env_dist > 0.0f && env_dist < ent_dist)
    {
        sShadowFlags |= 1;
    }
    if (hit_ent && ent_dist > 0.0f && ent_dist < env_dist)
    {
        sShadowFlags |= 2;
    }

    if (sShadowFlags & 1)
    {
        zone.r = radius;
        sSphere = zone;
        shadowZone.type = 3;
        shadowZone.t.sphere = *(RwSphere*)&zone;
        ShadowRender(ShadowCamera, ShadowRenderRaster, globals.sceneCur->env->geom->world,
                     &shadowZone, ShadowStrength, ShadowFade ? ShadowZoneRadius : 0.0f);
    }
    else
    {
        sSphere.r = 0.0f;
    }
}

static inline U32 xShadowEntIsOutsideFrustum(_xEnt* ent)
{
    F32 xScale2;
    F32 yScale2;
    F32 zScale2;
    F32 atomicScale;
    RwMatrix* mat;
    RwCamera* cam;
    RwSphere worldsph;
    RpAtomic* atomic;

    if (!ent || !ent->model || !(atomic = ent->model->Data))
    {
        return 1;
    }

    cam = ShadowCamera;
    mat = ent->model->Mat;
    RwV3dTransformPoints(&worldsph.center, &atomic->boundingSphere.center, 1, mat);
    yScale2 = mat->up.x * mat->up.x + mat->up.y * mat->up.y + mat->up.z * mat->up.z;
    zScale2 = mat->at.x * mat->at.x + mat->at.y * mat->at.y + mat->at.z * mat->at.z;
    xScale2 = mat->right.x * mat->right.x + mat->right.y * mat->right.y + mat->right.z * mat->right.z;
    atomicScale = (xScale2 >= (yScale2 >= zScale2 ? yScale2 : zScale2))
                      ? xScale2
                      : (yScale2 >= zScale2 ? yScale2 : zScale2);
    atomicScale = sqrtf(atomicScale);
    worldsph.radius = atomic->boundingSphere.radius * atomicScale;
    return RwCameraFrustumTestSphere(cam, &worldsph) == rwSPHEREOUTSIDE;
}

// Decide whether ent takes the shadow this frame: shadows are on, the entity is visible and in
// the shadow camera's view, and (in W027) it is a valid receiver.
U32 xShadowReceiveShadowSetup(_xEnt* ent)
{
    U32 sceneID = globals.sceneCur->sceneID;
    U32 rsf_valid;

    if (zMenuRunning())
    {
        return 0;
    }
    if (!(sShadowFlags & 2))
    {
        return 0;
    }
    if (!ent->model || !xEntIsVisible(ent))
    {
        return 0;
    }

    rsf_valid = 0;
    if (sceneID == zSceneIDFromName("w027"))
    {
        rsf_valid = 1;
    }
    if (rsf_valid && !(ent->baseFlags & 0x10))
    {
        return 0;
    }
    if (xShadowEntIsOutsideFrustum(ent))
    {
        return 0;
    }

    // One entity in C004 drops out near its spot.
    if (ent->id == 0xFC1DFD0B && sceneID == zSceneIDFromName("c004"))
    {
        xVec3 spot = { -458.0f, -17.0f, 121.0f };
        xVec3* pos = (xVec3*)&globals.player.ent.model->Mat->pos;
        F32 dx = pos->x - spot.x;
        F32 dy = pos->y - spot.y;
        F32 dz = pos->z - spot.z;

        if (dx * dx + dy * dy + dz * dz < 9.0f)
        {
            return 0;
        }
    }
    return 1;
}

// Project the shadow raster onto every front-facing triangle of ent's models.
void xShadowReceiveShadow(_xEnt* ent)
{
    RwCamera* shadowCamera = ShadowCamera;
    RwMatrix* shadowMatrix;
    RwMatrix invMatrix;
    F32 radius;
    RwV3d scl;
    RwV3d tr;
    U32 i;
    xVec3* xvert;
    RpTriangle* tri;
    RpGeometry* geom;
    F32 shadowFactor;
    U8 val;
    xModelInstance* model;
    U32 max_verts;
    RxObjSpace3DVertex* imv;
    RwV3d vIn[3];
    RwV3d vShad[3];
    xVec3* v0;
    xVec3* v1;
    xVec3* v2;
    RwV3d c;
    F32 len;
    RwV3d at;
    S32 fogstate;

    if (!xShadowEntIsOutsideFrustum(ent) && !(ent->flags & 0x4) &&
        !(ent->material_alpha < 255.0f))
    {
        if (ent->moreFlags & 0x1)
        {
            return;
        }

        RwRenderStateSet(rwRENDERSTATETEXTURERASTER, ShadowCamera->frameBuffer);
        RwRenderStateSet(rwRENDERSTATETEXTUREADDRESS, (void*)rwTEXTUREADDRESSCLAMP);
        RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, (void*)TRUE);
        RwRenderStateGet(rwRENDERSTATEFOGENABLE, &fogstate);
        RwRenderStateSet(rwRENDERSTATEFOGENABLE, (void*)FALSE);
        RwRenderStateSet(rwRENDERSTATESRCBLEND, (void*)rwBLENDZERO);

        shadowFactor = ShadowStrength;
        if (shadowFactor < 0.0f)
        {
            shadowFactor = -shadowFactor;
            RwRenderStateSet(rwRENDERSTATEDESTBLEND, (void*)rwBLENDSRCCOLOR);
        }
        else
        {
            RwRenderStateSet(rwRENDERSTATEDESTBLEND, (void*)rwBLENDINVSRCCOLOR);
        }

        radius = shadowCamera->viewWindow.x;
        shadowMatrix = &((RwFrame*)shadowCamera->object.object.parent)->modelling;
        at = shadowMatrix->at;
        RwMatrixInvert(&invMatrix, shadowMatrix);
        scl.x = scl.y = -0.5f / radius;
        scl.z = 1.0f / (0.0f + radius);
        RwMatrixScale(&invMatrix, &scl, rwCOMBINEPOSTCONCAT);
        tr.x = tr.y = 0.5f;
        tr.z = 0.0f;
        RwMatrixTranslate(&invMatrix, &tr, rwCOMBINEPOSTCONCAT);

        val = (U8)(255.0f * shadowFactor);

        max_verts = 0;
        for (model = ent->model; model; model = model->Next)
        {
            if ((U32)model->Data->geometry->numVertices > max_verts)
            {
                max_verts = model->Data->geometry->numVertices;
            }
        }

        xvert = (xVec3*)xMemPushTemp(max_verts * sizeof(xVec3));
        if (!xvert)
        {
            return;
        }

        Im3DBuffer = gRenderBuffer.m_vertex;
        for (model = ent->model; model; model = model->Next)
        {
            geom = model->Data->geometry;
            iModelVertEval(model->Data, 0, geom->numVertices, model->Mat, NULL, xvert);
            xDrawSetColor(0, 0, 255, 255);

            tri = geom->triangles;
            for (i = 0; i < (U32)geom->numTriangles; i++, tri++)
            {
                if (Im3DBufferPos > 477)
                {
                    if (RwIm3DTransform(Im3DBuffer, Im3DBufferPos, NULL,
                                        rwIM3D_VERTEXUV | rwIM3D_VERTEXXYZ | rwIM3D_VERTEXRGBA))
                    {
                        RwIm3DRenderPrimitive(rwPRIMTYPETRILIST);
                        RwIm3DEnd();
                    }
                    Im3DBufferPos = 0;
                }

                v0 = &xvert[tri->vertIndex[0]];
                v1 = &xvert[tri->vertIndex[1]];
                v2 = &xvert[tri->vertIndex[2]];
                imv = &Im3DBuffer[Im3DBufferPos];
                *(xVec3*)&vIn[0] = *v0;
                *(xVec3*)&vIn[1] = *v1;
                *(xVec3*)&vIn[2] = *v2;
                RwV3dTransformPoints(vShad, vIn, 3, &invMatrix);

                if ((vShad[0].z < 0.0f && vShad[1].z < 0.0f && vShad[2].z < 0.0f) ||
                    (vShad[0].x < 0.0f && vShad[1].x < 0.0f && vShad[2].x < 0.0f) ||
                    (vShad[0].x > 1.0f && vShad[1].x > 1.0f && vShad[2].x > 1.0f) ||
                    (vShad[0].y < 0.0f && vShad[1].y < 0.0f && vShad[2].y < 0.0f) ||
                    (vShad[0].y > 1.0f && vShad[1].y > 1.0f && vShad[2].y > 1.0f))
                {
                    continue;
                }

                {
                    F32 ax = v1->x - v0->x;
                    F32 ay = v1->y - v0->y;
                    F32 az = v1->z - v0->z;
                    F32 bx = v2->x - v0->x;
                    F32 by = v2->y - v0->y;
                    F32 bz = v2->z - v0->z;

                    c.x = ay * bz - az * by;
                    c.y = az * bx - ax * bz;
                    c.z = ax * by - ay * bx;
                }
                len = 0.001f / RwV3dLength(&c);
                c.x *= len;
                c.y *= len;
                c.z *= len;
                if (c.x * at.x + c.y * at.y + c.z * at.z > 0.0f)
                {
                    continue;
                }

                RwIm3DVertexSetPos(&imv[0], v0->x + c.x, v0->y + c.y, v0->z + c.z);
                RwIm3DVertexSetPos(&imv[1], v1->x + c.x, v1->y + c.y, v1->z + c.z);
                RwIm3DVertexSetPos(&imv[2], v2->x + c.x, v2->y + c.y, v2->z + c.z);
                imv[0].u = vShad[0].x;
                imv[1].u = vShad[1].x;
                imv[2].u = vShad[2].x;
                imv[0].v = vShad[0].y;
                imv[1].v = vShad[1].y;
                imv[2].v = vShad[2].y;
                RwIm3DVertexSetRGBA(&imv[0], val, val, val, val);
                RwIm3DVertexSetRGBA(&imv[1], val, val, val, val);
                RwIm3DVertexSetRGBA(&imv[2], val, val, val, val);
                Im3DBufferPos += 3;
            }

            if (Im3DBufferPos)
            {
                if (RwIm3DTransform(Im3DBuffer, Im3DBufferPos, NULL,
                                    rwIM3D_VERTEXUV | rwIM3D_VERTEXXYZ | rwIM3D_VERTEXRGBA))
                {
                    RwIm3DRenderPrimitive(rwPRIMTYPETRILIST);
                    RwIm3DEnd();
                }
                Im3DBufferPos = 0;
            }
        }

        xMemPopTemp(xvert);
        RwRenderStateSet(rwRENDERSTATEDESTBLEND, (void*)rwBLENDINVSRCALPHA);
        RwRenderStateSet(rwRENDERSTATESRCBLEND, (void*)rwBLENDSRCALPHA);
        RwRenderStateSet(rwRENDERSTATEFOGENABLE, &fogstate);
    }
}

// Fit the shadow camera's parallel view round a sphere and back it off along its view direction.
static inline RwCamera* ShadowCameraSetSpherePersp(RwCamera* camera, xVec3* center, F32 radius)
{
    RwV2d vw;
    RwFrame* camFrame;

    RwCameraSetFarClipPlane(camera, 2.0f * radius);
    RwCameraSetNearClipPlane(camera, 0.001f * radius);
    vw.x = radius;
    vw.y = radius;
    RwCameraSetViewWindow(camera, &vw);

    camFrame = (RwFrame*)camera->object.object.parent;
    *(xVec3*)&camFrame->modelling.pos = *center;
    camFrame->modelling.pos.x += camFrame->modelling.at.x * (-0.5f * camera->farPlane);
    camFrame->modelling.pos.y += camFrame->modelling.at.y * (-0.5f * camera->farPlane);
    camFrame->modelling.pos.z += camFrame->modelling.at.z * (-0.5f * camera->farPlane);
    RwMatrixUpdate(&camFrame->modelling);
    RwFrameUpdateObjects(camFrame);
    return camera;
}

static inline void GCSaveFrameBuffer()
{
    fn_80178A3C(gc_saveraster);
}

// Put back the frame buffer saved before the shadow pass.
static inline void GCRestoreFrameBuffer()
{
    F32 recipCamZ = 1.0f / (*(RwCamera**)RwEngineInstance)->farPlane;

    RwRenderStateSet(rwRENDERSTATESRCBLEND, (void*)rwBLENDONE);
    RwRenderStateSet(rwRENDERSTATEDESTBLEND, (void*)rwBLENDZERO);
    RwRenderStateSet(rwRENDERSTATEZTESTENABLE, (void*)FALSE);
    RwRenderStateSet(rwRENDERSTATEZWRITEENABLE, (void*)FALSE);
    RwRenderStateSet(rwRENDERSTATETEXTUREFILTER, (void*)rwFILTERNEAREST);
    RwRenderStateSet(rwRENDERSTATETEXTURERASTER, gc_saveraster);
    Im2DRenderQuad(0.0f, 0.0f, 256.0f, 256.0f, RwIm2DGetFarScreenZ(), recipCamZ, 0.0f);
    RwRenderStateSet(rwRENDERSTATEZTESTENABLE, (void*)TRUE);
    RwRenderStateSet(rwRENDERSTATEZWRITEENABLE, (void*)TRUE);
    RwRenderStateSet(rwRENDERSTATESRCBLEND, (void*)rwBLENDSRCALPHA);
    RwRenderStateSet(rwRENDERSTATEDESTBLEND, (void*)rwBLENDINVSRCALPHA);
}

// Render the model white on clear into the shadow camera's raster, then invert it, keeping the
// main camera's frame buffer intact.
static inline RwCamera* ShadowCameraUpdate(RwCamera* shadowCamera, xModelInstance* model)
{
    RwRGBA bgColor = { 255, 255, 255, 0 };
    S32 fogstate;
    RwCamera* camera;

    camera = *(RwCamera**)RwEngineInstance;
    RwRenderStateGet(rwRENDERSTATEFOGENABLE, &fogstate);
    RwRenderStateSet(rwRENDERSTATEFOGENABLE, (void*)FALSE);
    if (camera)
    {
        RwCameraEndUpdate(camera);
    }
    GCSaveFrameBuffer();
    RwCameraClear(shadowCamera, &bgColor, rwCAMERACLEARIMAGE);
    if (RwCameraBeginUpdate(shadowCamera))
    {
        RwRenderStateSet(rwRENDERSTATETEXTURERASTER, NULL);
        RwRenderStateSet(rwRENDERSTATEZWRITEENABLE, (void*)FALSE);
        RwRenderStateSet(rwRENDERSTATEZTESTENABLE, (void*)FALSE);
        xModelRender(model);
        InvertRaster(shadowCamera);
        RwCameraEndUpdate(shadowCamera);
        fn_80178A3C(shadowCamera->frameBuffer);
    }
    if (camera)
    {
        RwCameraBeginUpdate(camera);
    }
    GCRestoreFrameBuffer();
    RwRenderStateSet(rwRENDERSTATEFOGENABLE, (void*)fogstate);
    return shadowCamera;
}

// Render ent's model into the shadow raster from the light, then cast it onto the world.
// Two scenes tighten the entity/world distance that still counts as one surface near a spot.
void xShadowRender(_xEnt* ent)
{
    xVec3 center;
    F32 radius;
    xModelInstance* model;
    U32 sceneID;
    xVec3* pos;

    zEntGetShadowParams(ent, &center, &radius);

    model = ent->model;
    ShadowCameraSetSpherePersp(ShadowCamera, &center, radius);

    ShadowCameraUpdate(ShadowCamera, model);
    ShadowRenderRaster = ShadowCameraRaster;

    sceneID = globals.sceneCur->sceneID;
    pos = (xVec3*)&globals.player.ent.model->Mat->pos;
    if (sceneID == zSceneIDFromName("I001"))
    {
        xVec3 spot = { -7.0f, 10.0f, -6.0f };
        F32 dx = pos->x - spot.x;
        F32 dy = pos->y - spot.y;
        F32 dz = pos->z - spot.z;

        SHADOW_BOTH = (dx * dx + dy * dy + dz * dz < 16.0f) ? 0.1f : 2.0f;
    }
    else if (sceneID == zSceneIDFromName("I005"))
    {
        xVec3 spot = { -5.0f, 11.0f, -179.0f };
        F32 dx = pos->x - spot.x;
        F32 dy = pos->y - spot.y;
        F32 dz = pos->z - spot.z;

        SHADOW_BOTH = (dx * dx + dy * dy + dz * dz < 1.0f) ? 0.05f : 2.0f;
    }

    xShadowRenderWorld(&center, radius);
}

S32 Im2DRenderQuad(F32 x1, F32 y1, F32 x2, F32 y2, F32 z, F32 recipCamZ, F32 uvOffset)
{
    RwIm2DVertex vx[4];

    vx[0].x = x1;
    vx[0].y = y1;
    vx[0].z = z;
    vx[0].emissiveColor.red = 255;
    vx[0].emissiveColor.green = 255;
    vx[0].emissiveColor.blue = 255;
    vx[0].emissiveColor.alpha = 255;
    vx[0].u = uvOffset;
    vx[0].v = uvOffset;

    vx[1].x = x1;
    vx[1].y = y2;
    vx[1].z = z;
    vx[1].emissiveColor.red = 255;
    vx[1].emissiveColor.green = 255;
    vx[1].emissiveColor.blue = 255;
    vx[1].emissiveColor.alpha = 255;
    vx[1].u = uvOffset;
    vx[1].v = 1.0f + uvOffset;

    vx[2].x = x2;
    vx[2].y = y1;
    vx[2].z = z;
    vx[2].emissiveColor.red = 255;
    vx[2].emissiveColor.green = 255;
    vx[2].emissiveColor.blue = 255;
    vx[2].emissiveColor.alpha = 255;
    vx[2].u = 1.0f + uvOffset;
    vx[2].v = uvOffset;

    vx[3].x = x2;
    vx[3].y = y2;
    vx[3].z = z;
    vx[3].emissiveColor.red = 255;
    vx[3].emissiveColor.green = 255;
    vx[3].emissiveColor.blue = 255;
    vx[3].emissiveColor.alpha = 255;
    vx[3].u = 1.0f + uvOffset;
    vx[3].v = 1.0f + uvOffset;

    RwIm2DRenderPrimitive(rwPRIMTYPETRISTRIP, vx, 4);
    return 1;
}

// Invert the rendered silhouette: the model drew white on clear, and the shadow is its inverse.
void InvertRaster(RwCamera* cam)
{
    RwIm2DVertex vx[4];
    F32 cam_width = (F32)cam->frameBuffer->width;
    F32 cam_height = (F32)cam->frameBuffer->height;

    vx[0].x = 0.0f;
    vx[0].y = 0.0f;
    vx[0].z = RwIm2DGetNearScreenZ();
    vx[0].emissiveColor.red = 255;
    vx[0].emissiveColor.green = 255;
    vx[0].emissiveColor.blue = 255;
    vx[0].emissiveColor.alpha = 255;
    vx[0].u = 0.0f;
    vx[0].v = 0.0f;

    vx[1].x = 0.0f;
    vx[1].y = cam_height;
    vx[1].z = RwIm2DGetNearScreenZ();
    vx[1].emissiveColor.red = 255;
    vx[1].emissiveColor.green = 255;
    vx[1].emissiveColor.blue = 255;
    vx[1].emissiveColor.alpha = 255;
    vx[1].u = 0.0f;
    vx[1].v = 1.0f;

    vx[2].x = cam_width;
    vx[2].y = 0.0f;
    vx[2].z = RwIm2DGetNearScreenZ();
    vx[2].emissiveColor.red = 255;
    vx[2].emissiveColor.green = 255;
    vx[2].emissiveColor.blue = 255;
    vx[2].emissiveColor.alpha = 255;
    vx[2].u = 1.0f;
    vx[2].v = 0.0f;

    vx[3].x = cam_width;
    vx[3].y = cam_height;
    vx[3].z = RwIm2DGetNearScreenZ();
    vx[3].emissiveColor.red = 255;
    vx[3].emissiveColor.green = 255;
    vx[3].emissiveColor.blue = 255;
    vx[3].emissiveColor.alpha = 255;
    vx[3].u = 1.0f;
    vx[3].v = 1.0f;

    RwRenderStateSet(rwRENDERSTATEZTESTENABLE, (void*)FALSE);
    RwRenderStateSet(rwRENDERSTATETEXTURERASTER, NULL);
    RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, (void*)TRUE);
    RwRenderStateSet(rwRENDERSTATEDESTBLEND, (void*)rwBLENDZERO);
    RwRenderStateSet(rwRENDERSTATESRCBLEND, (void*)rwBLENDINVDESTCOLOR);
    RwIm2DRenderPrimitive(rwPRIMTYPETRISTRIP, vx, 4);
    RwRenderStateSet(rwRENDERSTATEZTESTENABLE, (void*)TRUE);
    RwRenderStateSet(rwRENDERSTATEDESTBLEND, (void*)rwBLENDINVSRCALPHA);
    RwRenderStateSet(rwRENDERSTATESRCBLEND, (void*)rwBLENDSRCALPHA);
}

// Called for each world triangle in the shadow zone: drop it when it faces away from the light
// or falls outside the shadow raster, else add it to the batch with its raster coordinates.
static RpCollisionTriangle* ShadowRenderTriangleCB(RpIntersection* intersection,
                                                   RpWorldSector* sector,
                                                   RpCollisionTriangle* collTriangle,
                                                   F32 distance, void* data)
{
    _ProjectionParam* param = (_ProjectionParam*)data;
    RwV3d vIn[3];
    RwV3d vShad[3];
    RxObjSpace3DVertex* imv;
    U8 val;
    F32 ox;
    F32 oy;
    F32 oz;

    if (collTriangle->normal.x * param->at.x + collTriangle->normal.y * param->at.y +
            collTriangle->normal.z * param->at.z >
        SHADOW_BF_DOT)
    {
        return collTriangle;
    }

    vIn[0] = *collTriangle->vertices[0];
    vIn[1] = *collTriangle->vertices[1];
    vIn[2] = *collTriangle->vertices[2];
    RwV3dTransformPoints(vShad, vIn, 3, &param->invMatrix);

    if ((vShad[0].z < 0.0f && vShad[1].z < 0.0f && vShad[2].z < 0.0f) ||
        (vShad[0].x < 0.0f && vShad[1].x < 0.0f && vShad[2].x < 0.0f) ||
        (vShad[0].x > 1.0f && vShad[1].x > 1.0f && vShad[2].x > 1.0f) ||
        (vShad[0].y < 0.0f && vShad[1].y < 0.0f && vShad[2].y < 0.0f) ||
        (vShad[0].y > 1.0f && vShad[1].y > 1.0f && vShad[2].y > 1.0f))
    {
        return collTriangle;
    }

    if (Im3DBufferPos > 477)
    {
        if (RwIm3DTransform(Im3DBuffer, Im3DBufferPos, NULL,
                            rwIM3D_VERTEXUV | rwIM3D_VERTEXXYZ | rwIM3D_VERTEXRGBA))
        {
            RwIm3DRenderPrimitive(rwPRIMTYPETRILIST);
            RwIm3DEnd();
        }
        param->numIm3DBatch++;
        Im3DBufferPos = 0;
    }

    // Lift the triangle a little off the surface along its normal.
    ox = 0.001f * collTriangle->normal.x;
    oy = 0.001f * collTriangle->normal.y;
    oz = 0.001f * collTriangle->normal.z;
    imv = &Im3DBuffer[Im3DBufferPos];
    RwIm3DVertexSetPos(&imv[0], vIn[0].x + ox, vIn[0].y + oy, vIn[0].z + oz);
    RwIm3DVertexSetPos(&imv[1], vIn[1].x + ox, vIn[1].y + oy, vIn[1].z + oz);
    RwIm3DVertexSetPos(&imv[2], vIn[2].x + ox, vIn[2].y + oy, vIn[2].z + oz);
    imv[0].u = vShad[0].x;
    imv[1].u = vShad[1].x;
    imv[2].u = vShad[2].x;
    imv[0].v = vShad[0].y;
    imv[1].v = vShad[1].y;
    imv[2].v = vShad[2].y;
    val = param->shadowValue;
    RwIm3DVertexSetRGBA(&imv[0], val, val, val, val);
    RwIm3DVertexSetRGBA(&imv[1], val, val, val, val);
    RwIm3DVertexSetRGBA(&imv[2], val, val, val, val);
    Im3DBufferPos += 3;
    ShadowNumTriangles++;
    return collTriangle;
}

// Project the shadow camera's raster onto the world triangles inside shadowZone.
static S32 ShadowRender(RwCamera* shadowCamera, RwRaster* shadowRaster, RpWorld* world,
                        RpIntersection* shadowZone, F32 shadowFactor, F32 fadeDist)
{
    _ProjectionParam param;
    RwMatrix* shadowMatrix;
    F32 radius;
    RwV3d scl;
    RwV3d tr;
    RwV3d A;
    RwV3d B;
    S32 fogstate;

    RwRenderStateSet(rwRENDERSTATETEXTURERASTER, shadowCamera->frameBuffer);
    RwRenderStateSet(rwRENDERSTATETEXTUREADDRESS, (void*)rwTEXTUREADDRESSCLAMP);
    RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, (void*)TRUE);
    RwRenderStateSet(rwRENDERSTATESRCBLEND, (void*)rwBLENDZERO);
    if (shadowFactor < 0.0f)
    {
        shadowFactor = -shadowFactor;
        RwRenderStateSet(rwRENDERSTATEDESTBLEND, (void*)rwBLENDSRCCOLOR);
    }
    else
    {
        RwRenderStateSet(rwRENDERSTATEDESTBLEND, (void*)rwBLENDINVSRCCOLOR);
    }

    shadowMatrix = &((RwFrame*)shadowCamera->object.object.parent)->modelling;
    param.at = shadowMatrix->at;
    radius = shadowCamera->viewWindow.x;
    RwMatrixInvert(&param.invMatrix, shadowMatrix);
    scl.x = scl.y = -0.5f / radius;
    scl.z = 1.0f / (fadeDist + radius);
    RwMatrixScale(&param.invMatrix, &scl, rwCOMBINEPOSTCONCAT);

    param.fade = fadeDist > 0.0f ? 1 : 0;
    param.numIm3DBatch = 0;
    Im3DBuffer = gRenderBuffer.m_vertex;
    Im3DBufferPos = 0;
    ShadowNumTriangles = 0;
    param.shadowValue = (U8)(255.0f * shadowFactor);

    A.x = shadowMatrix->pos.x + shadowMatrix->at.x;
    A.y = shadowMatrix->pos.y + shadowMatrix->at.y;
    A.z = shadowMatrix->pos.z + shadowMatrix->at.z;
    RwV3dTransformPoints(&B, &A, 1, &param.invMatrix);
    tr.x = tr.y = 0.5f;
    tr.z = -1.5f * B.z;
    RwMatrixTranslate(&param.invMatrix, &tr, rwCOMBINEPOSTCONCAT);

    RwRenderStateGet(rwRENDERSTATEFOGENABLE, &fogstate);
    RwRenderStateSet(rwRENDERSTATEFOGENABLE, (void*)FALSE);
    RpCollisionWorldForAllIntersections(world, shadowZone, ShadowRenderTriangleCB, &param);
    if (Im3DBufferPos)
    {
        if (RwIm3DTransform(Im3DBuffer, Im3DBufferPos, NULL,
                            rwIM3D_VERTEXUV | rwIM3D_VERTEXXYZ | rwIM3D_VERTEXRGBA))
        {
            RwIm3DRenderPrimitive(rwPRIMTYPETRILIST);
            RwIm3DEnd();
        }
        Im3DBufferPos = 0;
    }
    RwRenderStateSet(rwRENDERSTATEFOGENABLE, &fogstate);
    RwRenderStateSet(rwRENDERSTATEDESTBLEND, (void*)rwBLENDINVSRCALPHA);
    RwRenderStateSet(rwRENDERSTATESRCBLEND, (void*)rwBLENDSRCALPHA);
    return 1;
}
