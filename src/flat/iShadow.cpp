#include "iShadow.h"

#include "iCollide.h"
#include "xDebug.h"
#include "xEnt.h"
#include "xMathInlines.h"
#include "xModel.h"
#include "xString.h"
#include "xstransvc.h"
#include "zGlobals.h"
#include "zScene.h"

#include <rwcore.h>
#include <string.h>

struct RwGcnRenderStateCache
{
    U8 unk0[0x3C];
    U8 alphaRef0;
    U8 alphaRef1;
    U8 unk3E[2];
};

extern "C" RwGcnRenderStateCache lbl_8025E348;
extern "C" void RwGameCubeSetAlphaCompare(S32);

// Blob shadows: each entity's shadow is a quad laid on the ground under it. The quads of a
// frame collect in one vertex list and draw together, darkening what is under them.
static RxRenderStateVector rsv;
static _xRay3 sShadowRay;
static RxObjSpace3DVertex sShadowVert[384];
static RwRaster* sShadowRaster;
static U32 sShadowVertCount;
// Nonzero recomputes every shadow each frame, even for entities that have not moved.
static S32 sShadowForceUpdate;

void iShadowRender();
void iShadowAdd(iShadowCache* cache, U8 alpha);
void iShadowCollide(iShadowCache* cache, _xEnt* ent, F32 radius, F32 ecc);
void iShadowInit();

// Finds the shadow texture, aims the ground ray straight down (10 units long), and sets up the
// vertex list: normals up and texture corners for two triangles per quad.
void iShadowInit()
{
    U32 i;
    RwTexture* tex = (RwTexture*)xSTFindAsset(xStrHash("shadow"), NULL);

    sShadowRaster = tex ? tex->raster : NULL;
    sShadowRay.dir.x = 0.0f;
    sShadowRay.dir.y = -1.0f;
    sShadowRay.dir.z = 0.0f;
    sShadowRay.min_t = 0.0f;
    sShadowRay.max_t = 10.0f;
    sShadowRay.flags = 0xC00;
    sShadowVertCount = 0;
    memset(sShadowVert, 0, sizeof(sShadowVert));

    for (i = 0; i < 384; i += 6)
    {
        sShadowVert[i + 1].u = 1.0f;
        sShadowVert[i + 2].v = 1.0f;
        sShadowVert[i + 3].u = 1.0f;
        sShadowVert[i + 4].v = 1.0f;
        sShadowVert[i + 5].u = 1.0f;
        sShadowVert[i + 5].v = 1.0f;
        sShadowVert[i + 0].ny = 1.0f;
        sShadowVert[i + 1].ny = 1.0f;
        sShadowVert[i + 2].ny = 1.0f;
        sShadowVert[i + 3].ny = 1.0f;
        sShadowVert[i + 4].ny = 1.0f;
        sShadowVert[i + 5].ny = 1.0f;
    }
}

// Fits the entity's shadow quad to the ground below it: casts a ray down from the entity, sets
// the quad's corners just above the hit, and tilts the quad to the ground's slope (at most 30
// degrees). The quad spans twice radius, and ecc stretches it along the entity's right axis; a
// round shadow (ecc 1) only needs refitting when the entity moves, a stretched one also when it
// turns.
void iShadowCollide(iShadowCache* cache, _xEnt* ent, F32 radius, F32 ecc)
{
    F32 height;
    _xCollis crec;
    xVec3 tmppos[4];
    F32 mag;
    F32 dydx;
    F32 dydz;
    F32 centerx;
    F32 centery;
    F32 centerz;

    if (!ent->model)
    {
        return;
    }

    if (!sShadowForceUpdate)
    {
        if (ecc == 1.0f)
        {
            RwMatrix* mat = ent->model->Mat;

            if (cache->pos.x == mat->pos.x && cache->pos.y == mat->pos.y && cache->pos.z == mat->pos.z)
            {
                return;
            }
        }
        else
        {
            RwMatrix* mat = ent->model->Mat;

            if (cache->pos.x == mat->pos.x && cache->pos.y == mat->pos.y && cache->pos.z == mat->pos.z &&
                cache->right.x == mat->right.x && cache->right.y == mat->right.y &&
                cache->right.z == mat->right.z)
            {
                return;
            }
        }
    }

    crec.flags = 0x300;
    sShadowRay.origin = *(xVec3*)&ent->model->Mat->pos;
    cache->pos = sShadowRay.origin;
    cache->right = *(xVec3*)&ent->model->Mat->right;
    iRayHitsEnv(&sShadowRay, globals.sceneCur->env, &crec);

    if (crec.flags & 1)
    {
        height = 0.05f - crec.dist;
    }
    else
    {
        cache->corner[0].x = -1e10f;
        return;
    }

    tmppos[0].x = ecc * radius;
    tmppos[0].y = 0.0f;
    tmppos[0].z = 0.0f;
    tmppos[1].x = 0.0f;
    tmppos[1].y = 0.0f;
    tmppos[1].z = radius;
    tmppos[2].x = 0.0f;
    tmppos[2].y = 0.0f;
    tmppos[2].z = -radius;
    tmppos[3].x = -ecc * radius;
    tmppos[3].y = 0.0f;
    tmppos[3].z = 0.0f;
    RwV3dTransformPoints((RwV3d*)cache->corner, (RwV3d*)tmppos, 4, ent->model->Mat);

    cache->corner[0].y += height;
    cache->corner[1].y += height;
    cache->corner[2].y += height;
    cache->corner[3].y += height;

    if (crec.norm.y < 0.0f)
    {
        crec.norm.x = -crec.norm.x;
        crec.norm.y = -crec.norm.y;
        crec.norm.z = -crec.norm.z;
    }

    // Steeper than 30 degrees: clamp the normal to 30 degrees in the same direction.
    if (crec.norm.y < 0.8660254f)
    {
        mag = crec.norm.x * crec.norm.x + crec.norm.z * crec.norm.z;

        if (mag < 0.000001f)
        {
            xprintf("bonk!\n");
            mag = 0.000001f;
        }

        mag = 0.5f / xsqrt(mag);
        crec.norm.x *= mag;
        crec.norm.y = 0.8660254f;
        crec.norm.z *= mag;
    }

    dydx = -crec.norm.x / crec.norm.y;
    dydz = -crec.norm.z / crec.norm.y;
    centerx = 0.5f * (cache->corner[0].x + cache->corner[3].x);
    centery = 0.5f * (cache->corner[0].y + cache->corner[3].y);
    centerz = 0.5f * (cache->corner[0].z + cache->corner[3].z);
    cache->corner[0].y = centery + dydx * (cache->corner[0].x - centerx) + dydz * (cache->corner[0].z - centerz);
    cache->corner[1].y = centery + dydx * (cache->corner[1].x - centerx) + dydz * (cache->corner[1].z - centerz);
    cache->corner[2].y = centery + dydx * (cache->corner[2].x - centerx) + dydz * (cache->corner[2].z - centerz);
    cache->corner[3].y = centery + dydx * (cache->corner[3].x - centerx) + dydz * (cache->corner[3].z - centerz);
}

