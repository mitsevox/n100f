#ifndef ICOLLIDE_H
#define ICOLLIDE_H

#include "types.h"

struct _xSphere;
struct _xBox;
struct xModelInstance;
struct _xCollis;
struct _xRay3;
struct _xEnv;
struct _xMat3x3;

void iBoxForModelLocal(_xBox* o, const xModelInstance* m);
void iBoxForModel(_xBox* o, const xModelInstance* m);
U32 iRayHitsModel(const _xRay3* r, const xModelInstance* m, _xCollis* coll);
U32 iRayHitsEnv(const _xRay3* r, const _xEnv* env, _xCollis* coll);
U32 iRayHitsCamEnv(const _xRay3* r, const _xEnv* env, _xCollis* coll);
S32 iSphereHitsEnv4(const _xSphere* b, const _xEnv* env, const _xMat3x3* mat, _xCollis* colls);
S32 iSphereHitsModel3(const _xSphere* b, const xModelInstance* m, _xCollis* colls, U8 ncolls, F32 sth);
S32 iSphereHitsEnv3(const _xSphere* b, const _xEnv* env, _xCollis* colls, U8 ncolls, F32 sth);

#endif
