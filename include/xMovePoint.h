#ifndef XMOVEPOINT_H
#define XMOVEPOINT_H

#include "types.h"
#include "xBase.h"
#include "xVec3.h"

struct _xScene;
struct xSpline3;

struct p2MovePointAsset : p2BaseAsset
{
    xVec3 pos;
    U16 wt;
    U8 on;
    U8 bezIndex;
    U8 flg_props;
    U8 pad;
    U16 numPoints;
    F32 delay;
};

struct _xMovePoint : xBase
{
    p2MovePointAsset* asset;
    xVec3* pos;
    _xMovePoint** nodes;
    _xMovePoint* prev;
    U32 node_wt_sum;
    U8 on;
    U8 flg_props;
    U8 pad[2];
    F32 delay;
    xSpline3* spl;
};

typedef struct _xMovePoint xMovePoint;

void xMovePointInit(xMovePoint* m, p2MovePointAsset* asset);
void xMovePointSave(xMovePoint* ent, xSerial* s);
void xMovePointLoad(xMovePoint* ent, xSerial* s);
void xMovePointReset(xMovePoint* m);
void xMovePointSetup(xMovePoint* m, _xScene* sc);
void xMovePointSplineSetup(xMovePoint* m);
void xMovePointSplineDestroy(xMovePoint* m);
F32 xMovePointGetNext(const xMovePoint* m, const xMovePoint* prev, xMovePoint** next, xVec3* hdng);
xVec3* xMovePointGetPos(const xMovePoint* m);

#endif
