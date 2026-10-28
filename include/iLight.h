#ifndef ILIGHT_H
#define ILIGHT_H

#include "xMath3.h"
#include "xFColor.h"
#include <rpworld.h>

struct iLight
{
    U32 type;
    RpLight* hw;
    xSphere sph;
    F32 radius_sq;
    _xFColor color;
    xVec3 dir;
    F32 coneangle;
};

extern RpWorld* gLightWorld;

void iLightEnv(iLight* light, S32 env);
void iLightDestroy(iLight* light);
void iLightSetColor(iLight* light, _xFColor* col);

#endif
