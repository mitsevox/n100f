#ifndef ZPARSYS_H
#define ZPARSYS_H

#include "types.h"
#include "xBase.h"
#include "xParGroup.h"
#include "zParCmd.h"

struct _xScene;

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
    S32 cmdCount;
    zParCmd* cmd;
    zParSys* parent;
    _zParGroup* group;
    U8 visible;
};

void zParSysInit(void* b, void* tasset);
void zParSysInit(xBase* b, zParSysAsset* tasset);
void zParSysSetup(zParSys* t);
void zParSysExit(zParSys* t);
S32 zParSysEventCB(xBase* from, xBase* to, U32 toEvent, F32* toParam, xBase* toParamWidget);
void zParSysUpdate(xBase* to, _xScene*, F32 dt);
void zParSysRender(xBase* b);

#endif
