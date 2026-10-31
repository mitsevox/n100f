#ifndef ICOLLIDEFAST_H
#define ICOLLIDEFAST_H

#include "types.h"

struct _xScene;
struct _xRay3;
struct _xEnv;
struct xModelInstance;

void iCollideFastInit(_xScene* sc);
U32 iRayHitsEnvSpecialFast(const _xRay3* r, const _xEnv* env);
U32 iRayHitsEnvFast(const _xRay3* r, const _xEnv* env);
U32 iRayHitsModelFast(const _xRay3* r, const xModelInstance* m);

#endif
