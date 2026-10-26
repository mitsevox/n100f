#include "iCamera.h"
#include "xShadow.h"

RwCamera* globalCamera;
F32 sCameraNearClip = 0.2f;
F32 sCameraFarClip = 400.0f;

void iCameraShowRaster(RwCamera* cam)
{
    RwCameraShowRaster(cam, NULL, 0);
}

void iCameraAssignEnv(RwCamera* camera, iEnv* env_geom)
{
    globalCamera = camera;

    RpWorldAddCamera(env_geom->world, camera);
    xShadowSetWorld(env_geom->world);
}

void iCameraSetNearFarClip(F32 nearPlane, F32 farPlane)
{
    if (nearPlane <= 0.0f)
    {
        nearPlane = 0.2f;
    }

    sCameraNearClip = nearPlane;

    if (farPlane <= 0.0f)
    {
        farPlane = 400.0f;
    }

    sCameraFarClip = farPlane;
}
