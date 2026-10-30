#ifndef ZPARCMD_H
#define ZPARCMD_H

#include "types.h"
#include "xVec3.h"

struct xParGroup;

struct xParCmdAsset
{
    U32 type;
    U8 enabled;
    U8 mode;
    U8 pad[2];
};

struct xParCmdTex : xParCmdAsset
{
    F32 x1;
    F32 y1;
    F32 x2;
    F32 y2;
    U8 birthMode;
    U8 rows;
    U8 cols;
    U8 unit_count;
    F32 unit_width;
    F32 unit_height;
};

struct xParCmdMove : xParCmdAsset
{
    xVec3 dir;
};

struct xParCmdAccelerate : xParCmdAsset
{
    xVec3 acc;
};

struct xParCmdKillSlow : xParCmdAsset
{
    F32 speedLimitSqr;
    U32 kill_less_than;
};

struct xParCmdKillDistance : xParCmdAsset
{
    F32 dSqr;
    U32 kill_greater_than;
};

struct xParCmdFollow : xParCmdAsset
{
    F32 gravity;
    F32 epsilon;
};

struct xParCmdOrbitPoint : xParCmdAsset
{
    xVec3 center;
    F32 gravity;
    F32 epsilon;
    F32 maxRadiusSqr;
};

struct xParCmdOrbitLine : xParCmdAsset
{
    xVec3 p;
    xVec3 axis;
    F32 gravity;
    F32 epsilon;
    F32 maxRadiusSqr;
};

struct xParCmdMoveRandomPar : xParCmdAsset
{
    xVec3 dim;
};

struct xParCmdScale3rdPolyReg : xParCmdAsset
{
    F32 constant;
    F32 a;
    F32 a2;
    F32 a3;
};

struct xParCmdAlpha3rdPolyReg : xParCmdAsset
{
    F32 constant;
    F32 a;
    F32 a2;
    F32 a3;
};

struct xParCmdApplyWind : xParCmdAsset
{
    F32 strength;
};

// Not in the PS2 debug information; the age command keeps only its rate.
struct xParCmdAge : xParCmdAsset
{
    F32 age_rate;
};

struct xParCmd
{
    U32 flag;
};

struct zParCmd : xParCmd
{
    xParCmdAsset* tasset;
};

#define XPARCMD_TYPE_TEX 0xC

void (*zParCmdGetUpdateFunc(U32 parType))(zParCmd*, xParGroup*, F32);
U32 zParCmdGetSize(U32 parType);
void zParCmdFindClipVolumes();

#endif
