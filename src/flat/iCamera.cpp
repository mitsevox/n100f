#include "iCamera.h"
#include "iScrFX.h"
#include "xMath.h"
#include "xShadow.h"
#include "zGlobals.h"

#include <dolphin/os.h>
#include <math.h>
#include <string.h>

RwCamera* globalCamera;
F32 sCameraNearClip = 0.2f;
F32 sCameraFarClip = 400.0f;

void iCameraUpdateFog(RwCamera* cam, iTime t)
{
    iFogParams* fp;
    iFogParams* fa;
    iFogParams* fb;
    F32 a;
    U32 bite_me;

    fp = &globals.fog;
    fa = &globals.fogA;
    fb = &globals.fogB;

    if (fp->type == rwFOGTYPENAFOGTYPE)
    {
        RwRenderStateSet(rwRENDERSTATEFOGENABLE, (void*)FALSE);
        RwCameraSetFarClipPlane(cam, sCameraFarClip);
        return;
    }

    if (globals.fog_t0 == 0)
    {
        fp = fa;
    }
    else
    {
        t = iTimeGet();
        a = iTimeDiffSec(globals.fog_t0, t) / iTimeDiffSec(globals.fog_t0, globals.fog_t1);
        a = CLAMP(a, 0.0f, 1.0f);

        fp->type = fb->type;
        fp->table = fb->table;
        fp->start = fa->start + a * (fb->start - fa->start);
        fp->stop = fa->stop + a * (fb->stop - fa->stop);
        fp->density = fa->density + a * (fb->density - fa->density);
        fp->fogcolor.green = fa->fogcolor.green + a * (fb->fogcolor.green - fa->fogcolor.green);
        fp->fogcolor.blue = fa->fogcolor.blue + a * (fb->fogcolor.blue - fa->fogcolor.blue);
        fp->fogcolor.alpha = fa->fogcolor.alpha + a * (fb->fogcolor.alpha - fa->fogcolor.alpha);
        fp->fogcolor.red = fa->fogcolor.red + a * (fb->fogcolor.red - fa->fogcolor.red);
        fp->bgcolor.green = fa->bgcolor.green + a * (fb->bgcolor.green - fa->bgcolor.green);
        fp->bgcolor.blue = fa->bgcolor.blue + a * (fb->bgcolor.blue - fa->bgcolor.blue);
        fp->bgcolor.alpha = fa->bgcolor.alpha + a * (fb->bgcolor.alpha - fa->bgcolor.alpha);
        fp->bgcolor.red = fa->bgcolor.red + a * (fb->bgcolor.red - fa->bgcolor.red);

        if (a == 1.0f)
        {
            globals.fog_t0 = 0;
            *fa = *fb;
        }
    }

    bite_me = (fp->fogcolor.alpha << 24) | (fp->fogcolor.red << 16) | (fp->fogcolor.green << 8) |
              fp->fogcolor.blue;

    RwRenderStateSet(rwRENDERSTATEFOGENABLE, (void*)TRUE);
    RwRenderStateSet(rwRENDERSTATEFOGTYPE, (void*)fp->type);
    RwRenderStateSet(rwRENDERSTATEFOGCOLOR, (void*)bite_me);
    RwRenderStateSet(rwRENDERSTATEFOGDENSITY, (void*)&fp->density);
    RwRenderStateSet((RwRenderState)18, (void*)fp->table);

    RwCameraSetFogDistance(cam, fp->start);
    RwCameraSetFarClipPlane(cam, fp->stop);
}

