#ifndef XSHADOW_H
#define XSHADOW_H

#include <rpworld.h>

struct _xEnt;

extern F32 ShadowThreshold;

void xShadowSetWorld(RpWorld* world);
void xShadowRender(_xEnt* ent);

#endif
