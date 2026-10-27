#include "zEntShaggy.h"
#include "zGlobals.h"
#include "xEvent.h"
#include "xModel.h"
#include "xString.h"

ShaggyGuts gShaggy[16];
U32 gNumShaggy;
static S32 (*sVilEventFunc)(xBase*, xBase*, U32, F32*, xBase*);

void ShaggySwap(_zEnt* src, _zEnt* dest);

U32 Shaggy1_Grab(xAnimTransition* tran, xAnimSingle* anim, _zEnt* ent);
U32 Shaggy1_GrabCB(xAnimTransition* tran, xAnimSingle* anim, _zEnt* ent);
U32 Shaggy1_Walk(xAnimTransition* tran, xAnimSingle* anim, _zEnt* ent);
U32 Shaggy1_Stop(xAnimTransition* tran, xAnimSingle* anim, _zEnt* ent);
U32 Shaggy1_ThrowCB(xAnimTransition* tran, xAnimSingle* anim, _zEnt* ent);
U32 Shaggy1_LandCB(xAnimTransition* tran, xAnimSingle* anim, _zEnt* ent);
U32 Shaggy1_EventCB(xAnimTransition* tran, xAnimSingle* anim, _zEnt* ent);
U32 Shaggy1_IdleResetCB(xAnimTransition* tran, xAnimSingle* anim, _zEnt* ent);
U32 Shaggy1_IdlePick(xAnimTransition* tran, xAnimSingle* anim, _zEnt* ent);
U32 Shaggy1_IdleLeave(xAnimTransition* tran, xAnimSingle* anim, _zEnt* ent);
U32 Shaggy1_Scare(xAnimTransition* tran, xAnimSingle* anim, _zEnt* ent);
U32 Shaggy1_ScareReact(xAnimTransition* tran, xAnimSingle* anim, _zEnt* ent);
U32 Shaggy1_DeathReact(xAnimTransition* tran, xAnimSingle* anim, _zEnt* ent);
U32 Shaggy1_FallDeath(xAnimTransition* tran, xAnimSingle* anim, _zEnt* ent);
U32 Shaggy1_FallDeathCB(xAnimTransition* tran, xAnimSingle* anim, _zEnt* ent);
U32 Shaggy1_TalkFlag(xAnimTransition* tran, xAnimSingle* anim, _zEnt* ent);
U32 Shaggy1_TalkChooseCB(xAnimTransition* tran, xAnimSingle* anim, _zEnt* ent);
U32 Shaggy1_Notalk(xAnimTransition* tran, xAnimSingle* anim, _zEnt* ent);
U32 Shaggy1_NotalkCB(xAnimTransition* tran, xAnimSingle* anim, _zEnt* ent);

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
