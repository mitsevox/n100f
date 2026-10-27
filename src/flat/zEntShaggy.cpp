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
#include <string.h>
#include <math.h>

ShaggyGuts gShaggy[16];
U32 gNumShaggy;
F32 gACCEL_START;
F32 gACCEL_END;
F32 THROW_MINHEIGHT = 4.25f;
F32 THROW_DEFHEIGHT = 6.0f;
static F32 gShaggy3TurnAngle;
static S32 (*sVilEventFunc)(xBase*, xBase*, U32, F32*, xBase*);
static void (*sPlatMoveFunc)(_xEnt*, _xScene*, F32, _xEntFrame*);
F32 sHangVirtualPeak = 0.75f + THROW_DEFHEIGHT;

void ShaggySwap(_zEnt* src, _zEnt* dest);

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

static inline F32 Shaggy1_CalcThrowAccel(F32 peak, F32* timepeak)
{
    F32 sol[2];
    F32 Vthrow;
    F32 Ta;
    U32 solcnt;

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
    return (U32)(!globals.player.Health && (frame->mode & 0x2)) &&
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
    return (F32)fabs(gShaggy3TurnAngle) <= 0.0015f && !anim->Blend->State;
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
    sPlatMoveFunc(ent, sc, dt, frame);
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