// Queues an entity's shadow quad as two triangles at the given alpha. A shadow that found no
// ground, or a full vertex list, adds nothing.
void iShadowAdd(iShadowCache* cache, U8 alpha)
{
    if (cache->corner[0].x == -1e10f)
    {
        return;
    }

    if (sShadowVertCount >= 384)
    {
        return;
    }

    *(xVec3*)&sShadowVert[sShadowVertCount + 0].x = cache->corner[0];
    *(xVec3*)&sShadowVert[sShadowVertCount + 1].x = cache->corner[1];
    *(xVec3*)&sShadowVert[sShadowVertCount + 2].x = cache->corner[2];
    *(xVec3*)&sShadowVert[sShadowVertCount + 3].x = cache->corner[1];
    *(xVec3*)&sShadowVert[sShadowVertCount + 4].x = cache->corner[2];
    *(xVec3*)&sShadowVert[sShadowVertCount + 5].x = cache->corner[3];
    sShadowVert[sShadowVertCount + 5].a = alpha;
    sShadowVert[sShadowVertCount + 4].a = alpha;
    sShadowVert[sShadowVertCount + 3].a = alpha;
    sShadowVert[sShadowVertCount + 2].a = alpha;
    sShadowVert[sShadowVertCount + 1].a = alpha;
    sShadowVert[sShadowVertCount + 0].a = alpha;
    sShadowVertCount += 6;
}

// Draws the frame's shadow quads, then puts back the render states it changed.
void iShadowRender()
{
    if (sShadowVertCount)
    {
        RxRenderStateVectorLoadDriverState(&rsv);
        RwRenderStateSet(rwRENDERSTATESRCBLEND, (void*)rwBLENDINVSRCCOLOR);
        RwRenderStateSet(rwRENDERSTATEDESTBLEND, (void*)rwBLENDDESTCOLOR);
        RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, (void*)TRUE);
        RwRenderStateSet(rwRENDERSTATETEXTURERASTER, (void*)sShadowRaster);
        RwRenderStateSet(rwRENDERSTATESHADEMODE, (void*)rwSHADEMODEGOURAUD);
        RwRenderStateSet(rwRENDERSTATEZWRITEENABLE, (void*)FALSE);
        lbl_8025E348.alphaRef0 = 0;
        lbl_8025E348.alphaRef1 = 0;
        RwGameCubeSetAlphaCompare(1);
        RwIm3DTransform(sShadowVert, sShadowVertCount, NULL,
                        rwIM3D_VERTEXRGBA | rwIM3D_VERTEXXYZ | rwIM3D_VERTEXUV);
        RwIm3DRenderPrimitive(rwPRIMTYPETRILIST);
        RwIm3DEnd();
        RwRenderStateSet(rwRENDERSTATESRCBLEND, (void*)rsv.SrcBlend);
        RwRenderStateSet(rwRENDERSTATEDESTBLEND, (void*)rsv.DestBlend);
        RwRenderStateSet(rwRENDERSTATESHADEMODE, (void*)rsv.ShadeMode);
        // Render state vector flags: 4 is z-write, 8 is vertex alpha.
        RwRenderStateSet(rwRENDERSTATEZWRITEENABLE, (void*)((rsv.Flags >> 2) & 1));
        RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, (void*)((rsv.Flags >> 3) & 1));
        sShadowVertCount = 0;
    }
}
