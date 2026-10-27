#include "zEntShaggy.h"
#include "zGlobals.h"
#include "xEvent.h"
#include "xModel.h"
#include "xString.h"
#include "xMath.h"
#include "iMath.h"
#include "zEntHangable.h"
#include "zMovePoint.h"
#include "zEntVillain.h"
#include "zPlatform.h"
#include "zRumble.h"
#include "zPitchChange.h"
#include "xSnd.h"
#include "xMovePoint.h"
#include "zParEmitter.h"
#include "iModel.h"
#include "xScene.h"
#include "xFFX.h"
#include "zScene.h"
#include "zEntPickup.h"
#include "zEntTrigger.h"
#include "zVolume.h"
#include "iAnim.h"
#include "iAnimSKB.h"
#include <stdio.h>
#include <string.h>
#include <math.h>

ShaggyGuts gShaggy[16];
static iModelTag ChimneyTag;
static iModelTag ShaggyTag;
U32 gNumShaggy;
F32 gACCEL_START;
F32 gACCEL_END;
F32 THROW_MINHEIGHT = 4.25f;
F32 THROW_DEFHEIGHT = 6.0f;
static U32 ChimneyTagIndex = 27;
static U32 ShaggyTagIndex = 397;
static F32 gShaggy3TurnAngle;
static _zParEmitter* gTractorEmitter;
static _zParEmitter* gBurningManEmitter;
static S32 (*sVilEventFunc)(xBase*, xBase*, U32, F32*, xBase*);
static void (*sVilUpdateFunc)(_xEnt*, _xScene*, F32);
static void (*sVilMoveFunc)(_xEnt*, _xScene*, F32, _xEntFrame*);
F32 sHangVirtualPeak = 0.75f + THROW_DEFHEIGHT;
F32 sDefHangTime;
static ShaggyMagnet sMagnetList[64];
static S32 sMagnetCount;
static S16 sSndCallPitch;

static void ShaggySwap(_zEnt* src, _zEnt* dest);

static inline F32 Shaggy1_CalcThrowAccel(F32 peak, F32* timepeak);
static U32 Shaggy5_Action(xAnimTransition* tran, xAnimSingle* anim, _zEnt* ent);
static U32 Shaggy5_ActionCB(xAnimTransition* tran, xAnimSingle* anim, _zEnt* ent);

static inline ShaggyGuts* zEntShaggy_FindGuts(_zEnt* ent)
{
    for (U32 i = 0; i < gNumShaggy; i++)
    {
        if (gShaggy[i].ent == ent)
        {
            return &gShaggy[i];
        }
    }
    return NULL;
}

U32 zEntIsShaggy(_zEnt* ent)
{
    return ent->baseType == 2 && ent->subType >= 0x1C && ent->subType <= 0x25;
}

F32 zEntShaggy_GetSpin(_zEnt* ent)
{
    ShaggyGuts* guts = zEntShaggy_FindGuts(ent);

    if (guts)
    {
        return guts->hangspin;
    }
    return 0.0f;
}

void zEntShaggy_SetSpin(_zEnt* ent, F32 spin)
{
    ShaggyGuts* guts = zEntShaggy_FindGuts(ent);

    if (guts)
    {
        guts->hangspin = spin;
    }
}

_zEnt* zEntShaggy_GetSwap(_zEnt* ent)
{
    ShaggyGuts* guts = zEntShaggy_FindGuts(ent);

    if (guts)
    {
        return guts->swap;
    }
    return NULL;
}

static U32 Shaggy1_Grab(xAnimTransition* tran, xAnimSingle* anim, _zEnt* ent)
{
    return globals.player.Shaggy1.GrabTry &&
           (ent == &globals.player.ent || ent == globals.player.Shaggy1.GrabTry) &&
           globals.player.Shaggy1.GrabTry->subType == tran->UserFlags + 0x1C &&
           !globals.player.ScareTimer;
}

static U32 Shaggy1_GrabCB(xAnimTransition* tran, xAnimSingle* anim, _zEnt* ent)
{
    globals.player.Shaggy1.Grab = globals.player.Shaggy1.GrabTry;
    globals.player.Shaggy1.SetDown = 0;
    globals.player.Shaggy1.ThrowState = 0;
    zEntEvent(ent, eEventShaggyGrabbed);
    return 0;
}

static U32 Shaggy1_Walk(xAnimTransition* tran, xAnimSingle* anim, _zEnt* ent)
{
    return globals.player.Speed > 0 && !globals.player.Shaggy1.SetDown &&
           !globals.player.Shaggy1.ThrowState;
}

static U32 Shaggy1_Stop(xAnimTransition* tran, xAnimSingle* anim, _zEnt* ent)
{
    return globals.player.Speed == 0 || globals.player.Shaggy1.SetDown ||
           globals.player.Shaggy1.ThrowState;
}

static U32 Shaggy1_Throw(xAnimTransition* tran, xAnimSingle* anim, _zEnt* ent)
{
    return globals.player.Shaggy1.ThrowState == 3;
}

static U32 Shaggy1_ThrowCB(xAnimTransition* tran, xAnimSingle* anim, _zEnt* ent)
{
    globals.player.Shaggy1.ThrowState = 0;
    if (ent->subType == 0x1D)
    {
        globals.player.Shaggy1.ThrowTimer = 0.0f;
    }
    globals.player.Shaggy1.CatchYou = 0;
    zEntEvent(ent, eEventShaggyThrown);
    return 0;
}

static U32 Shaggy1_ThrowEndCB(xAnimTransition* tran, xAnimSingle* anim, _zEnt* ent)
{
    globals.player.Shaggy1.ThrowEnded = 1;
    globals.player.Shaggy1.CatchMe = 0;
    return 0;
}

static U32 Shaggy1_Land(xAnimTransition* tran, xAnimSingle* anim, _zEnt* ent)
{
    return globals.player.Shaggy1.Grab && 0.0f == globals.player.Shaggy1.Grab->frame->vel.x &&
           0.0f == globals.player.Shaggy1.Grab->frame->vel.y &&
           0.0f == globals.player.Shaggy1.Grab->frame->vel.z;
}

static U32 Shaggy1_Land2(xAnimTransition* tran, xAnimSingle* anim, _zEnt* ent)
{
    return 0.0f == ent->frame->vel.x && 0.0f == ent->frame->vel.y && 0.0f == ent->frame->vel.z;
}

static U32 Shaggy1_LandCB(xAnimTransition* tran, xAnimSingle* anim, _zEnt* ent)
{
    if (ent == globals.player.Shaggy1.Grab)
    {
        globals.player.Shaggy1.Grab = NULL;
    }
    return 0;
}

static U32 Shaggy1_Catch(xAnimTransition* tran, xAnimSingle* anim, _zEnt* ent)
{
    return globals.player.Shaggy1.CatchMe &&
           (ent == globals.player.Shaggy1.Grab || ent == &globals.player.ent);
}

static U32 Shaggy1_CatchCB(xAnimTransition* tran, xAnimSingle* anim, _zEnt* ent)
{
    zEntEvent(ent, eEventShaggyGrabbed);
    globals.player.Shaggy1.CatchHeight =
        ((xMat4x3*)ent->model->Mat)->pos.y - ((xMat4x3*)globals.player.ent.model->Mat)->pos.y;
    globals.player.Shaggy1.CatchVel = ent->frame->vel.y;
    if (globals.player.Shaggy1.CatchVel > -8.0f)
    {
        globals.player.Shaggy1.CatchVel = -8.0f;
    }
    return 0;
}

static U32 Shaggy1_Fall(xAnimTransition* tran, xAnimSingle* anim, _zEnt* ent)
{
    return ent->frame->vel.y < 0.0f;
}

static U32 Shaggy1_FallCB(xAnimTransition* tran, xAnimSingle* anim, _zEnt* ent)
{
    if (ent == globals.player.Shaggy1.Grab &&
        strcmp(globals.player.ent.model->Anim->Single->State->Name, "SD1_ThrowIdle") &&
        strcmp(globals.player.ent.model->Anim->Single->State->Name, "SD1_ThrowWalk"))
    {
        globals.player.Shaggy1.Grab = NULL;
    }
    return 0;
}

static U32 Shaggy1_Hang(xAnimTransition* tran, xAnimSingle* anim, _zEnt* ent)
{
    return globals.player.Shaggy1.Grab == NULL;
}

static U32 Shaggy1_HangSwing(xAnimTransition* tran, xAnimSingle* anim, _zEnt* ent)
{
    return zEntShaggy_FindGuts(ent)->hangme == globals.player.HangEnt;
}

static U32 Shaggy1_HangNoSwing(xAnimTransition* tran, xAnimSingle* anim, _zEnt* ent)
{
    return zEntShaggy_FindGuts(ent)->hangme != globals.player.HangEnt;
}

static U32 Shaggy1_LetGoShark(xAnimTransition* tran, xAnimSingle* anim, _zEnt* ent);

static U32 Shaggy1_LetGo(xAnimTransition* tran, xAnimSingle* anim, _zEnt* ent)
{
    if (!xStricmp(globals.player.ent.model->Anim->Single->State->Name, "Buttsmash_land"))
    {
        return 1;
    }
    return Shaggy1_LetGoShark(tran, anim, ent);
}

static U32 Shaggy1_LetGoShark(xAnimTransition* tran, xAnimSingle* anim, _zEnt* ent)
{
    ShaggyGuts* guts = zEntShaggy_FindGuts(ent);

    return !guts->hangme || guts->hangme->enabled <= 0;
}

static U32 Shaggy1_LetGoCB(xAnimTransition* tran, xAnimSingle* anim, _zEnt* ent)
{
    ShaggyGuts* guts = zEntShaggy_FindGuts(ent);

    ent->frame->vel.y = 0.0f;
    if (guts->hangme)
    {
        zEntHangable_SetShaggy(guts->hangme, NULL);
        guts->hangme = NULL;
    }
    return 0;
}

static U32 Shaggy1_Scare(xAnimTransition* tran, xAnimSingle* anim, _zEnt* ent)
{
    U8 scare = 1;

    if (0.0f == globals.player.ScareTimer &&
        !strstr(globals.player.ent.model->Anim->Single->State->Name, "Scare_"))
    {
        scare = 0;
    }
    return scare;
}

static U32 Shaggy1_ScareCB(xAnimTransition* tran, xAnimSingle* anim, _zEnt* ent)
{
    globals.player.Shaggy1.ThrowAccel = Shaggy1_CalcThrowAccel(THROW_MINHEIGHT, NULL);
    globals.player.Shaggy1.ThrowTgtSpeed = 0.0f;
    globals.player.Shaggy1.HangTarget = NULL;
    globals.player.Shaggy1.ThrowState = 0;
    if (ent->subType == 0x1D)
    {
        globals.player.Shaggy1.ThrowTimer = 0.0f;
    }
    globals.player.Shaggy1.CatchYou = 0;
    zEntEvent(ent, eEventShaggyThrown);
    return 0;
}

static U32 Shaggy1_IdleResetCB(xAnimTransition* tran, xAnimSingle* anim, _zEnt* ent)
{
    zEntShaggy_FindGuts(ent)->idleTimer = 0.0f;
    return 0;
}

static U32 Shaggy1_IdlePick(xAnimTransition* tran, xAnimSingle* anim, _zEnt* ent)
{
    ShaggyGuts* guts = zEntShaggy_FindGuts(ent);

    return !guts->swap && guts->idleTimer > tran->UserFlags &&
           guts->idleTimer - 0.5f < tran->UserFlags;
}

static U32 Shaggy1_IdleLeave(xAnimTransition* tran, xAnimSingle* anim, _zEnt* ent)
{
    return zEntShaggy_FindGuts(ent)->swap != NULL;
}

static U32 Shaggy1_EventCB(xAnimTransition* tran, xAnimSingle* anim, _zEnt* ent)
{
    zEntEvent(ent, tran->UserFlags);
    return 0;
}

static U32 Shaggy1_SetDown(xAnimTransition* tran, xAnimSingle* anim, _zEnt* ent)
{
    return globals.player.Shaggy1.SetDown && !globals.player.ent.model->Anim->Single->Blend->State;
}

static U32 Shaggy1_PlyrSet(xAnimTransition* tran, xAnimSingle* anim, _zEnt* ent)
{
    return !xStricmp(globals.player.ent.model->Anim->Single->State->Name, "SD1_SetDown");
}

static U32 Shaggy1_EndSetDownCB(xAnimTransition* tran, xAnimSingle* anim, _zEnt* ent)
{
    zEntShaggy_FindGuts(ent)->idleTimer = 0.0f;
    ent->frame->vel.x = 0.0f;
    ent->frame->vel.y = 0.0f;
    ent->frame->vel.z = 0.0f;
    globals.player.Shaggy1.Grab = NULL;
    xMat3x3LMulRotY((xMat3x3*)ent->model->Mat, (xMat3x3*)ent->model->Mat, 3.1415927f);
    return 0;
}

static U32 Shaggy1_Turn(xAnimTransition* tran, xAnimSingle* anim, _zEnt* ent)
{
    if (anim->Blend->State)
    {
        return 0;
    }
    return zEntShaggy_FindGuts(ent)->facept != NULL;
}

static U32 Shaggy1_TurnCB(xAnimTransition* tran, xAnimSingle* anim, _zEnt* ent)
{
    ShaggyGuts* guts = zEntShaggy_FindGuts(ent);
    F32 angsrc = xAngleClamp(atan2(ent->frame->mat.at.x, ent->frame->mat.at.z));
    F32 angdst = xAngleClamp(atan2(guts->facept->pos->x - ent->frame->mat.pos.x,
                                   guts->facept->pos->z - ent->frame->mat.pos.z));

    angdst -= angsrc;
    if (angdst > 3.1415927f)
    {
        angdst -= 6.2831855f;
    }
    else if (angdst < -3.1415927f)
    {
        angdst += 6.2831855f;
    }
    guts->facerot = angdst;
    ent->frame->rot.axis.x = 0.0f;
    ent->frame->rot.axis.y = 1.0f;
    ent->frame->rot.axis.z = 0.0f;
    ent->frame->rot.angle = angsrc;
    return 0;
}

static U32 Shaggy1_Noturn(xAnimTransition* tran, xAnimSingle* anim, _zEnt* ent)
{
    return 0.0f == zEntShaggy_FindGuts(ent)->facerot;
}

static U32 Shaggy1_ScareReact(xAnimTransition* tran, xAnimSingle* anim, _zEnt* ent)
{
    return globals.player.ScareShaggy && globals.player.Health;
}

static U32 Shaggy1_DeathReact(xAnimTransition* tran, xAnimSingle* anim, _zEnt* ent)
{
    return globals.player.Health == 0;
}

static U32 Shaggy1_FallDeath(xAnimTransition* tran, xAnimSingle* anim, _zEnt* ent)
{
    return 0.0f != globals.player.FallDeathTimer;
}

static U32 Shaggy1_FallDeathCB(xAnimTransition* tran, xAnimSingle* anim, _zEnt* ent)
{
    ent->collis->chk = 0;
    ent->collis->pen = 0;
    ent->chkby = 0;
    ent->penby = 0;
    return 0;
}

static U32 Shaggy1_Talk(xAnimTransition* tran, xAnimSingle* anim, _zEnt* ent)
{
    return 0.0f != zEntShaggy_FindGuts(ent)->talkTimer;
}

static U32 Shaggy1_TalkFlag(xAnimTransition* tran, xAnimSingle* anim, _zEnt* ent)
{
    ShaggyGuts* guts = zEntShaggy_FindGuts(ent);

    return 0.0f != guts->talkTimer && guts->talkSwitch <= 0.0f &&
           guts->talkRand % (tran->UserFlags & 0xFF) == 0 &&
           guts->talkFlag == (tran->UserFlags & 0xFF) >> 8;
}

static U32 Shaggy1_TalkChoose(xAnimTransition* tran, xAnimSingle* anim, _zEnt* ent)
{
    ShaggyGuts* guts = zEntShaggy_FindGuts(ent);

    return 0.0f != guts->talkTimer && guts->talkSwitch <= 0.0f &&
           guts->talkRand % (tran->UserFlags & 0xFF) == 0;
}

static U32 Shaggy1_TalkChooseCB(xAnimTransition* tran, xAnimSingle* anim, _zEnt* ent)
{
    U32 minT = (tran->UserFlags >> 20) & 0xF;
    U32 maxT = (tran->UserFlags >> 16) & 0xF;
    ShaggyGuts* guts = zEntShaggy_FindGuts(ent);

    guts->talkRand = irand() >> 16;
    guts->talkSwitch = (maxT - minT) * iurand() + minT;
    return 0;
}

static U32 Shaggy1_Notalk(xAnimTransition* tran, xAnimSingle* anim, _zEnt* ent)
{
    return 0.0f == zEntShaggy_FindGuts(ent)->talkTimer;
}

static U32 Shaggy1_NotalkCB(xAnimTransition* tran, xAnimSingle* anim, _zEnt* ent)
{
    zEntShaggy_FindGuts(ent)->talkSwitch = -1.0f;
    zEntShaggy_FindGuts(ent)->idleTimer = 0.0f;
    return 0;
}

static U32 Shaggy0_Throw(xAnimTransition* tran, xAnimSingle* anim, _zEnt* ent)
{
    return (globals.player.Shaggy1.SetDown &&
            (ent == &globals.player.ent ||
             !xStricmp(globals.player.ent.model->Anim->Single->State->Name, "SD0_Throw"))) ||
           globals.player.Shaggy1.ThrowState == 3;
}

static U32 Shaggy0_Catch(xAnimTransition* tran, xAnimSingle* anim, _zEnt* ent)
{
    return globals.player.Shaggy1.CatchYou;
}

static U32 Shaggy0_CatchCB(xAnimTransition* tran, xAnimSingle* anim, _zEnt* ent)
{
    globals.player.Shaggy1.CatchYou = 0;
    return 0;
}

static U32 Shaggy0_PlayerLanded(xAnimTransition* tran, xAnimSingle* anim, _zEnt* ent)
{
    ShaggyGuts* guts = zEntShaggy_FindGuts(ent);

    return (globals.player.JumpState == 0 &&
            !(globals.player.ent.model->Anim->Single->State->UserFlags & 0xA) &&
            !(globals.player.ent.model->Anim->Single->State->UserFlags & 0x800)) ||
           globals.player.HangEnt || (globals.player.Shaggy1.SetDown && anim->Time > 0.1f) ||
           !globals.player.Shaggy1.GrabOn || guts->swap;
}

U32 Shaggy0_FlipCB(xAnimTransition* tran, xAnimSingle* anim, _zEnt* ent)
{
    xMat3x3LMulRotY((xMat3x3*)ent->model->Mat, (xMat3x3*)ent->model->Mat, 3.1415927f);
    xMat3x3LMulRotY((xMat3x3*)ent->frame, (xMat3x3*)ent->frame, 3.1415927f);
    if (ent == &globals.player.ent)
    {
        ent->frame->rot.angle += 3.1415927f;
    }
    return 0;
}

static U32 Shaggy0_PlayerCaught(xAnimTransition* tran, xAnimSingle* anim, _zEnt* ent)
{
    return ent == globals.player.Shaggy1.Grab &&
           !xStricmp(globals.player.ent.model->Anim->Single->State->Name, "SD0_CatchThrow");
}

static U32 Shaggy0_CarryHitCB(xAnimTransition* tran, xAnimSingle* anim, _zEnt* ent)
{
    if (ent == globals.player.Shaggy1.Grab)
    {
        globals.player.Shaggy1.Grab = NULL;
    }
    zEntShaggy_FindGuts(ent)->idleTimer = 0.0f;
    return 0;
}

void Shaggy1_PlayerStates(xAnimTable* table)
{
    xAnimTableNewState(table, "SD1_Hold", 0x10, 0, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "SD1_HoldWalk", 0x10, 4, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "SD1_CatchJump", 0x20, 0x80, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "SD1_CatchThrow", 0x20, 0x80, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "SD1_Throw", 0x20, 0x80, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "SD1_ThrowIdle", 0x10, 0x80, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "SD1_ThrowWalk", 0x10, 0x804, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "SD1_HangReact", 0x20, 0, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "SD1_LandReact", 0x20, 0, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "SD1_SetDown", 0x20, 0x80, 1.0f, NULL, NULL, 0.0f, NULL);
}

