#ifndef ZCUTSCENEMGR_H
#define ZCUTSCENEMGR_H

#include "xBase.h"

struct xCutscene;
struct zCutsceneZbufferHack;
struct xSerial;
struct _xScene;

struct zCutsceneMgrAsset : p2BaseAsset
{
    U32 cutsceneAssetID;
    U32 flags;
    F32 interpSpeed;
    F32 startTime[15];
    F32 endTime[15];
    U32 emitID[15];
};

struct _zCutsceneMgr : xBase
{
    zCutsceneMgrAsset* tasset;
    xCutscene* csn;
    U32 stop;
    zCutsceneZbufferHack* zhack;
    U32 state;
};

U32 zCutSceneNamesTable_getStatus(U32 number);
void zCutsceneMgrUpdate(xBase* to, _xScene* sc, F32 dt);
void zCutsceneMgrFinishLoad(xBase* to);
S32 zCutsceneMgrEventCB(xBase* from, xBase* to, U32 toEvent, F32* toParam, xBase* toParamWidget);
void zCutSceneNamesTable_clearAll();
void zCutsceneMgrLoad(_zCutsceneMgr* t, xSerial* s);
void zCutsceneMgrSave(_zCutsceneMgr* t, xSerial* s);
void zCutsceneMgrInit(void* b, void* tasset);

#endif
