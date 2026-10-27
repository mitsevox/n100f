#ifndef ZENT_H
#define ZENT_H

#include "xEnt.h"

struct xAnimTable;

struct _zEnt : _xEnt
{
    xAnimTable* atbl;
};

void zEntInit(_zEnt* ent, p2EntAsset* asset, U32 type);
void zEntLoad(_zEnt* ent, xSerial* s);
void zEntSave(_zEnt* ent, xSerial* s);
void zEntEvent(xBase* from, xBase* to, U32 toEvent);
void zEntAnimEvent(_zEnt* ent, U32 animEvent, F32* animParam);

#endif
