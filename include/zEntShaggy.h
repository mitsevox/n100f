#ifndef ZENTSHAGGY_H
#define ZENTSHAGGY_H

#include "zEnt.h"
#include "xAnim.h"

struct _zEntHangable;
struct _zMovePoint;
struct _zPlatform;
struct st_zEntVillain;
struct zScene;

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

struct ShaggyMagnet
{
    _zEnt* shaggy;
    xBase* target;
    F32 radius;
};

extern ShaggyGuts gShaggy[16];
extern U32 gNumShaggy;

U32 Shaggy0_FlipCB(xAnimTransition* tran, xAnimSingle* anim, _zEnt* ent);
xAnimTable* zEntShaggy_Anim0();
void Shaggy0_PlayerStates(xAnimTable* table);
void Shaggy0_PlayerTransitions(xAnimTable* table);
xAnimTable* zEntShaggy_Anim1();
void Shaggy1_PlayerStates(xAnimTable* table);
void Shaggy1_PlayerTransitions(xAnimTable* table);
xAnimTable* zEntShaggy_Anim2();
xAnimTable* zEntShaggy_Anim3();
xAnimTable* zEntShaggy_Anim5();
void Shaggy5_WitchTransitions(xAnimTable* table);
xAnimTable* zEntShaggy_Anim8();
xAnimTable* zEntShaggy_Chimney();
S32 Shaggy2_EventCB(xBase* from, xBase* to, U32 toEvent, F32* toParam, xBase* toParamWidget);
S32 Shaggy2Plat_EventCB(xBase* from, xBase* to, U32 toEvent, F32* toParam, xBase* toParamWidget);
void Shaggy1_SetHangable(_zEnt* ent, _zEntHangable* hang);
void Shaggy1_PlayerUpdate(_zEnt* shag, _xScene* sc, F32 dt);
void Shaggy1_ChooseThrowTarget(_zEnt* ent);
_zEnt* zEntShaggy_GetSwap(_zEnt* ent);
void zEntShaggy_Init(st_zEntVillain* ent);
void zEntShaggy_SceneStart(zScene* sc);
void zEntShaggy_SceneReset();
void zEntShaggy_UpdateDep();
U32 zEntIsShaggy(_zEnt* ent);
F32 zEntShaggy_GetSpin(_zEnt* ent);
void zEntShaggy_SetSpin(_zEnt* ent, F32 spin);

#endif
