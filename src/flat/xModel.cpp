#include "xModel.h"

#include "iCamera.h"
#include "iModel.h"
#include "xAnim.h"
#include "xFX.h"
#include "xMath.h"
#include "xMath2.h"
#include "xMemMgr.h"
#include "xMorph.h"
#include "zGlobals.h"

#include <rpworld.h>
#include <string.h>

static xModelPool* sxModelPoolList;
static RwCamera* subcamera;

static RwCamera* CameraCreate(S32 a, S32 b, S32 c);
static void CameraDestroy(RwCamera* camera);

// Initialise the model system and create the spare camera used for 2D model rendering.
void xModelInit()
{
    iModelInit();

    sxModelPoolList = NULL;

    if (!subcamera)
    {
        subcamera = CameraCreate(0, 0, 1);
    }
}

// Allocate a pool of count model instances with numMatrices matrices each, and add it to the
// pool list, which is kept sorted by matrix count, largest first.
void xModelPoolInit(U32 count, U32 numMatrices)
{
    S32 i;
    U8* buffer;
    RwMatrix* mat;
    xModelPool* pool;
    xModelPool* curr;
    xModelPool** prev;
    xModelInstance* inst;

    if (numMatrices < 1)
    {
        numMatrices = 1;
    }

    buffer = (U8*)(((U32)xMemAlloc(gActiveHeap, count * numMatrices * sizeof(RwMatrix) +
                                                    sizeof(xModelPool) +
                                                    count * sizeof(xModelInstance) + 15) +
                    15) &
                   ~15);

    mat = (RwMatrix*)buffer;
    buffer += count * numMatrices * sizeof(RwMatrix);

    pool = (xModelPool*)buffer;
    buffer += sizeof(xModelPool);

    inst = (xModelInstance*)buffer;

    for (i = 0; i < (S32)count; i++)
    {
        inst[i].Next = &inst[i + 1];
        inst[i].Pool = pool;
        inst[i].Mat = mat;
        mat += numMatrices;
    }

    inst[count - 1].Next = NULL;

    pool->NumMatrices = numMatrices;
    pool->List = inst;

    curr = sxModelPoolList;
    prev = &sxModelPoolList;

    while (curr && numMatrices < curr->NumMatrices)
    {
        prev = &curr->Next;
        curr = curr->Next;
    }

    pool->Next = curr;
    *prev = pool;
}

// Take a model instance for an atomic from the smallest-matrix pool that still fits it.
xModelInstance* xModelInstanceAlloc(RpAtomic* data, void* object, U16 flags, U8 boneIndex,
                                    U8* boneRemap)
{
    S32 i;
    U32 boneCount;
    U32 matCount;
    xModelPool* curr;
    xModelPool* found;
    xModelInstance* dude;

    found = NULL;
    boneCount = iModelNumBones(data);
    matCount = 1 + boneCount + ((flags >> 6) & 0x1);

    curr = sxModelPoolList;

    while (curr)
    {
        if (curr->List && matCount <= curr->NumMatrices)
        {
            found = curr;
        }

        curr = curr->Next;
    }

    dude = found->List;
    found->List = dude->Next;

    dude->Next = NULL;
    dude->Parent = NULL;
    dude->Anim = NULL;
    dude->Data = data;
    dude->Object = object;
    dude->Flags = flags | 0x3;
    dude->BoneCount = boneCount;
    dude->BoneIndex = boneIndex;
    dude->BoneRemap = boneRemap;
    dude->Scale.x = 0.0f;
    dude->Scale.y = 0.0f;
    dude->Scale.z = 0.0f;

    for (i = 0; i < (S32)matCount; i++)
    {
        xMat4x3Identity((xMat4x3*)&dude->Mat[i]);
    }

    return dude;
}

// Find the first instance after this one in its chain whose parent is this model.
static xModelInstance* FindChild(xModelInstance* modelInst)
{
    xModelInstance* child = modelInst->Next;

    while (child)
    {
        if (child->Parent == modelInst)
        {
            return child;
        }

        child = child->Next;
    }

    return NULL;
}

