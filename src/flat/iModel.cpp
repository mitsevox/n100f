#include "iModel.h"
#include "iAnim.h"
#include "xMath.h"
#include "xMath3.h"

#include <math.h>
#include <rphanim.h>
#include <rpskin.h>

#define MAX2(a, b) ((a) >= (b) ? (a) : (b))
#define MAX3(a, b, c) (MAX2((a), MAX2((b), (c))))

#define IMODEL_MAX_ATOMICS 256

static RpAtomic* gLastAtomicList[IMODEL_MAX_ATOMICS];
static U32 gLastAtomicCount;

// Defined in reverse address order: the deferred profile emits the last-defined function first.
static RwFrame* GetChildFrameHierarchy(RwFrame* frame, void* data);
static inline RpHAnimHierarchy* GetHierarchy(RpAtomic* model);
static inline void SkinXform(xVec3* dest, const xVec3* vert, RwMatrix* mat, const RwMatrix* skinmat,
                             const F32* wt, const U32* idx, U32 count);
void iModelTagEval(RpAtomic* model, iModelTag* tag, RwMatrix* mat, xVec3* dest);
U32 iModelTagSetup(iModelTag* tag, RpAtomic* model, F32 x, F32 y, F32 z);
U32 iModelTagIndex(iModelTag* tag, RpAtomic* model, S32 v);
static U32 iModelTagInternal(iModelTag* tag, RpAtomic* model, F32 x, F32 y, F32 z, S32 closeV);
U32 iModelVertEval(RpAtomic* model, U32 index, U32 count, RwMatrix* mat, xVec3* vert, xVec3* dest);
U32 iModelVertCount(RpAtomic* model);
S32 iModelCull(RpAtomic* model, RwMatrix* mat);
void iModelRender(RpAtomic* model, RwMatrix* mat);
void iModelQuatToMat(xQuat* quat, xVec3* tran, RwMatrix* mat);
void iModelAnimMatrices(RpAtomic* model, xQuat* quat, xVec3* tran, RwMatrix* mat);
U32 iModelNumBones(RpAtomic* model);
RpAtomic* iModelFile_RWMultiAtomic(RpAtomic* model);
static RpAtomic* NextAtomicCallback(RpAtomic* atomic, void* data);
void iModelUnload(RpAtomic* userdata);
static RpAtomic* iModelStreamRead(RwStream* stream);
RpAtomic* iModelFileNew(void* buffer, U32 size);
static RpAtomic* FindAtomicCallback(RpAtomic* atomic, void* data);
void iModelInit();

static U32 iModelTagInternal(iModelTag* tag, RpAtomic* model, F32 x, F32 y, F32 z, S32 closeV);

static RpAtomic* NextAtomicCallback(RpAtomic* atomic, void* data);

static RpAtomic* FindAtomicCallback(RpAtomic* atomic, void* data);

void iModelInit()
{
}

static RpAtomic* FindAtomicCallback(RpAtomic* atomic, void* data)
{
    RpHAnimHierarchy* pHier;
    RpSkin* pSkin;

    pHier = GetHierarchy(atomic);
    pSkin = RpSkinGeometryGetSkin(atomic->geometry);

    if (pSkin && !pHier)
    {
        pHier = RpHAnimHierarchyCreate(RpSkinGetNumBones(pSkin), NULL, NULL,
                                       rpHANIMHIERARCHYLOCALSPACEMATRICES, 0x24);
        RpHAnimFrameSetHierarchy(RpAtomicGetFrame(atomic), pHier);
    }

    if (pHier && pSkin)
    {
        RpSkinAtomicSetHAnimHierarchy(atomic, pHier);
    }

    if (pHier)
    {
        pHier->flags = rpHANIMHIERARCHYLOCALSPACEMATRICES;
    }

    if (gLastAtomicCount < IMODEL_MAX_ATOMICS)
    {
        gLastAtomicList[gLastAtomicCount++] = atomic;
    }

    RwFrameGetRoot(RpAtomicGetFrame(atomic));

    return atomic;
}

RpAtomic* iModelFileNew(void* buffer, U32 size)
{
    RwMemory rwmem;

    rwmem.start = (U8*)buffer;
    rwmem.length = size;

    return iModelStreamRead(RwStreamOpen(rwSTREAMMEMORY, rwSTREAMREAD, &rwmem));
}

