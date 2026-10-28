#ifndef ZSCRIPT_H
#define ZSCRIPT_H

#include "types.h"
#include "xBase.h"

struct zScriptAsset : p2BaseAsset
{
    F32 scriptStartTime;
    U32 eventCount;
};

struct _zScript : xBase
{
    zScriptAsset* tasset;
    U8 state;
    U8 more;
    U8 pad[2];
    F32 time;
};

void zScriptInit(void* data, void* asset);
S32 zScriptEventCB(xBase* from, xBase* to, U32 toEvent, F32* toParam, xBase* toParamWidget);

#endif