// Free a model instance and all of its children, and return them to their pools.
void xModelInstanceFree(xModelInstance* modelInst)
{
    xModelInstance* child;
    xModelInstance* curr;
    xModelInstance** prev;

    while (child = FindChild(modelInst))
    {
        xModelInstanceFree(child);
    }

    if (modelInst->Parent)
    {
        prev = &modelInst->Parent->Next;
        curr = *prev;

        while (curr && curr != modelInst)
        {
            prev = &curr->Next;
            curr = curr->Next;
        }

        *prev = curr->Next;
    }

    if (modelInst->Anim)
    {
        xAnimPoolFree(modelInst->Anim);
        modelInst->Anim = NULL;
    }

    modelInst->Next = modelInst->Pool->List;
    modelInst->Pool->List = modelInst;
}

// Attach a model instance to the end of a parent's chain.
void xModelInstanceAttach(xModelInstance* inst, xModelInstance* parent)
{
    xModelInstance* curr = parent;

    while (curr->Next)
    {
        curr = curr->Next;
    }

    curr->Next = inst;
    inst->Parent = parent;
}

// Advance the animation of every instance in a chain.
void xModelUpdate(xModelInstance* modelInst, F32 timeDelta)
{
    for (; modelInst; modelInst = modelInst->Next)
    {
        if (modelInst->Anim && (modelInst->Flags & 0x4))
        {
            if (modelInst->Flags & 0x100)
            {
                xAnimPlayChooseTransition(modelInst->Anim);
            }

            xAnimPlayUpdate(modelInst->Anim, timeDelta);
        }
    }
}

// Evaluate one instance's bones, then build its matrices from its parent's: a full copy, a
// remapped copy, or the parent's bone matrix with an optional local offset.
void xModelEvalSingle(xModelInstance* modelInst)
{
    S32 i;
    xModelInstance* dad;
    U8* remap;
    U16 flags = modelInst->Flags;

    if (flags & 0x2)
    {
        if (modelInst->Anim)
        {
            xAnimPlayEval(modelInst->Anim);
        }

        dad = modelInst->Parent;

        if (dad)
        {
            if (flags & 0x8)
            {
                memcpy(modelInst->Mat, dad->Mat, (modelInst->BoneCount + 1) * sizeof(RwMatrix));
            }
            else if (flags & 0x10)
            {
                remap = modelInst->BoneRemap;
                memcpy(modelInst->Mat, dad->Mat, sizeof(RwMatrix));

                for (i = 0; i < modelInst->BoneCount; i++)
                {
                    memcpy(&modelInst->Mat[i + 1], &dad->Mat[remap[i] + 1], sizeof(RwMatrix));
                }
            }
            else if (flags & 0x20)
            {
                if (dad->BoneCount && modelInst->BoneIndex)
                {
                    xMat4x3Mul((xMat4x3*)modelInst->Mat,
                               (xMat4x3*)&dad->Mat[modelInst->BoneIndex + 1], (xMat4x3*)dad->Mat);
                }
                else
                {
                    memcpy(modelInst->Mat, dad->Mat, sizeof(RwMatrix));
                }

                if (flags & 0x40)
                {
                    xMat4x3Mul((xMat4x3*)modelInst->Mat,
                               (xMat4x3*)&modelInst->Mat[modelInst->BoneCount + 1],
                               (xMat4x3*)modelInst->Mat);
                }
            }
        }
    }
}

// Evaluate the animation and matrices of a model and its attached children.
void xModelEval(xModelInstance* modelInst)
{
    while (modelInst)
    {
        xModelEvalSingle(modelInst);
        modelInst = modelInst->Next;
    }
}

