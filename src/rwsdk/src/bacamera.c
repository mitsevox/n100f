#include <rwcore.h>

// The standard device functions the camera calls.
#define STDFUNC_CAMERABEGINUPDATE 1
#define STDFUNC_CAMERAENDUPDATE 10
#define STDFUNC_CAMERACLEAR 21

// The object type of a camera.
#define CAMERA_OBJECTTYPE 4

// The default view window, clip planes and fog plane of a new camera.
#define CAMERA_DEFAULTVIEWWINDOW 1.0f
#define CAMERA_DEFAULTNEARPLANE 0.05f
#define CAMERA_DEFAULTFARPLANE 10.0f
#define CAMERA_DEFAULTFOGPLANE 5.0f

// The share of the device's Z range kept clear at each end.
#define CAMERA_ZRANGEMARGIN 0.0001f

// The camera module's data in the engine instance.
typedef struct CameraModuleData CameraModuleData;
struct CameraModuleData
{
    RwFreeList* freeList;
};

#define CAMERA_DATA(var) \
    (((CameraModuleData*)((U8*)RwEngineInstance + cameraModule.globalsOffset))->var)

RwPluginRegistry cameraTKList = { sizeof(RwCamera), sizeof(RwCamera) };
static RwModuleInfo cameraModule;

RwRaster* RwRasterShowRaster(RwRaster* raster, void* dev, U32 flags);
void _rwPipeInitForCamera(const RwCamera* camera);

void CameraBuildPerspClipPlanes(RwCamera* camera);
void CameraBuildPerspViewMatrix(RwCamera* camera);
void CameraBuildParallelClipPlanes(RwCamera* camera);
void CameraBuildParallelViewMatrix(RwCamera* camera);

// Maps the camera's near and far planes onto the device's Z range.
static void rwCameraUpdateZShiftScale(RwCamera* camera)
{
    F32 nearScreenZ = RwIm2DGetNearScreenZ();
    F32 farScreenZ = RwIm2DGetFarScreenZ();
    F32 zMargin;
    F32 nearZ;
    F32 farZ;
    F32 zScale;

    switch (camera->projectionType)
    {
    case rwPARALLEL:
        farZ = camera->farPlane;
        nearZ = camera->nearPlane;
        break;
    case rwPERSPECTIVE:
    default:
        farZ = 1.0f / camera->farPlane;
        nearZ = 1.0f / camera->nearPlane;
        break;
    }

    zMargin = CAMERA_ZRANGEMARGIN * (farScreenZ - nearScreenZ);
    farScreenZ -= zMargin;
    nearScreenZ += zMargin;

    zScale = (farScreenZ - nearScreenZ) / (farZ - nearZ);
    camera->zScale = zScale;
    camera->zShift = 0.5f * ((farScreenZ + nearScreenZ) - zScale * (farZ + nearZ));
}

static RwObjectHasFrame* CameraSync(RwObjectHasFrame* object)
{
    RwCamera* camera = (RwCamera*)object;

    if (camera->projectionType == rwPERSPECTIVE)
    {
        CameraBuildPerspViewMatrix(camera);
        CameraBuildPerspClipPlanes(camera);
    }
    else
    {
        CameraBuildParallelViewMatrix(camera);
        CameraBuildParallelClipPlanes(camera);
    }

    RwBBoxCalculate(&camera->frustumBoundBox, camera->frustumCorners, 8);

    return object;
}

static RwCamera* CameraEndUpdate(RwCamera* camera)
{
    if (RWSRCGLOBAL(stdFunc)[STDFUNC_CAMERAENDUPDATE](NULL, camera, 0))
    {
        RWSRCGLOBAL(curCamera) = NULL;
        return camera;
    }

    return NULL;
}

static RwCamera* CameraBeginUpdate(RwCamera* camera)
{
    RWSRCGLOBAL(curCamera) = camera;
    _rwFrameSyncDirty();

    if (RWSRCGLOBAL(stdFunc)[STDFUNC_CAMERABEGINUPDATE](NULL, camera, 0))
    {
        _rwPipeInitForCamera(camera);
        return camera;
    }

    return NULL;
}

void* _rwCameraClose(void* instance, S32 offset, S32 size)
{
    if (CAMERA_DATA(freeList))
    {
        RwFreeListDestroy(CAMERA_DATA(freeList));
        CAMERA_DATA(freeList) = NULL;
    }

    cameraModule.numInstances--;

    return instance;
}

void* _rwCameraOpen(void* instance, S32 offset, S32 size)
{
    cameraModule.globalsOffset = offset;
    CAMERA_DATA(freeList) = RwFreeListCreate(cameraTKList.sizeOfStruct, 4, 4);

    if (!CAMERA_DATA(freeList))
    {
        return NULL;
    }

    cameraModule.numInstances++;

    return instance;
}

RwCamera* RwCameraEndUpdate(RwCamera* camera)
{
    return camera->endUpdate(camera);
}

RwCamera* RwCameraBeginUpdate(RwCamera* camera)
{
    return camera->beginUpdate(camera);
}

RwCamera* RwCameraSetViewOffset(RwCamera* camera, const RwV2d* offset)
{
    RwFrame* frame;

    camera->viewOffset = *offset;

    frame = RwCameraGetFrame(camera);
    if (frame)
    {
        RwFrameUpdateObjects(frame);
    }

    return camera;
}

