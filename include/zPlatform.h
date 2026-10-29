#ifndef ZPLATFORM_H
#define ZPLATFORM_H

#include "zEnt.h"
#include "xEntMotion.h"

struct p2PlatformBreakawayData
{
    F32 ba_delay;
    U32 bustModelID;
    F32 reset_delay;
};

struct p2PlatformSpringboardData
{
    F32 jmph[3];
    U32 animID[3];
    xVec3 jmpdir;
};

struct p2PlatformConvBeltData
{
    F32 speed;
};

struct p2PlatformTeeterData
{
    F32 itilt;
    F32 maxtilt;
    F32 invmass;
};

// Partial union: springboard is the largest member in the related build.
struct p2PlatformAsset
{
    U8 type;
    U8 pad;
    U16 flags;
    union
    {
        p2PlatformBreakawayData ba;
        p2PlatformSpringboardData sb;
        p2PlatformTeeterData teet;
        p2PlatformConvBeltData cb;
    };
};

struct _xMovePoint;

struct _zPlatform : _zEnt
{
    p2PlatformAsset* passet;
    _xEntMotion motion;
    U16 state;
    U16 plat_flags;
    F32 tmr;
    S32 ctr;
    _xMovePoint* src;
    xModelInstance* am;
    xModelInstance* bm;
    S32 moving;
};

struct _xScene;

void zPlatform_Update(_xEnt* entplat, _xScene* sc, F32 dt);
S32 zPlatformEventCB(xBase* from, xBase* to, U32 toEvent, F32* toParam, xBase* toParamWidget);

#endif