// Draw one instance, scaled by its Scale when set: a morph target sequence when the instance
// plays one, otherwise the atomic.
void xModelRenderSingle(xModelInstance* modelInst)
{
    if (!(modelInst->Flags & 0x1))
    {
        return;
    }

    xMat3x3 tmpmat;

    if (modelInst->Scale.x != 0.0f)
    {
        tmpmat = *(xMat3x3*)modelInst->Mat;

        modelInst->Mat->right.x *= modelInst->Scale.x;
        modelInst->Mat->right.y *= modelInst->Scale.x;
        modelInst->Mat->right.z *= modelInst->Scale.x;
        modelInst->Mat->up.x *= modelInst->Scale.y;
        modelInst->Mat->up.y *= modelInst->Scale.y;
        modelInst->Mat->up.z *= modelInst->Scale.y;
        modelInst->Mat->at.x *= modelInst->Scale.z;
        modelInst->Mat->at.y *= modelInst->Scale.z;
        modelInst->Mat->at.z *= modelInst->Scale.z;
    }

    if (!iModelCull(modelInst->Data, modelInst->Mat))
    {
        if (modelInst->Flags & 0x80)
        {
            xAnimPlay* a = modelInst->Anim;

            for (U16 i = 0; i < a->NumSingle; i++)
            {
                if (a->Single[i].SingleFlags & 0x8000)
                {
                    if (a->Single[i].State)
                    {
                        xMorphRender((xMorphSeqFile*)a->Single[i].State->Data->RawData[0],
                                     modelInst->Mat,
                                     xAnimFileRawTime(a->Single[i].State->Data, a->Single[i].Time));
                    }

                    break;
                }
            }
        }
        else
        {
            iModelRender(modelInst->Data, modelInst->Mat);
        }
    }

    if (modelInst->Scale.x)
    {
        *(xMat3x3*)modelInst->Mat = tmpmat;
    }
}

// Render a model and its attached children.
void xModelRender(xModelInstance* modelInst)
{
    while (modelInst)
    {
        xModelRenderSingle(modelInst);
        modelInst = modelInst->Next;
    }
}

static U32 sRender2DStyle;
static S32 sRender2DVisible;

// Choose the framing for the next xModelRender2D call; a fully transparent model is skipped.
void xModelSetRender2DStyle(U32 renderStyle, F32 alpha)
{
    sRender2DStyle = renderStyle;
    sRender2DVisible = (alpha > 0.0f) ? TRUE : FALSE;
}

// Draw a model into a screen rectangle (640x480 units) through the current camera. The render
// style picks the model's distance, its scale on screen and its turn.
void xModelRender2D(xModelInstance* modelInst, _xRect* sr, xVec3* from, xVec3* to, F32 fov)
{
    F32 dist = 0.0f;
    F32 scalex = 1.0f;
    F32 scaley = 1.0f;
    F32 yaw = PI;

    if (!sRender2DVisible)
    {
        return;
    }

    switch (sRender2DStyle)
    {
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
        dist = 6.3001f;
        scalex = 0.4893799f;
        scaley = 0.3772403f;
        yaw = 3.5f;
        break;
    case 9:
        dist = 5.750003f;
        scalex = 0.8769528f;
        scaley = 0.3797596f;
        yaw = 0.0f;
        break;
    case 7:
        dist = 11.874952f;
        scalex = 0.4462602f;
        scaley = 0.3378909f;
        break;
    case 8:
        dist = 19.99983f;
        scalex = 0.3664008f;
        scaley = 0.4427503f;
        break;
    case 14:
        dist = 33.12471f;
        scalex = 0.4910994f;
        scaley = 0.3686507f;
        break;
    case 11:
        dist = 5.0f;
        scalex = 0.6303097f;
        scaley = 0.50421f;
        break;
    case 12:
        dist = 55.626087f;
        scalex = 0.4766497f;
        scaley = 0.3861006f;
        break;
    case 13:
        dist = 9.899982f;
        scalex = 0.7654892f;
        scaley = 0.3862704f;
        break;
    case 10:
    case 15:
        break;
    }

    sRender2DStyle = 0;

    RwCamera* camera = RwCameraGetCurrentCamera();

    if (!(sr->w <= 0.0f || sr->h <= 0.0f || sr->x < -sr->w || sr->y < -sr->h ||
          sr->x > 640.0f + sr->w || sr->y > 480.0f + sr->y))
    {
        if (camera)
        {
            RwCameraEndUpdate(camera);
        }

        xMat4x3 cammat;
        xMat4x3Identity(&cammat);

        xMat4x3* mat = (xMat4x3*)modelInst->Mat;
        mat->pos.x = 0.0f;
        mat->pos.y = 0.0f;
        mat->pos.z = dist;

        if (yaw != 0.0f)
        {
            xMat3x3Euler((xMat3x3*)mat, yaw, 0.0f, 0.0f);
        }

        iCameraUpdatePos(camera, &cammat);

        RwVideoMode vm;
        RwEngineGetVideoModeInfo(&vm, RwEngineGetCurrentVideoMode());

        RwV2d offset;
        RwV2d vw;

        if (vm.flags & rwVIDEOMODEEXCLUSIVE)
        {
            vw.x = 0.5f;
            vw.y = 0.375f;
        }
        else
        {
            vw.x = 0.5f;
            vw.y = 0.375f;
        }

        RwCameraSetViewWindow(camera, &vw);

        F32 dy = sr->y - 240.0f;
        F32 dx = sr->x - 320.0f;
        offset.x = dx / 320.0f * scalex;
        offset.y = -(dy / 240.0f) * scaley;
        RwCameraSetViewOffset(camera, &offset);

        RwCameraBeginUpdate(camera);
        xModelRender(modelInst);
        RwCameraEndUpdate(camera);

        offset.x = 0.0f;
        offset.y = 0.0f;
        RwCameraSetViewOffset(camera, &offset);

        if (camera)
        {
            RwCameraBeginUpdate(camera);
        }

        iCameraUpdatePos(globals.camera.lo_cam, &globals.camera.hi_cam.mat);
        iCameraSetFOV(globals.camera.lo_cam, globals.camera.hi_cam.fov);
    }
}

