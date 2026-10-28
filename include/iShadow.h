#ifndef ISHADOW_H
#define ISHADOW_H

#include "xEnt.h"

void iShadowCollide(iShadowCache* cache, _xEnt* ent, F32 radius, F32 ecc);
void iShadowAdd(iShadowCache* cache, U8 alpha);

#endif
