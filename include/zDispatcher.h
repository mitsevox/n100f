#ifndef ZDISPATCH_H
#define ZDISPATCH_H

#include "xBase.h"

struct zScene;
struct xSerial;

struct st_ZDISPATCH_DATA : xBase
{
    p2BaseAsset* rawass;
    S32 placeholder;
};

void zDispatcher_Startup();
void zDispatcher_Shutdown();
void zDispatcher_scenePrepare();
void zDispatcher_sceneFinish();
st_ZDISPATCH_DATA* zDispatcher_memPool(S32 cnt);
st_ZDISPATCH_DATA* zDispatcher_getInst(st_ZDISPATCH_DATA* pool, S32 idx);
void zDispatcher_Init(st_ZDISPATCH_DATA* dspdata, p2BaseAsset* bass);
void zDispatcher_InitDep(st_ZDISPATCH_DATA* dspdata, zScene* sc);
void zDispatcher_Save(st_ZDISPATCH_DATA* dspdata, xSerial* s);
void zDispatcher_Load(st_ZDISPATCH_DATA* dspdata, xSerial* s);

#endif
