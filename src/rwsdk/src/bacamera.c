#include <rwcore.h>

// The standard device functions the camera calls.
#define STDFUNC_CAMERABEGINUPDATE 1
#define STDFUNC_CAMERAENDUPDATE 10
#define STDFUNC_CAMERACLEAR 21

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
