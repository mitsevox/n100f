#ifndef ICAMERA_H
#define ICAMERA_H

#include "types.h"
#include "iEnv.h"

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

void iCameraShowRaster(RwCamera* cam);
void iCameraAssignEnv(RwCamera* camera, iEnv* env_geom);
void iCameraSetNearFarClip(F32 nearPlane, F32 farPlane);
void iCameraSetFogParams(iFogParams* fp, F32 time);

#endif