RwCamera* RwCameraSetNearClipPlane(RwCamera* camera, F32 nearClip)
{
    RwFrame* frame;

    camera->nearPlane = nearClip;
    rwCameraUpdateZShiftScale(camera);

    frame = RwCameraGetFrame(camera);
    if (frame)
    {
        RwFrameUpdateObjects(frame);
    }

    return camera;
}

RwCamera* RwCameraSetFarClipPlane(RwCamera* camera, F32 farClip)
{
    RwFrame* frame;

    camera->farPlane = farClip;
    rwCameraUpdateZShiftScale(camera);

    frame = RwCameraGetFrame(camera);
    if (frame)
    {
        RwFrameUpdateObjects(frame);
    }

    return camera;
}

RwFrustumTestResult RwCameraFrustumTestSphere(const RwCamera* camera, const RwSphere* sphere)
{
    RwFrustumTestResult result = rwSPHEREINSIDE;
    const RwFrustumPlane* frustumPlane = camera->frustumPlanes;
    S32 numPlanes = 6;

    while (numPlanes--)
    {
        F32 dot = RwV3dDotProductMacro(&sphere->center, &frustumPlane->plane.normal) -
                  frustumPlane->plane.distance;

        if (dot > sphere->radius)
        {
            return rwSPHEREOUTSIDE;
        }

        if (dot > -sphere->radius)
        {
            result = rwSPHEREBOUNDARY;
        }

        frustumPlane++;
    }

    return result;
}

RwCamera* RwCameraClear(RwCamera* camera, RwRGBA* colour, S32 clearMode)
{
    if (RWSRCGLOBAL(stdFunc)[STDFUNC_CAMERACLEAR](camera, colour, clearMode))
    {
        return camera;
    }

    return NULL;
}

RwCamera* RwCameraShowRaster(RwCamera* camera, void* pDev, U32 flags)
{
    if (RwRasterShowRaster(camera->frameBuffer, pDev, flags))
    {
        return camera;
    }

    return NULL;
}

RwCamera* RwCameraSetProjection(RwCamera* camera, RwCameraProjection projection)
{
    switch (projection)
    {
    case rwPERSPECTIVE:
    case rwPARALLEL:
    {
        RwFrame* frame;

        camera->projectionType = projection;

        frame = RwCameraGetFrame(camera);
        if (frame)
        {
            RwFrameUpdateObjects(frame);
        }

        rwCameraUpdateZShiftScale(camera);

        return camera;
    }
    default:
    {
        RwError err;
        err.pluginID = 1;
        err.errorCode = _rwerror(0x80000003, "Invalid projection type specified");
        RwErrorSet(&err);
        return NULL;
    }
    }
}

RwCamera* RwCameraSetViewWindow(RwCamera* camera, const RwV2d* viewWindow)
{
    RwFrame* frame;

    camera->viewWindow = *viewWindow;
    camera->recipViewWindow.x = 1.0f / camera->viewWindow.x;
    camera->recipViewWindow.y = 1.0f / camera->viewWindow.y;

    frame = RwCameraGetFrame(camera);
    if (frame)
    {
        RwFrameUpdateObjects(frame);
    }

    return camera;
}

S32 RwCameraRegisterPlugin(S32 size, U32 pluginID, RwPluginObjectConstructor constructCB,
                           RwPluginObjectDestructor destructCB, RwPluginObjectCopy copyCB)
{
    return _rwPluginRegistryAddPlugin(&cameraTKList, size, pluginID, constructCB, destructCB,
                                      copyCB);
}

S32 RwCameraDestroy(RwCamera* camera)
{
    _rwPluginRegistryDeInitObject(&cameraTKList, camera);
    _rwObjectHasFrameReleaseFrame(camera);
    RWSRCGLOBAL(memoryFree)(CAMERA_DATA(freeList), camera);

    return 1;
}

RwCamera* RwCameraCreate()
{
    RwCamera* camera = (RwCamera*)RWSRCGLOBAL(memoryAlloc)(CAMERA_DATA(freeList));

    if (!camera)
    {
        return NULL;
    }

    rwObjectInitialize(camera, CAMERA_OBJECTTYPE, 0);
    camera->object.sync = CameraSync;
    camera->beginUpdate = CameraBeginUpdate;
    camera->endUpdate = CameraEndUpdate;

    camera->viewWindow.x = camera->viewWindow.y = CAMERA_DEFAULTVIEWWINDOW;
    camera->recipViewWindow.x = camera->recipViewWindow.y = CAMERA_DEFAULTVIEWWINDOW;
    camera->viewOffset.x = camera->viewOffset.y = 0.0f;
    camera->nearPlane = CAMERA_DEFAULTNEARPLANE;
    camera->farPlane = CAMERA_DEFAULTFARPLANE;
    camera->fogPlane = CAMERA_DEFAULTFOGPLANE;
    camera->frameBuffer = NULL;
    camera->zBuffer = NULL;
    camera->projectionType = rwPERSPECTIVE;

    rwCameraUpdateZShiftScale(camera);

    camera->viewMatrix.flags = 0;

    _rwPluginRegistryInitObject(&cameraTKList, camera);

    return camera;
}
