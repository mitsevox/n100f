#ifndef XCOLLIDEFAST_H
#define XCOLLIDEFAST_H

#include "types.h"

struct _xScene;
struct _xRay3;
struct _xEnv;
struct _xBound;

void xCollideFastInit(_xScene* sc);
U32 xRayHitsSceneFast(const _xRay3* r, const _xScene* sc);
U32 xRayHitsEnvFast(const _xRay3* r, const _xEnv* env);
U32 xRayHitsBoundFast(const _xRay3* r, const _xBound* b);

#endif
