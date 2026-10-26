#ifndef ICAMERA_H
#define ICAMERA_H

#include "types.h"
#include "iEnv.h"

#include <rwcore.h>

extern RwCamera* globalCamera;

void iCameraShowRaster(RwCamera* cam);
void iCameraAssignEnv(RwCamera* camera, iEnv* env_geom);
void iCameraSetNearFarClip(F32 nearPlane, F32 farPlane);

#endif
