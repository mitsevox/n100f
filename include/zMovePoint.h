#ifndef ZMOVEPOINT_H
#define ZMOVEPOINT_H

#include "xMovePoint.h"

struct _zMovePoint : _xMovePoint
{
    U32 unused;
};

struct zScene;

_zMovePoint* zMovePointFindNearest(S32 flg_opts, xVec3* yourpos, F32 okdist, F32 nohope, void* yourdata,
                                   S32 (*losfunc)(void*, xVec3*, const _zMovePoint*), S32* idx_resume,
                                   _zMovePoint** prev_best);
_zMovePoint* zMovePointFindNearest(S32 flg_opts, xVec3* yourpos, void* yourdata,
                                   S32 (*losfunc)(void*, xVec3*, const _zMovePoint*));
S32 zMovePointIsOn(const _zMovePoint* m);
S32 zMovePointEventCB(xBase* from, xBase* to, U32 toEvent, F32* toParam, xBase* toParamWidget);
void zMovePointLoad(_zMovePoint* ent, xSerial* s);
void zMovePointSave(_zMovePoint* ent, xSerial* s);
void zMovePointSetup(_zMovePoint* mvpt, zScene* scn);
_zMovePoint* zMovePoint_GetInst(S32 n);
void zMovePointInit(_zMovePoint* m, p2MovePointAsset* asset);
_zMovePoint* zMovePoint_GetMemPool(S32 cnt);
xVec3* zMovePointGetPos(const _zMovePoint* m);
S32 zMovePointIsOn(const _zMovePoint* m);
F32 zMovePointGetDelay(const _zMovePoint* m);
F32 zMovePointGetNext(const _zMovePoint* current, const _zMovePoint* prev, _zMovePoint** next, xVec3* hdng);
_zMovePoint* zMovePoint_From_xAssetID(U32 aid);

#endif