static RpAtomic* iModelStreamRead(RwStream* stream)
{
    RpClump* clump;

    if (!stream)
    {
        return NULL;
    }

    if (!RwStreamFindChunk(stream, rwID_CLUMP, NULL, NULL))
    {
        RwStreamClose(stream, NULL);
        return NULL;
    }

    clump = RpClumpStreamRead(stream);
    RwStreamClose(stream, NULL);

    if (!clump)
    {
        return NULL;
    }

    gLastAtomicCount = 0;
    RpClumpForAllAtomics(clump, FindAtomicCallback, NULL);

    return gLastAtomicList[0];
}

void iModelUnload(RpAtomic* userdata)
{
    RpClump* clump;
    RwFrame* frame;
    RwFrame* root;

    clump = userdata->clump;
    frame = RpClumpGetFrame(clump);

    if (frame)
    {
        root = RwFrameGetRoot(frame);
        if (root)
        {
            frame = root;
        }
        RwFrameDestroyHierarchy(frame);
        RpClumpSetFrame(clump, NULL);
    }

    if (clump)
    {
        RpClumpDestroy(clump);
    }
}

static RpAtomic* NextAtomicCallback(RpAtomic* atomic, void* data)
{
    RpAtomic** nextModel = (RpAtomic**)data;

    if (*nextModel == atomic)
    {
        *nextModel = NULL;
    }
    else if (*nextModel == NULL)
    {
        *nextModel = atomic;
    }

    return atomic;
}

RpAtomic* iModelFile_RWMultiAtomic(RpAtomic* model)
{
    RpClump* clump;
    RpAtomic* nextModel;

    if (!model)
    {
        return NULL;
    }

    clump = model->clump;
    nextModel = model;
    RpClumpForAllAtomics(clump, NextAtomicCallback, &nextModel);
    return nextModel;
}

U32 iModelNumBones(RpAtomic* model)
{
    RpHAnimHierarchy* hierarchy = GetHierarchy(model);
    return hierarchy == NULL ? 0 : hierarchy->numNodes;
}

void iModelAnimMatrices(RpAtomic* model, xQuat* quat, xVec3* tran, RwMatrix* mat)
{
    RpHAnimHierarchy* pHierarchy;
    RwMatrix matrixStack[32];
    RwMatrix* pMatrixStackTop;
    RpHAnimNodeInfo* pCurrentFrame;
    S32 pCurrentFrameFlags;
    S32 i;
    S32 numFrames;
    RwMatrix* pMatrixArray;

    pHierarchy = GetHierarchy(model);

    if (pHierarchy)
    {
        RwMatrix parentMatrix;

        pMatrixArray = mat;
        RwMatrixSetIdentityMacro(&parentMatrix);
        matrixStack[0] = parentMatrix;
        pMatrixStackTop = &matrixStack[1];

        numFrames = pHierarchy->numNodes;
        pCurrentFrame = pHierarchy->pNodeInfo;
        pCurrentFrameFlags = pCurrentFrame->flags;

        for (i = 0; i < numFrames; i++)
        {
            if (pCurrentFrameFlags & 2)
            {
                *pMatrixStackTop++ = parentMatrix;
            }

            RwMatrix local;
            iModelQuatToMat(quat, tran, &local);

            RwMatrix world;
            RwMatrixMultiply(&world, &local, &parentMatrix);
            *pMatrixArray = world;

            parentMatrix = (pCurrentFrameFlags & 1) ? *--pMatrixStackTop : world;

            pMatrixArray++;
            quat++;
            tran++;
            pCurrentFrame++;
            pCurrentFrameFlags = pCurrentFrame->flags;
        }
    }
}

void iModelQuatToMat(xQuat* quat, xVec3* tran, RwMatrix* mat)
{
    quat->s = -quat->s;
    xQuatToMat(quat, (xMat3x3*)mat);
    quat->s = -quat->s;

    mat->pos.x = tran->x;
    mat->pos.y = tran->y;
    mat->pos.z = tran->z;
}

