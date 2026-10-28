#ifndef ZPARSYS_H
#define ZPARSYS_H

#include "types.h"
#include "xBase.h"
#include "xParGroup.h"

struct zParCmd;

struct zParSysAsset : p2BaseAsset
{
    U32 type;
    U32 parentParSysID;
    U32 textureID;
    U8 parFlags;
    U8 priority;
    U16 maxPar;
    U8 renderFunc;
    U8 renderSrcBlendMode;
    U8 renderDstBlendMode;
    U8 cmdCount;
    U32 cmdSize;
};

struct _zParGroup : xParGroup
{
};

struct zParSys : xBase
{
    zParSysAsset* tasset;
    U32 cmdCount;
    zParCmd* cmd;
    zParSys* parent;
    _zParGroup* group;
    U8 visible;
};

S32 zParSysEventCB(xBase* from, xBase* to, U32 toEvent, F32* toParam, xBase* toParamWidget);
void zParSysExit(zParSys* t);

#endif
