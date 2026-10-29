#include "zEntPlayer.h"
#include "zGlobals.h"
#include "zEntHangable.h"
#include "zEntSimpleObj.h"
#include "zEntShaggy.h"
#include "zEntVillain.h"
#include "zEntBubble.h"
#include "zParEmitter.h"
#include "zRumble.h"
#include "zLaugh.h"
#include "zPitchChange.h"
#include "zSurface.h"
#include "zEGenerator.h"
#include "xBound.h"
#include "zScene.h"
#include "zEntPickup.h"
#include "zPlatform.h"
#include "zEntTrigger.h"
#include "zEntButton.h"
#include "zEntDestructObj.h"
#include "xEvent.h"
#include "xPad.h"
#include "xSnd.h"
#include "iSnd.h"
#include "xString.h"
#include "xMath.h"
#include "xMathInlines.h"
#include "xMath3.h"
#include "iCollide.h"
#include "xCollide.h"
#include "xMovePoint.h"
#include "xScene.h"
#include "xEnv.h"
#include "xstransvc.h"
#include "zFeet.h"
#include "zGust.h"
#include "xFFX.h"
#include "xShadow.h"
#include "xModel.h"
#include "zLight.h"
#include "zGame.h"
#include "zGameState.h"
#include "xScrFx.h"
#include "iAnimSKB.h"
#include "iMath3.h"
#include "iMath.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct st_zEntVillain;
struct _zEntPickup;
struct xSerial;
struct xAnimActiveEffect;

struct _tagTRCPadInfo
{
    S32 pad_init;
    S32 id;
    S32 state;
};

extern "C" _tagTRCPadInfo lbl_802289B8[4];

struct zPlayerSituation
{
    U32 priority;
    U32 countMax;
    F32 delay;
    F32 punchDelay;
    F32 elapsedTime;
    U32 count;
    U32 type;
};

enum _LostChanceFX
{
    LCFX_None,
    LCFX_StdShock,
    LCFX_Water,
    LCFX_Total
};

struct _tagPlayerSndFX
{
    S16 ammo_pitch;
    S16 special_pitch;
    S16 token_pitch;
    S16 bite_pitch;
    S16 snackcounter_pitch;
    S16 save_pitch;
    S16 key_pitch;
    S16 info_pitch;
    S16 error_pitch;
    U32 ammo_id;
    U32 special_id;
    U32 token_id;
    U32 bite_id;
    U32 snackcounter_id;
    U32 save_id;
    U32 key_id;
    U32 info_id;
    U32 error_id;
    U32 van_id;
    S16 butt_pitch;
    S16 head_pitch;
    S16 butt_snd;
    S16 head_snd;
    U32 butt_id[3];
    U32 head_id[3];
    U32 buttmiss_id;
    U32 skid_id[3];
    S32 vo_snd;
    U32 dig_id[2];
    U32 scare_id[2];
    U32 excite_id[13];
    U32 danger_id[9];
    U32 leap_id[12];
    U32 see_geronimo_id[1];
    U32 see_shaggy_id[5];
    U32 see_holly_id[3];
    U32 see_monster_id[5];
    U32 see_groundskeeper_id[1];
    U32 see_warpgate_id[1];
    U32 need_more_snacks_id[3];
    U32 success_id[1];
    U32 see_token_id[3];
    U32 see_crab_id[2];
    U32 see_rat_id[4];
    U32 see_gargoyle_id[2];
    U32 see_creeper_id[2];
    U32 see_ghost_diver_id[2];
    U32 see_sea_creature_id[2];
    U32 see_headless_specter_id[2];
    U32 see_scarecrow_id[3];
    U32 see_space_kook_id[2];
    U32 see_tar_monster_id[4];
    U32 see_witch_id[2];
    U32 see_robot_id[2];
    U32 see_wolfman_id[2];
    U32 see_ghost_id[4];
    U32 see_zombie_id[2];
    U32 sorry_id[4];
    U32 collect_all_snacks_id[1];
    U32 eat_sandwich_id[2];
    U32 eat_turkey_id[2];
    U32 eat_cake_id[2];
    U32 eat_snacks_id[3];
};

S32 zEntVillain_Headbutt(st_zEntVillain* vil, xVec3* hdng);
void xCameraSetTarget(_tagxCamFrame* cam, xVec3* target);
void xCameraSetTarget(_tagxCamFrame* cam, xMat4x3* target);
void xCameraPickInitial(_tagxCamFrame* cam, U32 id);
void xCameraSetController(_tagxCamFrame* cam, F32 x, F32 y, F32 dt);
void zSceneGameStatsTally_snack(U32 quantity);
void zSceneGameStatsTally_death();
void xCameraTargetOnGround(S32 on_ground, F32 height);
void zSceneGameStatsTally_scare();
void zEntVillain_HeroWasHurt();
void zEntEventAll(xBase* from, U32 fromEvent, U32 toEvent, F32* toParam);
U32 zEntPickup_lastSnackInArea();
void zEntPickup_hideSnackGateInfo(_zEntPickup* pickup);
void zEntPickup_UnlockSnackGate(_xEnt* ent);
void zEntPickup_Drop(_zEntPickup* ent);
void xEntBeginCollCheckTrigs(_xEnt* ent);
void xEntEndCollCheckTrigs(_xEnt* ent);
void xSceneForAllTriggers(_xScene* sc, _xEnt* (*func)(_xEnt*, _xScene*, void*), void* data);
void xEntCollCheckNPCs(_xEnt* p, _xScene* sc, _xEnt* (*hitIt)(_xEnt*, _xScene*, void*));
void xEntCollCheckDyns(_xEnt* p, _xScene* sc, _xEnt* (*hitIt)(_xEnt*, _xScene*, void*));
void xEntCollCheckStats(_xEnt* p, _xScene* sc, _xEnt* (*hitIt)(_xEnt*, _xScene*, void*));
S32 iSphereHitsEnv4(const xSphere* b, const _xEnv* env, const xMat3x3* mat, _xCollis* coll);
U32 iRayHitsEnv(const _xRay3* r, const _xEnv* env, _xCollis* coll);
U32 iRayHitsModel(const _xRay3* r, const xModelInstance* m, _xCollis* coll);
U32 xSphereHitsBox(const xSphere* a, const _xBox* b, _xCollis* coll);
void xLine3VecDist2(const xVec3* p1, const xVec3* p2, const xVec3* v, _xIsect* isect);
static void PlayerCollsSelectDepen(_xEnt* p, _xScene* sc, F32 dt);
static void PlayerCollisTranslate(_xCollis* c, F32 x, F32 y, F32 z);
static _xEnt* PlayerCollCheckOneVillain(_xEnt* ent, _xScene* sc, void* data);
static _xEnt* PlayerCollCheckOneEnt(_xEnt* ent, _xScene* sc, void* data);

static inline F32 xVec3Dist2(const xVec3* a, const xVec3* b)
{
    F32 dx = a->x - b->x;
    F32 dy = a->y - b->y;
    F32 dz = a->z - b->z;

    return dx * dx + dy * dy + dz * dz;
}

static inline F32 xVec3Dist(const xVec3* a, const xVec3* b)
{
    F32 dx = a->x - b->x;
    F32 dy = a->y - b->y;
    F32 dz = a->z - b->z;

    return sqrtf(dx * dx + dy * dy + dz * dz);
}

static inline void xEntSetTrig(_xEntCollis* coll, U16 idx)
{
    coll->trigs_cur[idx >> 3] |= (U8)(1 << (idx % 8));
}

static inline void xEntClearTrig(_xEntCollis* coll, U16 idx)
{
    coll->trigs_cur[idx >> 3] &= (U8)~(1 << (idx % 8));
}

_xEnt* zEntPlayerCollTriggerCB(_xEnt* ent, _xScene* sc, void* data);
void zHudShowAllSnacksArea();
void zHud_showSnack();
void zHud_showSnackSpecial();
static void PlayerAbsControl(_xEnt* ent, F32 x, F32 z, F32 dt);
static F32 CalcJumpImpulse_Smooth(F32 g, F32 j, F32 h, F32 Tgc, F32 Tgs);
S32 Shaggy8_HBCallback(_zEnt* ent, _xCollis* coll);
S32 Shaggy8_BBCallback(_zEnt* ent, _xCollis* coll);
S32 Shaggy8_CallToPlayer();
void iDrawSetFBMSK(U32 FBMSK);
void iModelSetMaterialAlpha(RpAtomic* model, S32 alpha);
void xAnimTableRemoveEffectByCB(xAnimTable* table,
                                U32 (*callback)(U32, xAnimActiveEffect*, xAnimSingle*, void*));
static U32 zEntPlayer_SNDSkidCB(U32 cbenum, xAnimActiveEffect* acteffect, xAnimSingle* single, void* object);
static U32 zEntPlayer_FXScratchCB(U32 cbenum, xAnimActiveEffect* acteffect, xAnimSingle* single,
                                  void* object);
static U32 zEntPlayer_FXDigupCB(U32 cbenum, xAnimActiveEffect* acteffect, xAnimSingle* single,
                                void* object);
static U32 zEntPlayer_FXSpringCB(U32 cbenum, xAnimActiveEffect* acteffect, xAnimSingle* single,
                                 void* object);
static U32 zEntPlayer_FXOpenBrollyCB(U32 cbenum, xAnimActiveEffect* acteffect, xAnimSingle* single,
                                     void* object);
static void zEntPlayer_ButtonHBFX(_xEnt*, _zEntButton*);
static void EGeneratorCheck(_xEnt* ent, _xScene* sc, F32 dt);
void zEntPlayerFloorUpdate(_xEnt* p, _xScene* sc, F32 dt);
void zEntPlayerDriveUpdate(_xEnt* p, _xScene* sc, F32 dt);
void zEntPlayerJumpUpdate(_xEnt* ent, _xScene* sc, F32 dt);
void zEntPlayerVelUpdate(_xEnt* ent, _xScene* sc, F32 dt);
void zEntPlayerSurfDamageUpdate(_xEnt* p, _xScene* sc, F32 dt);
void zEntPlayerCollide(_xEnt* ent, _xScene* sc, F32 dt);
void zEntPlayerCollTrigger(_xEnt* ent, _xScene* sc);
static void PlayerHangCheck(_xEnt* ent, _xScene* sc);
static void PlayerTeeterCheck(_xEnt* ent, _xScene* sc, F32 dt);
static void PlayerDigUpdate(_xEnt* ent, F32 dt);
void zEntPlayer_SituationDo();
static void zEntPlayer_FXUpdate(_xEnt* ent, F32 seconds);
static void zEntPlayer_SNDButtBounce(_xEnt* ent, S32 hit_villain);
void zEntPlayer_Update(_xEnt* ent, _xScene* sc, F32 dt);
void zEntPlayer_Move(_xEnt* ent, _xScene* sc, F32 dt, _xEntFrame* frame);
void zEntPlayer_Render(_zEnt* ent);
S32 zEntPlayerEventCB(xBase* from, xBase* to, U32 toEvent, F32* toParam, xBase* toParamWidget);
static void PlayerRotMatchUpdateEnt(_xEnt* ent, _xScene* sc, F32 dt, void* fdata);
void zEntPlayerReset(_xEnt* ent);
static void zEntPlayer_FXSetup(_xEnt* ent, xAnimTable* table);
void zEntPlayer_SNDInit();
static void zEntPlayer_ButtonBBFX(_xEnt* ent, _zEntButton*);
static S32 DestructObjHBCallback(_zEnt* ent, _xCollis* coll);

static xVec3 headcoll_oses[12] = {
    { 0.0f, 0.0f, 0.15f },  { 0.0f, 0.0f, 0.15f },   { 0.0f, 0.0f, 0.4f },  { 0.0f, 0.0f, 0.3f },
    { 0.0f, 1.15f, -0.15f }, { 0.0f, 1.0f, -0.2f },  { 0.0f, 0.9f, 0.45f }, { 0.0f, 0.75f, 0.5f },
    { 0.0f, 0.9f, -0.25f },  { 0.0f, 0.0f, -0.15f }, { 0.0f, 0.0f, 0.45f }, { 0.0f, 0.0f, 0.5f },
};
static F32 headcoll_bltimes[12] = { 0.15f, 0.15f, 0.1f,  0.1f,  0.15f, 0.15f,
                                    0.15f, 0.15f, 0.15f, 0.15f, 0.15f, 0.15f };
xVec3 headcoll_fromos = { 0.0f, 0.0f, 0.0f };
xVec3 headcoll_os = { 0.0f, 0.0f, 0.0f };

static zPlayerSituation sPlayerInfo[37] = {
    { 0, 0, 120.0f, 0.0f, 0.0f, 0, 0 },  { 1, 0, 2.0f, 0.0f, 0.0f, 0, 1 },
    { 0, 0, 60.0f, 0.0f, 0.0f, 0, 2 },   { 0, 0, 60.0f, 0.0f, 0.0f, 0, 3 },
    { 0, 0, 120.0f, 1.0f, 0.0f, 0, 4 },  { 0, 0, 2.0f, 0.5f, 0.0f, 0, 5 },
    { 0, 0, 120.0f, 1.0f, 0.0f, 0, 6 },  { 0, 0, 2.0f, 0.0f, 0.0f, 0, 7 },
    { 1, 0, 20.0f, 0.5f, 0.0f, 0, 8 },   { 0, 0, 1.0f, 0.0f, 0.0f, 0, 9 },
    { 0, 0, 60.0f, 0.0f, 0.0f, 0, 10 },  { 0, 0, 60.0f, 1.0f, 0.0f, 0, 11 },
    { 0, 0, 30.0f, 0.0f, 0.0f, 0, 12 },  { 0, 0, 60.0f, 0.0f, 0.0f, 0, 13 },
    { 5, 0, 60.0f, 0.5f, 0.0f, 0, 14 },  { 5, 0, 5.0f, 1.0f, 0.0f, 0, 15 },
    { 1, 0, 30.0f, 0.0f, 0.0f, 0, 16 },  { 1, 0, 30.0f, 0.0f, 0.0f, 0, 17 },
    { 1, 0, 30.0f, 0.0f, 0.0f, 0, 18 },  { 1, 0, 30.0f, 0.0f, 0.0f, 0, 19 },
    { 1, 0, 30.0f, 0.0f, 0.0f, 0, 20 },  { 1, 0, 30.0f, 0.0f, 0.0f, 0, 21 },
    { 1, 0, 30.0f, 0.0f, 0.0f, 0, 22 },  { 1, 0, 30.0f, 0.0f, 0.0f, 0, 23 },
    { 1, 0, 30.0f, 0.0f, 0.0f, 0, 24 },  { 1, 0, 30.0f, 0.0f, 0.0f, 0, 25 },
    { 1, 0, 30.0f, 0.0f, 0.0f, 0, 26 },  { 1, 0, 30.0f, 0.0f, 0.0f, 0, 27 },
    { 1, 0, 30.0f, 0.0f, 0.0f, 0, 28 },  { 1, 0, 30.0f, 0.0f, 0.0f, 0, 29 },
    { 1, 0, 30.0f, 0.0f, 0.0f, 0, 30 },  { 1, 0, 2.0f, 1.0f, 0.0f, 0, 31 },
    { 1, 0, 30.0f, 0.2f, 0.0f, 0, 32 },  { 1, 0, 30.0f, 0.5f, 0.0f, 0, 33 },
    { 1, 0, 30.0f, 0.5f, 0.0f, 0, 34 },  { 1, 0, 30.0f, 0.5f, 0.0f, 0, 35 },
    { 1, 0, 30.0f, 0.5f, 0.0f, 0, 36 },
};

S32 sPlayerSurfaceType;
static F32 sPlayerWaterOffset;
S32 gPlayerHitWater;
S32 gDeadScooby;
static _zParEmitter* sEmitterStars;
static _zParEmitter* sEmitterShockLeft;
static _zParEmitter* sEmitterShockRight;
static _zParEmitter* sEmitterWaterSplash;
static _zParEmitter* sEmitterBounceMiss;
static _zParEmitter* sEmitterSprings;
static _zParEmitter* sEmitterSpringBoard;
static _zParEmitter* sEmitterBounceOnVillain;
static _zParEmitter* sEmitterHeadButt;
static _zParEmitter* sEmitterHeadButt2;
static U32 sSoundWaterSplash;
_tagFeetSkidMark* sPlayerCurSkidMark;
U32 sSnackDecrement;
_zEntPickup* sDecrementSnackGateEnt;
F32 sDecrementDelay;
F32 sDecrementAmount;
F32 startJump;
F32 startDouble;
F32 startBounce;
F32 umbrellaFudgeX = -0.15f;
F32 umbrellaFudgeY = 0.95f;
F32 umbrellaFudgeRot = PI / 2;
static F32 minVelmag = 0.01f;
static F32 maxVelmag = 10.0f;
static F32 curVelmag;
static F32 curVelangle;
static S32 surfSlickness = 1;
static F32 surfFriction = 1.0f;
static F32 surfDamping = minVelmag;
static S32 lastSlickness = 1;
static _xEnt* lastFloorEnt;
static U32 surfSticky;
static F32 surfSlideStart = DEG2RAD(20);
static F32 surfSlideStop = DEG2RAD(10);
static F32 surfSlickRatio;
static F32 surfSlickTimer;
static F32 surfPeakRatio = 1.25f;
static F32 surfAccelWalk = 4.0f;
static F32 surfAccelRun = 8.0f;
static F32 surfDecelIdle = 2.0f;
static F32 surfDecelSkid = 8.0f;
static F32 surfMaxSpeed;
static F32 surfSlipTimer;
static F32 update_dt;
S32 headcoll_anim;
F32 headcoll_tmr;
F32 headcoll_bltime;
static U32 cchkButtbounce;
static U32 cchkSquish;
static U32 sVO;
static U32 sVOAssetID;
static U32 sVOPriority;
static zPlayerSituation* sVONextSituation;
static F32 sVOTimer;
static RpAtomic* sPlayerBodyAtomic;
static RpAtomic* sPlayerHeadAtomic;
static RpClump* sPlayerBodyClump;
static RpClump* sPlayerHeadClump;

xVec3 gPlayerWaterPos;
xVec3 gDeadScoobyPos;
_tagPlayerFX gPlayerfx;
tagzFeet sPlayerFeet[10];
_tagFeetSkidMark sPlayerSkidMark[5];
static _tagPlayerSndFX sPlayerSndfx;
static xVec3 lastDeltaPos;
static xVec3 lastFloorNorm;
static xVec3 update_motion;
static xVec3 req_motion;
xVec3 vHangBefore;
xVec3 vHangFirst;
xVec3 vHangLast;
zGustData gust_data;
xMat4x3 gPlayerAbsMat;
xMat4x3 rendermat;
xVec3 floor_supp[4];
F32 floor_dist[4];
F32 floor_tmr[4];
_zParEmitterCustomSettings g_info;

static void zEntPlayer_FXLostChance(_xEnt* ent, F32 seconds);
void zEntPlayer_Notify(S32 situation);
void zEntPlayerJumpStart(_xEnt* ent, zJumpParam* jump);
static void PlayerHangUpdate(_xEnt* ent, F32 mag, F32 angle, F32 dt);

static inline F32 isin(F32 x)
{
    return sin(x);
}

static inline F32 icos(F32 x)
{
    return cos(x);
}

static inline void DampenControls(F32* angle, F32* mag, F32 x, F32 y)
{
    if (x > -globals.player.s.AnalogMin && x < globals.player.s.AnalogMin)
    {
        x = 0.0f;
    }

    if (y > -globals.player.s.AnalogMin && y < globals.player.s.AnalogMin)
    {
        y = 0.0f;
    }

    if (!x && !y)
    {
        *angle = 0.0f;
        *mag = 0.0f;
        return;
    }

    if (xabs(x) > xabs(y))
    {
        *mag = xabs(x);
    }
    else
    {
        *mag = xabs(y);
    }
    *mag = (*mag - globals.player.s.AnalogMin) /
           (globals.player.s.AnalogMax - globals.player.s.AnalogMin);

    if (*mag < 0.0f)
    {
        *mag = 0.0f;
        *angle = 0.0f;
        return;
    }

    if (*mag > 1.0f)
    {
        *mag = 1.0f;
    }
    *angle = xAngleClamp(atan2(x, y));
}

static inline void SlideLeanUpdate(F32 angle, F32 dt)
{
    F32 abs = xabs(angle);
    F32 lerp;
    F32 t;

    if (abs < dt)
    {
        lerp = 0.0f;
    }
    else if (abs > 5.0f * dt)
    {
        lerp = 1.0f;
    }
    else
    {
        lerp = (abs - dt) * (1.0f / (5.0f * dt - dt));
    }

    if (angle < 0.0f)
    {
        lerp = -lerp;
    }
    lerp += 1.0f;

    t = 6.0f * (lerp - globals.player.LeanLerp);
    globals.player.LeanLerp += t * dt;
}

static inline void LeanUpdate(F32 angle, F32 dt)
{
    F32 abs = xabs(angle);
    F32 lerp;
    F32 t;

    if (abs < DEG2RAD(10))
    {
        lerp = 0.0f;
    }
    else if (abs > DEG2RAD(55))
    {
        lerp = 1.0f;
    }
    else
    {
        lerp = 1.2732396f * (abs - DEG2RAD(10));
    }

    if (angle > 0.0f)
    {
        lerp = -lerp;
    }
    lerp += 1.0f;

    t = 6.0f * (lerp - globals.player.LeanLerp);
    globals.player.LeanLerp += t * dt;
}

static inline void CalcAnimSpeed(_xEnt* ent, F32 f, F32* pf)
{
    if (!pf[0])
    {
        return;
    }

    f = f / pf[0];
    if (f < pf[1])
    {
        f = pf[1];
    }
    else if (f > pf[2])
    {
        f = pf[2];
    }

    ent->model->Anim->Single->CurrentSpeed = f;
}

static void PlayerAbsControl(_xEnt* ent, F32 x, F32 z, F32 dt)
{
    U32 animUserFlag;
    U32 blendUserFlag;
    F32 angle;
    F32 mag;
    F32 dir_dp;
    F32 turnfactor;
    F32 diffAngle = 0.0f;
    F32 autodist2d = 0.0f;
    F32 camAngle;
    xVec3 euler;

    if (lbl_802289B8[0].state != 2)
    {
        z = x = 0.0f;
    }

    if (!globals.player.ControlOn)
    {
        z = x = 0.0f;
    }

    xMat3x3GetEuler((xMat3x3*)&ent->frame->mat, &euler);
    ent->frame->rot.angle = euler.x >= 0.0f ? euler.x : 2 * PI + euler.x;
    surfMaxSpeed = 0.0f;

    animUserFlag =
        ent->model->Anim->Single->State ? ent->model->Anim->Single->State->UserFlags : 0;
    blendUserFlag = ent->model->Anim->Single->Blend && ent->model->Anim->Single->Blend->State ?
                        ent->model->Anim->Single->Blend->State->UserFlags | 0x80000000 :
                        0;

    if ((animUserFlag & 0x1E) == 0xE && !(animUserFlag & 0x800))
    {
        animUserFlag &= ~0x1E;
    }
    if ((blendUserFlag & 0x1E) == 0xE && !(blendUserFlag & 0x800))
    {
        blendUserFlag &= ~0x1E;
    }

    if ((animUserFlag & 0x1E) == 0xC)
    {
        turnfactor = globals.player.s.HeadbuttTurn;
    }
    else
    {
        turnfactor = 7.0f;
    }

    memset(&ent->frame->dpos, 0, sizeof(xVec3));

    if (animUserFlag & 0x100)
    {
        return;
    }

    DampenControls(&angle, &mag, x, z);
    memcpy(&gPlayerAbsMat, &globals.camera.hi_cam.mat, sizeof(xMat4x3));

    if (globals.player.Slide == 1 &&
        (globals.player.JumpState == 0 || globals.player.JumpState == 1))
    {
        xVec3* N;
        xVec3 D;
        xVec3 X;
        xMat3x3 N_to_Up;
        xVec3 D_flat;
        F32 downAngle;

        zLaughNotify(8);

        N = &globals.player.floor_norm;
        D.x = -N->z;
        D.y = 0.0f;
        D.z = N->x;
        xVec3Normalize(&D, &D);
        xVec3Cross(&X, N, &D);
        xVec3Normalize(&X, &X);

        if (globals.player.SlideTimer >= 0.3f)
        {
            xMat3x3RotC(&N_to_Up, D.x, D.y, D.z, acos(N->y));
            xMat3x3RMulVec(&D_flat, &N_to_Up, &X);
            downAngle = xAngleClamp(atan2(D_flat.x, D_flat.z));

            diffAngle = downAngle - ent->frame->rot.angle;
            if (diffAngle > PI)
            {
                diffAngle -= 2 * PI;
            }
            else if (diffAngle < -PI)
            {
                diffAngle = 2 * PI + diffAngle;
            }

            if (xabs(diffAngle) > DEG2RAD(5))
            {
                ent->frame->drot.angle = 5.0f * diffAngle * dt;
                ent->frame->mode |= 0x20;
            }

            if (mag)
            {
                xVec3 delta;

                camAngle = xAngleClamp(atan2(gPlayerAbsMat.right.z, gPlayerAbsMat.right.x));
                angle -= camAngle;
                if (angle > 2 * PI)
                {
                    angle -= 2 * PI;
                }
                else if (angle < 0.0f)
                {
                    angle += 2 * PI;
                }

                mag *= 5.0f * dt;
                delta.x = mag * isin(angle);
                delta.y = 0.0f;
                delta.z = mag * icos(angle);
                dir_dp = xVec3Dot(&D, &delta);
                xVec3SMul(&ent->frame->dpos, &D, dir_dp);
                ent->frame->mode |= 0x2;
                SlideLeanUpdate(dir_dp, dt);
            }
            else
            {
                SlideLeanUpdate(0.0f, dt);
            }
        }
        else
        {
            SlideLeanUpdate(0.0f, dt);
        }

        ent->model->Anim->Single->BilinearLerp[0] = globals.player.LeanLerp;
        ent->model->Anim->Single->Blend->BilinearLerp[0] = globals.player.LeanLerp;

        if (xVec3Length2(&ent->frame->vel) < 49.0f)
        {
            xVec3SMul(&ent->frame->dvel, &X, dt * (0.45f * globals.sceneCur->gravity * X.y));
            ent->frame->mode |= 0x8;
        }
        return;
    }

    if (globals.player.HotsauceTimer != 0.0f)
    {
        globals.player.HotsauceTimer -= dt;
        if (globals.player.HotsauceTimer <= 0.0f)
        {
            globals.player.HotsauceTimer = 0.0f;
        }
        else
        {
            animUserFlag = 0x8;
            if (mag < globals.player.s.SpeedHotsauce[2])
            {
                mag = globals.player.s.SpeedHotsauce[2];
                dir_dp = 1.0f;
                angle = ent->frame->rot.angle;
                globals.player.Speed = 3;
                goto move;
            }
        }
    }

    if (globals.player.Shaggy1.GrabTimer)
    {
        diffAngle = globals.player.Shaggy1.GrabRot - ent->frame->rot.angle;
        if (diffAngle > PI)
        {
            diffAngle -= 2 * PI;
        }
        else if (diffAngle < -PI)
        {
            diffAngle = 2 * PI + diffAngle;
        }

        ent->frame->drot.angle = 4.0f * diffAngle * dt;
        ent->frame->mode |= 0x20;
        if (xabs(ent->frame->drot.angle) < 0.006f)
        {
            if (xabs(diffAngle) > 0.006f)
            {
                diffAngle = diffAngle > 0.0f ? 0.006f : -0.006f;
            }
            ent->frame->drot.angle = diffAngle;
        }
        goto lean;
    }

    if (globals.player.FireTarget)
    {
        xVec3 delta;
        xVec3 fwd;

        delta.x = globals.player.FireTarget->bound.sph.center.x - ent->bound.sph.center.x;
        delta.y = globals.player.FireTarget->bound.sph.center.y - ent->bound.sph.center.y;
        delta.z = globals.player.FireTarget->bound.sph.center.z - ent->bound.sph.center.z;
        xVec3Normalize(&delta, &delta);

        fwd.x = isin(ent->frame->rot.angle);
        fwd.y = 0.0f;
        fwd.z = icos(ent->frame->rot.angle);
        if (xVec3Dot(&fwd, &delta) < 0.34202f)
        {
            globals.player.FireTarget = NULL;
        }
        else
        {
            angle = xAngleClamp(atan2(delta.x, delta.z));
            if (angle > 2 * PI)
            {
                angle -= 2 * PI;
            }
            else if (angle < 0.0f)
            {
                angle += 2 * PI;
            }

            if (mag)
            {
                goto turn;
            }

            diffAngle = angle - ent->frame->rot.angle;
            if (diffAngle > PI)
            {
                diffAngle -= 2 * PI;
            }
            else if (diffAngle < -PI)
            {
                diffAngle = 2 * PI + diffAngle;
            }
            ent->frame->drot.angle = dt * (diffAngle * turnfactor);
            ent->frame->mode |= 0x20;
        }
    }

    if (globals.player.Shaggy1.Grab &&
        (globals.player.Shaggy1.ThrowState == 1 || globals.player.Shaggy1.ThrowState == 2))
    {
        xVec3 delta;
        xVec3 fwd;
        F32 tmpdrot;

        delta.x = globals.player.Shaggy1.ThrowTgtVec.x - ent->bound.sph.center.x;
        delta.y = globals.player.Shaggy1.ThrowTgtVec.y - ent->bound.sph.center.y;
        delta.z = globals.player.Shaggy1.ThrowTgtVec.z - ent->bound.sph.center.z;
        xVec3Normalize(&delta, &delta);

        fwd.x = isin(ent->frame->rot.angle);
        fwd.y = 0.0f;
        fwd.z = icos(ent->frame->rot.angle);
        dir_dp = xVec3Dot(&fwd, &delta);
        angle = xAngleClamp(atan2(delta.x, delta.z));
        if (angle > 2 * PI)
        {
            angle -= 2 * PI;
        }
        else if (angle < 0.0f)
        {
            angle += 2 * PI;
        }

        diffAngle = angle - ent->frame->rot.angle;
        if (diffAngle > PI)
        {
            diffAngle -= 2 * PI;
        }
        else if (diffAngle < -PI)
        {
            diffAngle = 2 * PI + diffAngle;
        }

        tmpdrot = diffAngle;
        if (xabs(diffAngle) <= 4 * PI * dt)
        {
            globals.player.Shaggy1.ThrowState = 2;
        }
        else
        {
            tmpdrot = diffAngle * turnfactor;
            if (xabs(tmpdrot) < 4 * PI)
            {
                if (tmpdrot > 0.0f)
                {
                    tmpdrot = 4 * PI * dt;
                }
                else
                {
                    tmpdrot = 4 * PI * -dt;
                }
            }
            else
            {
                tmpdrot *= dt;
            }
        }

        ent->frame->drot.angle = tmpdrot;
        ent->frame->mode |= 0x20;
        globals.player.Speed = 0;
        goto lean;
    }

    if (animUserFlag & 0x80)
    {
        angle = ent->frame->rot.angle;
        if (mag)
        {
            goto turn;
        }
    }

    if (!globals.player.ControlOn && globals.player.AutoMoveSpeed)
    {
        F32 dx;
        F32 dz;
        F32 turnang;

        dx = globals.player.AutoMoveTarget.x - ent->model->Mat->pos.x;
        dz = globals.player.AutoMoveTarget.z - ent->model->Mat->pos.z;
        autodist2d = sqrtf(dx * dx + dz * dz);
        if (autodist2d < 0.001f)
        {
            xBase* obj;

            globals.player.AutoMoveSpeed = 0;
            obj = globals.player.AutoMoveObject;
            zEntEvent(obj, ent, 0x1F);
            if (obj->baseType == 0xD)
            {
                zEntEvent(ent, obj, 0x1F);
            }
        }
        else
        {
            angle = xAngleClamp(atan2(dx, dz));
            if (angle > 2 * PI)
            {
                angle -= 2 * PI;
            }
            else if (angle < 0.0f)
            {
                angle += 2 * PI;
            }

            diffAngle = angle - ent->frame->rot.angle;
            if (diffAngle > PI)
            {
                diffAngle -= 2 * PI;
            }
            else if (diffAngle < -PI)
            {
                diffAngle = 2 * PI + diffAngle;
            }

            turnang = diffAngle * turnfactor;
            if (xabs(turnang) < DEG2RAD(11.25f))
            {
                turnang = turnang < 0.0f ? -DEG2RAD(11.25f) : DEG2RAD(11.25f);
            }
            turnang *= dt;
            if (xabs(turnang) > xabs(diffAngle))
            {
                turnang = diffAngle;
            }

            ent->frame->drot.angle = turnang;
            ent->frame->mode |= 0x20;

            if (globals.player.AutoMoveSpeed == 4)
            {
                if (turnang == diffAngle)
                {
                    xBase* obj;

                    globals.player.AutoMoveSpeed = 0;
                    obj = globals.player.AutoMoveObject;
                    zEntEvent(obj, ent, 0x1F);
                    if (obj->baseType == 0xD)
                    {
                        zEntEvent(ent, obj, 0x1F);
                    }
                }
                mag = dir_dp = 0.0f;
            }
            else if (autodist2d <= globals.player.AutoMoveDist)
            {
                xBase* obj;

                globals.player.AutoMoveSpeed = 0;
                obj = globals.player.AutoMoveObject;
                zEntEvent(obj, ent, 0x1F);
                if (obj->baseType == 0xD)
                {
                    zEntEvent(ent, obj, 0x1F);
                }
            }
            else
            {
                mag = 1.0f;
                dir_dp = icos(diffAngle);
            }
            goto speed;
        }
    }

    if (mag)
    {
        camAngle = xAngleClamp(atan2(gPlayerAbsMat.right.z, gPlayerAbsMat.right.x));
        angle -= camAngle;
        if (angle > 2 * PI)
        {
            angle -= 2 * PI;
        }
        else if (angle < 0.0f)
        {
            angle += 2 * PI;
        }

    turn:
        if (!(animUserFlag & 0x800))
        {
            diffAngle = angle - ent->frame->rot.angle;
            if (diffAngle > PI)
            {
                diffAngle -= 2 * PI;
            }
            else if (diffAngle < -PI)
            {
                diffAngle = 2 * PI + diffAngle;
            }

            dir_dp = icos(diffAngle);
            ent->frame->drot.angle = dt * (diffAngle * turnfactor);
            ent->frame->mode |= 0x20;
        }

    speed:
        if (animUserFlag & 0x800)
        {
            dir_dp = 1.0f;
        }

        if (surfSticky && !(globals.player.Inv_Specials & 0x4))
        {
            if (dir_dp > 0.2f)
            {
                globals.player.Speed = 1;
            }
            else
            {
                globals.player.Speed = 0;
            }
        }
        else if (globals.player.HotsauceTimer)
        {
            globals.player.Speed = 3;
        }
        else if (((globals.player.ControlOn && (globals.pad0->on & 0x1000)) ||
                  globals.player.AutoMoveSpeed == 2) &&
                 !globals.player.Sneak)
        {
            globals.player.Speed = 2;
        }
        else if (dir_dp > 0.2f || globals.player.Speed)
        {
            globals.player.Speed = 1;
        }
    }
    else
    {
        dir_dp = 0.0f;
        globals.player.Speed = 0;
    }

move:
    ent->frame->mode &= ~0x2;

    if (globals.player.HangEnt)
    {
        zLaughNotify(7);
        PlayerHangUpdate(ent, mag, angle, dt);
        return;
    }

    if ((animUserFlag & 0x1E) || (blendUserFlag & 0x1E))
    {
        U32 moveFlag;

        moveFlag = animUserFlag & 0x1E;
        if (!moveFlag)
        {
            moveFlag = blendUserFlag & 0x1E;
        }

        switch (moveFlag)
        {
        case 0x2:
            if (dir_dp <= 0.0f || mag == 0.0f)
            {
                goto lean;
            }
            maxVelmag = globals.player.s.SpeedSneak[1];
            mag *= globals.player.s.SpeedSneak[1] * dir_dp;
            break;
        case 0x4:
            if (dir_dp <= 0.0f)
            {
                goto lean;
            }
            maxVelmag = globals.player.s.SpeedWalk[1];
            mag *= globals.player.s.SpeedWalk[1] * dir_dp;
            if (surfSticky && !(globals.player.Inv_Specials & 0x4))
            {
                mag *= 0.667f;
            }
            break;
        case 0x6:
            if (mag)
            {
                if (xabs(diffAngle) >= DEG2RAD(30))
                {
                    if (diffAngle > 0.0f)
                    {
                        angle = DEG2RAD(30) + ent->frame->rot.angle;
                    }
                    else
                    {
                        angle = ent->frame->rot.angle - DEG2RAD(30);
                    }
                }
            }
            else
            {
                angle = ent->frame->rot.angle;
            }

            if (animUserFlag & 0x20)
            {
                if (surfSlickRatio)
                {
                    goto lean;
                }
                mag = globals.player.DecelRunSpeed;
                globals.player.DecelRunSpeed -= 1.3333334f * (globals.player.DecelRun * dt);
                if (globals.player.DecelRunSpeed < 0.0f)
                {
                    globals.player.DecelRunSpeed = 0.0f;
                }
            }
            else
            {
                if (mag == 0.0f)
                {
                    goto lean;
                }
                maxVelmag = globals.player.s.SpeedRun[1];
                mag = mag >= globals.player.s.SpeedRun[3] ? globals.player.s.SpeedRun[1]
                    : mag <= globals.player.s.SpeedRun[2] ? globals.player.s.SpeedRun[0]
                    : (mag - globals.player.s.SpeedRun[2]) / (globals.player.s.SpeedRun[3] - globals.player.s.SpeedRun[2]) * (globals.player.s.SpeedRun[1] - globals.player.s.SpeedRun[0]) + globals.player.s.SpeedRun[0];
                globals.player.DecelRun = globals.player.DecelRunSpeed = mag;
            }
            break;
        case 0x8:
            maxVelmag = globals.player.s.SpeedHotsauce[1];
            mag = mag >= globals.player.s.SpeedHotsauce[3] ? globals.player.s.SpeedHotsauce[1]
                : mag <= globals.player.s.SpeedHotsauce[2] ? globals.player.s.SpeedHotsauce[0]
                : (mag - globals.player.s.SpeedHotsauce[2]) / (globals.player.s.SpeedHotsauce[3] - globals.player.s.SpeedHotsauce[2]) * (globals.player.s.SpeedHotsauce[1] - globals.player.s.SpeedHotsauce[0]) + globals.player.s.SpeedHotsauce[0];
            globals.player.DecelRun = globals.player.DecelRunSpeed = mag;
            break;
        case 0xA:
        case 0xE:
            if (dir_dp <= 0.0f && !globals.player.cheat_mode)
            {
                goto lean;
            }
            maxVelmag = globals.player.s.SpeedAir[1];
            mag *= globals.player.s.SpeedAir[1] * dir_dp;
            break;
        case 0xC:
            angle = ent->frame->rot.angle;
            mag = globals.player.HeadbuttVel;
            globals.player.DecelRun = globals.player.DecelRunSpeed = mag;
            break;
        }

        switch (animUserFlag & 0x1E)
        {
        case 0x2:
            if (!strcmp(ent->model->Anim->Single->State->Name, "Walk_sneak") ||
                !strcmp(ent->model->Anim->Single->State->Name, "Walk_blackknight"))
            {
                CalcAnimSpeed(ent, mag, globals.player.s.AnimSneak);
            }
            break;
        case 0x4:
            CalcAnimSpeed(ent, mag, globals.player.s.AnimWalk);
            if (ent->model->Anim->Single->State->Speed != 1.0f)
            {
                ent->model->Anim->Single->CurrentSpeed = ent->model->Anim->Single->State->Speed;
            }
            break;
        }

        if (surfSlickRatio && globals.player.ControlOn)
        {
            F32 slickLerp;
            F32 peakLerp;
            F32 accelMag;

            if (mag == 0.0f)
            {
                goto lean;
            }

            slickLerp = 1.3333334f * surfSlipTimer;
            accelMag = (1.0f - slickLerp) * surfSlickRatio + 20.0f * slickLerp * surfSlickRatio;
            if (moveFlag == 0x4 || moveFlag == 0x2)
            {
                accelMag = surfAccelWalk * accelMag;
                surfMaxSpeed = globals.player.s.SpeedWalk[1] * surfPeakRatio;
            }
            else
            {
                accelMag = surfAccelRun * accelMag;
                surfMaxSpeed = globals.player.s.SpeedRun[1] * surfPeakRatio;
            }

            ent->frame->vel.x += dt * (accelMag * isin(angle));
            ent->frame->vel.z += dt * (accelMag * icos(angle));

            peakLerp = 2.5f * surfSlipTimer;
            if (peakLerp >= 1.0f)
            {
                surfMaxSpeed = mag;
            }
            else
            {
                surfMaxSpeed = (1.0f - peakLerp) * surfMaxSpeed + peakLerp * mag;
            }
        }
        else
        {
            mag *= dt;
            if (globals.player.cheat_mode)
            {
                mag *= 3.0f;
            }

            ent->frame->dpos.x = mag * isin(angle);
            ent->frame->dpos.z = mag * icos(angle);
            ent->frame->mode |= 0x2;

            if (!globals.player.ControlOn && globals.player.AutoMoveSpeed &&
                globals.player.AutoMoveSpeed != 4 &&
                ent->frame->dpos.x * ent->frame->dpos.x +
                        ent->frame->dpos.z * ent->frame->dpos.z >=
                    autodist2d * autodist2d)
            {
                xBase* obj;

                ent->frame->dpos.x = globals.player.AutoMoveTarget.x - ent->model->Mat->pos.x;
                ent->frame->dpos.z = globals.player.AutoMoveTarget.z - ent->model->Mat->pos.z;
                globals.player.AutoMoveSpeed = 0;
                obj = globals.player.AutoMoveObject;
                zEntEvent(obj, ent, 0x1F);
                if (obj->baseType == 0xD)
                {
                    zEntEvent(ent, obj, 0x1F);
                }
            }
        }
    }

lean:
    LeanUpdate(diffAngle, dt);
    if (animUserFlag & 0x40)
    {
        ent->model->Anim->Single->BilinearLerp[0] = globals.player.LeanLerp;
    }
    if (blendUserFlag & 0x40)
    {
        ent->model->Anim->Single->Blend->BilinearLerp[0] = globals.player.LeanLerp;
    }

    if (globals.player.Shaggy1.GrabTimer != 0.0f)
    {
        ent->frame->mode &= ~0x6;
    }
}

static S32 GetHeadcollAnimState()
{
    xAnimState* state = globals.player.ent.model->Anim->Single->State;
    U32 type = state->UserFlags & 0x1E;

    if (type == 0xC)
    {
        return 1;
    }
    if (type == 0x4)
    {
        return 2;
    }
    if (type == 0x6 && type != 0x20)
    {
        return 3;
    }

    static U32 jumpNormal = xStrHash("Jump_normal");
    static U32 jumpDouble = xStrHash("Jump_double");
    static U32 jumpWalk = xStrHash("Jump_walk");
    static U32 jumpRun = xStrHash("Jump_run");
    static U32 jumpSpringboard = xStrHash("Jump_springboard");
    static U32 fallNormal = xStrHash("Fall_normal");
    static U32 fallWalk = xStrHash("Fall_walk");
    static U32 fallRun = xStrHash("Fall_run");

    if (state->ID == jumpNormal)
    {
        return globals.player.ent.frame->vel.y <= 0.0f ? 9 : 4;
    }
    if (state->ID == jumpDouble)
    {
        return globals.player.ent.frame->vel.y <= 0.0f ? 9 : 5;
    }
    if (state->ID == jumpWalk)
    {
        return globals.player.ent.frame->vel.y <= 0.0f ? 10 : 6;
    }
    if (state->ID == jumpRun)
    {
        return globals.player.ent.frame->vel.y <= 0.0f ? 11 : 7;
    }
    if (state->ID == jumpSpringboard)
    {
        return globals.player.ent.frame->vel.y <= 0.0f ? 9 : 8;
    }
    if (state->ID == fallNormal)
    {
        return 9;
    }
    if (state->ID == fallWalk)
    {
        return 10;
    }
    return state->ID == fallRun ? 11 : 0;
}

static void HeadcollUpdate(F32 dt)
{
    S32 anim = GetHeadcollAnimState();

    if (anim != headcoll_anim)
    {
        headcoll_fromos = headcoll_os;
        if (anim)
        {
            headcoll_bltime = headcoll_bltimes[anim];
        }
        else
        {
            headcoll_bltime = headcoll_bltimes[headcoll_anim];
        }
        headcoll_anim = anim;
        headcoll_tmr = headcoll_bltime;
    }
    if (headcoll_tmr > 0.0f)
    {
        headcoll_tmr -= dt;
        if (headcoll_tmr <= 0.0f)
        {
            headcoll_tmr = 0.0f;
            headcoll_os = headcoll_oses[headcoll_anim];
        }
        else
        {
            F32 lerp = headcoll_tmr / headcoll_bltime;
            headcoll_os.x = headcoll_fromos.x * lerp + headcoll_oses[headcoll_anim].x * (1.0f - lerp);
            headcoll_os.y = headcoll_fromos.y * lerp + headcoll_oses[headcoll_anim].y * (1.0f - lerp);
            headcoll_os.z = headcoll_fromos.z * lerp + headcoll_oses[headcoll_anim].z * (1.0f - lerp);
        }
    }
}

static void HeadbuttTest(_xCollis* coll, _xCollis* endcoll, F32 angle_cosine,
                         S32 (*headbuttCB)(_zEnt*, _xCollis*))
{
    _zEnt* ent = &globals.player.ent;
    F32 dx;
    F32 dz;
    F32 mag;
    _zEnt* villent;
    F32 dx2;
    F32 dz2;

    if ((ent->model->Anim->Single->State->UserFlags & 0x1E) != 0xC)
    {
        return;
    }
    dx = isin(ent->frame->rot.angle);
    dz = icos(ent->frame->rot.angle);
    for (; coll < endcoll; coll++)
    {
        villent = (_zEnt*)coll->optr;
        if (fabsf(coll->hdng.y) < 0.7071f)
        {
            dx2 = villent->frame->mat.pos.x - ((xMat4x3*)ent->model->Mat)->pos.x;
            dz2 = villent->frame->mat.pos.z - ((xMat4x3*)ent->model->Mat)->pos.z;
            mag = sqrtf(dx2 * dx2 + dz2 * dz2);
            if (mag > 0.0000001f && dx * dx2 + dz * dz2 > angle_cosine * mag)
            {
                if (headbuttCB(ent, coll))
                {
                    return;
                }
            }
        }
    }
}

static S32 VillHBCallback(_zEnt* ent, _xCollis* coll)
{
    S32 headResponse;

    if (((_xEnt*)coll->optr)->subType == 0x24)
    {
        return Shaggy8_HBCallback(ent, coll);
    }
    headResponse = zEntVillain_Headbutt((st_zEntVillain*)coll->optr, &coll->hdng);
    if (headResponse)
    {
        F32 pitch;

        ent->frame->mat.pos.x = ((xMat4x3*)ent->model->Mat)->pos.x;
        ent->frame->mat.pos.y = ((xMat4x3*)ent->model->Mat)->pos.y;
        ent->frame->mat.pos.z = ((xMat4x3*)ent->model->Mat)->pos.z;
        globals.player.HeadbuttHit = 1;
        globals.player.DecelRunSpeed = 0.25f * globals.player.DecelRunSpeed;
        sPlayerSndfx.head_pitch = zPitchChange(sPlayerSndfx.head_pitch, &pitch, 30, 1.5f);
        xSndPlay(sPlayerSndfx.head_id[sPlayerSndfx.head_snd], 0.5f, 0.5f, pitch, 0x80, 2, 0);
        S16 snd = sPlayerSndfx.head_snd;
        while (snd == sPlayerSndfx.head_snd)
        {
            snd = rand() % 3;
        }
        sPlayerSndfx.head_snd = snd;
        zRumbleStart(0, SDR_Headbutt);
    }
    return 0;
}

static U32 HealthCheck(xAnimTransition* tran, xAnimSingle* anim, void* param_3)
{
    return tran->UserFlags & (1 << (globals.player.Health < 3 ? globals.player.Health : 3));
}

static U32 PlungerCheck(xAnimTransition* tran, xAnimSingle* anim, void* param_3)
{
    return globals.player.Speed == 0 && globals.player.Slide == 2;
}

static U32 TeeterCheck(xAnimTransition* tran, xAnimSingle* anim, void* param_3)
{
    return globals.player.Speed == 0 && globals.player.Teeter && globals.player.Slide != 2 &&
           !(globals.player.SlideTimer > 0.0f) && !surfSlickTimer;
}

static U32 SlipperyCheck(xAnimTransition* tran, xAnimSingle* anim, void* param_3)
{
    return globals.player.Speed == 0 && globals.player.Slide == 0 && surfSlickTimer != 0.0f;
}

static U32 HotsauceCheck(xAnimTransition* tran, xAnimSingle* anim, void* param_3)
{
    return globals.player.Speed == 3;
}

static inline U32 MoveSurfaceCheck(xAnimTransition* tran, xAnimSingle* anim, void* param_3)
{
    if (globals.player.Slide == 2)
    {
        return tran->UserFlags & 0x20;
    }
    if (surfSlickTimer != 0.0f)
    {
        return tran->UserFlags & 0x10;
    }
    if (surfSticky)
    {
        if (globals.player.Inv_Specials & 0x4)
        {
            return tran->UserFlags & 0x80;
        }
        return tran->UserFlags & 0x40;
    }
    return HealthCheck(tran, anim, param_3);
}

static U32 RunCheck(xAnimTransition* tran, xAnimSingle* anim, void* param_3)
{
    return globals.player.Speed == 2 && !globals.player.Sneak &&
           MoveSurfaceCheck(tran, anim, param_3) && globals.player.Slide == 0;
}

static U32 NotRunCheck(xAnimTransition* tran, xAnimSingle* anim, void* param_3)
{
    return (globals.player.Speed < 2 || globals.player.Sneak) &&
           MoveSurfaceCheck(tran, anim, param_3);
}

static U32 NotRunCB(xAnimTransition* tran, xAnimSingle* anim, void* param_3)
{
    globals.player.Face_PantTimer = 2.0f;
    return 0;
}

static U32 WalkCheck(xAnimTransition* tran, xAnimSingle* anim, void* param_3)
{
    return globals.player.Speed == 1 && MoveSurfaceCheck(tran, anim, param_3) &&
           (globals.player.Slide == 0 || globals.player.Slide == 3);
}

static U32 StickySurfIdle(xAnimTransition* tran, xAnimSingle* anim, void* param_3)
{
    return globals.player.Speed == 0 && surfSticky;
}

static U32 StickySurfWalk(xAnimTransition* tran, xAnimSingle* anim, void* param_3)
{
    return globals.player.Speed == 1 && surfSticky;
}

static U32 StickNoBootsIdle(xAnimTransition* tran, xAnimSingle* anim, void* param_3)
{
    return (globals.player.Speed == 0 && surfSticky && !(globals.player.Inv_Specials & 0x4)) ||
           globals.player.Slide == 1 || globals.player.Slide == 3;
}

static U32 StickNoBootsWalk(xAnimTransition* tran, xAnimSingle* anim, void* param_3)
{
    return (globals.player.Speed == 1 && surfSticky && !(globals.player.Inv_Specials & 0x4)) ||
           globals.player.Slide == 1 || globals.player.Slide == 3;
}

static U32 WalkPlungeCheck(xAnimTransition* tran, xAnimSingle* anim, void* param_3)
{
    return globals.player.Slide == 2 && (globals.player.Speed == 1 || globals.player.Speed == 2);
}

static U32 IdleCheck(xAnimTransition* tran, xAnimSingle* anim, void* param_3)
{
    return (globals.player.Speed == 0 && globals.player.Slide == 0 && surfSlickTimer == 0.0f &&
            !globals.player.Teeter) &&
           HealthCheck(tran, anim, param_3);
}

static U32 SkidStopCheck(xAnimTransition* tran, xAnimSingle* anim, void* param_3)
{
    return globals.player.ent.frame->vel.x * globals.player.ent.frame->vel.x +
               globals.player.ent.frame->vel.z * globals.player.ent.frame->vel.z <
           0.5f;
}

static U32 SneakMoveCheck(xAnimTransition* tran, xAnimSingle* anim, void* param_3)
{
    return (1 << globals.player.Sneak) & tran->UserFlags &&
           (globals.player.Speed == 1 || globals.player.Speed == 2);
}

static U32 SneakIdleCheck(xAnimTransition* tran, xAnimSingle* anim, void* param_3)
{
    return (1 << globals.player.Sneak) & tran->UserFlags && globals.player.Speed == 0;
}

static U32 NosneakCheck(xAnimTransition* tran, xAnimSingle* anim, void* param_3)
{
    return !globals.player.Sneak && HealthCheck(tran, anim, param_3);
}

static U32 NosneakTeeter(xAnimTransition* tran, xAnimSingle* anim, void* param_3)
{
    return !globals.player.Sneak && TeeterCheck(tran, anim, param_3);
}

static U32 NosneakSlippery(xAnimTransition* tran, xAnimSingle* anim, void* param_3)
{
    return !globals.player.Sneak && SlipperyCheck(tran, anim, param_3);
}

static U32 NosneakPlunger(xAnimTransition* tran, xAnimSingle* anim, void* param_3)
{
    return !globals.player.Sneak && PlungerCheck(tran, anim, param_3);
}

static U32 ScareCheck(xAnimTransition* tran, xAnimSingle* anim, void* param_3)
{
    return globals.player.ScareTimer && HealthCheck(tran, anim, param_3);
}

static U32 CowerCheck(xAnimTransition* tran, xAnimSingle* anim, void* param_3)
{
    return 0.0f != globals.player.CowerTimer;
}

static U32 UncowerCheck(xAnimTransition* tran, xAnimSingle* anim, void* param_3)
{
    return 0.0f == globals.player.CowerTimer && HealthCheck(tran, anim, param_3);
}

static U32 ScareCheckNoHealth(xAnimTransition* tran, xAnimSingle* anim, void* param_3)
{
    return 0.0f != globals.player.ScareTimer;
}

static U32 ScareCheckCritter(xAnimTransition* tran, xAnimSingle* anim, void* param_3)
{
    return 0.0f != globals.player.ScareTimer && globals.player.ScareSource &&
           globals.player.ScareSource->baseType == 2 &&
           (((_xEnt*)globals.player.ScareSource)->subType == 0x15 ||
            ((_xEnt*)globals.player.ScareSource)->subType == 0x16 ||
            ((_xEnt*)globals.player.ScareSource)->subType == 0x18);
}

static U32 ScareCB(xAnimTransition* tran, xAnimSingle* anim, void* param_3)
{
    globals.player.ScareTimer = 0.0f;
    globals.player.CowerTimer = 0.0f;
    if (globals.player.JumpState)
    {
        globals.player.JumpState = 2;
        globals.player.Jump_CanFloat = 0;
        globals.player.Jump_CanDouble = 0;
    }
    zEntPlayer_FXLostChance(&globals.player.ent, 0.03f);
    zRumbleStart(0, SDR_GotScared);
    return 0;
}

static U32 ScareKnockbackCB(xAnimTransition* tran, xAnimSingle* anim, void* param_3)
{
    xVec3 knockback;

    knockback.x = ((xMat4x3*)globals.player.ent.model->Mat)->at.x;
    knockback.y = 0.0f;
    knockback.z = ((xMat4x3*)globals.player.ent.model->Mat)->at.z;
    xVec3Normalize(&knockback, &knockback);
    knockback.x *= -4.0f;
    knockback.y = 15.0f;
    knockback.z *= -4.0f;
    globals.player.ent.frame->vel = knockback;
    ScareCB(tran, anim, param_3);
    return 0;
}

static U32 LandCheck(xAnimTransition* tran, xAnimSingle* anim, void* param_3)
{
    return globals.player.JumpState == 0;
}

static U32 ScareLandCheck(xAnimTransition* tran, xAnimSingle* anim, void* param_3)
{
    return globals.player.JumpState == 0 || globals.player.DamageTimer < 0.5f;
}

static U32 LandSlowCheck(xAnimTransition* tran, xAnimSingle* anim, void* param_3)
{
    return globals.player.JumpState == 0 && globals.player.Speed <= 0;
}

static U32 LandWalkCheck(xAnimTransition* tran, xAnimSingle* anim, void* param_3)
{
    return globals.player.JumpState == 0 && globals.player.Speed == 1;
}

static U32 LandFastCheck(xAnimTransition* tran, xAnimSingle* anim, void* param_3)
{
    return globals.player.JumpState == 0 && globals.player.Speed >= 2;
}

static U32 LandHealthCheck(xAnimTransition* tran, xAnimSingle* anim, void* param_3)
{
    return globals.player.JumpState == 0 && HealthCheck(tran, anim, param_3);
}

static U32 FallingCheck(xAnimTransition* tran, xAnimSingle* anim, void* param_3)
{
    return globals.player.JumpState == 2;
}

static U32 IdleMinorCheck(xAnimTransition* tran, xAnimSingle* anim, void* param_3)
{
    return globals.player.IdleMinorTimer > 3.0f &&
           (tran->UserFlags & 0xFFFF) == globals.player.IdleRand % (tran->UserFlags >> 16);
}

static U32 IdleMajorCheck(xAnimTransition* tran, xAnimSingle* anim, void* param_3)
{
    return globals.player.IdleMajorTimer > 9.0f &&
           (tran->UserFlags & 0xFFFF) == globals.player.IdleRand % (tran->UserFlags >> 16);
}

static U32 IdleSitCheck(xAnimTransition* tran, xAnimSingle* anim, void* param_3)
{
    return globals.player.IdleSitTimer > 11.0f;
}

static U32 IdleGetupCheck(xAnimTransition* tran, xAnimSingle* anim, void* param_3)
{
    return globals.player.IdleSitTimer > 11.0f && anim->Blend->State == NULL;
}

static U32 IdleMinorCB(xAnimTransition* tran, xAnimSingle* anim, void* param_3)
{
    globals.player.IdleMinorTimer = -1.5f;
    globals.player.IdleRand = irand() >> 16;
    return 0;
}

static U32 IdleMajorCB(xAnimTransition* tran, xAnimSingle* anim, void* param_3)
{
    globals.player.IdleMinorTimer = 0.0001f;
    globals.player.IdleMajorTimer = 0.0f;
    globals.player.IdleRand = irand() >> 16;
    return 0;
}

static U32 IdleSitCB(xAnimTransition* tran, xAnimSingle* anim, void* param_3)
{
    globals.player.IdleMinorTimer = 0.0001f;
    globals.player.IdleMajorTimer = 0.0f;
    globals.player.IdleSitTimer = -20.0f;
    return 0;
}

static U32 IdleGetupCB(xAnimTransition* tran, xAnimSingle* anim, void* param_3)
{
    globals.player.IdleMinorTimer = 0.0001f;
    globals.player.IdleMajorTimer = 0.0f;
    globals.player.IdleSitTimer = -15.0f;
    return 0;
}

static U32 ActiveCheck(xAnimTransition* tran, xAnimSingle* anim, void* param_3)
{
    return 0.0f == globals.player.IdleMinorTimer && HealthCheck(tran, anim, param_3);
}

static U32 JumpCheck(xAnimTransition* tran, xAnimSingle* anim, void* param_3)
{
    return (globals.player.Slide != 1 || globals.player.SlideTimer >= 0.5f ||
            (globals.player.ent.collis->colls[0].optr &&
             ((xBase*)globals.player.ent.collis->colls[0].optr)->baseType == 6 &&
             ((_xEnt*)globals.player.ent.collis->colls[0].optr)->subType == 11)) &&
           globals.player.Slide != 3 && (!surfSticky || globals.player.Inv_Specials & 0x4) &&
           globals.player.ControlOn && globals.pad0->pressed & 0x10000;
}

static U32 JumpCB(xAnimTransition* tran, xAnimSingle* anim, void* param_3)
{
    zEntPlayerJumpStart(&globals.player.ent, &globals.player.s.Jump);
    startDouble = startJump = globals.player.ent.frame->mat.pos.y;
    return 0;
}

static U32 JumpHangCB(xAnimTransition* tran, xAnimSingle* anim, void* param_3)
{
    if (globals.player.HangEnt)
    {
        globals.player.HangEnt->grabTimer = globals.player.HangEnt->hangInfo->grabDelay;
        globals.player.HangEnt = NULL;
        globals.player.RootUpTarget.x = 0.0f;
        globals.player.RootUpTarget.y = 1.0f;
        globals.player.RootUpTarget.z = 0.0f;
        globals.player.HangElapsed = 0.0f;
        zEntPlayer_Notify(9);
    }
    globals.player.Jump_CanDouble = 1;
    globals.player.Jump_CanFloat = 1;
    zEntPlayerJumpStart(&globals.player.ent, &globals.player.s.Jump);
    startJump = startDouble = globals.player.ent.frame->mat.pos.y;
    return 0;
}

static U32 DblJumpCheck(xAnimTransition* tran, xAnimSingle* anim, void* param_3)
{
    return globals.player.ControlOn && globals.pad0->pressed & 0x10000 &&
           globals.player.Inv_Specials & 0x200 && globals.player.Jump_CanDouble;
}

static U32 DblJumpCheckTime(xAnimTransition* tran, xAnimSingle* anim, void* param_3)
{
    return anim->Time > 0.1f && globals.player.ControlOn && globals.pad0->pressed & 0x10000 &&
           globals.player.Inv_Specials & 0x200 && globals.player.Jump_CanDouble;
}

static U32 DblJumpCB(xAnimTransition* tran, xAnimSingle* anim, void* param_3)
{
    zEntPlayerJumpStart(&globals.player.ent, &globals.player.s.Double);
    globals.player.Jump_CanDouble = 0;
    startDouble = globals.player.ent.frame->mat.pos.y;
    return 0;
}

static U32 BounceCheck(xAnimTransition* tran, xAnimSingle* anim, void* param_3)
{
    return globals.player.Bounced == 1;
}

static U32 BounceCB(xAnimTransition* tran, xAnimSingle* anim, void* param_3)
{
    globals.player.Bounced = 0;
    globals.player.Jump_CanDouble = 1;
    globals.player.Jump_CanFloat = 1;
    return 0;
}

static U32 ButtSmashLandCB(xAnimTransition* tran, xAnimSingle* anim, void* param_3)
{
    if (globals.player.Inv_Specials & 0x4000)
    {
        if (sEmitterStars)
        {
            S32 i;

            g_info.custom_flags = 0x205E1;
            g_info.pos.x = ((xMat4x3*)globals.player.ent.model->Mat)->pos.x +
                           globals.player.ent.collis->colls[0].dist *
                               globals.player.ent.collis->colls[0].tohit.x;
            g_info.pos.y = 0.5f + (((xMat4x3*)globals.player.ent.model->Mat)->pos.y +
                                   globals.player.ent.collis->colls[0].dist *
                                       globals.player.ent.collis->colls[0].tohit.y);
            g_info.pos.z = ((xMat4x3*)globals.player.ent.model->Mat)->pos.z +
                           globals.player.ent.collis->colls[0].dist *
                               globals.player.ent.collis->colls[0].tohit.z;
            g_info.size_birth = 0.0f;
            g_info.size_death = 0.5f;
            g_info.color_death[0] = 0x40;
            g_info.color_death[1] = 0x40;
            g_info.color_death[2] = 0x40;
            g_info.color_death[3] = 0;
            g_info.life = 0.25f;
            g_info.vel.x = 0.0f;
            g_info.vel.y = 0.02f;
            g_info.vel.z = 0.0f;
            g_info.xzcircle_radius = 0.2f;
            for (i = 0; i < 2; i++)
            {
                if (i == 0)
                {
                    g_info.color_birth[0] = 0x80;
                    g_info.color_birth[1] = 0x80;
                    g_info.color_birth[2] = 0x80;
                    g_info.color_birth[3] = 0x40;
                }
                else
                {
                    g_info.color_birth[0] = 0x80;
                    g_info.color_birth[1] = 0x80;
                    g_info.color_birth[2] = 0x40;
                    g_info.color_birth[3] = 0x60;
                }
                zParEmitterEmitCustom(sEmitterStars, 1.0f, &g_info);
            }
        }
        globals.player.ShockRadius = 0.6f;
        globals.player.ShockRadiusOld = 0.0f;
        zLaughNotify(3);
    }
    else
    {
        zLaughNotify(2);
    }
    return 0;
}

static U32 SpringCheck(xAnimTransition* tran, xAnimSingle* anim, void* param_3)
{
    return globals.player.Bounced == 2;
}

static U32 NospringCheck(xAnimTransition* tran, xAnimSingle* anim, _zEnt* ent)
{
    return ent->frame->vel.y < 10.0f;
}

static U32 DigCheck(xAnimTransition* tran, xAnimSingle* anim, void* param_3)
{
    return globals.player.Diggable && globals.player.JumpState == 0 && globals.player.ControlOn &&
           globals.pad0->pressed & 0x20000 && globals.player.Inv_Specials & 0x2000;
}

static U32 DigCB(xAnimTransition* tran, xAnimSingle* anim, void* param_3)
{
    globals.player.DigTimer = 0.00001f;
    zEntEvent(globals.player.Diggable, 0xC1);
    return 0;
}

static U32 StopDigCheck(xAnimTransition* tran, xAnimSingle* anim, void* param_3)
{
    return !globals.player.DigTimer && HealthCheck(tran, anim, param_3);
}

static U32 ProjectileCheck(xAnimTransition* tran, xAnimSingle* anim, void* param_3)
{
    if (globals.player.JumpState == 0)
    {
        if (globals.player.ControlOn && globals.pad0->pressed & 0x200 &&
            globals.player.Inv_Specials & 0x2 && globals.player.Inv_Ammo[1])
        {
            globals.player.LastProjectile = 1;
            return 1;
        }
        if (globals.player.ControlOn && globals.pad0->pressed & 0x100 &&
            globals.player.Inv_Specials & 0x1 && globals.player.Inv_Ammo[0])
        {
            globals.player.LastProjectile = 0;
            return 1;
        }
    }
    return 0;
}

static U32 ProjectileRepeatSync(xAnimTransition* tran, xAnimSingle* anim, void* param_3)
{
    if (anim->Time >= globals.player.s.FirePress && anim->Time < globals.player.s.FireRepeat &&
        ProjectileCheck(tran, anim, param_3))
    {
        return 1;
    }
    return 0;
}

static U32 ProjectileRepeat(xAnimTransition* tran, xAnimSingle* anim, void* param_3)
{
    if (anim->Time >= globals.player.s.FireRepeat && ProjectileCheck(tran, anim, param_3))
    {
        return 1;
    }
    return 0;
}

static U32 ProjectileCB(xAnimTransition* tran, xAnimSingle* anim, void* param_3)
{
    zEntBubble_Launch(globals.player.s.FireDelay);
    return 0;
}

static U32 AttackCheck(xAnimTransition* tran, xAnimSingle* anim, void* param_3)
{
    if (globals.player.ControlOn && globals.pad0->pressed & 0x80000 &&
        globals.player.JumpState == 0 && (!surfSticky || globals.player.Inv_Specials & 0x4) &&
        globals.player.Slide != 1 && globals.player.Slide != 3)
    {
        return 1;
    }
    return 0;
}

static U32 AttackCB(xAnimTransition* tran, xAnimSingle* anim, void* param_3)
{
    globals.player.HeadbuttVel = globals.player.s.HeadbuttSpeedMin;
    globals.player.HeadbuttTimer = 0.00000001f;
    return 0;
}

static U32 AttackFinished(xAnimTransition* tran, xAnimSingle* anim, void* param_3)
{
    return 0.0f == globals.player.HeadbuttTimer && HealthCheck(tran, anim, param_3);
}

static U32 AttackFinishedRun(xAnimTransition* tran, xAnimSingle* anim, void* param_3)
{
    return 0.0f == globals.player.HeadbuttTimer && globals.player.Speed == 2 &&
           HealthCheck(tran, anim, param_3);
}

static U32 AttackFinishedHotsauce(xAnimTransition* tran, xAnimSingle* anim, void* param_3)
{
    return 0.0f == globals.player.HeadbuttTimer && globals.player.Speed == 3;
}

static U32 HeadHitCheck(xAnimTransition* tran, xAnimSingle* anim, void* param_3)
{
    return globals.player.HeadbuttHit;
}

static U32 HeadHitCB(xAnimTransition* tran, xAnimSingle* anim, void* param_3)
{
    globals.player.HeadbuttHit = 0;
    return 0;
}

static U32 ButtSmashCheck(xAnimTransition* tran, xAnimSingle* anim, void* param_3)
{
    return (globals.player.JumpState == 2 || globals.player.JumpState == 3) &&
           globals.player.Inv_Specials & 0x400 && globals.player.ControlOn &&
           globals.pad0->pressed & 0x80000;
}

static U32 ButtSmashCB(xAnimTransition* tran, xAnimSingle* anim, void* param_3)
{
    globals.player.JumpState = 4;
    globals.player.Jump_CurrGravity = globals.player.s.Gravity;
    globals.player.Jump_HoldTimer = 0.0f;
    globals.player.Jump_ChangeTimer = 0.0f;
    globals.player.ent.frame->vel.y = 0.0f;
    globals.player.ent.frame->dpos.y = 0.0f;
    globals.player.Bounced = 0;
    return 0;
}

static U32 ButtSmashStateCB(xAnimTransition* tran, xAnimSingle* anim, void* param_3)
{
    globals.player.JumpState = 5;
    return 0;
}

static U32 SlideCheck(xAnimTransition* tran, xAnimSingle* anim, void* param_3)
{
    return globals.player.Slide == 1 && globals.player.SlideTimer > 0.3f &&
           (globals.player.JumpState == 0 || globals.player.JumpState == 1);
}

static U32 FloatCheck(xAnimTransition* tran, xAnimSingle* anim, void* param_3)
{
    return globals.player.JumpState == 3 &&
           (tran->UserFlags & 0x4 ||
            (tran->UserFlags & 0x1 &&
             globals.player.ent.frame->mat.pos.y - globals.player.ent.frame->oldpos.y <= 0.0f) ||
            (tran->UserFlags & 0x2 &&
             globals.player.ent.frame->mat.pos.y - globals.player.ent.frame->oldpos.y > 0.0f));
}

static U32 NotFloatCheck(xAnimTransition* tran, xAnimSingle* anim, void* param_3)
{
    return globals.player.JumpState != 3;
}

static U32 HangCheck(xAnimTransition* tran, xAnimSingle* anim, void* param_3)
{
    return globals.player.HangFound != NULL;
}

static U32 NoHangCheck(xAnimTransition* tran, xAnimSingle* anim, void* param_3)
{
    return globals.player.HangEnt == NULL;
}

static U32 PowerupCheck(xAnimTransition* tran, xAnimSingle* anim, void* param_3)
{
    return globals.player.SpecialReceived == tran->UserFlags;
}

static U32 PowerupCB(xAnimTransition* tran, xAnimSingle* anim, void* param_3)
{
    globals.player.SpecialReceived = 0;
    return 0;
}

static U32 HangCB(xAnimTransition* tran, xAnimSingle* anim, void* param_3)
{
    F32 spinAngle;
    F32 scoobAngle;
    F32 hangAngle;
    xVec3 targetPos;

    globals.player.HangEnt = globals.player.HangFound;
    globals.player.HangPivot = globals.player.HangFound->pivot;
    globals.player.HangVel.x = 0.0f;
    globals.player.HangVel.y = 0.0f;
    globals.player.HangVel.z = 0.0f;
    globals.player.HangLength = globals.player.HangFound->hangInfo->leverArm;
    globals.player.HangStartPos = ((xMat4x3*)globals.player.ent.model->Mat)->pos;
    globals.player.HangStartLerp = 1.0f;

    targetPos.x = globals.player.ent.bound.sph.center.x - globals.player.HangPivot.x;
    targetPos.y = globals.player.ent.bound.sph.center.y - globals.player.HangPivot.y;
    targetPos.z = globals.player.ent.bound.sph.center.z - globals.player.HangPivot.z;
    xVec3Normalize(&targetPos, &targetPos);
    targetPos.x *= globals.player.HangFound->hangInfo->leverArm;
    targetPos.y *= globals.player.HangFound->hangInfo->leverArm;
    targetPos.z *= globals.player.HangFound->hangInfo->leverArm;
    targetPos.x += globals.player.HangFound->pivot.x;
    targetPos.y += globals.player.HangFound->pivot.y;
    targetPos.z += globals.player.HangFound->pivot.z;

    vHangBefore = ((xMat4x3*)globals.player.ent.model->Mat)->pos;
    ((xMat4x3*)globals.player.ent.model->Mat)->pos.x = targetPos.x;
    ((xMat4x3*)globals.player.ent.model->Mat)->pos.y = targetPos.y - globals.player.ent.bound.sph.r;
    ((xMat4x3*)globals.player.ent.model->Mat)->pos.z = targetPos.z;
    vHangFirst = ((xMat4x3*)globals.player.ent.model->Mat)->pos;

    if (globals.player.HangEnt->shaggy)
    {
        spinAngle = xAngleClamp(atan2(((xMat4x3*)globals.player.HangEnt->shaggy->model->Mat)->at.x,
                                      ((xMat4x3*)globals.player.HangEnt->shaggy->model->Mat)->at.z));
        scoobAngle = xAngleClamp(atan2(((xMat4x3*)globals.player.ent.model->Mat)->at.x,
                                       ((xMat4x3*)globals.player.ent.model->Mat)->at.z));
        hangAngle = scoobAngle - spinAngle;
        if (hangAngle < 0.0f)
        {
            hangAngle += 2 * PI;
        }
        if (hangAngle > 2 * PI)
        {
            hangAngle -= 2 * PI;
        }
        zEntShaggy_SetSpin(globals.player.HangEnt->shaggy, hangAngle);
    }
    return 0;
}

static U32 HangSwingCheck(xAnimTransition* tran, xAnimSingle* anim, void* param_3)
{
    return abs(globals.pad0->analog1.x) > globals.player.s.AnalogMin ||
           abs(globals.pad0->analog1.y) > globals.player.s.AnalogMin;
}

static U32 HangNoSwingCheck(xAnimTransition* tran, xAnimSingle* anim, void* param_3)
{
    return abs(globals.pad0->analog1.x) <= globals.player.s.AnalogMin &&
           abs(globals.pad0->analog1.y) <= globals.player.s.AnalogMin;
}

static U32 DeathCB(xAnimTransition* tran, xAnimSingle* anim, void* param_3)
{
    U32 deathSnd;

    if ((irand() >> 16) & 1)
    {
        deathSnd = xStrHash("Scooby_Help");
    }
    else
    {
        deathSnd = xStrHash("Scooby_Oh_No");
    }
    xSndPlay(deathSnd, 0.5f, 0.5f, 0.0f, 0xFF, 0, 0);
    ScareCB(tran, anim, param_3);
    globals.player.DamageTimer = 3.0f;
    return 0;
}

static U32 FallToDeathStart(xAnimTransition* tran, xAnimSingle* anim, void* param_3)
{
    return 0.0f != globals.player.FallDeathTimer;
}

static U32 FallToDeathDrop(xAnimTransition* tran, xAnimSingle* anim, void* param_3)
{
    return !globals.player.FallDeathTimer;
}

static U32 FallToDeathDropCB(xAnimTransition* tran, xAnimSingle* anim, void* param_3)
{
    ScareCB(tran, anim, param_3);
    globals.player.DamageTimer = 1.0f;
    globals.player.ent.frame->vel.y = -4.0f;
    return 0;
}

static U32 ClearHelmetCB(xAnimTransition* tran, xAnimSingle* anim, void* param_3)
{
    globals.player.HelmetTimer = 0.0f;
    return 0;
}

static U32 SD0_TossCB(xAnimTransition* tran, xAnimSingle* anim, void* param_3)
{
    zEntPlayerJumpStart(&globals.player.ent, &globals.player.s.Toss);
    globals.player.ent.frame->vel.x += 0.3f * globals.player.ent.frame->mat.at.x;
    globals.player.ent.frame->vel.z += 0.3f * globals.player.ent.frame->mat.at.z;
    return 0;
}

static inline void AddGroundStatesTransition(xAnimTable* table, xAnimTransition* tran)
{
    xAnimTableAddTransition(table, tran, "Idle_normal Idle_scared Idle_brave Walk_normal Walk_scared Walk_brave");
    xAnimTableAddTransition(table, tran, "Walk_slippery Walk_plunger Run_normal Run_scared Run_brave Run_slippery");
    xAnimTableAddTransition(table, tran, "RunToIdle_normal RunToIdle_scared RunToIdle_brave RunToSkid");
    xAnimTableAddTransition(table, tran, "Idle_sneak Idle_lampshade Idle_blackknight Idle_teeter Idle_slippery Idle_plunger Slide");
    xAnimTableAddTransition(table, tran, "Skid SkidToIdle Walk_sneak Walk_blackknight Walk_sticky Walk_boots Run_boots Run_hotsauce");
    xAnimTableAddTransition(table, tran, "IdleToSneak IdleToLampshade IdleToBlackknight LampshadeToSneak SneakToLampshade");
    xAnimTableAddTransition(table, tran, "Land_normal Land_walkidle Land_running Land_walk Buttsmash_land Buttsmash_hit");
    xAnimTableAddTransition(table, tran, "Action_attack Action_attackrun Action_attack Action_attackrun Action_attackhit");
    xAnimTableAddTransition(table, tran, "Action_projectile Action_dig PowerUpReward Inactive_1 Inactive_2 Inactive_3 Inactive_4");
    xAnimTableAddTransition(table, tran, "Inactive_SitStart Inactive_SitLoop Inactive_SitEnd Inactive_SitEndSlow Idle_LookLeft Idle_LookRight");
    xAnimTableAddTransition(table, tran, "Inactive_SitFlea Inactive_SitSniff Inactive_SitHungry Inactive_SitLookLeft Inactive_SniffAir Inactive_SniffGround");
    xAnimTableAddTransition(table, tran, "Inactive_Magnify Inactive_Dizzy Inactive_BlowFeet Inactive_LookAround");
}

void ButtonStandOnCheck(_xEnt* ent)
{
    _xCollis* coll;
    _zEnt* bn;
    U32 response;
    U32 buttonState;

    coll = &ent->collis->colls[0];
    bn = (_zEnt*)coll->optr;
    if (bn && bn->baseType == 0x18 && coll->norm.x == 0.0f && coll->norm.z == 0.0f)
    {
        response = zEntButton_Stand((_zEntButton*)bn);
        buttonState = zEntButton_CheckState((_zEntButton*)coll->optr);
        if (response == 1 && !buttonState)
        {
            zEntEvent((xBase*)coll->optr, eEventPress);
        }
    }
}

static S32 ButtonHBCallback(_zEnt* ent, _xCollis* coll)
{
    U32 headResponse;
    U32 buttonState;

    if (((xBase*)coll->optr)->baseType != 0x18)
    {
        return 0;
    }
    headResponse = zEntButton_Headbutt((_zEntButton*)coll->optr);
    buttonState = zEntButton_CheckState((_zEntButton*)coll->optr);
    if (headResponse == 4 && !buttonState)
    {
        if (!(globals.player.Inv_Specials & 0x800))
        {
            ent->frame->mat.pos.x = ((xMat4x3*)ent->model->Mat)->pos.x;
            ent->frame->mat.pos.y = ((xMat4x3*)ent->model->Mat)->pos.y;
            ent->frame->mat.pos.z = ((xMat4x3*)ent->model->Mat)->pos.z;
            globals.player.HeadbuttHit = 1;
            globals.player.DecelRunSpeed = 0.25f * globals.player.DecelRunSpeed;
            return 0;
        }
        zEntEvent((xBase*)coll->optr, eEventPress);
        zEntPlayer_ButtonHBFX(ent, (_zEntButton*)coll->optr);
        ent->frame->mat.pos.x = ((xMat4x3*)ent->model->Mat)->pos.x;
        ent->frame->mat.pos.y = ((xMat4x3*)ent->model->Mat)->pos.y;
        ent->frame->mat.pos.z = ((xMat4x3*)ent->model->Mat)->pos.z;
        globals.player.HeadbuttHit = 1;
        globals.player.DecelRunSpeed = 0.25f * globals.player.DecelRunSpeed;
    }
    return 0;
}

void ButtonBBCheck(_xEnt* ent)
{
    _xCollis* coll;
    _xCollis* endcoll;
    U32 bounceResponse;
    U32 buttonState;

    coll = &ent->collis->colls[ent->collis->dyn_sidx];
    endcoll = &ent->collis->colls[ent->collis->dyn_eidx];
    if (((xBase*)coll->optr)->baseType == 0x18 && globals.player.Health &&
        (ent->model->Anim->Single->State->UserFlags & 0x1E) == 0xE)
    {
        for (; coll < endcoll; coll++)
        {
            if (coll->hdng.y < -0.866025f)
            {
                bounceResponse = zEntButton_ButtBounce((_zEntButton*)coll->optr);
                buttonState = zEntButton_CheckState((_zEntButton*)coll->optr);
                if ((bounceResponse == 2 || bounceResponse == 3) && !buttonState)
                {
                    zEntEvent((xBase*)coll->optr, eEventPress);
                    zEntPlayer_ButtonBBFX(ent, (_zEntButton*)coll->optr);
                    return;
                }
                if (coll->hdng.x == 0.0f && coll->hdng.z == 0.0f)
                {
                    ent->frame->vel.y = 0.0f;
                }
            }
        }
    }
}

static inline void ButtonHeadbuttCheck(_xEnt* ent)
{
    _xCollis* coll;
    _xCollis* endcoll;

    coll = &ent->collis->colls[ent->collis->dyn_sidx];
    endcoll = &ent->collis->colls[ent->collis->dyn_eidx];
    if (((xBase*)coll->optr)->baseType == 0x18)
    {
        HeadbuttTest(coll, endcoll, 0.1f, ButtonHBCallback);
    }
}

void ButtonCheck(_xEnt* ent)
{
    _xCollis* coll;

    if (ent->collis->dyn_eidx > ent->collis->dyn_sidx)
    {
        coll = &ent->collis->colls[ent->collis->dyn_sidx];
        if (coll->optr && ((xBase*)coll->optr)->baseType == 0x18)
        {
            if (globals.player.Inv_Specials & 0x800)
            {
                ButtonHeadbuttCheck(ent);
            }
            ButtonBBCheck(ent);
            ButtonStandOnCheck(ent);
        }
    }
}

void DestructObjBBCheck(_xEnt* ent)
{
    _xCollis* coll;
    _xCollis* endcoll;
    U32 bounceResponse;
    U32 buttonState;

    coll = &ent->collis->colls[ent->collis->dyn_sidx];
    endcoll = &ent->collis->colls[ent->collis->dyn_eidx];
    if (globals.player.Health && (ent->model->Anim->Single->State->UserFlags & 0x1E) == 0xE)
    {
        for (; coll < endcoll; coll++)
        {
            if (coll->hdng.y < -0.5f)
            {
                bounceResponse = zEntDestructObj_ButtBounce((_zEntDestructObj*)coll->optr);
                buttonState = zEntDestructObj_CheckState((_zEntDestructObj*)coll->optr);
                if (bounceResponse == 3 && buttonState != 2)
                {
                    zEntEvent((xBase*)coll->optr, eEventHit);
                    ent->frame->mat.pos.x = ((xMat4x3*)ent->model->Mat)->pos.x;
                    ent->frame->mat.pos.y = ((xMat4x3*)ent->model->Mat)->pos.y;
                    ent->frame->mat.pos.z = ((xMat4x3*)ent->model->Mat)->pos.z;
                    startBounce = ent->frame->mat.pos.y;
                    zEntPlayerJumpStart(ent, &globals.player.s.Bounce);
                    globals.player.Jump_CanDouble = 1;
                    globals.player.Jump_CanFloat = 1;
                    return;
                }
                if (coll->hdng.x == 0.0f && coll->hdng.z == 0.0f)
                {
                    ent->frame->vel.y = 0.0f;
                }
            }
        }
    }
}

static inline void DestructObjHeadbuttCheck(_xEnt* ent)
{
    _xCollis* coll;
    _xCollis* endcoll;

    coll = &ent->collis->colls[ent->collis->dyn_sidx];
    endcoll = &ent->collis->colls[ent->collis->dyn_eidx];
    HeadbuttTest(coll, endcoll, 0.1f, DestructObjHBCallback);
}

void DestructObjCheck(_xEnt* ent)
{
    _xCollis* coll;

    if (ent->collis->dyn_eidx > ent->collis->dyn_sidx)
    {
        coll = &ent->collis->colls[ent->collis->dyn_sidx];
        if (coll->optr && ((xBase*)coll->optr)->baseType == 0x1B)
        {
            DestructObjHeadbuttCheck(ent);
            DestructObjBBCheck(ent);
        }
    }
}

static S32 DestructObjHBCallback(_zEnt* ent, _xCollis* coll)
{
    U32 headResponse;
    U32 dobjState;

    if (((xBase*)coll->optr)->baseType != 0x1B)
    {
        return 0;
    }
    headResponse = zEntDestructObj_Headbutt((_zEntDestructObj*)coll->optr);
    dobjState = zEntDestructObj_CheckState((_zEntDestructObj*)coll->optr);
    if ((headResponse == 4 || headResponse == 5) && dobjState != 2)
    {
        if (!(globals.player.Inv_Specials & 0x800))
        {
            ent->frame->mat.pos.x = ((xMat4x3*)ent->model->Mat)->pos.x;
            ent->frame->mat.pos.y = ((xMat4x3*)ent->model->Mat)->pos.y;
            ent->frame->mat.pos.z = ((xMat4x3*)ent->model->Mat)->pos.z;
            globals.player.HeadbuttHit = 1;
            globals.player.DecelRunSpeed = 0.25f * globals.player.DecelRunSpeed;
            return 0;
        }
        zEntEvent((xBase*)coll->optr, eEventHit);
        ent->frame->mat.pos.x = ((xMat4x3*)ent->model->Mat)->pos.x;
        ent->frame->mat.pos.y = ((xMat4x3*)ent->model->Mat)->pos.y;
        ent->frame->mat.pos.z = ((xMat4x3*)ent->model->Mat)->pos.z;
        if (dobjState != 1 || headResponse != 5)
        {
            globals.player.HeadbuttHit = 1;
        }
        globals.player.DecelRunSpeed = 0.25f * globals.player.DecelRunSpeed;
    }
    return 0;
}

xAnimTable* zEntPlayer_AnimTable()
{
    xAnimTransition* tran;
    xAnimTable* table;
    xAnimState* currst;

    table = xAnimTableNew("Player", NULL);
    xAnimTableNewState(table, "Idle_normal", 0x10, 1, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "Idle_scared", 0x10, 1, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "Idle_brave", 0x10, 1, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "Walk_normal", 0x10, 0x444, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "Walk_scared", 0x10, 0x444, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "Walk_brave", 0x10, 0x444, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "Walk_slippery", 0x10, 0x44, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "Walk_plunger", 0x10, 0x44, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "Walk_sticky", 0x10, 4, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "Walk_boots", 0x10, 4, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "Run_normal", 0x10, 0x46, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "Run_scared", 0x10, 0x46, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "Run_brave", 0x10, 0x46, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "Run_slippery", 0x10, 0x46, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "Run_boots", 0x10, 0x46, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "RunToIdle_normal", 0x20, 0x26, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "RunToIdle_scared", 0x20, 0x26, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "RunToIdle_brave", 0x20, 0x26, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "RunToSkid", 0x20, 0x26, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "Idle_sneak", 0x10, 3, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "Idle_lampshade", 0x10, 3, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "Idle_blackknight", 0x10, 3, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "Idle_teeter", 0x10, 0x40000, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "Idle_slippery", 0x10, 0x40000, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "Idle_plunger", 0x10, 0, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "Skid", 0x10, 0, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "SkidToIdle", 0x20, 0, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "Walk_sneak", 0x10, 2, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "Walk_blackknight", 0x10, 2, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "Run_hotsauce", 0x10, 8, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "IdleToSneak", 0x20, 0, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "IdleToLampshade", 0x20, 0, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "IdleToBlackknight", 0x20, 0, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "LampshadeToSneak", 0x20, 2, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "SneakToLampshade", 0x20, 2, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "Scare_normal", 0x20, 0, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "Scare_losechance", 0x20, 0x80, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "Scare_loseloop", 0x10, 0x80, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "Scare_brave", 0x20, 0, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "Scare_airhit", 0x20, 0x100, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "Scare_airfall", 0x10, 0x80A, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "Scare_airland", 0x20, 0, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "Scare_airlose", 0x20, 0x180, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "Scare_losefall", 0x10, 0x180, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "Scare_loseland", 0, 0, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "Scare_norm_cower", 0x10, 0x80, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "Scare_norm_hit", 0x20, 0x80, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "Scare_critter", 0x20, 0, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "Scare_thorns", 0x20, 0x80A, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "Jump_normal", 0x20, 0xA, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "Jump_run", 0x20, 0xA, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "Jump_walk", 0x20, 0xA, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "Jump_double", 0x20, 0xA, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "Jump_buttbounce", 0x20, 0xA, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "Jump_hang", 0x20, 0xA, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "Jump_springboard", 0x20, 0xA, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "Fall_normal", 0, 0xA, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "Fall_run", 0, 0xA, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "Fall_walk", 0, 0xA, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "Fall_floatup", 0x10, 0xA, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "Fall_floatdown", 0x10, 0xA, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "Fall_floatopen", 0x20, 0xA, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "Fall_spring", 0x10, 0xA, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "Fall_fromspring", 0x20, 0xA, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "Fall_losechance", 0x10, 0, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "Fall_losedrop", 0x10, 0, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "Land_normal", 0x20, 0, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "Land_running", 0x20, 6, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "Land_walk", 0x20, 4, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "Land_walkidle", 0x20, 4, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "HangGrab", 0x20, 0x20000, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "Hang", 0x10, 0x20000, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "HangToSwing", 0, 0x20000, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "SwingToHang", 0x20, 0x20000, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "Buttsmash_start", 0x20, 0x1080E, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "Buttsmash_fall", 0x10, 0x1080E, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "Buttsmash_land", 0x20, 0x10080, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "Buttsmash_hit", 0x20, 0x1080A, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "Slide", 0x10, 0, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "Action_attack", 0x10, 0x1000C, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "Action_attackrun", 0x10, 0x1000C, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "Action_attackhit", 0x20, 0x100A6, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "Action_projectile", 0x20, 0x80000, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "Action_dig", 0x10, 0x12000, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "PowerUpReward", 0x20, 0, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "Inactive_1", 0x20, 0, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "Inactive_2", 0x20, 0, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "Inactive_3", 0x20, 0, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "Inactive_4", 0x20, 0, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "Inactive_SitStart", 0x20, 0x80, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "Inactive_SitLoop", 0x10, 0x81, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "Inactive_SitEnd", 0x20, 0x80, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "Inactive_SitEndSlow", 0x20, 0x80, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "Idle_LookLeft", 0x20, 1, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "Idle_LookRight", 0x20, 1, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "Inactive_LookAround", 0x20, 0x80, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "Inactive_SitFlea", 0x20, 0x80, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "Inactive_SitSniff", 0x20, 0x80, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "Inactive_SitHungry", 0x20, 0x80, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "Inactive_SitLookLeft", 0x20, 0x81, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "Inactive_SniffAir", 0x20, 0x80, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "Inactive_SniffGround", 0x20, 0x80, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "Inactive_Magnify", 0x20, 0x80, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "Inactive_Dizzy", 0x20, 0x80, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "Inactive_BlowFeet", 0x20, 0x80, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "Powerup_helmet", 0x20, 0x80, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "Powerup_plunger", 0x20, 0x80, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "Powerup_soap", 0x20, 0x80, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "Powerup_lamp", 0x20, 0x80, 1.0f, NULL, NULL, 0.0f, NULL);
    Shaggy1_PlayerStates(table);
    Shaggy1_PlayerTransitions(table);
    Shaggy0_PlayerStates(table);
    Shaggy0_PlayerTransitions(table);
    tran = xAnimTableNewTransition(table, "SD0_Throw", "Fall_normal", NULL, SD0_TossCB, 0x10, 0, 0.0f, 0.0f, 0, 0, 0.0f, NULL);
    tran = xAnimTableNewTransition(table, "SD1_HangReact SD1_LandReact", "Idle_scared", HealthCheck, NULL, 0x10, 2, 0.0f, 0.0f, 0, 0, 0.15f, NULL);
    tran = xAnimTableNewTransition(table, "SD1_HangReact SD1_LandReact", "Idle_normal", HealthCheck, NULL, 0x10, 4, 0.0f, 0.0f, 0, 0, 0.15f, NULL);
    tran = xAnimTableNewTransition(table, "SD1_HangReact SD1_LandReact", "Idle_brave", HealthCheck, NULL, 0x10, 8, 0.0f, 0.0f, 0, 0, 0.15f, NULL);
    tran = xAnimTableNewTransition(table, "SD0_SetDown", "Idle_scared", HealthCheck, (xAnimTransitionCallback)Shaggy0_FlipCB, 0x10, 2, 0.0f, 0.0f, 0, 0, 0.15f, NULL);
    tran = xAnimTableNewTransition(table, "SD0_SetDown", "Idle_normal", HealthCheck, (xAnimTransitionCallback)Shaggy0_FlipCB, 0x10, 4, 0.0f, 0.0f, 0, 0, 0.15f, NULL);
    tran = xAnimTableNewTransition(table, "SD0_SetDown", "Idle_brave", HealthCheck, (xAnimTransitionCallback)Shaggy0_FlipCB, 0x10, 8, 0.0f, 0.0f, 0, 0, 0.15f, NULL);
    tran = xAnimTableNewTransition(table, "SD1_SetDown", "Idle_scared", HealthCheck, NULL, 0x10, 2, 0.0f, 0.0f, 0, 0, 0.15f, NULL);
    tran = xAnimTableNewTransition(table, "SD1_SetDown", "Idle_normal", HealthCheck, NULL, 0x10, 4, 0.0f, 0.0f, 0, 0, 0.15f, NULL);
    tran = xAnimTableNewTransition(table, "SD1_SetDown", "Idle_brave", HealthCheck, NULL, 0x10, 8, 0.0f, 0.0f, 0, 0, 0.15f, NULL);
    tran = xAnimTableNewTransition(table, "SD1_Hold SD1_HoldWalk SD1_CatchJump SD1_CatchThrow SD1_Throw SD1_ThrowIdle SD1_ThrowWalk SD1_HangReact SD1_LandReact", "Scare_norm_hit", ScareCheckNoHealth, ScareCB, 0, 0, 0.0f, 0.0f, 5, 0, 0.15f, NULL);
    tran = xAnimTableNewTransition(table, "SD1_Hold SD1_HoldWalk SD1_CatchJump SD1_CatchThrow SD1_Throw SD1_ThrowIdle SD1_ThrowWalk SD1_HangReact SD1_LandReact", "Scare_critter", ScareCheckCritter, ScareCB, 0, 0, 0.0f, 0.0f, 6, 0, 0.15f, NULL);
    tran = xAnimTableNewTransition(table, "SD1_Hold SD1_HoldWalk SD1_CatchJump SD1_CatchThrow SD1_Throw SD1_ThrowIdle SD1_ThrowWalk SD1_HangReact SD1_LandReact", "Scare_loseloop", ScareCheck, DeathCB, 0, 1, 0.0f, 0.0f, 0xA, 0, 0.15f, NULL);
    tran = xAnimTableNewTransition(table, "SD0_Hold SD0_HoldWalk SD0_CatchJump SD0_CatchThrow SD0_Throw", "Scare_airhit", ScareCheckNoHealth, ScareKnockbackCB, 0, 0, 0.0f, 0.0f, 5, 0, 0.15f, NULL);
    tran = xAnimTableNewTransition(table, "Action_attack Action_projectile Action_dig PowerUpReward", "Idle_scared", HealthCheck, NULL, 0x10, 2, 0.0f, 0.0f, 0, 0, 0.15f, NULL);
    tran = xAnimTableNewTransition(table, "Action_attack Action_projectile Action_dig PowerUpReward", "Idle_normal", HealthCheck, NULL, 0x10, 4, 0.0f, 0.0f, 0, 0, 0.15f, NULL);
    tran = xAnimTableNewTransition(table, "Action_attack Action_projectile Action_dig PowerUpReward", "Idle_brave", HealthCheck, NULL, 0x10, 8, 0.0f, 0.0f, 0, 0, 0.15f, NULL);
    tran = xAnimTableNewTransition(table, "Action_attack Action_projectile Action_dig PowerUpReward", "Idle_teeter", TeeterCheck, NULL, 0x10, 0, 0.0f, 0.0f, 0, 0, 0.15f, NULL);
    tran = xAnimTableNewTransition(table, "Action_attack Action_projectile Action_dig PowerUpReward", "Idle_slippery", SlipperyCheck, NULL, 0x10, 0, 0.0f, 0.0f, 0, 0, 0.15f, NULL);
    tran = xAnimTableNewTransition(table, "Action_attack Action_projectile Action_dig PowerUpReward", "Idle_plunger", PlungerCheck, NULL, 0x10, 0, 0.0f, 0.0f, 0, 0, 0.15f, NULL);
    tran = xAnimTableNewTransition(table, "Action_attackrun", "Run_scared", HealthCheck, NULL, 0x10, 2, 0.0f, 0.0f, 0, 0, 0.15f, NULL);
    tran = xAnimTableNewTransition(table, "Action_attackrun", "Run_normal", HealthCheck, NULL, 0x10, 4, 0.0f, 0.0f, 0, 0, 0.15f, NULL);
    tran = xAnimTableNewTransition(table, "Action_attackrun", "Run_brave", HealthCheck, NULL, 0x10, 8, 0.0f, 0.0f, 0, 0, 0.15f, NULL);
    tran = xAnimTableNewTransition(table, "Action_attackrun", "Run_hotsauce", HotsauceCheck, NULL, 0x10, 0, 0.0f, 0.0f, 0, 0, 0.15f, NULL);
    tran = xAnimTableNewTransition(table, "Scare_airhit", "Scare_airfall", NULL, NULL, 0x10, 0, 0.0f, 0.0f, 0, 0, 0.15f, NULL);
    tran = xAnimTableNewTransition(table, "Scare_airhit", "Idle_scared", LandHealthCheck, NULL, 0x10, 2, 0.0f, 0.0f, 1, 0, 0.1f, NULL);
    tran = xAnimTableNewTransition(table, "Scare_airhit", "Idle_normal", LandHealthCheck, NULL, 0x10, 4, 0.0f, 0.0f, 1, 0, 0.1f, NULL);
    tran = xAnimTableNewTransition(table, "Scare_airhit", "Idle_brave", LandHealthCheck, NULL, 0x10, 8, 0.0f, 0.0f, 1, 0, 0.1f, NULL);
    tran = xAnimTableNewTransition(table, "Scare_brave", "Idle_normal", NULL, NULL, 0x10, 0, 0.0f, 0.0f, 0, 0, 0.0f, NULL);
    tran = xAnimTableNewTransition(table, "Scare_normal", "Idle_scared", NULL, NULL, 0x10, 0, 0.0f, 0.0f, 0, 0, 0.0f, NULL);
    tran = xAnimTableNewTransition(table, "Scare_norm_hit Scare_critter", "Idle_scared", HealthCheck, NULL, 0x10, 2, 0.0f, 0.0f, 0, 0, 0.15f, NULL);
    tran = xAnimTableNewTransition(table, "Scare_norm_hit Scare_critter", "Idle_normal", HealthCheck, NULL, 0x10, 4, 0.0f, 0.0f, 0, 0, 0.15f, NULL);
    tran = xAnimTableNewTransition(table, "Scare_norm_hit Scare_critter", "Idle_brave", HealthCheck, NULL, 0x10, 8, 0.0f, 0.0f, 0, 0, 0.15f, NULL);
    tran = xAnimTableNewTransition(table, "Inactive_1 Inactive_2 Inactive_3 Inactive_4 Inactive_SitEnd Inactive_SitEndSlow", "Idle_scared", HealthCheck, NULL, 0x10, 2, 0.0f, 0.0f, 0, 0, 0.15f, NULL);
    tran = xAnimTableNewTransition(table, "Inactive_1 Inactive_2 Inactive_3 Inactive_4 Inactive_SitEnd Inactive_SitEndSlow", "Idle_normal", HealthCheck, NULL, 0x10, 4, 0.0f, 0.0f, 0, 0, 0.15f, NULL);
    tran = xAnimTableNewTransition(table, "Inactive_1 Inactive_2 Inactive_3 Inactive_4 Inactive_SitEnd Inactive_SitEndSlow", "Idle_brave", HealthCheck, NULL, 0x10, 8, 0.0f, 0.0f, 0, 0, 0.15f, NULL);
    tran = xAnimTableNewTransition(table, "Jump_normal Jump_run Jump_double Jump_buttbounce Jump_hang", "Fall_normal", NULL, NULL, 0x10, 0, 0.0f, 0.0f, 0, 0, 0.0f, NULL);
    tran = xAnimTableNewTransition(table, "Jump_walk", "Fall_walk", NULL, NULL, 0x10, 0, 0.0f, 0.0f, 0, 0, 0.0f, NULL);
    tran = xAnimTableNewTransition(table, "Jump_run", "Fall_run", NULL, NULL, 0x10, 0, 0.0f, 0.0f, 0, 0, 0.0f, NULL);
    tran = xAnimTableNewTransition(table, "Jump_springboard", "Fall_spring", NULL, NULL, 0x10, 0, 0.0f, 0.0f, 0, 0, 0.0f, NULL);
    tran = xAnimTableNewTransition(table, "Fall_fromspring", "Fall_normal", NULL, NULL, 0x10, 0, 0.0f, 0.0f, 0, 0, 0.0f, NULL);
    tran = xAnimTableNewTransition(table, "Land_normal Land_walk Land_walkidle Land_running", "Idle_scared", HealthCheck, NULL, 0x10, 2, 0.0f, 0.0f, 0, 0, 0.15f, NULL);
    tran = xAnimTableNewTransition(table, "Land_normal Land_walk Land_walkidle Land_running", "Idle_normal", HealthCheck, NULL, 0x10, 4, 0.0f, 0.0f, 0, 0, 0.15f, NULL);
    tran = xAnimTableNewTransition(table, "Land_normal Land_walk Land_walkidle Land_running", "Idle_brave", HealthCheck, NULL, 0x10, 8, 0.0f, 0.0f, 0, 0, 0.15f, NULL);
    tran = xAnimTableNewTransition(table, "Land_normal Land_walk Land_walkidle Land_running", "Walk_scared", WalkCheck, NULL, 0x10, 2, 0.0f, 0.0f, 0, 0, 0.15f, NULL);
    tran = xAnimTableNewTransition(table, "Land_normal Land_walk Land_walkidle Land_running", "Walk_normal", WalkCheck, NULL, 0x10, 4, 0.0f, 0.0f, 0, 0, 0.15f, NULL);
    tran = xAnimTableNewTransition(table, "Land_normal Land_walk Land_walkidle Land_running", "Walk_brave", WalkCheck, NULL, 0x10, 8, 0.0f, 0.0f, 0, 0, 0.15f, NULL);
    tran = xAnimTableNewTransition(table, "Land_running", "Run_scared", RunCheck, NULL, 0x10, 2, 0.0f, 0.0f, 0, 0, 0.15f, NULL);
    tran = xAnimTableNewTransition(table, "Land_running", "Run_normal", RunCheck, NULL, 0x10, 4, 0.0f, 0.0f, 0, 0, 0.15f, NULL);
    tran = xAnimTableNewTransition(table, "Land_running", "Run_brave", RunCheck, NULL, 0x10, 8, 0.0f, 0.0f, 0, 0, 0.15f, NULL);
    tran = xAnimTableNewTransition(table, "Land_running", "Run_slippery", RunCheck, NULL, 0x10, 0x10, 0.0f, 0.0f, 1, 0, 0.15f, NULL);
    tran = xAnimTableNewTransition(table, "Land_normal Land_walk Land_walkidle Land_running", "Walk_slippery", WalkCheck, NULL, 0x10, 0x10, 0.0f, 0.0f, 0, 0, 0.15f, NULL);
    tran = xAnimTableNewTransition(table, "Land_normal Land_walk Land_walkidle Land_running", "Walk_sticky", WalkCheck, NULL, 0x10, 0x40, 0.0f, 0.0f, 1, 0, 0.15f, NULL);
    tran = xAnimTableNewTransition(table, "Land_normal Land_walk Land_walkidle Land_running", "Walk_boots", WalkCheck, NULL, 0x10, 0x80, 0.0f, 0.0f, 1, 0, 0.15f, NULL);
    tran = xAnimTableNewTransition(table, "Land_normal Land_walk Land_walkidle Land_running", "Walk_plunger", WalkPlungeCheck, NULL, 0x10, 0, 0.0f, 0.0f, 1, 0, 0.15f, NULL);
    tran = xAnimTableNewTransition(table, "Land_normal Land_walk Land_walkidle Land_running", "Idle_plunger", PlungerCheck, NULL, 0x10, 0, 0.0f, 0.0f, 1, 0, 0.15f, NULL);
    tran = xAnimTableNewTransition(table, "Land_normal Land_walk Land_walkidle Land_running", "Slide", SlideCheck, NULL, 0x10, 0, 0.0f, 0.0f, 4, 0, 0.15f, NULL);
    tran = xAnimTableNewTransition(table, "RunToSkid", "Skid", NULL, NULL, 0x10, 0, 0.0f, 0.0f, 0, 0, 0.0f, NULL);
    tran = xAnimTableNewTransition(table, "SkidToIdle", "Idle_scared", HealthCheck, NULL, 0x10, 2, 0.0f, 0.0f, 1, 0, 0.15f, NULL);
    tran = xAnimTableNewTransition(table, "SkidToIdle", "Idle_normal", HealthCheck, NULL, 0x10, 4, 0.0f, 0.0f, 1, 0, 0.15f, NULL);
    tran = xAnimTableNewTransition(table, "SkidToIdle", "Idle_brave", HealthCheck, NULL, 0x10, 8, 0.0f, 0.0f, 1, 0, 0.15f, NULL);
    tran = xAnimTableNewTransition(table, "SkidToIdle", "Idle_teeter", TeeterCheck, NULL, 0x10, 0, 0.0f, 0.0f, 2, 0, 0.15f, NULL);
    tran = xAnimTableNewTransition(table, "SkidToIdle", "Idle_slippery", SlipperyCheck, NULL, 0x10, 0, 0.0f, 0.0f, 2, 0, 0.15f, NULL);
    tran = xAnimTableNewTransition(table, "SkidToIdle", "Idle_plunger", PlungerCheck, NULL, 0x10, 0, 0.0f, 0.0f, 3, 0, 0.15f, NULL);
    tran = xAnimTableNewTransition(table, "SkidToIdle", "Slide", SlideCheck, NULL, 0x10, 0, 0.0f, 0.0f, 4, 0, 0.15f, NULL);
    tran = xAnimTableNewTransition(table, "HangGrab", "Hang", NULL, NULL, 0x10, 0, 0.0f, 0.0f, 0, 0, 0.0f, NULL);
    tran = xAnimTableNewTransition(table, "SwingToHang", "Hang", NULL, NULL, 0x10, 0, 0.0f, 0.0f, 0, 0, 0.15f, NULL);
    tran = xAnimTableNewTransition(table, "IdleToSneak", "Walk_sneak", NULL, NULL, 0x10, 0, 0.0f, 0.0f, 0, 0, 0.0f, NULL);
    tran = xAnimTableNewTransition(table, "IdleToLampshade", "Idle_lampshade", NULL, NULL, 0x10, 0, 0.0f, 0.0f, 0, 0, 0.0f, NULL);
    tran = xAnimTableNewTransition(table, "IdleToBlackknight", "Idle_blackknight", NULL, NULL, 0x10, 0, 0.0f, 0.0f, 0, 0, 0.0f, NULL);
    tran = xAnimTableNewTransition(table, "LampshadeToSneak", "Walk_sneak", NULL, NULL, 0x10, 0, 0.0f, 0.0f, 0, 0, 0.0f, NULL);
    tran = xAnimTableNewTransition(table, "SneakToLampshade", "Idle_lampshade", NULL, NULL, 0x10, 0, 0.0f, 0.0f, 0, 0, 0.0f, NULL);
    tran = xAnimTableNewTransition(table, "Idle_normal Idle_brave  Idle_slippery Idle_plunger Run_boots Slide Walk_normal Walk_scared Walk_brave Walk_slippery Walk_plunger Walk_sticky Walk_boots", "Idle_scared", IdleCheck, NULL, 0, 2, 0.0f, 0.0f, 1, 0, 0.15f, NULL);
    tran = xAnimTableNewTransition(table, "Idle_scared Idle_brave  Idle_slippery Idle_plunger Run_boots Slide Walk_normal Walk_scared Walk_brave Walk_slippery Walk_plunger Walk_sticky Walk_boots", "Idle_normal", IdleCheck, NULL, 0, 4, 0.0f, 0.0f, 1, 0, 0.15f, NULL);
    tran = xAnimTableNewTransition(table, "Idle_scared Idle_normal Idle_slippery Idle_plunger Run_boots Slide Walk_normal Walk_scared Walk_brave Walk_slippery Walk_plunger Walk_sticky Walk_boots", "Idle_brave", IdleCheck, NULL, 0, 8, 0.0f, 0.0f, 1, 0, 0.15f, NULL);
    tran = xAnimTableNewTransition(table, "Idle_scared Idle_normal Idle_brave Idle_LookLeft Idle_LookRight Inactive_LookAround Idle_plunger Idle_slippery Run_boots Slide Inactive_SniffAir Inactive_SniffGround Inactive_Magnify Walk_normal Walk_scared Walk_brave Walk_slippery Walk_plunger Walk_sticky Walk_boots", "Idle_teeter", TeeterCheck, NULL, 0, 0, 0.0f, 0.0f, 2, 0, 0.15f, NULL);
    tran = xAnimTableNewTransition(table, "Idle_scared Idle_normal Idle_brave Idle_LookLeft Idle_LookRight Inactive_LookAround Idle_plunger Idle_teeter Run_boots Slide Inactive_SniffAir Inactive_SniffGround Inactive_Magnify Walk_normal Walk_scared Walk_brave Walk_slippery Walk_plunger Walk_sticky Walk_boots", "Idle_slippery", SlipperyCheck, NULL, 0, 0, 0.0f, 0.0f, 2, 0, 0.15f, NULL);
    tran = xAnimTableNewTransition(table, "Idle_scared Idle_normal Idle_brave Idle_LookLeft Idle_LookRight Inactive_LookAround Idle_teeter Idle_slippery Run_boots Slide Inactive_SniffAir Inactive_SniffGround Inactive_Magnify Walk_normal Walk_scared Walk_brave Walk_slippery Walk_plunger Walk_sticky Walk_boots", "Idle_plunger", PlungerCheck, NULL, 0, 0, 0.0f, 0.0f, 3, 0, 0.15f, NULL);
    tran = xAnimTableNewTransition(table, "Idle_scared Idle_normal Idle_brave Idle_LookLeft Idle_LookRight Inactive_LookAround Idle_plunger Idle_teeter Idle_slippery Inactive_SniffAir Inactive_SniffGround Inactive_Magnify Walk_normal Walk_scared Walk_brave Walk_slippery Walk_plunger Walk_sticky Walk_boots", "Slide", SlideCheck, NULL, 0, 0, 0.0f, 0.0f, 4, 0, 0.15f, NULL);
    tran = xAnimTableNewTransition(table, "Skid", "SkidToIdle", SkidStopCheck, NULL, 0, 0, 0.0f, 0.0f, 1, 0, 0.1f, NULL);
    tran = xAnimTableNewTransition(table, "Slide Walk_normal Walk_brave Run_boots Walk_slippery Walk_plunger Walk_sticky Walk_boots Idle_scared Idle_normal Idle_brave Idle_teeter Idle_slippery Idle_plunger Idle_LookLeft Idle_LookRight Inactive_LookAround Inactive_SniffAir Inactive_SniffGround Inactive_Magnify", "Walk_scared", WalkCheck, NULL, 0, 2, 0.0f, 0.0f, 1, 0, 0.15f, NULL);
    tran = xAnimTableNewTransition(table, "Slide Walk_scared Walk_brave Run_boots Walk_slippery Walk_plunger Walk_sticky Walk_boots Idle_scared Idle_normal Idle_brave Idle_teeter Idle_slippery Idle_plunger Idle_LookLeft Idle_LookRight Inactive_LookAround Inactive_SniffAir Inactive_SniffGround Inactive_Magnify", "Walk_normal", WalkCheck, NULL, 0, 4, 0.0f, 0.0f, 1, 0, 0.15f, NULL);
    tran = xAnimTableNewTransition(table, "Slide Walk_scared Walk_normal Run_boots Walk_slippery Walk_plunger Walk_sticky Walk_boots Idle_scared Idle_normal Idle_brave Idle_teeter Idle_slippery Idle_plunger Idle_LookLeft Idle_LookRight Inactive_LookAround Inactive_SniffAir Inactive_SniffGround Inactive_Magnify", "Walk_brave", WalkCheck, NULL, 0, 8, 0.0f, 0.0f, 1, 0, 0.15f, NULL);
    tran = xAnimTableNewTransition(table, "Slide Walk_plunger  Walk_sticky  Walk_boots Run_boots Walk_scared Walk_normal Walk_brave Idle_scared Idle_normal Idle_brave Idle_teeter Idle_slippery Idle_plunger Idle_LookLeft Idle_LookRight Inactive_LookAround Inactive_SniffAir Inactive_SniffGround Inactive_Magnify", "Walk_slippery", WalkCheck, NULL, 0, 0x10, 0.0f, 0.0f, 1, 0, 0.15f, NULL);
    tran = xAnimTableNewTransition(table, "Slide Walk_slippery Walk_sticky  Walk_boots Run_boots Walk_scared Walk_normal Walk_brave Idle_scared Idle_normal Idle_brave Idle_teeter Idle_slippery Idle_plunger Idle_LookLeft Idle_LookRight Inactive_LookAround Inactive_SniffAir Inactive_SniffGround Inactive_Magnify", "Walk_plunger", WalkCheck, NULL, 0, 0x20, 0.0f, 0.0f, 1, 0, 0.15f, NULL);
    tran = xAnimTableNewTransition(table, "Slide Walk_slippery Walk_plunger Walk_boots Run_boots Walk_scared Walk_normal Walk_brave Idle_scared Idle_normal Idle_brave Idle_teeter Idle_slippery Idle_plunger Idle_LookLeft Idle_LookRight Inactive_LookAround Inactive_SniffAir Inactive_SniffGround Inactive_Magnify", "Walk_sticky", WalkCheck, NULL, 0, 0x40, 0.0f, 0.0f, 1, 0, 0.15f, NULL);
    tran = xAnimTableNewTransition(table, "Slide Walk_slippery Walk_plunger Walk_sticky Run_boots Walk_scared Walk_normal Walk_brave Idle_scared Idle_normal Idle_brave Idle_teeter Idle_slippery Idle_plunger Idle_LookLeft Idle_LookRight Inactive_LookAround Inactive_SniffAir Inactive_SniffGround Inactive_Magnify", "Walk_boots", WalkCheck, NULL, 0, 0x80, 0.0f, 0.0f, 1, 0, 0.15f, NULL);
    tran = xAnimTableNewTransition(table, "Run_normal Run_scared Run_brave Run_slippery Run_boots", "Walk_plunger", WalkPlungeCheck, NULL, 0, 0, 0.0f, 0.0f, 5, 0, 0.15f, NULL);
    xAnimTableAddTransition(table, tran, "Walk_normal Walk_scared Walk_brave Walk_slippery Walk_sticky Walk_boots");
    xAnimTableAddTransition(table, tran, "RunToIdle_normal RunToIdle_scared RunToIdle_brave RunToSkid");
    xAnimTableAddTransition(table, tran, "Idle_sneak Idle_lampshade Idle_blackknight Idle_teeter Idle_slippery Idle_plunger");
    xAnimTableAddTransition(table, tran, "Skid SkidToIdle Walk_sneak Walk_blackknight");
    xAnimTableAddTransition(table, tran, "IdleToSneak IdleToLampshade IdleToBlackknight LampshadeToSneak SneakToLampshade");
    xAnimTableAddTransition(table, tran, "Inactive_1 Inactive_2 Inactive_3 Inactive_4 Inactive_SitFlea");
    xAnimTableAddTransition(table, tran, "Inactive_SitStart Inactive_SitLoop Inactive_SitEnd Inactive_SitEndSlow Idle_LookLeft Idle_LookRight");
    xAnimTableAddTransition(table, tran, "Inactive_SitSniff Inactive_SitHungry Inactive_SitLookLeft Inactive_SniffAir Inactive_SniffGround");
    xAnimTableAddTransition(table, tran, "Inactive_Magnify Inactive_Dizzy Inactive_BlowFeet Inactive_LookAround");
    tran = xAnimTableNewTransition(table, "Run_normal Run_scared Run_brave Run_slippery", "Slide", SlideCheck, NULL, 0, 0, 0.0f, 0.0f, 4, 0, 0.15f, NULL);
    xAnimTableAddTransition(table, tran, "Walk_normal Walk_scared Walk_brave Walk_slippery Walk_sticky Walk_boots");
    xAnimTableAddTransition(table, tran, "RunToIdle_normal RunToIdle_scared RunToIdle_brave RunToSkid");
    xAnimTableAddTransition(table, tran, "Idle_sneak Idle_lampshade Idle_blackknight Idle_teeter Idle_slippery Idle_plunger");
    xAnimTableAddTransition(table, tran, "Skid SkidToIdle Walk_sneak Walk_blackknight Run_slippery");
    xAnimTableAddTransition(table, tran, "IdleToSneak IdleToLampshade IdleToBlackknight LampshadeToSneak SneakToLampshade");
    xAnimTableAddTransition(table, tran, "Inactive_1 Inactive_2 Inactive_3 Inactive_4 Inactive_SitFlea");
    xAnimTableAddTransition(table, tran, "Inactive_SitStart Inactive_SitLoop Inactive_SitEnd Inactive_SitEndSlow Idle_LookLeft Idle_LookRight");
    xAnimTableAddTransition(table, tran, "Inactive_SitSniff Inactive_SitHungry Inactive_SitLookLeft Inactive_SniffAir Inactive_SniffGround");
    xAnimTableAddTransition(table, tran, "Inactive_Magnify Inactive_Dizzy Inactive_BlowFeet Inactive_LookAround");
    tran = xAnimTableNewTransition(table, "Slide Run_normal Run_brave  Run_hotsauce Run_slippery Run_boots Idle_scared Idle_normal Idle_brave Idle_teeter Idle_slippery Idle_plunger Idle_LookLeft Idle_LookRight Inactive_LookAround Inactive_SniffAir Inactive_SniffGround Inactive_Magnify Walk_normal Walk_scared Walk_brave Walk_slippery Walk_plunger Walk_sticky Walk_boots", "Run_scared", RunCheck, NULL, 0, 2, 0.0f, 0.0f, 1, 0, 0.15f, NULL);
    tran = xAnimTableNewTransition(table, "Slide Run_scared Run_brave  Run_hotsauce Run_slippery Run_boots Idle_scared Idle_normal Idle_brave Idle_teeter Idle_slippery Idle_plunger Idle_LookLeft Idle_LookRight Inactive_LookAround Inactive_SniffAir Inactive_SniffGround Inactive_Magnify Walk_normal Walk_scared Walk_brave Walk_slippery Walk_plunger Walk_sticky Walk_boots", "Run_normal", RunCheck, NULL, 0, 4, 0.0f, 0.0f, 1, 0, 0.15f, NULL);
    tran = xAnimTableNewTransition(table, "Slide Run_scared Run_normal Run_hotsauce Run_slippery Run_boots Idle_scared Idle_normal Idle_brave Idle_teeter Idle_slippery Idle_plunger Idle_LookLeft Idle_LookRight Inactive_LookAround Inactive_SniffAir Inactive_SniffGround Inactive_Magnify Walk_normal Walk_scared Walk_brave Walk_slippery Walk_plunger Walk_sticky Walk_boots", "Run_brave", RunCheck, NULL, 0, 8, 0.0f, 0.0f, 1, 0, 0.15f, NULL);
    tran = xAnimTableNewTransition(table, "Slide Run_scared Run_normal Run_hotsauce Run_brave    Run_boots Idle_scared Idle_normal Idle_brave Idle_teeter Idle_slippery Idle_plunger Idle_LookLeft Idle_LookRight Inactive_LookAround Inactive_SniffAir Inactive_SniffGround Inactive_Magnify Walk_normal Walk_scared Walk_brave Walk_slippery Walk_plunger Walk_sticky Walk_boots", "Run_slippery", RunCheck, NULL, 0, 0x10, 0.0f, 0.0f, 1, 0, 0.15f, NULL);
    tran = xAnimTableNewTransition(table, "Slide Run_scared Run_normal Run_hotsauce Run_brave Run_slippery Idle_scared Idle_normal Idle_brave Idle_teeter Idle_slippery Idle_plunger Idle_LookLeft Idle_LookRight Inactive_LookAround Inactive_SniffAir Inactive_SniffGround Inactive_Magnify Walk_normal Walk_scared Walk_brave Walk_slippery Walk_plunger Walk_sticky Walk_boots", "Run_boots", RunCheck, NULL, 0, 0x80, 0.0f, 0.0f, 1, 0, 0.15f, NULL);
    tran = xAnimTableNewTransition(table, "Run_scared Run_normal Run_brave Run_hotsauce Run_slippery Run_boots", "Walk_sticky", StickySurfWalk, ClearHelmetCB, 0, 0, 0.0f, 0.0f, 1, 0, 0.15f, NULL);
    tran = xAnimTableNewTransition(table, "Run_scared Run_normal Run_brave Run_hotsauce Run_slippery Run_boots", "Idle_normal", StickySurfIdle, ClearHelmetCB, 0, 0, 0.0f, 0.0f, 1, 0, 0.15f, NULL);
    tran = xAnimTableNewTransition(table, "Action_attackrun Action_attack", "Walk_sticky", StickNoBootsWalk, ClearHelmetCB, 0, 0, 0.0f, 0.0f, 1, 0, 0.15f, NULL);
    tran = xAnimTableNewTransition(table, "Action_attackrun Action_attack", "Idle_normal", StickNoBootsIdle, ClearHelmetCB, 0, 0, 0.0f, 0.0f, 1, 0, 0.15f, NULL);
    tran = xAnimTableNewTransition(table, "Run_scared", "RunToIdle_scared", NotRunCheck, NotRunCB, 0, 0x1F, 0.0f, 0.0f, 2, 0, 0.15f, NULL);
    tran = xAnimTableNewTransition(table, "Run_normal", "RunToIdle_normal", NotRunCheck, NotRunCB, 0, 0x1F, 0.0f, 0.0f, 2, 0, 0.15f, NULL);
    tran = xAnimTableNewTransition(table, "Run_brave", "RunToIdle_brave", NotRunCheck, NotRunCB, 0, 0x1F, 0.0f, 0.0f, 2, 0, 0.15f, NULL);
    tran = xAnimTableNewTransition(table, "Run_slippery", "RunToSkid", NotRunCheck, NULL, 0, 0x1F, 0.0f, 0.0f, 2, 0, 0.15f, NULL);
    tran = xAnimTableNewTransition(table, "Run_hotsauce", "RunToIdle_scared", NotRunCheck, NotRunCB, 0, 2, 0.0f, 0.0f, 2, 0, 0.15f, NULL);
    tran = xAnimTableNewTransition(table, "Run_hotsauce", "RunToIdle_normal", NotRunCheck, NotRunCB, 0, 2, 0.0f, 0.0f, 2, 0, 0.15f, NULL);
    tran = xAnimTableNewTransition(table, "Run_hotsauce", "RunToIdle_brave", NotRunCheck, NotRunCB, 0, 2, 0.0f, 0.0f, 2, 0, 0.15f, NULL);
    tran = xAnimTableNewTransition(table, "Run_hotsauce", "RunToSkid", NotRunCheck, NULL, 0, 0x10, 0.0f, 0.0f, 2, 0, 0.15f, NULL);
    tran = xAnimTableNewTransition(table, "RunToIdle_scared", "Idle_scared", NULL, NULL, 0x10, 0, 0.0f, 0.0f, 0, 0, 0.0f, NULL);
    tran = xAnimTableNewTransition(table, "RunToIdle_normal", "Idle_normal", NULL, NULL, 0x10, 0, 0.0f, 0.0f, 0, 0, 0.0f, NULL);
    tran = xAnimTableNewTransition(table, "RunToIdle_brave", "Idle_brave", NULL, NULL, 0x10, 0, 0.0f, 0.0f, 0, 0, 0.0f, NULL);
    tran = xAnimTableNewTransition(table, "Walk_scared Walk_normal Walk_brave Run_scared Run_normal Run_brave", "Walk_sneak", SneakMoveCheck, NULL, 0, 6, 0.0f, 0.0f, 1, 0, 0.15f, NULL);
    xAnimTableAddTransition(table, tran, "Idle_sneak Idle_blackknight Walk_blackknight");
    xAnimTableAddTransition(table, tran, " Idle_scared Idle_normal Idle_brave Idle_teeter Idle_slippery Idle_plunger Idle_LookLeft Idle_LookRight Inactive_LookAround Inactive_SniffAir Inactive_SniffGround Inactive_Magnify");
    tran = xAnimTableNewTransition(table, "Walk_scared Walk_normal Walk_brave Run_scared Run_normal Run_brave", "Walk_blackknight", SneakMoveCheck, NULL, 0, 8, 0.0f, 0.0f, 1, 0, 0.15f, NULL);
    xAnimTableAddTransition(table, tran, "Idle_sneak Idle_blackknight Walk_sneak");
    xAnimTableAddTransition(table, tran, " Idle_scared Idle_normal Idle_brave Idle_teeter Idle_slippery Idle_plunger Idle_LookLeft Idle_LookRight Inactive_LookAround Inactive_SniffAir Inactive_SniffGround Inactive_Magnify");
    tran = xAnimTableNewTransition(table, " Idle_scared Idle_normal Idle_brave Idle_teeter Idle_slippery Idle_plunger Idle_LookLeft Idle_LookRight Inactive_LookAround Inactive_SniffAir Inactive_SniffGround Inactive_Magnify", "IdleToLampshade", SneakIdleCheck, NULL, 0, 4, 0.0f, 0.0f, 1, 0, 0.15f, NULL);
    tran = xAnimTableNewTransition(table, "Walk_sneak", "SneakToLampshade", SneakIdleCheck, NULL, 0, 4, 0.0f, 0.0f, 1, 0, 0.0f, NULL);
    tran = xAnimTableNewTransition(table, "Idle_lampshade", "LampshadeToSneak", SneakMoveCheck, NULL, 0, 4, 0.0f, 0.0f, 1, 0, 0.0f, NULL);
    tran = xAnimTableNewTransition(table, "SneakToLampshade", "LampshadeToSneak", SneakMoveCheck, NULL, 0x20, 4, 0.0f, 0.0f, 2, 0, 0.0f, NULL);
    tran = xAnimTableNewTransition(table, "LampshadeToSneak", "SneakToLampshade", SneakIdleCheck, NULL, 0x20, 4, 0.0f, 0.0f, 2, 0, 0.0f, NULL);
    tran = xAnimTableNewTransition(table, "Walk_sneak Walk_blackknight Idle_scared Idle_normal Idle_brave Idle_teeter Idle_slippery Idle_plunger Idle_LookLeft Idle_LookRight Inactive_LookAround Inactive_SniffAir Inactive_SniffGround Inactive_Magnify", "Idle_sneak", SneakIdleCheck, NULL, 0, 2, 0.0f, 0.0f, 1, 0, 0.15f, NULL);
    tran = xAnimTableNewTransition(table, "Walk_sneak Walk_blackknight Idle_scared Idle_normal Idle_brave Idle_teeter Idle_slippery Idle_plunger Idle_LookLeft Idle_LookRight Inactive_LookAround Inactive_SniffAir Inactive_SniffGround Inactive_Magnify", "Idle_blackknight", SneakIdleCheck, NULL, 0, 8, 0.0f, 0.0f, 1, 0, 0.15f, NULL);
    tran = xAnimTableNewTransition(table, "Idle_sneak Idle_lampshade Idle_blackknight", "Idle_scared", NosneakCheck, NULL, 0, 2, 0.0f, 0.0f, 1, 0, 0.15f, NULL);
    tran = xAnimTableNewTransition(table, "Idle_sneak Idle_lampshade Idle_blackknight", "Idle_normal", NosneakCheck, NULL, 0, 4, 0.0f, 0.0f, 1, 0, 0.15f, NULL);
    tran = xAnimTableNewTransition(table, "Idle_sneak Idle_lampshade Idle_blackknight", "Idle_brave", NosneakCheck, NULL, 0, 8, 0.0f, 0.0f, 1, 0, 0.15f, NULL);
    tran = xAnimTableNewTransition(table, "Idle_sneak Idle_lampshade Idle_blackknight", "Idle_teeter", NosneakTeeter, NULL, 0, 0, 0.0f, 0.0f, 2, 0, 0.15f, NULL);
    tran = xAnimTableNewTransition(table, "Idle_sneak Idle_lampshade Idle_blackknight", "Idle_slippery", NosneakSlippery, NULL, 0, 0, 0.0f, 0.0f, 2, 0, 0.15f, NULL);
    tran = xAnimTableNewTransition(table, "Idle_sneak Idle_lampshade Idle_blackknight", "Idle_plunger", NosneakPlunger, NULL, 0, 0, 0.0f, 0.0f, 2, 0, 0.15f, NULL);
    tran = xAnimTableNewTransition(table, "Walk_sneak Walk_blackknight", "Walk_scared", NosneakCheck, NULL, 0, 2, 0.0f, 0.0f, 1, 0, 0.15f, NULL);
    tran = xAnimTableNewTransition(table, "Walk_sneak Walk_blackknight", "Walk_normal", NosneakCheck, NULL, 0, 4, 0.0f, 0.0f, 1, 0, 0.15f, NULL);
    tran = xAnimTableNewTransition(table, "Walk_sneak Walk_blackknight", "Walk_brave", NosneakCheck, NULL, 0, 8, 0.0f, 0.0f, 1, 0, 0.15f, NULL);
    tran = xAnimTableNewTransition(table, " Idle_scared Idle_normal Idle_brave Idle_teeter Idle_slippery Idle_plunger Idle_LookLeft Idle_LookRight Inactive_LookAround Inactive_SniffAir Inactive_SniffGround Inactive_Magnify", "Run_hotsauce", HotsauceCheck, NULL, 0, 0, 0.0f, 0.0f, 3, 0, 0.15f, NULL);
    xAnimTableAddTransition(table, tran, "Walk_scared Walk_normal Walk_brave Walk_sneak Walk_blackknight");
    xAnimTableAddTransition(table, tran, "Idle_sneak Idle_lampshade Idle_blackknight Idle_teeter Idle_slippery Idle_plunger");
    xAnimTableAddTransition(table, tran, "Run_scared Run_normal Run_brave");
    xAnimTableAddTransition(table, tran, "Inactive_1 Inactive_2 Inactive_3 Inactive_4");
    xAnimTableAddTransition(table, tran, "Inactive_SitStart Inactive_SitLoop Inactive_SitEnd Inactive_SitEndSlow");
    xAnimTableAddTransition(table, tran, "Idle_LookLeft Idle_LookRight Inactive_SitFlea");
    xAnimTableAddTransition(table, tran, "Inactive_SitSniff Inactive_SitHungry Inactive_SitLookLeft Inactive_SniffAir Inactive_SniffGround");
    xAnimTableAddTransition(table, tran, "Inactive_Magnify Inactive_Dizzy Inactive_BlowFeet Inactive_LookAround");
    tran = xAnimTableNewTransition(table, "", "Scare_norm_cower", CowerCheck, NULL, 0, 0, 0.0f, 0.0f, 5, 0, 0.15f, NULL);
    AddGroundStatesTransition(table, tran);
    tran = xAnimTableNewTransition(table, "Scare_norm_cower", "Idle_scared", UncowerCheck, NULL, 0, 2, 0.0f, 0.0f, 5, 0, 0.15f, NULL);
    tran = xAnimTableNewTransition(table, "Scare_norm_cower", "Idle_normal", UncowerCheck, NULL, 0, 4, 0.0f, 0.0f, 5, 0, 0.15f, NULL);
    tran = xAnimTableNewTransition(table, "Scare_norm_cower", "Idle_brave", UncowerCheck, NULL, 0, 8, 0.0f, 0.0f, 5, 0, 0.15f, NULL);
    tran = xAnimTableNewTransition(table, "", "Scare_norm_hit", ScareCheckNoHealth, ScareCB, 0, 0, 0.0f, 0.0f, 5, 0, 0.15f, NULL);
    AddGroundStatesTransition(table, tran);
    tran = xAnimTableNewTransition(table, "", "Scare_critter", ScareCheckCritter, ScareCB, 0, 0, 0.0f, 0.0f, 6, 0, 0.15f, NULL);
    AddGroundStatesTransition(table, tran);
    tran = xAnimTableNewTransition(table, "", "Scare_loseloop", ScareCheck, DeathCB, 0, 1, 0.0f, 0.0f, 0xA, 0, 0.15f, NULL);
    AddGroundStatesTransition(table, tran);
    xAnimTableAddTransition(table, tran, "Powerup_helmet Powerup_lamp Powerup_plunger Powerup_soap Scare_brave");
    xAnimTableAddTransition(table, tran, "Scare_critter Scare_norm_cower Scare_norm_hit Scare_normal Scare_thorns");
    tran = xAnimTableNewTransition(table, "Scare_airlose", "Scare_losefall", NULL, DeathCB, 0x10, 0, 0.0f, 0.0f, 0, 0, 0.0f, NULL);
    tran = xAnimTableNewTransition(table, "Scare_losefall", "Scare_loseland", LandCheck, NULL, 0, 0, 0.0f, 0.0f, 0, 0, 0.0f, NULL);
    tran = xAnimTableNewTransition(table, "", "Fall_losechance", FallToDeathStart, ButtSmashCB, 0, 0, 0.0f, 0.0f, 0x14, 0, 0.15f, NULL);
    for (currst = table->StateList; currst; currst = currst->Next)
    {
        if (xStricmp(currst->Name, "Fall_losechance") && xStricmp(currst->Name, "Fall_losedrop"))
        {
            xAnimTableAddTransition(table, tran, currst->Name);
        }
    }
    tran = xAnimTableNewTransition(table, "Fall_losechance", "Fall_losedrop", FallToDeathDrop, FallToDeathDropCB, 0, 0, 0.0f, 0.0f, 1, 0, 0.15f, NULL);
    tran = xAnimTableNewTransition(table, "Jump_normal Jump_run Jump_walk Jump_double Jump_buttbounce Jump_hang Jump_springboard", "Scare_airhit", ScareCheck, ScareCB, 0, 0xF, 0.0f, 0.0f, 5, 0, 0.15f, NULL);
    xAnimTableAddTransition(table, tran, "Fall_normal Fall_run Fall_walk Fall_high Fall_floatup");
    xAnimTableAddTransition(table, tran, "Fall_floatdown Fall_floatopen HangGrab Hang HangToSwing SwingToHang");
    xAnimTableAddTransition(table, tran, "Buttsmash_start Buttsmash_fall Fall_spring Fall_fromspring");
    tran = xAnimTableNewTransition(table, "Jump_normal Jump_run Jump_walk Jump_double Jump_buttbounce Jump_hang Jump_springboard", "Scare_airlose", ScareCheck, ScareKnockbackCB, 0, 1, 0.0f, 0.0f, 7, 0, 0.15f, NULL);
    xAnimTableAddTransition(table, tran, "Fall_normal Fall_run Fall_walk Fall_high Fall_floatup");
    xAnimTableAddTransition(table, tran, "Fall_floatdown Fall_floatopen HangGrab Hang HangToSwing SwingToHang");
    xAnimTableAddTransition(table, tran, "Buttsmash_start Buttsmash_fall Fall_spring Fall_fromspring");
    xAnimTableAddTransition(table, tran, "Scare_airhit Scare_airfall Scare_airland");
    tran = xAnimTableNewTransition(table, " Idle_scared Idle_normal Idle_brave Idle_teeter Idle_slippery Idle_plunger Idle_LookLeft Idle_LookRight Inactive_LookAround Inactive_SniffAir Inactive_SniffGround Inactive_Magnify", "Jump_normal", JumpCheck, JumpCB, 0, 0, 0.0f, 0.0f, 4, 0, 0.15f, NULL);
    xAnimTableAddTransition(table, tran, "Idle_sneak Idle_lampshade Idle_blackknight Walk_sneak Walk_blackknight");
    xAnimTableAddTransition(table, tran, "Idle_teeter Idle_slippery Idle_plunger Slide Inactive_1 Inactive_2 Inactive_3 Inactive_4");
    xAnimTableAddTransition(table, tran, "Inactive_SitStart Inactive_SitLoop Inactive_SitEnd Inactive_SitEndSlow");
    xAnimTableAddTransition(table, tran, "Idle_LookLeft Idle_LookRight Inactive_SitFlea");
    xAnimTableAddTransition(table, tran, "Land_normal Land_walkidle");
    xAnimTableAddTransition(table, tran, "RunToIdle_scared RunToIdle_normal RunToIdle_brave");
    xAnimTableAddTransition(table, tran, "RunToSkid Skid SkidToIdle Walk_slippery Walk_plunger Walk_boots Idle_plunger");
    xAnimTableAddTransition(table, tran, "Inactive_SitSniff Inactive_SitHungry Inactive_SitLookLeft Inactive_SniffAir Inactive_SniffGround");
    xAnimTableAddTransition(table, tran, "Inactive_Magnify Inactive_Dizzy Inactive_BlowFeet Inactive_LookAround");
    tran = xAnimTableNewTransition(table, "Fall_spring", "Fall_fromspring", (xAnimTransitionConditionalCallback)NospringCheck, NULL, 0, 0, 0.0f, 0.0f, 1, 0, 0.1f, NULL);
    tran = xAnimTableNewTransition(table, "Walk_normal Walk_scared Walk_brave Land_walk", "Jump_walk", JumpCheck, JumpCB, 0, 0, 0.0f, 0.0f, 4, 0, 0.15f, NULL);
    tran = xAnimTableNewTransition(table, "Run_normal Run_scared Run_brave Run_hotsauce Land_running Run_slippery Run_boots", "Jump_run", JumpCheck, JumpCB, 0, 0, 0.0f, 0.0f, 4, 0, 0.15f, NULL);
    tran = xAnimTableNewTransition(table, "Fall_normal Fall_run Fall_walk Fall_floatup Fall_floatdown Fall_floatopen Fall_high Fall_spring Fall_fromspring Jump_hang", "Jump_double", DblJumpCheck, DblJumpCB, 0, 0, 0.0f, 0.0f, 4, 0, 0.15f, NULL);
    tran = xAnimTableNewTransition(table, "Jump_normal Jump_run Jump_walk Jump_springboard", "Jump_double", DblJumpCheckTime, DblJumpCB, 0, 0, 0.0f, 0.0f, 4, 0, 0.15f, NULL);
    tran = xAnimTableNewTransition(table, "Buttsmash_start Scare_airhit Scare_airfall", "Jump_buttbounce", BounceCheck, BounceCB, 0, 0, 0.0f, 0.0f, 5, 0, 0.15f, NULL);
    xAnimTableAddTransition(table, tran, "Fall_normal Fall_run Fall_walk Fall_floatup Fall_floatdown Fall_floatopen Fall_high");
    xAnimTableAddTransition(table, tran, "Jump_normal Jump_run Jump_walk");
    xAnimTableAddTransition(table, tran, "Jump_double Jump_buttbounce Jump_hang Jump_springboard");
    xAnimTableAddTransition(table, tran, "Fall_spring Fall_fromspring");
    tran = xAnimTableNewTransition(table, "Buttsmash_start Scare_airhit Scare_airfall", "Jump_springboard", SpringCheck, BounceCB, 0, 0, 0.0f, 0.0f, 5, 0, 0.15f, NULL);
    xAnimTableAddTransition(table, tran, "Fall_normal Fall_run Fall_walk Fall_floatup Fall_floatdown Fall_floatopen Fall_high");
    xAnimTableAddTransition(table, tran, "Jump_normal Jump_run Jump_walk");
    xAnimTableAddTransition(table, tran, "Jump_double Jump_buttbounce Jump_hang Jump_springboard");
    xAnimTableAddTransition(table, tran, "Fall_spring Fall_fromspring");
    tran = xAnimTableNewTransition(table, "Fall_normal Fall_run Fall_walk Fall_floatup Fall_floatdown Fall_floatopen Fall_high", "Hang", HangCheck, HangCB, 0, 0, 0.0f, 0.0f, 5, 0, 0.1f, NULL);
    xAnimTableAddTransition(table, tran, "Jump_normal Jump_walk Jump_run Jump_double");
    xAnimTableAddTransition(table, tran, "Fall_spring Fall_fromspring Jump_springboard");
    tran = xAnimTableNewTransition(table, "Hang HangGrab SwingToHang HangToSwing", "Jump_hang", JumpCheck, JumpHangCB, 0, 0, 0.0f, 0.0f, 4, 0, 0.15f, NULL);
    tran = xAnimTableNewTransition(table, "Hang HangGrab SwingToHang HangToSwing", "Fall_normal", NoHangCheck, NULL, 0, 0, 0.0f, 0.0f, 4, 0, 0.15f, NULL);
    tran = xAnimTableNewTransition(table, "Hang", "HangToSwing", HangSwingCheck, NULL, 0, 0, 0.0f, 0.0f, 1, 0, 0.1f, NULL);
    tran = xAnimTableNewTransition(table, "HangToSwing", "SwingToHang", HangNoSwingCheck, NULL, 0, 0, 0.0f, 0.0f, 1, 0, 0.1f, NULL);
    tran = xAnimTableNewTransition(table, "Walk_scared Walk_normal Walk_brave Idle_scared Idle_normal Idle_brave Idle_teeter Idle_slippery Idle_plunger Idle_LookLeft Idle_LookRight Inactive_LookAround Inactive_SniffAir Inactive_SniffGround Inactive_Magnify", "Fall_normal", FallingCheck, NULL, 0, 0, 0.0f, 0.0f, 5, 0, 0.15f, NULL);
    xAnimTableAddTransition(table, tran, "Idle_sneak Idle_lampshade Idle_blackknight Idle_plunger Idle_slippery Idle_teeter Slide");
    xAnimTableAddTransition(table, tran, "Walk_sneak Walk_blackknight Land_normal Land_walk Land_walkidle Land_running");
    xAnimTableAddTransition(table, tran, "Inactive_1 Inactive_2 Inactive_3 Inactive_4");
    xAnimTableAddTransition(table, tran, "Inactive_SitStart Inactive_SitLoop Inactive_SitEnd Inactive_SitEndSlow");
    xAnimTableAddTransition(table, tran, "Idle_LookLeft Idle_LookRight Inactive_SitFlea");
    xAnimTableAddTransition(table, tran, "Walk_slippery Walk_plunger Walk_sticky Walk_boots");
    xAnimTableAddTransition(table, tran, "Inactive_SitSniff Inactive_SitHungry Inactive_SitLookLeft Inactive_SniffAir Inactive_SniffGround");
    xAnimTableAddTransition(table, tran, "Inactive_Magnify Inactive_Dizzy Inactive_BlowFeet Inactive_LookAround");
    tran = xAnimTableNewTransition(table, "Run_normal Run_scared Run_brave Run_hotsauce", "Fall_run", FallingCheck, NULL, 0, 0, 0.0f, 0.0f, 5, 0, 0.15f, NULL);
    tran = xAnimTableNewTransition(table, "Fall_normal Fall_run Fall_walk Fall_high Fall_spring Fall_fromspring", "Fall_floatopen", FloatCheck, NULL, 0, 4, 0.0f, 0.0f, 2, 0, 0.15f, NULL);
    tran = xAnimTableNewTransition(table, "Fall_floatopen", "Fall_floatdown", NULL, NULL, 0x10, 0, 0.0f, 0.0f, 1, 0, 0.1f, NULL);
    tran = xAnimTableNewTransition(table, "Fall_floatopen", "Fall_floatup", FloatCheck, NULL, 0x10, 2, 0.0f, 0.0f, 2, 0, 0.1f, NULL);
    tran = xAnimTableNewTransition(table, "Fall_floatdown", "Fall_floatup", FloatCheck, NULL, 0, 2, 0.0f, 0.0f, 2, 0, 0.15f, NULL);
    tran = xAnimTableNewTransition(table, "Fall_floatup", "Fall_floatdown", FloatCheck, NULL, 0, 1, 0.0f, 0.0f, 2, 0, 0.15f, NULL);
    tran = xAnimTableNewTransition(table, "Fall_floatdown Fall_floatup", "Fall_normal", NotFloatCheck, NULL, 0, 0, 0.0f, 0.0f, 2, 0, 0.15f, NULL);
    tran = xAnimTableNewTransition(table, "Fall_normal Fall_run Fall_walk Fall_floatup Fall_floatdown Fall_floatopen Fall_high Jump_double Jump_walk Jump_run Jump_normal Jump_hang Fall_spring Fall_fromspring", "Buttsmash_start", ButtSmashCheck, ButtSmashCB, 0, 0, 0.0f, 0.0f, 5, 0, 0.1f, NULL);
    tran = xAnimTableNewTransition(table, "Buttsmash_start", "Buttsmash_fall", NULL, ButtSmashStateCB, 0x10, 0, 0.0f, 0.0f, 1, 0, 0.0f, NULL);
    tran = xAnimTableNewTransition(table, "Buttsmash_fall", "Buttsmash_land", LandCheck, ButtSmashLandCB, 0, 0, 0.0f, 0.0f, 5, 0, 0.0f, NULL);
    tran = xAnimTableNewTransition(table, "Buttsmash_fall", "Jump_buttbounce", BounceCheck, BounceCB, 0, 0, 0.0f, 0.0f, 5, 0, 0.15f, NULL);
    tran = xAnimTableNewTransition(table, "Buttsmash_fall", "Jump_springboard", SpringCheck, BounceCB, 0, 0, 0.0f, 0.0f, 5, 0, 0.1f, NULL);
    tran = xAnimTableNewTransition(table, "Buttsmash_land", "Idle_scared", HealthCheck, NULL, 0x10, 2, 0.0f, 0.0f, 0, 0, 0.1f, NULL);
    tran = xAnimTableNewTransition(table, "Buttsmash_land", "Idle_normal", HealthCheck, NULL, 0x10, 4, 0.0f, 0.0f, 0, 0, 0.1f, NULL);
    tran = xAnimTableNewTransition(table, "Buttsmash_land", "Idle_brave", HealthCheck, NULL, 0x10, 8, 0.0f, 0.0f, 0, 0, 0.1f, NULL);
    tran = xAnimTableNewTransition(table, "Buttsmash_hit", "Idle_scared", HealthCheck, NULL, 0x10, 2, 0.0f, 0.0f, 0, 0, 0.1f, NULL);
    tran = xAnimTableNewTransition(table, "Buttsmash_hit", "Idle_normal", HealthCheck, NULL, 0x10, 4, 0.0f, 0.0f, 0, 0, 0.1f, NULL);
    tran = xAnimTableNewTransition(table, "Buttsmash_hit", "Idle_brave", HealthCheck, NULL, 0x10, 8, 0.0f, 0.0f, 0, 0, 0.1f, NULL);
    tran = xAnimTableNewTransition(table, "Fall_normal Fall_run Fall_floatup Fall_floatdown Fall_floatopen Fall_high", "Land_normal", LandSlowCheck, NULL, 0, 0, 0.0f, 0.0f, 4, 0, 0.15f, NULL);
    tran = xAnimTableNewTransition(table, "Fall_walk", "Land_walkidle", LandSlowCheck, NULL, 0, 0, 0.0f, 0.0f, 4, 0, 0.15f, NULL);
    tran = xAnimTableNewTransition(table, "Fall_normal Fall_run Fall_walk Fall_floatup Fall_floatdown Fall_floatopen Fall_high", "Land_walk", LandWalkCheck, NULL, 0, 0, 0.0f, 0.0f, 4, 0, 0.15f, NULL);
    tran = xAnimTableNewTransition(table, "Fall_normal Fall_run Fall_walk Fall_floatup Fall_floatdown Fall_floatopen Fall_high", "Land_running", LandFastCheck, NULL, 0, 0, 0.0f, 0.0f, 4, 0, 0.15f, NULL);
    tran = xAnimTableNewTransition(table, "Fall_spring Fall_fromspring", "Land_normal", LandCheck, NULL, 0, 0, 0.0f, 0.0f, 4, 0, 0.15f, NULL);
    tran = xAnimTableNewTransition(table, "Scare_airfall", "Scare_airland", ScareLandCheck, NULL, 0, 0, 0.0f, 0.0f, 5, 0, 0.15f, NULL);
    tran = xAnimTableNewTransition(table, "Scare_airland", "Idle_scared", HealthCheck, NULL, 0x10, 2, 0.0f, 0.0f, 0, 0, 0.1f, NULL);
    tran = xAnimTableNewTransition(table, "Scare_airland", "Idle_normal", HealthCheck, NULL, 0x10, 4, 0.0f, 0.0f, 0, 0, 0.1f, NULL);
    tran = xAnimTableNewTransition(table, "Scare_airland", "Idle_brave", HealthCheck, NULL, 0x10, 8, 0.0f, 0.0f, 0, 0, 0.1f, NULL);
    tran = xAnimTableNewTransition(table, "Idle_scared Idle_normal Idle_brave", "Inactive_SitStart", IdleSitCheck, NULL, 0, 0, 0.0f, 0.0f, 2, 0, 0.1f, NULL);
    tran = xAnimTableNewTransition(table, "Inactive_SitStart", "Inactive_SitLoop", NULL, IdleSitCB, 0x10, 0, 0.0f, 0.0f, 1, 0, 0.0f, NULL);
    tran = xAnimTableNewTransition(table, "Inactive_SitLoop Inactive_SitFlea Inactive_SitSniff Inactive_SitHungry Inactive_SitLookLeft", "Inactive_SitEnd", ActiveCheck, NULL, 0, 0xE, 0.0f, 0.0f, 1, 0, 0.15f, NULL);
    tran = xAnimTableNewTransition(table, "Inactive_SitEndSlow", "Inactive_SitEnd", ActiveCheck, NULL, 0x20, 0, 0.0f, 0.0f, 2, 0, 0.0f, NULL);
    tran = xAnimTableNewTransition(table, "Inactive_SitLoop", "Inactive_SitEndSlow", IdleGetupCheck, IdleGetupCB, 0, 0, 0.0f, 0.0f, 2, 0, 0.1f, NULL);
    tran = xAnimTableNewTransition(table, "Inactive_SitEnd Inactive_SitEndSlow", "Idle_scared", HealthCheck, NULL, 0x10, 2, 0.0f, 0.0f, 1, 0, 0.2f, NULL);
    tran = xAnimTableNewTransition(table, "Inactive_SitEnd Inactive_SitEndSlow", "Idle_normal", HealthCheck, NULL, 0x10, 4, 0.0f, 0.0f, 1, 0, 0.2f, NULL);
    tran = xAnimTableNewTransition(table, "Inactive_SitEnd Inactive_SitEndSlow", "Idle_brave", HealthCheck, NULL, 0x10, 8, 0.0f, 0.0f, 1, 0, 0.2f, NULL);
    tran = xAnimTableNewTransition(table, "Inactive_SitLoop", "Inactive_SitSniff", IdleMinorCheck, IdleMinorCB, 0, 0x20000, 0.0f, 0.0f, 1, 0, 0.1f, NULL);
    tran = xAnimTableNewTransition(table, "Inactive_SitLoop", "Inactive_SitLookLeft", IdleMinorCheck, IdleMinorCB, 0, 0x20001, 0.0f, 0.0f, 1, 0, 0.1f, NULL);
    tran = xAnimTableNewTransition(table, "Inactive_SitLoop", "Inactive_SitFlea", IdleMajorCheck, IdleMajorCB, 0, 0x30000, 0.0f, 0.0f, 2, 0, 0.1f, NULL);
    tran = xAnimTableNewTransition(table, "Inactive_SitLoop", "Inactive_SitHungry", IdleMajorCheck, IdleMajorCB, 0, 0x30001, 0.0f, 0.0f, 2, 0, 0.1f, NULL);
    tran = xAnimTableNewTransition(table, "Inactive_SitLoop", "Inactive_Magnify", IdleMajorCheck, IdleMajorCB, 0, 0x30002, 0.0f, 0.0f, 2, 0, 0.1f, NULL);
    tran = xAnimTableNewTransition(table, "Inactive_SitFlea Inactive_SitSniff Inactive_SitHungry Inactive_Magnify Inactive_SitLookLeft", "Inactive_SitLoop", NULL, NULL, 0x10, 0, 0.0f, 0.0f, 0, 0, 0.1f, NULL);
    tran = xAnimTableNewTransition(table, "Idle_normal Idle_brave", "Idle_LookLeft", IdleMinorCheck, IdleMinorCB, 0, 0x40000, 0.0f, 0.0f, 1, 0, 0.2f, NULL);
    tran = xAnimTableNewTransition(table, "Idle_normal Idle_brave", "Idle_LookRight", IdleMinorCheck, IdleMinorCB, 0, 0x40001, 0.0f, 0.0f, 1, 0, 0.2f, NULL);
    tran = xAnimTableNewTransition(table, "Idle_normal Idle_brave", "Inactive_SniffAir", IdleMinorCheck, IdleMinorCB, 0, 0x40002, 0.0f, 0.0f, 1, 0, 0.2f, NULL);
    tran = xAnimTableNewTransition(table, "Idle_normal Idle_brave", "Inactive_SniffGround", IdleMinorCheck, IdleMinorCB, 0, 0x40003, 0.0f, 0.0f, 1, 0, 0.2f, NULL);
    tran = xAnimTableNewTransition(table, "Idle_normal Idle_brave", "Inactive_LookAround", IdleMajorCheck, IdleMajorCB, 0, 0x10000, 0.0f, 0.0f, 2, 0, 0.1f, NULL);
    tran = xAnimTableNewTransition(table, "Idle_LookLeft Idle_LookRight Inactive_SniffAir Inactive_SniffGround Inactive_LookAround", "Idle_scared", HealthCheck, NULL, 0x10, 2, 0.0f, 0.0f, 1, 0, 0.2f, NULL);
    tran = xAnimTableNewTransition(table, "Idle_LookLeft Idle_LookRight Inactive_SniffAir Inactive_SniffGround Inactive_LookAround", "Idle_normal", HealthCheck, NULL, 0x10, 4, 0.0f, 0.0f, 1, 0, 0.2f, NULL);
    tran = xAnimTableNewTransition(table, "Idle_LookLeft Idle_LookRight Inactive_SniffAir Inactive_SniffGround Inactive_LookAround", "Idle_brave", HealthCheck, NULL, 0x10, 8, 0.0f, 0.0f, 1, 0, 0.2f, NULL);
    tran = xAnimTableNewTransition(table, " Idle_scared Idle_normal Idle_brave Idle_teeter Idle_slippery Idle_plunger Idle_LookLeft Idle_LookRight Inactive_LookAround Inactive_SniffAir Inactive_SniffGround Inactive_Magnify Walk_normal Walk_scared Walk_brave Walk_slippery Walk_plunger Walk_sticky Walk_boots", "Powerup_helmet", PowerupCheck, PowerupCB, 0, 0xB, 0.0f, 0.0f, 3, 0, 0.15f, NULL);
    tran = xAnimTableNewTransition(table, " Idle_scared Idle_normal Idle_brave Idle_teeter Idle_slippery Idle_plunger Idle_LookLeft Idle_LookRight Inactive_LookAround Inactive_SniffAir Inactive_SniffGround Inactive_Magnify Walk_normal Walk_scared Walk_brave Walk_slippery Walk_plunger Walk_sticky Walk_boots", "Powerup_plunger", PowerupCheck, PowerupCB, 0, 3, 0.0f, 0.0f, 3, 0, 0.15f, NULL);
    tran = xAnimTableNewTransition(table, " Idle_scared Idle_normal Idle_brave Idle_teeter Idle_slippery Idle_plunger Idle_LookLeft Idle_LookRight Inactive_LookAround Inactive_SniffAir Inactive_SniffGround Inactive_Magnify Walk_normal Walk_scared Walk_brave Walk_slippery Walk_plunger Walk_sticky Walk_boots", "Powerup_soap", PowerupCheck, PowerupCB, 0, 1, 0.0f, 0.0f, 3, 0, 0.15f, NULL);
    tran = xAnimTableNewTransition(table, " Idle_scared Idle_normal Idle_brave Idle_teeter Idle_slippery Idle_plunger Idle_LookLeft Idle_LookRight Inactive_LookAround Inactive_SniffAir Inactive_SniffGround Inactive_Magnify Walk_normal Walk_scared Walk_brave Walk_slippery Walk_plunger Walk_sticky Walk_boots", "Powerup_lamp", PowerupCheck, PowerupCB, 0, 5, 0.0f, 0.0f, 3, 0, 0.15f, NULL);
    tran = xAnimTableNewTransition(table, "Powerup_helmet Powerup_plunger Powerup_soap Powerup_lamp", "Idle_scared", HealthCheck, NULL, 0x10, 2, 0.0f, 0.0f, 1, 0, 0.2f, NULL);
    tran = xAnimTableNewTransition(table, "Powerup_helmet Powerup_plunger Powerup_soap Powerup_lamp", "Idle_normal", HealthCheck, NULL, 0x10, 4, 0.0f, 0.0f, 1, 0, 0.2f, NULL);
    tran = xAnimTableNewTransition(table, "Powerup_helmet Powerup_plunger Powerup_soap Powerup_lamp", "Idle_brave", HealthCheck, NULL, 0x10, 8, 0.0f, 0.0f, 1, 0, 0.2f, NULL);
    tran = xAnimTableNewTransition(table, " Idle_scared Idle_normal Idle_brave Idle_teeter Idle_slippery Idle_plunger Idle_LookLeft Idle_LookRight Inactive_LookAround Inactive_SniffAir Inactive_SniffGround Inactive_Magnify", "Action_projectile", ProjectileCheck, ProjectileCB, 0, 0, 0.0f, 0.0f, 4, 0, 0.15f, NULL);
    xAnimTableAddTransition(table, tran, "Walk_normal Walk_scared Walk_brave");
    xAnimTableAddTransition(table, tran, "Idle_sneak Idle_lampshade Idle_blackknight Walk_sneak Walk_blackknight");
    xAnimTableAddTransition(table, tran, "Idle_teeter Idle_slippery Idle_plunger Inactive_1 Inactive_2 Inactive_3 Inactive_4");
    xAnimTableAddTransition(table, tran, "Run_normal Run_scared Run_brave");
    tran = xAnimTableNewTransition(table, "Action_projectile", "Action_projectile", ProjectileRepeatSync, ProjectileCB, 0, 0, globals.player.s.FireRepeat, 0.0f, 4, 5, 10.0f, NULL);
    tran = xAnimTableNewTransition(table, "Action_projectile", "Action_projectile", ProjectileRepeat, ProjectileCB, 0, 0, 0.0f, 0.0f, 4, 0, 10.0f, NULL);
    tran = xAnimTableNewTransition(table, " Idle_scared Idle_normal Idle_brave Idle_teeter Idle_slippery Idle_plunger Idle_LookLeft Idle_LookRight Inactive_LookAround Inactive_SniffAir Inactive_SniffGround Inactive_Magnify", "Action_attackrun", AttackCheck, AttackCB, 0, 0, 0.0f, 0.0f, 4, 0, 0.15f, NULL);
    xAnimTableAddTransition(table, tran, "Walk_normal Walk_scared Walk_brave");
    xAnimTableAddTransition(table, tran, "Idle_sneak Idle_lampshade Idle_blackknight Walk_sneak Walk_blackknight");
    xAnimTableAddTransition(table, tran, "Idle_teeter Idle_slippery Idle_plunger Inactive_1 Inactive_2 Inactive_3 Inactive_4");
    xAnimTableAddTransition(table, tran, "Run_normal Run_scared Run_brave Run_hotsauce");
    xAnimTableAddTransition(table, tran, "Land_normal Land_walk Land_walkidle Land_running");
    xAnimTableAddTransition(table, tran, "Walk_boots Run_boots");
    tran = xAnimTableNewTransition(table, "Action_Attack Action_AttackRun", "Action_attackhit", HeadHitCheck, HeadHitCB, 0, 0, 0.0f, 0.0f, 2, 0, 0.1f, NULL);
    tran = xAnimTableNewTransition(table, "Action_attackhit", "Idle_scared", HealthCheck, NULL, 0x10, 2, 0.0f, 0.0f, 0, 0, 0.1f, NULL);
    tran = xAnimTableNewTransition(table, "Action_attackhit", "Idle_normal", HealthCheck, NULL, 0x10, 4, 0.0f, 0.0f, 0, 0, 0.1f, NULL);
    tran = xAnimTableNewTransition(table, "Action_attackhit", "Idle_brave", HealthCheck, NULL, 0x10, 8, 0.0f, 0.0f, 0, 0, 0.1f, NULL);
    tran = xAnimTableNewTransition(table, "Action_attackrun", "RunToIdle_scared", AttackFinished, NotRunCB, 0, 2, 0.0f, 0.0f, 1, 0, 0.15f, NULL);
    tran = xAnimTableNewTransition(table, "Action_attackrun", "RunToIdle_normal", AttackFinished, NotRunCB, 0, 4, 0.0f, 0.0f, 1, 0, 0.15f, NULL);
    tran = xAnimTableNewTransition(table, "Action_attackrun", "RunToIdle_brave", AttackFinished, NotRunCB, 0, 8, 0.0f, 0.0f, 1, 0, 0.15f, NULL);
    tran = xAnimTableNewTransition(table, "Action_attackrun", "Run_scared", AttackFinishedRun, NULL, 0, 2, 0.0f, 0.0f, 2, 0, 0.15f, NULL);
    tran = xAnimTableNewTransition(table, "Action_attackrun", "Run_normal", AttackFinishedRun, NULL, 0, 4, 0.0f, 0.0f, 2, 0, 0.15f, NULL);
    tran = xAnimTableNewTransition(table, "Action_attackrun", "Run_brave", AttackFinishedRun, NULL, 0, 8, 0.0f, 0.0f, 2, 0, 0.15f, NULL);
    tran = xAnimTableNewTransition(table, "Action_attackrun", "Run_hotsauce", AttackFinishedHotsauce, NULL, 0, 1, 0.0f, 0.0f, 3, 0, 0.15f, NULL);
    tran = xAnimTableNewTransition(table, " Idle_scared Idle_normal Idle_brave Idle_teeter Idle_slippery Idle_plunger Idle_LookLeft Idle_LookRight Inactive_LookAround Inactive_SniffAir Inactive_SniffGround Inactive_Magnify", "Action_dig", DigCheck, DigCB, 0, 0, 0.0f, 0.0f, 4, 0, 0.15f, NULL);
    xAnimTableAddTransition(table, tran, "Walk_normal Walk_scared Walk_brave");
    xAnimTableAddTransition(table, tran, "Idle_sneak Idle_lampshade Idle_blackknight Walk_sneak Walk_blackknight");
    xAnimTableAddTransition(table, tran, "Inactive_1 Inactive_2 Inactive_3 Inactive_4");
    tran = xAnimTableNewTransition(table, "Action_dig", "Idle_scared", StopDigCheck, NULL, 0, 2, 0.0f, 0.0f, 0, 0, 0.15f, NULL);
    tran = xAnimTableNewTransition(table, "Action_dig", "Idle_normal", StopDigCheck, NULL, 0, 4, 0.0f, 0.0f, 0, 0, 0.15f, NULL);
    tran = xAnimTableNewTransition(table, "Action_dig", "Idle_brave", StopDigCheck, NULL, 0, 8, 0.0f, 0.0f, 0, 0, 0.15f, NULL);

    return table;
}

static U32 FaceBlow(xAnimTransition* tran, xAnimSingle* anim)
{
    return globals.player.Face_AnimSpecific == 6;
}

static U32 FaceMad(xAnimTransition* tran, xAnimSingle* anim)
{
    return globals.player.Face_AnimSpecific == 5;
}

static U32 FaceClearScareCB(xAnimTransition* tran, xAnimSingle* anim)
{
    globals.player.Face_ScareTimer = 0.0f;
    return 0;
}

static U32 FaceScare(xAnimTransition* tran, xAnimSingle* anim, void* param_3)
{
    return 0.0f != globals.player.Face_ScareTimer &&
           globals.player.Face_ScareRandom == tran->UserFlags;
}

static U32 FaceEvent(xAnimTransition* tran, xAnimSingle* anim, void* param_3)
{
    return 0.0f != globals.player.Face_EventTimer && globals.player.Face_Event == tran->UserFlags &&
           0.0f == globals.player.Face_ScareTimer && globals.player.Face_AnimSpecific < 5;
}

static U32 FaceRun(xAnimTransition* tran, xAnimSingle* anim, void* param_3)
{
    return globals.player.Face_AnimSpecific == 4 && 0.0f == globals.player.Face_ScareTimer &&
           0.0f == globals.player.Face_EventTimer;
}

static U32 FacePant(xAnimTransition* tran, xAnimSingle* anim, void* param_3)
{
    return 0.0f != globals.player.Face_PantTimer && globals.player.Face_AnimSpecific < 4 &&
           0.0f == globals.player.Face_ScareTimer && 0.0f == globals.player.Face_EventTimer;
}

static U32 FaceTeeter(xAnimTransition* tran, xAnimSingle* anim, void* param_3)
{
    return globals.player.Face_AnimSpecific == 1 && 0.0f == globals.player.Face_ScareTimer &&
           0.0f == globals.player.Face_EventTimer && 0.0f == globals.player.Face_PantTimer;
}

static U32 FaceSneak(xAnimTransition* tran, xAnimSingle* anim, void* param_3)
{
    return globals.player.Face_AnimSpecific == 2 && 0.0f == globals.player.Face_ScareTimer &&
           0.0f == globals.player.Face_EventTimer && 0.0f == globals.player.Face_PantTimer;
}

static U32 FaceSwing(xAnimTransition* tran, xAnimSingle* anim, void* param_3)
{
    return globals.player.Face_AnimSpecific == 3 && 0.0f == globals.player.Face_ScareTimer &&
           0.0f == globals.player.Face_EventTimer && 0.0f == globals.player.Face_PantTimer;
}

static U32 FaceIdle(xAnimTransition* tran, xAnimSingle* anim, void* param_3)
{
    return globals.player.Face_AnimSpecific == 0 && 0.0f == globals.player.Face_ScareTimer &&
           0.0f == globals.player.Face_EventTimer && 0.0f == globals.player.Face_PantTimer;
}

static void PlayerFaceUpdate(F32 dt)
{
    xAnimState* astate;

    if (globals.player.Face_ScareTimer > 0.0f)
    {
        globals.player.Face_ScareTimer -= dt;
        if (globals.player.Face_ScareTimer < 0.0f)
        {
            globals.player.Face_ScareTimer = 0.0f;
        }
    }
    if (globals.player.Face_EventTimer > 0.0f)
    {
        globals.player.Face_EventTimer -= dt;
        if (globals.player.Face_EventTimer < 0.0f)
        {
            globals.player.Face_EventTimer = 0.0f;
        }
    }
    if (globals.player.Face_PantTimer > 0.0f)
    {
        globals.player.Face_PantTimer -= dt;
        if (globals.player.Face_PantTimer < 0.0f)
        {
            globals.player.Face_PantTimer = 0.0f;
        }
    }
    globals.player.Face_AnimSpecific = 0;
    astate = globals.player.ent.model->Anim->Single->State;
    if (astate->UserFlags & 0x40000)
    {
        globals.player.Face_AnimSpecific = 1;
    }
    if ((astate->UserFlags & 0x1E) == 2)
    {
        globals.player.Face_AnimSpecific = 2;
    }
    if ((astate->UserFlags & 0x1E) == 6 && !(astate->UserFlags & 0x20))
    {
        globals.player.Face_AnimSpecific = 4;
    }
    if (astate->UserFlags & 0x20000)
    {
        globals.player.Face_AnimSpecific = 3;
    }
    if (astate->UserFlags & 0x10000)
    {
        globals.player.Face_AnimSpecific = 5;
    }
    if (astate->UserFlags & 0x80000)
    {
        globals.player.Face_AnimSpecific = 6;
    }
}

static inline void AddTransitionToAllStates(xAnimTable* table, xAnimTransition* tran)
{
    xAnimState* currst;

    for (currst = table->StateList; currst; currst = currst->Next)
    {
        if (xStricmp(currst->Name, tran->Dest->Name))
        {
            xAnimTableAddTransition(table, tran, currst->Name);
        }
    }
}

xAnimTable* zEntPlayer_HeadAnimTable()
{
    xAnimTable* table;
    xAnimTransition* tran;

    table = xAnimTableNew("PlayerHead", NULL);
    xAnimTableNewState(table, "Idle", 0, 0, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "IdleA", 0, 0, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "IdleB", 0, 0, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "IdleC", 0, 0, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "ScareA", 0, 0, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "ScareB", 0, 0, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "ScareC", 0, 0, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "ScareD", 0, 0, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "ScareE", 0, 0, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "ScareF", 0, 0, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "Blink", 0, 0, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "Blow", 0, 0, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "Dizzy", 0, 0, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "Grin", 0, 0, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "Happy", 0, 0, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "Mad", 0, 0, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "Sneak", 0, 0, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "Swing", 0, 0, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "Teeter", 0, 0, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "Run", 0x10, 0, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "Pant", 0x10, 0, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "LickFace", 0x20, 0, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "WinkGrin", 0x20, 0, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewTransition(table, "LickFace WinkGrin", "Idle", NULL, NULL, 0x10, 0, 0.0f, 0.0f, 0, 0, 0.0f, NULL);
    tran = xAnimTableNewTransition(table, "", "ScareA", FaceScare, NULL, 0, 0, 0.0f, 0.0f, 6, 0, 0.0f, NULL);
    AddTransitionToAllStates(table, tran);
    tran = xAnimTableNewTransition(table, "", "ScareB", FaceScare, NULL, 0, 1, 0.0f, 0.0f, 6, 0, 0.0f, NULL);
    AddTransitionToAllStates(table, tran);
    tran = xAnimTableNewTransition(table, "", "ScareC", FaceScare, NULL, 0, 2, 0.0f, 0.0f, 6, 0, 0.0f, NULL);
    AddTransitionToAllStates(table, tran);
    tran = xAnimTableNewTransition(table, "", "ScareD", FaceScare, NULL, 0, 3, 0.0f, 0.0f, 6, 0, 0.0f, NULL);
    AddTransitionToAllStates(table, tran);
    tran = xAnimTableNewTransition(table, "", "ScareE", FaceScare, NULL, 0, 4, 0.0f, 0.0f, 6, 0, 0.0f, NULL);
    AddTransitionToAllStates(table, tran);
    tran = xAnimTableNewTransition(table, "", "ScareF", FaceScare, NULL, 0, 5, 0.0f, 0.0f, 6, 0, 0.0f, NULL);
    AddTransitionToAllStates(table, tran);
    tran = xAnimTableNewTransition(table, "", "Mad", (xAnimTransitionConditionalCallback)FaceMad, (xAnimTransitionCallback)FaceClearScareCB, 0, 0, 0.0f, 0.0f, 5, 0, 0.0f, NULL);
    AddTransitionToAllStates(table, tran);
    tran = xAnimTableNewTransition(table, "", "Blow", (xAnimTransitionConditionalCallback)FaceBlow, (xAnimTransitionCallback)FaceClearScareCB, 0, 0, 0.0f, 0.0f, 5, 0, 0.0f, NULL);
    AddTransitionToAllStates(table, tran);
    tran = xAnimTableNewTransition(table, "", "Happy", FaceEvent, NULL, 0, 0, 0.0f, 0.0f, 4, 0, 0.0f, NULL);
    AddTransitionToAllStates(table, tran);
    tran = xAnimTableNewTransition(table, "", "Grin", FaceEvent, NULL, 0, 1, 0.0f, 0.0f, 4, 0, 0.0f, NULL);
    AddTransitionToAllStates(table, tran);
    tran = xAnimTableNewTransition(table, "", "Dizzy", FaceEvent, NULL, 0, 2, 0.0f, 0.0f, 4, 0, 0.0f, NULL);
    AddTransitionToAllStates(table, tran);
    tran = xAnimTableNewTransition(table, "", "Run", FaceRun, NULL, 0, 0, 0.0f, 0.0f, 3, 0, 0.0f, NULL);
    AddTransitionToAllStates(table, tran);
    tran = xAnimTableNewTransition(table, "", "Pant", FacePant, NULL, 0, 0, 0.0f, 0.0f, 3, 0, 0.0f, NULL);
    AddTransitionToAllStates(table, tran);
    tran = xAnimTableNewTransition(table, "", "Teeter", FaceTeeter, NULL, 0, 0, 0.0f, 0.0f, 3, 0, 0.0f, NULL);
    AddTransitionToAllStates(table, tran);
    tran = xAnimTableNewTransition(table, "", "Sneak", FaceSneak, NULL, 0, 0, 0.0f, 0.0f, 3, 0, 0.0f, NULL);
    AddTransitionToAllStates(table, tran);
    tran = xAnimTableNewTransition(table, "", "Swing", FaceSwing, NULL, 0, 0, 0.0f, 0.0f, 3, 0, 0.0f, NULL);
    AddTransitionToAllStates(table, tran);
    tran = xAnimTableNewTransition(table, "", "Idle", FaceIdle, NULL, 0, 0, 0.0f, 0.0f, 1, 0, 0.0f, NULL);
    AddTransitionToAllStates(table, tran);

    return table;
}

xAnimTable* zEntPlayer_TongueAnimTable()
{
    xAnimTable* table;

    table = xAnimTableNew("PlayerHead", NULL);
    xAnimTableNewState(table, "Idle", 0, 0, 1.0f, NULL, NULL, 0.0f, NULL);
    return table;
}

xAnimTable* zEntPlayer_UmbrellaAnimTable()
{
    xAnimTable* table;

    table = xAnimTableNew("PlayerUmbrella", NULL);
    xAnimTableNewState(table, "Idle", 0, 0, 1.0f, NULL, NULL, 0.0f, NULL);
    return table;
}

static U32 LampDrawCheck(xAnimTransition* tran, xAnimSingle* anim, void* param_3)
{
    return !(anim->Play->ModelInst->Flags & 0x1);
}

static U32 LamponCheck(xAnimTransition* tran, xAnimSingle* anim, void* param_3)
{
    if (anim->Play->ModelInst->Flags & 0x1)
    {
        return 0;
    }
    return !strcmp(globals.player.ent.model->Anim->Single->State->Name, "IdleToLampshade");
}

static U32 LampoffCheck(xAnimTransition* tran, xAnimSingle* anim, void* param_3)
{
    return !strcmp(globals.player.ent.model->Anim->Single->State->Name, "LampshadeToSneak");
}

static U32 LampbackCheck(xAnimTransition* tran, xAnimSingle* anim, void* param_3)
{
    return !strcmp(globals.player.ent.model->Anim->Single->State->Name, "SneakToLampshade");
}

static U32 LampFixbugCheck(xAnimTransition* tran, xAnimSingle* anim, void* param_3)
{
    return !strcmp(globals.player.ent.model->Anim->Single->State->Name, "Idle_lampshade");
}

xAnimTable* zEntPlayer_LampshadeAnimTable()
{
    xAnimTable* table;

    table = xAnimTableNew("PlayerLampshade", NULL);
    xAnimTableNewState(table, "Idle", 0, 0, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "TurnOff", 0, 0, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "TurnBackOn", 0x20, 0, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewTransition(table, "Idle TurnOff", "Idle", LamponCheck, NULL, 0, 0, 0.0f, 0.0f, 2, 0, 0.0f, NULL);
    xAnimTableNewTransition(table, "Idle", "TurnOff", LampoffCheck, NULL, 0, 0, 0.0f, 0.0f, 2, 0, 0.0f, NULL);
    xAnimTableNewTransition(table, "TurnOff Idle", "TurnBackOn", LampbackCheck, NULL, 0, 0, 0.0f, 0.0f, 2, 0, 0.0f, NULL);
    xAnimTableNewTransition(table, "TurnBackOn", "TurnOff", LampoffCheck, NULL, 0x20, 0, 0.0f, 0.0f, 2, 0, 0.0f, NULL);
    xAnimTableNewTransition(table, "TurnOff", "TurnBackOn", LampbackCheck, NULL, 0x20, 0, 0.0f, 0.0f, 2, 0, 0.0f, NULL);
    xAnimTableNewTransition(table, "TurnOff", "TurnBackOn", LampFixbugCheck, NULL, 0x20, 0, 0.0f, 0.0f, 2, 0, 0.0f, NULL);
    xAnimTableNewTransition(table, "TurnOff", "Idle", LampDrawCheck, NULL, 0, 0, 0.0f, 10.0f, 1, 0, 0.0f, NULL);
    xAnimTableNewTransition(table, "TurnBackOn", "Idle", NULL, NULL, 0x10, 0, 0.0f, 10.0f, 1, 0, 0.0f, NULL);
    return table;
}

static _zParEmitter* zEntPlayerFindSurfaceEmitter(S32 type, char* prefix)
{
    char name[128];
    char surface[128];
    _zParEmitter* emitter;

    strcpy(name, "PAREMIT_SD_");
    strcat(name, prefix);
    zSurfaceGetName(type, surface);
    strcat(name, surface);
    emitter = zParEmitterFind(xStrHash(name));
    if (emitter && (emitter->emit_flags & 1))
    {
        emitter->emit_flags ^= 1;
    }
    return emitter;
}

static void zEntPlayerFindEmitters()
{
    S32 i;

    memset(&gPlayerfx, 0, sizeof(_tagPlayerFX));
    for (i = 0; i < 23; i++)
    {
        gPlayerfx.emit_land[i] = zEntPlayerFindSurfaceEmitter(i, "LAND_");
        gPlayerfx.emit_stop[i] = zEntPlayerFindSurfaceEmitter(i, "STOP_");
    }
    gPlayerfx.emit_scratch = zParEmitterFind("PAREMIT_SD_FLEASCRATCH");
    gPlayerfx.emit_digup[0] = zParEmitterFind("PAREMIT_SD_DIGUPDUST");
    gPlayerfx.emit_digup[1] = zParEmitterFind("PAREMIT_SD_DIGUPDIRT");
    sEmitterSprings = zParEmitterFind("PAREMIT_SPRING_THING");
    sEmitterShockLeft = zParEmitterFind("PAREMIT_SD_SHOCKLEFT");
    sEmitterShockRight = zParEmitterFind("PAREMIT_SD_SHOCKRIGHT");
    sEmitterWaterSplash = zParEmitterFind("PAREMIT_SD_WATERSPLASH");
    sEmitterBounceMiss = zParEmitterFind("PAREMIT_SD_BOUNCEMISS");
    sEmitterSpringBoard = zParEmitterFind("PAREMIT_PLAT_SPRINGBOARD");
    sEmitterBounceOnVillain = zParEmitterFind("PAREMIT_SD_BOUNCEONVILLAIN");
    sEmitterHeadButt = zParEmitterFind("PAREMIT_SD_HEADBUTT");
    sEmitterHeadButt2 = zParEmitterFind("PAREMIT_SD_HEADBUTT2");
    sEmitterStars = zParEmitterFind("PAREMIT_STARS");
    sSoundWaterSplash = xStrHash("scooby_splash");
}

void zEntPlayer_Init(_xEnt* ent, p2EntAsset* asset)
{
    xFFXRotMatchState* rms;
    S32 i;

    zEntInit((_zEnt*)ent, asset, 'PLYR');
    ent->baseFlags |= 1;
    ent->flags |= 1;
    globals.player.Visible = 1;
    globals.player.AutoMoveSpeed = 0;
    ent->pflags &= 0xFB;
    ent->collis->chk &= 0xFE;
    ent->update = zEntPlayer_Update;
    ent->move = zEntPlayer_Move;
    ent->render = (void (*)(_xEnt*))zEntPlayer_Render;
    ent->eventFunc = zEntPlayerEventCB;
    if (ent->linkCount)
    {
        ent->link = (p2LinkAsset*)(asset + 1);
    }
    else
    {
        ent->link = NULL;
    }

    if (globals.sceneCur->baseCount[28])
    {
        xFFXAddEffect(ent, zGustUpdateEnt, &gust_data);
    }

    rms = xFFXRotMatchAlloc();
    if (rms)
    {
        rms->max_decl = globals.player.s.RotMatchMaxAngle;
        rms->tmatch = globals.player.s.RotMatchMatchTime;
        rms->trelax = globals.player.s.RotMatchRelaxTime;
        rms->tmr = 0.0f;
        xFFXAddEffect(ent, PlayerRotMatchUpdateEnt, rms);
    }

    zEntPlayerFindEmitters();

    zFeetInit(-1, ent, &sPlayerFeet[0], 0, ent->model->Anim->Table);
    zFeetInit(-1, ent, &sPlayerFeet[1], 1, ent->model->Anim->Table);
    zFeetInit(-1, ent, &sPlayerFeet[2], 2, ent->model->Anim->Table);
    zFeetInit(-1, ent, &sPlayerFeet[3], 3, ent->model->Anim->Table);
    zFeetInit(-1, ent, &sPlayerFeet[4], 4, ent->model->Anim->Table);
    zFeetInit(-1, ent, &sPlayerFeet[5], 5, ent->model->Anim->Table);
    zFeetInit(-1, ent, &sPlayerFeet[6], 6, ent->model->Anim->Table);
    zFeetInit(-1, ent, &sPlayerFeet[7], 7, ent->model->Anim->Table);
    zFeetInit(-1, ent, &sPlayerFeet[8], 8, ent->model->Anim->Table);
    zFeetInit(-1, ent, &sPlayerFeet[9], 9, ent->model->Anim->Table);

    zEntPlayer_FXSetup(ent, ent->model->Anim->Table);

    sSnackDecrement = 0;
    sDecrementSnackGateEnt = NULL;
    sDecrementDelay = 0.0f;
    sDecrementAmount = 2.0f;
    sVONextSituation = NULL;
    sVOTimer = 0.0f;

    for (i = 0; i < 15; i++)
    {
        sPlayerInfo[i].elapsedTime = sPlayerInfo[i].delay;
        sPlayerInfo[i].count = 0;
    }

    zEntPlayerReset(ent);
}

void zEntPlayer_Load(_xEnt* ent, xSerial* s)
{
}

static void zEntPlayer_SnackDecrement(F32 dt)
{
    U32 amount;

    if (sSnackDecrement)
    {
        zEntEvent(&globals.player.ent, 8);
        zHud_showSnack();
        sDecrementDelay -= dt;
        if (sDecrementDelay <= 0.0f)
        {
            sDecrementDelay = 0.0f;
            zEntPickup_hideSnackGateInfo(sDecrementSnackGateEnt);
            xSndPlay(sPlayerSndfx.snackcounter_id, 0.5f, 0.5f, 0.0f, 0x80, 2, 0);
            sDecrementAmount += 2.5f * dt;
            amount = sDecrementAmount;
            if (amount > sSnackDecrement)
            {
                amount = sSnackDecrement;
            }
            if ((S32)(globals.player.Inv_ScoobySnacks - amount) < 0)
            {
                globals.player.Inv_ScoobySnacks = 0;
                sSnackDecrement = 0;
            }
            else
            {
                globals.player.Inv_ScoobySnacks -= amount;
                sSnackDecrement -= amount;
            }
            if (!sSnackDecrement)
            {
                zEntEvent(&globals.player.ent, 9);
                zEntPickup_UnlockSnackGate((_xEnt*)sDecrementSnackGateEnt);
                zHud_showSnackSpecial();
                sDecrementSnackGateEnt = NULL;
            }
        }
    }
}

// Whether target lies within the cone of ent's facing direction (flat, in x and z) whose half-angle
// has the cosine cosang.
static inline S32 PlayerIsFacing(_xEnt* ent, _xEnt* target, F32 cosang)
{
    F32 ax = ent->frame->mat.at.x;
    F32 az = ent->frame->mat.at.z;
    F32 alen = sqrtf(ax * ax + az * az);
    F32 dx = target->frame->mat.pos.x - ent->model->Mat->pos.x;
    F32 dz = target->frame->mat.pos.z - ent->model->Mat->pos.z;
    F32 dlen = sqrtf(dx * dx + dz * dz);

    return ax * dx + az * dz > dlen * (cosang * alen);
}

// Per-frame player update: timers, health and death, movement in sub-steps with collision, Shaggy grabs,
// villain bounces and bumps, bubble platforms, then the floor, jump and drive updates, hanging and digging.
void zEntPlayer_Update(_xEnt* ent, _xScene* sc, F32 dt)
{
    xAnimState* astate;

    zEntPlayer_SnackDecrement(dt);

    if (!globals.player.ControlOn || globals.cmgr)
    {
        globals.player.ControlOffTimer = 1.0f;
    }

    globals.player.Sneak = 0;
    if ((globals.player.ControlOn && (globals.pad0->on & 0x2000)) ||
        (!globals.player.ControlOn && globals.player.AutoMoveSpeed == 3))
    {
        if (!surfSticky)
        {
            if (globals.player.Inv_Specials & 0x40)
            {
                globals.player.Sneak = 3;
                zLaughNotify(0);
            }
            else if (globals.player.Inv_Specials & 0x10)
            {
                globals.player.Sneak = ((globals.player.Inv_Specials >> 5) & 1) + 1;
                zLaughNotify(0);
            }
        }
    }

    if (globals.player.ScareShaggy)
    {
        globals.player.ScareShaggy--;
    }

    astate = globals.player.ent.model->Anim->Single->State;
    if (!strcmp(astate->Name, "SD0_Hold") || !strcmp(astate->Name, "SD0_HoldWalk") ||
        !strcmp(astate->Name, "SD1_Hold") || !strcmp(astate->Name, "SD1_HoldWalk"))
    {
        if ((globals.pad0->pressed & 0x30000) ||
            ((globals.pad0->pressed & 0xC0000) && !globals.player.cheat_mode))
        {
            if (!globals.player.Shaggy1.SetDown && !globals.player.Shaggy1.ThrowState)
            {
                zLaughNotify(14);
                if (astate->Name[2] == '1')
                {
                    Shaggy1_ChooseThrowTarget(globals.player.Shaggy1.Grab);
                }
                else
                {
                    globals.player.Shaggy1.ThrowState = 3;
                }
            }
        }
    }

    if (globals.player.Shaggy1.ThrowState == 2)
    {
        globals.player.Shaggy1.ThrowState = 3;
    }

    if (globals.player.ControlOn && !globals.pad0->on &&
        abs(globals.pad0->analog1.x) <= globals.player.s.AnalogMin &&
        abs(globals.pad0->analog1.y) <= globals.player.s.AnalogMin)
    {
        if (globals.player.ent.model->Anim->Single->State &&
            (globals.player.ent.model->Anim->Single->State->UserFlags & 1))
        {
            globals.player.IdleMinorTimer += dt;
            globals.player.IdleMajorTimer += dt;
            if (!surfSticky)
            {
                globals.player.IdleSitTimer += dt;
            }
            if (globals.player.IdleMinorTimer == 0.0f)
            {
                globals.player.IdleMinorTimer = 0.0001f;
            }
        }
    }
    else
    {
        globals.player.IdleMinorTimer = 0.0f;
        globals.player.IdleMajorTimer = -3.0f;
        globals.player.IdleSitTimer = 0.0f;
    }

    if (globals.player.Inv_Specials & 0x40)
    {
        globals.player.WorldDisguise = 6;
    }
    else
    {
        globals.player.WorldDisguise = 5;
    }

    if (globals.player.HeadbuttTimer)
    {
        zLaughNotify(1);
        globals.player.HeadbuttTimer += dt;
        if (!globals.player.ControlOn || globals.player.HeadbuttTimer > globals.player.s.HeadbuttTimeMax ||
            (globals.player.HeadbuttTimer > globals.player.s.HeadbuttTimeMin && !(globals.pad0->on & 0x80000)))
        {
            globals.player.HeadbuttTimer = 0.0f;
        }
        else
        {
            globals.player.HeadbuttVel += globals.player.s.HeadbuttAccel * dt;
            if (globals.player.HeadbuttVel > globals.player.s.HeadbuttSpeedMax)
            {
                globals.player.HeadbuttVel = globals.player.s.HeadbuttSpeedMax;
            }
        }
    }

    for (U32 i = 0; i < 37; i++)
    {
        sPlayerInfo[i].elapsedTime += dt;
    }

    if (sVOTimer || sVONextSituation)
    {
        if (sVOTimer > 0.0f)
        {
            sVOTimer -= dt;
            if (sVOTimer < 0.0f)
            {
                sVOTimer = 0.0f;
            }
        }
        if (sVOTimer == 0.0f && sVONextSituation)
        {
            zEntPlayer_SituationDo();
        }
    }

    if (globals.player.ScareTimer > 0.0f)
    {
        globals.player.ScareTimer -= dt;
        if (globals.player.ScareTimer < 0.0f)
        {
            globals.player.ScareTimer = 0.0f;
        }
    }

    if (globals.player.DamageTimer > 0.0f)
    {
        globals.player.DamageTimer -= dt;
        if (globals.player.DamageTimer < 0.0f)
        {
            globals.player.DamageTimer = 0.0f;
            globals.player.DamageSource = NULL;
        }
    }

    if (globals.player.ControlOffTimer > 0.0f)
    {
        globals.player.ControlOffTimer -= dt;
        if (globals.player.ControlOffTimer < 0.0f)
        {
            globals.player.ControlOffTimer = 0.0f;
        }
    }

    if (globals.player.DamageTimer && globals.player.Health)
    {
        if (++globals.player.Transparent == 4)
        {
            globals.player.Transparent = -2;
        }
    }
    else
    {
        globals.player.Transparent = 0;
    }

    if (globals.player.CowerTimer > 0.0f)
    {
        globals.player.CowerTimer -= dt;
        if (globals.player.CowerTimer < 0.0f)
        {
            globals.player.CowerTimer = 0.0f;
        }
    }

    if (globals.player.FallDeathTimer > 0.0f)
    {
        globals.player.FallDeathTimer -= dt;
        if (globals.player.FallDeathTimer < 0.0f)
        {
            globals.player.FallDeathTimer = 0.0f;
        }
    }

    if (globals.player.HelmetTimer > 0.0f)
    {
        globals.player.HelmetTimer -= dt;
        if (globals.player.HelmetTimer < 0.0f)
        {
            globals.player.HelmetTimer = 0.0f;
        }
    }

    PlayerFaceUpdate(dt);

    iColor_tag black = { 0, 0, 0, 255 };
    iColor_tag clear = { 0, 0, 0, 0 };

    if (!globals.player.Health)
    {
        if (globals.player.DamageTimer >= 0.3f && !xScrFxIsFading())
        {
            xScrFxFade(&clear, &black, 0.3f, NULL, 1, 1);
        }
        if (globals.player.DamageTimer == 0.0f)
        {
            zSceneGameStatsTally_death();
            zGameStateSwitch(2);
            gPlayerHitWater = 0;
            globals.player.Health = 3;
            sPlayerWaterOffset = 0.0f;
            gDeadScooby = 0;
        }
    }

    PlayerHangCheck(ent, sc);
    update_dt = dt;
    xEntBeginUpdate(ent, sc, dt);

    if (globals.player.ShockRadius)
    {
        globals.player.ShockRadius += 6.0f * dt;
        if (globals.player.ShockRadius > 4.0f)
        {
            globals.player.ShockRadius = 4.0f;
        }

        U32 i;
        F32 rad2 = globals.player.ShockRadius * globals.player.ShockRadius;
        F32 radold2 = globals.player.ShockRadiusOld * globals.player.ShockRadiusOld;
        F32 sdist2;
        F32 sdistold2;
        F32 ydist;
        _xEnt* vill;

        for (i = 0; i < sc->num_npcs; i++)
        {
            vill = sc->npcs[i];
            sdist2 = (vill->frame->mat.pos.x - ent->frame->mat.pos.x) *
                         (vill->frame->mat.pos.x - ent->frame->mat.pos.x) +
                     (vill->frame->mat.pos.z - ent->frame->mat.pos.z) *
                         (vill->frame->mat.pos.z - ent->frame->mat.pos.z);
            sdistold2 = (vill->frame->oldpos.x - ent->frame->mat.pos.x) *
                            (vill->frame->oldpos.x - ent->frame->mat.pos.x) +
                        (vill->frame->oldpos.z - ent->frame->mat.pos.z) *
                            (vill->frame->oldpos.z - ent->frame->mat.pos.z);
            ydist = fabs(vill->frame->mat.pos.y - ent->frame->mat.pos.y);
            if (((rad2 > sdist2 && radold2 <= sdist2) || (rad2 > sdistold2 && radold2 <= sdistold2)) &&
                ydist < 0.5f)
            {
                zEntVillain_Stun((st_zEntVillain*)vill, (4.0f - sqrtf(rad2)) / 3.4f);
            }
        }

        if (globals.player.ShockRadius == 4.0f)
        {
            globals.player.ShockRadius = 0.0f;
        }
        else
        {
            globals.player.ShockRadiusOld = globals.player.ShockRadius;
        }
    }

    zEntBubble_EnableCollision();

    S32 num_updates = 1;
    if (!globals.player.HangEnt)
    {
        if (dt > 0.1f)
        {
            num_updates = 6;
        }
        else if (dt > 1.0f / 12)
        {
            num_updates = 5;
        }
        else if (dt > 1.0f / 15)
        {
            num_updates = 4;
        }
        else if (dt > 0.05f)
        {
            num_updates = 3;
        }
        else if (dt > 1.0f / 30)
        {
            num_updates = 2;
        }
    }

    dt /= num_updates;

    for (S32 updidx = 0; updidx < num_updates; updidx++)
    {
        xVec3 suboldpos = ent->frame->mat.pos;
        xFFX* gust_fkt;
        F32 ndotm;
        F32 sett2;
        F32 dy;
        F32 s;
        F32 sft;
        F32 fg;
        F32 m;

        HeadcollUpdate(dt);
        xEntApplyPhysics(ent, sc, dt);
        xEntMove(ent, sc, dt);
        xVec3Copy(&req_motion, &ent->frame->dpos);
        xEntDriveUpdate(&globals.player.drv, sc, dt);

        if (globals.player.MountChimney && globals.player.MountChimney->eventFunc == Shaggy2Plat_EventCB)
        {
            globals.player.ent.frame->mat.pos.y +=
                globals.player.MountChimney->model->Mat->pos.y - globals.player.MountChimOldY;
        }

        if (globals.sceneCur->baseCount[28])
        {
            for (gust_fkt = ent->ffx; gust_fkt; gust_fkt = gust_fkt->next)
            {
                if (gust_fkt->doEffect == zGustUpdateEnt)
                {
                    if (globals.player.JumpState == 3 || surfSlickRatio)
                    {
                        xFFXTurnOn(gust_fkt);
                    }
                    else
                    {
                        xFFXTurnOff(gust_fkt);
                        if (gust_data.gust_on)
                        {
                            memset(&gust_data, 0, sizeof(zGustData));
                        }
                    }
                }
            }
        }
        else
        {
            gust_data.gust_on = 0;
        }

        xFFXApply(ent, sc, dt);

        xVec3 dpos;
        xVec3Sub(&dpos, &ent->frame->mat.pos, &suboldpos);

        if (!globals.player.cheat_mode && (globals.player.JumpState == 0 || globals.player.JumpState == 1) &&
            !globals.player.FallDeathTimer)
        {
            ent->frame->mat.pos.y -= 0.5f * dt;
            ndotm = globals.player.floor_norm.x * dpos.x + globals.player.floor_norm.z * dpos.z;
            if (ndotm > 0.0f && (0.0f != req_motion.x || 0.0f != req_motion.z || surfSlickRatio))
            {
                globals.player.slope = -1;
                m = sqrtf(ndotm);
                ent->frame->mat.pos.y -= m;
                sett2 = dpos.x * dpos.x + dpos.y * dpos.y + dpos.z * dpos.z;
                dpos.y = dpos.y - 0.5f * dt - m;
                s = sqrtf(sett2 / (dpos.x * dpos.x + dpos.y * dpos.y + dpos.z * dpos.z));
                xVec3 tmp;
                s = 0.25f * (s - 1.0f);
                tmp.x = s * dpos.x;
                tmp.y = s * dpos.y;
                tmp.z = s * dpos.z;
                xVec3Add(&ent->frame->mat.pos, &ent->frame->mat.pos, &tmp);
            }
            else if (ndotm < 0.0f)
            {
                globals.player.slope = 1;
            }
            else
            {
                globals.player.slope = 0;
            }
        }

        if (lastFloorEnt)
        {
            fg = globals.player.SlipFadeTimer;
            if (surfSlickRatio)
            {
                m = globals.player.floor_norm.x * globals.player.floor_norm.x +
                    globals.player.floor_norm.z * globals.player.floor_norm.z;
                ent->frame->mat.pos.x += 13.0f * dt * globals.player.floor_norm.x;
                ent->frame->mat.pos.y -= 13.0f * dt * sqrtf(m);
                ent->frame->mat.pos.z += 13.0f * dt * globals.player.floor_norm.z;
            }
            else if (fg > 0.0f)
            {
                sft = 30.0f * (fg / 0.8f * dt);
                ent->frame->mat.pos.x += sft * lastFloorNorm.x;
                ent->frame->mat.pos.z += sft * lastFloorNorm.z;
            }
        }

        zEntPlayerCollide(ent, sc, dt);
    }

    dt = update_dt;
    xVec3Sub(&update_motion, &ent->frame->mat.pos, &ent->frame->oldpos);

    if (!globals.player.cheat_mode)
    {
        _xCollis bbc;
        bbc.flags = 0;
        _xBox bbox = *(_xBox*)&globals.sceneCur->env->geom->world->boundingBox;
        bbox.upper.y += 10.0f;
        xSphereHitsBox(&ent->bound.sph, &bbox, &bbc);
        if (!(bbc.flags & 0x11))
        {
            zEntEvent(ent, 0x11);
        }
    }

    if (!globals.player.cheat_mode)
    {
        _xRay3 ctr;
        _xCollis ctc;
        _xCollis* c;
        _xCollis* cend;
        _xEnt* cent;

        xVec3Copy(&ctr.origin, &ent->frame->oldpos);
        ctr.origin.y += 0.7f;
        ctr.max_t = xVec3Normalize(&ctr.dir, &update_motion);
        ctr.min_t = 0.0f;
        ctr.flags = 0xC00;
        ctc.flags = 0;
        iRayHitsEnv(&ctr, globals.sceneCur->env, &ctc);
        if (ctc.flags & 1)
        {
            xVec3Copy(&ent->frame->mat.pos, &ent->frame->oldpos);
        }
        else
        {
            cend = &ent->collis->colls[ent->collis->idx];
            for (c = &ent->collis->colls[6]; c < cend; c++)
            {
                cent = (_xEnt*)c->optr;
                if (cent && cent &&
                    (cent->id == xStrHash("slipdeck01") || cent->id == xStrHash("slipdeck02") ||
                     cent->id == xStrHash("slipdeck03") || cent->id == xStrHash("slipdeck04")))
                {
                    iRayHitsModel(&ctr, cent->model, &ctc);
                    if (ctc.flags & 1)
                    {
                        xVec3Copy(&ent->frame->mat.pos, &ent->frame->oldpos);
                        break;
                    }
                }
            }
        }
    }

    {
        _xCollis* wcoll;
        _xCollis* gcoll;
        F32 hdotm;
        xSurface* wsurf;

        gcoll = &ent->collis->colls[6];
        for (wcoll = &ent->collis->colls[2]; wcoll < gcoll; wcoll++)
        {
            if (wcoll->flags & 1)
            {
                hdotm = wcoll->hdng.x * req_motion.x + wcoll->hdng.z * req_motion.z;
                wsurf = zSurfaceGetSurface(wcoll);
                if (hdotm > 0.0f && wcoll->hdng.y < 0.0f && zSurfaceGetStep(wsurf))
                {
                    ent->frame->mat.pos.y += 1.5f * dt;
                    ent->collis->colls[0] = *wcoll;
                    wcoll->flags = 0;
                }
            }
        }
    }

    zEntBubble_DisableCollision();
    EGeneratorCheck(ent, sc, dt);
    globals.player.HeadbuttHit = 0;
    ButtonCheck(ent);
    DestructObjCheck(ent);

    if (globals.player.Shaggy1.GrabOn && ent->collis->npc_eidx > ent->collis->npc_sidx &&
        (((_xEnt*)ent)->model->Anim->Single->State->UserFlags & 0x400))
    {
        _xCollis* coll;
        _xCollis* endcoll;
        _zEnt* shagent;

        coll = &ent->collis->colls[ent->collis->npc_sidx];
        endcoll = &ent->collis->colls[ent->collis->npc_eidx];
        for (; coll < endcoll; coll++)
        {
            shagent = (_zEnt*)coll->optr;
            if ((shagent->subType == 0x1C || shagent->subType == 0x1D) &&
                fabs(shagent->model->Mat->pos.y - ent->model->Mat->pos.y) < 0.5f &&
                PlayerIsFacing(ent, shagent, 0.86602f))
            {
                globals.player.Shaggy1.GrabTry = shagent;
                break;
            }
        }
    }

    if (ent->collis->npc_eidx > ent->collis->npc_sidx && globals.player.Shaggy1.Grab &&
        globals.player.Shaggy1.GrabOn && !zEntShaggy_GetSwap(globals.player.Shaggy1.Grab) &&
        globals.player.Shaggy1.Grab->subType == 0x1C)
    {
        _xCollis* coll;
        _xCollis* endcoll;
        _zEnt* shagent;

        shagent = globals.player.Shaggy1.Grab;
        coll = &ent->collis->colls[ent->collis->npc_sidx];
        endcoll = &ent->collis->colls[ent->collis->npc_eidx];
        for (; coll < endcoll; coll++)
        {
            if (coll->optr == shagent && coll->hdng.y < -0.7071f)
            {
                ent->frame->mat.pos.x += 0.001f;
                globals.player.Shaggy1.CatchYou = 1;
                globals.player.Shaggy1.CatchHeight = ent->frame->oldpos.y - shagent->frame->mat.pos.y;
                xVec3Copy(&ent->frame->mat.pos, &shagent->frame->mat.pos);
                xVec3Copy((xVec3*)&ent->model->Mat->pos, (xVec3*)&shagent->model->Mat->pos);
                break;
            }
        }
    }

    if (ent->collis->npc_eidx > ent->collis->npc_sidx)
    {
        _xCollis* coll;
        _xCollis* endcoll;
        S32 bounceResponse;
        en_ZVILLAIN_BUMPED_RESPONSE bumpedResponse;
        S32 took_damage;
        U32 type;

        coll = &ent->collis->colls[ent->collis->npc_sidx];
        endcoll = &ent->collis->colls[ent->collis->npc_eidx];
        type = ent->model->Anim->Single->State->UserFlags & 0x1E;
        if (type == 0xC)
        {
            HeadbuttTest(coll, endcoll, 0.7071f, VillHBCallback);
        }
        else if (globals.player.Health &&
                 (type == 0xE ||
                  (type == 0xA && ent->frame->mat.pos.y < ent->frame->oldpos.y &&
                   (((_xEnt*)coll->optr)->subType == 0x16 || ((_xEnt*)coll->optr)->subType == 0x18 ||
                    ((_xEnt*)coll->optr)->subType == 0 || ((_xEnt*)coll->optr)->subType == 0x15 ||
                    ((_xEnt*)coll->optr)->subType == 0x24))))
        {
            for (; coll < endcoll; coll++)
            {
                if (coll->hdng.y < -0.5f)
                {
                    bounceResponse = 0;
                    if (((_xEnt*)coll->optr)->subType == 0x24)
                    {
                        if (Shaggy8_BBCallback((_zEnt*)ent, coll))
                        {
                            bounceResponse = 2;
                        }
                    }
                    else
                    {
                        bounceResponse = zEntVillain_Bounce((st_zEntVillain*)coll->optr);
                    }

                    if (bounceResponse == 1 || bounceResponse == 2)
                    {
                        ent->frame->mat.pos.x = ((xMat4x3*)ent->model->Mat)->pos.x;
                        ent->frame->mat.pos.y = ((xMat4x3*)ent->model->Mat)->pos.y;
                        ent->frame->mat.pos.z = ((xMat4x3*)ent->model->Mat)->pos.z;
                        startBounce = ent->frame->mat.pos.y;
                        zEntPlayerJumpStart(ent, bounceResponse == 1 ? &globals.player.s.Bounce
                                                                     : &globals.player.s.Spring);
                        if (((_xEnt*)coll->optr)->subType == 0x16 || ((_xEnt*)coll->optr)->subType == 0x18 ||
                            ((_xEnt*)coll->optr)->subType == 0x15 || ((_xEnt*)coll->optr)->subType == 0)
                        {
                            globals.player.Bounced = 1;
                        }
                        if (((_xEnt*)coll->optr)->subType == 0x24)
                        {
                            globals.player.Bounced = 2;
                        }
                        globals.player.Jump_CanDouble = 1;
                        globals.player.Jump_CanFloat = 1;
                        zRumbleStart(0, SDR_ButtbounceJump);
                        zEntPlayer_SNDButtBounce(ent, 1);
                        break;
                    }
                    else if (bounceResponse == 4 && 0.0f == coll->hdng.x && 0.0f == coll->hdng.z)
                    {
                        ent->frame->vel.y = 0.0f;
                    }
                }
            }
        }
        else
        {
            took_damage = 0;
            for (; coll < endcoll; coll++)
            {
                bumpedResponse = zEntVillain_Bumped((st_zEntVillain*)coll->optr, coll);
                switch (bumpedResponse)
                {
                case ZVIL_BUMPED_GUM:
                    if (coll->hdng.y < -0.5f)
                    {
                        startBounce = ent->frame->mat.pos.y;
                        zEntPlayerJumpStart(ent, &globals.player.s.Spring);
                        globals.player.Bounced = 2;
                        globals.player.Jump_CanDouble = 1;
                        globals.player.Jump_CanFloat = 1;
                        zRumbleStart(0, SDR_ButtbounceJump);
                    }
                    break;
                case ZVIL_BUMPED_DAMAGE:
                    if (zEntPlayer_Damage((xBase*)coll->optr, 1) ? 1 : 0)
                    {
                        zRumbleStart(0, SDR_VillainCollideScare);
                    }
                    took_damage = 1;
                    break;
                case ZVIL_BUMPED_ATTACK:
                    if (zEntPlayer_Damage((xBase*)coll->optr, 1) ? 1 : 0)
                    {
                        zRumbleStart(0, SDR_VillainCollideScare);
                    }
                    took_damage = 1;
                    break;
                }
                if (took_damage)
                {
                    break;
                }
            }
        }
    }

    if (globals.player.Health && ent->collis->dyn_eidx > ent->collis->dyn_sidx)
    {
        _xCollis* coll;
        _xCollis* endcoll;
        _zEntBubble* bub;

        coll = &ent->collis->colls[ent->collis->dyn_sidx];
        endcoll = &ent->collis->colls[ent->collis->dyn_eidx];
        for (; coll < endcoll; coll++)
        {
            bub = (_zEntBubble*)coll->optr;
            if (!bub)
            {
                continue;
            }
            if (bub->baseType == 0xF)
            {
                if (coll->hdng.y < -0.866025f)
                {
                    if (bub->subType == 0)
                    {
                        startBounce = ent->frame->mat.pos.y;
                        zEntPlayerJumpStart(ent, &globals.player.s.Spring);
                        globals.player.Jump_CanDouble = 1;
                        globals.player.Jump_CanFloat = 1;
                    }
                    else if (bub->subType == 1 && bub->villain &&
                             (ent->model->Anim->Single->State->UserFlags & 0x1E) == 0xE)
                    {
                        zEntVillain_Bounce((st_zEntVillain*)bub->villain);
                    }
                    break;
                }
                if (bub->subType == 1 && bub->villain &&
                    (ent->model->Anim->Single->State->UserFlags & 0x1E) == 0xC)
                {
                    zEntVillain_Headbutt((st_zEntVillain*)bub->villain, &coll->hdng);
                }
            }
        }
    }

    zEntPlayerFloorUpdate(ent, sc, dt);
    PlayerTeeterCheck(ent, sc, dt);
    zEntPlayerSurfDamageUpdate(ent, sc, dt);
    zEntPlayerDriveUpdate(ent, sc, dt);
    zEntPlayerJumpUpdate(ent, sc, dt);
    zEntPlayerCollTrigger(ent, sc);

    if (globals.player.RootUp.y != globals.player.RootUpTarget.y || globals.player.RootUp.y != 1.0f)
    {
        F32 mag;
        F32 lerpspeed;

        mag = (globals.player.RootUpTarget.x - globals.player.RootUp.x) *
                  (globals.player.RootUpTarget.x - globals.player.RootUp.x) +
              (globals.player.RootUpTarget.y - globals.player.RootUp.y) *
                  (globals.player.RootUpTarget.y - globals.player.RootUp.y) +
              (globals.player.RootUpTarget.z - globals.player.RootUp.z) *
                  (globals.player.RootUpTarget.z - globals.player.RootUp.z);
        if (globals.player.HangElapsed <= 0.25f)
        {
            lerpspeed = 0.07f;
        }
        else if (globals.player.HangElapsed >= 0.5f)
        {
            lerpspeed = 0.5f;
        }
        else
        {
            lerpspeed = 1.72f * (globals.player.HangElapsed - 0.25f);
        }

        if (mag < lerpspeed * lerpspeed)
        {
            xVec3Copy(&globals.player.RootUp, &globals.player.RootUpTarget);
        }
        else
        {
            mag = 1.0f / sqrtf(mag) * lerpspeed;
            globals.player.RootUp.x += (globals.player.RootUpTarget.x - globals.player.RootUp.x) * mag;
            globals.player.RootUp.y += (globals.player.RootUpTarget.y - globals.player.RootUp.y) * mag;
            globals.player.RootUp.z += (globals.player.RootUpTarget.z - globals.player.RootUp.z) * mag;
            xVec3Normalize(&globals.player.RootUp, &globals.player.RootUp);
        }

        if (isnan(globals.player.RootUp.y) || isnan(globals.player.RootUpTarget.y))
        {
            globals.player.RootUp.x = 0.0f;
            globals.player.RootUp.y = 1.0f;
            globals.player.RootUp.z = 0.0f;
            globals.player.RootUpTarget.x = 0.0f;
            globals.player.RootUpTarget.y = 1.0f;
            globals.player.RootUpTarget.z = 0.0f;
            globals.player.HangElapsed = 0.0f;
        }
    }

    zEntPlayerVelUpdate(ent, sc, dt);
    xEntEndUpdate(ent, sc, dt);

    if (globals.player.HangEnt && globals.player.HangEnt != globals.player.HangEntLast)
    {
        xVec3Copy(&vHangLast, (xVec3*)&globals.player.ent.model->Mat->pos);
        ent->frame->vel.x = 0.0f;
        ent->frame->vel.y = 0.0f;
        ent->frame->vel.z = 0.0f;
    }

    if (globals.player.HangEntLast != globals.player.HangEnt)
    {
        if (globals.player.HangEnt)
        {
            zEntEvent(globals.player.HangEnt, 0x20);
            if (globals.player.HangEnt->follow)
            {
                zEntEvent(globals.player.HangEnt->follow, 0x20);
            }
        }
        if (globals.player.HangEntLast)
        {
            zEntEvent(globals.player.HangEntLast, 0x21);
            if (globals.player.HangEntLast->follow)
            {
                zEntEvent(globals.player.HangEntLast->follow, 0x21);
            }
        }
    }
    globals.player.HangEntLast = globals.player.HangEnt;

    xVec3Sub(&lastDeltaPos, &ent->frame->mat.pos, &ent->frame->oldpos);

    if (surfSlickRatio)
    {
        xVec3Copy(&lastFloorNorm, &globals.player.floor_norm);
        lastFloorEnt = (_xEnt*)ent->collis->colls[0].optr;
        if (lastFloorEnt && lastFloorEnt->id != xStrHash("slipdeck01") &&
            lastFloorEnt->id != xStrHash("slipdeck02") && lastFloorEnt->id != xStrHash("slipdeck03") &&
            lastFloorEnt->id != xStrHash("slipdeck04"))
        {
            lastFloorEnt = NULL;
        }
    }

    if (globals.player.DigTimer)
    {
        PlayerDigUpdate(ent, dt);
    }
    else
    {
        U32 i;
        zScene* zsc;
        _zEntSimpleObj* simpent;
        F32 parm[4];

        globals.player.Diggable = NULL;
        zsc = (zScene*)sc;
        for (i = 0; i < zsc->num_stats; i++)
        {
            if (zsc->stats[i] && (zsc->stats[i]->baseFlags & 1) && xEntIsVisible(zsc->stats[i]))
            {
                simpent = (_zEntSimpleObj*)zsc->stats[i];
                if (simpent->sflags & 1)
                {
                    if (xVec3Dist2(&globals.player.ent.bound.sph.center, (xVec3*)&simpent->model->Mat->pos) <
                        (1.25f * globals.player.ent.bound.sph.r) * (1.25f * globals.player.ent.bound.sph.r))
                    {
                        globals.player.Diggable = simpent;
                        if (!(simpent->sflags & 2))
                        {
                            zEntPlayer_Notify(0);
                            parm[0] = 2.0f;
                            zEntEvent(simpent, 0xC3, parm);
                            simpent->sflags |= 2;
                        }
                    }
                    else if (simpent->sflags & 2)
                    {
                        simpent->sflags ^= 2;
                    }
                }
            }
        }

        if (!globals.player.Diggable && globals.player.JumpState == 0 && globals.player.ControlOn &&
            (globals.pad0->pressed & 0x20000))
        {
            Shaggy8_CallToPlayer();
        }
    }

    if (globals.player.Shaggy1.Grab)
    {
        Shaggy1_PlayerUpdate(globals.player.Shaggy1.Grab, sc, dt);
    }

    if (globals.player.JumpState == 0)
    {
        xCameraTargetOnGround(1, ent->frame->mat.pos.y);
    }
    else
    {
        xCameraTargetOnGround(0, 0.0f);
    }

    zEntPlayer_FXUpdate(ent, dt);

    if (!globals.player.ControlOn && globals.player.ControlOnEvent)
    {
        globals.player.ControlOn = 1;
        globals.player.ControlOnEvent = 0;
    }
}

// Linear interpolation from a to b by t, written into o.
static inline void LerpVec3(xVec3* o, const xVec3* a, const xVec3* b, F32 t)
{
    o->x = a->x + (b->x - a->x) * t;
    o->y = a->y + (b->y - a->y) * t;
    o->z = a->z + (b->z - a->z) * t;
}

void zEntPlayer_Render(_zEnt* ent)
{
    if (!globals.player.Visible)
    {
        return;
    }

    F32 rootZadjust = 0.0f;
    RwMatrix rootOldMat = *ent->model->Mat;
    xAnimSingle* asingle = ent->model->Anim->Single;
    xAnimSingle* ablend = asingle->Blend;
    if (!strcmp(asingle->State->Name, "SD0_SetDown") || !strcmp(asingle->State->Name, "SD1_SetDown"))
    {
        if (asingle->Time >= 0.85f)
        {
            rootZadjust = -0.6f;
        }
        else if (asingle->Time > 0.65f)
        {
            rootZadjust = -0.6f * (asingle->Time - 0.65f) / (0.85f - 0.65f);
        }
    }
    else if (ablend->State &&
             (!strcmp(ablend->State->Name, "SD0_SetDown") || !strcmp(ablend->State->Name, "SD1_SetDown")))
    {
        rootZadjust = -0.6f * (1.0f - asingle->BlendFactor * asingle->Tran->BlendRecip);
    }

    if (rootZadjust)
    {
        xVec3 rootZvec = { 0.0f, 0.0f, rootZadjust };

        xMat3x3RMulVec(&rootZvec, (xMat3x3*)ent->model->Mat, &rootZvec);
        ent->model->Mat->pos.x += rootZvec.x;
        ent->model->Mat->pos.z += rootZvec.z;
    }

    if (1.0f != globals.player.RootUp.y)
    {
        xMat4x3 tmpMat;

        tmpMat.up = globals.player.RootUp;
        xVec3Cross(&tmpMat.right, &tmpMat.up, (xVec3*)&ent->model->Mat->at);
        xVec3Normalize(&tmpMat.right, &tmpMat.right);
        xVec3Cross(&tmpMat.at, &tmpMat.right, &tmpMat.up);
        tmpMat.pos.x = 0.0f;
        if (globals.player.HangEnt)
        {
            tmpMat.pos.y = -ent->bound.sph.r;
        }
        else
        {
            tmpMat.pos.y = 0.0f;
        }
        tmpMat.pos.z = 0.0f;
        xMat3x3RMulVec(&tmpMat.pos, (xMat3x3*)&tmpMat, &tmpMat.pos);
        tmpMat.pos.x += ent->model->Mat->pos.x;
        if (globals.player.HangEnt)
        {
            tmpMat.pos.y += ent->model->Mat->pos.y + ent->bound.sph.r;
        }
        else
        {
            tmpMat.pos.y += ent->model->Mat->pos.y;
        }
        tmpMat.pos.z += ent->model->Mat->pos.z;
        if (globals.player.HangEnt && globals.player.HangStartLerp < 1.0f)
        {
            tmpMat.pos.x = globals.player.HangStartPos.x +
                           globals.player.HangStartLerp * (tmpMat.pos.x - globals.player.HangStartPos.x);
            tmpMat.pos.y = globals.player.HangStartPos.y +
                           globals.player.HangStartLerp * (tmpMat.pos.y - globals.player.HangStartPos.y);
            tmpMat.pos.z = globals.player.HangStartPos.z +
                           globals.player.HangStartLerp * (tmpMat.pos.z - globals.player.HangStartPos.z);
        }
        tmpMat.flags = 0;
        *(xMat4x3*)ent->model->Mat = tmpMat;
        rendermat = tmpMat;
    }
    else
    {
        rendermat = *(xMat4x3*)ent->model->Mat;
    }

    if (1.0f != globals.player.RootUp.y || rootZadjust)
    {
        for (xModelInstance* minst = ent->model->Next; minst; minst = minst->Next)
        {
            *(xMat4x3*)minst->Mat = *(xMat4x3*)ent->model->Mat;
        }
    }

    {
        xModelInstance* minst = ent->model;
        U32 index = 0;
        xAnimSingle* asingle = ent->model->Anim->Single;
        xVec3 pos0;
        xVec3 pos1;
        while (minst)
        {
            switch (index)
            {
            case 2:
                if (((asingle->State->UserFlags & 0x1E) == 0xC && (globals.player.Inv_Specials & 0x800)) ||
                    !strcmp(asingle->State->Name, "Powerup_helmet"))
                {
                    globals.player.HelmetTimer = 0.3f;
                    if (minst->Flags & 2)
                    {
                        minst->Flags |= 1;
                    }
                    else
                    {
                        minst->Flags |= 2;
                    }
                }
                else if (0.0f == globals.player.HelmetTimer)
                {
                    minst->Flags &= ~3;
                }
                break;
            case 3:
                if ((asingle->State->UserFlags & 0x1E) == 2 && globals.player.WorldDisguise != 6)
                {
                    if (minst->Flags & 2)
                    {
                        minst->Flags |= 1;
                    }
                    else
                    {
                        minst->Flags |= 2;
                    }
                }
                else
                {
                    minst->Flags &= ~3;
                }
                break;
            case 4:
                if (!xStricmp(asingle->State->Name, "Action_dig"))
                {
                    if (minst->Flags & 2)
                    {
                        minst->Flags |= 1;
                    }
                    else
                    {
                        minst->Flags |= 2;
                    }
                }
                else
                {
                    minst->Flags &= ~3;
                }
                break;
            case 5:
                if (globals.player.WorldDisguise == 5 &&
                    (!xStricmp(asingle->State->Name, "Idle_lampshade") ||
                     !xStricmp(asingle->State->Name, "Idle_blackknight") ||
                     !xStricmp(asingle->State->Name, "IdleToLampshade") ||
                     !xStricmp(asingle->State->Name, "LampshadeToSneak") ||
                     !xStricmp(asingle->State->Name, "SneakToLampshade") ||
                     !xStricmp(asingle->State->Name, "Powerup_lamp")))
                {
                    if (minst->BoneCount <= 10)
                    {
                        minst->Flags &= (U16)~8;
                        minst->Flags |= 0x20;
                        minst->BoneIndex = 0;
                    }
                    if (minst->Anim)
                    {
                        minst->Flags |= 4;
                    }
                    if (minst->Flags & 2)
                    {
                        minst->Flags |= 1;
                    }
                    else
                    {
                        minst->Flags |= 2;
                    }
                }
                else
                {
                    minst->Flags &= ~7;
                }
                break;
            case 6:
                if (globals.player.WorldDisguise == 6 &&
                    (!xStricmp(asingle->State->Name, "Idle_lampshade") ||
                     !xStricmp(asingle->State->Name, "Idle_blackknight") ||
                     !xStricmp(asingle->State->Name, "Walk_blackknight")))
                {
                    if (minst->Flags & 2)
                    {
                        ent->model->Flags &= ~1;
                        minst->Flags |= 1;
                    }
                    else
                    {
                        minst->Flags |= 2;
                    }
                }
                else
                {
                    ent->model->Flags |= 1;
                    minst->Flags &= ~7;
                }
                break;
            case 7:
                if (surfSticky && (globals.player.Inv_Specials & 0x4))
                {
                    if (minst->Flags & 2)
                    {
                        minst->Flags |= 1;
                    }
                    else
                    {
                        minst->Flags |= 2;
                    }
                }
                else
                {
                    minst->Flags &= ~7;
                }
                break;
            case 8:
                if (!strcmp(asingle->State->Name, "Powerup_plunger") ||
                    (globals.player.Slide == 2 &&
                     (!strcmp(asingle->State->Name, "Land_normal") || !strcmp(asingle->State->Name, "Land_walk") ||
                      !strcmp(asingle->State->Name, "Land_walkidle") ||
                      !strcmp(asingle->State->Name, "Land_running") ||
                      !strcmp(asingle->State->Name, "Walk_plunger") ||
                      !strcmp(asingle->State->Name, "Idle_plunger"))))
                {
                    if (minst->Flags & 2)
                    {
                        minst->Flags |= 1;
                    }
                    else
                    {
                        minst->Flags |= 2;
                    }
                }
                else
                {
                    minst->Flags &= ~3;
                }
                break;
            case 9:
                if (!xStricmp(asingle->State->Name, "Fall_floatup") ||
                    !xStricmp(asingle->State->Name, "Fall_floatdown") ||
                    !xStricmp(asingle->State->Name, "Fall_floatopen"))
                {
                    if (!(minst->Flags & 1) && minst->Anim)
                    {
                        minst->Anim->Single->Time = 0.0f;
                        minst->Anim->Single->CurrentSpeed = minst->Anim->Single->State->Speed;
                    }
                    minst->Flags |= 1;
                    memcpy(minst->Mat, ent->model->Mat, sizeof(xMat3x3));
                    iModelTagEval(ent->model->Data, &globals.player.HangPawTag[0], ent->model->Mat, &pos0);
                    iModelTagEval(ent->model->Data, &globals.player.HangPawTag[1], ent->model->Mat, &pos1);
                    LerpVec3((xVec3*)&minst->Mat->pos, &pos0, &pos1, 0.5f);
                    xMat3x3RMulRotY((xMat3x3*)minst->Mat, (xMat3x3*)minst->Mat, umbrellaFudgeRot);
                    minst->Mat->pos.y += umbrellaFudgeY;
                    minst->Mat->pos.x += umbrellaFudgeX * minst->Mat->right.x;
                    minst->Mat->pos.z += umbrellaFudgeX * minst->Mat->right.z;
                }
                else
                {
                    minst->Flags &= ~1;
                }
                break;
            case 10:
                minst->Flags &= (U16)~8;
                minst->Flags |= 0x20;
                minst->Flags &= ~1;
                break;
            case 11:
                if (!strcmp(asingle->State->Name, "Inactive_Magnify") && asingle->Time > 0.55f &&
                    asingle->Time < 221.0f / 30.0f)
                {
                    if (minst->Flags & 2)
                    {
                        minst->Flags |= 1;
                    }
                    else
                    {
                        minst->Flags |= 2;
                    }
                }
                else
                {
                    minst->Flags &= ~3;
                }
                break;
            }
            minst = minst->Next;
            index++;
        }
    }

    S32 player_trans = 80;
    if (globals.player.s.TakeDamage && gPlayerHitWater)
    {
        globals.player.Transparent = 1;
        player_trans = 255.0f - 0.5f * (255.0f * sPlayerWaterOffset);
        if (player_trans < 0)
        {
            player_trans = 0;
        }
        sPlayerWaterOffset += 2.0f * sTimeElapsed;
        for (xModelInstance* minst = globals.player.ent.model; minst; minst = minst->Next)
        {
            if (minst->Flags & 1)
            {
                globals.player.ent.frame->mat.pos.x = minst->Mat->pos.x = gPlayerWaterPos.x;
                globals.player.ent.frame->mat.pos.y = minst->Mat->pos.y = gPlayerWaterPos.y - sPlayerWaterOffset;
                globals.player.ent.frame->mat.pos.z = minst->Mat->pos.z = gPlayerWaterPos.z;
            }
        }
    }

    if (globals.player.Transparent > 0)
    {
        if (!iModelCull(ent->model->Data, ent->model->Mat))
        {
            zLightAddLocal(ent);
            xShadowRender(ent);
            iDrawSetFBMSK(-1);
            xModelRender(ent->model);
            iDrawSetFBMSK(0);
            for (xModelInstance* minst = globals.player.ent.model; minst; minst = minst->Next)
            {
                if (minst->Flags & 1)
                {
                    iModelSetMaterialAlpha(minst->Data, player_trans);
                    iModelRender(minst->Data, minst->Mat);
                    iModelSetMaterialAlpha(minst->Data, 255);
                }
            }
            zLightRemoveLocal();
        }
    }
    else
    {
        xEntRender(&globals.player.ent);
    }

    *ent->model->Mat = rootOldMat;
}

void zEntPlayer_Move(_xEnt* ent, _xScene* sc, F32 dt, _xEntFrame* frame)
{
    S32 px;
    S32 pz;

    frame->mode = 0;
    frame->mode &= ~0x400;
    frame->mode &= ~0x800;
    if (globals.player.Health)
    {
        px = -globals.pad0->analog1.x;
        pz = -globals.pad0->analog1.y;
        if (globals.pad0->on & 0x80)
        {
            px = globals.player.s.AnalogMax;
        }
        else if (globals.pad0->on & 0x20)
        {
            px = -globals.player.s.AnalogMax;
        }
        if (globals.pad0->on & 0x10)
        {
            pz = globals.player.s.AnalogMax;
        }
        else if (globals.pad0->on & 0x40)
        {
            pz = -globals.player.s.AnalogMax;
        }
        PlayerAbsControl(ent, px, pz, dt);
        if (globals.player.ControlOn)
        {
            xCameraSetController(&globals.camera, globals.pad0->analog1.x,
                                 globals.pad0->analog1.y, dt);
        }
    }
}

S32 zEntPlayer_Damage(xBase* src, U32 damage, xVec3* knockback)
{
    if (!zEntPlayer_Damage(src, damage))
    {
        return 0;
    }
    if (knockback)
    {
        globals.player.ent.frame->vel.x = knockback->x;
        globals.player.ent.frame->vel.y = knockback->y;
        globals.player.ent.frame->vel.z = knockback->z;
    }
    return 1;
}

S32 zEntPlayer_Damage(xBase* src, U32 damage)
{
    if (globals.player.Shaggy1.Grab &&
        (!strcmp(globals.player.Shaggy1.Grab->model->Anim->Single->State->Name, "CatchJump") ||
         !strcmp(globals.player.Shaggy1.Grab->model->Anim->Single->State->Name, "CatchThrow") ||
         !strcmp(globals.player.Shaggy1.Grab->model->Anim->Single->State->Name, "SetDown") ||
         !strcmp(globals.player.Shaggy1.Grab->model->Anim->Single->State->Name, "SD1_SetDown")))
    {
        return 0;
    }
    if (globals.player.DamageTimer > 0.0f || globals.player.ControlOffTimer > 0.0f)
    {
        return 0;
    }

    globals.player.ScareSource = src;
    if (damage > globals.player.Health)
    {
        damage = globals.player.Health;
    }
    if (globals.player.s.TakeDamage)
    {
        globals.player.Health -= damage;
    }
    zEntVillain_HeroWasHurt();
    globals.player.ScareTimer = 0.35f;
    if (globals.player.Health)
    {
        globals.player.Face_ScareTimer = 3.0f;
        globals.player.Face_ScareRandom = (irand() >> 16) % 6;
    }
    globals.player.ScareShaggy = 2;
    if (!globals.player.Health)
    {
        globals.player.DamageTimer = 10.0f;
        if (globals.player.ent.driver)
        {
            globals.player.ent.driver = NULL;
            globals.player.ent.frame->vel = g_O3;
            globals.player.ent.frame->oldvel = g_O3;
        }
        zEntEventAll(NULL, 0, 0x33, NULL);
        if (globals.player.s.TakeDamage)
        {
            gDeadScooby = 1;
            gDeadScoobyPos = *(xVec3*)&globals.player.ent.model->Mat->pos;
        }
    }
    else
    {
        globals.player.DamageTimer = 2.0f;
    }
    if (globals.player.HangEnt)
    {
        globals.player.HangEnt->grabTimer = globals.player.HangEnt->hangInfo->grabDelay;
        globals.player.HangEnt = NULL;
        globals.player.RootUpTarget.x = 0.0f;
        globals.player.RootUpTarget.y = 1.0f;
        globals.player.RootUpTarget.z = 0.0f;
        globals.player.HangElapsed = 0.0f;
    }
    zSceneGameStatsTally_scare();
    return 1;
}

U32 zEntPlayer_MoveInfo()
{
    U32 infoflags = 0;
    _xEnt* plyr = &globals.player.ent;
    U32 animflags;

    animflags = plyr->model->Anim->Single->State->UserFlags & 0x1E;
    if (!animflags || plyr->model->Anim->Single->State->UserFlags & 1)
    {
        infoflags |= 0x1;
    }
    if (animflags == 0x4)
    {
        infoflags |= 0x2;
    }
    if (animflags == 0x6 || animflags == 0x8)
    {
        infoflags |= 0x4;
    }
    if (animflags == 0xE)
    {
        infoflags |= 0x8;
    }
    if (animflags == 0xC)
    {
        infoflags |= 0x10;
    }
    if (!strcmp(plyr->model->Anim->Single->State->Name, "Idle_lampshade"))
    {
        infoflags |= 0x800;
    }
    else if (!strcmp(plyr->model->Anim->Single->State->Name, "Walk_sneak"))
    {
        infoflags |= 0x400;
    }
    else if (!strcmp(plyr->model->Anim->Single->State->Name, "Idle_blackknight"))
    {
        infoflags |= 0x1000;
    }
    else if (!strcmp(plyr->model->Anim->Single->State->Name, "Walk_blackknight"))
    {
        infoflags |= 0x1000;
    }
    if (animflags == 0x2)
    {
        infoflags |= 0x100;
    }
    if (infoflags & 0x1800)
    {
        infoflags |= 0x200;
    }
    return infoflags;
}

void zEntPlayer_GiveScoobySnack(U32 quantity)
{
    globals.player.Inv_ScoobySnacks += quantity;
    zSceneGameStatsTally_snack(quantity);
    if (zEntPickup_lastSnackInArea())
    {
        zHudShowAllSnacksArea();
    }
}

S32 zEntPlayer_HasScoobySnack(U32 quantity)
{
    return globals.player.Inv_ScoobySnacks >= quantity;
}

void zEntPlayer_TakeScoobySnack(_xEnt* ent, U32 quantity)
{
    sDecrementSnackGateEnt = (_zEntPickup*)ent;
    sDecrementDelay = 1.1f;
    sDecrementAmount = 2.0f;
    sSnackDecrement += quantity;
}

void zEntPlayer_GiveClue(U32 quantity)
{
    globals.player.Inv_Clue += quantity;
}

void zEntPlayer_GiveKey(U32 quantity)
{
    globals.player.Inv_Key += quantity;
    if (globals.player.Inv_Key > 99)
    {
        globals.player.Inv_Key = 99;
    }
}

void zEntPlayer_GiveChance(U32 quantity)
{
    globals.player.Inv_Chances += quantity;
    if (globals.player.Inv_Chances > 99)
    {
        globals.player.Inv_Chances = 99;
    }
}

void zEntPlayer_GiveHealth(U32 quantity)
{
    globals.player.Health += quantity;
    if (globals.player.Health > 5)
    {
        globals.player.Health = 5;
    }
}

void zEntPlayer_GiveHotsauce(U32 value)
{
    if (value == 0)
    {
        globals.player.HotsauceTimer = 10.0f;
    }
    else
    {
        globals.player.HotsauceTimer = value;
    }
}

// Launch speed for a jump that peaks at height h. Gravity starts at j, blends over Tgs seconds into g
// once Tgc seconds have passed, so the rise is up to three phases: constant j, the blend (a cubic in time)
// and constant g. Solves for the time the peak falls in each phase and returns the matching speed.
static F32 CalcJumpImpulse_Smooth(F32 g, F32 j, F32 h, F32 Tgc, F32 Tgs)
{
    F32 Tmfound;
    F32 Tf;
    F32 Tm[3];
    F32 Ts;
    F32 Kc;
    F32 AF;
    F32 D;
    F32 Kbf;
    F32 B;
    F32 A;
    F32 AS;
    U32 solcnt;
    U32 i;

    AS = -j / 2.0f;
    A = (j - g) / (6.0f * Tgs);
    B = (g * Tgc - j * Tgc - j * Tgs) / (2.0f * Tgs);
    Kc = -(j * Tgc + 3.0f * A * SQ(Tgc) + 2.0f * B * Tgc);
    D = AS * SQ(Tgc) - A * (Tgc * SQ(Tgc)) - B * SQ(Tgc) - Kc * Tgc;
    AF = -g / 2.0f;
    Kbf = 3.0f * A * SQ(Tgc + Tgs) + 2.0f * B * (Tgc + Tgs) + g * (Tgc + Tgs);
    Tmfound = 0.0f;

    Tf = D + (A * ((Tgc + Tgs) * SQ(Tgc + Tgs)) + B * SQ(Tgc + Tgs)) - AF * SQ(Tgc + Tgs) - Kbf * (Tgc + Tgs);
    Ts = sqrtf((Tmfound - h) / AS);
    Tf = sqrtf((Tf - h) / AF);
    solcnt = xMathSolveCubic(-2.0f * A, -B, 0.0f, D - h, &Tm[0], &Tm[1], &Tm[2]);

    if (Ts <= Tgc && Ts >= 0.0f)
    {
        return -Tmfound - 2.0f * AS * Ts;
    }

    if (Tf >= Tgc + Tgs)
    {
        return -(Kbf + Kc) - 2.0f * AF * Tf;
    }

    Tmfound = -1.0f;
    for (i = 0; i < solcnt; i++)
    {
        if (Tm[i] >= Tgc && Tm[i] <= Tgc + Tgs)
        {
            if (Tmfound < 0.0f || Tm[i] < Tmfound)
            {
                Tmfound = Tm[i];
            }
        }
    }

    if (Tmfound != -1.0f)
    {
        return -Kc - 2.0f * B * Tmfound - 3.0f * A * SQ(Tmfound);
    }

    return 1.0f;
}

void CalcJumpImpulse(zJumpParam* param)
{
    param->ImpulseVel = CalcJumpImpulse_Smooth(globals.player.s.Gravity, globals.player.s.JumpGravity,
                                               param->PeakHeight, param->TimeGravChange,
                                               globals.player.s.GravSmooth);
}

void zEntPlayerJumpStart(_xEnt* ent, zJumpParam* jump)
{
    globals.player.Jump_CurrGravity = globals.player.s.JumpGravity;
    globals.player.Jump_HoldTimer = jump->TimeHold;
    globals.player.Jump_ChangeTimer = jump->TimeGravChange;
    if (ent->frame->vel.y > 0.0f && globals.player.JumpState == 2)
    {
        ent->frame->vel.y = jump->ImpulseVel;
    }
    else
    {
        ent->frame->vel.y = jump->ImpulseVel;
    }
    globals.player.JumpState = 2;
    if ((ent->model->Anim->Single->State->UserFlags & 0x1E) == 0xE ||
        ent->model->Anim->Single->State->UserFlags & 0x100)
    {
        globals.player.Bounced = 1;
    }
}

// Butt-smash landing: a burst of bounce-miss particles where the floor was hit, a rumble and the miss sound.
static inline void PlayerButtSmashLandFX(_xEnt* ent, _xCollis* cfloor)
{
    if (sEmitterBounceMiss)
    {
        _zParEmitterCustomSettings info;
        S32 i;

        info.custom_flags = 0x20;
        info.pos.x = ((xMat4x3*)ent->model->Mat)->pos.x + cfloor->dist * cfloor->tohit.x;
        info.pos.y = ((xMat4x3*)ent->model->Mat)->pos.y + cfloor->dist * cfloor->tohit.y;
        info.pos.z = ((xMat4x3*)ent->model->Mat)->pos.z + cfloor->dist * cfloor->tohit.z;
        for (i = 0; i < 25; i++)
        {
            zParEmitterEmitCustom(sEmitterBounceMiss, 1.0f, &info);
        }
    }
    zRumbleStart(0, SDR_Buttbounce);
    zEntPlayer_SNDButtBounce(ent, 0);
}

// Springboard launch: a burst of particles at the springboard's centre.
static inline void PlayerSpringboardFX(_zPlatform* plat)
{
    if (sEmitterSpringBoard)
    {
        _zParEmitterCustomSettings info;
        S32 i;

        info.custom_flags = 0x20;
        info.pos = *xBoundCenter(&plat->bound);
        for (i = 0; i < 25; i++)
        {
            zParEmitterEmitCustom(sEmitterSpringBoard, 1.0f / 30.0f, &info);
        }
    }
}

// Jump state machine: hold and gravity-change timers, landing (and the butt-smash landing), rising and falling,
// springboard launches, then gravity and the float special.
void zEntPlayerJumpUpdate(_xEnt* ent, _xScene* sc, F32 dt)
{
    _xCollis* cfloor;
    F32 lerp;

    globals.player.LastJumpState = globals.player.JumpState;
    if (globals.player.JumpState == 4)
    {
        return;
    }

    if (globals.player.JumpState == 5)
    {
        ent->frame->vel.y = -globals.player.s.ButtsmashSpeed;
    }

    if (globals.player.Jump_HoldTimer)
    {
        globals.player.Jump_HoldTimer -= dt;
        if (globals.player.Jump_HoldTimer <= 0.0f)
        {
            globals.player.Jump_HoldTimer = 0.0f;
        }
        else if (!globals.player.ControlOn || !(globals.pad0->on & 0x10000))
        {
            globals.player.Jump_HoldTimer = 0.0f;
            globals.player.Jump_ChangeTimer = 0.0f;
            globals.player.Jump_CurrGravity = globals.player.s.Gravity;
        }
    }

    if (globals.player.Jump_ChangeTimer)
    {
        globals.player.Jump_ChangeTimer -= dt;
        if (0.0f == globals.player.Jump_ChangeTimer)
        {
            globals.player.Jump_ChangeTimer = 0.0000001f;
        }
        if (globals.player.Jump_ChangeTimer <= -globals.player.s.GravSmooth)
        {
            globals.player.Jump_HoldTimer = 0.0f;
            globals.player.Jump_ChangeTimer = 0.0f;
            globals.player.Jump_CurrGravity = globals.player.s.Gravity;
        }
        else if (globals.player.Jump_ChangeTimer < 0.0f)
        {
            lerp = -globals.player.Jump_ChangeTimer / globals.player.s.GravSmooth;
            globals.player.Jump_CurrGravity =
                (1.0f - lerp) * globals.player.s.JumpGravity + lerp * globals.player.s.Gravity;
        }
    }

    cfloor = &ent->collis->colls[0];
    if (cfloor->flags & 1 && !(cfloor->optr && ((xBase*)cfloor->optr)->baseType == 2) &&
        ent->frame->vel.y <= 0.0f)
    {
        if (globals.player.JumpState != 0)
        {
            if (globals.player.JumpState == 5)
            {
                PlayerButtSmashLandFX(ent, cfloor);
            }
            globals.player.JumpState = 0;
            globals.player.Jump_CanDouble = 1;
            globals.player.Jump_CanFloat = 1;
            globals.player.Jump_Springboard = NULL;
            globals.player.Jump_SpringboardStart = 0;
        }
    }
    else
    {
        if (globals.player.JumpState == 0)
        {
            globals.player.JumpState = 1;
            globals.player.JumpTimer = 0.0f;
        }
        else if (globals.player.JumpState == 1)
        {
            globals.player.JumpTimer += dt;
            if (globals.player.JumpTimer >= 0.25f)
            {
                globals.player.JumpState = 2;
            }
        }
        else if (globals.player.JumpState == 2)
        {
            if (ent->frame->vel.y < 16.0f && globals.player.Jump_SpringboardStart)
            {
                PlayerSpringboardFX(globals.player.Jump_Springboard);
                globals.player.Jump_SpringboardStart = 0;
                if (0.0f == globals.player.Jump_Springboard->passet->sb.jmpdir.x &&
                    0.0f == globals.player.Jump_Springboard->passet->sb.jmpdir.z)
                {
                    globals.player.Jump_CanDouble = 1;
                }
            }
        }

        if (globals.player.JumpState == 3)
        {
            ent->frame->vel.x *= 0.96f;
            ent->frame->vel.z *= 0.96f;
        }
    }

    if (globals.player.Jump_CurrGravity)
    {
        if (globals.player.JumpState == 2 || globals.player.JumpState == 1)
        {
            ent->frame->vel.y -= globals.player.Jump_CurrGravity * dt;
        }
        if (globals.player.JumpState == 2 || globals.player.JumpState == 3)
        {
            if (globals.player.Inv_Specials & 0x1000 && globals.player.ControlOn &&
                globals.pad0->on & 0x10000 && globals.player.Jump_CanFloat && !globals.player.Jump_CanDouble &&
                (ent->frame->vel.y <= -globals.player.s.FloatSpeed || gust_data.gust_on))
            {
                zLaughNotify(4);
                globals.player.JumpState = 3;
            }
            if (globals.player.JumpState == 3)
            {
                ent->frame->vel.y = -globals.player.s.FloatSpeed;
                if (!globals.player.ControlOn || !(globals.pad0->on & 0x10000))
                {
                    globals.player.JumpState = 2;
                }
            }
        }
    }
}

// Flags the player as touching an electric arc when its bound sphere comes near the line between an active
// generator's two ends, and damages it.
static void EGeneratorCheck(_xEnt* ent, _xScene* sc, F32 dt)
{
    zScene* zsc = (zScene*)sc;
    S32 i;
    zEGenerator* egen;
    _xIsect isect;

    globals.player.earc_coll.flags &= ~1;
    for (i = 0; i < (S32)zsc->baseCount[41]; i++)
    {
        egen = &((zEGenerator*)zsc->baseList[41])[i];
        if (egen->flags & 1)
        {
            xLine3VecDist2(&egen->src_pos, &egen->dst_pos, &ent->bound.sph.center, &isect);
            if (isect.dist < ent->bound.sph.r * ent->bound.sph.r + 0.1f * (2.0f * ent->bound.sph.r) + 0.1f * 0.1f)
            {
                globals.player.earc_coll.flags |= 1;
                if (zEntPlayer_Damage(egen, 1) ? 1 : 0)
                {
                    globals.player.DamageTimer = 5.0f;
                }
            }
        }
    }
}

void zEntPlayerVelUpdate(_xEnt* ent, _xScene* sc, F32 dt)
{
    xVec3* v;
    F32 velen2;
    _xCollis* colls;
    _xCollis* coll;
    S32 i;
    F32 s;

    if (ent->model->Anim->Single->State->UserFlags & 0x100)
    {
        return;
    }

    v = &ent->frame->vel;

    if (!(globals.player.SlideTimer > 0.0f) && !surfSlickRatio &&
        !(globals.player.SlipFadeTimer > 0.0f))
    {
        if (globals.player.JumpState == 0)
        {
            v->x = 0.0f;
            v->y = 0.0f;
            v->z = 0.0f;
        }
        return;
    }

    if (!globals.player.Slide && globals.player.SlideTimer > 0.0f)
    {
        F32 sft = 2.15f - globals.player.SlideTimer;
        if (globals.player.JumpState == 0 || globals.player.JumpState == 1)
        {
            if (globals.player.JumpState == 0)
            {
                v->y = 0.0f;
            }
            if (sft < 0.4f)
            {
                sft = 0.4f - sft;
                s = sft / (sft + dt);
                v->x *= s;
                v->z *= s;
            }
            else
            {
                v->x = 0.0f;
                v->z = 0.0f;
                return;
            }
        }
        else
        {
            if (sft < 2.15f)
            {
                sft = 2.15f - sft;
                s = sft / (sft + dt);
                v->x *= s;
                v->z *= s;
            }
            else
            {
                v->x = 0.0f;
                v->z = 0.0f;
                return;
            }
        }
    }

    velen2 = xVec3Length2(v);
    if ((F32)fabs(velen2) < 0.0001f)
    {
        v->x = 0.0f;
        v->y = 0.0f;
        v->z = 0.0f;
        return;
    }

    i = 0;
    colls = ent->collis->colls;
    coll = colls;
    if (surfSlickRatio)
    {
        if (coll->optr)
        {
            if (ent->frame->mat.pos.y - ent->frame->oldpos.y > 0.0f)
            {
                v->x *= 0.97f;
                v->z *= 0.97f;
            }
        }
        else
        {
            v->y = 0.0f;
        }
        coll = &colls[2];
        i = 2;
    }
    else if (!(globals.player.SlideTimer > 0.0f))
    {
        if (globals.player.SlipFadeTimer > 0.0f)
        {
            s = globals.player.SlipFadeTimer / (globals.player.SlipFadeTimer + dt);
            v->x *= s;
            v->z *= s;
        }
        else
        {
            v->x = 0.0f;
            v->z = 0.0f;
            return;
        }
    }

    for (; i < 6; i++, coll++)
    {
        if (!(coll->flags & 1) || !(coll->dist < 0.7f))
        {
            continue;
        }
        if (coll->optr && ((xBase*)coll->optr)->baseType == 2)
        {
            continue;
        }
        if (xVec3Dot(v, &coll->hdng) < 0.0f)
        {
            continue;
        }
        F32 rft = xVec3Dot(v, &coll->norm);
        if (rft > 0.0f)
        {
            continue;
        }
        if (-rft >= 0.965926f * velen2)
        {
            v->x = 0.0f;
            v->y = 0.0f;
            v->z = 0.0f;
            return;
        }
        xVec3 dv;
        xVec3SMul(&dv, &coll->norm, 0.5f * -rft);
        v->x += dv.x;
        v->y += dv.y;
        v->z += dv.z;
        velen2 = xVec3Length2(v);
    }
}

// Floor update: read the surface under the player (stickiness, friction, slickness and the skid slow-down on slick
// floors), fit the floor normal from four rays cast down around the player, and start, hold or end a slide down a
// steep slide surface.
static inline void PlayerFloorNormal(xVec3* o, const xVec3* p)
{
    xVec3 a;
    xVec3 b;
    xVec3 c;
    xVec3 n1;
    xVec3 n2;

    xVec3Sub(&a, &p[1], &p[0]);
    xVec3Sub(&b, &p[2], &p[0]);
    xVec3Sub(&c, &p[3], &p[0]);
    xVec3Cross(&n1, &a, &b);
    xVec3Cross(&n2, &b, &c);
    xVec3Add(o, &n1, &n2);
}

static inline void xMat4x3Toworld(xVec3* o, const xMat4x3* m, const xVec3* v)
{
    xMat3x3RMulVec(o, (const xMat3x3*)m, v);
    xVec3Add(o, o, &m->pos);
}

void zEntPlayerFloorUpdate(_xEnt* p, _xScene* sc, F32 dt)
{
    _xCollis* fcoll;
    xSurface* fsurf;
    xVec3* vel;
    F32 velmag;
    F32 decelmag;
    F32 slickLerp;
    xVec3* fnorm;
    _xRay3 ray;
    _xCollis rcoll;
    _xEnt* fent;
    S32 i;
    F32 dave;
    F32 dmin;
    F32 dmax;
    xVec3 D;
    xVec3 X;
    _xMat3x3 N_to_Up;
    xVec3 D_flat;
    F32 ddotm;
    F32 dz;
    F32 s;
    static xVec3 lorigins[4] = {
        { -0.3f, 0.0f, 0.3f },
        { 0.3f, 0.0f, 0.3f },
        { 0.3f, 0.0f, -0.3f },
        { -0.3f, 0.0f, -0.3f },
    };

    fcoll = &p->collis->colls[0];
    fsurf = zSurfaceGetSurface(fcoll);
    globals.player.floor_surf = fsurf;

    surfSlickTimer -= dt;
    if (surfSlickTimer < 0.0f)
    {
        surfSlickTimer = 0.0f;
    }

    surfSticky = 0;
    surfSlickRatio = 0.0f;

    if (globals.player.CanSlip)
    {
        if ((globals.player.JumpState == 0 || globals.player.JumpState == 1) && fsurf && !fsurf->state)
        {
            surfSticky = zSurfaceGetSticky(fsurf);
        }

        if (fsurf && !fsurf->state)
        {
            surfFriction = zSurfaceGetFriction(fsurf);
            surfSlickness = zSurfaceGetSlickness(fsurf);
            surfDamping = zSurfaceGetDamping(fsurf, minVelmag);
            surfSlipTimer = 0.00001f;
        }
        else
        {
            surfFriction = 1.0f;
            surfDamping = minVelmag;
            if (globals.player.JumpState)
            {
                surfSlipTimer = 0.0f;
            }
            if (surfSlipTimer)
            {
                surfSlipTimer += dt;
                if (surfSlipTimer >= 0.75f)
                {
                    surfSlipTimer = 0.0f;
                }
            }
            if (!surfSlipTimer)
            {
                surfSlickness = 1;
            }
        }

        if (surfSlickness != 1)
        {
            if (surfSlickness > 1)
            {
                surfSlickRatio = 150.0f / surfSlickness;
            }
            else
            {
                surfSlickRatio = 1.0f;
            }
            surfSlickTimer = 0.1f;
        }
        else
        {
            surfSlickRatio = 0.0f;
        }

        vel = &p->frame->vel;
        if (lastSlickness != surfSlickness)
        {
            if (surfSlickness == 1)
            {
                if (lastFloorEnt)
                {
                    globals.player.SlipFadeTimer = 0.8f;
                }
                else
                {
                    vel->x = 0.0f;
                    vel->z = 0.0f;
                }
            }
            else
            {
                s = 1.0f / dt;
                dz = lastDeltaPos.z * s;
                vel->x = lastDeltaPos.x * s;
                vel->z = dz;
            }
        }

        if (surfSlickRatio)
        {
            velmag = vel->x * vel->x + vel->z * vel->z;
            if (surfMaxSpeed)
            {
                if (velmag > surfMaxSpeed * surfMaxSpeed)
                {
                    s = surfMaxSpeed / sqrtf(velmag);
                    vel->x *= s;
                    vel->z *= s;
                }
            }
            else
            {
                if (xStricmp(p->model->Anim->Single->State->Name, "Skid"))
                {
                    decelmag = surfDecelIdle * dt;
                }
                else
                {
                    decelmag = surfDecelSkid * dt;
                }
                slickLerp = (4.0f / 3.0f) * surfSlipTimer;
                decelmag = (1.0f - slickLerp) * decelmag + 20.0f * slickLerp * decelmag;
                velmag = sqrtf(velmag);
                if (velmag <= decelmag)
                {
                    vel->x = 0.0f;
                    vel->z = 0.0f;
                }
                else
                {
                    s = (velmag - decelmag) / velmag;
                    vel->x *= s;
                    vel->z *= s;
                }
            }
        }
        else if (globals.player.SlipFadeTimer > 0.0f)
        {
            globals.player.SlipFadeTimer -= dt;
            if (globals.player.SlipFadeTimer < 0.0f)
            {
                globals.player.SlipFadeTimer = 0.0f;
            }
        }

        lastSlickness = surfSlickness;
    }

    fnorm = &fcoll->norm;
    if (fcoll->flags & 1)
    {
        xVec3Copy(&globals.player.floor_norm, fnorm);
    }
    else
    {
        xVec3Copy(&globals.player.floor_norm, &g_Y3);
        xVec3Copy(&ray.origin, xBoundCenter(&p->bound));
        ray.dir.x = 0.0f;
        ray.dir.y = -1.0f;
        ray.dir.z = 0.0f;
        ray.max_t = 10.0f;
        ray.flags = 0x800;
        rcoll.flags = 0;
        rcoll.optr = NULL;
        rcoll.dist = 1e19f;
        xRayHitsScene(globals.sceneCur, &ray, &rcoll);
        fcoll->dist = rcoll.dist;
    }

    ray.dir.x = 0.0f;
    ray.dir.y = -1.0f;
    ray.dir.z = 0.0f;
    ray.min_t = -0.7f;
    ray.max_t = 1.0f;
    ray.flags = 0xC00;
    dave = 0.0f;
    fent = (_xEnt*)fcoll->optr;
    for (i = 0; i < 4; i++)
    {
        xMat4x3Toworld(&ray.origin, &p->frame->mat, &lorigins[i]);
        rcoll.flags = 0;
        rcoll.optr = NULL;
        rcoll.dist = 1e19f;
        if (fent)
        {
            if (fent->collLev == 5)
            {
                iRayHitsModel(&ray, fent->model, &rcoll);
            }
            else
            {
                xRayHitsBound(&ray, &fent->bound, &rcoll);
            }
        }
        else
        {
            iRayHitsEnv(&ray, globals.sceneCur->env, &rcoll);
        }
        xVec3Copy(&floor_supp[i], &ray.origin);
        floor_dist[i] = rcoll.dist;
        if (rcoll.flags & 1)
        {
            dave += rcoll.dist;
        }
    }

    dave *= 0.25f;
    dmin = dave - 0.17320508f;
    dmax = 0.17320508f + dave;
    for (i = 0; i < 4; i++)
    {
        floor_supp[i].y -= MAX(dmin, MIN(floor_dist[i], dmax));
    }

    PlayerFloorNormal(&globals.player.floor_norm, floor_supp);
    xVec3Normalize(&globals.player.floor_norm, &globals.player.floor_norm);

    if (globals.player.Slide == 1 || globals.player.Slide == 2)
    {
        vel = &p->frame->vel;
        if (((fcoll->flags & 1) &&
             (fnorm->y > (F32)cos(surfSlideStop) || (fsurf && (fsurf->state || !zSurfaceGetSlide(fsurf))))) ||
            vel->y > 0.0f || surfSlickRatio != 0.0f)
        {
            if (!(globals.player.Inv_Specials & 8))
            {
                p->pflags &= (U8)~0x80;
                globals.player.SlideTimer = 2.15f;
            }
            if (surfSlickRatio)
            {
                globals.player.SlideTimer = 0.0f;
            }
            globals.player.Slide = 0;
        }
    }
    else if ((fcoll->flags & 1) && fnorm->y < (F32)cos(zSurfaceGetSlideStartAngle(fsurf)) && fsurf &&
             !fsurf->state && zSurfaceGetSlide(fsurf))
    {
        if ((globals.player.Inv_Specials & 8) &&
            !(fcoll->optr && ((_xEnt*)fcoll->optr)->baseType == 6 && ((_xEnt*)fcoll->optr)->subType == 0xB))
        {
            globals.player.Slide = 2;
        }
        else
        {
            D.x = -fnorm->z;
            D.y = 0.0f;
            D.z = fnorm->x;
            xVec3Normalize(&D, &D);
            xVec3Cross(&X, fnorm, &D);
            xVec3Normalize(&X, &X);
            xMat3x3RotC(&N_to_Up, D.x, D.y, D.z, acos(fnorm->y));
            xMat3x3RMulVec(&D_flat, &N_to_Up, &X);
            ddotm = D_flat.x * req_motion.x + D_flat.z * req_motion.z;
            if (ddotm < 0.0f && (globals.player.LastJumpState == 0 || globals.player.LastJumpState == 1))
            {
                globals.player.Slide = 3;
                s = ddotm / sqrtf(D_flat.x * D_flat.x + D_flat.z * D_flat.z);
                dz = D_flat.x * s;
                s *= D_flat.z;
                p->frame->mat.pos.x -= dz;
                p->frame->mat.pos.z -= s;
                if (update_motion.y > 0.0f)
                {
                    p->frame->mat.pos.y = p->frame->oldpos.y;
                }
            }
            else
            {
                globals.player.Slide = 1;
                surfSlideStart = zSurfaceGetSlideStartAngle(fsurf);
                surfSlideStop = zSurfaceGetSlideStopAngle(fsurf);
                p->pflags |= 0x80;
                s = 0.5f / update_dt;
                p->frame->vel.x += req_motion.x * s;
                p->frame->vel.z += req_motion.z * s;
                p->frame->vel.y = -4.0f;
            }
            globals.player.SlideTimer = 0.0f;
        }
    }
    else
    {
        globals.player.Slide = 0;
    }

    if (globals.player.Slide == 1)
    {
        globals.player.SlideTimer += dt;
    }
    else if (globals.player.Slide == 0 && globals.player.SlideTimer > 0.0f)
    {
        globals.player.SlideTimer -= dt;
        if (globals.player.SlideTimer < 0.0f)
        {
            globals.player.SlideTimer = 0.0f;
        }
    }
}

static U32 PlayerDepenQuery(_xEnt* ent, _xEnt* other, _xScene* sc, F32 dt, _xCollis* coll)
{
    zSurfaceProps* prop;
    xSurface* surf;

    if (globals.player.DamageTimer > 0.0f)
    {
        surf = zSurfaceGetSurface(coll);
        if (surf && !surf->state)
        {
            prop = (zSurfaceProps*)surf->moprops;
            if (prop && prop->asset)
            {
                switch (prop->asset->game_damage_type)
                {
                case 0:
                    return 1;
                case 1:
                case 2:
                case 3:
                case 4:
                case 5:
                case 6:
                    if (prop->asset->game_damage_flags & 1)
                    {
                        return 0;
                    }
                    break;
                }
            }
        }
    }
    return 1;
}

static void PlayerBoundUpdate(_xEnt* ent, xVec3* pos)
{
    xVec3 dpos;

    xEntDefaultBoundUpdate(ent, pos);

    if (headcoll_anim == 4 || headcoll_anim == 5 || headcoll_anim == 6 || headcoll_anim == 7 ||
        headcoll_anim == 8)
    {
        xVec3Sub(&dpos, pos, (xVec3*)&ent->model->Mat->pos);
        iModelTagEval(globals.player.ent.model->Next->Data, &globals.player.head_tag,
                      globals.player.ent.model->Next->Mat, &globals.player.head_sph.center);
        xVec3Add(&globals.player.head_sph.center, &globals.player.head_sph.center, &dpos);
        xVec3Sub(&dpos, xBoundCenter(&ent->bound), &globals.player.head_sph.center);
        xVec3Normalize(&dpos, &dpos);
        xVec3SMul(&dpos, &dpos, 0.4f);
        xVec3Add(&globals.player.head_sph.center, &globals.player.head_sph.center, &dpos);
        globals.player.head_sph.r = 0.7f;
        if (globals.player.head_sph.center.y < ent->bound.sph.center.y)
        {
            globals.player.head_sph.center.y = ent->bound.sph.center.y;
        }
    }
    else
    {
        xMat3x3RMulVec(&dpos, (xMat3x3*)&rendermat, &headcoll_os);
        xVec3Add(&globals.player.head_sph.center, xBoundCenter(&ent->bound), &dpos);
        globals.player.head_sph.r = 0.7f;
    }
}

void zEntPlayerSurfDamageUpdate(_xEnt* p, _xScene* sc, F32 dt)
{
    _xCollis* coll;
    _xCollis* cend;
    xSurface* surf;
    S32 damaged;
    zSurfaceProps* prop;
    _xEnt* cent;

    if (globals.player.DamageTimer > 0.0f)
    {
        return;
    }

    coll = &p->collis->colls[0];
    cend = &coll[p->collis->idx];
    for (; coll < cend; coll++)
    {
        if (!(coll->flags & 1))
        {
            continue;
        }

        surf = zSurfaceGetSurface(coll);
        if (!surf || surf->state)
        {
            continue;
        }

        prop = (zSurfaceProps*)surf->moprops;
        damaged = 0;
        if (!prop || !prop->asset)
        {
            continue;
        }

        switch (prop->asset->game_damage_type)
        {
        case 4:
        case 6:
            if (zEntPlayer_Damage(surf, 1) ? 1 : 0)
            {
                globals.player.DamageTimer = 5.0f;
            }
            zRumbleStart(0, SDR_GotScared);
            damaged = 1;
            break;
        case 1:
            cent = (_xEnt*)coll->optr;
            if (cent && cent->baseType == 0x29 && !(((zEGenerator*)cent)->flags & 1))
            {
                break;
            }
        case 2:
        case 3:
        case 5:
            if (globals.player.Health)
            {
                globals.player.DamageTimer = 0.0f;
                zEntPlayer_Damage(surf, globals.player.Health);
                damaged = 1;
            }
            break;
        }

        if (damaged)
        {
            if (coll->optr)
            {
                globals.player.DamageSource = (_zEnt*)coll->optr;
            }
            return;
        }
    }
}

// A springboard launch: the jump start with the impulse already worked out from the springboard's heights.
static inline void PlayerSpringboardJump(_xEnt* ent, F32 impulse)
{
    startBounce = ent->frame->mat.pos.y;
    globals.player.Jump_CurrGravity = globals.player.s.JumpGravity;
    globals.player.Jump_HoldTimer = 0.0f;
    globals.player.Jump_ChangeTimer = 0.3f;
    if (ent->frame->vel.y > 0.0f && globals.player.JumpState == 2)
    {
        ent->frame->vel.y = impulse;
    }
    else
    {
        ent->frame->vel.y = impulse;
    }
    globals.player.JumpState = 2;
    if ((ent->model->Anim->Single->State->UserFlags & 0x1E) == 0xE ||
        ent->model->Anim->Single->State->UserFlags & 0x100)
    {
        globals.player.Bounced = 1;
    }
}

// Riding: let go of the platform the player stepped off, follow the platform under the feet (mounting a new one,
// and remembering a chimney top), and launch the player off a springboard.
void zEntPlayerDriveUpdate(_xEnt* p, _xScene* sc, F32 dt)
{
    _xEntDrive* drv;
    _zPlatform* oplat;
    _xCollis* coll;
    _zPlatform* plat;
    xSurface* surf;
    F32* jmphs;
    xVec3* jmpdir;
    xAnimPlay* aplay;
    xAnimSingle* asing;

    drv = &globals.player.drv;

    if (drv->odriver && !drv->os)
    {
        plat = (_zPlatform*)drv->odriver;
        zEntEvent(plat, 0x21);
        drv->odriver = NULL;
        drv->otm = 0.0f;
        if (plat->subType == 9)
        {
            zPlatform_Dismount(plat);
        }
        if (plat->passet->flags & 1)
        {
            zPlatform_Shake(plat, dt, 0.2f, 37.699112f);
        }
    }

    coll = &p->collis->colls[0];
    if (coll->flags & 1)
    {
        plat = (_zPlatform*)coll->optr;
    }
    else
    {
        plat = NULL;
    }

    if (plat && (plat->eventFunc == Shaggy2_EventCB || plat->eventFunc == Shaggy2Plat_EventCB))
    {
        globals.player.MountChimney = plat;
        globals.player.MountChimOldY = plat->model->Mat->pos.y;
    }
    else
    {
        globals.player.MountChimney = NULL;
    }

    if (!plat)
    {
        if (drv->driver)
        {
            if (globals.player.JumpState != 0 && globals.player.JumpState != 1)
            {
                xEntDriveDismount(drv, 1.0f);
            }
            else
            {
                xEntDriveDismount(drv, 0.3f);
            }
        }
        return;
    }

    surf = zSurfaceGetSurface(coll);
    if (surf && !surf->state && zSurfaceGetDamageType(surf))
    {
        return;
    }

    if (plat != (_zPlatform*)drv->driver)
    {
        if (drv->driver)
        {
            xEntDriveDismount(drv, 0.3f);
        }

        if (plat->baseType != 6)
        {
            return;
        }

        if (plat->eventFunc == Shaggy2_EventCB || plat->eventFunc == Shaggy2Plat_EventCB)
        {
            zEntEvent(plat, 0x20);
            return;
        }

        oplat = plat;

        if (!(plat->plat_flags & 1))
        {
            return;
        }

        if (!gPlayerHitWater && plat != (_zPlatform*)drv->odriver)
        {
            zEntEvent(plat, 0x20);
        }

        if (oplat->subType != 10)
        {
            if (oplat->subType == 11)
            {
                xEntDriveMount(drv, oplat, 0.5f);
            }
            else
            {
                xEntDriveMount(drv, oplat, 0.15f);
            }

            if (oplat->subType == 9)
            {
                zPlatform_Mount(oplat);
            }

            if (oplat->passet->flags & 1)
            {
                zPlatform_Shake(oplat, dt, 0.2f, 37.699112f);
            }
        }
    }
    else
    {
        oplat = (_zPlatform*)drv->driver;
    }

    if (oplat->subType == 11 || oplat->subType == 9)
    {
        zEntPlayer_Notify(8);
    }

    if (oplat->subType == 10 && globals.player.Health)
    {
        if ((oplat->passet->flags & 2) && (globals.player.JumpState == 0 || globals.player.JumpState == 1))
        {
            return;
        }

        if (oplat->tmr <= 0.0f)
        {
            oplat->ctr = 0;
        }
        else if (oplat->ctr < 2)
        {
            oplat->ctr++;
        }

        jmphs = oplat->passet->sb.jmph;
        jmpdir = &oplat->passet->sb.jmpdir;

        PlayerSpringboardJump(p, CalcJumpImpulse_Smooth(globals.player.s.Gravity, globals.player.s.JumpGravity,
                                                        MAX(MAX(jmphs[0], jmphs[1]), jmphs[2]), 0.3f,
                                                        globals.player.s.GravSmooth));
        xVec3SMul(&p->frame->vel, jmpdir, p->frame->vel.y);

        globals.player.Jump_CanDouble = 0;
        globals.player.Jump_CanFloat = 1;
        globals.player.Jump_Springboard = oplat;
        globals.player.Jump_SpringboardStart = 1;

        aplay = oplat->model->Anim;
        if (aplay)
        {
            asing = aplay->Single;
            asing->Time = 0.0f;
            asing->CurrentSpeed = 1.0f;
        }

        globals.player.Bounced = 2;
        zEntPlayer_Notify(9);
    }
}

void zEntPlayerExit(_xEnt* ent)
{
    xAnimTableRemoveEffectByCB(ent->model->Anim->Table, zEntPlayer_FXScratchCB);
    xAnimTableRemoveEffectByCB(ent->model->Anim->Table, zEntPlayer_SNDSkidCB);
    xAnimTableRemoveEffectByCB(ent->model->Anim->Table, zEntPlayer_FXDigupCB);
    xAnimTableRemoveEffectByCB(ent->model->Anim->Table, zEntPlayer_FXOpenBrollyCB);
    xAnimTableRemoveEffectByCB(ent->model->Anim->Table, zEntPlayer_FXSpringCB);
    zFeetExit(ent, ent->model->Anim->Table);
}

void zEntPlayerReset(_xEnt* ent)
{
    xModelInstance* tongue;
    U32 tongueIdx;
    U32 tngV;
    U32 widx;
    U32 wnum;
    U32 numV;
    iModelTag tmpTag;
    xAnimState* curr;
    xModelInstance* minst;
    U32 index;

    _iAnimSKBFixTransPop(
        xAnimTableGetState(ent->model->Anim->Table, "RunToIdle_normal")->Data->RawData[0],
        xAnimTableGetState(ent->model->Anim->Table, "Idle_normal")->Data->RawData[0], 0);
    _iAnimSKBFixTransPop(xAnimTableGetState(ent->model->Anim->Table, "SkidToIdle")->Data->RawData[0],
                         xAnimTableGetState(ent->model->Anim->Table, "Idle_normal")->Data->RawData[0], 0);

    if (zGameModeGet() != eGameMode_Game ||
        (zGameStateGet() != 5 && zGameStateGet() != 1 && zGameStateGet() != 4))
    {
        if (globals.player.Inv_Ammo[0] < globals.player.s.AmmoSceneGum)
        {
            globals.player.Inv_Ammo[0] = globals.player.s.AmmoSceneGum;
        }
        if (globals.player.Inv_Ammo[1] < globals.player.s.AmmoSceneSoap)
        {
            globals.player.Inv_Ammo[1] = globals.player.s.AmmoSceneSoap;
        }

        if (zGameModeGet() == eGameMode_Game && zGameStateGet() == 2)
        {
            globals.player.Health = 3;
            gPlayerHitWater = 0;
            sPlayerWaterOffset = 0.0f;
            gDeadScooby = 0;
        }
        else
        {
            globals.player.Health = 3;
            globals.player.Inv_Chances = 3;
            globals.player.Inv_ScoobySnacks = 0;
            globals.player.Inv_Specials = globals.player.s.Initial_Specials;
            globals.player.Inv_Mtokens[0] = 0;
            gPlayerHitWater = 0;
            sPlayerWaterOffset = 0.0f;
            gDeadScooby = 0;
            globals.player.Inv_Ammo[0] = globals.player.s.AmmoSceneGum;
            globals.player.Inv_Ammo[1] = globals.player.s.AmmoSceneSoap;
            globals.player.Inv_Key = 0;
            globals.player.Inv_Clue = 0;
        }
    }

    globals.player.ControlOn = 1;
    globals.player.ControlOnEvent = 0;
    globals.player.Speed = 0;
    globals.player.Sneak = 0;
    globals.player.Teeter = 0;
    globals.player.CanSlip = globals.sceneCur->baseCount[26];
    globals.player.Slide = 0;
    globals.player.SlideTimer = 0.0f;
    globals.player.Stepping = 0;
    globals.player.JumpState = 0;
    globals.player.LastJumpState = 0;
    globals.player.IdleMinorTimer = 0.0f;
    globals.player.IdleMajorTimer = 0.0f;
    globals.player.IdleSitTimer = 0.0f;
    globals.player.cheat_mode = 0;
    globals.player.FireTarget = NULL;
    globals.player.HeadbuttHit = 0;
    globals.player.LeanLerp = 1.0f;
    globals.player.ScareTimer = 0.0f;
    globals.player.DamageTimer = 0.0f;
    globals.player.ControlOffTimer = 0.0f;
    globals.player.HelmetTimer = 0.0f;
    globals.player.Bounced = 0;
    globals.player.FallDeathTimer = 0.0f;
    globals.player.Diggable = NULL;
    globals.player.Shaggy8Call = NULL;
    globals.player.Visible = 1;
    globals.player.AutoMoveSpeed = 0;
    globals.player.Shaggy1.SetDown = 0;
    globals.player.Shaggy1.GrabOn = 1;
    globals.player.Face_ScareTimer = 0.0f;
    globals.player.Face_EventTimer = 0.0f;
    globals.player.Face_PantTimer = 0.0f;
    globals.player.SpecialReceived = 0;
    globals.player.Jump_CanFloat = 1;
    globals.player.Jump_CanDouble = 1;
    globals.player.DamageSource = NULL;
    globals.player.Jump_Springboard = NULL;
    globals.player.Jump_SpringboardStart = 0;
    gPlayerHitWater = 0;
    sPlayerWaterOffset = 0.0f;
    gDeadScooby = 0;

    ent->collis->depenq = PlayerDepenQuery;
    ent->bupdate = PlayerBoundUpdate;
    surfSlickRatio = 0.0f;
    surfSlickTimer = 0.0f;
    globals.player.floor_surf = NULL;
    globals.player.floor_norm = g_Y3;
    ent->collis->chk = 0x2E;
    xEntReset(ent);
    ent->frame->rot.axis = g_Y3;
    ent->frame->rot.angle = ent->asset->ang.x;
    xEntDriveInit(&globals.player.drv, ent);
    globals.player.drv.flags = 1;

    xCameraSetTarget(&globals.camera, xEntGetPos(&globals.player.ent));
    xCameraSetTarget(&globals.camera, xEntGetFrame(&globals.player.ent));
    memcpy(&gPlayerAbsMat, &globals.camera.hi_cam.mat, sizeof(xMat4x3));

    globals.player.Jump_CurrGravity = globals.player.s.Gravity;

    iModelTagSetup(&globals.player.BubbleTag, globals.player.ent.model->Next->Data, 0.0f, 1.334f, 0.475f);
    iModelTagSetup(&globals.player.HangPawTag[0], globals.player.ent.model->Data, 0.12f, 0.015f, 0.175f);
    iModelTagSetup(&globals.player.HangPawTag[1], globals.player.ent.model->Data, -0.12f, 0.015f, 0.195f);
    iModelTagSetup(&globals.player.HangPawTag[2], globals.player.ent.model->Data, 0.2f, 0.018f, -0.62f);
    iModelTagSetup(&globals.player.HangPawTag[3], globals.player.ent.model->Data, -0.2f, 0.018f, -0.62f);
    globals.player.HangPawOffset = 1.5f;
    iModelTagSetup(&globals.player.center_tag, globals.player.ent.model->Data, 0.0f, 0.465f, 0.099f);
    iModelTagSetup(&globals.player.head_tag, globals.player.ent.model->Next->Data, 0.0f, 1.251f, 0.293f);

    globals.player.TongueFlags[0] = 0;
    globals.player.TongueFlags[1] = 0;
    tongue = globals.player.ent.model;
    tongueIdx = 0;
    while (tongue)
    {
        if (tongueIdx == 10)
        {
            numV = iModelVertCount(tongue->Data);
            for (tngV = 0; tngV < numV; tngV++)
            {
                iModelTagIndex(&tmpTag, tongue->Data, tngV);
                for (widx = 0; widx < 4; widx++)
                {
                    if (tmpTag.wt[widx])
                    {
                        wnum = (U8)(tmpTag.matidx >> (widx * 8));
                        globals.player.TongueFlags[wnum >> 5] |= 1 << (wnum & 0x1F);
                    }
                }
            }
            break;
        }
        tongue = tongue->Next;
        tongueIdx++;
    }

    for (curr = globals.player.ent.model->Anim->Table->StateList; curr; curr = curr->Next)
    {
        if (curr->UserFlags & 0x40)
        {
            if (curr->Data->NumAnims[0] != 3 || curr->Data->NumAnims[1] != 1)
            {
                curr->UserFlags &= ~0x40;
            }
        }
    }

    minst = globals.player.ent.model;
    index = 0;
    while (minst)
    {
        if (index >= 2)
        {
            minst->Flags &= ~3;
        }
        minst = minst->Next;
        index++;
    }

    globals.player.RootUp.x = 0.0f;
    globals.player.RootUp.y = 1.0f;
    globals.player.RootUp.z = 0.0f;
    globals.player.RootUpTarget.x = 0.0f;
    globals.player.RootUpTarget.y = 1.0f;
    globals.player.RootUpTarget.z = 0.0f;
    globals.player.HangElapsed = 0.0f;
    globals.player.WorldDisguise = 5;
    globals.player.HangEnt = NULL;
    globals.player.HangEntLast = NULL;
    globals.player.Shaggy1.Grab = NULL;
    globals.player.Shaggy1.GrabTry = NULL;
    globals.player.Shaggy1.GrabTimer = 0.0f;
    globals.player.Shaggy1.HangTarget = NULL;
    globals.player.Shaggy1.ThrowTimer = -1.0f;
    globals.player.Shaggy1.ThrowTgt = NULL;

    xEntBeginCollide(&globals.player.ent, globals.sceneCur, 0.0f);
    xEntSetNostepNormAngle(surfSlideStart);

    curVelmag = 0.0f;
    curVelangle = 0.0f;
    surfSlickness = 1;
    surfFriction = 1.0f;
    surfDamping = minVelmag;
    lastSlickness = 1;
    surfSticky = 0;

    zEntPlayer_SNDInit();

    sVONextSituation = NULL;
    sVOTimer = 0.0f;
    for (index = 0; index < 15; index++)
    {
        sPlayerInfo[index].elapsedTime = sPlayerInfo[index].delay;
        sPlayerInfo[index].count = 0;
    }

    xAnimPlaySetState(ent->model->Anim->Single, ent->model->Anim->Table->StateList, 0.0f);

    if (globals.sceneCur->sceneID == 'MNU3')
    {
        globals.player.ControlOn = 0;
        globals.player.ControlOnEvent = 0;
    }

    iColor_tag black = { 0, 0, 0, 255 };
    iColor_tag clear = { 0, 0, 0, 0 };
    xScrFxFade(&black, &clear, 1.0f, NULL, 0, 1);
}

// Collision of the player (data) with one dynamic or static object: the body bound and the head sphere are both
// tested and the nearer hit is kept; a model-collision object then collects the model's triangle hits, otherwise
// a side hit is pushed off sideways and a player sitting on a standable sphere is lifted out.
static _xEnt* PlayerCollCheckOneEnt(_xEnt* ent, _xScene* sc, void* data)
{
    _xEnt* p;
    _xCollis* coll;
    U32 modl_coll;
    U32 head_hit;
    _xCollis coll2;
    U8 ncolls;
    U8 idx;
    F32 rsum;
    F32 dx;
    F32 dy;
    F32 dz;
    F32 hsqr;

    p = (_xEnt*)data;
    modl_coll = 0;
    head_hit = 0;

    if (p->collis->idx >= 15)
    {
        return NULL;
    }

    if (!(ent->chkby & p->collType))
    {
        return ent;
    }

    if (ent->id == p->id)
    {
        return ent;
    }

    coll = &p->collis->colls[p->collis->idx];

    if (ent->collLev == 5 && (p->collType & 0x18))
    {
        modl_coll = 1;
    }

    if (modl_coll)
    {
        coll->flags = 0;
    }
    else
    {
        coll->flags = 0x1F00;
    }

    xBoundHitsBound(&p->bound, &ent->bound, coll);

    coll2.flags = 0x1F00;
    coll2.optr = NULL;
    coll2.dist = 1e19f;

    _xSphere temp = p->bound.sph;
    p->bound.sph = globals.player.head_sph;
    xBoundHitsBound(&p->bound, &ent->bound, &coll2);
    p->bound.sph = temp;

    if (coll2.flags & 1)
    {
        if (!(coll->flags & 1) || !(coll->dist < coll2.dist))
        {
            *coll = coll2;
            head_hit = 1;
        }
    }

    if (coll->flags & 1)
    {
        if (modl_coll)
        {
            coll->flags = 0x1F00;
            ncolls = iSphereHitsModel3(head_hit ? &globals.player.head_sph : &p->bound.sph, ent->model, coll,
                                       15 - p->collis->idx, DEG2RAD(45));
            for (idx = 0; idx < ncolls; idx++)
            {
                coll[idx].optr = ent;
                p->collis->idx++;
            }
            return ent;
        }

        coll->oid = 0;
        coll->optr = ent;
        p->collis->idx++;

        if (coll->flags & 0x10)
        {
            xVec3Sub(&coll->tohit, xBoundCenter(&ent->bound), xBoundCenter(&p->bound));
            hsqr = xVec3Length(&coll->tohit);
            xVec3SMul(&coll->depen, &coll->tohit, -0.35f * (1.0f / hsqr));
            coll->depen.y = 0.0f;
            hsqr = xVec3Length(&coll->depen);
            xVec3SMul(&coll->hdng, &coll->depen, -1.0f / hsqr);
            coll->dist = 0.7f - hsqr;
            xVec3SMul(&coll->tohit, &coll->hdng, coll->dist);
        }

        if ((ent->pflags & 0x20) && ent->bound.type == 1 && p->bound.type == 1 && coll->hdng.y < -0.866025f)
        {
            rsum = p->bound.sph.r + ent->bound.sph.r;
            dx = p->bound.sph.center.x - ent->bound.sph.center.x;
            dy = p->bound.sph.center.y - ent->bound.sph.center.y;
            dz = p->bound.sph.center.z - ent->bound.sph.center.z;
            hsqr = rsum * rsum - (dx * dx + dz * dz);
            if (hsqr >= 0.0f)
            {
                coll->depen.x = 0.0f;
                coll->depen.y = sqrtf(hsqr) - dy;
                coll->depen.z = 0.0f;
                coll->dist = p->bound.sph.r - coll->depen.y;
                coll->hdng.x = 0.0f;
                coll->hdng.y = -1.0f;
                coll->hdng.z = 0.0f;
            }
        }
    }

    return ent;
}

// Collision of the player (data) with one villain: a butt bounce or squish lands on the villain's top, otherwise
// the bound tests (with the villain's second bound), a sideways push off a side hit, and a lift out when the player
// sits on a villain flagged as standable.
static _xEnt* PlayerCollCheckOneVillain(_xEnt* ent, _xScene* sc, void* data)
{
    _xEnt* p;
    _xCollis* coll;
    U32 modl_coll;
    F32 playerOldRad;
    st_zEntVillain* vill;
    _xCollis coll2;
    F32 rsum;
    F32 dx;
    F32 dy;
    F32 dz;
    F32 hsqr;

    p = (_xEnt*)data;
    modl_coll = 0;
    if (p->collis->idx >= 15)
    {
        return NULL;
    }

    if (!(ent->chkby & p->collType))
    {
        return ent;
    }

    if (ent->id == p->id)
    {
        return ent;
    }

    coll = &p->collis->colls[p->collis->idx];
    if (ent->collLev == 5 && (p->collType & 0x18))
    {
        modl_coll = 1;
    }

    if (modl_coll)
    {
        coll->flags = 0;
    }
    else
    {
        coll->flags = 0x1F00;
    }

    if (cchkButtbounce ||
        (cchkSquish && (ent->subType == 0x16 || ent->subType == 0x18 || ent->subType == 0x15 || ent->subType == 0)))
    {
        if ((p->frame->oldpos.y >= ent->frame->mat.pos.y + ent->bound.sph.r ||
             p->frame->oldpos.y >= ent->frame->oldpos.y + ent->bound.sph.r) &&
            p->frame->mat.pos.y <= ent->frame->mat.pos.y + ent->bound.sph.r)
        {
            playerOldRad = p->bound.sph.r;
            p->bound.sph.r *= 1.5f;
            xBoundHitsBound(&p->bound, &ent->bound, coll);
            p->bound.sph.r = playerOldRad;
            if (coll->flags & 1)
            {
                coll->norm.x = 0.0f;
                coll->norm.y = 1.0f;
                coll->norm.z = 0.0f;
                coll->depen.x = 0.0f;
                coll->depen.y = ent->frame->mat.pos.y + ent->bound.sph.r - p->frame->mat.pos.y;
                coll->depen.z = 0.0f;
                coll->dist = p->bound.sph.r - coll->depen.y;
                coll->hdng.x = 0.0f;
                coll->hdng.y = -1.0f;
                coll->hdng.z = 0.0f;
                coll->tohit.x = coll->hdng.x * coll->dist;
                coll->tohit.y = coll->hdng.y * coll->dist;
                coll->tohit.z = coll->hdng.z * coll->dist;
                coll->oid = ent->id;
                coll->optr = ent;
                p->collis->idx++;
                return ent;
            }
        }
    }

    xBoundHitsBound(&p->bound, &ent->bound, coll);
    vill = (st_zEntVillain*)ent;
    if (vill->bound2.sph.r)
    {
        coll2.flags = 0x1F00;
        coll2.optr = NULL;
        coll2.dist = 1e19f;
        xBoundHitsBound(&p->bound, &vill->bound2, &coll2);
        if (coll2.flags & 1)
        {
            if (!(coll->flags & 1) || !(coll->dist < coll2.dist))
            {
                *coll = coll2;
            }
        }
    }

    if (coll->flags & 1)
    {
        coll->oid = 0;
        coll->optr = ent;
        p->collis->idx++;
        if (coll->flags & 0x10)
        {
            xVec3Sub(&coll->tohit, xBoundCenter(&ent->bound), xBoundCenter(&p->bound));
            hsqr = xVec3Length(&coll->tohit);
            xVec3SMul(&coll->depen, &coll->tohit, -0.35f * (1.0f / hsqr));
            coll->depen.y = 0.0f;
            hsqr = xVec3Length(&coll->depen);
            xVec3SMul(&coll->hdng, &coll->depen, -1.0f / hsqr);
            coll->dist = 0.7f - hsqr;
            xVec3SMul(&coll->tohit, &coll->hdng, coll->dist);
        }

        if ((ent->pflags & 0x20) && ent->bound.type == 1 && p->bound.type == 1 && coll->hdng.y < -0.866025f)
        {
            rsum = p->bound.sph.r + ent->bound.sph.r;
            dx = p->bound.sph.center.x - ent->bound.sph.center.x;
            dy = p->bound.sph.center.y - ent->bound.sph.center.y;
            dz = p->bound.sph.center.z - ent->bound.sph.center.z;
            hsqr = rsum * rsum - (dx * dx + dz * dz);
            if (hsqr >= 0.0f)
            {
                coll->depen.x = 0.0f;
                coll->depen.y = sqrtf(hsqr) - dy;
                coll->depen.z = 0.0f;
                coll->dist = p->bound.sph.r - coll->depen.y;
                coll->hdng.x = 0.0f;
                coll->hdng.y = -1.0f;
                coll->hdng.z = 0.0f;
            }
        }
    }

    return ent;
}

static void PlayerCollsWallsTranslate(_xCollis* colls, F32 x, F32 y, F32 z)
{
    _xCollis* c;

    c = &colls[3];
    if (c->dist < 1e19f && (c->depen.x != 0.0f || c->depen.y != 0.0f || c->depen.z != 0.0f))
    {
        PlayerCollisTranslate(c, x, y, z);
    }

    c = &colls[5];
    if (c->dist < 1e19f && (c->depen.x != 0.0f || c->depen.y != 0.0f || c->depen.z != 0.0f))
    {
        PlayerCollisTranslate(c, x, y, z);
    }
}

static void PlayerCollisTranslate(_xCollis* c, F32 x, F32 y, F32 z)
{
    F32 dx;
    F32 dy;
    F32 dz;
    F32 dist;
    F32 s;

    dx = c->tohit.x - x;
    dy = c->tohit.y - y;
    dz = c->tohit.z - z;
    c->tohit.x = dx;
    c->tohit.y = dy;
    c->tohit.z = dz;
    dist = sqrtf(dx * dx + dy * dy + dz * dz);
    c->dist = dist;
    s = 1.0f / dist;
    c->hdng.x = dx * s;
    c->hdng.y = dy * s;
    c->hdng.z = dz * s;
    s = MIN(0.0f, s * (dist - 0.7f));
    c->depen.x = dx * s;
    c->depen.y = dy * s;
    c->depen.z = dz * s;
}

static void PlayerCollsAllTranslate(_xCollis* colls, F32 x, F32 y, F32 z)
{
    _xCollis* cend;
    _xCollis* c;

    cend = colls + 6;
    for (c = colls; c < cend; c++)
    {
        if (c->dist < 1e19f && (c->depen.x != 0.0f || c->depen.y != 0.0f || c->depen.z != 0.0f))
        {
            PlayerCollisTranslate(c, x, y, z);
        }
    }
}

static inline void PlayerCollsWallsLoopTranslate(_xCollis* colls, F32 x, F32 y, F32 z)
{
    _xCollis* cend;
    _xCollis* c;

    cend = colls + 6;
    for (c = colls + 2; c < cend; c++)
    {
        if (c->dist < 1e19f && (c->depen.x != 0.0f || c->depen.y != 0.0f || c->depen.z != 0.0f))
        {
            PlayerCollisTranslate(c, x, y, z);
        }
    }
}

static void PlayerCollsSelectDepen(_xEnt* p, _xScene* sc, F32 dt)
{
    _xCollis* colls;
    _xMat4x3* mat;
    _xCollis* c;
    _xCollis* cend;
    U8 idx;
    _xCollis* curr;
    _xCollis* coll;
    F32 h_dot_n;
    F32 depen_len;
    _xEnt* cent;
    xSurface* surf;
    xSurface* sfloor;
    _xCollis* cceil;
    S32 num_walls;
    _xCollis* first_wall;
    _xCollis* inside_wall;
    _xCollis* cfront;
    _xCollis* crear;
    F32 dx;
    F32 dz;
    _xCollis* cleft;
    _xCollis* cright;

    colls = p->collis->colls;
    mat = (_xMat4x3*)p->frame;

    cend = &colls[p->collis->idx];
    for (c = &colls[k_XCOLLS_IDX_COUNT]; c < cend; c++)
    {
        idx = xCollideGetCollsIdx(&c->tohit, (xMat3x3*)mat);
        curr = &colls[idx];
        if (c->dist < curr->dist)
        {
            *curr = *c;
        }
    }

    if (p->pflags & 0x80)
    {
        coll = &p->collis->colls[k_XCOLLS_IDX_FLOOR];
        if ((coll->flags & 1) && coll->dist < 0.7f)
        {
            h_dot_n = xVec3Dot(&coll->hdng, &coll->norm);
            if (h_dot_n > 0.0f)
            {
                xVec3SMul(&coll->norm, &coll->norm, -1.0f);
                h_dot_n = -h_dot_n;
            }

            if (fabsf(acos(xVec3Dot(&coll->norm, &update_motion))) < PI / 4)
            {
                depen_len = h_dot_n * coll->dist + 0.7f;
                if (depen_len < 0.0f || depen_len > 0.7f)
                {
                    depen_len = CLAMP(depen_len, 0.0f, 0.7f);
                }
                xVec3SMul(&coll->depen, &coll->norm, depen_len);
            }
        }
    }

    cend = &colls[k_XCOLLS_IDX_COUNT];
    for (c = colls; c < cend; c++)
    {
        cent = (_xEnt*)c->optr;
        if (cent)
        {
            if (!(cent->penby & p->collType) || !(p->collis->pen & cent->collType))
            {
                c->depen.x = 0.0f;
                c->depen.y = 0.0f;
                c->depen.z = 0.0f;
            }
            else if (globals.player.DamageTimer > 0.0f && (surf = zSurfaceGetSurface(c)) &&
                     !surf->state && zSurfaceGetDamageType(surf) &&
                     zSurfaceGetDamagePassthrough(surf))
            {
                c->depen.x = 0.0f;
                c->depen.y = 0.0f;
                c->depen.z = 0.0f;
            }
            else if (cent->baseType == 2)
            {
                if (c == &colls[k_XCOLLS_IDX_FLOOR])
                {
                    c->depen.y = 0.0f;
                }
                else if (c != &colls[k_XCOLLS_IDX_FRONT])
                {
                    c->depen.x = 0.0f;
                    c->depen.y = 0.0f;
                    c->depen.z = 0.0f;
                }
            }
        }
    }

    sfloor = zSurfaceGetSurface(&colls[k_XCOLLS_IDX_FLOOR]);
    if (p->frame->vel.y > 0.0f)
    {
        colls[k_XCOLLS_IDX_FLOOR].flags &= ~1;
    }

    if ((colls[k_XCOLLS_IDX_FLOOR].flags & 1) && colls[k_XCOLLS_IDX_FLOOR].dist < 0.7f)
    {
        if (sfloor && (!zSurfaceGetStandOn(sfloor) || globals.player.Slide == 1 ||
                       globals.player.Slide == 3))
        {
            mat->pos.x += colls[k_XCOLLS_IDX_FLOOR].depen.x;
            mat->pos.y += colls[k_XCOLLS_IDX_FLOOR].depen.y;
            mat->pos.z += colls[k_XCOLLS_IDX_FLOOR].depen.z;
            PlayerCollsWallsLoopTranslate(colls, colls[k_XCOLLS_IDX_FLOOR].depen.x, colls[k_XCOLLS_IDX_FLOOR].depen.y, colls[k_XCOLLS_IDX_FLOOR].depen.z);
        }
        else
        {
            mat->pos.y += colls[k_XCOLLS_IDX_FLOOR].depen.y;
            PlayerCollsWallsLoopTranslate(colls, 0.0f, colls[k_XCOLLS_IDX_FLOOR].depen.y, 0.0f);
        }
    }
    else
    {
        cceil = &colls[k_XCOLLS_IDX_CEIL];
        if ((cceil->flags & 1) && cceil->dist < 0.7f)
        {
            if (cceil->hdng.y < icos(DEG2RAD(30)))
            {
                mat->pos.x += cceil->depen.x;
                mat->pos.y += cceil->depen.y;
                mat->pos.z += cceil->depen.z;
                PlayerCollsWallsLoopTranslate(colls, cceil->depen.x, cceil->depen.y, cceil->depen.z);
            }
            else
            {
                mat->pos.y += cceil->depen.y;
                PlayerCollsWallsLoopTranslate(colls, 0.0f, cceil->depen.y, 0.0f);
                p->frame->vel.y = 0.0f;
            }
        }
    }

    num_walls = 0;
    first_wall = NULL;
    inside_wall = NULL;
    cend = &colls[k_XCOLLS_IDX_COUNT];
    for (c = &colls[k_XCOLLS_IDX_FRONT]; c < cend; c++)
    {
        if (c->flags & 0x10)
        {
            inside_wall = c;
        }
        if (c->flags & 1)
        {
            if (c->optr)
            {
                num_walls++;
                if (!first_wall)
                {
                    first_wall = c;
                }
            }
            else
            {
                num_walls++;
                if (!first_wall)
                {
                    first_wall = c;
                }
            }
        }
    }

    if (inside_wall)
    {
        inside_wall->flags &= ~1;
    }

    if (num_walls == 0)
    {
        return;
    }

    if (num_walls == 1)
    {
        mat->pos.x += first_wall->depen.x;
        mat->pos.z += first_wall->depen.z;
        return;
    }

    cfront = &colls[k_XCOLLS_IDX_FRONT];
    crear = &colls[k_XCOLLS_IDX_REAR];
    if ((cfront->flags & 1) && cfront->dist < 0.7f)
    {
        if (cfront->optr)
        {
            if ((crear->flags & 1) && crear->dist < 0.7f)
            {
                if (crear->optr)
                {
                    dx = 0.5f * (crear->depen.x + cfront->depen.x);
                    dz = 0.5f * (crear->depen.z + cfront->depen.z);
                    mat->pos.x += dx;
                    mat->pos.z += dz;
                    PlayerCollsWallsTranslate(colls, dx, 0.0f, dz);
                }
                else
                {
                    mat->pos.x += crear->depen.x;
                    mat->pos.z += crear->depen.z;
                    PlayerCollsWallsTranslate(colls, crear->depen.x, 0.0f, crear->depen.z);
                }
            }
            else
            {
                mat->pos.x += cfront->depen.x;
                mat->pos.z += cfront->depen.z;
                PlayerCollsWallsTranslate(colls, cfront->depen.x, 0.0f, cfront->depen.z);
            }
        }
        else
        {
            if ((crear->flags & 1) && crear->dist < 0.7f)
            {
                if (crear->optr)
                {
                    mat->pos.x += cfront->depen.x;
                    mat->pos.z += cfront->depen.z;
                    PlayerCollsWallsTranslate(colls, cfront->depen.x, 0.0f, cfront->depen.z);
                }
                else
                {
                    dx = 0.5f * (crear->depen.x + cfront->depen.x);
                    dz = 0.5f * (crear->depen.z + cfront->depen.z);
                    mat->pos.x += dx;
                    mat->pos.z += dz;
                    PlayerCollsWallsTranslate(colls, dx, 0.0f, dz);
                }
            }
            else
            {
                mat->pos.x += cfront->depen.x;
                mat->pos.z += cfront->depen.z;
                PlayerCollsWallsTranslate(colls, cfront->depen.x, 0.0f, cfront->depen.z);
            }
        }
    }
    else if ((crear->flags & 1) && crear->dist < 0.7f)
    {
        if (crear->optr)
        {
            mat->pos.x += crear->depen.x;
            mat->pos.z += crear->depen.z;
            PlayerCollsWallsTranslate(colls, crear->depen.x, 0.0f, crear->depen.z);
        }
        else
        {
            mat->pos.x += crear->depen.x;
            mat->pos.z += crear->depen.z;
            PlayerCollsWallsTranslate(colls, crear->depen.x, 0.0f, crear->depen.z);
        }
    }

    cleft = &colls[k_XCOLLS_IDX_LEFT];
    cright = &colls[k_XCOLLS_IDX_RIGHT];
    if ((cleft->flags & 1) && cleft->dist < 0.7f)
    {
        if (cleft->optr)
        {
            if ((cright->flags & 1) && cright->dist < 0.7f)
            {
                if (cright->optr)
                {
                    dx = 0.5f * (cright->depen.x + cleft->depen.x);
                    dz = 0.5f * (cright->depen.z + cleft->depen.z);
                    mat->pos.x += dx;
                    mat->pos.z += dz;
                }
                else
                {
                    mat->pos.x += cright->depen.x;
                    mat->pos.z += cright->depen.z;
                }
            }
            else
            {
                mat->pos.x += cleft->depen.x;
                mat->pos.z += cleft->depen.z;
            }
        }
        else
        {
            if ((cright->flags & 1) && cright->dist < 0.7f)
            {
                if (cright->optr)
                {
                    mat->pos.x += cleft->depen.x;
                    mat->pos.z += cleft->depen.z;
                }
                else
                {
                    dx = 0.5f * (cright->depen.x + cleft->depen.x);
                    dz = 0.5f * (cright->depen.z + cleft->depen.z);
                    mat->pos.x += dx;
                    mat->pos.z += dz;
                }
            }
            else
            {
                mat->pos.x += cleft->depen.x;
                mat->pos.z += cleft->depen.z;
            }
        }
    }
    else if ((cright->flags & 1) && cright->dist < 0.7f)
    {
        if (cright->optr)
        {
            mat->pos.x += cright->depen.x;
            mat->pos.z += cright->depen.z;
        }
        else
        {
            mat->pos.x += cright->depen.x;
            mat->pos.z += cright->depen.z;
        }
    }
}

void zEntPlayerCollide(_xEnt* ent, _xScene* sc, F32 dt)
{
    _xCollis* colls;
    _xCollis* cend;
    _xCollis* c;

    cchkButtbounce =
        globals.player.Health && (ent->model->Anim->Single->State->UserFlags & 0x1E) == 0xE;
    cchkSquish = globals.player.Health &&
                 (ent->model->Anim->Single->State->UserFlags & 0x1E) == 0xA &&
                 ent->frame->mat.pos.y < ent->frame->oldpos.y;

    xEntBeginCollide(ent, sc, dt);

    if (ent->collis->chk & 0x20)
    {
        ent->collis->env_sidx = ent->collis->idx;
        ent->bound.sph.r = 0.7f * 1.3f;
        iSphereHitsEnv4(&ent->bound.sph, sc->env, (xMat3x3*)&ent->frame->mat, ent->collis->colls);
        ent->bound.sph.r = 0.7f;
        globals.player.head_sph.r = 0.7f * 1.3f;
        iSphereHitsEnv4(&globals.player.head_sph, sc->env, (xMat3x3*)&ent->frame->mat,
                        ent->collis->colls);
        globals.player.head_sph.r = 0.7f;
        PlayerCollsAllTranslate(ent->collis->colls, 0.0f, 0.0f, 0.0f);
        ent->collis->env_eidx = ent->collis->idx;
    }

    if (ent->collis->chk & 0x8)
    {
        xEntCollCheckNPCs(ent, sc, PlayerCollCheckOneVillain);
    }
    if (ent->collis->chk & 0x4)
    {
        xEntCollCheckDyns(ent, sc, PlayerCollCheckOneEnt);
    }
    if (ent->collis->chk & 0x2)
    {
        xEntCollCheckStats(ent, sc, PlayerCollCheckOneEnt);
    }

    if (ent->collis->chk & 0x2E)
    {
        PlayerCollsSelectDepen(ent, sc, dt);

        colls = ent->collis->colls;
        cend = &colls[ent->collis->idx];
        for (c = &colls[2]; c < cend; c++)
        {
            if (c->dist > 0.7f)
            {
                c->flags &= ~1;
            }
        }
        xEntEndCollide(ent, sc, dt);
    }
}

void zEntPlayerCollTrigger(_xEnt* ent, _xScene* sc)
{
    U16 i;

    xEntBeginCollCheckTrigs(ent);
    xSceneForAllTriggers(sc, zEntPlayerCollTriggerCB, ent);
    xEntEndCollCheckTrigs(ent);

    for (i = 0; i < sc->num_trigs; i++)
    {
        if (!(ent->baseFlags & 1))
        {
            continue;
        }

        if (ent->collis->trigs_entered[i >> 3] & (1 << (i % 8)))
        {
            p2EntTriggerAsset* asset = (p2EntTriggerAsset*)(sc->trigs[i]->asset + 1);
            if (asset->flags & 1)
            {
                if (xVec3Dot(&asset->direction, (xVec3*)&ent->model->Mat->at) <= 0.0f)
                {
                    zEntEvent(sc->trigs[i], 5, NULL);
                }
            }
            else
            {
                zEntEvent(sc->trigs[i], 5, NULL);
            }
        }

        if (ent->collis->trigs_exited[i >> 3] & (1 << (i % 8)))
        {
            p2EntTriggerAsset* asset = (p2EntTriggerAsset*)(sc->trigs[i]->asset + 1);
            if (asset->flags & 1)
            {
                if (xVec3Dot(&asset->direction, (xVec3*)&ent->model->Mat->at) <= 0.0f)
                {
                    zEntEvent(sc->trigs[i], 6, NULL);
                }
            }
            else
            {
                zEntEvent(sc->trigs[i], 6, NULL);
            }
        }

        if (ent->collis->trigs_cur[i >> 3] & (1 << (i % 8)) && globals.player.ControlOn &&
            globals.pad0->pressed & 0x20000)
        {
            zEntEvent(sc->trigs[i], 0x3B);
        }
    }
}

_xEnt* zEntPlayerCollTriggerCB(_xEnt* ent, _xScene* sc, void* data)
{
    _zEnt* p = (_zEnt*)data;
    _zEnt* t = (_zEnt*)ent;

    if (t->baseFlags & 1)
    {
        p2EntTriggerAsset* tasset = (p2EntTriggerAsset*)(t->asset + 1);

        switch (t->subType)
        {
        case 0:
        {
            _xBox box;
            xIsect isect;

            box.upper = tasset->p[1];
            box.lower = tasset->p[0];
            iBoxIsectVec(&box, &p->bound.sph.center, &isect);
            if (isect.penned <= 0.0f)
            {
                xEntSetTrig(p->collis, p->collis->trig_idx);
            }
            else
            {
                xEntClearTrig(p->collis, p->collis->trig_idx);
            }
            break;
        }
        case 1:
        {
            xSphere sphere;
            xIsect isect;

            sphere.center = tasset->p[0];
            sphere.r = tasset->p[1].x;
            iSphereIsectVec(&sphere, &p->bound.sph.center, &isect);
            if (isect.penned <= 0.0f)
            {
                xEntSetTrig(p->collis, p->collis->trig_idx);
            }
            else
            {
                xEntClearTrig(p->collis, p->collis->trig_idx);
            }
            break;
        }
        case 2:
        {
            _xCylinder cylinder;
            xIsect isect;

            cylinder.center = tasset->p[0];
            cylinder.r = tasset->p[1].x;
            cylinder.h = tasset->p[1].y;
            iCylinderIsectVec(&cylinder, &p->bound.sph.center, &isect);
            if (isect.penned <= 0.0f)
            {
                xEntSetTrig(p->collis, p->collis->trig_idx);
            }
            else
            {
                xEntClearTrig(p->collis, p->collis->trig_idx);
            }
            break;
        }
        case 3:
        {
            xSphere sphere;
            xIsect isect;

            sphere.center = tasset->p[0];
            sphere.r = tasset->p[1].x;
            iSphereIsectVec(&sphere, &p->bound.sph.center, &isect);
            if (isect.penned <= 0.0f)
            {
                xEntSetTrig(p->collis, p->collis->trig_idx);
            }
            else
            {
                xEntClearTrig(p->collis, p->collis->trig_idx);
            }
            break;
        }
        case 4:
        case 5:
            break;
        }
    }
    p->collis->trig_idx++;
    return t;
}

static xVec3* GetPosVec(xBase* target)
{
    xVec3* pos = (xVec3*)&g_O3;

    switch (target->baseType)
    {
    case 13:
        pos = ((xMovePoint*)target)->pos;
        break;
    case 2:
    case 3:
    case 4:
    case 6:
    case 8:
    case 11:
    case 12:
    case 18:
    case 23:
    case 24:
    case 27:
        pos = xEntGetPos((_xEnt*)target);
        break;
    }
    return pos;
}

S32 zEntPlayerEventCB(xBase* from, xBase* to, U32 toEvent, F32* toParam, xBase* toParamWidget)
{
    _zEnt* z;
    char* astatename;

    switch (toEvent)
    {
    case 0x111:
        zEntPlayer_Notify(1);
        break;
    case 0x112:
        break;
    case 0x113:
        zEntPlayer_Notify(8);
        break;
    case 0x114:
        break;
    case 0x115:
        break;
    case 0x116:
        zEntPlayer_Notify(2);
        break;
    case 0x117:
        break;
    case 0x118:
        zEntPlayer_Notify(3);
        break;
    case 0x119:
        break;
    case 0x11A:
        break;
    case 0x11B:
        zEntPlayer_Notify(10);
        break;
    case 0x11C:
        zEntPlayer_Notify(5);
        break;
    case 0x11D:
        break;
    case 8:
        globals.player.ControlOn = 0;
        globals.player.ControlOnEvent = 0;
        globals.player.ControlOffTimer = 1.0f;
        globals.player.AutoMoveSpeed = 0;
        xScrFxLetterbox(1);
        break;
    case 9:
        globals.player.ControlOnEvent = 1;
        globals.player.AutoMoveSpeed = 0;
        xScrFxLetterbox(0);
        break;
    case 0x36:
        globals.player.Health++;
        if (globals.player.Health > 5)
        {
            globals.player.Health = 5;
        }
        break;
    case 0x34:
        globals.player.Inv_Chances++;
        if (globals.player.Inv_Chances > 99)
        {
            globals.player.Inv_Chances = 99;
        }
        break;
    case 0x35:
        globals.player.Inv_ScoobySnacks++;
        zSceneGameStatsTally_snack(1);
        if (zEntPickup_lastSnackInArea())
        {
            zHudShowAllSnacksArea();
        }
        break;
    case 0x25:
        if (globals.player.Health)
        {
            globals.player.DamageTimer = 0.0f;
            zEntPlayer_Damage(from, globals.player.Health);
        }
        break;
    case 0x33:
        break;
    case 0x11:
        z = (_zEnt*)to;
        if (z->asset)
        {
            z->frame->mat.pos = z->asset->pos;
            *(xVec3*)&z->model->Mat->pos = z->asset->pos;
            z->frame->vel = g_O3;
            z->frame->rot.axis = g_Y3;
            z->frame->rot.angle = 0.0f;
        }
        break;
    case 0x5D:
        astatename = ((_zEnt*)to)->model->Anim->Single->State->Name;
        if (xStricmp(astatename, "Fall_losechance") && xStricmp(astatename, "Fall_losedrop"))
        {
            globals.player.FallDeathTimer = 1.2f;
            globals.player.Health = 0;
            globals.player.DamageTimer = 15.0f;
            globals.camera.hi_cam.flags |= 2;
            if (globals.player.s.TakeDamage)
            {
                gDeadScooby = 1;
                gDeadScoobyPos = *(xVec3*)&globals.player.ent.model->Mat->pos;
            }
            globals.player.ent.frame->vel.x = 0.0f;
            globals.player.ent.frame->vel.z = 0.0f;
        }
        break;
    case 3:
        globals.player.Visible = 1;
        break;
    case 4:
        globals.player.Visible = 0;
        break;
    case 0xCD:
        if (!globals.player.ControlOn && toParamWidget)
        {
            globals.player.AutoMoveDist = toParam[0];
            globals.player.AutoMoveSpeed = (U32)toParam[1] + 1;
            globals.player.AutoMoveTarget = *GetPosVec(toParamWidget);
            globals.player.AutoMoveObject = toParamWidget;
            if (globals.player.AutoMoveSpeed > 3 || globals.player.AutoMoveSpeed < 1)
            {
                globals.player.AutoMoveSpeed = 1;
            }
        }
        break;
    case 0xDA:
        if (!globals.player.ControlOn && toParamWidget)
        {
            globals.player.AutoMoveSpeed = 4;
            globals.player.AutoMoveTarget = *GetPosVec(toParamWidget);
            globals.player.AutoMoveObject = toParamWidget;
        }
        break;
    case 0x29:
        globals.player.AutoMoveSpeed = 0;
        break;
    case 0xE5:
        globals.player.Shaggy1.SetDown = 1;
        break;
    case 0xE6:
        globals.player.Shaggy1.GrabOn = 1;
        break;
    case 0xE7:
        globals.player.Shaggy1.GrabOn = 0;
        break;
    case 0xDD:
        globals.player.Shaggy1.ThrowTgt = toParamWidget;
        globals.player.Shaggy1.ThrowTgtAngle = toParam[0] ? toParam[0] : 45.0f;
        globals.player.Shaggy1.ThrowTgtPeak = toParam[1] ? toParam[1] : 6.0f;
        globals.player.Shaggy1.ThrowTgtOffset = toParam[2];
        break;
    case 0x12C:
        zRumbleStart(0, SDR_Test);
        break;
    case 0x12D:
        zRumbleStart(0, SDR_EventLight);
        break;
    case 0x12E:
        zRumbleStart(0, SDR_EventMedium);
        break;
    case 0x12F:
        zRumbleStart(0, SDR_EventHeavy);
        break;
    }

    return 1;
}

// When the player hangs from a hangable entity, its end follows the player, takes the swing velocity and
// turns with the player's spin.
static inline void PlayerHangableFollow(_zEntHangable* hang, _xEnt* ent, F32 dt)
{
    if (hang && hang->baseType == 0x17)
    {
        hang->endpos.x = ent->frame->mat.pos.x;
        hang->endpos.y = ent->frame->mat.pos.y + ent->bound.sph.r;
        hang->endpos.z = ent->frame->mat.pos.z;
        hang->vel.x = globals.player.HangVel.x;
        hang->vel.y = globals.player.HangVel.y;
        hang->vel.z = globals.player.HangVel.z;
        if (ent->frame->mode & 0x20)
        {
            hang->spin += ent->frame->drot.angle;
        }
        zEntHangable_SetMatrix(hang, dt);
    }
}

// Swinging on a hangable: lets go when the hangable is disabled or the hang animation has ended; otherwise
// pushes the swing with the stick (mag, angle), keeps the player on the rope's length around the pivot,
// damps the swing, eases in the grab, and moves the hangable's end with the player.
static void PlayerHangUpdate(_xEnt* ent, F32 mag, F32 angle, F32 dt)
{
    xAnimState* astate;
    F32 removedot;
    xVec3 unitHang;
    F32 px;
    F32 py;
    F32 pz;
    F32 ax;
    F32 ay;
    F32 az;

    zEntHangable_FollowUpdate(globals.player.HangEnt);
    astate = ent->model->Anim->Single->State;
    if (globals.player.HangEnt->enabled <= 0 || !(astate->UserFlags & 0x20000))
    {
        globals.player.HangEnt->grabTimer = globals.player.HangEnt->hangInfo->grabDelay;
        globals.player.HangEnt = NULL;
        globals.player.RootUpTarget.x = 0.0f;
        globals.player.RootUpTarget.y = 1.0f;
        globals.player.RootUpTarget.z = 0.0f;
        globals.player.HangElapsed = 0.0f;
        globals.player.Jump_CanDouble = 1;
        globals.player.Jump_CanFloat = 1;
        ent->frame->vel.y = globals.player.HangVel.y;
        return;
    }

    px = ((xMat4x3*)ent->model->Mat)->pos.x;
    py = ((xMat4x3*)ent->model->Mat)->pos.y;
    pz = ((xMat4x3*)ent->model->Mat)->pos.z;
    py += ent->bound.sph.r;

    ax = mag * isin(angle);
    ay = -globals.player.HangEnt->hangInfo->gravity;
    az = mag * icos(angle);
    if (globals.player.HangEnt->hangInfo->flags & 1)
    {
        F32 mag;

        ax *= 1.2f;
        az *= 1.2f;
        removedot = ax * globals.player.HangEnt->swingplane.x + az * globals.player.HangEnt->swingplane.z;
        ax -= removedot * globals.player.HangEnt->swingplane.x;
        az -= removedot * globals.player.HangEnt->swingplane.z;
        mag = ax * ax + az * az;
        if (mag > 1.0f)
        {
            mag = 1.0f / mag;
            ax *= mag;
            az *= mag;
        }
    }
    ax *= globals.player.HangEnt->hangInfo->accel;
    az *= globals.player.HangEnt->hangInfo->accel;

    unitHang.x = px - globals.player.HangPivot.x;
    unitHang.y = py - globals.player.HangPivot.y;
    unitHang.z = pz - globals.player.HangPivot.z;
    xVec3Normalize(&unitHang, &unitHang);

    removedot = unitHang.x * ax + unitHang.y * ay + unitHang.z * az;
    ax -= removedot * unitHang.x;
    globals.player.HangVel.x += ax * dt;
    ay -= removedot * unitHang.y;
    globals.player.HangVel.y += ay * dt;
    az -= removedot * unitHang.z;
    globals.player.HangVel.z += az * dt;

    removedot = unitHang.x * globals.player.HangVel.x + unitHang.y * globals.player.HangVel.y +
                unitHang.z * globals.player.HangVel.z;
    globals.player.HangVel.x -= removedot * unitHang.x;
    globals.player.HangVel.y -= removedot * unitHang.y;
    globals.player.HangVel.z -= removedot * unitHang.z;

    xVec3SMul(&globals.player.HangVel, &globals.player.HangVel, 1.0f - globals.player.HangEnt->hangInfo->decay);
    px += globals.player.HangVel.x * dt;
    py += globals.player.HangVel.y * dt;
    pz += globals.player.HangVel.z * dt;

    unitHang.x = px - globals.player.HangPivot.x;
    unitHang.y = py - globals.player.HangPivot.y;
    unitHang.z = pz - globals.player.HangPivot.z;
    xVec3Normalize(&unitHang, &unitHang);

    globals.player.RootUpTarget = unitHang;
    xVec3SMul(&globals.player.RootUpTarget, &globals.player.RootUpTarget, -1.0f);
    globals.player.HangElapsed += dt;

    xVec3SMul(&unitHang, &unitHang, globals.player.HangLength);
    xVec3Add(&ent->frame->mat.pos, &globals.player.HangPivot, &unitHang);
    ent->frame->mat.pos.y -= ent->bound.sph.r;
    ent->frame->mode |= 1;

    if (globals.player.HangStartLerp < 1.0f)
    {
        globals.player.HangStartLerp = MIN(1.0f, globals.player.HangStartLerp + 5.0f * dt);
    }

    PlayerHangableFollow(globals.player.HangEnt, ent, dt);
}

static void PlayerHangCheck(_xEnt* ent, _xScene* sc)
{
    U32 i;
    U32 sphereresult;
    F32 closedist;
    _zEntHangable* hangclose;
    _zEntHangable* hangme;
    xVec3 oldpos;
    F32 d1;
    F32 d2;

    globals.player.HangFound = NULL;
    if (globals.player.HangEnt)
    {
        return;
    }

    hangclose = NULL;
    closedist = 1e7f;
    for (i = 0; i < sc->num_dyns; i++)
    {
        hangme = (_zEntHangable*)sc->dyns[i];
        if (hangme->baseType == 0x17 && hangme->enabled > 0 && !hangme->grabTimer &&
            (hangme->state != 1 || hangme->shaggy))
        {
            F32 dist2 = xVec3Dist2(&ent->bound.sph.center, &hangme->endpos);
            if (dist2 < closedist)
            {
                closedist = dist2;
                hangclose = hangme;
            }
        }
    }

    if (!hangclose || closedist > 16.0f)
    {
        return;
    }

    oldpos = ent->frame->oldpos;
    oldpos.y += ent->bound.sph.r;
    d1 = xVec3Dist(&hangclose->pivot, &oldpos);
    d2 = xVec3Dist(&hangclose->pivot, &ent->bound.sph.center);
    d1 -= hangclose->hangInfo->leverArm;
    d2 -= hangclose->hangInfo->leverArm;
    sphereresult = d1 * d2 <= 0.0f || fabsf(d2) < 0.15f;

    xVec3 dpos;
    F32 dot;

    xVec3Sub(&dpos, &hangclose->endpos, &ent->bound.sph.center);
    dot = xVec3Dot(&dpos, &gPlayerAbsMat.right);
    dpos.x -= dot * gPlayerAbsMat.right.x;
    dpos.y -= dot * gPlayerAbsMat.right.y;
    dpos.z -= dot * gPlayerAbsMat.right.z;
    if (xVec3Length2(&dpos) < 2.25f && fabsf(dot) < 0.65f && sphereresult)
    {
        globals.player.HangFound = hangclose;
    }
}

static void PlayerTeeterCheck(_xEnt* ent, _xScene* sc, F32 dt)
{
    S32 i;

    if (globals.player.MountChimney)
    {
        globals.player.Teeter = 4;
        return;
    }

    if (globals.player.Teeter)
    {
        globals.player.Teeter--;
    }

    if (globals.player.JumpState == 0)
    {
        for (i = 0; i < 4; i++)
        {
            if (floor_dist[i] > 0.424264f)
            {
                if (floor_tmr[i] >= 0.2f)
                {
                    floor_tmr[i] = 0.2f;
                    globals.player.Teeter = 1;
                    return;
                }
                floor_tmr[i] += dt;
            }
            else
            {
                floor_tmr[i] = 0.0f;
            }
        }
    }
}

static void PlayerDigUpdate(_xEnt* ent, F32 dt)
{
    F32 digT;
    F32 digDelta;
    U32 i;
    p2LinkAsset* curlink;
    xBase* tgt;
    _xEnt* tgtEnt;

    digT = globals.player.DigTimer;
    digDelta = digT + dt;

    if (digT < 0.8f && digDelta >= 0.8f)
    {
        curlink = globals.player.Diggable->link;
        for (i = 0; i < globals.player.Diggable->linkCount; i++, curlink++)
        {
            tgt = zSceneFindObject(curlink->dstAssetID);
            if (tgt->baseType == 4 && ((_zEntPickup*)tgt)->state == 0x10)
            {
                zEntPickup_Drop((_zEntPickup*)tgt);
                zEntPlayer_Notify(5);
            }
        }
    }

    if (digT < 0.4f && digDelta >= 0.4f)
    {
        curlink = globals.player.Diggable->link;
        for (i = 0; i < globals.player.Diggable->linkCount; i++, curlink++)
        {
            tgt = zSceneFindObject(curlink->dstAssetID);
            if (tgt->baseType == 6)
            {
                tgtEnt = (_xEnt*)tgt;
                tgtEnt->flags |= 1;
                tgtEnt->chkby = 0x18;
                if (tgtEnt->model->Anim)
                {
                    tgtEnt->model->Anim->Single->CurrentSpeed = tgtEnt->model->Anim->Single->State->Speed;
                    tgtEnt->model->Anim->Single->Time = 0.0f;
                }
            }
        }
    }

    if (digT < 0.8f && digDelta >= 0.8f)
    {
        globals.player.Diggable->flags &= ~1;
        globals.player.Diggable->chkby = 0;
    }

    if (digDelta > 2.0f)
    {
        digDelta = 0.0f;
    }
    globals.player.DigTimer = digDelta;
}

// Tilts the player's up vector (RootUpTarget) toward the floor normal while standing on a surface that
// asks for it, within the maximum tilt; otherwise, when not hanging, relaxes it back to straight up.
static void PlayerRotMatchUpdateEnt(_xEnt* ent, _xScene* sc, F32 dt, void* fdata)
{
    xVec3* eup;
    xVec3* fup;
    S32 grounded;
    xSurface* surf;
    S32 hit_it;
    _xCollis* coll;
    xFFXRotMatchState* rms;

    rms = (xFFXRotMatchState*)fdata;
    if (ent->collis && ent->frame)
    {
        coll = &ent->collis->colls[0];
        grounded = coll->flags & 1;
        surf = zSurfaceGetSurface(coll);
        hit_it = grounded && surf && !surf->state && zSurfaceGetMatchOrient(surf);
        if (hit_it)
        {
            xVec3 nfup;
            xVec3 neup;
            F32 fdecl;
            F32 edecl;
            xVec3 raxis;
            F32 rang;
            F32 dang;
            xMat4x3 rot;

            if (!rms->lgrounded)
            {
                rms->tmr = 0.0f;
            }

            fup = &globals.player.floor_norm;
            eup = &globals.player.RootUpTarget;
            xVec3Normalize(&nfup, fup);
            xVec3Normalize(&neup, eup);
            fdecl = acos(nfup.y);
            edecl = acos(neup.y);
            if (edecl < rms->max_decl || fdecl < rms->max_decl)
            {
                xVec3Cross(&raxis, eup, fup);
                xVec3Normalize(&raxis, &raxis);
                dang = acos(xVec3Dot(&nfup, &neup));
                if (dang)
                {
                    rang = dt / rms->tmatch;
                    rang = MIN(1.0f, rang);
                    if (fdecl >= rms->max_decl)
                    {
                        rang = MIN(rang, (rms->max_decl - edecl) / (fdecl - edecl));
                    }
                    if (rang)
                    {
                        rang *= dang;
                        rang -= 0.001f;
                        xMat4x3Rot(&rot, &raxis, rang, &((xMat4x3*)ent->model->Mat)->pos);
                        xMat3x3RMulVec(eup, (xMat3x3*)&rot, &neup);
                        globals.player.HangElapsed = 0.0f;
                    }
                }
            }
        }
        else if (!globals.player.HangEnt)
        {
            if (rms->lgrounded)
            {
                rms->tmr = -0.1f;
            }
            if (rms->tmr > 0.0f)
            {
                xVec3 neup;
                xVec3 raxis;
                F32 rang;
                F32 dang;
                xMat4x3 rot;

                eup = &globals.player.RootUpTarget;
                xVec3Normalize(&neup, eup);
                raxis.x = -neup.z;
                raxis.y = 0.0f;
                raxis.z = neup.x;
                dang = acos(neup.y);
                if (dang)
                {
                    rang = MIN(1.0f, dt / rms->trelax);
                    xMat4x3Rot(&rot, &raxis, rang * dang, &((xMat4x3*)ent->model->Mat)->pos);
                    xMat3x3RMulVec(eup, (xMat3x3*)&rot, &neup);
                    globals.player.HangElapsed = 0.0f;
                }
            }
        }

        if ((F32)fabs(globals.player.RootUpTarget.y - 1.0f) < 0.00001f)
        {
            globals.player.RootUpTarget.y = 1.0f;
            globals.player.RootUpTarget.x = 0.0f;
            globals.player.RootUpTarget.z = 0.0f;
            globals.player.HangElapsed = 0.0f;
        }

        rms->tmr += dt;
        rms->lgrounded = hit_it;
    }
}

void zEntPlayer_StoreCheckPoint(F32* toParam)
{
    xVec3 p = globals.player.ent.frame->mat.pos;
    F32 r = globals.player.ent.frame->rot.angle;
    xVec3Copy(&globals.player.cp.pos, &p);
    if (toParam)
    {
        if (toParam[0])
        {
            globals.player.cp.rot = toParam[0];
        }
        else
        {
            globals.player.cp.rot = r;
        }
    }
    else
    {
        globals.player.cp.rot = r;
    }
    globals.player.cp.initCamID = globals.camera.hi_cam.id;
}

void zEntPlayer_LoadCheckPoint()
{
    xVec3Copy(&globals.player.ent.frame->mat.pos, &globals.player.cp.pos);
    xVec3Copy((xVec3*)&globals.player.ent.model->Mat->pos, &globals.player.cp.pos);
    if (globals.player.cp.rot)
    {
        globals.player.ent.frame->rot.angle = globals.player.cp.rot;
    }
    xCameraSetTarget(&globals.camera, &globals.player.ent.frame->mat.pos);
    xCameraSetTarget(&globals.camera, (xMat4x3*)globals.player.ent.model->Mat);
    xCameraPickInitial(&globals.camera, globals.player.cp.initCamID);
}

// Plays the butt-bounce sound with a rising pitch, then picks a different one of the three bounce sounds
// for next time. A miss plays the miss sound instead.
static void zEntPlayer_SNDButtBounce(_xEnt* ent, S32 hit_villain)
{
    S16 choice;
    F32 pitch;

    sPlayerSndfx.butt_pitch = zPitchChange(sPlayerSndfx.butt_pitch, &pitch, 30, 1.5f);
    if (hit_villain)
    {
        xSndPlay(sPlayerSndfx.butt_id[sPlayerSndfx.butt_snd], 0.5f, 0.5f, pitch, 0x80, 2, 0);
        choice = sPlayerSndfx.butt_snd;
        while (choice == sPlayerSndfx.butt_snd)
        {
            choice = rand() % 3;
        }
        sPlayerSndfx.butt_snd = choice;
    }
    else
    {
        xSndPlay(sPlayerSndfx.buttmiss_id, 0.5f, 0.5f, 0.0f, 0x80, 2, 0);
    }
}

void zEntPlayer_SNDInit()
{
    S32 i;
    char tmp[256];

    i = 0;
    sPlayerSndfx.butt_id[i++] = xStrHash("sndfx_butt_smash1");
    sPlayerSndfx.butt_id[i++] = xStrHash("sndfx_butt_smash2");
    sPlayerSndfx.butt_id[i++] = xStrHash("sndfx_butt_smash3");
    sPlayerSndfx.butt_pitch = iurand() * 359.0f;
    sPlayerSndfx.butt_snd = rand() % 3;
    i = 0;
    sPlayerSndfx.head_id[i++] = xStrHash("sndfx_head_butt1");
    sPlayerSndfx.head_id[i++] = xStrHash("sndfx_head_butt2");
    sPlayerSndfx.head_id[i++] = xStrHash("sndfx_head_butt3");
    sPlayerSndfx.head_pitch = iurand() * 359.0f;
    sPlayerSndfx.head_snd = rand() % 3;
    sPlayerSndfx.buttmiss_id = xStrHash("sndfx_butt_miss1");
    i = 0;
    sPlayerSndfx.skid_id[i++] = xStrHash("sndfx_skid1");
    sPlayerSndfx.skid_id[i++] = xStrHash("sndfx_skid2");
    sPlayerSndfx.skid_id[i++] = xStrHash("sndfx_skid3");
    sPlayerSndfx.ammo_pitch = iurand() * 359.0f;
    sPlayerSndfx.special_pitch = iurand() * 359.0f;
    sPlayerSndfx.token_pitch = iurand() * 359.0f;
    sPlayerSndfx.save_pitch = iurand() * 359.0f;
    sPlayerSndfx.key_pitch = iurand() * 359.0f;
    sPlayerSndfx.info_pitch = iurand() * 359.0f;
    sPlayerSndfx.error_pitch = iurand() * 359.0f;
    sPlayerSndfx.bite_pitch = iurand() * 359.0f;
    sPlayerSndfx.snackcounter_pitch = iurand() * 359.0f;
    sPlayerSndfx.ammo_id = xStrHash("sndfx_pu_ammo");
    sPlayerSndfx.special_id = xStrHash("sndfx_pu_special");
    sPlayerSndfx.token_id = xStrHash("sndfx_pu_token");
    sPlayerSndfx.save_id = xStrHash("sndfx_pu_save");
    sPlayerSndfx.info_id = xStrHash("sndfx_pu_info");
    sPlayerSndfx.key_id = xStrHash("sndfx_pu_key");
    sPlayerSndfx.error_id = xStrHash("sndfx_pu_error");
    sPlayerSndfx.bite_id = xStrHash("sndfx_pu_bite");
    sPlayerSndfx.snackcounter_id = xStrHash("sndfx_pu_counter");
    sPlayerSndfx.van_id = xStrHash("sndfx_pu_van");
    i = 0;
    sPlayerSndfx.dig_id[i++] = xStrHash("scooby_one_30");
    sPlayerSndfx.dig_id[i++] = xStrHash("scooby_one_29");
    i = 0;
    sPlayerSndfx.scare_id[i++] = xStrHash("Scooby_Help");
    sPlayerSndfx.scare_id[i++] = xStrHash("Scooby_Oh_No");
    i = 0;
    sPlayerSndfx.danger_id[i++] = xStrHash("scooby_gulp_01");
    sPlayerSndfx.danger_id[i++] = xStrHash("scooby_gulp_02");
    sPlayerSndfx.danger_id[i++] = xStrHash("scooby_gulp_03");
    sPlayerSndfx.danger_id[i++] = xStrHash("scooby_gulp_04");
    sPlayerSndfx.danger_id[i++] = xStrHash("scooby_gulp_05");
    sPlayerSndfx.danger_id[i++] = xStrHash("scooby_gulp_06");
    sPlayerSndfx.danger_id[i++] = xStrHash("scooby_gulp_07");
    sPlayerSndfx.danger_id[i++] = xStrHash("scooby_gulp_08");
    sPlayerSndfx.danger_id[i++] = xStrHash("scooby_one_26");
    i = 0;
    sPlayerSndfx.leap_id[i++] = xStrHash("scooby_excite_09");
    sPlayerSndfx.leap_id[i++] = xStrHash("scooby_excite_10");
    sPlayerSndfx.leap_id[i++] = xStrHash("scooby_bounce_01");
    sPlayerSndfx.leap_id[i++] = xStrHash("scooby_bounce_02");
    sPlayerSndfx.leap_id[i++] = xStrHash("scooby_bounce_03");
    sPlayerSndfx.leap_id[i++] = xStrHash("scooby_bounce_09");
    sPlayerSndfx.leap_id[i++] = xStrHash("scooby_bounce_10");
    sPlayerSndfx.leap_id[i++] = xStrHash("scooby_bounce_15");
    sPlayerSndfx.leap_id[i++] = xStrHash("scooby_bounce_16");
    sPlayerSndfx.leap_id[i++] = xStrHash("scooby_bounce_18");
    sPlayerSndfx.leap_id[i++] = xStrHash("scooby_bounce_19");
    sPlayerSndfx.leap_id[i++] = xStrHash("scooby_bounce_20");
    for (i = 0; i < 13; i++)
    {
        sprintf(tmp, "scooby_excite_%02d", i + 1);
        sPlayerSndfx.excite_id[i] = xStrHash(tmp);
    }
    i = 0;
    sPlayerSndfx.see_geronimo_id[i++] = xStrHash("scooby_one_42b");
    i = 0;
    sPlayerSndfx.see_shaggy_id[i++] = xStrHash("scooby_one_19a");
    sPlayerSndfx.see_shaggy_id[i++] = xStrHash("scooby_one_19b");
    sPlayerSndfx.see_shaggy_id[i++] = xStrHash("scooby_one_20a");
    sPlayerSndfx.see_shaggy_id[i++] = xStrHash("scooby_one_20b");
    sPlayerSndfx.see_shaggy_id[i++] = xStrHash("scooby_one_20c");
    i = 0;
    sPlayerSndfx.see_monster_id[i++] = xStrHash("scooby_one_53c");
    sPlayerSndfx.see_monster_id[i++] = xStrHash("scooby_one_21a");
    sPlayerSndfx.see_monster_id[i++] = xStrHash("scooby_one_21b");
    sPlayerSndfx.see_monster_id[i++] = xStrHash("scooby_one_22a");
    sPlayerSndfx.see_monster_id[i++] = xStrHash("scooby_one_22b");
    i = 0;
    sPlayerSndfx.see_holly_id[i++] = xStrHash("scooby_holly1a");
    sPlayerSndfx.see_holly_id[i++] = xStrHash("scooby_holly1b");
    sPlayerSndfx.see_holly_id[i++] = xStrHash("scooby_holly2");
    i = 0;
    sPlayerSndfx.see_groundskeeper_id[i++] = xStrHash("scooby_holly1a");
    i = 0;
    sPlayerSndfx.see_warpgate_id[i++] = xStrHash("scooby_one_25");
    i = 0;
    sPlayerSndfx.need_more_snacks_id[i++] = xStrHash("scooby_one_23a");
    sPlayerSndfx.need_more_snacks_id[i++] = xStrHash("scooby_one_23b");
    sPlayerSndfx.need_more_snacks_id[i++] = xStrHash("scooby_one_23c");
    i = 0;
    sPlayerSndfx.success_id[i++] = xStrHash("scooby_one_07");
    i = 0;
    sPlayerSndfx.see_token_id[i++] = xStrHash("scooby_one_24a");
    sPlayerSndfx.see_token_id[i++] = xStrHash("scooby_one_24b");
    sPlayerSndfx.see_token_id[i++] = xStrHash("scooby_one_24c");
    i = 0;
    sPlayerSndfx.see_crab_id[i++] = xStrHash("scooby_one_53c");
    sPlayerSndfx.see_crab_id[i++] = xStrHash("scooby_one_31");
    i = 0;
    sPlayerSndfx.see_rat_id[i++] = xStrHash("scooby_one_53c");
    sPlayerSndfx.see_rat_id[i++] = xStrHash("scooby_one_32");
    sPlayerSndfx.see_rat_id[i++] = xStrHash("scooby_one_32b");
    sPlayerSndfx.see_rat_id[i++] = xStrHash("scooby_one_32c");
    i = 0;
    sPlayerSndfx.see_gargoyle_id[i++] = xStrHash("scooby_one_53c");
    sPlayerSndfx.see_gargoyle_id[i++] = xStrHash("scooby_one_41");
    i = 0;
    sPlayerSndfx.see_creeper_id[i++] = xStrHash("scooby_one_53c");
    sPlayerSndfx.see_creeper_id[i++] = xStrHash("scooby_one_44");
    i = 0;
    sPlayerSndfx.see_ghost_diver_id[i++] = xStrHash("scooby_one_53c");
    sPlayerSndfx.see_ghost_diver_id[i++] = xStrHash("scooby_one_45");
    i = 0;
    sPlayerSndfx.see_sea_creature_id[i++] = xStrHash("scooby_one_53c");
    sPlayerSndfx.see_sea_creature_id[i++] = xStrHash("scooby_one_46");
    i = 0;
    sPlayerSndfx.see_headless_specter_id[i++] = xStrHash("scooby_one_53c");
    sPlayerSndfx.see_headless_specter_id[i++] = xStrHash("scooby_one_47");
    i = 0;
    sPlayerSndfx.see_scarecrow_id[i++] = xStrHash("scooby_one_53c");
    sPlayerSndfx.see_scarecrow_id[i++] = xStrHash("scooby_one_49");
    sPlayerSndfx.see_scarecrow_id[i++] = xStrHash("scooby_one_49b");
    i = 0;
    sPlayerSndfx.see_space_kook_id[i++] = xStrHash("scooby_one_53c");
    sPlayerSndfx.see_space_kook_id[i++] = xStrHash("scooby_one_50");
    i = 0;
    sPlayerSndfx.see_tar_monster_id[i++] = xStrHash("scooby_one_53c");
    sPlayerSndfx.see_tar_monster_id[i++] = xStrHash("scooby_one_51");
    sPlayerSndfx.see_tar_monster_id[i++] = xStrHash("scooby_one_51b");
    sPlayerSndfx.see_tar_monster_id[i++] = xStrHash("scooby_one_51c");
    i = 0;
    sPlayerSndfx.see_witch_id[i++] = xStrHash("scooby_one_53c");
    sPlayerSndfx.see_witch_id[i++] = xStrHash("scooby_one_52");
    i = 0;
    sPlayerSndfx.see_robot_id[i++] = xStrHash("scooby_one_53c");
    sPlayerSndfx.see_robot_id[i++] = xStrHash("scooby_one_48");
    i = 0;
    sPlayerSndfx.see_wolfman_id[i++] = xStrHash("scooby_one_53c");
    sPlayerSndfx.see_wolfman_id[i++] = xStrHash("scooby_one_53");
    i = 0;
    sPlayerSndfx.see_ghost_id[i++] = xStrHash("scooby_one_53c");
    sPlayerSndfx.see_ghost_id[i++] = xStrHash("scooby_one_57a");
    sPlayerSndfx.see_ghost_id[i++] = xStrHash("scooby_one_57b");
    sPlayerSndfx.see_ghost_id[i++] = xStrHash("scooby_one_57c");
    i = 0;
    sPlayerSndfx.see_zombie_id[i++] = xStrHash("scooby_one_53c");
    sPlayerSndfx.see_zombie_id[i++] = xStrHash("scooby_one_54");
    i = 0;
    sPlayerSndfx.sorry_id[i++] = xStrHash("scooby_one_14a");
    sPlayerSndfx.sorry_id[i++] = xStrHash("scooby_one_14b");
    sPlayerSndfx.sorry_id[i++] = xStrHash("scooby_one_15a");
    sPlayerSndfx.sorry_id[i++] = xStrHash("scooby_one_14b");
    i = 0;
    sPlayerSndfx.collect_all_snacks_id[i++] = xStrHash("scooby_one_07");
    i = 0;
    sPlayerSndfx.eat_sandwich_id[i++] = xStrHash("scooby_one_09");
    sPlayerSndfx.eat_sandwich_id[i++] = xStrHash("scooby_one_06b");
    i = 0;
    sPlayerSndfx.eat_turkey_id[i++] = xStrHash("scooby_one_10");
    sPlayerSndfx.eat_turkey_id[i++] = xStrHash("scooby_one_06b");
    i = 0;
    sPlayerSndfx.eat_cake_id[i++] = xStrHash("scooby_one_11");
    sPlayerSndfx.eat_cake_id[i++] = xStrHash("scooby_one_06b");
    i = 0;
    sPlayerSndfx.eat_snacks_id[i++] = xStrHash("scooby_one_03a");
    sPlayerSndfx.eat_snacks_id[i++] = xStrHash("scooby_one_03b");
    sPlayerSndfx.eat_snacks_id[i++] = xStrHash("scooby_one_03c");
}

void zEntPlayer_SNDBitePickup(_xEnt*)
{
    F32 pitch;

    sPlayerSndfx.bite_pitch = zPitchChange(sPlayerSndfx.bite_pitch, &pitch, 30, 0.25f);
    xSndPlay(sPlayerSndfx.bite_id, 0.5f, 0.5f, pitch, 0x80, 2, 0);
}

void zEntPlayer_SNDAmmoPickup(_xEnt*)
{
    F32 pitch;

    sPlayerSndfx.ammo_pitch = zPitchChange(sPlayerSndfx.ammo_pitch, &pitch, 30, 0.25f);
    xSndPlay(sPlayerSndfx.ammo_id, 0.5f, 0.5f, pitch, 0x80, 2, 0);
}

void zEntPlayer_SNDSpecialPickup(_xEnt*)
{
    F32 pitch;

    sPlayerSndfx.special_pitch = zPitchChange(sPlayerSndfx.special_pitch, &pitch, 30, 0.25f);
    xSndPlay(sPlayerSndfx.special_id, 0.5f, 0.5f, pitch, 0x80, 2, 0);
}

void zEntPlayer_SNDTokenPickup(_xEnt*)
{
    F32 pitch;

    sPlayerSndfx.token_pitch = zPitchChange(sPlayerSndfx.token_pitch, &pitch, 30, 0.25f);
    xSndPlay(sPlayerSndfx.token_id, 0.5f, 0.5f, pitch, 0x80, 2, 0);
}

void zEntPlayer_SNDSavePickup(_xEnt*)
{
    F32 pitch;

    sPlayerSndfx.save_pitch = zPitchChange(sPlayerSndfx.save_pitch, &pitch, 30, 0.25f);
    xSndPlay(sPlayerSndfx.save_id, 0.5f, 0.5f, pitch, 0x80, 2, 0);
}

void zEntPlayer_SNDInfoPickup(_xEnt*)
{
    F32 pitch;

    sPlayerSndfx.info_pitch = zPitchChange(sPlayerSndfx.info_pitch, &pitch, 30, 0.25f);
    xSndPlay(sPlayerSndfx.info_id, 0.5f, 0.5f, pitch, 0x80, 2, 0);
}

void zEntPlayer_SNDErrorPickup(_xEnt*)
{
    F32 pitch;

    sPlayerSndfx.error_pitch = zPitchChange(sPlayerSndfx.error_pitch, &pitch, 30, 0.25f);
    xSndPlay(sPlayerSndfx.error_id, 0.5f, 0.5f, pitch, 0x80, 2, 0);
}

void zEntPlayer_SNDKeyPickup(_xEnt*)
{
    F32 pitch;

    sPlayerSndfx.key_pitch = zPitchChange(sPlayerSndfx.key_pitch, &pitch, 30, 0.25f);
    xSndPlay(sPlayerSndfx.key_id, 0.5f, 0.5f, pitch, 0x80, 2, 0);
}

void zEntPlayer_SNDSnackCounter(_xEnt*)
{
    xSndPlay(sPlayerSndfx.snackcounter_id, 0.5f, 0.5f, 0.0f, 0x80, 2, 0);
}

void zEntPlayer_SNDVan(_xEnt*)
{
    xSndPlay(sPlayerSndfx.van_id, 0.5f, 0.5f, 0.0f, 0x80, 2, 0);
}

static void zEntPlayer_SNDPlayRandom(U32* array, U32 max)
{
    U32 i;
    U32 idx;

    i = 0;
    idx = irand() % max;
    sVOAssetID = array[idx];
    do
    {
        sVO = xSndPlay(sVOAssetID, 1.0f, 1.0f, 0.0f, 0x80, 0x202, 0);
        if (sVO != 0 && sVO != 0xFFFFFFFF)
        {
            return;
        }

        idx++;
        if (idx >= max)
        {
            idx = 0;
        }
        i++;
        sVOAssetID = array[idx];
    } while (i < max);
}

void zEntPlayer_SituationDo()
{
    if (sVONextSituation && globals.player.Health)
    {
        switch (sVONextSituation->type)
        {
        case 33:
            zEntPlayer_SNDPlayRandom(sPlayerSndfx.eat_cake_id, 2);
            break;
        case 34:
            zEntPlayer_SNDPlayRandom(sPlayerSndfx.eat_sandwich_id, 2);
            break;
        case 35:
            zEntPlayer_SNDPlayRandom(sPlayerSndfx.eat_turkey_id, 2);
            break;
        case 36:
            zEntPlayer_SNDPlayRandom(sPlayerSndfx.eat_snacks_id, 3);
            break;
        case 31:
            zEntPlayer_SNDPlayRandom(sPlayerSndfx.sorry_id, 4);
            break;
        case 3:
            zEntPlayer_SNDPlayRandom(sPlayerSndfx.see_token_id, 3);
            break;
        case 5:
            zEntPlayer_SNDPlayRandom(sPlayerSndfx.success_id, 1);
            break;
        case 15:
            zEntPlayer_SNDPlayRandom(sPlayerSndfx.need_more_snacks_id, 3);
            break;
        case 14:
            zEntPlayer_SNDPlayRandom(sPlayerSndfx.see_warpgate_id, 1);
            break;
        case 9:
            zEntPlayer_SNDPlayRandom(sPlayerSndfx.leap_id, 12);
            break;
        case 0:
            zEntPlayer_SNDPlayRandom(sPlayerSndfx.dig_id, 2);
            break;
        case 8:
            zEntPlayer_SNDPlayRandom(sPlayerSndfx.danger_id, 9);
            break;
        case 1:
            zEntPlayer_SNDPlayRandom(sPlayerSndfx.scare_id, 2);
            break;
        case 7:
            zEntPlayer_SNDPlayRandom(sPlayerSndfx.excite_id, 13);
            break;
        case 12:
            zEntPlayer_SNDPlayRandom(sPlayerSndfx.see_geronimo_id, 1);
            break;
        case 16:
            zEntPlayer_SNDPlayRandom(sPlayerSndfx.see_crab_id, 2);
            break;
        case 17:
            zEntPlayer_SNDPlayRandom(sPlayerSndfx.see_rat_id, 4);
            break;
        case 18:
            zEntPlayer_SNDPlayRandom(sPlayerSndfx.see_gargoyle_id, 2);
            break;
        case 19:
            zEntPlayer_SNDPlayRandom(sPlayerSndfx.see_creeper_id, 2);
            break;
        case 20:
            zEntPlayer_SNDPlayRandom(sPlayerSndfx.see_ghost_diver_id, 2);
            break;
        case 21:
            zEntPlayer_SNDPlayRandom(sPlayerSndfx.see_sea_creature_id, 2);
            break;
        case 22:
            zEntPlayer_SNDPlayRandom(sPlayerSndfx.see_headless_specter_id, 2);
            break;
        case 23:
            zEntPlayer_SNDPlayRandom(sPlayerSndfx.see_scarecrow_id, 3);
            break;
        case 24:
            zEntPlayer_SNDPlayRandom(sPlayerSndfx.see_space_kook_id, 2);
            break;
        case 25:
            zEntPlayer_SNDPlayRandom(sPlayerSndfx.see_tar_monster_id, 4);
            break;
        case 26:
            zEntPlayer_SNDPlayRandom(sPlayerSndfx.see_witch_id, 2);
            break;
        case 27:
            zEntPlayer_SNDPlayRandom(sPlayerSndfx.see_robot_id, 2);
            break;
        case 28:
            zEntPlayer_SNDPlayRandom(sPlayerSndfx.see_wolfman_id, 2);
            break;
        case 29:
            zEntPlayer_SNDPlayRandom(sPlayerSndfx.see_ghost_id, 4);
            break;
        case 30:
            zEntPlayer_SNDPlayRandom(sPlayerSndfx.see_zombie_id, 2);
            break;
        case 2:
            zEntPlayer_SNDPlayRandom(sPlayerSndfx.see_shaggy_id, 5);
            break;
        case 10:
            zEntPlayer_SNDPlayRandom(sPlayerSndfx.see_monster_id, 5);
            break;
        case 11:
            if (globals.sceneCur->sceneID != 'S005')
            {
                zEntPlayer_SNDPlayRandom(sPlayerSndfx.see_holly_id, 3);
            }
            break;
        case 13:
            zEntPlayer_SNDPlayRandom(sPlayerSndfx.see_groundskeeper_id, 1);
            break;
        case 32:
            zEntPlayer_SNDPlayRandom(sPlayerSndfx.collect_all_snacks_id, 1);
            break;
        }

        sVONextSituation->elapsedTime = 0.0f;
        sVONextSituation->count++;
        sVONextSituation = NULL;
    }
}

void zEntPlayer_Notify(S32 characterSubType, F32)
{
    if (!globals.player.ControlOn)
    {
        return;
    }

    switch (characterSubType)
    {
    case 2:
        zEntPlayer_Notify(12);
        break;
    case 21:
        zEntPlayer_Notify(16);
        break;
    case 22:
        zEntPlayer_Notify(17);
        break;
    case 5:
        zEntPlayer_Notify(18);
        break;
    case 3:
        zEntPlayer_Notify(19);
        break;
    case 7:
        zEntPlayer_Notify(20);
        break;
    case 4:
        zEntPlayer_Notify(21);
        break;
    case 9:
        zEntPlayer_Notify(22);
        break;
    case 13:
        zEntPlayer_Notify(23);
        break;
    case 15:
        zEntPlayer_Notify(24);
        break;
    case 16:
        zEntPlayer_Notify(25);
        break;
    case 17:
        zEntPlayer_Notify(26);
        break;
    case 12:
        zEntPlayer_Notify(27);
        break;
    case 19:
        zEntPlayer_Notify(28);
        break;
    case 6:
        zEntPlayer_Notify(29);
        break;
    case 20:
        zEntPlayer_Notify(30);
        break;
    case 47:
        zEntPlayer_Notify(11);
        break;
    case 48:
        zEntPlayer_Notify(13);
        break;
    case 28:
    case 29:
    case 30:
    case 31:
    case 32:
    case 33:
    case 34:
    case 35:
    case 36:
    case 37:
        zEntPlayer_Notify(2);
        break;
    default:
        zEntPlayer_Notify(10);
        break;
    }
}

void zEntPlayer_Notify(S32 situation)
{
    zPlayerSituation* s;

    if (!globals.player.ControlOn)
    {
        return;
    }

    s = &sPlayerInfo[situation];
    if (sVONextSituation && sVONextSituation->priority >= s->priority)
    {
        return;
    }
    if (iSndIsPlaying(sVOAssetID) && s->priority <= sVOPriority)
    {
        return;
    }
    if (s->countMax && s->count >= s->countMax)
    {
        return;
    }
    if (s->delay > s->elapsedTime)
    {
        return;
    }

    sVONextSituation = s;
    sVOTimer = s->punchDelay;
}

static void zEntPlayer_FXLostChance(_xEnt* ent, F32 seconds)
{
    S32 sd_surface = -1;
    _LostChanceFX fx_to_play = LCFX_StdShock;
    xSurface* fsurf;
    zSurfaceProps* prop;

    fsurf = zSurfaceGetSurface(&ent->collis->colls[0]);
    if (fsurf && !fsurf->state)
    {
        prop = (zSurfaceProps*)fsurf->moprops;
        sd_surface = prop->asset->surf_type;
        if (prop && prop->asset)
        {
            switch (prop->asset->game_damage_type)
            {
            case 3:
                fx_to_play = LCFX_Water;
                break;
            case 2:
            default:
                fx_to_play = LCFX_StdShock;
                break;
            }
        }
    }

    if (fx_to_play == LCFX_StdShock)
    {
        if (!gPlayerHitWater)
        {
            xVec3* center = xBoundCenter(&ent->bound);
            F32 dx = 0.5f * globals.camera.hi_cam.mat.right.x;
            F32 dz = 0.5f * globals.camera.hi_cam.mat.right.z;

            if (sEmitterShockLeft)
            {
                _zParEmitterCustomSettings info;

                info.custom_flags = 0x20;
                info.pos = *center;
                info.pos.x += dx;
                info.pos.y += 1.0f;
                info.pos.z += dz;
                zParEmitterEmitCustom(sEmitterShockLeft, 1.0f / 30.0f, &info);
            }
            if (sEmitterShockRight)
            {
                _zParEmitterCustomSettings info;

                info.custom_flags = 0x20;
                info.pos = *center;
                info.pos.x -= dx;
                info.pos.y += 1.0f;
                info.pos.z -= dz;
                zParEmitterEmitCustom(sEmitterShockRight, 1.0f / 30.0f, &info);
            }
        }
    }
    else if (fx_to_play == LCFX_Water)
    {
        xVec3* center = xBoundCenter(&ent->bound);
        _zParEmitterCustomSettings info;

        info.custom_flags = 0x20;
        info.pos = *center;
        info.pos.y += 0.1f;
        zParEmitterEmitCustom(sEmitterWaterSplash, 1.0f / 30.0f, &info);
        xSndPlay3D(sSoundWaterSplash, 0.5f, 0.0f, 0, 2, &info.pos, 100.0f);
        if (globals.player.s.TakeDamage)
        {
            gPlayerWaterPos = *center;
            sPlayerWaterOffset = 0.0f;
            gPlayerHitWater = 1;
        }
    }

    if (sd_surface == 0x15 && globals.player.s.TakeDamage)
    {
        gPlayerWaterPos = *xBoundCenter(&ent->bound);
        sPlayerWaterOffset = 0.0f;
        gPlayerHitWater = 1;
    }
}

static void zEntPlayer_FXUpdate(_xEnt* ent, F32 seconds)
{
    xSurface* xsurf;
    zSurfaceProps* surface;
    zSurfAsset* surface_asset;
    S32 i;

    sPlayerSurfaceType = 0;
    xsurf = zSurfaceGetSurface(&ent->collis->colls[0]);
    if (xsurf && !xsurf->state)
    {
        surface = (zSurfaceProps*)xsurf->moprops;
        if (surface)
        {
            surface_asset = surface->asset;
            if (surface_asset)
            {
                sPlayerSurfaceType = surface_asset->surf_type;
            }
        }
    }

    for (i = 0; i < 23; i++)
    {
        if (gPlayerfx.emit_land[i])
        {
            gPlayerfx.emit_land[i]->emit_flags &= ~1;
        }
        if (gPlayerfx.emit_stop[i])
        {
            gPlayerfx.emit_stop[i]->emit_flags &= ~1;
        }
    }
    gPlayerfx.emit_scratch->emit_flags &= ~1;
    gPlayerfx.emit_digup[0]->emit_flags &= ~1;
    gPlayerfx.emit_digup[1]->emit_flags &= ~1;

    if (gPlayerfx.emit_stop[sPlayerSurfaceType] && gPlayerfx.timer_stop > 0.0f)
    {
        if (xEntIsVisible(ent))
        {
            if (!sPlayerCurSkidMark)
            {
                S32 least_edges = 16;
                S32 i;

                for (i = 0; i < 5; i++)
                {
                    if (least_edges > sPlayerSkidMark[i].total_edges - sPlayerSkidMark[i].first_idx)
                    {
                        sPlayerCurSkidMark = &sPlayerSkidMark[i];
                        least_edges = sPlayerSkidMark[i].total_edges - sPlayerSkidMark[i].first_idx;
                    }
                }
                if (!sPlayerCurSkidMark)
                {
                    sPlayerCurSkidMark = &sPlayerSkidMark[0];
                }
                sPlayerCurSkidMark->total_edges = 0;
                sPlayerCurSkidMark->first_idx = 0;
                sPlayerCurSkidMark->sample_timer = 0.0f;
                sPlayerCurSkidMark->remove_timer = 0.0f;
            }
            zFeetAddSkidMark(ent, sPlayerCurSkidMark, seconds);
            zLaughNotify(6);

            zParEmitterAsset* a;

            gPlayerfx.emit_stop[sPlayerSurfaceType]->emit_flags |= 1;
            a = gPlayerfx.emit_stop[sPlayerSurfaceType]->tasset;
            a->pos = ent->frame->mat.pos;
            a->pos.y += 0.1f;
            gPlayerfx.timer_stop -= seconds;
        }
        else
        {
            gPlayerfx.timer_stop = 0.0f;
        }
    }
    else
    {
        sPlayerCurSkidMark = NULL;
    }

    for (i = 0; i < 5; i++)
    {
        zFeetUpdateSkidMark(&sPlayerSkidMark[i], seconds);
    }

    xAnimState* state = globals.player.ent.model->Anim->Single->State;
    if (state && state->ID != xStrHash("Inactive_SitFlea"))
    {
        gPlayerfx.timer_scratch = 0.0f;
    }

    if (gPlayerfx.timer_scratch > 0.0f && xEntIsVisible(ent))
    {
        zParEmitterAsset* a;
        xVec3 paw;

        gPlayerfx.emit_scratch->emit_flags |= 1;
        a = gPlayerfx.emit_scratch->tasset;
        iModelTagEval(ent->model->Data, &globals.player.HangPawTag[2], ent->model->Mat, &paw);
        a->pos = paw;
        gPlayerfx.timer_scratch -= seconds;
    }

    if (gPlayerfx.timer_digup > 0.0f && xEntIsVisible(ent))
    {
        zParEmitterAsset* a;

        zLaughNotify(5);
        gPlayerfx.emit_digup[0]->emit_flags |= 1;
        a = gPlayerfx.emit_digup[0]->tasset;
        a->pos = *(xVec3*)&ent->model->Mat->pos;
        gPlayerfx.emit_digup[1]->emit_flags |= 1;
        a = gPlayerfx.emit_digup[1]->tasset;
        a->pos = *(xVec3*)&ent->model->Mat->pos;
        gPlayerfx.timer_digup -= seconds;
    }
}

static void zEntPlayer_FXSetup(_xEnt* ent, xAnimTable* table)
{
    xAnimState* state;
    S32 i;

    state = xAnimTableGetState(table, "Inactive_SitFlea");
    xAnimStateNewEffect(state, 1, 0.05f, 0.0f, zEntPlayer_FXScratchCB, 0);
    gPlayerfx.timer_scratch = 0.0f;
    state = xAnimTableGetState(table, "Fall_floatopen");
    xAnimStateNewEffect(state, 1, 0.05f, 0.0f, zEntPlayer_FXOpenBrollyCB, 0);
    state = xAnimTableGetState(table, "Action_dig");
    xAnimStateNewEffect(state, 1, 0.05f, 0.0f, zEntPlayer_FXDigupCB, 0);
    gPlayerfx.timer_digup = 0.0f;
    state = xAnimTableGetState(table, "Jump_double");
    xAnimStateNewEffect(state, 1, 0.0f, 0.0f, zEntPlayer_FXSpringCB, 0);
    state = xAnimTableGetState(table, "RunToIdle_normal");
    xAnimStateNewEffect(state, 1, 0.05f, 0.0f, zEntPlayer_SNDSkidCB, 0);
    state = xAnimTableGetState(table, "RunToIdle_scared");
    xAnimStateNewEffect(state, 1, 0.05f, 0.0f, zEntPlayer_SNDSkidCB, 0);
    state = xAnimTableGetState(table, "RunToIdle_brave");
    xAnimStateNewEffect(state, 1, 0.05f, 0.0f, zEntPlayer_SNDSkidCB, 0);
    state = xAnimTableGetState(table, "RunToSkid");
    xAnimStateNewEffect(state, 1, 0.05f, 0.0f, zEntPlayer_SNDSkidCB, 0);

    for (i = 0; i < 5; i++)
    {
        zFeetInitSkidMark(&sPlayerSkidMark[i], 4);
    }
}

static U32 zEntPlayer_FXScratchCB(U32 cbenum, xAnimActiveEffect* acteffect, xAnimSingle* single,
                                  void* object)
{
    gPlayerfx.timer_scratch = 1.65f;
    zRumbleStart(0, SDR_FleaScratch);
    return 0;
}

static U32 zEntPlayer_FXDigupCB(U32 cbenum, xAnimActiveEffect* acteffect, xAnimSingle* single,
                                void* object)
{
    gPlayerfx.timer_digup = 0.3f;
    zRumbleStart(0, SDR_Digging);
    return 0;
}

static U32 zEntPlayer_FXSpringCB(U32 cbenum, xAnimActiveEffect* acteffect, xAnimSingle* single,
                                 void* object)
{
    _zParEmitterCustomSettings info;
    _xEnt* ent;

    if (sEmitterSprings)
    {
        ent = &globals.player.ent;
        info.custom_flags = 0x20;
        iModelTagEval(ent->model->Data, &globals.player.HangPawTag[2], ent->model->Mat,
                      &info.pos);
        info.pos.y += 0.5f;
        zParEmitterEmitCustom(sEmitterSprings, 1.0f / 30.0f, &info);
        iModelTagEval(ent->model->Data, &globals.player.HangPawTag[3], ent->model->Mat,
                      &info.pos);
        info.pos.y += 0.5f;
        zParEmitterEmitCustom(sEmitterSprings, 1.0f / 30.0f, &info);
    }
    return 0;
}

static U32 zEntPlayer_FXOpenBrollyCB(U32 cbenum, xAnimActiveEffect* acteffect, xAnimSingle* single,
                                     void* object)
{
    return 0;
}

static S32 PlayerFloorSurfType()
{
    S32 type = 0;
    xSurface* surf = zSurfaceGetSurface(&globals.player.ent.collis->colls[0]);
    if (surf && !surf->state && surf->moprops && ((zSurfaceProps*)surf->moprops)->asset)
    {
        type = ((zSurfaceProps*)surf->moprops)->asset->surf_type;
    }
    return type;
}

static U32 zEntPlayer_SNDSkidCB(U32 cbenum, xAnimActiveEffect* acteffect, xAnimSingle* single,
                                void* object)
{
    F32 pitch;
    S32 newsnd;
    static S32 snd = 0;
    static S16 pitch_angle = 0;

    pitch_angle = zPitchChange(pitch_angle, &pitch, 30, 1.5f);
    xSndPlay(sPlayerSndfx.skid_id[snd], 0.5f, 0.5f, pitch, 0x80, 2, 0);
    newsnd = snd;
    while (newsnd == snd)
    {
        newsnd = rand() % 3;
    }
    snd = newsnd;

    if (PlayerFloorSurfType() == 0x16)
    {
        gPlayerfx.timer_stop = 0.0f;
    }
    else
    {
        gPlayerfx.timer_stop = 0.5f;
    }
    zRumbleStart(0, SDR_ScreechingStop);
    return 0;
}

static void zEntPlayer_ButtonBBFX(_xEnt* ent, _zEntButton*)
{
    _zParEmitterCustomSettings info;

    if (sEmitterBounceOnVillain)
    {
        info.custom_flags = 0x20;
        info.pos.x = ((xMat4x3*)ent->model->Mat)->pos.x;
        info.pos.y = ((xMat4x3*)ent->model->Mat)->pos.y - 1.0f;
        info.pos.z = ((xMat4x3*)ent->model->Mat)->pos.z;
        switch (ent->bound.type)
        {
        case 0:
            break;
        case 1:
            info.pos.x = ent->bound.sph.center.x;
            info.pos.y = ent->bound.sph.center.y;
            info.pos.z = ent->bound.sph.center.z;
            break;
        case 2:
            info.pos.y += ent->bound.box.box.upper.y - ent->bound.box.box.lower.y;
            break;
        case 3:
            info.pos.y += ent->bound.cyl.h;
            break;
        case 4:
            break;
        }
        zParEmitterEmitCustom(sEmitterBounceOnVillain, 1.0f / 30.0f, &info);
    }
}

static void zEntPlayer_ButtonHBFX(_xEnt*, _zEntButton*)
{
    xVec3 angles;
    _xEnt* player_ent;
    _zParEmitterCustomSettings info;

    if (sEmitterHeadButt && sEmitterHeadButt2)
    {
        info.custom_flags = 0x20;
        player_ent = &globals.player.ent;
        info.pos.x = ((xMat4x3*)player_ent->model->Mat)->pos.x + ((xMat4x3*)player_ent->model->Mat)->at.x;
        info.pos.y = 1.1f + ((xMat4x3*)player_ent->model->Mat)->pos.y;
        info.pos.z = ((xMat4x3*)player_ent->model->Mat)->pos.z + ((xMat4x3*)player_ent->model->Mat)->at.z;
        info.custom_flags |= 0x10000;
        xMat3x3GetEuler((xMat3x3*)player_ent->model->Mat, &angles);
        info.rot[0] = 255.0f * (angles.x / (2.0f * PI));
        info.rot[1] = 0x40;
        info.rot[2] = 255.0f * (angles.z / (2.0f * PI));
        zParEmitterEmitCustom(sEmitterHeadButt, 1.0f / 30.0f, &info);
        info.rot[0] += 0x40;
        zParEmitterEmitCustom(sEmitterHeadButt2, 1.0f / 30.0f, &info);
    }
}

void zEntPlayer_InitCutsceneModelHack()
{
    sPlayerBodyAtomic = (RpAtomic*)xSTFindAsset(0xF2D545DF, NULL);
    sPlayerHeadAtomic = (RpAtomic*)xSTFindAsset(0xB2E286A1, NULL);
    sPlayerBodyClump = sPlayerBodyAtomic->clump;
    sPlayerHeadClump = sPlayerHeadAtomic->clump;
    RpClumpRemoveAtomic(sPlayerHeadClump, sPlayerHeadAtomic);
    RpClumpAddAtomic(sPlayerBodyClump, sPlayerHeadAtomic);
    sPlayerBodyAtomic->worldBoundingSphere.radius = 1.9f;
    sPlayerBodyAtomic->boundingSphere.radius = 1.9f;
    sPlayerBodyAtomic->geometry->morphTarget->boundingSphere.radius = 1.9f;
    sPlayerHeadAtomic->worldBoundingSphere.radius = 1.9f;
    sPlayerHeadAtomic->boundingSphere.radius = 1.9f;
    sPlayerHeadAtomic->geometry->morphTarget->boundingSphere.radius = 1.9f;
}
