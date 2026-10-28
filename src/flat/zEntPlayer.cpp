#include "zEntPlayer.h"
#include "zGlobals.h"
#include "zEntHangable.h"
#include "zEntShaggy.h"
#include "zEntBubble.h"
#include "zParEmitter.h"
#include "zRumble.h"
#include "zLaugh.h"
#include "zPitchChange.h"
#include "xPad.h"
#include "xSnd.h"
#include "iSnd.h"
#include "xString.h"
#include "xMath.h"
#include "xMath3.h"
#include "iMath.h"

#include <math.h>
#include <stdlib.h>

struct st_zEntVillain;

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
S32 Shaggy8_HBCallback(_zEnt* ent, _xCollis* coll);

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
F32 startJump;
F32 startDouble;
F32 startBounce;
static U32 surfSticky;
static F32 surfSlickTimer;
S32 headcoll_anim;
F32 headcoll_tmr;
F32 headcoll_bltime;
static U32 sVOAssetID;
static U32 sVOPriority;
static zPlayerSituation* sVONextSituation;
static F32 sVOTimer;

static _tagPlayerSndFX sPlayerSndfx;
xVec3 vHangBefore;
xVec3 vHangFirst;
_zParEmitterCustomSettings g_info;

static void zEntPlayer_FXLostChance(_xEnt* ent, F32 seconds);
void zEntPlayer_Notify(S32 situation);
void zEntPlayerJumpStart(_xEnt* ent, zJumpParam* jump);

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
    F32 sinr;
    F32 cosr;

    if ((globals.player.ent.model->Anim->Single->State->UserFlags & 0x1E) != 0xC)
    {
        return;
    }
    sinr = sin(globals.player.ent.frame->rot.angle);
    cosr = cos(globals.player.ent.frame->rot.angle);
    for (; coll < endcoll; coll++)
    {
        if (fabsf(coll->hdng.z) < 0.7071f)
        {
            _xEnt* hit = (_xEnt*)coll->optr;
            F32 dx = hit->frame->mat.pos.x - ((xMat4x3*)globals.player.ent.model->Mat)->pos.x;
            F32 dz = hit->frame->mat.pos.z - ((xMat4x3*)globals.player.ent.model->Mat)->pos.z;
            F32 dist = sqrtf(dx * dx + dz * dz);
            if (dist > 0.0000001f && sinr * dx + cosr * dz > angle_cosine * dist)
            {
                if (headbuttCB((_zEnt*)&globals.player.ent, coll))
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
           !(globals.player.SlideTimer > 0.0f) && 0.0f == surfSlickTimer;
}

static U32 SlipperyCheck(xAnimTransition* tran, xAnimSingle* anim, void* param_3)
{
    return globals.player.Speed == 0 && globals.player.Slide == 0 && 0.0f != surfSlickTimer;
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
    if (0.0f != surfSlickTimer)
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
    return (globals.player.Speed == 0 && globals.player.Slide == 0 && 0.0f == surfSlickTimer &&
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
    return 0.0f != globals.player.ScareTimer && HealthCheck(tran, anim, param_3);
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
    zRumbleStart(0, (_tagSDRumbleType)7);
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
    globals.player.HeadbuttTimer = 0.00000001f;
    globals.player.HeadbuttVel = globals.player.s.HeadbuttSpeedMin;
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

void zEntPlayer_Notify(S32 situation)
{
    zPlayerSituation* s;

    if (globals.player.ControlOn)
    {
        s = &sPlayerInfo[situation];
        if ((!sVONextSituation || sVONextSituation->priority < s->priority) &&
            (!iSndIsPlaying(sVOAssetID) || s->priority > sVOPriority) &&
            (!s->countMax || s->count < s->countMax) && s->delay <= s->elapsedTime)
        {
            sVONextSituation = s;
            sVOTimer = s->punchDelay;
        }
    }
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
