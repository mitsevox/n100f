#ifndef ICOLLIDE_H
#define ICOLLIDE_H

#include "types.h"

struct _xSphere;
struct xModelInstance;
struct _xCollis;

S32 iSphereHitsModel3(const _xSphere* b, const xModelInstance* m, _xCollis* colls, U8 ncolls, F32 sth);

#endif
