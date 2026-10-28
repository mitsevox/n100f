#include <rwcore.h>

// The standard device function that clears a camera's rasters.
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