void iModelRender(RpAtomic* model, RwMatrix* mat)
{
    RpHAnimHierarchy* hierarchy;
    RwMatrix* pAnimOldMatrix;
    RwFrame* frame;

    hierarchy = GetHierarchy(model);

    if (hierarchy)
    {
        pAnimOldMatrix = hierarchy->pMatrixArray;
        hierarchy->pMatrixArray = mat + 1;
    }

    frame = RpAtomicGetFrame(model);
    frame->ltm = *mat;
    RwMatrixUpdate(&frame->ltm);

    model->renderCallBack(model);

    if (hierarchy)
    {
        hierarchy->pMatrixArray = pAnimOldMatrix;
    }
}

S32 iModelCull(RpAtomic* model, RwMatrix* mat)
{
    RwCamera* cam = RwCameraGetCurrentCamera();
    RwSphere sph;
    F32 xScale2, yScale2, zScale2;

    RwV3dTransformPoints(&sph.center, &model->boundingSphere.center, 1, mat);

    xScale2 = mat->right.x * mat->right.x + mat->right.y * mat->right.y + mat->right.z * mat->right.z;
    yScale2 = mat->up.x * mat->up.x + mat->up.y * mat->up.y + mat->up.z * mat->up.z;
    zScale2 = mat->at.x * mat->at.x + mat->at.y * mat->at.y + mat->at.z * mat->at.z;
    sph.radius = model->boundingSphere.radius * sqrtf(MAX3(xScale2, yScale2, zScale2));

    model->worldBoundingSphere = sph;

    return RwCameraFrustumTestSphere(cam, &sph) == rwSPHEREOUTSIDE;
}

U32 iModelVertCount(RpAtomic* model)
{
    return model->geometry->numVertices;
}

U32 iModelVertEval(RpAtomic* model, U32 index, U32 count, RwMatrix* mat, xVec3* vert, xVec3* dest)
{
    RpGeometry* geom;
    U32 numV;
    RpSkin* skin;

    geom = model->geometry;

    if (vert == NULL)
    {
        numV = geom->numVertices;
        if (index >= numV || count == 0)
        {
            return 0;
        }
        count = (count < numV - index) ? count : (numV - index);
        vert = (xVec3*)(geom->morphTarget->verts + index);
    }

    skin = RpSkinGeometryGetSkin(geom);
    if (skin)
    {
        SkinXform(dest, vert, mat, RpSkinGetSkinToBoneMatrices(skin),
                  (const F32*)(RpSkinGetVertexBoneWeights(skin) + index),
                  RpSkinGetVertexBoneIndices(skin) + index, count);
    }
    else
    {
        RwV3dTransformPoints((RwV3d*)dest, (RwV3d*)vert, count, mat);
    }

    return count;
}

static U32 iModelTagInternal(iModelTag* tag, RpAtomic* model, F32 x, F32 y, F32 z, S32 closeV)
{
    RpGeometry* geom;
    RwV3d* vert;
    S32 v;
    S32 numV;
    F32 distSqr;
    F32 closeDistSqr;
    RpSkin* skin;
    const RwMatrixWeights* wt;

    geom = model->geometry;
    vert = geom->morphTarget->verts;
    numV = geom->numVertices;
    closeDistSqr = 1.0e9f;

    if (closeV < 0 || closeV > numV)
    {
        closeV = 0;
        for (v = 0; v < numV; v++)
        {
            distSqr = (vert[v].x - x) * (vert[v].x - x) + (vert[v].y - y) * (vert[v].y - y) +
                      (vert[v].z - z) * (vert[v].z - z);
            if (distSqr < closeDistSqr)
            {
                closeV = v;
                closeDistSqr = distSqr;
            }
        }
        if (tag)
        {
            tag->v.x = x;
            tag->v.y = y;
            tag->v.z = z;
        }
    }
    else
    {
        if (tag)
        {
            tag->v.x = vert[closeV].x;
            tag->v.y = vert[closeV].y;
            tag->v.z = vert[closeV].z;
        }
    }

    if (tag)
    {
        skin = RpSkinGeometryGetSkin(model->geometry);
        if (skin)
        {
            wt = RpSkinGetVertexBoneWeights(skin) + closeV;
            tag->matidx = RpSkinGetVertexBoneIndices(skin)[closeV];
            tag->wt[0] = wt->w0;
            tag->wt[1] = wt->w1;
            tag->wt[2] = wt->w2;
            tag->wt[3] = wt->w3;
        }
        else
        {
            tag->matidx = 0;
            tag->wt[0] = 0.0f;
            tag->wt[1] = 0.0f;
            tag->wt[2] = 0.0f;
            tag->wt[3] = 0.0f;
        }
    }

    return closeV;
}

