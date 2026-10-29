#include "zConditional.h"
#include "xEvent.h"
#include "zEnt.h"
#include "version.h"
#include "zVar.h"

#if VERSION_GIHE78_REV0
#define VAR_TABLE zVarEntryTable
#define VAR_INIT zVarInit
#elif VERSION_GIHE78_REV1
extern "C" zVarEntry lbl_801D7670[];
extern "C" void fn_800D7864(zVarEntry* table);
#define VAR_TABLE lbl_801D7670
#define VAR_INIT fn_800D7864
#endif

// Set up the base, its event handler, and the variable table.
void zConditionalInit(void* b, void* asset)
{
    _zConditional* t = (_zConditional*)b;
    zCondAsset* a = (zCondAsset*)asset;
    xBaseInit(t, a);
    t->eventFunc = zConditionalEventCB;
    t->asset = a;
    if (t->linkCount)
        t->link = (p2LinkAsset*)(t->asset + 1);
    else
        t->link = NULL;
    VAR_INIT(VAR_TABLE);
}

// Persist the base state; the condition has no additional runtime state.
void zConditionalSave(_zConditional* ent, xSerial* s)
{
    xBaseSave(ent, s);
}

// Restore the base state; the condition has no additional runtime state.
void zConditionalLoad(_zConditional* ent, xSerial* s)
{
    xBaseLoad(ent, s);
}

// Find the last matching variable, then compare its value with the asset constant.
U32 zConditional_Evaluate(_zConditional* c)
{
    zVarEntry* v = NULL;
    for (S32 i = 0; i < 107; ++i)
    {
        if (VAR_TABLE[i].varNameID == c->asset->expr1)
            v = &VAR_TABLE[i];
    }
    if (!v)
        return 0;
    U32 temp = v->varCB();
    switch (c->asset->op)
    {
    case 0: return temp == c->asset->constNum ? 1 : 0;
    case 1: return temp > c->asset->constNum ? 1 : 0;
    case 2: return temp < c->asset->constNum ? 1 : 0;
    case 3: return temp >= c->asset->constNum ? 1 : 0;
    case 4: return temp <= c->asset->constNum ? 1 : 0;
    case 5: return temp != c->asset->constNum ? 1 : 0;
    default: return 0;
    }
}

// Event 60 tests the condition and sends 61 or 62; reset restores base flags.
S32 zConditionalEventCB(xBase*, xBase* to, U32 toEvent, F32*, xBase*)
{
    _zConditional* t = (_zConditional*)to;
    switch (toEvent)
    {
    case 60:
        if (zConditional_Evaluate(t))
            zEntEvent(t, t, 61);
        else
            zEntEvent(t, t, 62);
        break;
    case eEventReset:
        xBaseReset(t, t->asset);
        break;
    }
    return 1;
}
