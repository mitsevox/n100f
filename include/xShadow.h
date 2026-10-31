#ifndef XSHADOW_H
#define XSHADOW_H

#include "xVec3.h"

#include <rpworld.h>

struct _xEnt;

extern F32 ShadowThreshold;
extern F32 ShadowZoneRadius;
extern F32 ShadowStrength;
extern S32 ShadowFade;

void xShadowCameraCreate();
void xShadowSetWorld(RpWorld* world);
void xShadowSetLight(xVec3* target_pos, xVec3* in_vec, F32 strength);
void xShadowRenderWorld(xVec3* center, F32 radius);
U32 xShadowReceiveShadowSetup(_xEnt* ent);
void xShadowReceiveShadow(_xEnt* ent);
void xShadowRender(_xEnt* ent);

#endif
