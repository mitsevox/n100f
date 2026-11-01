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
RwCamera* CameraBuildPerspViewMatrix(RwCamera* camera);
void CameraBuildParallelClipPlanes(RwCamera* camera);
RwCamera* CameraBuildParallelViewMatrix(RwCamera* camera);

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

// Fills a frustum plane's closest-corner flags: 1 for each axis along which the normal is positive,
// 0 where it is negative, read from the sign bit.
#define CAMERA_PLANESIGN(_value) ((U8)((*(const S32*)&(_value) >> 31) + 1))

#define CameraSetPlaneClosest(_plane)                                                              \
    do                                                                                             \
    {                                                                                              \
        (_plane)->closestX = CAMERA_PLANESIGN((_plane)->plane.normal.x);                           \
        (_plane)->closestY = CAMERA_PLANESIGN((_plane)->plane.normal.y);                           \
        (_plane)->closestZ = CAMERA_PLANESIGN((_plane)->plane.normal.z);                           \
    } while (0)

#define CameraNormalizePlane(_plane)                                                               \
    do                                                                                             \
    {                                                                                              \
        F32 recip = _rwInvSqrt(                                                                    \
            RwV3dDotProductMacro(&(_plane)->plane.normal, &(_plane)->plane.normal));               \
        RwV3dScaleMacro(&(_plane)->plane.normal, &(_plane)->plane.normal, recip);                  \
    } while (0)

// Builds the perspective frustum: the eight corners (near then far, each starting top right in
// the window's terms and going round) in world space, then the far and near planes along the
// camera's at vector and the four side planes through the corners, with normals pointing out.
void CameraBuildPerspClipPlanes(RwCamera* camera)
{
    RwMatrix* camMatrix = &RwCameraGetFrame(camera)->ltm;
    RwV3d* corners = camera->frustumCorners;
    RwFrustumPlane* plane;
    RwV3d vector;
    RwV3d xWindow;
    RwV3d yWindow;
    RwV3d xWindow2;
    RwV3d offset;
    RwV3d edge;
    S32 i;

    RwV3dScaleMacro(&offset, &camMatrix->right, -camera->viewOffset.x);
    RwV3dIncrementScaledMacro(&offset, &camMatrix->up, camera->viewOffset.y);
    RwV3dScaleMacro(&xWindow, &camMatrix->right, camera->viewWindow.x);
    RwV3dScaleMacro(&yWindow, &camMatrix->up, camera->viewWindow.y);
    RwV3dScaleMacro(&xWindow2, &xWindow, 2.0f);

    RwV3dAddMacro(&vector, &camMatrix->at, &xWindow);
    RwV3dAddMacro(&vector, &vector, &yWindow);
    corners[0] = vector;
    RwV3dSubMacro(&vector, &vector, &xWindow2);
    corners[1] = vector;
    vector.x -= 2.0f * yWindow.x;
    vector.y -= 2.0f * yWindow.y;
    vector.z -= 2.0f * yWindow.z;
    corners[2] = vector;
    RwV3dAddMacro(&vector, &vector, &xWindow2);
    corners[3] = vector;

    for (i = 0; i < 4; i++)
    {
        RwV3dSubMacro(&vector, &corners[i], &offset);
        RwV3dAddMacro(&corners[i], &offset, &camMatrix->pos);
        RwV3dIncrementScaledMacro(&corners[i], &vector, camera->nearPlane);
        RwV3dAddMacro(&corners[i + 4], &offset, &camMatrix->pos);
        RwV3dIncrementScaledMacro(&corners[i + 4], &vector, camera->farPlane);
    }

    plane = camera->frustumPlanes;

    plane->plane.normal = camMatrix->at;
    plane->plane.distance = RwV3dDotProductMacro(&corners[4], &camMatrix->at);
    CameraSetPlaneClosest(plane);
    plane++;

    plane->plane.normal.x = -camera->frustumPlanes[0].plane.normal.x;
    plane->plane.normal.y = -camera->frustumPlanes[0].plane.normal.y;
    plane->plane.normal.z = -camera->frustumPlanes[0].plane.normal.z;
    plane->plane.distance = RwV3dDotProductMacro(&corners[0], &plane->plane.normal);
    CameraSetPlaneClosest(plane);
    plane++;

    RwV3dSubMacro(&vector, &corners[1], &corners[5]);
    RwV3dSubMacro(&edge, &corners[6], &corners[5]);
    RwV3dCrossProductMacro(&plane->plane.normal, &vector, &edge);
    CameraNormalizePlane(plane);
    plane->plane.distance = RwV3dDotProductMacro(&corners[1], &plane->plane.normal);
    CameraSetPlaneClosest(plane);
    plane++;

    RwV3dSubMacro(&edge, &corners[4], &corners[5]);
    RwV3dCrossProductMacro(&plane->plane.normal, &edge, &vector);
    CameraNormalizePlane(plane);
    plane->plane.distance = RwV3dDotProductMacro(&corners[1], &plane->plane.normal);
    CameraSetPlaneClosest(plane);
    plane++;

    RwV3dSubMacro(&vector, &corners[3], &corners[7]);
    RwV3dSubMacro(&edge, &corners[4], &corners[7]);
    RwV3dCrossProductMacro(&plane->plane.normal, &vector, &edge);
    CameraNormalizePlane(plane);
    plane->plane.distance = RwV3dDotProductMacro(&corners[3], &plane->plane.normal);
    CameraSetPlaneClosest(plane);
    plane++;

    RwV3dSubMacro(&edge, &corners[6], &corners[7]);
    RwV3dCrossProductMacro(&plane->plane.normal, &edge, &vector);
    CameraNormalizePlane(plane);
    plane->plane.distance = RwV3dDotProductMacro(&corners[3], &plane->plane.normal);
    CameraSetPlaneClosest(plane);
}

