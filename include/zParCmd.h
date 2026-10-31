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

struct xParCmdJet : xParCmdAsset
{
    xVec3 center;
    xVec3 acc;
    F32 gravity;
    F32 epsilon;
    F32 radiusSqr;
};

struct xParCmdRotateAround : xParCmdAsset
{
    xVec3 pos;
    F32 unused1;
    F32 radius_growth;
    F32 yaw;
};

struct xParCmdApplyCamMat : xParCmdAsset
{
    xVec3 apply;
};

struct xParCmdRotPar : xParCmdAsset
{
    xVec3 min;
    xVec3 max;
};

struct xParCmdRandomVelocityPar : xParCmdAsset
{
    F32 x;
    F32 y;
    F32 z;
};

struct xParCmdPlayerCollision : xParCmdAsset
{
    F32 min;
    F32 max;
};

struct xParCmdTexAnim : xParCmdAsset
{
    U8 anim_mode;
    U8 anim_wrap_mode;
    U8 pad_anim;
    U8 throttle_spd_less_than;
    F32 throttle_spd_sqr;
    F32 throttle_time;
    F32 throttle_time_elapsed;
};

// The commands below are not in the PS2 debug information. Their sizes come from the command table
// and their fields from what the GameCube update functions read.

// Picks one of two built-in curves.
struct xParCmdSmokeAlpha : xParCmdAsset
{
    S32 curve;
};

// Picks one of two built-in curves.
struct xParCmdScale : xParCmdAsset
{
    S32 curve;
};

struct xParCmdAnimalMagnetism : xParCmdAsset
{
    F32 strength;
};

// Picks one of the hard-coded behaviours.
struct xParCmdCustom : xParCmdAsset
{
    S32 type;
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
