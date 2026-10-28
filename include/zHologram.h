#ifndef ZHOLOGRAM_H
#define ZHOLOGRAM_H

#include "types.h"

struct _xEnt;

void zHologramInit();
void zHologramAdd(_xEnt* ent);
void zHologramUpdate(F32 seconds);

#endif