// Builds the perspective view matrix: the inverse of the camera's frame matrix, scaled and offset
// so the view window maps onto the range 0 to 1 after the divide by Z.
RwCamera* CameraBuildPerspViewMatrix(RwCamera* camera)
{
    RwMatrix* camMatrix = &RwCameraGetFrame(camera)->ltm;
    RwMatrix* viewMatrix = &camera->viewMatrix;
    RwV3d column;
    F32 scale;
    F32 offset;

    scale = -0.5f * camera->recipViewWindow.x;
    offset = 0.5f - scale * camera->viewOffset.x;
    RwV3dScaleMacro(&column, &camMatrix->right, scale);
    RwV3dIncrementScaledMacro(&column, &camMatrix->at, offset);
    viewMatrix->right.x = column.x;
    viewMatrix->up.x = column.y;
    viewMatrix->at.x = column.z;
    viewMatrix->pos.x = 0.5f - (offset + RwV3dDotProductMacro(&camMatrix->pos, &column));

    scale = -0.5f * camera->recipViewWindow.y;
    offset = 0.5f + scale * camera->viewOffset.y;
    RwV3dScaleMacro(&column, &camMatrix->up, scale);
    RwV3dIncrementScaledMacro(&column, &camMatrix->at, offset);
    viewMatrix->right.y = column.x;
    viewMatrix->up.y = column.y;
    viewMatrix->at.y = column.z;
    viewMatrix->pos.y = 0.5f - (offset + RwV3dDotProductMacro(&camMatrix->pos, &column));

    viewMatrix->right.z = camMatrix->at.x;
    viewMatrix->up.z = camMatrix->at.y;
    viewMatrix->at.z = camMatrix->at.z;
    viewMatrix->pos.z = -RwV3dDotProductMacro(&camMatrix->pos, &camMatrix->at);

    RwMatrixOptimize(viewMatrix, NULL);

    return camera;
}