xAnimTable* zEntShaggy_Anim1()
{
    xAnimTable* table;

    table = xAnimTableNew("Shaggy1", NULL);
    xAnimTableNewState(table, "Idle", 0x10, 0, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "Idle2", 0x10, 0, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "Idle3", 0x10, 0, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "Fall", 0x10, 0, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "Apex", 0x20, 0, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "Hang", 0x10, 0, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "HangTran", 0x20, 0, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "HangSwing", 0x10, 0, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "Hold", 0x10, 0x1000, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "HoldWalk", 0x10, 0x1000, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "CatchJump", 0x20, 0x1000, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "CatchThrow", 0x20, 0x1000, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "Throw", 0x20, 0, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "ThrowIdle", 0x10, 0, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "Land", 0x20, 0, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "SetDown", 0x20, 0, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "Turn", 0x10, 0, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "React_scare", 0x20, 0, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "React_death", 0, 0, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "Talk", 0x10, 0, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "Talk4", 0x10, 0, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "SharkControls", 0x20, 0, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "SharkHang", 0x10, 0, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "SharkHang2", 0x10, 0, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "SharkHang3", 0x10, 0, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "SharkSwing", 0x10, 0, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "SharkTalk", 0x10, 0, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "SD1_Hold", 0x10, 0, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "SD1_HoldWalk", 0x10, 4, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "SD1_CatchJump", 0x20, 0x80, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "SD1_CatchThrow", 0x20, 0x80, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "SD1_Throw", 0x20, 0x80, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "SD1_ThrowIdle", 0x10, 0x80, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "SD1_ThrowWalk", 0x10, 0x804, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "SD1_HangReact", 0x20, 0, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "SD1_LandReact", 0x20, 0, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "SD1_SetDown", 0x20, 0x80, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewTransition(table, "CatchJump CatchThrow", "Hold", NULL, NULL, 0x10, 0, 0.0f, 0.0f,
                            0, 0, 0.0f, NULL);
    xAnimTableNewTransition(table, "Throw", "ThrowIdle", NULL,
                            (xAnimTransitionCallback)Shaggy1_ThrowEndCB, 0x10, 0, 0.0f, 0.0f, 0, 0,
                            0.0f, NULL);
    xAnimTableNewTransition(table, "Land SharkControls", "Idle", NULL,
                            (xAnimTransitionCallback)Shaggy1_IdleResetCB, 0x10, 0, 0.0f, 0.0f, 0, 0,
                            0.0f, NULL);
    xAnimTableNewTransition(table, "React_scare", "Idle", NULL,
                            (xAnimTransitionCallback)Shaggy1_IdleResetCB, 0x10, 0, 0.0f, 0.0f, 0, 0,
                            0.15f, NULL);
    xAnimTableNewTransition(table, "HangTran", "Hang", NULL, NULL, 0x10, 0, 0.0f, 0.0f, 0, 0, 0.0f,
                            NULL);
    xAnimTableNewTransition(table, "Apex", "Fall", NULL, NULL, 0x10, 0, 0.0f, 0.0f, 0, 0, 0.0f,
                            NULL);
    xAnimTableNewTransition(table, "SetDown", "Idle", NULL,
                            (xAnimTransitionCallback)Shaggy1_EndSetDownCB, 0x10, 0, 0.0f, 0.0f, 0,
                            0, 0.0f, NULL);
    xAnimTableNewTransition(
        table, "Idle Idle2 Idle3", "CatchJump", (xAnimTransitionConditionalCallback)Shaggy1_Grab,
        (xAnimTransitionCallback)Shaggy1_GrabCB, 0, 1, 0.0f, 0.0f, 10, 0, 0.0f, NULL);
    xAnimTableNewTransition(table, "Idle Idle2 Idle3", "SharkControls",
                            (xAnimTransitionConditionalCallback)Shaggy5_Action,
                            (xAnimTransitionCallback)Shaggy5_ActionCB, 0, 1, 0.0f, 0.0f, 9, 0,
                            0.15f, NULL);
    xAnimTableNewTransition(
        table, "Idle Idle2 Idle3", "Turn", (xAnimTransitionConditionalCallback)Shaggy1_Turn,
        (xAnimTransitionCallback)Shaggy1_TurnCB, 0, 0, 0.0f, 0.0f, 4, 0, 0.15f, NULL);
    xAnimTableNewTransition(table, "Hold", "HoldWalk",
                            (xAnimTransitionConditionalCallback)Shaggy1_Walk, NULL, 0, 0, 0.0f,
                            0.0f, 1, 0, 0.15f, NULL);
    xAnimTableNewTransition(table, "HoldWalk", "Hold",
                            (xAnimTransitionConditionalCallback)Shaggy1_Stop, NULL, 0, 0, 0.0f,
                            0.0f, 1, 0, 0.15f, NULL);
    xAnimTableNewTransition(
        table, "Hold HoldWalk", "Throw", (xAnimTransitionConditionalCallback)Shaggy1_Throw,
        (xAnimTransitionCallback)Shaggy1_ThrowCB, 0, 0, 0.0f, 0.0f, 1, 0, 0.15f, NULL);
    xAnimTableNewTransition(table, "Fall Apex", "Land",
                            (xAnimTransitionConditionalCallback)Shaggy1_Land2, NULL, 0, 0, 0.0f,
                            0.0f, 2, 0, 0.0f, NULL);
    xAnimTableNewTransition(
        table, "Fall Apex", "Land", (xAnimTransitionConditionalCallback)Shaggy1_Land,
        (xAnimTransitionCallback)Shaggy1_LandCB, 0, 0, 0.0f, 0.0f, 3, 0, 0.0f, NULL);
    xAnimTableNewTransition(
        table, "ThrowIdle", "Apex", (xAnimTransitionConditionalCallback)Shaggy1_Fall,
        (xAnimTransitionCallback)Shaggy1_FallCB, 0, 0, 0.0f, 0.0f, 1, 0, 0.0f, NULL);
    xAnimTableNewTransition(
        table, "Fall Apex", "CatchThrow", (xAnimTransitionConditionalCallback)Shaggy1_Catch,
        (xAnimTransitionCallback)Shaggy1_CatchCB, 0, 0, 0.0f, 0.0f, 5, 0, 0.15f, NULL);
    xAnimTableNewTransition(table, "ThrowIdle", "HangTran",
                            (xAnimTransitionConditionalCallback)Shaggy1_Hang, NULL, 0, 0, 0.0f,
                            0.0f, 5, 0, 0.0f, NULL);
    xAnimTableNewTransition(table, "Hang", "HangSwing",
                            (xAnimTransitionConditionalCallback)Shaggy1_HangSwing, NULL, 0, 0, 0.0f,
                            0.0f, 1, 0, 0.15f, NULL);
    xAnimTableNewTransition(table, "HangSwing", "Hang",
                            (xAnimTransitionConditionalCallback)Shaggy1_HangNoSwing, NULL, 0, 0,
                            0.0f, 0.0f, 1, 0, 0.15f, NULL);
    xAnimTableNewTransition(
        table, "Hang", "Fall", (xAnimTransitionConditionalCallback)Shaggy1_LetGo,
        (xAnimTransitionCallback)Shaggy1_LetGoCB, 0, 0, 0.0f, 0.0f, 2, 0, 0.15f, NULL);
    xAnimTableNewTransition(
        table, "Hold HoldWalk", "Throw", (xAnimTransitionConditionalCallback)Shaggy1_Scare,
        (xAnimTransitionCallback)Shaggy1_ScareCB, 0, 0, 0.0f, 0.0f, 6, 0, 0.15f, NULL);
    xAnimTableNewTransition(
        table, "Hold", "SetDown", (xAnimTransitionConditionalCallback)Shaggy1_PlyrSet,
        (xAnimTransitionCallback)Shaggy1_EventCB, 0, 0xe9, 0.0f, 0.0f, 11, 0, 0.0f, NULL);
    xAnimTableNewTransition(
        table, "Turn", "Idle", (xAnimTransitionConditionalCallback)Shaggy1_Noturn,
        (xAnimTransitionCallback)Shaggy1_IdleResetCB, 0, 0, 0.0f, 0.0f, 1, 0, 0.15f, NULL);
    xAnimTableNewTransition(table, "SharkHang SharkHang2 SharkHang3 SharkTalk", "Fall",
                            (xAnimTransitionConditionalCallback)Shaggy1_LetGoShark,
                            (xAnimTransitionCallback)Shaggy1_LetGoCB, 0, 0, 0.0f, 0.0f, 2, 0, 0.15f,
                            NULL);
    xAnimTableNewTransition(table, "SharkHang SharkHang2 SharkHang3 SharkTalk", "SharkSwing",
                            (xAnimTransitionConditionalCallback)Shaggy1_HangSwing, NULL, 0, 0, 0.0f,
                            0.0f, 1, 0, 0.15f, NULL);
    xAnimTableNewTransition(
        table, "SharkSwing", "SharkHang", (xAnimTransitionConditionalCallback)Shaggy1_HangNoSwing,
        (xAnimTransitionCallback)Shaggy1_IdleResetCB, 0, 0, 0.0f, 0.0f, 1, 0, 0.15f, NULL);
    xAnimTableNewTransition(table, "SharkHang SharkHang2 SharkHang3", "SharkTalk",
                            (xAnimTransitionConditionalCallback)Shaggy1_Talk, NULL, 0, 0, 0.0f,
                            0.0f, 1, 0, 0.15f, NULL);
    xAnimTableNewTransition(
        table, "SharkTalk", "SharkHang", (xAnimTransitionConditionalCallback)Shaggy1_Notalk,
        (xAnimTransitionCallback)Shaggy1_IdleResetCB, 0, 0, 0.0f, 0.0f, 1, 0, 0.15f, NULL);
    xAnimTableNewTransition(
        table, "Idle Idle2 Idle3", "Talk", (xAnimTransitionConditionalCallback)Shaggy1_Talk,
        (xAnimTransitionCallback)Shaggy1_TalkChooseCB, 0, 0x550001, 0.0f, 0.0f, 1, 0, 0.15f, NULL);
    xAnimTableNewTransition(
        table, "Talk", "Talk4", (xAnimTransitionConditionalCallback)Shaggy1_TalkChoose,
        (xAnimTransitionCallback)Shaggy1_TalkChooseCB, 0, 0x230001, 0.0f, 0.0f, 1, 0, 0.15f, NULL);
    xAnimTableNewTransition(
        table, "Talk4", "Talk", (xAnimTransitionConditionalCallback)Shaggy1_TalkChoose,
        (xAnimTransitionCallback)Shaggy1_TalkChooseCB, 0, 0x340001, 0.0f, 0.0f, 1, 0, 0.15f, NULL);
    xAnimTableNewTransition(
        table, "Talk Talk4", "Idle", (xAnimTransitionConditionalCallback)Shaggy1_Notalk,
        (xAnimTransitionCallback)Shaggy1_NotalkCB, 0, 0, 0.0f, 0.0f, 1, 0, 0.15f, NULL);
    xAnimTableNewTransition(table, "Idle", "Idle2",
                            (xAnimTransitionConditionalCallback)Shaggy1_IdlePick, NULL, 0, 4, 0.0f,
                            0.0f, 1, 0, 0.2f, NULL);
    xAnimTableNewTransition(table, "Idle2", "Idle",
                            (xAnimTransitionConditionalCallback)Shaggy1_IdlePick, NULL, 0, 7, 0.0f,
                            0.0f, 1, 0, 0.2f, NULL);
    xAnimTableNewTransition(table, "Idle", "Idle2",
                            (xAnimTransitionConditionalCallback)Shaggy1_IdlePick, NULL, 0, 12, 0.0f,
                            0.0f, 1, 0, 0.2f, NULL);
    xAnimTableNewTransition(table, "Idle2", "Idle",
                            (xAnimTransitionConditionalCallback)Shaggy1_IdlePick, NULL, 0, 14, 0.0f,
                            0.0f, 1, 0, 0.2f, NULL);
    xAnimTableNewTransition(table, "Idle", "Idle3",
                            (xAnimTransitionConditionalCallback)Shaggy1_IdlePick, NULL, 0, 0x11,
                            0.0f, 0.0f, 1, 0, 0.2f, NULL);
    xAnimTableNewTransition(
        table, "Idle3", "Idle", (xAnimTransitionConditionalCallback)Shaggy1_IdlePick,
        (xAnimTransitionCallback)Shaggy1_IdleResetCB, 0, 0x13, 0.0f, 0.0f, 1, 0, 0.2f, NULL);
    xAnimTableNewTransition(
        table, "Idle2 Idle3", "Idle", (xAnimTransitionConditionalCallback)Shaggy1_IdleLeave,
        (xAnimTransitionCallback)Shaggy1_IdleResetCB, 0, 0, 0.0f, 0.0f, 2, 0, 0.15f, NULL);
    xAnimTableNewTransition(table, "SharkHang", "SharkHang2",
                            (xAnimTransitionConditionalCallback)Shaggy1_IdlePick, NULL, 0, 4, 0.0f,
                            0.0f, 1, 0, 0.2f, NULL);
    xAnimTableNewTransition(table, "SharkHang2", "SharkHang",
                            (xAnimTransitionConditionalCallback)Shaggy1_IdlePick, NULL, 0, 7, 0.0f,
                            0.0f, 1, 0, 0.2f, NULL);
    xAnimTableNewTransition(table, "SharkHang", "SharkHang2",
                            (xAnimTransitionConditionalCallback)Shaggy1_IdlePick, NULL, 0, 12, 0.0f,
                            0.0f, 1, 0, 0.2f, NULL);
    xAnimTableNewTransition(table, "SharkHang2", "SharkHang",
                            (xAnimTransitionConditionalCallback)Shaggy1_IdlePick, NULL, 0, 14, 0.0f,
                            0.0f, 1, 0, 0.2f, NULL);
    xAnimTableNewTransition(table, "SharkHang", "SharkHang3",
                            (xAnimTransitionConditionalCallback)Shaggy1_IdlePick, NULL, 0, 0x11,
                            0.0f, 0.0f, 1, 0, 0.2f, NULL);
    xAnimTableNewTransition(
        table, "SharkHang3", "SharkHang", (xAnimTransitionConditionalCallback)Shaggy1_IdlePick,
        (xAnimTransitionCallback)Shaggy1_IdleResetCB, 0, 0x13, 0.0f, 0.0f, 1, 0, 0.2f, NULL);
    xAnimTableNewTransition(table, "Idle Idle2 Idle3", "React_scare",
                            (xAnimTransitionConditionalCallback)Shaggy1_ScareReact, NULL, 0, 0,
                            0.0f, 0.0f, 11, 0, 0.15f, NULL);
    xAnimTableNewTransition(table, "Idle Idle2 Idle3 React_scare", "React_death",
                            (xAnimTransitionConditionalCallback)Shaggy1_DeathReact, NULL, 0, 0,
                            0.0f, 0.0f, 12, 0, 0.15f, NULL);

    return table;
}

void Shaggy1_PlayerTransitions(xAnimTable* table)
{
    xAnimTableNewTransition(table, "SD1_CatchJump SD1_CatchThrow", "SD1_Hold", NULL, NULL, 0x10, 0,
                            0.0f, 0.0f, 0, 0, 0.0f, NULL);
    xAnimTableNewTransition(table, "SD1_Throw", "SD1_ThrowIdle", NULL, NULL, 0x10, 0, 0.0f, 0.0f, 0,
                            0, 0.0f, NULL);
    xAnimTableNewTransition(table, "Walk_normal Walk_scared Walk_brave", "SD1_CatchJump",
                            (xAnimTransitionConditionalCallback)Shaggy1_Grab, NULL, 0, 1, 0.0f,
                            0.0f, 10, 0, 0.15f, NULL);
    xAnimTableNewTransition(table, "SD1_Hold", "SD1_HoldWalk",
                            (xAnimTransitionConditionalCallback)Shaggy1_Walk, NULL, 0, 0, 0.0f,
                            0.0f, 1, 0, 0.15f, NULL);
    xAnimTableNewTransition(table, "SD1_HoldWalk", "SD1_Hold",
                            (xAnimTransitionConditionalCallback)Shaggy1_Stop, NULL, 0, 0, 0.0f,
                            0.0f, 1, 0, 0.15f, NULL);
    xAnimTableNewTransition(table, "SD1_ThrowIdle", "SD1_ThrowWalk",
                            (xAnimTransitionConditionalCallback)Shaggy1_Walk, NULL, 0, 0, 0.0f,
                            0.0f, 1, 0, 0.15f, NULL);
    xAnimTableNewTransition(table, "SD1_ThrowWalk", "SD1_ThrowIdle",
                            (xAnimTransitionConditionalCallback)Shaggy1_Stop, NULL, 0, 0, 0.0f,
                            0.0f, 1, 0, 0.15f, NULL);
    xAnimTableNewTransition(table, "SD1_Hold SD1_HoldWalk", "SD1_Throw",
                            (xAnimTransitionConditionalCallback)Shaggy1_Throw, NULL, 0, 0, 0.0f,
                            0.0f, 1, 0, 0.15f, NULL);
    xAnimTableNewTransition(table, "SD1_ThrowIdle SD1_ThrowWalk", "SD1_LandReact",
                            (xAnimTransitionConditionalCallback)Shaggy1_Land, NULL, 0, 0, 0.0f,
                            0.0f, 2, 0, 0.0f, NULL);
    xAnimTableNewTransition(table, "SD1_ThrowIdle SD1_ThrowWalk", "SD1_CatchThrow",
                            (xAnimTransitionConditionalCallback)Shaggy1_Catch, NULL, 0, 0, 0.0f,
                            0.0f, 5, 0, 0.15f, NULL);
    xAnimTableNewTransition(table, "SD1_ThrowIdle SD1_ThrowWalk", "SD1_HangReact",
                            (xAnimTransitionConditionalCallback)Shaggy1_Hang, NULL, 0, 0, 0.0f,
                            0.0f, 5, 0, 0.15f, NULL);
    xAnimTableNewTransition(table, "SD1_Hold", "SD1_SetDown",
                            (xAnimTransitionConditionalCallback)Shaggy1_SetDown, NULL, 0, 0, 0.0f,
                            0.0f, 11, 0, 0.0f, NULL);
}

void Shaggy0_PlayerStates(xAnimTable* table)
{
    xAnimTableNewState(table, "SD0_Hold", 0x10, 0, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "SD0_HoldWalk", 0x10, 4, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "SD0_CatchJump", 0x20, 0x80, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "SD0_CatchThrow", 0x20, 0x80, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "SD0_Throw", 0x20, 0x80, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "SD0_SetDown", 0x20, 0x80, 1.0f, NULL, NULL, 0.0f, NULL);
}

xAnimTable* zEntShaggy_Anim0()
{
    xAnimTable* table;

    table = xAnimTableNew("Shaggy0", NULL);
    xAnimTableNewState(table, "Idle", 0x10, 0, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "Idle2", 0x10, 0, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "Idle3", 0x10, 0, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "Hold", 0x10, 0x1000, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "HoldWalk", 0x10, 0x1000, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "CatchJump", 0x20, 0x1000, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "CatchThrow", 0x20, 0x1000, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "SetDown", 0x20, 0x1000, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "Throw", 0x20, 0, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "ThrowIdle", 0x10, 0, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "LandReact", 0x20, 0, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "Talk", 0x10, 0, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "Talk2", 0x10, 0, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "Talk3", 0x10, 0, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "Talk4", 0x10, 0, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "React_scare", 0x20, 0, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "React_death", 0, 0, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "Carry_hit", 0x20, 0, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "SD0_Hold", 0x10, 0, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "SD0_HoldWalk", 0x10, 4, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "SD0_CatchJump", 0x20, 0x80, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "SD0_CatchThrow", 0x20, 0x80, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "SD0_Throw", 0x20, 0x80, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "SD0_SetDown", 0x20, 0x80, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewTransition(table, "CatchJump", "Hold", NULL,
                            (xAnimTransitionCallback)Shaggy0_FlipCB, 0x10, 0, 0.0f, 0.0f, 0, 0,
                            0.0f, NULL);
    xAnimTableNewTransition(table, "CatchThrow", "Hold", NULL, NULL, 0x10, 0, 0.0f, 0.0f, 0, 0,
                            0.0f, NULL);
    xAnimTableNewTransition(table, "Throw", "ThrowIdle", NULL, NULL, 0x10, 0, 0.0f, 0.0f, 0, 0,
                            0.0f, NULL);
    xAnimTableNewTransition(table, "LandReact", "Idle", NULL,
                            (xAnimTransitionCallback)Shaggy1_IdleResetCB, 0x10, 0, 0.0f, 0.0f, 0, 0,
                            0.0f, NULL);
    xAnimTableNewTransition(table, "React_scare", "Idle", NULL,
                            (xAnimTransitionCallback)Shaggy1_IdleResetCB, 0x10, 0, 0.0f, 0.0f, 0, 0,
                            0.15f, NULL);
    xAnimTableNewTransition(
        table, "Idle Idle2 Idle3", "CatchJump", (xAnimTransitionConditionalCallback)Shaggy1_Grab,
        (xAnimTransitionCallback)Shaggy1_GrabCB, 0, 0, 0.0f, 0.0f, 10, 0, 0.0f, NULL);
    xAnimTableNewTransition(table, "Hold", "HoldWalk",
                            (xAnimTransitionConditionalCallback)Shaggy1_Walk, NULL, 0, 0, 0.0f,
                            0.0f, 1, 0, 0.15f, NULL);
    xAnimTableNewTransition(table, "HoldWalk", "Hold",
                            (xAnimTransitionConditionalCallback)Shaggy1_Stop, NULL, 0, 0, 0.0f,
                            0.0f, 1, 0, 0.15f, NULL);
    xAnimTableNewTransition(
        table, "Hold HoldWalk", "Throw", (xAnimTransitionConditionalCallback)Shaggy0_Throw,
        (xAnimTransitionCallback)Shaggy1_ThrowCB, 0, 0, 0.0f, 0.0f, 1, 0, 0.15f, NULL);
    xAnimTableNewTransition(
        table, "ThrowIdle", "LandReact", (xAnimTransitionConditionalCallback)Shaggy0_PlayerLanded,
        (xAnimTransitionCallback)Shaggy1_LandCB, 0, 0, 0.0f, 0.0f, 5, 0, 0.0f, NULL);
    xAnimTableNewTransition(
        table, "ThrowIdle", "CatchThrow", (xAnimTransitionConditionalCallback)Shaggy0_PlayerCaught,
        (xAnimTransitionCallback)Shaggy1_EventCB, 0, 0xe8, 0.0f, 0.0f, 6, 0, 0.0f, NULL);
    xAnimTableNewTransition(table, "Idle Idle2 Idle3 Talk2 Talk3", "Talk",
                            (xAnimTransitionConditionalCallback)Shaggy1_TalkFlag,
                            (xAnimTransitionCallback)Shaggy1_TalkChooseCB, 0, 0x230003, 0.0f, 0.0f,
                            1, 0, 0.15f, NULL);
    xAnimTableNewTransition(table, "Idle Idle2 Idle3 Talk  Talk3", "Talk2",
                            (xAnimTransitionConditionalCallback)Shaggy1_TalkFlag,
                            (xAnimTransitionCallback)Shaggy1_TalkChooseCB, 0, 0x230003, 0.0f, 0.0f,
                            1, 0, 0.15f, NULL);
    xAnimTableNewTransition(table, "Idle Idle2 Idle3 Talk  Talk2", "Talk3",
                            (xAnimTransitionConditionalCallback)Shaggy1_TalkFlag,
                            (xAnimTransitionCallback)Shaggy1_TalkChooseCB, 0, 0x230003, 0.0f, 0.0f,
                            1, 0, 0.15f, NULL);
    xAnimTableNewTransition(table, "Idle Idle2 Idle3 Talk Talk2 Talk3", "Talk4",
                            (xAnimTransitionConditionalCallback)Shaggy1_TalkFlag,
                            (xAnimTransitionCallback)Shaggy1_TalkChooseCB, 0, 0xFF0101, 0.0f, 0.0f,
                            1, 0, 0.15f, NULL);
    xAnimTableNewTransition(
        table, "Talk Talk2 Talk3 Talk4", "Idle", (xAnimTransitionConditionalCallback)Shaggy1_Notalk,
        (xAnimTransitionCallback)Shaggy1_NotalkCB, 0, 0, 0.0f, 0.0f, 1, 0, 0.15f, NULL);
    xAnimTableNewTransition(table, "Idle", "Idle2",
                            (xAnimTransitionConditionalCallback)Shaggy1_IdlePick, NULL, 0, 4, 0.0f,
                            0.0f, 1, 0, 0.2f, NULL);
    xAnimTableNewTransition(table, "Idle2", "Idle",
                            (xAnimTransitionConditionalCallback)Shaggy1_IdlePick, NULL, 0, 7, 0.0f,
                            0.0f, 1, 0, 0.2f, NULL);
    xAnimTableNewTransition(table, "Idle", "Idle2",
                            (xAnimTransitionConditionalCallback)Shaggy1_IdlePick, NULL, 0, 12, 0.0f,
                            0.0f, 1, 0, 0.2f, NULL);
    xAnimTableNewTransition(table, "Idle2", "Idle",
                            (xAnimTransitionConditionalCallback)Shaggy1_IdlePick, NULL, 0, 14, 0.0f,
                            0.0f, 1, 0, 0.2f, NULL);
    xAnimTableNewTransition(table, "Idle", "Idle3",
                            (xAnimTransitionConditionalCallback)Shaggy1_IdlePick, NULL, 0, 0x11,
                            0.0f, 0.0f, 1, 0, 0.2f, NULL);
    xAnimTableNewTransition(
        table, "Idle3", "Idle", (xAnimTransitionConditionalCallback)Shaggy1_IdlePick,
        (xAnimTransitionCallback)Shaggy1_IdleResetCB, 0, 0x13, 0.0f, 0.0f, 1, 0, 0.2f, NULL);
    xAnimTableNewTransition(
        table, "Idle2 Idle3", "Idle", (xAnimTransitionConditionalCallback)Shaggy1_IdleLeave,
        (xAnimTransitionCallback)Shaggy1_IdleResetCB, 0, 0, 0.0f, 0.0f, 2, 0, 0.15f, NULL);
    xAnimTableNewTransition(table, "Idle Idle2 Idle3 Throw ThrowIdle", "React_scare",
                            (xAnimTransitionConditionalCallback)Shaggy1_ScareReact,
                            (xAnimTransitionCallback)Shaggy1_LandCB, 0, 0, 0.0f, 0.0f, 11, 0, 0.15f,
                            NULL);
    xAnimTableNewTransition(table, "Idle Idle2 Idle3 Throw ThrowIdle React_scare", "React_death",
                            (xAnimTransitionConditionalCallback)Shaggy1_DeathReact,
                            (xAnimTransitionCallback)Shaggy1_LandCB, 0, 0, 0.0f, 0.0f, 12, 0, 0.15f,
                            NULL);
    xAnimTableNewTransition(
        table, "Hold HoldWalk", "Idle", (xAnimTransitionConditionalCallback)Shaggy1_FallDeath,
        (xAnimTransitionCallback)Shaggy1_FallDeathCB, 0, 0, 0.0f, 0.0f, 12, 0, 0.15f, NULL);
    xAnimTableNewTransition(table, "Hold HoldWalk", "Carry_hit",
                            (xAnimTransitionConditionalCallback)Shaggy1_Scare, NULL, 0, 0xe9, 0.0f,
                            0.0f, 11, 0, 0.15f, NULL);
    xAnimTableNewTransition(table, "Carry_hit", "Idle", NULL,
                            (xAnimTransitionCallback)Shaggy0_CarryHitCB, 0x10, 0, 0.0f, 0.0f, 1, 0,
                            0.15f, NULL);

    return table;
}

void Shaggy0_PlayerTransitions(xAnimTable* table)
{
    xAnimTransition* tran;

    xAnimTableNewTransition(table, "SD0_CatchJump", "SD0_Hold", NULL,
                            (xAnimTransitionCallback)Shaggy0_FlipCB, 0x10, 0, 0.0f, 0.0f, 0, 0,
                            0.0f, NULL);
    xAnimTableNewTransition(table, "SD0_CatchThrow", "SD0_Hold", NULL, NULL, 0x10, 0, 0.0f, 0.0f, 0,
                            0, 0.0f, NULL);
    xAnimTableNewTransition(table, "Walk_normal Walk_scared Walk_brave", "SD0_CatchJump",
                            (xAnimTransitionConditionalCallback)Shaggy1_Grab, NULL, 0, 0, 0.0f,
                            0.0f, 10, 0, 0.15f, NULL);
    xAnimTableNewTransition(table, "SD0_Hold", "SD0_HoldWalk",
                            (xAnimTransitionConditionalCallback)Shaggy1_Walk, NULL, 0, 0, 0.0f,
                            0.0f, 1, 0, 0.15f, NULL);
    xAnimTableNewTransition(table, "SD0_HoldWalk", "SD0_Hold",
                            (xAnimTransitionConditionalCallback)Shaggy1_Stop, NULL, 0, 0, 0.0f,
                            0.0f, 1, 0, 0.15f, NULL);
    xAnimTableNewTransition(table, "SD0_Hold SD0_HoldWalk", "SD0_Throw",
                            (xAnimTransitionConditionalCallback)Shaggy0_Throw, NULL, 0, 0, 0.0f,
                            0.0f, 1, 0, 0.15f, NULL);
    tran = xAnimTableNewTransition(
        table, "Jump_normal Jump_double Jump_buttbounce Jump_hang Jump_springboard",
        "SD0_CatchThrow", (xAnimTransitionConditionalCallback)Shaggy0_Catch,
        (xAnimTransitionCallback)Shaggy0_CatchCB, 0, 0, 0.0f, 0.0f, 10, 0, 0.15f, NULL);
    xAnimTableAddTransition(
        table, tran,
        "Fall_normal Fall_run Fall_walk Fall_high Fall_normal Fall_run Fall_walk Fall_floatup");
    xAnimTableAddTransition(
        table, tran,
        "Fall_floatdown Fall_floatopen Fall_losechance HangGrab Hang HangToSwing SwingToHang");
    xAnimTableAddTransition(table, tran, "Fall_spring Fall_fromspring");
    xAnimTableAddTransition(table, tran, "Buttsmash_start Buttsmash_fall");
}

void Shaggy1_SetHangable(_zEnt* ent, _zEntHangable* hang)
{
    ShaggyGuts* guts = zEntShaggy_FindGuts(ent);

    guts->hangme = hang;
}

static void Shaggy1_BonusAnims(_zEnt* ent)
{
    U32 rootbone;
    F32 catch_tranadjust[3] = { 0.0f, 0.0f, 0.6f };
    F32 shark_tranadjust[3] = { 0.0f, -0.56f, 0.0f };
    xQuat tempquat;
    xAnimState* state;
    xAnimTable* plyrtbl;
    xAnimState* plyrstate;
    void* hang_animdata = NULL;
    void* hangswing_animdata = NULL;
    xVec3 va;
    xVec3 vb;
    F32 temptran[3];

    state = ent->model->Anim->Table->StateList;
    plyrtbl = globals.player.ent.model->Anim->Table;
    while (state)
    {
        if (state->Name[0] == 'S' && state->Name[1] == 'D' &&
            (state->Name[2] == '1' || state->Name[2] == '0'))
        {
            for (plyrstate = plyrtbl->StateList; plyrstate; plyrstate = plyrstate->Next)
            {
                if (plyrstate->ID == state->ID)
                {
                    plyrstate->Speed = state->Speed;
                    plyrstate->Data = state->Data;
                }
            }
            rootbone = 2;
        }
        else
        {
            rootbone = 1;
        }

        if (!(((iAnimSKBHeader*)state->Data->RawData[0])->Flags & 0x80000000))
        {
            ((iAnimSKBHeader*)state->Data->RawData[0])->Flags |= 0x80000000;

            if (!xStricmp(state->Name, "SD0_CatchJump") || !xStricmp(state->Name, "SD0_Hold") ||
                !xStricmp(state->Name, "SD0_HoldWalk") || !xStricmp(state->Name, "SD0_CatchThrow") ||
                !xStricmp(state->Name, "SD0_Throw") || !xStricmp(state->Name, "SD0_SetDown") ||
                !xStricmp(state->Name, "SD1_Hold") || !xStricmp(state->Name, "SD1_HoldWalk") ||
                !xStricmp(state->Name, "SD1_CatchJump") || !xStricmp(state->Name, "SD1_CatchThrow") ||
                !xStricmp(state->Name, "SD1_Throw") || !xStricmp(state->Name, "SD1_ThrowIdle") ||
                !xStricmp(state->Name, "SD1_ThrowWalk") || !xStricmp(state->Name, "SD1_HangReact") ||
                !xStricmp(state->Name, "SD1_LandReact") || !xStricmp(state->Name, "SD1_SetDown") ||
                (!xStricmp(state->Name, "Hold") && ent->subType == 0x1C) ||
                (!xStricmp(state->Name, "ThrowIdle") && ent->subType == 0x1C) ||
                !xStricmp(state->Name, "LandReact") || !xStricmp(state->Name, "CatchJump") ||
                !xStricmp(state->Name, "CatchThrow") || !xStricmp(state->Name, "Throw") ||
                !xStricmp(state->Name, "HoldWalk") || !xStricmp(state->Name, "SetDown"))
            {
                _iAnimSKBAdjustTranslate((iAnimSKBHeader*)state->Data->RawData[0], rootbone,
                                         catch_tranadjust);
            }

            if (!xStricmp(state->Name, "SharkHang") || !xStricmp(state->Name, "SharkHang2") ||
                !xStricmp(state->Name, "SharkHang3") || !xStricmp(state->Name, "SharkSwing") ||
                !xStricmp(state->Name, "SharkTalk"))
            {
                _iAnimSKBAdjustTranslate((iAnimSKBHeader*)state->Data->RawData[0], rootbone,
                                         shark_tranadjust);
            }

            if (ent->subType == 0x1C)
            {
                if (!xStricmp(state->Name, "SD0_Hold") || !xStricmp(state->Name, "SD0_HoldWalk") ||
                    !xStricmp(state->Name, "SD0_CatchThrow") || !xStricmp(state->Name, "SD0_Throw") ||
                    !xStricmp(state->Name, "Hold") || !xStricmp(state->Name, "ThrowIdle") ||
                    !xStricmp(state->Name, "LandReact") || !xStricmp(state->Name, "CatchThrow") ||
                    !xStricmp(state->Name, "Throw") || !xStricmp(state->Name, "HoldWalk"))
                {
                    _iAnimSKBFlip180((iAnimSKBHeader*)state->Data->RawData[0], rootbone);
                }
            }

            if (!(state->Name[0] == 'S' && state->Name[1] == 'D' &&
                  (state->Name[2] == '1' || state->Name[2] == '0') && state->Name[3] == '_') &&
                (xStricmp(state->Name, "Hold") || ent->subType == 0x1C))
            {
                _iAnimSKBSwapTranslate((iAnimSKBHeader*)state->Data->RawData[0], 0, rootbone);
            }

            if (!xStricmp(state->Name, "SD1_HangReact"))
            {
                _iAnimSKBFixTransPop(state->Data->RawData[0],
                                     plyrtbl->StateList->Data->RawData[0], 3);
            }
        }

        if (!xStricmp(state->Name, "Apex") && ent->subType == 0x1D)
        {
            iAnimEval(state->Data->RawData[0], 0.0f, 1, &globals.player.Shaggy1.ReleaseTrans,
                      &tempquat);
        }
        if (!xStricmp(state->Name, "Hang"))
        {
            iAnimEval(state->Data->RawData[0], 0.0f, 1, &globals.player.Shaggy1.HangTrans,
                      &tempquat);
            hang_animdata = state->Data->RawData[0];
        }
        if (!xStricmp(state->Name, "HangSwing"))
        {
            hangswing_animdata = state->Data->RawData[0];
        }
        if (!xStricmp(state->Name, "Throw") && ent->subType == 0x1D)
        {
            iAnimEval(state->Data->RawData[0], state->Data->Duration - 0.03333f, 1, &va,
                      &tempquat);
            iAnimEval(state->Data->RawData[0], state->Data->Duration, 1, &vb, &tempquat);
            globals.player.Shaggy1.ThrowPos.x = vb.x;
            globals.player.Shaggy1.ThrowPos.y = vb.y;
            globals.player.Shaggy1.ThrowPos.z = vb.z;
            globals.player.Shaggy1.ThrowVel.x = (vb.x - va.x) / 0.03333f;
            globals.player.Shaggy1.ThrowVel.y = (vb.y - va.y) / 0.03333f;
            globals.player.Shaggy1.ThrowVel.z = (vb.z - va.z) / 0.03333f;
            gACCEL_END = state->Data->Duration;
            gACCEL_START = gACCEL_END - 0.2f;
        }
        state = state->Next;
    }

    temptran[0] = 0.0f;
    temptran[1] = globals.player.Shaggy1.ReleaseTrans.y - globals.player.Shaggy1.HangTrans.y;
    temptran[2] = 0.0f;
    if (hang_animdata)
    {
        _iAnimSKBAdjustTranslate((iAnimSKBHeader*)hang_animdata, 0, temptran);
    }
    if (hangswing_animdata)
    {
        _iAnimSKBAdjustTranslate((iAnimSKBHeader*)hangswing_animdata, 0, temptran);
    }
}

static void Shaggy1_MiddleUpdate(_zEnt* shag, _xScene* sc, F32 dt)
{
    F32 dz = 0.0f;
    F32 dx = 0.0f;
    F32 oldgravity;
    xAnimState* plyrstate;
    xAnimState* astate;

    astate = shag->model->Anim->Single->State;
    plyrstate = globals.player.ent.model->Anim->Single->State;
    oldgravity = sc->gravity;

    if (!xStricmp(astate->Name, "Throw"))
    {
        sc->gravity = 0.0f;
    }
    else
    {
        sc->gravity = -20.0f;
    }

    if (shag->pflags & 0x2)
    {
        xEntApplyPhysics(shag, sc, dt);
    }
    if (shag->pflags & 0x1)
    {
        xEntMove(shag, sc, dt);
    }
    if (shag != globals.player.Shaggy1.Grab || !globals.player.Shaggy1.HangTarget)
    {
        xFFXApply(shag, sc, dt);
        xEntCollide(shag, sc, dt);
    }

    if (!xStricmp(astate->Name, "Fall") || !xStricmp(astate->Name, "Apex"))
    {
        if (shag->collis->colls[0].flags & 0x1)
        {
            shag->frame->vel.x = 0.0f;
            shag->frame->vel.y = 0.0f;
            shag->frame->vel.z = 0.0f;
        }
        if (!xStricmp(plyrstate->Name, "SD1_ThrowIdle") ||
            !xStricmp(plyrstate->Name, "SD1_ThrowWalk"))
        {
            dx = shag->frame->mat.pos.x - globals.player.ent.frame->mat.pos.x;
            dz = shag->frame->mat.pos.z - globals.player.ent.frame->mat.pos.z;
            if (shag->frame->mat.pos.y - globals.player.ent.frame->mat.pos.y < 1.8f &&
                dx * dx + dz * dz < 0.5f)
            {
                globals.player.Shaggy1.CatchMe = 1;
            }
        }
    }

    sc->gravity = oldgravity;
}

static void Shaggy1_Update(_zEnt* ent, _xScene* sc, F32 dt)
{
    F32 dz;
    F32 dx;
    xMat3x3* mat;
    F32 mag;
    xVec3* pos;
    _zEntPickup* pup;
    zScene* zsc;
    F32 cz;
    F32 cy;
    F32 cx;
    U32 i;

    if (!xEntIsVisible(ent))
    {
        return;
    }

    ShaggyGuts* guts = zEntShaggy_FindGuts(ent);

    if (!guts->swap)
    {
        guts->idleTimer += dt;
    }
    if (guts->talkTimer)
    {
        guts->talkTimer -= dt;
        if (guts->talkTimer < 0.0f)
        {
            guts->talkTimer = 0.0f;
        }
    }

    if (ent->subType == 0x1D)
    {
        if (!xStricmp(ent->model->Anim->Single->State->Name, "Throw") ||
            !xStricmp(ent->model->Anim->Single->State->Name, "ThrowIdle") ||
            !xStricmp(ent->model->Anim->Single->State->Name, "Fall") ||
            !xStricmp(ent->model->Anim->Single->State->Name, "Apex"))
        {
            zsc = (zScene*)sc;
            cx = ent->bound.sph.center.x;
            cy = ent->bound.sph.center.y;
            cz = ent->bound.sph.center.z;
            for (i = 0; i < zsc->num_ents; i++)
            {
                pup = (_zEntPickup*)zsc->ents[i];
                if (pup->baseType == 4 && (pup->state & 0x3F) == 1)
                {
                    pos = &((xMat4x3*)pup->model->Mat)->pos;
                    if ((cx - pos->x) * (cx - pos->x) + (cy - pos->y) * (cy - pos->y) +
                            (cz - pos->z) * (cz - pos->z) <
                        4.0f)
                    {
                        pup->state = (pup->state & ~0x3F) | 2;
                    }
                }
            }
        }

        if (!guts->hangme)
        {
            mat = (xMat3x3*)ent->model->Mat;
            if (xabs(mat->up.y - 1.0f) > 0.0001f)
            {
                dx = -mat->up.x;
                cy = 1.0f - mat->up.y;
                dz = -mat->up.z;
                mag = dx * dx + cy * cy + dz * dz;
                if (mag < 0.0049f)
                {
                    mat->up.x = 0.0f;
                    mat->up.y = 1.0f;
                    mat->up.z = 0.0f;
                }
                else
                {
                    mag = 0.07f * (1.0f / sqrtf(mag));
                    dx *= mag;
                    cy *= mag;
                    dz *= mag;
                    mat->up.x += dx;
                    mat->up.y += cy;
                    mat->up.z += dz;
                    xVec3Normalize(&mat->up, &mat->up);
                    xVec3Cross(&mat->right, &mat->up, &mat->at);
                    xVec3Normalize(&mat->right, &mat->right);
                    xVec3Cross(&mat->at, &mat->right, &mat->up);
                }
            }
        }
    }

    if (globals.player.Shaggy1.Grab == ent)
    {
        if (ent->subType == 0x1C && globals.player.JumpState &&
            globals.player.ent.frame->vel.y < 0.0f &&
            !xStricmp(ent->model->Anim->Single->State->Name, "ThrowIdle"))
        {
            ent->penby |= 0x10;
            ent->chkby |= 0x10;
        }
        else
        {
            ent->penby &= (U8)~0x10;
            ent->chkby &= (U8)~0x10;
        }
    }
    else if (guts->hangme)
    {
        ent->penby &= (U8)~0x10;
        ent->chkby &= (U8)~0x10;
        xEntBeginUpdate(ent, sc, dt);
        xEntEndUpdate(ent, sc, dt);
    }
    else
    {
        ent->penby |= 0x10;
        ent->chkby |= 0x10;
        xEntBeginUpdate(ent, sc, dt);
        Shaggy1_MiddleUpdate(ent, sc, dt);
        xEntEndUpdate(ent, sc, dt);

        if (ent == globals.player.Shaggy1.GrabTry && ent != globals.player.Shaggy1.Grab)
        {
            globals.player.Shaggy1.Grab = NULL;
            globals.player.Shaggy1.GrabTry = NULL;
        }

        if (ent == globals.player.Shaggy1.Grab)
        {
            dx = ent->bound.sph.center.x - globals.player.ent.bound.sph.center.x;
            dz = ent->bound.sph.center.z - globals.player.ent.bound.sph.center.z;
            globals.player.Shaggy1.GrabTimer = 1e-6f;
            globals.player.Shaggy1.GrabRot = xAngleClamp(atan2(dx, dz));
            globals.player.Shaggy1.GrabOffset =
                sqrtf(dx * dx + dz * dz) - ent->model->Anim->Translate[1].z;
        }

        if (guts->swap)
        {
            ShaggySwap(ent, guts->swap);
        }
    }
}

static void Shaggy1_Move(_xEnt* ent, _xScene* s, F32 dt, _xEntFrame* frame)
{
    F32 idealz;
    F32 idealx;
    U32 solcnt;
    F32 sol[2];
    F32 a;
    F32 b;
    F32 relheight;
    F32 curheight;
    _zEntHangable* hang;
    F32 newTransY;
    F32 falltime;
    F32 falldist;
    xAnimPlay* play;
    xAnimState* astate;
    S32 retnow;

    play = ent->model->Anim;
    astate = play->Single->State;
    ShaggyGuts* guts = zEntShaggy_FindGuts((_zEnt*)ent);

    frame->mode = 0;
    frame->dpos.x = 0.0f;
    frame->dpos.y = 0.0f;
    frame->dpos.z = 0.0f;

    if (!xStricmp(astate->Name, "Turn") && guts->facerot != 0.0f)
    {
        frame->mode |= 0x20;
        if (xabs(guts->facerot) <= 1.5707964f * dt)
        {
            zEntEvent((xBase*)guts->facept, 0x1F);
            frame->drot.angle = guts->facerot;
            guts->facerot = 0.0f;
            guts->facept = NULL;
        }
        else if (guts->facerot > 0.0f)
        {
            frame->drot.angle = 1.5707964f * dt;
            guts->facerot -= 1.5707964f * dt;
        }
        else
        {
            frame->drot.angle = -1.5707964f * dt;
            guts->facerot += 1.5707964f * dt;
        }
        return;
    }

    if (!xStricmp(astate->Name, "SetDown"))
    {
        xVec3 delta;

        delta.x = play->Translate[1].x - globals.player.Shaggy1.ReleaseTrans.x;
        delta.y = play->Translate[1].y - globals.player.Shaggy1.ReleaseTrans.y;
        delta.z = play->Translate[1].z - globals.player.Shaggy1.ReleaseTrans.z;
        play->Translate[1].x = globals.player.Shaggy1.ReleaseTrans.x;
        play->Translate[1].y = globals.player.Shaggy1.ReleaseTrans.y;
        play->Translate[1].z = globals.player.Shaggy1.ReleaseTrans.z;
        xMat3x3RMulVec(&delta, (xMat3x3*)ent->model->Mat, &delta);
        delta.x += globals.player.ent.frame->mat.pos.x - ((xMat4x3*)ent->model->Mat)->pos.x;
        delta.y += globals.player.ent.frame->mat.pos.y - ((xMat4x3*)ent->model->Mat)->pos.y;
        delta.z += globals.player.ent.frame->mat.pos.z - ((xMat4x3*)ent->model->Mat)->pos.z;
        frame->dpos.x = delta.x;
        frame->dpos.y = delta.y;
        frame->dpos.z = delta.z;
        frame->mode |= 0x2;
        return;
    }

    if (globals.player.Shaggy1.ThrowTgtAdjust == 2 && frame->vel.y <= 0.0f)
    {
        falldist = frame->mat.pos.y - globals.player.Shaggy1.ThrowTgtVec.y;
        if (falldist > 0.5f)
        {
            falltime = sqrtf(0.1f * falldist);
            printf("Vel  %f %f (%f)  -->  ", frame->vel.x, frame->vel.z,
                   sqrtf(frame->vel.x * frame->vel.x + frame->vel.z * frame->vel.z));
            frame->vel.x = (globals.player.Shaggy1.ThrowTgtVec.x - frame->mat.pos.x) / falltime;
            frame->vel.z = (globals.player.Shaggy1.ThrowTgtVec.z - frame->mat.pos.z) / falltime;
            printf("%f %f (%f)\n", frame->vel.x, frame->vel.z,
                   sqrtf(frame->vel.x * frame->vel.x + frame->vel.z * frame->vel.z));
        }
        globals.player.Shaggy1.ThrowTgtAdjust = 0;
    }

    if (ent == globals.player.Shaggy1.Grab && ent->subType == 0x1D &&
        globals.player.Shaggy1.ThrowTimer >= 0.0f)
    {
        globals.player.Shaggy1.ThrowTimer += dt;
        if (globals.player.Shaggy1.ThrowTimer < gACCEL_START)
        {
            ent->pflags &= 0xFB;
            retnow = 1;
            frame->vel.y = 0.0f;
            frame->mat.pos = ((xMat4x3*)globals.player.ent.model->Mat)->pos;
            frame->mode |= 0x1;
            globals.player.Shaggy1.ExtraTransY = 0.0f;
        }
        else if (!xStricmp(astate->Name, "Throw"))
        {
            xVec3 delta;

            ((xMat4x3*)ent->model->Mat)->pos.y -= globals.player.Shaggy1.ExtraTransY;
            delta.x = play->Translate[1].x - globals.player.Shaggy1.ReleaseTrans.x;
            delta.y = play->Translate[1].y - globals.player.Shaggy1.ReleaseTrans.y;
            delta.z = play->Translate[1].z - globals.player.Shaggy1.ReleaseTrans.z;
            play->Translate[1].x = globals.player.Shaggy1.ReleaseTrans.x;
            play->Translate[1].y = globals.player.Shaggy1.ReleaseTrans.y;
            play->Translate[1].z = globals.player.Shaggy1.ReleaseTrans.z;
            xMat3x3RMulVec(&delta, (xMat3x3*)ent->model->Mat, &delta);
            retnow = 1;
            delta.x += globals.player.ent.frame->mat.pos.x - ((xMat4x3*)ent->model->Mat)->pos.x;
            delta.y += globals.player.ent.frame->mat.pos.y - ((xMat4x3*)ent->model->Mat)->pos.y;
            delta.z += globals.player.ent.frame->mat.pos.z - ((xMat4x3*)ent->model->Mat)->pos.z;
            frame->dpos.x = delta.x;
            frame->dpos.y = delta.y;
            frame->dpos.z = delta.z;
            ((xMat4x3*)ent->model->Mat)->pos.y += globals.player.Shaggy1.ExtraTransY;
            newTransY = 0.5f * globals.player.Shaggy1.ThrowAccel *
                        ((globals.player.Shaggy1.ThrowTimer - gACCEL_START) *
                         (globals.player.Shaggy1.ThrowTimer - gACCEL_START));
            frame->dpos.y += newTransY - globals.player.Shaggy1.ExtraTransY;
            globals.player.Shaggy1.ExtraTransY = newTransY;
            frame->mode |= 0x2;
        }
        else
        {
            if (globals.player.Shaggy1.ThrowEnded)
            {
                ent->pflags |= 0x4;
                if (globals.player.Shaggy1.ThrowTgtSpeed == 0.0f && globals.player.Shaggy1.ThrowTgt)
                {
                    frame->vel.x =
                        (globals.player.Shaggy1.ThrowTgtVec.x - frame->mat.pos.x) / sDefHangTime;
                    frame->vel.z =
                        (globals.player.Shaggy1.ThrowTgtVec.z - frame->mat.pos.z) / sDefHangTime;
                }
                else
                {
                    frame->vel.x = 0.0f;
                    frame->vel.z = globals.player.Shaggy1.ThrowTgtSpeed ?
                                       globals.player.Shaggy1.ThrowTgtSpeed :
                                       globals.player.Shaggy1.ThrowVel.z;
                    xMat3x3RMulVec(&frame->vel, (xMat3x3*)ent->model->Mat, &frame->vel);
                }
                frame->vel.y = globals.player.Shaggy1.ThrowVel.y +
                               globals.player.Shaggy1.ThrowAccel * (gACCEL_END - gACCEL_START);
                frame->mode |= 0x4;
                frame->mat.pos = ((xMat4x3*)ent->model->Mat)->pos;
                frame->mat.pos.y += frame->vel.y * dt;
                if (globals.player.Shaggy1.ThrowTgtAdjust)
                {
                    globals.player.Shaggy1.ThrowTgtAdjust = 2;
                }
            }
            retnow = 1;
        }

        hang = (_zEntHangable*)globals.player.Shaggy1.HangTarget;
        if (hang)
        {
            if (globals.player.Shaggy1.ThrowTimer >= 0.365f)
            {
                if (globals.player.Shaggy1.HangStart.y == 0.0f &&
                    globals.player.Shaggy1.HangRelative.y == 0.0f)
                {
                    globals.player.Shaggy1.HangStart = ((xMat4x3*)ent->model->Mat)->pos;
                    globals.player.Shaggy1.HangRelative.x =
                        hang->endpos.x - ((xMat4x3*)ent->model->Mat)->pos.x;
                    globals.player.Shaggy1.HangRelative.y =
                        hang->endpos.y - ((xMat4x3*)ent->model->Mat)->pos.y - 2.2f;
                    globals.player.Shaggy1.HangRelative.z =
                        hang->endpos.z - ((xMat4x3*)ent->model->Mat)->pos.z;
                    globals.player.Shaggy1.HangRelative.y +=
                        globals.player.Shaggy1.HangTrans.y - globals.player.Shaggy1.ReleaseTrans.y -
                        globals.player.ent.bound.sph.r;
                }
            }
            else
            {
                return;
            }

            curheight = ((xMat4x3*)ent->model->Mat)->pos.y - globals.player.Shaggy1.HangStart.y;
            relheight = globals.player.Shaggy1.HangRelative.y;
            if (curheight >= relheight)
            {
                guts->hangme = (_zEntHangable*)globals.player.Shaggy1.HangTarget;
                zEntHangable_SetShaggy(guts->hangme, (_zEnt*)ent);
                globals.player.Shaggy1.Grab = NULL;
                globals.player.Shaggy1.HangTarget = NULL;
                zEntEvent(guts->hangme, 0xCF);
                if (guts->hangme->follow)
                {
                    zEntEvent(guts->hangme->follow, 0xCF);
                }
                globals.player.Shaggy1.ThrowTimer = -1.0f;
                return;
            }

            solcnt = xMathSolveQuadratic(1.0f, -4.0f * sHangVirtualPeak,
                                         4.0f * sHangVirtualPeak * relheight,
                                         &sol[0], &sol[1]);
            if (solcnt)
            {
                if (solcnt == 2 && sol[1] < sol[0])
                {
                    b = sol[1];
                }
                else
                {
                    b = sol[0];
                }
                a = relheight - b;
                if (a > 0.0f)
                {
                    solcnt = 0;
                }
            }
            if (solcnt)
            {
                solcnt = xMathSolveQuadratic(a, b, -curheight, &sol[0], &sol[1]);
                if (solcnt && solcnt == 2 && sol[1] < sol[0])
                {
                    sol[0] = sol[1];
                }
            }
            if (solcnt)
            {
                idealx =
                    sol[0] * globals.player.Shaggy1.HangRelative.x + globals.player.Shaggy1.HangStart.x;
                idealz =
                    sol[0] * globals.player.Shaggy1.HangRelative.z + globals.player.Shaggy1.HangStart.z;
                ent->frame->mat.pos.y += frame->dpos.y;
                ent->frame->mat.pos.x = idealx;
                ent->frame->mat.pos.z = idealz;
                frame->mode = 0x1;
            }
            else
            {
                globals.player.Shaggy1.HangTarget = NULL;
            }

            if (!xStricmp(astate->Name, "ThrowIdle") && frame->vel.y < 1.5f)
            {
                frame->vel.y = 1.5f;
            }
            return;
        }

        if (retnow)
        {
            return;
        }
        globals.player.Shaggy1.ThrowTimer = -1.0f;
    }

    sVilMoveFunc(ent, s, dt, frame);
}

void Shaggy1_PlayerUpdate(_zEnt* shag, _xScene* sc, F32 dt)
{
    xAnimState* astate;

    globals.player.Shaggy1.ThrowEnded = 0;
    if (!globals.player.Shaggy1.GrabTry)
    {
        xEntBeginUpdate(shag, sc, dt);
    }

    astate = shag->model->Anim->Single->State;
    if (astate->UserFlags & 0x1000)
    {
        if (globals.player.Shaggy1.GrabTimer)
        {
            globals.player.Shaggy1.GrabTimer += dt;
            if (globals.player.Shaggy1.GrabTimer > 0.75f)
            {
                globals.player.Shaggy1.GrabTimer = 0.0f;
            }
            xMat3x3RotY((xMat3x3*)&shag->frame->mat, globals.player.Shaggy1.GrabRot);
            shag->frame->mat.pos = globals.player.ent.frame->mat.pos;
        }
        else
        {
            shag->frame->mat = globals.player.ent.frame->mat;
            if (shag->subType == 0x1D && !xStricmp(astate->Name, "CatchThrow"))
            {
                globals.player.Shaggy1.CatchHeight += globals.player.Shaggy1.CatchVel * dt;
                if (globals.player.Shaggy1.CatchHeight < 0.0f)
                {
                    globals.player.Shaggy1.CatchHeight = 0.0f;
                }
                shag->frame->mat.pos.y += globals.player.Shaggy1.CatchHeight;
            }
        }
        *(xMat4x3*)shag->model->Mat = shag->frame->mat;
        shag->model->Anim->Single->Time = globals.player.ent.model->Anim->Single->Time;
    }
    else
    {
        Shaggy1_MiddleUpdate(shag, sc, dt);
    }

    if (!globals.player.Shaggy1.GrabTry)
    {
        xEntEndUpdate(shag, sc, dt);
    }
    globals.player.Shaggy1.GrabTry = NULL;
}

static inline void NearestPointBox2D(xVec3* near, xVec3* vec, xVec3* lower, xVec3* upper)
{
    near->x = vec->x;
    near->y = lower->y;
    near->z = vec->z;
    if (vec->x < lower->x)
    {
        near->x = lower->x;
    }
    else if (vec->x > upper->x)
    {
        near->x = upper->x;
    }
    if (vec->z < lower->z)
    {
        near->z = lower->z;
    }
    else if (vec->z > upper->z)
    {
        near->z = upper->z;
    }
}

static inline void NearestPointSphere2D(xVec3* near, xVec3* vec, xVec3* center, F32 radius)
{
    F32 dx;
    F32 dz;
    F32 mag2;
    F32 magmul;

    near->y = center->y;
    dx = vec->x - center->x;
    dz = vec->z - center->z;
    mag2 = dx * dx + dz * dz;
    if (mag2 <= radius * radius)
    {
        near->x = vec->x;
        near->z = vec->z;
    }
    else
    {
        magmul = radius / sqrtf(mag2);
        near->x = dx * magmul + center->x;
        near->z = dz * magmul + center->z;
    }
}

static inline S32 PointInQuadXZ(xVec3* quad, xVec3* vec)
{
    F32 fNx;
    F32 fNz;
    F32 fDx;
    F32 fDz;

    fNx = quad[2].z - quad[0].z;
    fNz = quad[0].x - quad[2].x;
    fDx = vec->x - quad[0].x;
    fDz = vec->z - quad[0].z;
    if (fNx * fDx + fNz * fDz > 0.0f)
    {
        fNx = quad[1].z - quad[0].z;
        fNz = quad[0].x - quad[1].x;
        if (fNx * fDx + fNz * fDz > 0.0f)
        {
            return 0;
        }
        fNx = quad[2].z - quad[1].z;
        fNz = quad[1].x - quad[2].x;
        fDx = vec->x - quad[1].x;
        fDz = vec->z - quad[1].z;
        if (fNx * fDx + fNz * fDz > 0.0f)
        {
            return 0;
        }
    }
    else
    {
        fNx = quad[0].z - quad[3].z;
        fNz = quad[3].x - quad[0].x;
        if (fNx * fDx + fNz * fDz > 0.0f)
        {
            return 0;
        }
        fNx = quad[3].z - quad[2].z;
        fNz = quad[2].x - quad[3].x;
        fDx = vec->x - quad[3].x;
        fDz = vec->z - quad[3].z;
        if (fNx * fDx + fNz * fDz > 0.0f)
        {
            return 0;
        }
    }
    return 1;
}

static void NearestPointObb2D(xVec3* near, xVec3* vec, _xBox* b, xMat4x3* m)
{
    xVec3 test;
    S32 i;
    xVec3 corner[4];
    F32 planeD;
    F32 dx;
    F32 dz;
    F32 pdot;
    F32 ddot;
    F32 lerp;
    F32 testdist2;
    xVec3* v2;

    if (m->up.y > 0.0f)
    {
        corner[0].x = corner[3].x = b->lower.x;
        corner[1].x = corner[2].x = b->upper.x;
        corner[0].z = corner[1].z = b->lower.z;
        corner[2].z = corner[3].z = b->upper.z;
    }
    else
    {
        corner[0].x = corner[1].x = b->lower.x;
        corner[2].x = corner[3].x = b->upper.x;
        corner[0].z = corner[3].z = b->lower.z;
        corner[1].z = corner[2].z = b->upper.z;
    }
    corner[0].y = corner[1].y = corner[2].y = corner[3].y = b->lower.y;

    xMat3x3RMulVec(&corner[0], (xMat3x3*)m, &corner[0]);
    corner[0].x += m->pos.x;
    corner[0].y += m->pos.y;
    corner[0].z += m->pos.z;
    xMat3x3RMulVec(&corner[1], (xMat3x3*)m, &corner[1]);
    corner[1].x += m->pos.x;
    corner[1].y += m->pos.y;
    corner[1].z += m->pos.z;
    xMat3x3RMulVec(&corner[2], (xMat3x3*)m, &corner[2]);
    corner[2].x += m->pos.x;
    corner[2].y += m->pos.y;
    corner[2].z += m->pos.z;
    xMat3x3RMulVec(&corner[3], (xMat3x3*)m, &corner[3]);
    corner[3].x += m->pos.x;
    corner[3].y += m->pos.y;
    corner[3].z += m->pos.z;

    if (iabs(m->up.y) > 0.0001f && PointInQuadXZ(corner, vec))
    {
        planeD = corner[0].x * m->up.x + corner[0].y * m->up.y + corner[0].z * m->up.z;
        near->x = vec->x;
        near->z = vec->z;
        near->y = (planeD - m->up.x * vec->x - m->up.z * vec->z) / m->up.y;
        return;
    }

    for (i = 0; i < 4; i++)
    {
        v2 = &corner[(i + 1) & 3];
        dx = v2->x - corner[i].x;
        dz = v2->z - corner[i].z;
        pdot = dx * (vec->x - corner[i].x) + dz * (vec->z - corner[i].z);
        if (pdot <= 0.0f)
        {
            test = corner[i];
        }
        else
        {
            ddot = dx * dx + dz * dz;
            if (pdot >= ddot)
            {
                test = *v2;
            }
            else
            {
                lerp = pdot / sqrtf(ddot);
                test.x = corner[i].x + lerp * (v2->x - corner[i].x);
                test.y = corner[i].y + lerp * (v2->y - corner[i].y);
                test.z = corner[i].z + lerp * (v2->z - corner[i].z);
            }
        }
        testdist2 = (test.x - vec->x) * (test.x - vec->x) + (test.z - vec->z) * (test.z - vec->z);
        if (i == 0 || testdist2 < 1e20f)
        {
            *near = test;
        }
    }
}

static inline void NearestPointTrigger2D(xVec3* near, xVec3* vec, zEntTrigger* trig)
{
    p2EntTriggerAsset* tasset = (p2EntTriggerAsset*)(trig->asset + 1);

    switch (trig->subType)
    {
    case 0:
        NearestPointBox2D(near, vec, &tasset->p[0], &tasset->p[1]);
        break;
    case 1:
    case 2:
    case 3:
        NearestPointSphere2D(near, vec, &tasset->p[0], tasset->p[1].x);
        break;
    }
}

static inline void fn_80074C1C(xVec3* near, xVec3* vec, _xBound* b)
{
    switch (b->type)
    {
    case 1:
        NearestPointSphere2D(near, vec, &b->sph.center, b->sph.r);
        break;
    case 2:
        NearestPointBox2D(near, vec, &b->box.box.lower, &b->box.box.upper);
        break;
    case 3:
        NearestPointSphere2D(near, vec, &b->cyl.center, b->cyl.r);
        near->y -= b->cyl.h;
        break;
    case 4:
        NearestPointObb2D(near, vec, &b->box.box, b->mat);
        break;
    }
}

static inline F32 Shaggy1_CalcThrowAccel(F32 peak, F32* timepeak)
{
    U32 solcnt;
    F32 Ta;
    F32 Vthrow;
    F32 sol[2];

    Ta = gACCEL_END - gACCEL_START;
    Vthrow = globals.player.Shaggy1.ThrowVel.y;
    solcnt = xMathSolveQuadratic(
        Ta * Ta, 20.0f * (Ta * Ta) + 2.0f * Vthrow * Ta,
        40.0f * (globals.player.Shaggy1.ThrowPos.y - globals.player.Shaggy1.ReleaseTrans.y - peak) +
            Vthrow * Vthrow,
        &sol[0], &sol[1]);
    if (solcnt == 0)
    {
        sol[0] = 0.0f;
    }
    else
    {
        if (solcnt == 2)
        {
            sol[0] = sol[1];
        }
        if (sol[0] < 0.0f)
        {
            sol[0] = 0.0f;
        }
    }
    if (timepeak)
    {
        *timepeak = (Vthrow + sol[0] * Ta) / 20.0f;
    }
    return sol[0];
}

void Shaggy1_ChooseThrowTarget(_zEnt* ent)
{
    U32 i;
    F32 dot;
    F32 mag;
    F32 timepeak;
    xVec3 nearpt;
    F32 dx;
    F32 dz;
    F32 throwdist;
    F32 dropheight;
    F32 hangtime;
    _zEntHangable* hang;

    globals.player.Shaggy1.ThrowState = 3;
    globals.player.Shaggy1.HangTarget = NULL;
    globals.player.Shaggy1.ThrowAccel = Shaggy1_CalcThrowAccel(THROW_DEFHEIGHT, &timepeak);
    globals.player.Shaggy1.ThrowTgtSpeed = 0.0f;
    globals.player.Shaggy1.ThrowTgtAdjust = 0;
    sHangVirtualPeak = 0.75f + THROW_DEFHEIGHT;

    if (globals.player.Shaggy1.ThrowTgt)
    {
        switch (globals.player.Shaggy1.ThrowTgt->baseType)
        {
        case 0x1:
            NearestPointTrigger2D(&nearpt, &((xMat4x3*)ent->model->Mat)->pos,
                                  (zEntTrigger*)globals.player.Shaggy1.ThrowTgt);
            break;
        case 0x1D:
            fn_80074C1C(&nearpt, &((xMat4x3*)ent->model->Mat)->pos,
                        &((_zVolume*)globals.player.Shaggy1.ThrowTgt)->asset->bound);
            break;
        case 0xD:
            nearpt = *((_zMovePoint*)globals.player.Shaggy1.ThrowTgt)->pos;
            break;
        case 0x2:
        case 0x3:
        case 0x4:
        case 0x6:
        case 0xB:
        case 0xC:
        case 0x12:
        case 0x17:
        case 0x18:
        case 0x1B:
            nearpt = ((xMat4x3*)((_xEnt*)globals.player.Shaggy1.ThrowTgt)->model->Mat)->pos;
            break;
        default:
            return;
        }

        dx = nearpt.x - ent->frame->mat.pos.x;
        dz = nearpt.z - ent->frame->mat.pos.z;
        dot = dx * ent->frame->mat.at.x + dz * ent->frame->mat.at.z;
        mag = sqrtf(dx * dx + dz * dz);
        if (mag < 0.00001f ||
            dot < mag * (F32)cos(3.1415927f * globals.player.Shaggy1.ThrowTgtAngle / 180.0f))
        {
            globals.player.Shaggy1.ThrowTgtVec = ((xMat4x3*)globals.player.ent.model->Mat)->pos;
            sDefHangTime = timepeak + sqrtf(2.0f * THROW_DEFHEIGHT / 20.0f);
            return;
        }

        nearpt.y += globals.player.Shaggy1.ThrowTgtOffset;
        globals.player.Shaggy1.ThrowState = 1;
        globals.player.Shaggy1.ThrowTgtVec = nearpt;
        globals.player.Shaggy1.ThrowAccel =
            Shaggy1_CalcThrowAccel(globals.player.Shaggy1.ThrowTgtPeak, &timepeak);
        if (globals.player.Shaggy1.ThrowTgt->baseType == 0x17)
        {
            sHangVirtualPeak = 0.75f + globals.player.Shaggy1.ThrowTgtPeak;
            globals.player.Shaggy1.HangTarget = (_zEnt*)globals.player.Shaggy1.ThrowTgt;
            globals.player.Shaggy1.HangStart.y = 0.0f;
            globals.player.Shaggy1.HangRelative.y = 0.0f;
        }
        else
        {
            dropheight = ((xMat4x3*)globals.player.ent.model->Mat)->pos.y +
                         globals.player.Shaggy1.ThrowTgtPeak - nearpt.y;
            hangtime = timepeak;
            if (dropheight > 0.0f)
            {
                hangtime += sqrtf(2.0f * dropheight / 20.0f);
            }
            dx = nearpt.x - ((xMat4x3*)globals.player.ent.model->Mat)->pos.x;
            dz = nearpt.z - ((xMat4x3*)globals.player.ent.model->Mat)->pos.z;
            throwdist = sqrtf(dx * dx + dz * dz);
            globals.player.Shaggy1.ThrowTgtAdjust = 1;
            globals.player.Shaggy1.ThrowTgtSpeed = throwdist / hangtime;
        }
    }
    else
    {
        for (i = 0; i < 10; i++)
        {
            hang = (_zEntHangable*)globals.player.Shaggy1.Hangable[i];
            if (!hang)
            {
                return;
            }
            if (hang->enabled <= 0)
            {
                continue;
            }
            dx = ((xMat4x3*)hang->model->Mat)->pos.x -
                 (ent->frame->mat.pos.x + -2.0f * ent->frame->mat.at.x);
            dz = ((xMat4x3*)hang->model->Mat)->pos.z -
                 (ent->frame->mat.pos.z + -2.0f * ent->frame->mat.at.z);
            dot = dx * ent->frame->mat.at.x + dz * ent->frame->mat.at.z;
            mag = sqrtf(dx * dx + dz * dz);
            if (mag < 0.00001f || dot < 0.7071f * mag)
            {
                continue;
            }
            dx = ((xMat4x3*)hang->model->Mat)->pos.x - ((xMat4x3*)ent->model->Mat)->pos.x;
            dz = ((xMat4x3*)hang->model->Mat)->pos.z - ((xMat4x3*)ent->model->Mat)->pos.z;
            if (dx * dx + dz * dz > 25.0f)
            {
                continue;
            }
            globals.player.Shaggy1.HangTarget = hang;
            globals.player.Shaggy1.HangStart.y = 0.0f;
            globals.player.Shaggy1.HangRelative.y = 0.0f;
            if (hang->follow)
            {
                zEntEvent(hang->follow, eEventStop);
            }
            globals.player.Shaggy1.ThrowState = 1;
            globals.player.Shaggy1.ThrowTgtVec = ((xMat4x3*)hang->model->Mat)->pos;
            return;
        }
    }
}

static void ShaggyVil_HandleVisible(_zEnt* ent, U32 toEvent)
{
    ShaggyGuts* guts = zEntShaggy_FindGuts(ent);

    switch (toEvent)
    {
    case eEventInvisible:
    case eEventCollision_Visible_Off:
        if (1e10f == guts->oldY)
        {
            guts->oldY = ((xMat4x3*)ent->model->Mat)->pos.y;
            ((xMat4x3*)ent->model->Mat)->pos.y = 1e10f;
            ent->frame->mat.pos.y = ((xMat4x3*)ent->model->Mat)->pos.y;
            ent->frame->oldpos.y = ((xMat4x3*)ent->model->Mat)->pos.y;
            ent->bound.sph.center.y = ((xMat4x3*)ent->model->Mat)->pos.y + ent->bound.sph.r;
        }
        ent->flags &= ~0x1;
        break;
    case eEventVisible:
    case eEventCollision_Visible_On:
        if (1e10f != guts->oldY)
        {
            ((xMat4x3*)ent->model->Mat)->pos.y = guts->oldY;
            guts->oldY = 1e10f;
            ent->frame->mat.pos.y = ((xMat4x3*)ent->model->Mat)->pos.y;
            ent->frame->oldpos.y = ((xMat4x3*)ent->model->Mat)->pos.y;
            ent->bound.sph.center.y = ((xMat4x3*)ent->model->Mat)->pos.y + ent->bound.sph.r;
        }
        ent->flags |= 0x1;
        break;
    }
}

static S32 Shaggy1_EventCB(xBase* from, xBase* to, U32 toEvent, F32* toParam, xBase* toParamWidget)
{
    S32 retval = 0;

    switch (toEvent)
    {
    case eEventVisible:
    case eEventInvisible:
    case eEventCollision_Visible_On:
    case eEventCollision_Visible_Off:
        ShaggyVil_HandleVisible((_zEnt*)to, toEvent);
        break;
    case 0x21:
    {
        ShaggyGuts* guts = zEntShaggy_FindGuts((_zEnt*)to);

        if (guts->hangme)
        {
            zEntEvent(to, (xBase*)guts->hangme, 0x21);
        }
        retval = 1;
        break;
    }
    case 0xCF:
        if (toParamWidget)
        {
            if (toParam && *toParam == 1.0f)
            {
                xAnimPlaySetState(
                    ((_zEnt*)to)->model->Anim->Single,
                    xAnimTableGetState(((_zEnt*)to)->model->Anim->Table, "SharkHang"), 0.0f);
            }
            else
            {
                xAnimPlaySetState(((_zEnt*)to)->model->Anim->Single,
                                  xAnimTableGetState(((_zEnt*)to)->model->Anim->Table, "Hang"),
                                  0.0f);
            }
            zEntHangable_SetShaggy((_zEntHangable*)toParamWidget, (_zEnt*)to);
            zEntShaggy_FindGuts((_zEnt*)to)->hangme = (_zEntHangable*)toParamWidget;
        }
        retval = 1;
        break;
    case eEventShaggySwap:
        if (toParamWidget && toParamWidget->baseType == 2)
        {
            ShaggySwap((_zEnt*)to, (_zEnt*)toParamWidget);
        }
        break;
    case 0xD3:
        zEntShaggy_FindGuts((_zEnt*)to)->actionIndex = 1;
        break;
    case 0xDA:
        if (toParamWidget && toParamWidget->baseType == 0xD)
        {
            ShaggyGuts* guts = zEntShaggy_FindGuts((_zEnt*)to);

            guts->facept = (_zMovePoint*)toParamWidget;
            guts->facerot = 0.0f;
        }
        break;
    case eEventShaggyGrabbed:
    case eEventShaggyThrown:
        break;
    default:
        retval = sVilEventFunc(from, to, toEvent, toParam, toParamWidget);
        break;
    }

    return retval;
}

static S32 Shaggy0_EventCB(xBase* from, xBase* to, U32 toEvent, F32* toParam, xBase* toParamWidget)
{
    S32 retval = 0;

    switch (toEvent)
    {
    case eEventVisible:
    case eEventInvisible:
    case eEventCollision_Visible_On:
    case eEventCollision_Visible_Off:
        ShaggyVil_HandleVisible((_zEnt*)to, toEvent);
        break;
    case eEventShaggySwap:
        if (toParamWidget && toParamWidget->baseType == 2)
        {
            ShaggySwap((_zEnt*)to, (_zEnt*)toParamWidget);
        }
        break;
    case eEventShaggyGrabbed:
    case eEventShaggyThrown:
        break;
    default:
        retval = sVilEventFunc(from, to, toEvent, toParam, toParamWidget);
        break;
    }

    return retval;
}

static void Shaggy2_PrintModlTime(F32 time)
{
    printf("ModlTime = %f\n", time);
}

static U32 Shaggy2_StateChange(xAnimTransition* tran, xAnimSingle* anim, _zEnt* ent)
{
    ShaggyGuts* guts = zEntShaggy_FindGuts(ent);

    return guts && guts->chimneyState == tran->UserFlags && guts->chimneyTimer == 0.0f;
}

static U32 Shaggy2_Apex(xAnimTransition* tran, xAnimSingle* anim, _zEnt* ent)
{
    ShaggyGuts* guts = zEntShaggy_FindGuts(ent);

    return guts && guts->chimneyVel < 1.0f;
}

static U32 Shaggy2_RandCheck(xAnimTransition* tran, xAnimSingle* anim, _zEnt* ent)
{
    return (S32)irand() > 0;
}

xAnimTable* zEntShaggy_Anim2()
{
    xAnimTable* table;

    table = xAnimTableNew("Shaggy2", NULL);
    xAnimTableNewState(table, "Idle", 0x10, 0, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "Idle2", 0x10, 0, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "Shoot", 0x10, 0, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "Apex", 0x20, 0, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "Fall", 0x10, 0, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "Intro", 0x20, 0, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewTransition(table, "Idle Idle2 Fall Apex", "Intro",
                            (xAnimTransitionConditionalCallback)Shaggy2_StateChange, NULL, 0, 2,
                            0.0f, 0.0f, 1, 0, 0.0f, NULL);
    xAnimTableNewTransition(table, "Idle Idle2 Intro", "Shoot",
                            (xAnimTransitionConditionalCallback)Shaggy2_StateChange, NULL, 0, 3,
                            0.0f, 0.0f, 1, 0, 0.15f, NULL);
    xAnimTableNewTransition(table, "Shoot", "Apex",
                            (xAnimTransitionConditionalCallback)Shaggy2_Apex, NULL, 0, 0, 0.0f,
                            0.0f, 1, 0, 0.1f, NULL);
    xAnimTableNewTransition(table, "Apex", "Fall", NULL, NULL, 0x10, 0, 0.0f, 0.0f, 1, 0, 0.0f,
                            NULL);
    xAnimTableNewTransition(table, "Intro", "Idle", NULL, NULL, 0x10, 0, 0.0f, 0.0f, 1, 0, 0.1f,
                            NULL);
    xAnimTableNewTransition(table, "Intro", "Idle2",
                            (xAnimTransitionConditionalCallback)Shaggy2_RandCheck, NULL, 0x10, 0,
                            0.0f, 0.0f, 1, 0, 0.1f, NULL);

    return table;
}

xAnimTable* zEntShaggy_Chimney()
{
    xAnimTable* table;

    table = xAnimTableNew("ShaggyChimney", NULL);
    xAnimTableNewState(table, "Idle", 0x10, 0, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "Shoot", 0x20, 0, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "Land", 0x20, 0, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewTransition(table, "Idle", "Shoot",
                            (xAnimTransitionConditionalCallback)Shaggy2_StateChange, NULL, 0, 2,
                            0.0f, 0.0f, 1, 0, 0.0f, NULL);
    xAnimTableNewTransition(table, "Idle", "Land",
                            (xAnimTransitionConditionalCallback)Shaggy2_StateChange, NULL, 0, 4,
                            0.0f, 0.0f, 1, 0, 0.0f, NULL);
    xAnimTableNewTransition(table, "Shoot Land", "Idle", NULL, NULL, 0x10, 0, 0.0f, 0.0f, 1, 0,
                            0.0f, NULL);

    return table;
}

static void Shaggy2_Update(_zEnt* ent, _xScene* sc, F32 dt)
{
    xVec3 chimVec;
    xVec3 shagVec;
    F32 hdiff;
    xModelInstance* shagmodel;
    xModelInstance* chimmodel;
    xModelInstance* collmodel;
    xVec3* shaggyvec;
    F32 oldvel;
    U32 i;
    ShaggyGuts* guts = zEntShaggy_FindGuts(ent);

    shagmodel = ent->model;
    chimmodel = shagmodel->Next;
    collmodel = chimmodel->Next;
    shaggyvec = &((xMat4x3*)collmodel->Mat)[collmodel->BoneCount + 1].pos;

    for (;;)
    {
        switch (guts->chimneyState)
        {
        case 1:
            for (i = 0; i < gNumShaggy; i++)
            {
                if (gShaggy[i].chimneyState > 1)
                {
                    break;
                }
            }
            if (i != gNumShaggy)
            {
                break;
            }
            guts->chimneyState = 2;
            guts->chimneyTimer = 0.0f;
        case 2:
            if (guts->chimneyTimer == 0.0f)
            {
                collmodel->Flags |= 0x6;
                shaggyvec->y = -0.6f + ((xMat4x3*)shagmodel->Mat)->pos.y;
            }
            else
            {
                collmodel->Flags |= 0x1;
                shaggyvec->y += 3.0f * dt;
                if (shaggyvec->y > 0.0f)
                {
                    shaggyvec->y = 0.0f;
                }
                if (guts->chimneyTimer + dt >= guts->timeLaunch - 0.9f &&
                    guts->chimneyTimer < guts->timeLaunch - 0.9f)
                {
                    xSndPlay3D(0xF03423C, 0.5f, 0.0f, 0, 0x14,
                               &((xMat4x3*)collmodel->Mat)->pos, 12.0f);
                }
                if (guts->chimneyTimer >= guts->timeLaunch)
                {
                    guts->chimneyState = 3;
                    guts->chimneyTimer = 0.0f;
                    hdiff = guts->chimneyHeight - shaggyvec->y;
                    if (hdiff < 0.0f)
                    {
                        hdiff = 0.0f;
                    }
                    guts->chimneyVel = sqrtf(2.0f * guts->chimneyGrav * hdiff);
                }
            }
            break;
        case 3:
            if (guts->chimneyTimer >= guts->timeTop || guts->chimneyVel)
            {
                oldvel = guts->chimneyVel;
                shaggyvec->y += oldvel * dt;
                guts->chimneyVel -= guts->chimneyGrav * dt;
                if (oldvel > 0.0f && guts->chimneyVel <= 0.0f)
                {
                    guts->chimneyVel = 0.0f;
                    guts->chimneyTimer = 0.0f;
                    xSndPlay3D(0xF0343C3, 0.5f, 0.0f, 0, 0x14,
                               &((xMat4x3*)collmodel->Mat)->pos, 12.0f);
                }
            }
            if (shaggyvec->y <= -0.6f)
            {
                collmodel->Flags &= ~0x7;
                guts->chimneyState = 4;
                guts->chimneyTimer = 0.0f;
            }
            break;
        case 4:
            if (guts->chimneyTimer >= guts->timeEvent)
            {
                guts->chimneyState = 0;
                guts->chimneyTimer = 0.0f;
                zEntEvent(ent, eEventStop);
                if (guts->chimneyState)
                {
                    continue;
                }
            }
            break;
        }
        break;
    }

    xEntUpdate(ent, sc, dt);

    if (guts)
    {
        guts->chimneyTimer += dt;
    }

    ent->chkby |= 0x10;
    ent->penby |= 0x10;

    if (guts && guts->chimneyPlat)
    {
        iModelTagEval(chimmodel->Data, &ChimneyTag, chimmodel->Mat, &chimVec);
        iModelTagEval(collmodel->Data, &ShaggyTag, collmodel->Mat, &shagVec);
        if (chimVec.y < shagVec.y && (collmodel->Flags & 0x1))
        {
            chimVec.y = shagVec.y;
        }

        guts->chimneyPlat->frame->oldpos = ((xMat4x3*)guts->chimneyPlat->model->Mat)->pos;
        ((xMat4x3*)guts->chimneyPlat->model->Mat)->pos.x = ((xMat4x3*)chimmodel->Mat)->pos.x;
        ((xMat4x3*)guts->chimneyPlat->model->Mat)->pos.y = chimVec.y - 2.5f;
        ((xMat4x3*)guts->chimneyPlat->model->Mat)->pos.z = ((xMat4x3*)chimmodel->Mat)->pos.z;
        guts->chimneyPlat->frame->mat.pos = ((xMat4x3*)guts->chimneyPlat->model->Mat)->pos;
    }
}

S32 Shaggy2_EventCB(xBase* from, xBase* to, U32 toEvent, F32* toParam, xBase* toParamWidget)
{
    S32 rval = 1;

    switch (toEvent)
    {
    case 0x12:
    case 0x15:
    {
        ShaggyGuts* guts = zEntShaggy_FindGuts((_zEnt*)to);

        if (guts && guts->chimneyState == 0)
        {
            guts->chimneyState = 1;
            guts->chimneyTimer = 0.0f;
            guts->timeEvent = *toParam;
            guts->chimneyRand = (irand() >> 15) & 1;
        }
        break;
    }
    case 0x13:
        break;
    default:
        rval = zPlatformEventCB(from, to, toEvent, toParam, toParamWidget);
        break;
    }

    return rval;
}

S32 Shaggy2Plat_EventCB(xBase* from, xBase* to, U32 toEvent, F32* toParam, xBase* toParamWidget)
{
    for (U32 i = 0; i < gNumShaggy; i++)
    {
        if ((xBase*)gShaggy[i].chimneyPlat == to &&
            (toEvent == 0x12 || toEvent == 0x15 || toEvent == 0x20))
        {
            zEntEvent(from, 0, gShaggy[i].ent, toEvent, toParam, toParamWidget, 0);
            return 1;
        }
    }
    return zPlatformEventCB(from, to, toEvent, toParam, toParamWidget);
}

static void Shaggy2_Render(_xEnt* ent)
{
    if (xEntIsVisible(ent))
    {
        xModelRender(ent->model);
    }
}

static U32 Shaggy3_Move(xAnimTransition* tran, xAnimSingle* anim, _zPlatform* ent)
{
    _xEntFrame* frame = ent->frame;

    if (frame == NULL)
    {
        return 0;
    }
    return (frame->mode & 0x2) && frame->dpos.x * frame->dpos.x + frame->dpos.y * frame->dpos.y +
                                          frame->dpos.z * frame->dpos.z >=
                                      0.0005f;
}

static U32 Shaggy3_MoveDead(xAnimTransition* tran, xAnimSingle* anim, _zPlatform* ent)
{
    _xEntFrame* frame = ent->frame;

    if (frame == NULL)
    {
        return 0;
    }
    return !globals.player.Health && (frame->mode & 0x2) &&
           frame->dpos.x * frame->dpos.x + frame->dpos.y * frame->dpos.y +
                   frame->dpos.z * frame->dpos.z >=
               0.0005f;
}

static U32 Shaggy3_Stop(xAnimTransition* tran, xAnimSingle* anim, _zPlatform* ent)
{
    _xEntFrame* frame = ent->frame;

    if (frame == NULL)
    {
        return 0;
    }
    U8 stopped = 1;

    if ((frame->mode & 0x2) && !(frame->dpos.x * frame->dpos.x + frame->dpos.y * frame->dpos.y +
                                     frame->dpos.z * frame->dpos.z <
                                 0.0005f))
    {
        stopped = 0;
    }
    return stopped && globals.player.DamageTimer < 1.25f;
}

static U32 Shaggy3_StopScare(xAnimTransition* tran, xAnimSingle* anim, _zPlatform* ent)
{
    return zEntShaggy_FindGuts(ent)->stopEvent;
}

static U32 Shaggy3_Left(xAnimTransition* tran, xAnimSingle* anim, _zPlatform* ent)
{
    return globals.player.Health && gShaggy3TurnAngle > 0.0015f && !anim->Blend->State;
}

static U32 Shaggy3_Right(xAnimTransition* tran, xAnimSingle* anim, _zPlatform* ent)
{
    return globals.player.Health && gShaggy3TurnAngle < -0.0015f && !anim->Blend->State;
}

static U32 Shaggy3_Center(xAnimTransition* tran, xAnimSingle* anim, _zPlatform* ent)
{
    return xabs(gShaggy3TurnAngle) <= 0.0015f && !anim->Blend->State;
}

static U32 Shaggy3_MowerStopCB(U32 cbenum, xAnimActiveEffect* acteffect, xAnimSingle* single,
                               void* object)
{
    zPlatformEventCB(NULL, (xBase*)object, eEventStop, NULL, NULL);
    return 0;
}

xAnimTable* zEntShaggy_Anim3()
{
    xAnimTable* table;

    table = xAnimTableNew("Shaggy3", NULL);
    xAnimTableNewState(table, "Idle", 0x10, 0, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "Drive", 0x10, 0, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "Drive2", 0x10, 0, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "TurnLeft", 0x10, 0, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "TurnRight", 0x10, 0, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "StopReact", 0x20, 0, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "Talk", 0x10, 0, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewTransition(
        table, "Idle", "Drive", (xAnimTransitionConditionalCallback)Shaggy3_Move,
        (xAnimTransitionCallback)Shaggy1_IdleResetCB, 0, 0, 0.0f, 0.0f, 1, 0, 0.1f, NULL);
    xAnimTableNewTransition(table, "Drive Drive2 TurnLeft TurnRight", "Idle",
                            (xAnimTransitionConditionalCallback)Shaggy3_Stop, NULL, 0, 0, 0.0f,
                            0.0f, 2, 0, 0.1f, NULL);
    xAnimTableNewTransition(table, "Drive Drive2 TurnLeft TurnRight", "StopReact",
                            (xAnimTransitionConditionalCallback)Shaggy3_StopScare, NULL, 0, 0, 0.0f,
                            0.0f, 3, 0, 0.1f, NULL);
    xAnimTableNewTransition(table, "Drive Drive2 TurnRight", "TurnLeft",
                            (xAnimTransitionConditionalCallback)Shaggy3_Left, NULL, 0, 0, 0.0f,
                            0.0f, 1, 0, 0.15f, NULL);
    xAnimTableNewTransition(table, "Drive Drive2 TurnLeft", "TurnRight",
                            (xAnimTransitionConditionalCallback)Shaggy3_Right, NULL, 0, 0, 0.0f,
                            0.0f, 1, 0, 0.15f, NULL);
    xAnimTableNewTransition(
        table, "TurnLeft TurnRight", "Drive", (xAnimTransitionConditionalCallback)Shaggy3_Center,
        (xAnimTransitionCallback)Shaggy1_IdleResetCB, 0, 0, 0.0f, 0.0f, 1, 0, 0.15f, NULL);
    xAnimTableNewTransition(table, "StopReact", "Idle", NULL, NULL, 0x10, 0, 0.0f, 0.0f, 0, 0, 0.1f,
                            NULL);
    xAnimTableNewTransition(table, "Idle", "Talk", (xAnimTransitionConditionalCallback)Shaggy1_Talk,
                            NULL, 0, 0, 0.0f, 0.0f, 1, 0, 0.1f, NULL);
    xAnimTableNewTransition(table, "Talk", "Idle",
                            (xAnimTransitionConditionalCallback)Shaggy1_Notalk, NULL, 0, 0, 0.0f,
                            0.0f, 1, 0, 0.1f, NULL);
    xAnimTableNewTransition(table, "Drive", "Drive2",
                            (xAnimTransitionConditionalCallback)Shaggy1_IdlePick, NULL, 0, 3, 0.0f,
                            0.0f, 1, 0, 0.2f, NULL);
    xAnimTableNewTransition(
        table, "Drive2", "Drive", (xAnimTransitionConditionalCallback)Shaggy1_IdlePick,
        (xAnimTransitionCallback)Shaggy1_IdleResetCB, 0, 5, 0.0f, 0.0f, 1, 0, 0.2f, NULL);
    xAnimTableNewTransition(table, "Idle Drive TurnLeft TurnRight", "Drive2",
                            (xAnimTransitionConditionalCallback)Shaggy3_MoveDead,
                            (xAnimTransitionCallback)Shaggy1_IdleResetCB, 0, 0, 0.0f, 0.0f, 3, 0,
                            0.15f, NULL);
    xAnimStateNewEffect(xAnimTableGetState(table, "StopReact"), 9, 0.5f, 0.0f, Shaggy3_MowerStopCB,
                        0);

    return table;
}

static void Shaggy3_Update(_zEnt* ent, _xScene* sc, F32 dt)
{
    F32 dz;
    F32 dx;
    xAnimState* astate;
    _zParEmitterCustomSettings info;

    if (gTractorEmitter && ((_zPlatform*)ent)->moving)
    {
        info.custom_flags = 0x60;
        info.pos.x = -1.45f * ((xMat4x3*)ent->model->Mat)->at.x +
                     ((xMat4x3*)ent->model->Mat)->pos.x;
        info.pos.y = -1.45f * ((xMat4x3*)ent->model->Mat)->at.y +
                     ((xMat4x3*)ent->model->Mat)->pos.y;
        info.pos.z = -1.45f * ((xMat4x3*)ent->model->Mat)->at.z +
                     ((xMat4x3*)ent->model->Mat)->pos.z;
        info.vel.x = -1.25f * ((xMat4x3*)ent->model->Mat)->at.x;
        info.vel.y = 1.0f;
        info.vel.z = -1.25f * ((xMat4x3*)ent->model->Mat)->at.z;
        zParEmitterEmitCustom(gTractorEmitter, dt, &info);
    }

    ShaggyGuts* guts = zEntShaggy_FindGuts(ent);

    guts->oldAt2 = guts->oldAt;
    guts->oldAt = ((xMat4x3*)ent->model->Mat)->at;

    F32 oldAngle = atan2(guts->oldAt2.x, guts->oldAt2.z);
    F32 newAngle = atan2(guts->oldAt.x, guts->oldAt.z);

    gShaggy3TurnAngle = xAngleClamp(newAngle) - xAngleClamp(oldAngle);
    while (gShaggy3TurnAngle > 3.1415927f)
    {
        gShaggy3TurnAngle -= 6.2831855f;
    }
    while (gShaggy3TurnAngle < -3.1415927f)
    {
        gShaggy3TurnAngle += 6.2831855f;
    }

    if (globals.player.Health)
    {
        guts->idleTimer += dt;
    }

    if (guts->runEvent)
    {
        astate = ent->model->Anim->Single->State;
        if (xStricmp(astate->Name, "Talk") && xStricmp(astate->Name, "StopReact"))
        {
            zPlatformEventCB(NULL, ent, 0x12, NULL, NULL);
            guts->runEvent = 0;
        }
    }

    dx = ((xMat4x3*)globals.player.ent.model->Mat)->pos.x - ((xMat4x3*)ent->model->Mat)->pos.x;
    dz = ((xMat4x3*)globals.player.ent.model->Mat)->pos.z - ((xMat4x3*)ent->model->Mat)->pos.z;

    if (guts->stopMode == 3)
    {
        if (dx * dx + dz * dz > guts->stopParam * guts->stopParam)
        {
            guts->stopEvent = 1;
            guts->runEvent = 0;
        }
        else if (globals.player.drv.driver && globals.player.drv.driver == ent)
        {
            guts->stopEvent = 0;
            guts->runEvent = 1;
        }
    }

    zPlatform_Update(ent, sc, dt);
}

static S32 Shaggy3_EventCB(xBase* from, xBase* to, U32 toEvent, F32* toParam,
                           xBase* toParamWidget)
{
    xAnimState* astate = ((_zEnt*)to)->model->Anim->Single->State;
    ShaggyGuts* guts = zEntShaggy_FindGuts((_zEnt*)to);

    switch (toEvent)
    {
    case 0x20:
        if (guts->stopMode == 3)
        {
            guts->runEvent = 1;
            guts->stopEvent = 0;
        }
        else
        {
            return zPlatformEventCB(from, to, toEvent, toParam, toParamWidget);
        }
        break;
    case 0x12:
        guts->stopEvent = 0;
        if (!xStricmp(astate->Name, "Talk") || !xStricmp(astate->Name, "StopReact"))
        {
            guts->runEvent = 1;
        }
        else
        {
            return zPlatformEventCB(from, to, toEvent, toParam, toParamWidget);
        }
        break;
    case eEventStop:
        if (guts->stopMode)
        {
            guts->runEvent = 0;
            if (guts->stopMode == 2 || guts->stopMode == 3)
            {
                guts->stopEvent = 1;
            }
            else
            {
                return zPlatformEventCB(from, to, toEvent, toParam, toParamWidget);
            }
        }
        break;
    case 0x123:
        if (toParam)
        {
            guts->stopMode = toParam[0];
            guts->stopParam = toParam[1];
            if (!guts->stopMode && strcmp(astate->Name, "Drive") &&
                strcmp(astate->Name, "Drive2") && strcmp(astate->Name, "TurnLeft") &&
                strcmp(astate->Name, "TurnRight"))
            {
                guts->runEvent = 1;
                guts->stopEvent = 0;
            }
        }
        else
        {
            guts->stopMode = 1;
        }
        break;
    default:
        return zPlatformEventCB(from, to, toEvent, toParam, toParamWidget);
    }

    return 1;
}

static U32 Shaggy5_MoveTran(xAnimTransition* tran, xAnimSingle* anim, _zEnt* ent)
{
    ShaggyGuts* guts = zEntShaggy_FindGuts(ent);
    en_BEHAVIOUR_GOAL goal = ((st_zEntVillain*)ent)->goal;

    return (goal == ZVIL_GOAL_PATROL || goal == ZVIL_GOAL_FOLLOW) &&
           guts->prefMove == tran->UserFlags;
}

static U32 Shaggy5_IdleTran(xAnimTransition* tran, xAnimSingle* anim, _zEnt* ent)
{
    ShaggyGuts* guts = zEntShaggy_FindGuts(ent);
    en_BEHAVIOUR_GOAL goal = ((st_zEntVillain*)ent)->goal;

    return goal != ZVIL_GOAL_PATROL && goal != ZVIL_GOAL_FOLLOW &&
           guts->prefIdle == tran->UserFlags;
}

static U32 Shaggy5_WitchAttached(xAnimTransition* tran, xAnimSingle* anim, _zEnt* ent)
{
    return zEntShaggy_FindGuts(ent)->witchFollow != NULL;
}

static U32 Shaggy5_WitchDropped(xAnimTransition* tran, xAnimSingle* anim, _zEnt* ent)
{
    return zEntShaggy_FindGuts(ent)->witchDrop != NULL;
}

static U32 Shaggy5_Action(xAnimTransition* tran, xAnimSingle* anim, _zEnt* ent)
{
    ShaggyGuts* guts = zEntShaggy_FindGuts(ent);

    return guts->actionIndex == tran->UserFlags;
}

static U32 Shaggy5_ActionCB(xAnimTransition* tran, xAnimSingle* anim, _zEnt* ent)
{
    zEntShaggy_FindGuts(ent)->actionIndex = 0;
    return 0;
}

static U32 Shaggy5_EatMoveCB(xAnimTransition* tran, xAnimSingle* anim, _zEnt* ent)
{
    ShaggyGuts* guts = zEntShaggy_FindGuts(ent);

    guts->eatTimer = 1000.0f;
    guts->actionIndex = 0;
    return 0;
}

static U32 Shaggy5_EatStop(xAnimTransition* tran, xAnimSingle* anim, _zEnt* ent)
{
    ShaggyGuts* guts = zEntShaggy_FindGuts(ent);
    en_BEHAVIOUR_GOAL goal = ((st_zEntVillain*)ent)->goal;

    return goal != ZVIL_GOAL_PATROL && goal != ZVIL_GOAL_FOLLOW && guts->eatTimer > 0.0f;
}

static U32 Shaggy5_EatStopCB(xAnimTransition* tran, xAnimSingle* anim, _zEnt* ent)
{
    zEntShaggy_FindGuts(ent)->eatTimer = 2.0f;
    return 0;
}

static U32 Shaggy5_EatTimeUp(xAnimTransition* tran, xAnimSingle* anim, _zEnt* ent)
{
    return zEntShaggy_FindGuts(ent)->eatTimer <= 0.0f;
}

static U32 Shaggy5_GargRunPick(xAnimTransition* tran, xAnimSingle* anim, _zEnt* ent)
{
    ShaggyGuts* guts = zEntShaggy_FindGuts(ent);

    return guts->prefIdle == 3 && !guts->swap && guts->idleTimer > 0.0625f * tran->UserFlags &&
           guts->idleTimer - 0.5f < 0.0625f * tran->UserFlags;
}

static U32 Witch_HasShaggy(xAnimTransition* tran, xAnimSingle* anim, _zEnt* ent)
{
    for (S32 i = 0; i < (S32)gNumShaggy; i++)
    {
        if (ent == gShaggy[i].witchFollow)
        {
            return 1;
        }
    }
    return 0;
}

static U32 Witch_DropShaggyMove(xAnimTransition* tran, xAnimSingle* anim, _zEnt* ent)
{
    en_BEHAVIOUR_GOAL goal = ((st_zEntVillain*)ent)->goal;

    return (goal == ZVIL_GOAL_PATROL || goal == ZVIL_GOAL_FOLLOW) &&
           !Witch_HasShaggy(tran, anim, ent);
}

static U32 Witch_DropShaggyIdle(xAnimTransition* tran, xAnimSingle* anim, _zEnt* ent)
{
    en_BEHAVIOUR_GOAL goal = ((st_zEntVillain*)ent)->goal;

    return goal != ZVIL_GOAL_PATROL && goal != ZVIL_GOAL_FOLLOW &&
           !Witch_HasShaggy(tran, anim, ent);
}

static U32 Witch_GrabShaggyCB(xAnimTransition* tran, xAnimSingle* anim, _zEnt* ent)
{
    ent->collis->chk &= 0xDF;
    return 0;
}

static U32 Witch_DropShaggyCB(xAnimTransition* tran, xAnimSingle* anim, _zEnt* ent)
{
    ent->collis->chk |= 0x20;
    return 0;
}

void Shaggy5_WitchTransitions(xAnimTable* table)
{
    xAnimTransition* tran;

    tran = xAnimTableNewTransition(table, "Idle Idle_Fidget_1 Idle_Fidget_2", "Shaggy_hold",
                                   (xAnimTransitionConditionalCallback)Witch_HasShaggy,
                                   (xAnimTransitionCallback)Witch_GrabShaggyCB, 0, 0, 0.0f, 0.0f, 1,
                                   0, 0.15f, NULL);
    xAnimTableAddTransition(table, tran, "Bored Bored_Fidget_1 Bored_Fidget_1 Move Chase Surprize");
    xAnimTableAddTransition(table, tran,
                            "Taunt Taunt_1 Taunt_2 Hear_Start Hear_Loop Look_Start Look_Loop");
    xAnimTableAddTransition(table, tran,
                            "Investigate_Start Investigate AttackIdle Attack Attack_1 Attack_2");
    xAnimTableAddTransition(
        table, tran,
        "Combo Combo_1 Combo_2 Lob_Attack Lob_Combo Extend_Attack_Start Extend_Attack_Loop");
    xAnimTableAddTransition(
        table, tran,
        "Extend_Combo Hit_By_Gum Gummed Gummed_Resist Gummed_BreakFree Hit_When_Gummed");
    xAnimTableAddTransition(
        table, tran, "Bounced_When_Gummed Hit_By_Soap Soaped Soaped_Resist Soaped_BreakFree");
    xAnimTableAddTransition(
        table, tran, "Hit_When_Soaped Bounced_When_Soaped Dazed_Begin Dazed Dazed_Recovery Death");
    xAnimTableAddTransition(
        table, tran,
        "Death_From_Above Death_From_Dazed Death_Idle Jump_Windup Jump_In_Air Jump_Landing");
    xAnimTableNewTransition(
        table, "Shaggy_hold", "Idle", (xAnimTransitionConditionalCallback)Witch_DropShaggyIdle,
        (xAnimTransitionCallback)Witch_DropShaggyCB, 0, 0, 0.0f, 0.0f, 1, 0, 0.15f, NULL);
    xAnimTableNewTransition(
        table, "Shaggy_hold", "Move", (xAnimTransitionConditionalCallback)Witch_DropShaggyMove,
        (xAnimTransitionCallback)Witch_DropShaggyCB, 0, 0, 0.0f, 0.0f, 1, 0, 0.15f, NULL);
}

xAnimTable* zEntShaggy_Anim5()
{
    xAnimTable* table;
    xAnimTransition* tran;

    table = xAnimTableNew("Shaggy5", NULL);
    xAnimTableNewState(table, "Idle", 0x10, 0, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "Idle2", 0x10, 0, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "Idle3", 0x10, 0, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "Idle_scared", 0x10, 0, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "Idle_scared2", 0x10, 0, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "Idle_scared3", 0x10, 0, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "Idle_hungry", 0x10, 0, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "Idle_hungry2", 0x10, 0, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "Idle_garg", 0x10, 0, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "Idle_garg2", 0x10, 0, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "Idle_garg3", 0x10, 0, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "Idle_witch", 0x10, 0, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "Move_walk", 0x10, 0x2000004, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "Move_run", 0x10, 0x2000006, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "Move_hungry", 0x10, 0x2000004, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "Move_sneak", 0x10, 0x2000004, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "Move_turn", 0x10, 0x2000000, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "Chimney_intro", 0x10, 0, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "Fire_hit", 0x20, 0, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "Eat_tomove", 0x20, 0, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "Eat_tostop", 0x20, 0, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "Eat_loop", 0x10, 0, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "React_scare", 0x20, 0, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "React_death", 0, 0, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "Talk", 0x10, 0, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "Talk2", 0x10, 0, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "Talk3", 0x10, 0, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "Talk4", 0x10, 0, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewTransition(
        table,
        "Idle Idle2 Idle3 Idle_scared Idle_scared2 Idle_scared3 Idle_hungry Idle_hungry2 Idle_garg "
        "Idle_garg2 Idle_garg3 Move_sneak Move_run Move_hungry",
        "Move_walk", (xAnimTransitionConditionalCallback)Shaggy5_MoveTran, NULL, 0, 0, 0.0f, 0.0f,
        1, 0, 0.15f, NULL);
    xAnimTableNewTransition(
        table,
        "Idle Idle2 Idle3 Idle_scared Idle_scared2 Idle_scared3 Idle_hungry Idle_hungry2 Idle_garg "
        "Idle_garg2 Idle_garg3 Move_walk Move_run Move_hungry",
        "Move_sneak", (xAnimTransitionConditionalCallback)Shaggy5_MoveTran, NULL, 0, 3, 0.0f, 0.0f,
        1, 0, 0.15f, NULL);
    xAnimTableNewTransition(
        table,
        "Idle Idle2 Idle3 Idle_scared Idle_scared2 Idle_scared3 Idle_hungry Idle_hungry2 Idle_garg "
        "Idle_garg2 Idle_garg3 Move_walk Move_sneak Move_hungry",
        "Move_run", (xAnimTransitionConditionalCallback)Shaggy5_MoveTran,
        (xAnimTransitionCallback)Shaggy1_IdleResetCB, 0, 1, 0.0f, 0.0f, 1, 0, 0.15f, NULL);
    xAnimTableNewTransition(
        table,
        "Idle Idle2 Idle3 Idle_scared Idle_scared2 Idle_scared3 Idle_hungry Idle_hungry2 Idle_garg "
        "Idle_garg2 Idle_garg3 Move_walk Move_sneak Move_run",
        "Move_hungry", (xAnimTransitionConditionalCallback)Shaggy5_MoveTran, NULL, 0, 2, 0.0f, 0.0f,
        1, 0, 0.15f, NULL);
    xAnimTableNewTransition(
        table,
        "Move_walk Move_sneak Move_run Move_hungry Idle_scared Idle_scared2 Idle_scared3 "
        "Idle_hungry Idle_hungry2 Idle_garg Idle_garg2 Idle_garg3 ",
        "Idle", (xAnimTransitionConditionalCallback)Shaggy5_IdleTran,
        (xAnimTransitionCallback)Shaggy1_IdleResetCB, 0, 0, 0.0f, 0.0f, 1, 0, 0.15f, NULL);
    xAnimTableNewTransition(table,
                            "Move_walk Move_sneak Move_run Move_hungry Idle Idle2 Idle3 "
                            "Idle_hungry Idle_hungry2 Idle_garg Idle_garg2 Idle_garg3 ",
                            "Idle_scared", (xAnimTransitionConditionalCallback)Shaggy5_IdleTran,
                            (xAnimTransitionCallback)Shaggy1_IdleResetCB, 0, 1, 0.0f, 0.0f, 1, 0,
                            0.15f, NULL);
    xAnimTableNewTransition(
        table,
        "Move_walk Move_sneak Move_run Move_hungry Idle Idle2 Idle3 Idle_scared Idle_scared2 "
        "Idle_scared3 Idle_garg Idle_garg2 Idle_garg3 ",
        "Idle_hungry", (xAnimTransitionConditionalCallback)Shaggy5_IdleTran,
        (xAnimTransitionCallback)Shaggy1_IdleResetCB, 0, 2, 0.0f, 0.0f, 1, 0, 0.15f, NULL);
    xAnimTableNewTransition(table,
                            "Move_walk Move_sneak Move_run Move_hungry Idle Idle2 Idle3 "
                            "Idle_scared Idle_scared2 Idle_scared3 Idle_hungry Idle_hungry2 ",
                            "Idle_garg", (xAnimTransitionConditionalCallback)Shaggy5_IdleTran,
                            (xAnimTransitionCallback)Shaggy1_IdleResetCB, 0, 3, 0.0f, 0.0f, 1, 0,
                            0.15f, NULL);
    tran = xAnimTableNewTransition(table,
                                   "Idle Idle2 Idle3 Idle_scared Idle_scared2 Idle_scared3 "
                                   "Idle_hungry Idle_hungry2 Idle_garg Idle_garg2 Idle_garg3 ",
                                   "Idle_witch",
                                   (xAnimTransitionConditionalCallback)Shaggy5_WitchAttached, NULL,
                                   0, 0, 0.0f, 0.0f, 1, 0, 0.15f, NULL);
    xAnimTableAddTransition(table, tran, "Move_walk Move_sneak Move_run Move_turn Fire_hit");
    xAnimTableNewTransition(table, "Idle_witch", "Chimney_intro",
                            (xAnimTransitionConditionalCallback)Shaggy5_WitchDropped, NULL, 0, 0,
                            0.0f, 0.0f, 1, 0, 0.15f, NULL);
    xAnimTableNewTransition(
        table,
        "Idle Idle2 Idle3 Idle_scared Idle_scared2 Idle_scared3 Idle_hungry Idle_hungry2 Idle_garg "
        "Idle_garg2 Idle_garg3 Move_walk Move_sneak Move_run Move_hungry ",
        "Eat_tomove", (xAnimTransitionConditionalCallback)Shaggy5_Action,
        (xAnimTransitionCallback)Shaggy5_EatMoveCB, 0, 1, 0.0f, 0.0f, 2, 0, 0.15f, NULL);
    xAnimTableNewTransition(table, "Eat_tomove", "Move_hungry", NULL, NULL, 0x10, 0, 0.0f, 0.0f, 0,
                            0, 0.0f, NULL);
    xAnimTableNewTransition(
        table, "Move_hungry", "Eat_tostop", (xAnimTransitionConditionalCallback)Shaggy5_EatStop,
        (xAnimTransitionCallback)Shaggy5_EatStopCB, 0, 0, 0.0f, 0.0f, 3, 0, 0.15f, NULL);
    xAnimTableNewTransition(table, "Eat_tostop", "Eat_loop", NULL, NULL, 0x10, 0, 0.0f, 0.0f, 0, 0,
                            0.0f, NULL);
    xAnimTableNewTransition(table, "Eat_loop", "Idle_hungry",
                            (xAnimTransitionConditionalCallback)Shaggy5_EatTimeUp, NULL, 0, 0, 0.0f,
                            0.0f, 1, 0, 0.15f, NULL);
    xAnimTableNewTransition(
        table, "React_scare", "Idle", (xAnimTransitionConditionalCallback)Shaggy5_IdleTran,
        (xAnimTransitionCallback)Shaggy1_IdleResetCB, 0x10, 0, 0.0f, 0.0f, 1, 0, 0.15f, NULL);
    xAnimTableNewTransition(
        table, "React_scare", "Idle_scared", (xAnimTransitionConditionalCallback)Shaggy5_IdleTran,
        (xAnimTransitionCallback)Shaggy1_IdleResetCB, 0x10, 1, 0.0f, 0.0f, 1, 0, 0.15f, NULL);
    xAnimTableNewTransition(
        table, "React_scare", "Idle_hungry", (xAnimTransitionConditionalCallback)Shaggy5_IdleTran,
        (xAnimTransitionCallback)Shaggy1_IdleResetCB, 0x10, 2, 0.0f, 0.0f, 1, 0, 0.15f, NULL);
    xAnimTableNewTransition(
        table, "React_scare", "Idle_garg", (xAnimTransitionConditionalCallback)Shaggy5_IdleTran,
        (xAnimTransitionCallback)Shaggy1_IdleResetCB, 0x10, 3, 0.0f, 0.0f, 1, 0, 0.15f, NULL);
    xAnimTableNewTransition(table, "React_scare", "Move_walk",
                            (xAnimTransitionConditionalCallback)Shaggy5_MoveTran, NULL, 0x10, 0,
                            0.0f, 0.0f, 1, 0, 0.15f, NULL);
    xAnimTableNewTransition(table, "React_scare", "Move_sneak",
                            (xAnimTransitionConditionalCallback)Shaggy5_MoveTran, NULL, 0x10, 3,
                            0.0f, 0.0f, 1, 0, 0.15f, NULL);
    xAnimTableNewTransition(
        table, "React_scare", "Move_run", (xAnimTransitionConditionalCallback)Shaggy5_MoveTran,
        (xAnimTransitionCallback)Shaggy1_IdleResetCB, 0x10, 1, 0.0f, 0.0f, 1, 0, 0.15f, NULL);
    xAnimTableNewTransition(table, "React_scare", "Move_hungry",
                            (xAnimTransitionConditionalCallback)Shaggy5_MoveTran, NULL, 0x10, 2,
                            0.0f, 0.0f, 1, 0, 0.15f, NULL);
    xAnimTableNewTransition(table, "Idle", "Idle2",
                            (xAnimTransitionConditionalCallback)Shaggy1_IdlePick, NULL, 0, 4, 0.0f,
                            0.0f, 1, 0, 0.2f, NULL);
    xAnimTableNewTransition(table, "Idle2", "Idle",
                            (xAnimTransitionConditionalCallback)Shaggy1_IdlePick, NULL, 0, 7, 0.0f,
                            0.0f, 1, 0, 0.2f, NULL);
    xAnimTableNewTransition(table, "Idle", "Idle2",
                            (xAnimTransitionConditionalCallback)Shaggy1_IdlePick, NULL, 0, 12, 0.0f,
                            0.0f, 1, 0, 0.2f, NULL);
    xAnimTableNewTransition(table, "Idle2", "Idle",
                            (xAnimTransitionConditionalCallback)Shaggy1_IdlePick, NULL, 0, 14, 0.0f,
                            0.0f, 1, 0, 0.2f, NULL);
    xAnimTableNewTransition(table, "Idle", "Idle3",
                            (xAnimTransitionConditionalCallback)Shaggy1_IdlePick, NULL, 0, 0x11,
                            0.0f, 0.0f, 1, 0, 0.2f, NULL);
    xAnimTableNewTransition(
        table, "Idle3", "Idle", (xAnimTransitionConditionalCallback)Shaggy1_IdlePick,
        (xAnimTransitionCallback)Shaggy1_IdleResetCB, 0, 0x13, 0.0f, 0.0f, 1, 0, 0.2f, NULL);
    xAnimTableNewTransition(
        table, "Idle2 Idle3", "Idle", (xAnimTransitionConditionalCallback)Shaggy1_IdleLeave,
        (xAnimTransitionCallback)Shaggy1_IdleResetCB, 0, 0, 0.0f, 0.0f, 2, 0, 0.15f, NULL);
    xAnimTableNewTransition(table, "Idle_scared", "Idle_scared2",
                            (xAnimTransitionConditionalCallback)Shaggy1_IdlePick, NULL, 0, 4, 0.0f,
                            0.0f, 1, 0, 0.2f, NULL);
    xAnimTableNewTransition(table, "Idle_scared2", "Idle_scared",
                            (xAnimTransitionConditionalCallback)Shaggy1_IdlePick, NULL, 0, 7, 0.0f,
                            0.0f, 1, 0, 0.2f, NULL);
    xAnimTableNewTransition(table, "Idle_scared", "Idle_scared2",
                            (xAnimTransitionConditionalCallback)Shaggy1_IdlePick, NULL, 0, 12, 0.0f,
                            0.0f, 1, 0, 0.2f, NULL);
    xAnimTableNewTransition(table, "Idle_scared2", "Idle_scared",
                            (xAnimTransitionConditionalCallback)Shaggy1_IdlePick, NULL, 0, 14, 0.0f,
                            0.0f, 1, 0, 0.2f, NULL);
    xAnimTableNewTransition(table, "Idle_scared", "Idle_scared3",
                            (xAnimTransitionConditionalCallback)Shaggy1_IdlePick, NULL, 0, 0x11,
                            0.0f, 0.0f, 1, 0, 0.2f, NULL);
    xAnimTableNewTransition(
        table, "Idle_scared3", "Idle_scared", (xAnimTransitionConditionalCallback)Shaggy1_IdlePick,
        (xAnimTransitionCallback)Shaggy1_IdleResetCB, 0, 0x13, 0.0f, 0.0f, 1, 0, 0.2f, NULL);
    xAnimTableNewTransition(table, "Idle_scared2 Idle_scared3", "Idle_scared",
                            (xAnimTransitionConditionalCallback)Shaggy1_IdleLeave,
                            (xAnimTransitionCallback)Shaggy1_IdleResetCB, 0, 0, 0.0f, 0.0f, 2, 0,
                            0.15f, NULL);
    xAnimTableNewTransition(table, "Idle_hungry", "Idle_hungry2",
                            (xAnimTransitionConditionalCallback)Shaggy1_IdlePick, NULL, 0, 4, 0.0f,
                            0.0f, 1, 0, 0.2f, NULL);
    xAnimTableNewTransition(table, "Idle_hungry2", "Idle_hungry",
                            (xAnimTransitionConditionalCallback)Shaggy1_IdlePick, NULL, 0, 7, 0.0f,
                            0.0f, 1, 0, 0.2f, NULL);
    xAnimTableNewTransition(table, "Idle_hungry", "Idle_hungry2",
                            (xAnimTransitionConditionalCallback)Shaggy1_IdlePick, NULL, 0, 12, 0.0f,
                            0.0f, 1, 0, 0.2f, NULL);
    xAnimTableNewTransition(table, "Idle_hungry2", "Idle_hungry",
                            (xAnimTransitionConditionalCallback)Shaggy1_IdlePick, NULL, 0, 14, 0.0f,
                            0.0f, 1, 0, 0.2f, NULL);
    xAnimTableNewTransition(table, "Idle_hungry", "Idle_hungry2",
                            (xAnimTransitionConditionalCallback)Shaggy1_IdlePick, NULL, 0, 0x11,
                            0.0f, 0.0f, 1, 0, 0.2f, NULL);
    xAnimTableNewTransition(
        table, "Idle_hungry2", "Idle_hungry", (xAnimTransitionConditionalCallback)Shaggy1_IdlePick,
        (xAnimTransitionCallback)Shaggy1_IdleResetCB, 0, 0x13, 0.0f, 0.0f, 1, 0, 0.2f, NULL);
    xAnimTableNewTransition(
        table, "Idle_hungry2", "Idle_hungry", (xAnimTransitionConditionalCallback)Shaggy1_IdleLeave,
        (xAnimTransitionCallback)Shaggy1_IdleResetCB, 0, 0, 0.0f, 0.0f, 2, 0, 0.15f, NULL);
    xAnimTableNewTransition(table, "Idle_garg", "Idle_garg2",
                            (xAnimTransitionConditionalCallback)Shaggy1_IdlePick, NULL, 0, 4, 0.0f,
                            0.0f, 1, 0, 0.2f, NULL);
    xAnimTableNewTransition(table, "Idle_garg2", "Idle_garg",
                            (xAnimTransitionConditionalCallback)Shaggy1_IdlePick, NULL, 0, 7, 0.0f,
                            0.0f, 1, 0, 0.2f, NULL);
    xAnimTableNewTransition(table, "Idle_garg", "Idle_garg2",
                            (xAnimTransitionConditionalCallback)Shaggy1_IdlePick, NULL, 0, 12, 0.0f,
                            0.0f, 1, 0, 0.2f, NULL);
    xAnimTableNewTransition(table, "Idle_garg2", "Idle_garg",
                            (xAnimTransitionConditionalCallback)Shaggy1_IdlePick, NULL, 0, 14, 0.0f,
                            0.0f, 1, 0, 0.2f, NULL);
    xAnimTableNewTransition(table, "Idle_garg", "Idle_garg3",
                            (xAnimTransitionConditionalCallback)Shaggy1_IdlePick, NULL, 0, 0x11,
                            0.0f, 0.0f, 1, 0, 0.2f, NULL);
    xAnimTableNewTransition(
        table, "Idle_garg3", "Idle_garg", (xAnimTransitionConditionalCallback)Shaggy1_IdlePick,
        (xAnimTransitionCallback)Shaggy1_IdleResetCB, 0, 0x13, 0.0f, 0.0f, 1, 0, 0.2f, NULL);
    xAnimTableNewTransition(table, "Idle_garg2 Idle_garg3", "Idle_garg",
                            (xAnimTransitionConditionalCallback)Shaggy1_IdleLeave,
                            (xAnimTransitionCallback)Shaggy1_IdleResetCB, 0, 0, 0.0f, 0.0f, 2, 0,
                            0.15f, NULL);
    xAnimTableNewTransition(table, "Move_run", "Fire_hit",
                            (xAnimTransitionConditionalCallback)Shaggy5_GargRunPick, NULL, 0, 0x46,
                            0.0f, 0.0f, 1, 0, 0.15f, NULL);
    xAnimTableNewTransition(
        table, "Move_run", "Fire_hit", (xAnimTransitionConditionalCallback)Shaggy5_GargRunPick,
        (xAnimTransitionCallback)Shaggy1_IdleResetCB, 0, 0x7b, 0.0f, 0.0f, 1, 0, 0.15f, NULL);
    xAnimTableNewTransition(table, "Fire_hit", "Move_run", NULL, NULL, 0x10, 0, 0.0f, 0.0f, 1, 0,
                            0.15f, NULL);
    xAnimTableNewTransition(
        table,
        "Idle Idle2 Idle3 Idle_scared Idle_scared2 Idle_scared3 Idle_hungry Idle_hungry2 Idle_garg "
        "Idle_garg2 Idle_garg3 Move_walk Move_sneak Move_run",
        "React_scare", (xAnimTransitionConditionalCallback)Shaggy1_ScareReact, NULL, 0, 0, 0.0f,
        0.0f, 11, 0, 0.15f, NULL);
    tran = xAnimTableNewTransition(
        table,
        "Idle Idle2 Idle3 Idle_scared Idle_scared2 Idle_scared3 Idle_hungry Idle_hungry2 Idle_garg "
        "Idle_garg2 Idle_garg3 Move_walk Move_sneak Move_run Move_hungry React_scare",
        "React_death", (xAnimTransitionConditionalCallback)Shaggy1_DeathReact, NULL, 0, 0, 0.0f,
        0.0f, 12, 0, 0.15f, NULL);
    xAnimTableAddTransition(table, tran, "Chimney_intro Fire_hit Eat_tomove Eat_tostop Eat_loop");

    return table;
}

static void Shaggy5_Move(_xEnt* ent, _xScene* sc, F32 dt, _xEntFrame* frame)
{
    sVilMoveFunc(ent, sc, dt, frame);
}

static void Shaggy5_Update(_zEnt* ent, _xScene* sc, F32 dt)
{
    _zParEmitterCustomSettings info;

    if (xEntIsVisible(ent))
    {
        if (gBurningManEmitter && zEntShaggy_FindGuts(ent)->prefIdle == 3)
        {
            info.custom_flags = 0x20;
            info.pos.x = -0.15f * ((xMat4x3*)ent->model->Mat)->at.x +
                         ((xMat4x3*)ent->model->Mat)->pos.x;
            info.pos.y = -0.15f * ((xMat4x3*)ent->model->Mat)->at.y +
                         ((xMat4x3*)ent->model->Mat)->pos.y;
            info.pos.z = -0.15f * ((xMat4x3*)ent->model->Mat)->at.z +
                         ((xMat4x3*)ent->model->Mat)->pos.z;
            info.pos.y += 0.9f;
            zParEmitterEmitCustom(gBurningManEmitter, dt, &info);
        }

        ShaggyGuts* guts = zEntShaggy_FindGuts(ent);

        if (guts->witchFollow)
        {
            xEntBeginUpdate(ent, sc, dt);
            xEntEndUpdate(ent, sc, dt);
            return;
        }

        if (!guts->swap)
        {
            guts->idleTimer += dt;
        }
        if (guts->talkTimer)
        {
            guts->talkTimer -= dt;
            guts->talkSwitch -= dt;
            if (guts->talkTimer < 0.0f)
            {
                guts->talkTimer = 0.0f;
            }
        }

        if (guts->witchDrop)
        {
            ((xMat4x3*)ent->model->Mat)->pos.y += ent->frame->vel.y * dt;
            ent->frame->vel.y -= 10.0f * dt;
            if (((xMat4x3*)ent->model->Mat)->pos.y <
                -2.0f + ((xMat4x3*)guts->witchDrop->model->Mat)->pos.y)
            {
                guts->witchDrop = NULL;
                ent->collis->chk = 0x26;
                ent->chkby = 0x1C;
                ShaggyVil_HandleVisible(ent, eEventInvisible);
                zEntEvent(ent, 2);
            }
            xEntBeginUpdate(ent, sc, dt);
            xEntEndUpdate(ent, sc, dt);
            return;
        }

        guts->eatTimer -= dt;
        if (guts->geronimoChase)
        {
            F32 dx = ((xMat4x3*)ent->model->Mat)->pos.x -
                     ((xMat4x3*)guts->geronimoChase->model->Mat)->pos.x;
            F32 dz = ((xMat4x3*)ent->model->Mat)->pos.z -
                     ((xMat4x3*)guts->geronimoChase->model->Mat)->pos.z;
            if (dx * dx + dz * dz < 16.0f)
            {
                ((st_zEntVillain*)ent)->spd_move += 0.5f * dt;
                ((st_zEntVillain*)ent)->spd_chase += 0.5f * dt;
                if (((st_zEntVillain*)ent)->spd_move > 4.1f)
                {
                    ((st_zEntVillain*)ent)->spd_move = 4.1f;
                }
                if (((st_zEntVillain*)ent)->spd_chase > 4.1f)
                {
                    ((st_zEntVillain*)ent)->spd_chase = 4.1f;
                }
            }
            if (dx * dx + dz * dz > 36.0f)
            {
                ((st_zEntVillain*)ent)->spd_move -= 0.5f * dt;
                ((st_zEntVillain*)ent)->spd_chase -= 0.5f * dt;
                if (((st_zEntVillain*)ent)->spd_move < 3.6f)
                {
                    ((st_zEntVillain*)ent)->spd_move = 3.6f;
                }
                if (((st_zEntVillain*)ent)->spd_chase < 3.6f)
                {
                    ((st_zEntVillain*)ent)->spd_chase = 3.6f;
                }
            }
        }

        sVilUpdateFunc(ent, sc, dt);
        if (guts->swap)
        {
            ShaggySwap(ent, guts->swap);
        }
    }
}

static void ShaggySwap(_zEnt* src, _zEnt* dest)
{
    ShaggyGuts* sguts = zEntShaggy_FindGuts(src);
    ShaggyGuts* dguts = zEntShaggy_FindGuts(dest);
    char* dstState;
    char* srcState;

    if (!sguts->swap)
    {
        sguts->swap = dest;
    }

    if (xEntIsVisible(src) && !src->model->Anim->Single->Blend->State)
    {
        dstState = "Idle";
        srcState = src->model->Anim->Single->State->Name;
        if (sguts->shaggyType == 5)
        {
            switch (dguts->shaggyType)
            {
            case 0:
            case 5:
                sguts->prefIdle = 0;
                if (strcmp(srcState, "Idle") != 0)
                {
                    return;
                }
                break;
            case 1:
                sguts->prefIdle = 1;
                if (strcmp(srcState, "Idle_scared") != 0)
                {
                    return;
                }
                break;
            case 7:
                sguts->prefIdle = 2;
                if (strcmp(srcState, "Idle_hungry") != 0)
                {
                    return;
                }
                break;
            }
        }
        else
        {
            if (strcmp(srcState, "Idle") != 0)
            {
                return;
            }
            dguts->prefIdle = 0;
            switch (sguts->shaggyType)
            {
            case 1:
                dguts->prefIdle = 1;
                dstState = "Idle_scared";
                break;
            case 7:
                dguts->prefIdle = 2;
                dstState = "Idle_hungry";
                break;
            }
        }

        sguts->swap = NULL;
        zEntEvent(src, dest, eEventVisible);
        *(xMat4x3*)dest->model->Mat = *(xMat4x3*)src->model->Mat;
        dest->frame->mat = src->frame->mat;
        dest->frame->oldpos = src->frame->oldpos;
        dest->bound.sph.center = src->bound.sph.center;
        xAnimPlaySetState(dest->model->Anim->Single,
                          xAnimTableGetState(dest->model->Anim->Table, dstState), 0.0f);
        dest->model->Anim->Single->Time = src->model->Anim->Single->Time;
        zEntEvent(src, eEventInvisible);
        memcpy((xMat4x3*)dest->model->Mat + 1, (xMat4x3*)src->model->Mat + 1,
               dest->model->BoneCount << 6);
    }
}

static S32 Shaggy5_EventCB(xBase* from, xBase* to, U32 toEvent, F32* toParam, xBase* toParamWidget)
{
    S32 retval = 0;
    ShaggyGuts* guts = zEntShaggy_FindGuts((_zEnt*)to);

    switch (toEvent)
    {
    case eEventVisible:
    case eEventInvisible:
    case eEventCollision_Visible_On:
    case eEventCollision_Visible_Off:
        ShaggyVil_HandleVisible((_zEnt*)to, toEvent);
        break;
    case 0xCF:
        if (toParamWidget && toParamWidget->baseType == 2)
        {
            guts->witchFollow = (_zEnt*)toParamWidget;
            ((_zEnt*)to)->collis->chk = 0;
            ((_zEnt*)to)->chkby = 0;
            ((_zEnt*)to)->frame->vel.y = 0.0f;
        }
        retval = 1;
        break;
    case 0xD0:
        if (toParamWidget && toParamWidget->baseType == 6 && guts->witchFollow)
        {
            guts->witchFollow = NULL;
            guts->witchDrop = (_zEnt*)toParamWidget;
            ((_zEnt*)to)->frame->vel.y = 0.0f;
            guts->dropDiff.x = ((xMat4x3*)guts->witchDrop->model->Mat)->pos.x -
                               ((xMat4x3*)((_zEnt*)to)->model->Mat)->pos.x;
            guts->dropDiff.y = ((xMat4x3*)guts->witchDrop->model->Mat)->pos.y -
                               ((xMat4x3*)((_zEnt*)to)->model->Mat)->pos.y;
            guts->dropDiff.z = ((xMat4x3*)guts->witchDrop->model->Mat)->pos.z -
                               ((xMat4x3*)((_zEnt*)to)->model->Mat)->pos.z;
        }
        retval = 1;
        break;
    case eEventShaggySwap:
        if (toParamWidget && toParamWidget->baseType == 2)
        {
            ShaggySwap((_zEnt*)to, (_zEnt*)toParamWidget);
        }
        break;
    case 0xD2:
        guts->prefIdle = toParam[0];
        guts->prefMove = toParam[1];
        if (guts->prefIdle > 3)
        {
            guts->prefIdle = 3;
        }
        if (guts->prefMove > 3)
        {
            guts->prefMove = 3;
        }
        break;
    case 0xD3:
        guts->actionIndex = toParam[0];
        if (guts->actionIndex == 1 && toParamWidget)
        {
            F32 tempToParam[4] = { 0.0f, 0.0f, 0.0f, 0.0f };

            retval = sVilEventFunc(from, to, 0xCD, tempToParam, toParamWidget);
        }
        break;
    default:
        retval = sVilEventFunc(from, to, toEvent, toParam, toParamWidget);
        break;
    }

    return retval;
}

static U32 Shaggy8_FlipOver(xAnimTransition* tran, xAnimSingle* anim, _zEnt* ent)
{
    ShaggyGuts* guts = zEntShaggy_FindGuts(ent);

    return guts && guts->barrelButted;
}

static U32 Shaggy8_FlipOverCB(xAnimTransition* tran, xAnimSingle* anim, _zEnt* ent)
{
    zEntShaggy_FindGuts(ent)->barrelButted = 0;
    return 0;
}

static U32 Shaggy8_FlipBack(xAnimTransition* tran, xAnimSingle* anim, _zEnt* ent)
{
    ShaggyGuts* guts = zEntShaggy_FindGuts(ent);

    return guts && guts->barrelFellOver;
}

static U32 Shaggy8_FlipBackCB(xAnimTransition* tran, xAnimSingle* anim, _zEnt* ent)
{
    zEntShaggy_FindGuts(ent)->barrelFellOver = 0;
    return 0;
}

static U32 Shaggy8_Stopped(xAnimTransition* tran, xAnimSingle* anim, _zEnt* ent)
{
    en_BEHAVIOUR_GOAL goal = ((st_zEntVillain*)ent)->goal;

    return goal != ZVIL_GOAL_PATROL && goal != ZVIL_GOAL_FOLLOW;
}

static U32 Shaggy8_Crash(xAnimTransition* tran, xAnimSingle* anim, _zEnt* ent)
{
    return zEntShaggy_FindGuts(ent)->actionIndex != 0;
}

static U32 Shaggy8_Bounce(xAnimTransition* tran, xAnimSingle* anim, _zEnt* ent)
{
    return zEntShaggy_FindGuts(ent)->barrelBounced != 0;
}

static U32 Shaggy8_BounceCB(xAnimTransition* tran, xAnimSingle* anim, _zEnt* ent)
{
    zEntShaggy_FindGuts(ent)->barrelBounced = 0;
    return 0;
}

static U32 Shaggy8_Getup(xAnimTransition* tran, xAnimSingle* anim, _zEnt* ent)
{
    return zEntShaggy_FindGuts(ent)->swapTimer >= 3.0f;
}

static U32 Shaggy8_BarrelDestroy(U32 cbenum, xAnimActiveEffect* acteffect, xAnimSingle* single,
                                 void* object)
{
    _zEnt* ent = (_zEnt*)object;
    xModelInstance* inst;

    zEntShaggy_FindGuts(ent);
    inst = ent->model->Next;
    if (inst)
    {
        inst->Flags &= ~0x3;
    }
    return 0;
}

xAnimTable* zEntShaggy_Anim8()
{
    xAnimTable* table;

    table = xAnimTableNew("Shaggy8", NULL);
    xAnimTableNewState(table, "Idle", 0x10, 0, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "Idle2", 0x10, 0, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "Idle3", 0x10, 0, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "Stand_idle", 0x10, 0x10001, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "Stand_walk", 0x10, 0x2010004, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "Stand_run", 0x10, 0x2010006, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "Stand_bounce", 0x20, 0, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "Crash", 0x20, 0, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "Sit", 0x10, 0, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "GetUp", 0x20, 0, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "NoBarrelIdle", 0x10, 0, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "FlipOver", 0x20, 0, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "FlipBack", 0x20, 0, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewTransition(table, "Crash", "Sit", NULL, NULL, 0x10, 0, 0.0f, 0.0f, 1, 0, 0.0f,
                            NULL);
    xAnimTableNewTransition(table, "GetUp", "NoBarrelIdle", NULL, NULL, 0x10, 0, 0.0f, 0.0f, 1, 0,
                            0.0f, NULL);
    xAnimTableNewTransition(
        table, "Idle Idle2 Idle3", "FlipOver", (xAnimTransitionConditionalCallback)Shaggy8_FlipOver,
        (xAnimTransitionCallback)Shaggy8_FlipOverCB, 0, 0, 0.0f, 0.0f, 2, 0, 0.15f, NULL);
    xAnimTableNewTransition(table, "Stand_idle Stand_walk Stand_run", "FlipBack",
                            (xAnimTransitionConditionalCallback)Shaggy8_FlipBack,
                            (xAnimTransitionCallback)Shaggy8_FlipBackCB, 0, 0, 0.0f, 0.0f, 2, 0,
                            0.15f, NULL);
    xAnimTableNewTransition(table, "Stand_idle Stand_run", "Stand_walk",
                            (xAnimTransitionConditionalCallback)Shaggy5_MoveTran, NULL, 0, 0, 0.0f,
                            0.0f, 3, 0, 0.15f, NULL);
    xAnimTableNewTransition(table, "Stand_idle Stand_run", "Stand_walk",
                            (xAnimTransitionConditionalCallback)Shaggy5_MoveTran, NULL, 0, 3, 0.0f,
                            0.0f, 3, 0, 0.15f, NULL);
    xAnimTableNewTransition(table, "Stand_idle Stand_walk", "Stand_run",
                            (xAnimTransitionConditionalCallback)Shaggy5_MoveTran, NULL, 0, 1, 0.0f,
                            0.0f, 3, 0, 0.15f, NULL);
    xAnimTableNewTransition(table, "Stand_walk Stand_run", "Stand_idle",
                            (xAnimTransitionConditionalCallback)Shaggy8_Stopped, NULL, 0, 0, 0.0f,
                            0.0f, 3, 0, 0.15f, NULL);
    xAnimTableNewTransition(table, "Stand_idle Stand_walk Stand_run", "Crash",
                            (xAnimTransitionConditionalCallback)Shaggy8_Crash,
                            (xAnimTransitionCallback)Shaggy5_ActionCB, 0, 0, 0.0f, 0.0f, 4, 0,
                            0.15f, NULL);
    xAnimTableNewTransition(table, "Sit", "GetUp",
                            (xAnimTransitionConditionalCallback)Shaggy8_Getup, NULL, 0, 0, 0.0f,
                            0.0f, 1, 0, 0.1f, NULL);
    xAnimTableNewTransition(table, "Stand_idle Stand_walk Stand_run Stand_bounce", "Stand_bounce",
                            (xAnimTransitionConditionalCallback)Shaggy8_Bounce,
                            (xAnimTransitionCallback)Shaggy8_BounceCB, 0, 0, 0.0f, 0.0f, 5, 0,
                            0.08f, NULL);
    xAnimTableNewTransition(table, "Stand_bounce", "Stand_walk",
                            (xAnimTransitionConditionalCallback)Shaggy5_MoveTran, NULL, 0x10, 0,
                            0.0f, 0.0f, 3, 0, 0.15f, NULL);
    xAnimTableNewTransition(table, "Stand_bounce", "Stand_walk",
                            (xAnimTransitionConditionalCallback)Shaggy5_MoveTran, NULL, 0x10, 3,
                            0.0f, 0.0f, 3, 0, 0.15f, NULL);
    xAnimTableNewTransition(table, "Stand_bounce", "Stand_run",
                            (xAnimTransitionConditionalCallback)Shaggy5_MoveTran, NULL, 0x10, 1,
                            0.0f, 0.0f, 3, 0, 0.15f, NULL);
    xAnimTableNewTransition(table, "Stand_bounce", "Stand_idle",
                            (xAnimTransitionConditionalCallback)Shaggy8_Stopped, NULL, 0x10, 0,
                            0.0f, 0.0f, 3, 0, 0.15f, NULL);
    xAnimTableNewTransition(table, "FlipOver", "Stand_idle", NULL, NULL, 0x10, 0, 0.0f, 0.0f, 0, 0,
                            0.0f, NULL);
    xAnimTableNewTransition(table, "FlipBack", "Idle", NULL,
                            (xAnimTransitionCallback)Shaggy1_IdleResetCB, 0x10, 0, 0.0f, 0.0f, 0, 0,
                            0.1f, NULL);
    xAnimTableNewTransition(table, "Idle", "Idle2",
                            (xAnimTransitionConditionalCallback)Shaggy1_IdlePick, NULL, 0, 4, 0.0f,
                            0.0f, 1, 0, 0.2f, NULL);
    xAnimTableNewTransition(table, "Idle2", "Idle",
                            (xAnimTransitionConditionalCallback)Shaggy1_IdlePick, NULL, 0, 7, 0.0f,
                            0.0f, 1, 0, 0.2f, NULL);
    xAnimTableNewTransition(table, "Idle", "Idle2",
                            (xAnimTransitionConditionalCallback)Shaggy1_IdlePick, NULL, 0, 12, 0.0f,
                            0.0f, 1, 0, 0.2f, NULL);
    xAnimTableNewTransition(table, "Idle2", "Idle",
                            (xAnimTransitionConditionalCallback)Shaggy1_IdlePick, NULL, 0, 14, 0.0f,
                            0.0f, 1, 0, 0.2f, NULL);
    xAnimTableNewTransition(table, "Idle", "Idle3",
                            (xAnimTransitionConditionalCallback)Shaggy1_IdlePick, NULL, 0, 0x11,
                            0.0f, 0.0f, 1, 0, 0.2f, NULL);
    xAnimTableNewTransition(
        table, "Idle3", "Idle", (xAnimTransitionConditionalCallback)Shaggy1_IdlePick,
        (xAnimTransitionCallback)Shaggy1_IdleResetCB, 0, 0x13, 0.0f, 0.0f, 1, 0, 0.2f, NULL);
    xAnimStateNewEffect(xAnimTableGetState(table, "Crash"), 1, 0.2f, 0.0f, Shaggy8_BarrelDestroy,
                        0);

    return table;
}

S32 Shaggy8_HBCallback(_zEnt* plyr, _xCollis* coll)
{
    _zEnt* ent = (_zEnt*)coll->optr;

    if (ent->baseType == 2 && ent->subType == 0x24 &&
        strncmp(ent->model->Anim->Single->State->Name, "Idle", 4) == 0)
    {
        ShaggyGuts* guts = zEntShaggy_FindGuts(ent);

        if (!guts)
        {
            return 0;
        }
        guts->barrelButted = 1;
        plyr->frame->mat.pos.x = ((xMat4x3*)plyr->model->Mat)->pos.x;
        plyr->frame->mat.pos.y = ((xMat4x3*)plyr->model->Mat)->pos.y;
        plyr->frame->mat.pos.z = ((xMat4x3*)plyr->model->Mat)->pos.z;
        globals.player.DecelRunSpeed *= 0.25f;
        globals.player.HeadbuttHit = 1;
        zRumbleStart(0, SDR_Headbutt);
        return 1;
    }
    return 0;
}

S32 Shaggy8_BBCallback(_zEnt* plyr, _xCollis* coll)
{
    _zEnt* ent = (_zEnt*)coll->optr;

    if (ent->baseType == 2 && ent->subType == 0x24 &&
        (strcmp(ent->model->Anim->Single->State->Name, "Stand_idle") == 0 ||
         strcmp(ent->model->Anim->Single->State->Name, "Stand_walk") == 0 ||
         strcmp(ent->model->Anim->Single->State->Name, "Stand_run") == 0))
    {
        ShaggyGuts* guts = zEntShaggy_FindGuts(ent);

        if (!guts)
        {
            return 0;
        }
        guts->barrelBounced = 1;
        return 1;
    }
    return 0;
}

static void Shaggy8_Update(_zEnt* ent, _xScene* sc, F32 dt)
{
    if (ent == globals.player.Shaggy8Call)
    {
        globals.player.Shaggy8CallTimer -= dt;
    }

    if (xEntIsVisible(ent))
    {
        xAnimState* astate = ent->model->Anim->Single->State;
        xAnimState* bstate =
            ent->model->Anim->Single->Blend ? ent->model->Anim->Single->Blend->State : NULL;
        ShaggyGuts* guts = zEntShaggy_FindGuts(ent);

        if (!guts->swap)
        {
            guts->idleTimer += dt;
        }
        if (guts->swap)
        {
            guts->swapTimer += dt;
        }

        if (!astate->UserFlags || (bstate && !bstate->UserFlags))
        {
            xEntBeginUpdate(ent, sc, dt);
            xEntEndUpdate(ent, sc, dt);
        }
        else
        {
            sVilUpdateFunc(ent, sc, dt);

            for (S32 cnum = 6; cnum < 18; cnum++)
            {
                if (!(ent->frame->mode & 0x20) || xabs(ent->frame->drot.angle) < 0.013089970f)
                {
                    if ((ent->collis->colls[cnum].flags & 0x1) &&
                        !(ent->collis->colls[cnum].flags & 0xC) &&
                        !ent->collis->colls[cnum].optr &&
                        ent->collis->colls[cnum].depen.x * ent->collis->colls[cnum].depen.x +
                                ent->collis->colls[cnum].depen.z *
                                    ent->collis->colls[cnum].depen.z >
                            0.001f)
                    {
                        guts->barrelFellOver = 1;
                        zEntEvent(ent, 0x29);
                        return;
                    }
                }
            }
        }
    }
}

static S32 Shaggy8_EventCB(xBase* from, xBase* to, U32 toEvent, F32* toParam, xBase* toParamWidget)
{
    S32 retval = 0;
    ShaggyGuts* guts = zEntShaggy_FindGuts((_zEnt*)to);

    switch (toEvent)
    {
    case eEventVisible:
    case eEventInvisible:
    case eEventCollision_Visible_On:
    case eEventCollision_Visible_Off:
        ShaggyVil_HandleVisible((_zEnt*)to, toEvent);
        break;
    case eEventShaggySwap:
        if (toParamWidget && toParamWidget->baseType == 2)
        {
            ShaggySwap((_zEnt*)to, (_zEnt*)toParamWidget);
        }
        break;
    case 0xD2:
        guts->prefIdle = toParam[0];
        guts->prefMove = toParam[1];
        break;
    case 0xD3:
        guts->actionIndex = 1;
        if (toParamWidget && toParamWidget->baseType == 2 &&
            zEntShaggy_FindGuts((_zEnt*)toParamWidget))
        {
            guts->swap = (_zEnt*)toParamWidget;
            guts->swapTimer = 0.0f;
            ((xMat4x3*)guts->swap->model->Mat)->pos.y = 1000000.0f;
            zEntEvent(to, toParamWidget, eEventVisible);
        }
        break;
    case 0xDE:
        globals.player.Shaggy8Call = (_zEnt*)to;
        globals.player.Shaggy8CallTimer = toParam[0];
        break;
    case 0xDF:
        globals.player.Shaggy8Call = NULL;
        break;
    case 0xE0:
        if (toParamWidget)
        {
            S32 magnum;

            for (magnum = 0; magnum < sMagnetCount; magnum++)
            {
                if (sMagnetList[magnum].shaggy == (_zEnt*)to &&
                    sMagnetList[magnum].target == toParamWidget)
                {
                    sMagnetList[magnum].radius = toParam[0];
                    break;
                }
            }
            if (magnum == sMagnetCount)
            {
                sMagnetList[sMagnetCount].shaggy = (_zEnt*)to;
                sMagnetList[sMagnetCount].target = toParamWidget;
                sMagnetList[sMagnetCount].radius = toParam[0];
                sMagnetCount++;
            }
        }
        break;
    case 0xE1:
        if (toParamWidget)
        {
            for (S32 magnum = 0; magnum < sMagnetCount; magnum++)
            {
                if (sMagnetList[magnum].shaggy == (_zEnt*)to &&
                    sMagnetList[magnum].target == toParamWidget)
                {
                    sMagnetList[magnum] = sMagnetList[sMagnetCount - 1];
                    sMagnetCount--;
                    break;
                }
            }
        }
        break;
    default:
        retval = sVilEventFunc(from, to, toEvent, toParam, toParamWidget);
        break;
    }

    return retval;
}

S32 Shaggy8_CallToPlayer()
{
    _zEnt* ent;
    _zEnt* plyr;
    F32 rayx;
    F32 rayz;
    F32 mag;
    F32 pitch;
    S32 magnum;
    S32 bestmag;
    xVec3* magvec;
    F32 magrad;
    F32 bestmagdist;
    F32 mx;
    F32 mz;
    F32 normdist;
    F32 tandist;
    F32 oldX;
    F32 oldZ;

    if (globals.player.Shaggy8CallTimer > 0.0f || !(ent = globals.player.Shaggy8Call) ||
        !(ent->baseFlags & 0x1))
    {
        return 0;
    }
    if (!(ent->model->Anim->Single->State->UserFlags & 0x10000))
    {
        return 0;
    }

    plyr = &globals.player.ent;
    rayx = ((xMat4x3*)plyr->model->Mat)->pos.x - ((xMat4x3*)ent->model->Mat)->pos.x;
    rayz = ((xMat4x3*)plyr->model->Mat)->pos.z - ((xMat4x3*)ent->model->Mat)->pos.z;
    mag = sqrtf(rayx * rayx + rayz * rayz);
    if (mag < 0.1f)
    {
        return 0;
    }
    rayx *= 1.0f / mag;
    rayz *= 1.0f / mag;

    sSndCallPitch = zPitchChange(sSndCallPitch, &pitch, 30, 0.15f);
    U32 sndID = (irand() >> 16) & 1 ? 0x176632C : 0x176632D;
    if (xEntIsVisible(globals.player.Shaggy8Call))
    {
        xSndPlay(sndID, 0.5f, 0.5f, pitch, 0x80, 2, 0);
    }

    bestmag = -1;
    bestmagdist = 1000000.0f;
    for (magnum = 0; magnum < sMagnetCount; magnum++)
    {
        if (sMagnetList[magnum].shaggy == ent)
        {
            switch (sMagnetList[magnum].target->baseType)
            {
            case 0xD:
                magvec = ((_xMovePoint*)sMagnetList[magnum].target)->pos;
                break;
            case 0x2:
            case 0x3:
            case 0x4:
            case 0x6:
            case 0x8:
            case 0xB:
            case 0xC:
            case 0x12:
            case 0x17:
            case 0x18:
            case 0x1B:
                magvec = &((xMat4x3*)((_xEnt*)sMagnetList[magnum].target)->model->Mat)->pos;
                break;
            }

            mx = magvec->x - ((xMat4x3*)ent->model->Mat)->pos.x;
            mz = magvec->z - ((xMat4x3*)ent->model->Mat)->pos.z;
            magrad = sMagnetList[magnum].radius;
            if (!(mx * mx + mz * mz < ent->bound.sph.r * ent->bound.sph.r))
            {
                normdist = mx * rayx + mz * rayz;
                tandist = mx * rayz - mz * rayx;
                if (normdist >= ent->bound.sph.r && normdist < bestmagdist &&
                    xabs(tandist) < magrad)
                {
                    bestmag = magnum;
                    bestmagdist = normdist;
                }
            }
        }
    }

    globals.player.Shaggy8CallTimer = 1.3333f;
    F32 dummyParam[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
    if (bestmag != -1)
    {
        zEntEvent(NULL, 0, ent, 0xCD, dummyParam, sMagnetList[bestmag].target, 0);
    }
    else
    {
        oldX = ((xMat4x3*)plyr->model->Mat)->pos.x;
        oldZ = ((xMat4x3*)plyr->model->Mat)->pos.z;
        ((xMat4x3*)plyr->model->Mat)->pos.x += 20.0f * rayx;
        ((xMat4x3*)plyr->model->Mat)->pos.z += 20.0f * rayz;
        zEntEvent(NULL, 0, ent, 0xCD, dummyParam, plyr, 0);
        ((xMat4x3*)plyr->model->Mat)->pos.x = oldX;
        ((xMat4x3*)plyr->model->Mat)->pos.z = oldZ;
    }

    return 1;
}

void zEntShaggy_UpdateDep()
{
    for (S32 i = 0; i < (S32)gNumShaggy; i++)
    {
        if (gShaggy[i].witchFollow)
        {
            *(xMat4x3*)gShaggy[i].ent->model->Mat = *(xMat4x3*)gShaggy[i].witchFollow->model->Mat;
        }
    }
}

void zEntShaggy_Init(st_zEntVillain* ent)
{
    for (U32 i = 0; i < 16; i++)
    {
        gShaggy[i].hangme = NULL;
        gShaggy[i].hangspin = 0.0f;
    }

    if (!sVilUpdateFunc)
    {
        sVilUpdateFunc = ent->update;
    }
    if (!sVilMoveFunc)
    {
        sVilMoveFunc = ent->move;
    }
    if (!sVilEventFunc)
    {
        sVilEventFunc = ent->eventFunc;
    }

    switch (ent->subType)
    {
    case 0x1C:
    case 0x1D:
        ent->update = (void (*)(_xEnt*, _xScene*, F32))Shaggy1_Update;
        ent->move = Shaggy1_Move;
        if (ent->subType == 0x1C)
        {
            ent->eventFunc = Shaggy0_EventCB;
        }
        if (ent->subType == 0x1D)
        {
            ent->eventFunc = Shaggy1_EventCB;
        }
        break;
    case 0x21:
        ent->update = (void (*)(_xEnt*, _xScene*, F32))Shaggy5_Update;
        ent->move = Shaggy5_Move;
        ent->eventFunc = Shaggy5_EventCB;
        break;
    case 0x24:
        ent->update = (void (*)(_xEnt*, _xScene*, F32))Shaggy8_Update;
        ent->eventFunc = Shaggy8_EventCB;
        break;
    }

    ent->collis->chk = 0x26;
    ent->collis->pen = 0x66;
    ent->chkby = 0x1C;
    ent->penby = 0x1C;
}

static inline xMat4x3* zEntShaggy_BoneMat(xModelInstance* minst, U32 bone)
{
    return &((xMat4x3*)minst->Mat)[bone];
}

static inline S32 zEntShaggy_HasAnimTable(_zEnt* ent, const char* name)
{
    xModelInstance* minst;

    for (minst = ent->model; minst; minst = minst->Next)
    {
        if (minst->Anim && !xStricmp(minst->Anim->Table->Name, name))
        {
            return 1;
        }
    }
    return 0;
}

static inline void zEntShaggy_SyncVisible()
{
    for (S32 i = 0; i < (S32)gNumShaggy; i++)
    {
        _zEnt* ent = gShaggy[i].ent;

        if (ent->baseType == 2)
        {
            ShaggyVil_HandleVisible(ent, (ent->asset->flags & 0x1) ? eEventVisible : eEventInvisible);
        }
    }
}

void zEntShaggy_SceneStart(zScene* sc)
{
    U32 i;
    U32 j;
    U32 hangHashID;
    xModelInstance* inst;
    xModelInstance* inst2;
    RpAtomic* data1;
    xModelInstance* minst;
    _zPlatform* plat;
    xModelInstance* tmpinst;
    U32 hangcount;

    sSndCallPitch = 359.0f * iurand();
    gTractorEmitter = zParEmitterFind("THORN CLIPPINGS EMITTER");
    gBurningManEmitter = zParEmitterFind("SHAGGY BURNNING EMITTER");
    gNumShaggy = 0;
    memset(gShaggy, 0, sizeof(gShaggy));

    hangcount = 0;
    hangHashID = xStrHash("SHAGGY_HANG0");
    memset(globals.player.Shaggy1.Hangable, 0, sizeof(globals.player.Shaggy1.Hangable));

    for (i = 0; i < sc->num_ents; i++)
    {
        if (zEntIsShaggy(sc->ents[i]))
        {
            switch (sc->ents[i]->subType)
            {
            case 0x1C:
            case 0x1D:
                Shaggy1_BonusAnims(sc->ents[i]);
                break;
            case 0x1E:
            case 0x1F:
                break;
            case 0x24:
                inst = sc->ents[i]->model;
                inst2 = inst->Next;
                if (inst && inst2 && iModelVertCount(inst2->Data) > iModelVertCount(inst->Data))
                {
                    data1 = inst->Data;
                    inst->Data = inst2->Data;
                    inst2->Data = data1;
                }
                break;
            }

            gShaggy[gNumShaggy].ent = sc->ents[i];
            gShaggy[gNumShaggy].shaggyType = sc->ents[i]->subType - 0x1C;
            gShaggy[gNumShaggy].hangme = NULL;
            gShaggy[gNumShaggy].oldAt = ((xMat4x3*)sc->ents[i]->model->Mat)->at;
            gShaggy[gNumShaggy].oldAt2 = gShaggy[gNumShaggy].oldAt;
            gShaggy[gNumShaggy].oldY = 1e10f;
            gShaggy[gNumShaggy].talkTimer = 0.0f;
            gShaggy[gNumShaggy].talkSwitch = -1.0f;
            gShaggy[gNumShaggy].talkRand = irand() >> 16;
            gShaggy[gNumShaggy++].geronimoChase =
                (globals.sceneCur->sceneID == zSceneIDFromName("o006") &&
                 gShaggy[gNumShaggy].ent->id == xStrHash("SHAGGY5")) ?
                    (_zEnt*)zSceneFindObject(xStrHash("GERONIMO")) :
                    NULL;
        }

        if (sc->ents[i]->baseType == 0x17 && sc->ents[i]->id >= hangHashID &&
            sc->ents[i]->id <= hangHashID + 9)
        {
            globals.player.Shaggy1.Hangable[hangcount++] = sc->ents[i];
        }

        if (sc->ents[i]->baseType == 6 && sc->ents[i]->model->Anim &&
            strcmp(sc->ents[i]->model->Anim->Table->Name, "Shaggy3") == 0)
        {
            gShaggy[gNumShaggy].ent = sc->ents[i];
            gShaggy[gNumShaggy].shaggyType = 3;
            gShaggy[gNumShaggy].hangme = NULL;
            gShaggy[gNumShaggy].oldAt = ((xMat4x3*)sc->ents[i]->model->Mat)->at;
            gShaggy[gNumShaggy].oldAt2 = gShaggy[gNumShaggy].oldAt;
            gShaggy[gNumShaggy].oldY = 1e10f;
            gShaggy[gNumShaggy].stopMode = 1;
            gShaggy[gNumShaggy].stopParam = 5.0f;
            gShaggy[gNumShaggy].stopEvent = 0;
            gShaggy[gNumShaggy].runEvent = 0;
            gNumShaggy++;
            sc->ents[i]->update = (void (*)(_xEnt*, _xScene*, F32))Shaggy3_Update;
            sc->ents[i]->eventFunc = Shaggy3_EventCB;
        }

        if (sc->ents[i]->baseType == 6 && zEntShaggy_HasAnimTable(sc->ents[i], "ShaggyChimney"))
        {
            minst = sc->ents[i]->model;
            minst->Flags = 0;
            tmpinst = minst->Next;
            tmpinst->Flags = (tmpinst->Flags & ~0x8) | 0x20;
            if (tmpinst->Next)
            {
                xModelInstance* newminst = xModelInstanceAlloc(tmpinst->Next->Data, tmpinst->Next->Object,
                                               (tmpinst->Next->Flags & ~0x8) | 0x60, 0, NULL);
                newminst->Anim = tmpinst->Next->Anim;
                *(xMat4x3*)newminst->Mat = *(xMat4x3*)tmpinst->Next->Mat;
                newminst->Anim->ModelInst = newminst;
                tmpinst->Next->Anim = NULL;
                xModelInstanceFree(tmpinst->Next);
                xModelInstanceAttach(newminst, tmpinst);
                zEntShaggy_BoneMat(newminst, newminst->BoneCount + 1)->pos.y = -0.6f;
                newminst->Flags &= ~0x7;
                iModelTagIndex(&ChimneyTag, tmpinst->Data, ChimneyTagIndex);
                iModelTagIndex(&ShaggyTag, tmpinst->Next->Data, ShaggyTagIndex);
            }

            gShaggy[gNumShaggy].ent = sc->ents[i];
            gShaggy[gNumShaggy].shaggyType = 2;
            gShaggy[gNumShaggy].chimneyState = 0;
            gShaggy[gNumShaggy].chimneyTimer = 0.0f;
            sc->ents[i]->update = (void (*)(_xEnt*, _xScene*, F32))Shaggy2_Update;
            sc->ents[i]->render = Shaggy2_Render;
            sc->ents[i]->eventFunc = Shaggy2_EventCB;
            sc->ents[i]->chkby |= 0x10;
            sc->ents[i]->penby |= 0x10;

            plat = (_zPlatform*)sc->ents[i];
            gShaggy[gNumShaggy].chimneyHeight = plat->motion.asset->er.ext_dpos.y;
            gShaggy[gNumShaggy].timeLaunch = plat->motion.asset->er.ret_wait_tm;
            gShaggy[gNumShaggy].timeTop = plat->motion.asset->er.ext_wait_tm;
            gShaggy[gNumShaggy].chimneyGrav = plat->motion.asset->er.ext_tm;
            gShaggy[gNumShaggy].timeEvent = 0.0f;
            gShaggy[gNumShaggy].oldY = 1e10f;
            if (!gShaggy[gNumShaggy].chimneyHeight)
            {
                gShaggy[gNumShaggy].chimneyHeight = 4.0f;
            }
            if (!gShaggy[gNumShaggy].timeLaunch)
            {
                gShaggy[gNumShaggy].timeLaunch = 3.0f;
            }
            gShaggy[gNumShaggy].chimneyPlat = NULL;

            for (j = 0; j < sc->num_ents; j++)
            {
                if (i != j && sc->ents[j]->baseType == 6)
                {
                    if (xVec3Dist(&sc->ents[j]->asset->pos, &sc->ents[i]->asset->pos) < 0.1f)
                    {
                        gShaggy[gNumShaggy].chimneyPlat = (_zPlatform*)sc->ents[j];
                        sc->ents[j]->flags &= ~0x1;
                        sc->ents[j]->model->Flags = 0;
                        sc->ents[j]->eventFunc = Shaggy2Plat_EventCB;
                        break;
                    }
                }
            }
            gNumShaggy++;
        }

        if (sc->ents[i]->baseType == 0xB && zEntShaggy_HasAnimTable(sc->ents[i], "ShaggyChimney"))
        {
            for (minst = sc->ents[i]->model->Next; minst; minst = minst->Next)
            {
                minst->Flags = 0;
            }
        }
    }

    zEntShaggy_SyncVisible();
}

void zEntShaggy_SceneReset()
{
    U32 i;

    for (i = 0; i < gNumShaggy; i++)
    {
        gShaggy[i].hangme = NULL;
        gShaggy[i].hangspin = 0.0f;
        gShaggy[i].chimneyState = 0;
        gShaggy[i].oldAt = ((xMat4x3*)gShaggy[i].ent->model->Mat)->at;
        gShaggy[i].oldAt2 = gShaggy[i].oldAt;
        gShaggy[i].witchFollow = NULL;
        gShaggy[i].witchDrop = NULL;
        gShaggy[i].barrelButted = 0;
        gShaggy[i].barrelBounced = 0;
        gShaggy[i].swap = NULL;
        gShaggy[i].actionIndex = 0;
        gShaggy[i].eatTimer = 0.0f;
        gShaggy[i].facerot = 0.0f;
        gShaggy[i].facept = NULL;
        gShaggy[i].talkTimer = 0.0f;
        gShaggy[i].talkSwitch = -1.0f;
        gShaggy[i].stopMode = 1;
        gShaggy[i].stopParam = 5.0f;
        gShaggy[i].stopEvent = 0;
        gShaggy[i].runEvent = 0;
        gShaggy[i].geronimoChase =
            (globals.sceneCur->sceneID == zSceneIDFromName("o006") &&
             gShaggy[i].ent->id == xStrHash("SHAGGY5")) ?
                (_zEnt*)zSceneFindObject(xStrHash("GERONIMO")) :
                NULL;

        if (gShaggy[i].ent->baseType == 2)
        {
            gShaggy[i].ent->pflags |= 0x4;
        }
        if (gShaggy[i].ent->baseType == 6 &&
            zEntShaggy_HasAnimTable(gShaggy[i].ent, "ShaggyChimney"))
        {
            xModelInstance* minst = gShaggy[i].ent->model->Next;

            if (minst)
            {
                minst = minst->Next;
                if (minst)
                {
                    zEntShaggy_BoneMat(minst, minst->BoneCount + 1)->pos.y = -0.6f;
                    minst->Flags &= ~0x7;
                }
            }
        }
        if (gShaggy[i].ent->baseType == 2 && gShaggy[i].ent->subType == 0x24)
        {
            xModelInstance* minst = gShaggy[i].ent->model->Next;

            if (minst)
            {
                minst->Flags |= 0x3;
            }
        }
        if (gShaggy[i].ent->baseType == 6 && zEntShaggy_HasAnimTable(gShaggy[i].ent, "Shaggy3"))
        {
            xAnimPlaySetState(gShaggy[i].ent->model->Anim->Single,
                              gShaggy[i].ent->model->Anim->Table->StateList, 0.0f);
        }
    }

    sMagnetCount = 0;
    zEntShaggy_SyncVisible();
}