U32 iModelTagIndex(iModelTag* tag, RpAtomic* model, S32 v)
{
    return iModelTagInternal(tag, model, 0.0f, 0.0f, 0.0f, v);
}

U32 iModelTagSetup(iModelTag* tag, RpAtomic* model, F32 x, F32 y, F32 z)
{
    return iModelTagInternal(tag, model, x, y, z, -1);
}

void iModelTagEval(RpAtomic* model, iModelTag* tag, RwMatrix* mat, xVec3* dest)
{
    if (tag->wt[0])
    {
        SkinXform(dest, &tag->v, mat,
                  RpSkinGetSkinToBoneMatrices(RpSkinGeometryGetSkin(model->geometry)), tag->wt,
                  &tag->matidx, 1);
    }
    else
    {
        RwV3dTransformPoints((RwV3d*)dest, (RwV3d*)&tag->v, 1, mat);
    }
}

static inline void SkinXform(xVec3* dest, const xVec3* vert, RwMatrix* mat, const RwMatrix* skinmat,
                             const F32* wt, const U32* idx, U32 count)
{
    U32 catMatFlags[2] = { 0, 0 };
    RwMatrix* catmat = (RwMatrix*)giAnimScratch;
    RwMatrix* rootmat = mat;
    mat++;

    while (count != 0)
    {
        for (U32 i = 0; i < 4; i++)
        {
            U32 midx = (*idx >> (i << 3)) & 0xFF;
            U32 bit = 1 << (midx & 0x1F);
            if (!(bit & catMatFlags[midx >> 5]))
            {
                xMat4x3Mul((xMat4x3*)(catmat + midx), (const xMat4x3*)(skinmat + midx),
                           (const xMat4x3*)(mat + midx));
                catMatFlags[midx >> 5] |= bit;
            }
        }

        xVec3 accumV;
        accumV.x = accumV.y = accumV.z = 0.0f;

        RwMatrix* pMatrix;
        const F32* fwt = wt;
        U32 wtidx = *idx;
        U32 maxwt = 4;

        while (*fwt && maxwt)
        {
            pMatrix = catmat + (wtidx & 0xFF);

            accumV.x += *fwt * (pMatrix->right.x * vert->x + pMatrix->up.x * vert->y +
                                pMatrix->at.x * vert->z + pMatrix->pos.x);
            accumV.y += *fwt * (pMatrix->right.y * vert->x + pMatrix->up.y * vert->y +
                                pMatrix->at.y * vert->z + pMatrix->pos.y);
            accumV.z += *fwt * (pMatrix->right.z * vert->x + pMatrix->up.z * vert->y +
                                pMatrix->at.z * vert->z + pMatrix->pos.z);

            wtidx >>= 8;
            fwt++;
            maxwt--;
        }

        dest->x = rootmat->right.x * accumV.x + rootmat->up.x * accumV.y +
                  rootmat->at.x * accumV.z + rootmat->pos.x;
        dest->y = rootmat->right.y * accumV.x + rootmat->up.y * accumV.y +
                  rootmat->at.y * accumV.z + rootmat->pos.y;
        dest->z = rootmat->right.z * accumV.x + rootmat->up.z * accumV.y +
                  rootmat->at.z * accumV.z + rootmat->pos.z;

        vert++;
        idx++;
        wt += 4;
        count--;
        dest++;
    }
}

static inline RpHAnimHierarchy* GetHierarchy(RpAtomic* model)
{
    RpHAnimHierarchy* hierarchy = NULL;
    GetChildFrameHierarchy(RpAtomicGetFrame(model), &hierarchy);
    return hierarchy;
}

static RwFrame* GetChildFrameHierarchy(RwFrame* frame, void* data)
{
    RpHAnimHierarchy* hierarchy = RpHAnimFrameGetHierarchy(frame);

    if (hierarchy == NULL)
    {
        RwFrameForAllChildren(frame, GetChildFrameHierarchy, data);
        return frame;
    }

    *(RpHAnimHierarchy**)data = hierarchy;
    return NULL;
}
