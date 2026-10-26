#ifndef RPWORLD_H
#define RPWORLD_H

#include <rwcore.h>

// RenderWare types are opaque until code that accesses their members is reconstructed.
struct RpAtomic;
struct RpWorld;
struct RpLight;

#ifdef __cplusplus
extern "C" {
#endif

RpWorld* RpWorldAddCamera(RpWorld* world, RwCamera* camera);
RpWorld* RpWorldRemoveCamera(RpWorld* world, RwCamera* camera);
S32 RpWorldDestroy(RpWorld* world);

#ifdef __cplusplus
}
#endif

#endif
