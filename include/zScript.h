#ifndef ZSCRIPT_H
#define ZSCRIPT_H

#include "types.h"
#include "xBase.h"

struct zScriptAsset : p2BaseAsset
{
    F32 scriptStartTime;
    U32 eventCount;
};

struct zScriptEventAsset
{
    F32 time;
    U32 widget;
    U32 paramEvent;
    F32 param[4];
    U32 paramWidget;
};

struct _zScript : xBase
{
    zScriptAsset* tasset;
    U8 state;
    U8 more;
    U8 pad[2];
    F32 time;
};

struct xSerial;
struct _xScene;

void zScriptInit(void* data, void* asset);
void zScriptSave(_zScript* ent, xSerial* s);
void zScriptLoad(_zScript* ent, xSerial* s);
void zScriptUpdate(xBase* to, _xScene* sc, F32 dt);
S32 zScriptEventCB(xBase* from, xBase* to, U32 toEvent, F32* toParam, xBase* toParamWidget);

#endif
