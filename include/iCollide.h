#ifndef ICOLLIDE_H
#define ICOLLIDE_H

#include "types.h"

struct _xSphere;
struct xModelInstance;
struct _xCollis;
struct _xRay3;
struct _xEnv;

U32 iRayHitsModel(const _xRay3* r, const xModelInstance* m, _xCollis* coll);
U32 iRayHitsEnv(const _xRay3* r, const _xEnv* env, _xCollis* coll);
S32 iSphereHitsModel3(const _xSphere* b, const xModelInstance* m, _xCollis* colls, U8 ncolls, F32 sth);

#endif
