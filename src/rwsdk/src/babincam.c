#include <rwcore.h>
#include <string.h>

extern RwPluginRegistry cameraTKList;

// A camera's stream chunk: its view window and offset, its clip and fog planes and its projection.
typedef struct CameraChunkInfo CameraChunkInfo;
struct CameraChunkInfo
{
    RwV2d viewWindow;
    RwV2d viewOffset;
    F32 nearPlane;
    F32 farPlane;
    F32 fogPlane;
    RwCameraProjection projection;
};

S32 RwCameraRegisterPluginStream(U32 pluginID, RwPluginDataChunkReadCallBack readCB,
                                 RwPluginDataChunkWriteCallBack writeCB,
                                 RwPluginDataChunkGetSizeCallBack getSizeCB)
{
    return _rwPluginRegistryAddPluginStream(&cameraTKList, pluginID, readCB, writeCB, getSizeCB);
}

RwCamera* RwCameraStreamRead(RwStream* stream)
{
    U32 size;
    U32 version;

    if (!RwStreamFindChunk(stream, rwID_STRUCT, &size, &version))
    {
        return NULL;
    }

    if (version >= rwLIBRARYBASEVERSION && version <= rwLIBRARYCURRENTVERSION)
    {
        CameraChunkInfo info;
        RwCamera* camera;

        memset(&info, 0, sizeof(info));

        if (RwStreamRead(stream, &info, size) != size)
        {
            return NULL;
        }

        RwMemNative32(&info, sizeof(info));
        RwMemFloat32ToReal(&info.viewWindow, sizeof(info.viewWindow));
        RwMemFloat32ToReal(&info.viewOffset, sizeof(info.viewOffset));
        RwMemFloat32ToReal(&info.nearPlane, sizeof(info.nearPlane));
        RwMemFloat32ToReal(&info.farPlane, sizeof(info.farPlane));
        RwMemFloat32ToReal(&info.fogPlane, sizeof(info.fogPlane));

        camera = RwCameraCreate();
        if (!camera)
        {
            return NULL;
        }

        if (!_rwPluginRegistryReadDataChunks(&cameraTKList, stream, camera))
        {
            return NULL;
        }

        RwCameraSetViewWindow(camera, &info.viewWindow);
        RwCameraSetViewOffset(camera, &info.viewOffset);
        RwCameraSetNearClipPlane(camera, info.nearPlane);
        RwCameraSetFarClipPlane(camera, info.farPlane);
        RwCameraSetFogDistance(camera, info.fogPlane);
        RwCameraSetProjection(camera, info.projection);

        return camera;
    }
    else
    {
        RwError err;
        err.pluginID = 1;
        err.errorCode = _rwerror(0x80000004);
        RwErrorSet(&err);
        return NULL;
    }
}
