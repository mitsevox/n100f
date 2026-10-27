#ifndef ZENTSHAGGY_H
#define ZENTSHAGGY_H

#include "zEnt.h"
#include "xAnim.h"

struct _zEntHangable;
struct _zMovePoint;
struct _zPlatform;

struct ShaggyGuts
{
    _zEnt* ent;
    U32 shaggyType;
    _zEnt* swap;
    F32 oldY;
    U32 prefIdle;
    U32 prefMove;
    U32 actionIndex;
    F32 idleTimer;
    F32 talkTimer;
    U32 talkFlag;
    U32 talkRand;
    F32 talkSwitch;
    _zEntHangable* hangme;
    F32 hangspin;
    F32 facerot;
    _zMovePoint* facept;
    U16 chimneyState;
    U16 chimneyRand;
    F32 chimneyTimer;
    F32 chimneyHeight;
    F32 chimneyVel;
    F32 chimneyGrav;
    F32 timeLaunch;
    F32 timeTop;
    F32 timeEvent;
    _zPlatform* chimneyPlat;
    xVec3 oldAt;
    xVec3 oldAt2;
    U32 stopMode;
    F32 stopParam;
    U32 stopEvent;
    U32 runEvent;
    _zEnt* witchFollow;
    _zEnt* witchDrop;
    xVec3 dropDiff;
    F32 eatTimer;
    _zEnt* geronimoChase;
    U32 barrelButted;
    U32 barrelBounced;
    U32 barrelFellOver;
    F32 swapTimer;
};

extern ShaggyGuts gShaggy[16];
extern U32 gNumShaggy;

U32 Shaggy0_FlipCB(xAnimTransition* tran, xAnimSingle* anim, _zEnt* ent);
xAnimTable* zEntShaggy_Anim0();
void Shaggy0_PlayerStates(xAnimTable* table);
void Shaggy0_PlayerTransitions(xAnimTable* table);

#endif