// Create a camera with its own frame, frame buffer and (when c is set) z buffer.
static inline RwCamera* CameraCreate(S32 a, S32 b, S32 c)
{
    RwCamera* camera = RwCameraCreate();

    if (camera)
    {
        _rwObjectHasFrameSetFrame(camera, RwFrameCreate());
        camera->frameBuffer = RwRasterCreate(a, b, 0, 2);

        if (c)
        {
            camera->zBuffer = RwRasterCreate(a, b, 0, 1);
        }

        if (RwCameraGetFrame(camera) && camera->frameBuffer && (!c || camera->zBuffer))
        {
            return camera;
        }
    }

    CameraDestroy(camera);
    return NULL;
}

// Destroy a camera with its frame and rasters.
static void CameraDestroy(RwCamera* camera)
{
    RwFrame* frame;
    RwRaster* raster;

    if (camera)
    {
        _rwFrameSyncDirty();

        frame = RwCameraGetFrame(camera);

        if (frame)
        {
            _rwObjectHasFrameSetFrame(camera, NULL);
            RwFrameDestroy(frame);
        }

        raster = camera->frameBuffer;

        if (raster)
        {
            RwRasterDestroy(raster);
            camera->frameBuffer = NULL;
        }

        raster = camera->zBuffer;

        if (raster)
        {
            RwRasterDestroy(raster);
            camera->zBuffer = NULL;
        }

        RwCameraDestroy(camera);
    }
}

// Swap every material texture of a model for another.
void xModelSetMaterialTexture(xModelInstance* modelInst, void* texture)
{
    iModelSetMaterialTexture(modelInst->Data, texture);
}

// Put back the textures saved by xModelSetMaterialTexture.
void xModelSetMaterialTextureRestore(xModelInstance* modelInst)
{
    iModelSetMaterialTextureRestore(modelInst->Data);
}

// Set the alpha of every material of a model.
void xModelSetMaterialAlpha(xModelInstance* modelInst, S32 alpha)
{
    iModelSetMaterialAlpha(modelInst->Data, alpha);
}

// Scale the colour of every material of a model.
void xModelMaterialMul(xModelInstance* modelInst, F32 rm, F32 gm, F32 bm)
{
    iModelMaterialMul(modelInst->Data, rm, gm, bm);
}

// Put back the colours saved by xModelMaterialMul.
void xModelResetMaterial(xModelInstance* modelInst)
{
    iModelResetMaterial(modelInst->Data);
}

// Add the model sub-camera to the scene's world.
void xModel_SceneEnter(RpWorld* world)
{
    RpWorldAddCamera(world, subcamera);
}

// Remove the model sub-camera from the world.
void xModel_SceneExit(RpWorld* world)
{
    RpWorldRemoveCamera(world, subcamera);
}
