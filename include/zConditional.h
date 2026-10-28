#ifndef ZCONDITIONAL_H
#define ZCONDITIONAL_H

#include "xBase.h"

struct zCondAsset : p2BaseAsset
{
    U32 constNum;
    U32 expr1;
    U32 op;
};

struct _zConditional : xBase
{
    zCondAsset* asset;
};

void zConditionalInit(void* b, void* asset);
void zConditionalSave(_zConditional* ent, xSerial* s);
void zConditionalLoad(_zConditional* ent, xSerial* s);
U32 zConditional_Evaluate(_zConditional* c);
S32 zConditionalEventCB(xBase*, xBase* to, U32 toEvent, F32*, xBase*);

#endif
