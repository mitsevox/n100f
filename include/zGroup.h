#ifndef ZGROUP_H
#define ZGROUP_H

#include "xBase.h"

struct zGroupAsset : p2BaseAsset
{
    U16 itemCount;
    U16 groupFlags;
};

struct _zGroup : xBase
{
    zGroupAsset* asset;
};

U32 zGroupGetItem(_zGroup* g, U32 index);
xBase* zGroupGetItemPtr(_zGroup* g, U32 index);
U32 zGroupGetCount(_zGroup* g);
S32 zGroupEventCB(xBase*, xBase* to, U32 toEvent, F32* toParam, xBase* toParamWidget);
void zGroupLoad(_zGroup* ent, xSerial* s);
void zGroupSave(_zGroup* ent, xSerial* s);
void zGroupInit(void* b, void* asset);

#endif
