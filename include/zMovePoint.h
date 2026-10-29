#ifndef ZMOVEPOINT_H
#define ZMOVEPOINT_H

#include "xMovePoint.h"

struct _zMovePoint : _xMovePoint
{
    U32 unused;
};

xVec3* zMovePointGetPos(const _zMovePoint* m);
S32 zMovePointIsOn(const _zMovePoint* m);
F32 zMovePointGetDelay(const _zMovePoint* m);
F32 zMovePointGetNext(const _zMovePoint* current, const _zMovePoint* prev, _zMovePoint** next, xVec3* hdng);
_zMovePoint* zMovePoint_From_xAssetID(U32 aid);

#endif
