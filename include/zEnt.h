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
struct zAssetModelParam
{
    U32 HashID;
    U8 WordLength;
    U8 String[3];
};

char* zParamGetString(zAssetModelParam* param, U32 size, char* tok, char* def);
S32 zParamGetInt(zAssetModelParam* param, U32 size, const char* tok, S32 def);
void zEntEvent(xBase* to, U32 toEvent);
void zEntEvent(xBase* to, U32 toEvent, F32* toParam);
void zEntEvent(xBase* to, U32 toEvent, F32* toParam, xBase* toParamWidget);
void zEntEvent(xBase* from, xBase* to, U32 toEvent);
void zEntEvent(xBase* from, U32 fromEvent, xBase* to, U32 toEvent, F32* toParam,
               xBase* toParamWidget, S32 forceEvent);
void zEntAnimEvent(_zEnt* ent, U32 animEvent, F32* animParam);

#endif