void iCameraSetFogParams(iFogParams* fp, F32 time)
{
    if (!fp || fp->type == rwFOGTYPENAFOGTYPE)
    {
        globals.fog.type = rwFOGTYPENAFOGTYPE;
        globals.fogA.type = rwFOGTYPENAFOGTYPE;
    }
    else if (0.0f == time || fp->type != globals.fogA.type)
    {
        globals.fog = *fp;
        globals.fogA = *fp;
        globals.fog_t0 = 0;
    }
    else
    {
        globals.fogA = globals.fog;
        globals.fogB = *fp;

        globals.fog_t0 = iTimeGet();
        globals.fog_t1 = globals.fog_t0 + (iTime)OSSecondsToTicks(time);
    }
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

void iCameraRemoveEnv(RwCamera* camera, iEnv* env_geom)
{
    RpWorldRemoveCamera(env_geom->world, camera);
}

void iCameraAssignEnv(RwCamera* camera, iEnv* env_geom)
{
    globalCamera = camera;

    RpWorldAddCamera(env_geom->world, camera);
    xShadowSetWorld(env_geom->world);
}

void iCameraSetFOV(RwCamera* cam, F32 fov)
{
    RwV2d vw;

    vw.x = tan(PI * (0.5f * fov) / 180.0f);
    vw.y = 480.0f * vw.x / 640.0f;

    RwCameraSetViewWindow(cam, &vw);
}

void iCameraUpdatePos(RwCamera* cam, xMat4x3* pos)
{
    RwFrame* f;
    RwMatrix* m;

    f = RwCameraGetFrame(cam);
    m = RwFrameGetMatrix(f);

    memcpy(m, pos, sizeof(xMat4x3));

    RwFrameOrthoNormalize(f);
    RwMatrixUpdate(m);
    RwFrameUpdateObjects(f);
}

void iCameraFrustumPlanes(RwCamera* cam, xVec4* frustplane)
{
    RwFrustumPlane* rwPlane;

    rwPlane = &cam->frustumPlanes[2];
    frustplane[0].x = rwPlane->plane.normal.x;
    frustplane[1].x = rwPlane->plane.normal.y;
    frustplane[2].x = rwPlane->plane.normal.z;
    frustplane[3].x = rwPlane->plane.distance;

    rwPlane = &cam->frustumPlanes[4];
    frustplane[0].y = rwPlane->plane.normal.x;
    frustplane[1].y = rwPlane->plane.normal.y;
    frustplane[2].y = rwPlane->plane.normal.z;
    frustplane[3].y = rwPlane->plane.distance;

    rwPlane = &cam->frustumPlanes[5];
    frustplane[0].z = rwPlane->plane.normal.x;
    frustplane[1].z = rwPlane->plane.normal.y;
    frustplane[2].z = rwPlane->plane.normal.z;
    frustplane[3].z = rwPlane->plane.distance;

    rwPlane = &cam->frustumPlanes[3];
    frustplane[0].w = rwPlane->plane.normal.x;
    frustplane[1].w = rwPlane->plane.normal.y;
    frustplane[2].w = rwPlane->plane.normal.z;
    frustplane[3].w = rwPlane->plane.distance;

    rwPlane = &cam->frustumPlanes[1];
    frustplane[4].x = rwPlane->plane.normal.x;
    frustplane[5].x = rwPlane->plane.normal.y;
    frustplane[6].x = rwPlane->plane.normal.z;
    frustplane[7].x = rwPlane->plane.distance;

    rwPlane = &cam->frustumPlanes[0];
    frustplane[4].y = rwPlane->plane.normal.x;
    frustplane[5].y = rwPlane->plane.normal.y;
    frustplane[6].y = rwPlane->plane.normal.z;
    frustplane[7].y = rwPlane->plane.distance;

    rwPlane = &cam->frustumPlanes[2];
    frustplane[4].z = rwPlane->plane.normal.x;
    frustplane[5].z = rwPlane->plane.normal.y;
    frustplane[6].z = rwPlane->plane.normal.z;
    frustplane[7].z = rwPlane->plane.distance;

    rwPlane = &cam->frustumPlanes[4];
    frustplane[4].w = rwPlane->plane.normal.x;
    frustplane[5].w = rwPlane->plane.normal.y;
    frustplane[6].w = rwPlane->plane.normal.z;
    frustplane[7].w = rwPlane->plane.distance;
}

void iCameraShowRaster(RwCamera* cam)
{
    RwCameraShowRaster(cam, NULL, 0);
}

void iCameraEnd(RwCamera* cam)
{
    iScrFxCameraEndScene(cam);
    RwCameraEndUpdate(cam);
}

void iCameraBegin(RwCamera* cam, S32 clear)
{
    if (clear)
    {
        if (globals.fog.type != rwFOGTYPENAFOGTYPE)
        {
            RwCameraClear(cam, &globals.fog.bgcolor, rwCAMERACLEARIMAGE | rwCAMERACLEARZ);
        }
        else
        {
            RwCameraClear(cam, NULL, rwCAMERACLEARZ);
        }
    }

    RwCameraSetNearClipPlane(cam, sCameraNearClip);
    RwCameraBeginUpdate(cam);
}

void iCameraDestroy(RwCamera* camera)
{
    RwRaster* raster;
    RwFrame* frame;

    _rwFrameSyncDirty();

    if (camera)
    {
        frame = RwCameraGetFrame(camera);

        if (frame)
        {
            RwCameraSetFrame(camera, NULL);
            RwFrameDestroy(frame);
        }

        raster = RwCameraGetRaster(camera);

        if (raster)
        {
            RwRasterDestroy(raster);
            RwCameraSetRaster(camera, NULL);
        }

        raster = RwCameraGetZRaster(camera);

        if (raster)
        {
            RwRasterDestroy(raster);
            RwCameraSetZRaster(camera, NULL);
        }

        RwCameraDestroy(camera);
    }
}

RwCamera* iCameraCreate(S32 width, S32 height)
{
    RwV2d vw;
    RwCamera* camera;

    camera = RwCameraCreate();

    RwCameraSetFrame(camera, RwFrameCreate());
    RwCameraSetRaster(camera, RwRasterCreate(width, height, 0, rwRASTERTYPECAMERA));
    RwCameraSetZRaster(camera, RwRasterCreate(width, height, 0, rwRASTERTYPEZBUFFER));
    RwCameraSetFarClipPlane(camera, sCameraFarClip);
    RwCameraSetNearClipPlane(camera, sCameraNearClip);

    vw.x = 1.0f;
    vw.y = 1.0f;

    RwCameraSetViewWindow(camera, &vw);

    return camera;
}
