#ifndef XBASE_H
#define XBASE_H

#include "types.h"
#include "xserializer.h"

struct p2LinkAsset;

struct p2BaseAsset
{
    U32 id;
    U8 baseType;
    U8 linkCount;
    U16 baseFlags;
};

struct xBase
{
    U32 id;
    U8 baseType;
    U8 linkCount;
    U16 baseFlags;
    p2LinkAsset* link;
    S32 (*eventFunc)(xBase*, xBase*, U32, F32*, xBase*);
};

void xBaseInit(xBase* xb, p2BaseAsset* asset);
void xBaseSave(xBase* ent, xSerial* s);
void xBaseLoad(xBase* ent, xSerial* s);

#endif