// Builds the parallel frustum: the eight corners in camera space, the window shifted by the view
// offset scaled down towards the far plane, moved to world space by the frame matrix, then the
// far and near planes along the camera's at vector and the four side planes, the last two facing
// the first two.
void CameraBuildParallelClipPlanes(RwCamera* camera)
{
    RwV3d* corners = camera->frustumCorners;
    RwMatrix* camMatrix = &RwCameraGetFrame(camera)->ltm;
    RwFrustumPlane* plane;
    RwV3d vector;
    RwV3d edge;
    F32 nearX;
    F32 nearY;
    F32 farX;
    F32 farY;

    nearX = (1.0f - camera->nearPlane) * -camera->viewOffset.x;
    nearY = (1.0f - camera->nearPlane) * camera->viewOffset.y;
    farX = (1.0f - camera->farPlane) * -camera->viewOffset.x;
    farY = (1.0f - camera->farPlane) * camera->viewOffset.y;

    corners[0].z = corners[1].z = corners[2].z = corners[3].z = camera->nearPlane;
    corners[4].z = corners[5].z = corners[6].z = corners[7].z = camera->farPlane;

    corners[0].x = corners[3].x = camera->viewWindow.x + nearX;
    corners[1].x = corners[2].x = -camera->viewWindow.x + nearX;
    corners[4].x = corners[7].x = camera->viewWindow.x + farX;
    corners[5].x = corners[6].x = -camera->viewWindow.x + farX;

    corners[0].y = corners[1].y = camera->viewWindow.y + nearY;
    corners[2].y = corners[3].y = -camera->viewWindow.y + nearY;
    corners[4].y = corners[5].y = camera->viewWindow.y + farY;
    corners[6].y = corners[7].y = -camera->viewWindow.y + farY;

    RwV3dTransformPoints(corners, corners, 8, camMatrix);

    plane = camera->frustumPlanes;

    plane->plane.normal = camMatrix->at;
    plane->plane.distance = RwV3dDotProductMacro(&corners[4], &plane->plane.normal);
    CameraSetPlaneClosest(plane);
    plane++;

    plane->plane.normal.x = -camera->frustumPlanes[0].plane.normal.x;
    plane->plane.normal.y = -camera->frustumPlanes[0].plane.normal.y;
    plane->plane.normal.z = -camera->frustumPlanes[0].plane.normal.z;
    plane->plane.distance = RwV3dDotProductMacro(&corners[0], &plane->plane.normal);
    CameraSetPlaneClosest(plane);
    plane++;

    RwV3dSubMacro(&vector, &corners[1], &corners[5]);
    RwV3dSubMacro(&edge, &corners[6], &corners[5]);
    RwV3dCrossProductMacro(&plane->plane.normal, &vector, &edge);
    CameraNormalizePlane(plane);
    plane->plane.distance = RwV3dDotProductMacro(&corners[1], &plane->plane.normal);
    CameraSetPlaneClosest(plane);
    plane++;

    RwV3dSubMacro(&edge, &corners[4], &corners[5]);
    RwV3dCrossProductMacro(&plane->plane.normal, &edge, &vector);
    CameraNormalizePlane(plane);
    plane->plane.distance = RwV3dDotProductMacro(&corners[1], &plane->plane.normal);
    CameraSetPlaneClosest(plane);
    plane++;

    plane->plane.normal.x = -camera->frustumPlanes[2].plane.normal.x;
    plane->plane.normal.y = -camera->frustumPlanes[2].plane.normal.y;
    plane->plane.normal.z = -camera->frustumPlanes[2].plane.normal.z;
    plane->plane.distance = RwV3dDotProductMacro(&corners[3], &plane->plane.normal);
    CameraSetPlaneClosest(plane);
    plane++;

    plane->plane.normal.x = -camera->frustumPlanes[3].plane.normal.x;
    plane->plane.normal.y = -camera->frustumPlanes[3].plane.normal.y;
    plane->plane.normal.z = -camera->frustumPlanes[3].plane.normal.z;
    plane->plane.distance = RwV3dDotProductMacro(&corners[3], &plane->plane.normal);
    CameraSetPlaneClosest(plane);
}

// Builds the parallel view matrix: the same as the perspective one without the 0.5 offset in the
// window terms, since there is no divide by Z.
RwCamera* CameraBuildParallelViewMatrix(RwCamera* camera)
{
    RwMatrix* camMatrix = &RwCameraGetFrame(camera)->ltm;
    RwMatrix* viewMatrix = &camera->viewMatrix;
    RwV3d column;
    F32 scale;
    F32 offset;

    scale = -0.5f * camera->recipViewWindow.x;
    offset = -(scale * camera->viewOffset.x);
    RwV3dScaleMacro(&column, &camMatrix->right, scale);
    RwV3dIncrementScaledMacro(&column, &camMatrix->at, offset);
    viewMatrix->right.x = column.x;
    viewMatrix->up.x = column.y;
    viewMatrix->at.x = column.z;
    viewMatrix->pos.x = 0.5f - (offset + RwV3dDotProductMacro(&camMatrix->pos, &column));

    scale = -0.5f * camera->recipViewWindow.y;
    offset = scale * camera->viewOffset.y;
    RwV3dScaleMacro(&column, &camMatrix->up, scale);
    RwV3dIncrementScaledMacro(&column, &camMatrix->at, offset);
    viewMatrix->right.y = column.x;
    viewMatrix->up.y = column.y;
    viewMatrix->at.y = column.z;
    viewMatrix->pos.y = 0.5f - (offset + RwV3dDotProductMacro(&camMatrix->pos, &column));

    viewMatrix->right.z = camMatrix->at.x;
    viewMatrix->up.z = camMatrix->at.y;
    viewMatrix->at.z = camMatrix->at.z;
    viewMatrix->pos.z = -RwV3dDotProductMacro(&camMatrix->pos, &camMatrix->at);

    RwMatrixOptimize(viewMatrix, NULL);

    return camera;
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
