#ifndef ICOLLIDE_H
#define ICOLLIDE_H

#include "xEnt.h"

struct _xEnv;

void iBoxForModelLocal(_xBox* o, const xModelInstance* m);
void iBoxForModel(_xBox* o, const xModelInstance* m);
S32 iSphereHitsModel3(const xSphere* b, const xModelInstance* m, _xCollis* colls, U8 ncolls,
                      F32 sth);
S32 iSphereHitsEnv3(const xSphere* b, const _xEnv* env, _xCollis* colls, U8 ncolls, F32 sth);

#endif
