#ifndef ICAMERA_H
#define ICAMERA_H

#include "types.h"
#include "iEnv.h"
#include "iTime.h"
#include "xMath3.h"

#include <rwcore.h>

struct iFogParams
{
    RwFogType type;
    F32 start;
    F32 stop;
    F32 density;
    RwRGBA fogcolor;
    RwRGBA bgcolor;
    U8* table;
};

extern RwCamera* globalCamera;

void iCameraUpdateFog(RwCamera* cam, iTime t);
void iCameraSetFogParams(iFogParams* fp, F32 time);
void iCameraSetNearFarClip(F32 nearPlane, F32 farPlane);
void iCameraRemoveEnv(RwCamera* camera, iEnv* env_geom);
void iCameraAssignEnv(RwCamera* camera, iEnv* env_geom);
void iCameraSetFOV(RwCamera* cam, F32 fov);
void iCameraUpdatePos(RwCamera* cam, xMat4x3* pos);
void iCameraFrustumPlanes(RwCamera* cam, xVec4* frustplane);
void iCameraShowRaster(RwCamera* cam);
void iCameraEnd(RwCamera* cam);
void iCameraBegin(RwCamera* cam, S32 clear);
void iCameraDestroy(RwCamera* camera);
RwCamera* iCameraCreate(S32 width, S32 height);

#endif
