#include "zEntVillain.h"

#include "iMath.h"
#include "xAnim.h"
#include "xEvent.h"
#include "xMath.h"
#include "xMemMgr.h"
#include "xScene.h"
#include "zEntShaggy.h"
#include "zGlobals.h"
#include "zScene.h"

#include <string.h>

enum en_RBS_STAGE
{
    RBS_STAGE_PRELIGHT = 4000,
    RBS_STAGE_OUTBOUND,
    RBS_STAGE_INBOUND
};

// Redbeard's thrown sword.
struct st_REDBEARD_SWORD : _xEnt
{
    S32 flg_stat;
    en_RBS_STAGE stage;
    U32 aid_mdlFile;
    RpAtomic* mdlFile;
    st_zEntVillain* rbvil;
    xModelInstance* mdl_animsword;
    _xEntFrame raw_frame;
    _xEntCollis raw_collis;
    _xEnt* ent_colchk[5];
    F32 tym_age;
    F32 dst_far;
    F32 spd_fly;
    _xVec3 dir_end;
    _xVec3 pos_end;
};


// Settings for one villain subtype.
struct st_ZVIL_NPCTYPE_LOOKUP
{
    S32 enabled;
    F32 decay;
    F32 ds2_atpos;
    F32 strength;
    F32 inv_mass;
    S32 dmg_give;
    F32 dst_noisy;
    F32 rad_bound;
    _xVec3 vec_bndoff;
    F32 rad_bound2;
    _xVec3 vec_bnd2off;
    S32 ability;
    S32 abl_atak;
    S32 abl_dtct;
    S32 abl_move;
    S32 abl_spyk;
    S32 abl_vuln;
    en_NPC_PERSONALITIES typ_person;
    U8 flg_physics;
    U8 pad[3];
    U8 flg_chk;
    U8 flg_pen;
    U8 flg_chkby;
    U8 flg_penby;
};


static xAnimTable* VANM_mktable_STD();
static xAnimTable* VANM_mktable_GANG();
static xAnimTable* VANM_mktable_SIMP();
static xAnimTable* VANM_mktable_SHARK();
static xAnimTable* VANM_mktable_SHAM();
static xAnimTable* VANM_mktable_MM();
static xAnimTable* VANM_mktable_RB();
static xAnimTable* VANM_mktable_GG();
static xAnimTable* VANM_mktable_BK();
static U32 VANM_dotran(xAnimTransition* tran, xAnimSingle* single, st_zEntVillain* ent);
static U32 VANM_didtran(xAnimTransition* tran, xAnimSingle* single, st_zEntVillain* ent);
static U32 VANM_dodflt(xAnimTransition* tran, xAnimSingle* single, st_zEntVillain* ent);
static U32 VANM_diddflt(xAnimTransition* tran, xAnimSingle* single, st_zEntVillain* ent);
static en_NPC_ANIMSTATES VANM_set(st_zEntVillain* ent, en_NPC_ANIMSTATES* best, S32 cnt,
                                  S32 sync);
static en_NPC_ANIMSTATES VANM_randset(st_zEntVillain* ent, F32* wts,
                                      en_NPC_ANIMSTATES* choice_list, S32 cnt);
static xAnimTransition* VANM_findtran(st_zEntVillain* ent, xAnimTable* table, xAnimState* from,
                                      xAnimState* to, S32 autoslamit, xAnimTransition** newtran);
static xAnimSingle* VANM_entSingle(const _zEnt* ent);
static S32 VSUP_accum_collide(st_zEntVillain* vil, _xVec3* vec_accum);
static void VSUP_cheap_reflect(_xVec3* out, _xVec3* alter);

static S32 VSUP_choose(S32* list, F32* wts, S32 cnt);
static void VSUP_wtadjust(F32* wts, S32 cnt, F32 arbref);

static void zVIL_tag_verts(st_zEntVillain* ent);
static void zVIL_precalc(st_zEntVillain* vil, S32 flg_needinfo);

static S32 VILN_std_flag(st_zEntVillain* vil, en_ZVIL_FLAG_GROUPS flaggroup,
                         en_BEHAVIOUR_GOAL goal);
static S32 VILN_std_query(st_zEntVillain* vil, en_ZVIL_QUERY query);
static S32 VILN_std_sysevent(st_zEntVillain* vil, xBase* from, U32 toEvent, F32* toParam,
                             xBase* toParamWidget, st_STIMULUS_CONTEXT_DATA* resp);
static S32 VILN_std_notice(st_zEntVillain* vil, en_ZVIL_NOTICE note,
                           st_STIMULUS_CONTEXT_DATA* resp);
static S32 VILN_std_aware(st_zEntVillain* vil, _xScene* scn, F32 dt,
                          st_STIMULUS_CONTEXT_DATA* resp);
static S32 VILN_std_stimulus(st_zEntVillain* vil, st_STIMULUS_CONTEXT_DATA* stimctxt,
                             st_STIMULUS_CONTEXT_DATA* resp);
static S32 VILN_std_trangoal(st_zEntVillain* vil, st_STIMULUS_CONTEXT_DATA* stimctxt, F32 dt);
static S32 VILN_std_calcmove(st_zEntVillain* vil, _xScene* scn, F32 dt, const _xEntFrame* frame);
static S32 VILN_std_respond(st_zEntVillain* vil, st_STIMULUS_CONTEXT_DATA* resp);
static S32 VILN_std_pickanim(st_zEntVillain* vil, en_BEHAVIOUR_GOAL goal, en_ZVIL_GOAL_SPOT gspot);

// Per-subtype villain settings, indexed by the asset subtype (en_zEntVillainSubTypeID). Only the
// members up to the bound offset have initial values.
static st_ZVIL_NPCTYPE_LOOKUP g_npcTable[eEntVillainType_NoMore + 1] = {
    // eEntVillainTypeBat
    { 1, 1.0f, 0.3f, 1.0f, 2.0f, 1, 8.5f, 0.7f, { 0.0f, 0.7f, 0.0f } },
    // eEntVillainTypeBlackKnight
    { 1, -1.0f, 0.3f, -1.0f, 1.0f, 1, 90.5f, 0.7f, { 0.0f, 0.7f, 0.0f } },
    // eEntVillainTypeBuckMasters
    { 1, 2.0f, 1.0f, 2.0f, 0.7f, 1, 15.5f, 0.7f, { 0.0f, 0.7f, 0.0f } },
    // eEntVillainTypeCreeper
    { 1, 2.0f, 1.0f, 3.0f, 0.4f, 1, 15.5f, 0.7f, { 0.0f, 0.7f, 0.0f } },
    // eEntVillainTypeSeaCreature
    { 1, 2.0f, 1.0f, 2.0f, 0.2f, 1, 15.5f, 0.7f, { 0.0f, 0.7f, 0.0f } },
    // eEntVillainTypeGargoyle
    { 1, 2.0f, 1.0f, -1.0f, 0.5f, 1, 15.5f, 0.7f, { 0.0f, 0.7f, 0.0f } },
    // eEntVillainTypeGhost
    { 1, 2.0f, 0.5f, 0.5f, 2.0f, 1, 15.5f, 0.7f, { 0.0f, 0.7f, 0.0f } },
    // eEntVillainTypeGhostDiver
    { 1, 2.0f, 1.0f, 1.0f, 0.5f, 1, 15.5f, 0.7f, { 0.0f, 0.7f, 0.0f } },
    // eEntVillainTypeGreenGhost
    { 1, 2.0f, 0.5f, -1.0f, 1.0f, 1, 90.5f, 0.7f, { 0.0f, 0.7f, 0.0f } },
    // eEntVillainTypeHeadlessSpecter
    { 1, 2.0f, 1.0f, 0.3f, 1.0f, 1, 12.5f, 0.7f, { 0.0f, 0.7f, 0.0f } },
    // eEntVillainTypePhantom
    { 1, -1.0f, 0.5f, 1.0f, 1.0f, 1, 90.5f, 0.7f, { 0.0f, 0.7f, 0.0f } },
    // eEntVillainTypeRedbeard
    { 1, -1.0f, 0.5f, 1.0f, 1.0f, 1, 90.5f, 0.7f, { 0.0f, 0.7f, 0.0f } },
    // eEntVillainTypeRobot
    { 1, 2.0f, 1.0f, 1.0f, 0.3f, 1, 15.5f, 0.7f, { 0.0f, 0.7f, 0.0f } },
    // eEntVillainTypeScarecrow
    { 1, 2.0f, 1.0f, -0.5f, 0.7f, 1, 15.5f, 0.7f, { 0.0f, 0.7f, 0.0f } },
    // eEntVillainTypeShark
    { 1, -1.0f, 1.0f, 5.0f, 1.0f, 1, 20.5f, 0.7f, { 0.0f, 0.7f, 0.0f } },
    // eEntVillainTypeSpaceKook
    { 1, 2.0f, 1.0f, -1.0f, 0.6f, 1, 15.5f, 0.7f, { 0.0f, 0.7f, 0.0f } },
    // eEntVillainTypeTarMonster
    { 1, 2.0f, 1.0f, -1.0f, 0.3f, 1, 15.5f, 0.7f, { 0.0f, 0.7f, 0.0f } },
    // eEntVillainTypeWitch
    { 1, -1.0f, 0.01f, -1.0f, 0.9f, 1, 15.5f, 0.7f, { 0.0f, 0.7f, 0.0f } },
    // eEntVillainTypeWitchDoctor
    { 1, 2.0f, 1.0f, -1.0f, 0.7f, 1, 15.5f, 0.7f, { 0.0f, 0.7f, 0.0f } },
    // eEntVillainTypeWolfman
    { 1, 2.0f, 1.0f, 0.5f, 0.7f, 1, 15.5f, 0.7f, { 0.0f, 0.7f, 0.0f } },
    // eEntVillainTypeZombie
    { 1, 2.0f, 1.0f, 1.0f, 0.6f, 1, 15.5f, 0.7f, { 0.0f, 0.7f, 0.0f } },
    // eEntVillainTypeCrab
    { 1, 1.0f, 0.5f, 0.2f, 1.0f, 1, 10.5f, 0.7f, { 0.0f, 0.7f, 0.0f } },
    // eEntVillainTypeRat
    { 1, 1.0f, 0.5f, 0.2f, 1.0f, 1, 12.5f, 0.7f, { 0.0f, 0.7f, 0.0f } },
    // eEntVillainTypeFlyingFish
    { 1, -1.0f, 0.5f, 0.2f, 1.0f, 1, 12.5f, 0.7f, { 0.0f, 0.7f, 0.0f } },
    // eEntVillainTypeSpider
    { 1, 1.0f, 0.5f, 0.2f, 1.0f, 1, 10.5f, 0.7f, { 0.0f, 0.7f, 0.0f } },
    // eEntVillainTypeHand
    { 1, 1.0f, 0.5f, 0.2f, 1.0f, 1, 10.5f, 0.7f, { 0.0f, 0.7f, 0.0f } },
    // eEntVillainTypeKillerPlant
    { 1, -1.0f, 0.5f, 0.2f, 1.0f, 1, 15.5f, 0.7f, { 0.0f, 0.7f, 0.0f } },
    // eEntVillainTypeBird
    { 1, 1.0f, 0.5f, 0.2f, 1.0f, 1, 12.5f, 0.7f, { 0.0f, 0.7f, 0.0f } },
    // eEntVillainTypeShaggy0
    { 1, -1.0f, 1.0f, -1.0f, -1.0f, 0, 20.5f, 0.7f, { 0.0f, 0.7f, 0.0f } },
    // eEntVillainTypeShaggy1
    { 1, -1.0f, 1.0f, -1.0f, -1.0f, 0, 20.5f, 0.7f, { 0.0f, 0.7f, 0.0f } },
    // eEntVillainTypeShaggy2
    { 1, -1.0f, 1.0f, -1.0f, -1.0f, 0, 20.5f, 0.7f, { 0.0f, 0.7f, 0.0f } },
    // eEntVillainTypeShaggy3
    { 1, -1.0f, 1.0f, -1.0f, -1.0f, 0, 20.5f, 0.7f, { 0.0f, 0.7f, 0.0f } },
    // eEntVillainTypeShaggy4
    { 1, -1.0f, 1.0f, -1.0f, -1.0f, 0, 20.5f, 0.7f, { 0.0f, 0.7f, 0.0f } },
    // eEntVillainTypeShaggy5
    { 1, -1.0f, 0.4f, -1.0f, -1.0f, 0, 20.5f, 0.7f, { 0.0f, 0.7f, 0.0f } },
    // eEntVillainTypeShaggy6
    { 1, -1.0f, 1.0f, -1.0f, -1.0f, 0, 20.5f, 0.7f, { 0.0f, 0.7f, 0.0f } },
    // eEntVillainTypeShaggy7
    { 1, -1.0f, 1.0f, -1.0f, -1.0f, 0, 20.5f, 0.7f, { 0.0f, 0.7f, 0.0f } },
    // eEntVillainTypeShaggy8
    { 1, -1.0f, 0.3f, -1.0f, -1.0f, 0, 20.5f, 0.7f, { 0.0f, 0.7f, 0.0f } },
    // eEntVillainTypeShaggy9
    { 1, -1.0f, 1.0f, -1.0f, -1.0f, 0, 20.5f, 0.7f, { 0.0f, 0.7f, 0.0f } },
    // eEntVillainTypeFred
    { 1, -1.0f, 0.5f, -1.0f, -1.0f, 0, 90.5f, 0.7f, { 0.0f, 0.7f, 0.0f } },
    // eEntVillainTypeDaphne
    { 1, -1.0f, 0.5f, -1.0f, -1.0f, 0, 90.5f, 0.7f, { 0.0f, 0.7f, 0.0f } },
    // eEntVillainTypeVelma
    { 1, -1.0f, 0.5f, -1.0f, -1.0f, 0, 90.5f, 0.7f, { 0.0f, 0.7f, 0.0f } },
    // eEntVillainBossBlackKnight
    { 1, 3.0f, 0.5f, 1.0f, 1.0f, 1, 100.5f, 0.7f, { 0.0f, 0.7f, 0.0f } },
    // eEntVillainBossGreenGhost
    { 1, 3.0f, 0.3f, 1.0f, 1.0f, 1, 100.5f, 0.7f, { 0.0f, 0.7f, 0.0f } },
    // eEntVillainBossRedbeard
    { 1, 3.0f, 0.5f, 1.0f, 1.0f, 1, 100.5f, 0.7f, { 0.0f, 0.7f, 0.0f } },
    // eEntVillainBossMastermind
    { 1, 3.0f, 0.5f, 1.0f, 1.0f, 1, 100.5f, 0.7f, { 0.0f, 0.7f, 0.0f } },
    // eEntVillainCaptainMoody
    { 1, 2.0f, 0.5f, 2.0f, 1.0f, 1, 25.0f, 0.7f, { 0.0f, 0.7f, 0.0f } },
    // eEntVillainCaveMan
    { 1, 2.0f, 0.5f, 3.0f, 0.4f, 1, 15.5f, 0.7f, { 0.0f, 0.7f, 0.0f } },
    // eEntVillainHolly
    { 1, -1.0f, 0.5f, 0.0f, 0.0f, 0, 90.5f, 0.7f, { 0.0f, 0.7f, 0.0f } },
    // eEntVillainGroundsKeeper
    { 1, -1.0f, 0.5f, 0.0f, 0.0f, 0, 90.5f, 0.7f, { 0.0f, 0.7f, 0.0f } },
    // eEntVillainProfessor
    { 1, -1.0f, 0.5f, 0.0f, 0.0f, 0, 90.5f, 0.7f, { 0.0f, 0.7f, 0.0f } },
    // eEntVillainType_NoMore
    { 0 },
};

// Redbeard's swords.
static st_REDBEARD_SWORD g_rbsword[4];


// Hash of each animation state name, filled in at startup; the index is en_NPC_ANIMSTATES.
static U32 g_anamhash[ZVIL_ANIM_NOMORE] = { 0 };

// Animation state names, indexed by en_NPC_ANIMSTATES.
static char* g_animname[ZVIL_ANIM_NOMORE + 1] = {
    "UNKNOWN", "Idle", "Idle_Fidget_1", "Idle_Fidget_2", "Bored", "Bored_Fidget_1",
    "Bored_Fidget_2", "Move", "Fashion_Pose", "Turn", "Chase", "Action_OnceNoMove",
    "Action_LoopAndMove", "Surprize", "Taunt", "Taunt_1", "Taunt_2", "Hear_Start", "Hear_Loop",
    "Look_Start", "Look_Loop", "Investigate_Start", "Investigate", "AttackIdle", "Attack",
    "Attack_1", "Attack_2", "Combo", "Combo_1", "Combo_2", "Lob_Attack", "Lob_Combo",
    "Extend_Attack_Start", "Extend_Attack_Loop", "Extend_Combo", "Hit_By_Gum", "Gummed",
    "Gummed_Resist", "Gummed_BreakFree", "Hit_When_Gummed", "Bounced_When_Gummed", "Hit_By_Soap",
    "Soaped", "Soaped_Resist", "Soaped_BreakFree", "Hit_When_Soaped", "Bounced_When_Soaped",
    "Dazed_Begin", "Dazed", "Dazed_Recovery", "Death", "Death_From_Above", "Death_From_Dazed",
    "Death_Idle", "Jump_Begin", "Jump_Loop", "Jump_Land", "Fall_Loop", "Fall_Splat", "BK_ScaryIdle",
    "BK_Walk", "BK_Turn", "BK_Patrol", "BK_PatrolFlip", "BK_Attack", "BK_Shock", "BK_ShockSkel",
    "BK_Gloat", "BK_Gloat_1", "BK_Gloat_2", "BK_Death", "GG_Chase", "GG_ChaseLeft", "GG_ChaseRight",
    "GG_AttackBuildBegin", "GG_AttackBuildLoop", "GG_Attack", "GG_Gloat", "GG_Gloat_1", "GG_Hit",
    "GG_Hit_1", "GG_Hit_2", "RB_IdleWind", "RB_Walk", "RB_Turn", "RB_Chase", "RB_TauntFeel",
    "RB_TauntThroat", "RB_TauntLaugh", "RB_AttackPunch", "RB_AttackThrow", "RB_AttackThrowWait",
    "RB_AttackCatch", "RB_React", "RB_LookBegin", "RB_LookLoop", "RB_LookEnd", "RB_DeathLoop",
    "MM_Foreplay_Move", "MM_Foreplay_Taunt", "MM_Foreplay_Action", "MM_Foreplay_JumpDown",
    "MM_Battle_Idle", "MM_Battle_Move", "MM_Battle_Turn", "MM_Battle_Backstep", "MM_Battle_Taunt",
    "MM_AngerReact", "MM_Battle_Attack_start", "MM_Battle_Attack_ready_loop", "MM_Battle_Attack",
    "MM_Battle_Shield_start", "MM_Battle_Shield_loop", "MM_Battle_Shield_end",
    "MM_Battle_Stunned_start", "MM_Battle_Stunned_loop", "MM_Battle_Stunned_end",
    "MM_Battle_Knocked_begin", "MM_Battle_Knocked_loop", "MM_Battle_Knocked_recover",
    "MM_Death_start", "MM_Death", "GANG_Idle_1", "GANG_Idle_2", "GANG_Idle_3", "GANG_IdleFidget",
    "GANG_IdleFidget_1", "GANG_IdleFidget_2", "GANG_IdleFidget_3", "GANG_Turn", "GANG_Walk",
    "GANG_Run", "GANG_RunChased", "GANG_Action", "GANG_BossAction", "GANG_Talk", "GANG_Talk_1",
    "GANG_Talk_2", "GANG_Talk_3", "GANG_TalkAndTurn", "SHAM_Move", "SHAM_Taunt", "CRIT_Move",
    "CRIT_BatSleep", "CRIT_SpiderDrop", "CRIT_Smashed", "SIMP_Action", NULL,
};

xAnimTable* zEntVillain_AnimTable_BlackKnight()
{
    return VANM_mktable_BK();
}

xAnimTable* zEntVillain_AnimTable_GreenGhost()
{
    return VANM_mktable_GG();
}

xAnimTable* zEntVillain_AnimTable_RedBeard()
{
    return VANM_mktable_RB();
}

xAnimTable* zEntVillain_AnimTable_MasterMind()
{
    return VANM_mktable_MM();
}

xAnimTable* zEntVillain_AnimTable_BossSham()
{
    return VANM_mktable_SHAM();
}

xAnimTable* zEntVillain_AnimTable_Shark()
{
    return VANM_mktable_SHARK();
}

xAnimTable* zEntVillain_AnimTable_Witch()
{
    xAnimTable* table = VANM_mktable_STD();
    xAnimTableNewState(table, "Shaggy_hold", 0x10, 0x2000004, 1.0f, NULL, NULL, 0.0f, NULL);
    Shaggy5_WitchTransitions(table);
    return table;
}

xAnimTable* zEntVillain_AnimTable_Simple()
{
    return VANM_mktable_SIMP();
}

xAnimTable* zEntVillain_AnimTable_Gang()
{
    return VANM_mktable_GANG();
}

xAnimTable* zEntVillain_AnimTable_Standard()
{
    return VANM_mktable_STD();
}

// Reads one of the villain's flag groups from its subtype's settings; the allow group reads as 0.
static S32 VILN_std_flag(st_zEntVillain* vil, en_ZVIL_FLAG_GROUPS flaggroup, en_BEHAVIOUR_GOAL)
{
    S32 result = 0;

    switch (flaggroup)
    {
    case ZVIL_FGRP_PHYS:
        result = g_npcTable[vil->asset->subtype].flg_physics;
        break;
    case ZVIL_FGRP_CHK:
        result = g_npcTable[vil->asset->subtype].flg_chk;
        break;
    case ZVIL_FGRP_PEN:
        result = g_npcTable[vil->asset->subtype].flg_pen;
        break;
    case ZVIL_FGRP_CHKBY:
        result = g_npcTable[vil->asset->subtype].flg_chkby;
        break;
    case ZVIL_FGRP_PENBY:
        result = g_npcTable[vil->asset->subtype].flg_penby;
        break;
    case ZVIL_FGRP_ABLE:
        result = g_npcTable[vil->asset->subtype].ability;
        break;
    case ZVIL_FGRP_ATAK:
        result = g_npcTable[vil->asset->subtype].abl_atak;
        break;
    case ZVIL_FGRP_DTCT:
        result = g_npcTable[vil->asset->subtype].abl_dtct;
        break;
    case ZVIL_FGRP_MOVE:
        result = g_npcTable[vil->asset->subtype].abl_move;
        break;
    case ZVIL_FGRP_SPYK:
        result = g_npcTable[vil->asset->subtype].abl_spyk;
        break;
    case ZVIL_FGRP_VULN:
        result = g_npcTable[vil->asset->subtype].abl_vuln;
        break;
    case ZVIL_FGRP_ALOW:
        break;
    }

    return result;
}


// Handles the villain's notices: on prepare, sets up its model vertex tags; on precalc, refreshes
// its precalculated data once it is in normal mode or later.
static S32 VILN_std_notice(st_zEntVillain* vil, en_ZVIL_NOTICE note, st_STIMULUS_CONTEXT_DATA*)
{
    switch (note)
    {
    case ZVIL_NOTE_PREPARE:
        zVIL_tag_verts(vil);
        break;
    case ZVIL_NOTE_PRECALC:
        if (vil->mode >= ZVIL_MODE_NORMAL) zVIL_precalc(vil, 0xF);
        break;
    case ZVIL_NOTE_OLDTIME:
        break;
    }

    return 1;
}


// When a soaped villain with ability bit 20 has bumped into something, it bounces its knockback
// velocity off it. Nothing is returned.
static S32 VILN_std_respond(st_zEntVillain* vil, st_STIMULUS_CONTEXT_DATA*)
{
    _xVec3 vec_accum = { 0.0f, 0.0f, 0.0f };
    S32 yep = VSUP_accum_collide(vil, &vec_accum);

    if (yep && vil->goal == ZVIL_GOAL_SOAPED && (vil->ability & 0x100000))
    {
        VSUP_cheap_reflect(&vil->vec_knocked, &vec_accum);
    }
}


// Draws the villain's gum blob, when it has one.
static S32 VILN_std_xtrarend(st_zEntVillain* vil)
{
    if (vil->mdl_gumBlob) xModelRender(vil->mdl_gumBlob);
    return 1;
}

// Render callback: draws the entity with back faces culled, then turns culling off again.
static void VILN_KookRender(_xEnt* ent)
{
    RwRenderStateSet(rwRENDERSTATECULLMODE, (void*)rwCULLMODECULLBACK);
    xEntRender(ent);
    RwRenderStateSet(rwRENDERSTATECULLMODE, (void*)rwCULLMODECULLNONE);
}

// The killer plant can attack only when bit 3 of its allow flags and bit 0 of its player flags
// are set; any other question gets the standard answer.
static S32 VILN_kp_query(st_zEntVillain* vil, en_ZVIL_QUERY query)
{
    st_ZVIL_PRECALC* prec = &vil->precalc;
    S32 result;

    switch (query)
    {
    case ZVIL_Q_CANATTACK:
        if (!(vil->allow & 0x8))
        {
            result = 0;
        }
        else if (!(prec->flg_plyr & 0x1))
        {
            result = 0;
        }
        else
        {
            result = 1;
        }
        break;
    default:
        result = VILN_std_query(vil, query);
        break;
    }

    return result;
}


// Small creatures (bats, crabs, rats, flying fish, birds) read flag groups the standard way.
static S32 VILN_crit_flag(st_zEntVillain* vil, en_ZVIL_FLAG_GROUPS flaggroup,
                          en_BEHAVIOUR_GOAL goal)
{
    return VILN_std_flag(vil, flaggroup, goal);
}


// Small creatures (bats, crabs, rats, flying fish, birds) answer queries the standard way.
static S32 VILN_crit_query(st_zEntVillain* vil, en_ZVIL_QUERY query)
{
    return VILN_std_query(vil, query);
}

// Small creatures (bats, crabs, rats, flying fish, birds) handle game events the standard way.
static S32 VILN_crit_sysevent(st_zEntVillain* vil, xBase* from, U32 toEvent, F32* toParam,
                              xBase* toParamWidget, st_STIMULUS_CONTEXT_DATA* resp)
{
    return VILN_std_sysevent(vil, from, toEvent, toParam, toParamWidget, resp);
}

// Small creatures (bats, crabs, rats, flying fish, birds) react to notices the standard way.
static S32 VILN_crit_notice(st_zEntVillain* vil, en_ZVIL_NOTICE note,
                            st_STIMULUS_CONTEXT_DATA* resp)
{
    return VILN_std_notice(vil, note, resp);
}


// Small creatures (bats, crabs, rats, flying fish, birds) sense the player the standard way.
static S32 VILN_crit_aware(st_zEntVillain* vil, _xScene* scn, F32 dt,
                           st_STIMULUS_CONTEXT_DATA* resp)
{
    return VILN_std_aware(vil, scn, dt, resp);
}

// Small creatures (bats, crabs, rats, flying fish, birds) react to stimuli the standard way.
static S32 VILN_crit_stimulus(st_zEntVillain* vil, st_STIMULUS_CONTEXT_DATA* stimctxt,
                              st_STIMULUS_CONTEXT_DATA* resp)
{
    return VILN_std_stimulus(vil, stimctxt, resp);
}

// Small creatures (bats, crabs, rats, flying fish, birds) change goals the standard way.
static S32 VILN_crit_trangoal(st_zEntVillain* vil, st_STIMULUS_CONTEXT_DATA* stimctxt, F32 dt)
{
    return VILN_std_trangoal(vil, stimctxt, dt);
}

// Small creatures (bats, crabs, rats, flying fish, birds) move the standard way.
static S32 VILN_crit_calcmove(st_zEntVillain* vil, _xScene* scn, F32 dt, const _xEntFrame* frame)
{
    return VILN_std_calcmove(vil, scn, dt, frame);
}

// Small creatures (bats, crabs, rats, flying fish, birds) respond to stimuli the standard way.
static S32 VILN_crit_respond(st_zEntVillain* vil, st_STIMULUS_CONTEXT_DATA* resp)
{
    return VILN_std_respond(vil, resp);
}


// Small creatures (bats, crabs, rats, flying fish, birds) pick animations the standard way.
static S32 VILN_crit_pickanim(st_zEntVillain* vil, en_BEHAVIOUR_GOAL goal, en_ZVIL_GOAL_SPOT gspot)
{
    return VILN_std_pickanim(vil, goal, gspot);
}

// The shark reads flag groups the standard way.
static S32 VILN_shark_flag(st_zEntVillain* vil, en_ZVIL_FLAG_GROUPS flaggroup,
                           en_BEHAVIOUR_GOAL goal)
{
    return VILN_std_flag(vil, flaggroup, goal);
}


// The shark answers queries the standard way.
static S32 VILN_shark_query(st_zEntVillain* vil, en_ZVIL_QUERY query)
{
    return VILN_std_query(vil, query);
}

// The shark handles game events the standard way.
static S32 VILN_shark_sysevent(st_zEntVillain* vil, xBase* from, U32 toEvent, F32* toParam,
                               xBase* toParamWidget, st_STIMULUS_CONTEXT_DATA* resp)
{
    return VILN_std_sysevent(vil, from, toEvent, toParam, toParamWidget, resp);
}

// The shark changes goals the standard way.
static S32 VILN_shark_trangoal(st_zEntVillain* vil, st_STIMULUS_CONTEXT_DATA* stimctxt, F32 dt)
{
    return VILN_std_trangoal(vil, stimctxt, dt);
}

// The shark has nothing extra to do here; this always succeeds.
static S32 VILN_shark_calcmove(st_zEntVillain*, _xScene*, F32, const _xEntFrame*)
{
    return 1;
}

// The shark has nothing extra to do here; this always succeeds.
static S32 VILN_shark_respond(st_zEntVillain*, st_STIMULUS_CONTEXT_DATA*)
{
    return 1;
}

// The Shaggy villain types other than Shaggy5 read flag groups the standard way.
static S32 VILN_shag_flag(st_zEntVillain* vil, en_ZVIL_FLAG_GROUPS flaggroup,
                          en_BEHAVIOUR_GOAL goal)
{
    return VILN_std_flag(vil, flaggroup, goal);
}


// The Shaggy villain types other than Shaggy5 answer queries the standard way.
static S32 VILN_shag_query(st_zEntVillain* vil, en_ZVIL_QUERY query)
{
    return VILN_std_query(vil, query);
}

// The Shaggy villain types other than Shaggy5 handle game events the standard way.
static S32 VILN_shag_sysevent(st_zEntVillain* vil, xBase* from, U32 toEvent, F32* toParam,
                              xBase* toParamWidget, st_STIMULUS_CONTEXT_DATA* resp)
{
    return VILN_std_sysevent(vil, from, toEvent, toParam, toParamWidget, resp);
}

// The Shaggy villain types other than Shaggy5 react to notices the standard way.
static S32 VILN_shag_notice(st_zEntVillain* vil, en_ZVIL_NOTICE note,
                            st_STIMULUS_CONTEXT_DATA* resp)
{
    return VILN_std_notice(vil, note, resp);
}


// The Shaggy villain types other than Shaggy5 sense the player the standard way.
static S32 VILN_shag_aware(st_zEntVillain* vil, _xScene* scn, F32 dt,
                           st_STIMULUS_CONTEXT_DATA* resp)
{
    return VILN_std_aware(vil, scn, dt, resp);
}

// The Shaggy villain types other than Shaggy5 react to stimuli the standard way.
static S32 VILN_shag_stimulus(st_zEntVillain* vil, st_STIMULUS_CONTEXT_DATA* stimctxt,
                              st_STIMULUS_CONTEXT_DATA* resp)
{
    return VILN_std_stimulus(vil, stimctxt, resp);
}

// The Shaggy villain types other than Shaggy5 change goals the standard way.
static S32 VILN_shag_trangoal(st_zEntVillain* vil, st_STIMULUS_CONTEXT_DATA* stimctxt, F32 dt)
{
    return VILN_std_trangoal(vil, stimctxt, dt);
}

// The Shaggy villain types other than Shaggy5 move the standard way.
static S32 VILN_shag_calcmove(st_zEntVillain* vil, _xScene* scn, F32 dt, const _xEntFrame* frame)
{
    return VILN_std_calcmove(vil, scn, dt, frame);
}

// The Shaggy villain types other than Shaggy5 respond to stimuli the standard way.
static S32 VILN_shag_respond(st_zEntVillain* vil, st_STIMULUS_CONTEXT_DATA* resp)
{
    return VILN_std_respond(vil, resp);
}


// The Shaggy villain types other than Shaggy5 pick animations the standard way.
static S32 VILN_shag_pickanim(st_zEntVillain* vil, en_BEHAVIOUR_GOAL goal, en_ZVIL_GOAL_SPOT gspot)
{
    return VILN_std_pickanim(vil, goal, gspot);
}

// Shaggy villain type 5 reads flag groups the standard way.
static S32 VILN_shagM_flag(st_zEntVillain* vil, en_ZVIL_FLAG_GROUPS flaggroup,
                           en_BEHAVIOUR_GOAL goal)
{
    return VILN_std_flag(vil, flaggroup, goal);
}


// Shaggy villain type 5 answers queries the standard way.
static S32 VILN_shagM_query(st_zEntVillain* vil, en_ZVIL_QUERY query)
{
    return VILN_std_query(vil, query);
}

// Shaggy villain type 5 handles game events the standard way.
static S32 VILN_shagM_sysevent(st_zEntVillain* vil, xBase* from, U32 toEvent, F32* toParam,
                               xBase* toParamWidget, st_STIMULUS_CONTEXT_DATA* resp)
{
    return VILN_std_sysevent(vil, from, toEvent, toParam, toParamWidget, resp);
}

// Shaggy villain type 5 reacts to notices the standard way.
static S32 VILN_shagM_notice(st_zEntVillain* vil, en_ZVIL_NOTICE note,
                             st_STIMULUS_CONTEXT_DATA* resp)
{
    return VILN_std_notice(vil, note, resp);
}


// Shaggy villain type 5 has nothing extra to do here; this always succeeds.
static S32 VILN_shagM_respond(st_zEntVillain*, st_STIMULUS_CONTEXT_DATA*)
{
    return 1;
}

// Shaggy villain type 5 asks for no particular animation and sets its play speed to 0.97.
static S32 VILN_shagM_pickanim(st_zEntVillain* vil, en_BEHAVIOUR_GOAL, en_ZVIL_GOAL_SPOT)
{
    vil->anim_playnext = ZVIL_ANIM_UNKNOWN;
    vil->anim_playing = 0;
    vil->anim_loopmult = 0.97f;
    return 1;
}


// Fred, Daphne, Velma, Holly, the groundskeeper and the professor read flag groups the standard
// way.
static S32 VILN_gang_flag(st_zEntVillain* vil, en_ZVIL_FLAG_GROUPS flaggroup,
                          en_BEHAVIOUR_GOAL goal)
{
    return VILN_std_flag(vil, flaggroup, goal);
}


// Fred, Daphne, Velma, Holly, the groundskeeper and the professor answer queries the standard way.
static S32 VILN_gang_query(st_zEntVillain* vil, en_ZVIL_QUERY query)
{
    return VILN_std_query(vil, query);
}

// Fred, Daphne, Velma, Holly, the groundskeeper and the professor handle game events the standard
// way.
static S32 VILN_gang_sysevent(st_zEntVillain* vil, xBase* from, U32 toEvent, F32* toParam,
                              xBase* toParamWidget, st_STIMULUS_CONTEXT_DATA* resp)
{
    return VILN_std_sysevent(vil, from, toEvent, toParam, toParamWidget, resp);
}

// Fred, Daphne, Velma, Holly, the groundskeeper and the professor have nothing extra to do here;
// this always succeeds.
static S32 VILN_gang_respond(st_zEntVillain*, st_STIMULUS_CONTEXT_DATA*)
{
    return 1;
}

// When Fred changes animation, his first model part gets flag bits 1 and 0 set, and his second
// part has bit 0 set only while he plays the gang boss action.
static S32 VILN_gang_animtran(st_zEntVillain* vil, xAnimTransition* tran, xAnimSingle*)
{
    xModelInstance* minst = vil->model;
    S32 idx = 0;

    if (vil->asset->subtype != eEntVillainTypeFred) return 1;

    while (minst)
    {
        switch (idx)
        {
        case 0:
            minst->Flags |= 0x2;
            minst->Flags |= 0x1;
            break;
        case 1:
            if (tran->Dest->ID == g_anamhash[ZVIL_ANIM_GANG_BOSSACTION])
            {
                minst->Flags |= 0x1;
            }
            else
            {
                minst->Flags &= ~0x1;
            }
            break;
        }
        minst = minst->Next;
        idx++;
    }

    return 1;
}


// The Black Knight, Green Ghost, Phantom and Redbeard outside their boss fights read flag groups
// the standard way.
static S32 VILN_sham_flag(st_zEntVillain* vil, en_ZVIL_FLAG_GROUPS flaggroup,
                          en_BEHAVIOUR_GOAL goal)
{
    return VILN_std_flag(vil, flaggroup, goal);
}


// The Black Knight, Green Ghost, Phantom and Redbeard outside their boss fights answer queries the
// standard way.
static S32 VILN_sham_query(st_zEntVillain* vil, en_ZVIL_QUERY query)
{
    return VILN_std_query(vil, query);
}

// The Black Knight, Green Ghost, Phantom and Redbeard outside their boss fights handle game events
// the standard way.
static S32 VILN_sham_sysevent(st_zEntVillain* vil, xBase* from, U32 toEvent, F32* toParam,
                              xBase* toParamWidget, st_STIMULUS_CONTEXT_DATA* resp)
{
    return VILN_std_sysevent(vil, from, toEvent, toParam, toParamWidget, resp);
}

// The Black Knight, Green Ghost, Phantom and Redbeard outside their boss fights react to notices
// the standard way.
static S32 VILN_sham_notice(st_zEntVillain* vil, en_ZVIL_NOTICE note,
                            st_STIMULUS_CONTEXT_DATA* resp)
{
    return VILN_std_notice(vil, note, resp);
}


// The Black Knight, Green Ghost, Phantom and Redbeard outside their boss fights have nothing extra
// to do here; this always succeeds.
static S32 VILN_sham_respond(st_zEntVillain*, st_STIMULUS_CONTEXT_DATA*)
{
    return 1;
}

// The Black Knight boss reads flag groups the standard way.
static S32 VILN_BBK_flag(st_zEntVillain* vil, en_ZVIL_FLAG_GROUPS flaggroup, en_BEHAVIOUR_GOAL goal)
{
    return VILN_std_flag(vil, flaggroup, goal);
}


// The Black Knight boss answers queries the standard way.
static S32 VILN_BBK_query(st_zEntVillain* vil, en_ZVIL_QUERY query)
{
    return VILN_std_query(vil, query);
}

// The Black Knight boss has nothing extra to do here; this always succeeds.
static S32 VILN_BBK_respond(st_zEntVillain*, st_STIMULUS_CONTEXT_DATA*)
{
    return 1;
}

// When the Black Knight boss changes animation, his first model part has flag bit 0 cleared during
// the shock skeleton animation and bits 1 and 0 set otherwise, and his second part the reverse.
static S32 VILN_BBK_animtran(st_zEntVillain* vil, xAnimTransition* tran, xAnimSingle*)
{
    xModelInstance* minst = vil->model;
    S32 idx;

    idx = 0;
    while (minst)
    {
        switch (idx)
        {
        case 0:
            if (tran->Dest->ID == g_anamhash[ZVIL_ANIM_BK_SHOCKSKEL])
            {
                minst->Flags &= ~0x1;
            }
            else
            {
                minst->Flags |= 0x2;
                minst->Flags |= 0x1;
            }
            break;
        case 1:
            if (tran->Dest->ID == g_anamhash[ZVIL_ANIM_BK_SHOCKSKEL])
            {
                minst->Flags |= 0x2;
                minst->Flags |= 0x1;
            }
            else
            {
                minst->Flags &= ~0x1;
            }
            break;
        }
        minst = minst->Next;
        idx++;
    }

    return 1;
}


// The Green Ghost boss reads flag groups the standard way.
static S32 VILN_BGG_flag(st_zEntVillain* vil, en_ZVIL_FLAG_GROUPS flaggroup, en_BEHAVIOUR_GOAL goal)
{
    return VILN_std_flag(vil, flaggroup, goal);
}


// The Green Ghost boss answers queries the standard way.
static S32 VILN_BGG_query(st_zEntVillain* vil, en_ZVIL_QUERY query)
{
    return VILN_std_query(vil, query);
}

// The Green Ghost boss handles game events the standard way.
static S32 VILN_BGG_sysevent(st_zEntVillain* vil, xBase* from, U32 toEvent, F32* toParam,
                             xBase* toParamWidget, st_STIMULUS_CONTEXT_DATA* resp)
{
    return VILN_std_sysevent(vil, from, toEvent, toParam, toParamWidget, resp);
}

// The Green Ghost boss has nothing extra to do here; this always succeeds.
static S32 VILN_BGG_respond(st_zEntVillain*, st_STIMULUS_CONTEXT_DATA*)
{
    return 1;
}

// The Redbeard boss reads flag groups the standard way.
static S32 VILN_BRB_flag(st_zEntVillain* vil, en_ZVIL_FLAG_GROUPS flaggroup, en_BEHAVIOUR_GOAL goal)
{
    return VILN_std_flag(vil, flaggroup, goal);
}


// The Redbeard boss answers queries the standard way.
static S32 VILN_BRB_query(st_zEntVillain* vil, en_ZVIL_QUERY query)
{
    return VILN_std_query(vil, query);
}

// The Redbeard boss handles game events the standard way.
static S32 VILN_BRB_sysevent(st_zEntVillain* vil, xBase* from, U32 toEvent, F32* toParam,
                             xBase* toParamWidget, st_STIMULUS_CONTEXT_DATA* resp)
{
    return VILN_std_sysevent(vil, from, toEvent, toParam, toParamWidget, resp);
}

// The Redbeard boss has nothing extra to do here; this always succeeds.
static S32 VILN_BRB_respond(st_zEntVillain*, st_STIMULUS_CONTEXT_DATA*)
{
    return 1;
}

// The Redbeard boss draws the model of his first sword when its status bit 2 is set.
static S32 VILN_BRB_xtrarend(st_zEntVillain*)
{
    st_REDBEARD_SWORD* rbs = &g_rbsword[0];
    if (rbs->flg_stat & 0x4) xModelRender(rbs->model);
    return 1;
}


// The Mastermind boss reads flag groups the standard way.
static S32 VILN_BMM_flag(st_zEntVillain* vil, en_ZVIL_FLAG_GROUPS flaggroup, en_BEHAVIOUR_GOAL goal)
{
    return VILN_std_flag(vil, flaggroup, goal);
}


// The Mastermind boss answers whether the player is too close (within a squared distance of 9)
// and whether he can attack (bit 3 of his allow flags); any other question gets the standard
// answer.
static S32 VILN_BMM_query(st_zEntVillain* vil, en_ZVIL_QUERY query)
{
    st_ZVIL_PRECALC* prec = &vil->precalc;
    S32 result;

    switch (query)
    {
    case ZVIL_Q_PLYRTOOCLOSE:
        if (prec->ds2_VtoP < 9.0f)
        {
            result = 1;
        }
        else
        {
            result = 0;
        }
        break;
    case ZVIL_Q_CANATTACK:
        if (vil->allow & 0x8)
        {
            result = 1;
        }
        else
        {
            result = 0;
        }
        break;
    default:
        result = VILN_std_query(vil, query);
        break;
    }

    return result;
}

// The Mastermind boss turns the push-button event into the push-button stimulus, unless he is set
// up with more than one hit point and is down to his last; other events are handled the standard
// way.
static S32 VILN_BMM_sysevent(st_zEntVillain* vil, xBase* from, U32 toEvent, F32* toParam,
                             xBase* toParamWidget, st_STIMULUS_CONTEXT_DATA* resp)
{
    S32 result;

    switch (toEvent)
    {
    case eEventBossMMPushButton:
        if (vil->vilcfg.hitpts > 1)
        {
            if (vil->hitpts > 1)
            {
                resp->stim = ZVIL_STIM_MMPUSHBUTTON;
            }
        }
        else
        {
            resp->stim = ZVIL_STIM_MMPUSHBUTTON;
        }
        result = 1;
        break;
    case eEventNPCDetectOn:
    case eEventNPCDetectOff:
    case eEventNPCChaseOn:
    case eEventNPCChaseOff:
    default:
        result = VILN_std_sysevent(vil, from, toEvent, toParam, toParamWidget, resp);
        break;
    }

    return result;
}


// Sets up a model tag for each of the villain's configured vertex positions that is not zero, and
// marks it in flg_vert.
static void zVIL_tag_verts(st_zEntVillain* ent)
{
    _xVec3* tmp_pos;
    S32 i;

    for (i = 0; i < 12; i++)
    {
        tmp_pos = &ent->vilcfg.pos_verts[i];
        if (xVec3Length2(tmp_pos))
        {
            iModelTagSetup(&ent->tag_vert[i], ent->model->Data, tmp_pos->x, tmp_pos->y,
                           tmp_pos->z);
            ent->flg_vert |= 1 << i;
            xSceneID2Name(globals.sceneCur, ent->id);
        }
        else
        {
            xSceneID2Name(globals.sceneCur, ent->id);
        }
    }
}


// Builds the Black Knight's animation table: patrol, shock, attack and gloat states.
static xAnimTable* VANM_mktable_BK()
{
    xAnimTable* table;
    xAnimTransition* tmp_tran;
    char* strptrz[32] = {NULL};
    S32 i;

    table = xAnimTableNew("Villain_BlackKnight", NULL);
    xAnimTableNewState(table, "Idle", 0x110, 1, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "BK_ScaryIdle", 0x20, 1, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "BK_Walk", 0x10, 0x2000004, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "BK_Turn", 0x10, 0x2000001, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "BK_Patrol", 0x10, 0x2000004, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "BK_PatrolFlip", 0x10, 0x2000004, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "BK_Attack", 0x20, 0x2000001, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "BK_Shock", 0x10, 0x2000001, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "BK_ShockSkel", 0x10, 0x2000001, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "BK_Gloat", 0x10, 0x2000001, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "BK_Gloat_1", 0x10, 0x2000001, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "BK_Gloat_2", 0x10, 0x2000001, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "BK_Death", 0, 0x2000001, 1.0f, NULL, NULL, 0.0f, NULL);
    for (i = ZVIL_ANIM_BK_SCARYIDLE; i < ZVIL_ANIM_GG_CHASE; i++)
    {
        xAnimTableNewTransition(
            table, g_animname[i], "Idle", (xAnimTransitionConditionalCallback)VANM_dodflt,
            (xAnimTransitionCallback)VANM_diddflt, 0x10, 0, 0.0f, 0.0f, 0, 0, 0.1f, NULL);
        xAnimTableNewTransition(
            table, g_animname[i], "Idle", (xAnimTransitionConditionalCallback)VANM_dotran,
            (xAnimTransitionCallback)VANM_didtran, 0, 0, 0.0f, 0.0f, 5, 0, 0.1f, NULL);
        xAnimTableNewTransition(
            table, "Idle", g_animname[i], (xAnimTransitionConditionalCallback)VANM_dotran,
            (xAnimTransitionCallback)VANM_didtran, 0, 0, 0.0f, 0.0f, 10, 0, 0.1f, NULL);
    }
    strptrz[0] = "BK_ScaryIdle";
    strptrz[1] = "BK_Patrol";
    strptrz[2] = "BK_PatrolFlip";
    strptrz[3] = "BK_Shock";
    strptrz[4] = "BK_ShockSkel";
    strptrz[5] = "BK_Gloat";
    strptrz[6] = "BK_Gloat_1";
    strptrz[7] = "BK_Gloat_2";
    for (i = 0; i < 8; i++)
    {
        xAnimTableNewTransition(
            table, strptrz[i], "BK_Turn", (xAnimTransitionConditionalCallback)VANM_dotran,
            (xAnimTransitionCallback)VANM_didtran, 0, 0, 0.0f, 0.0f, 10, 0, 0.1f, NULL);
        xAnimTableNewTransition(
            table, strptrz[i], "BK_Walk", (xAnimTransitionConditionalCallback)VANM_dotran,
            (xAnimTransitionCallback)VANM_didtran, 0, 0, 0.0f, 0.0f, 10, 0, 0.1f, NULL);
    }
    xAnimTableNewTransition(
        table, "BK_Patrol", "BK_PatrolFlip", (xAnimTransitionConditionalCallback)VANM_dotran,
        (xAnimTransitionCallback)VANM_didtran, 0, 0, 0.0f, 0.0f, 10, 0, 0.1f, NULL);
    xAnimTableNewTransition(
        table, "BK_PatrolFlip", "BK_Patrol", (xAnimTransitionConditionalCallback)VANM_dotran,
        (xAnimTransitionCallback)VANM_didtran, 0, 0, 0.0f, 0.0f, 10, 0, 0.1f, NULL);
    strptrz[0] = "BK_ScaryIdle";
    strptrz[1] = "BK_Walk";
    strptrz[2] = "BK_Turn";
    strptrz[3] = "BK_Shock";
    strptrz[4] = "BK_ShockSkel";
    strptrz[5] = "BK_Gloat";
    strptrz[6] = "BK_Gloat_1";
    strptrz[7] = "BK_Gloat_2";
    for (i = 0; i < 8; i++)
    {
        xAnimTableNewTransition(
            table, strptrz[i], "BK_Patrol", (xAnimTransitionConditionalCallback)VANM_dotran,
            (xAnimTransitionCallback)VANM_didtran, 0, 0, 0.0f, 0.0f, 10, 0, 0.1f, NULL);
        xAnimTableNewTransition(
            table, strptrz[i], "BK_PatrolFlip", (xAnimTransitionConditionalCallback)VANM_dotran,
            (xAnimTransitionCallback)VANM_didtran, 0, 0, 0.0f, 0.0f, 10, 0, 0.1f, NULL);
    }
    strptrz[0] = "BK_ScaryIdle";
    strptrz[1] = "BK_Walk";
    strptrz[2] = "BK_Turn";
    strptrz[3] = "BK_Patrol";
    strptrz[4] = "BK_PatrolFlip";
    strptrz[5] = "BK_Shock";
    strptrz[6] = "BK_ShockSkel";
    strptrz[7] = "BK_Gloat";
    strptrz[8] = "BK_Gloat_1";
    strptrz[9] = "BK_Gloat_2";
    for (i = 0; i < 10; i++)
    {
        xAnimTableNewTransition(
            table, strptrz[i], "BK_Attack", (xAnimTransitionConditionalCallback)VANM_dotran,
            (xAnimTransitionCallback)VANM_didtran, 0, 0, 0.0f, 0.0f, 10, 0, 0.1f, NULL);
    }
    tmp_tran = xAnimTableNewTransition(
        table, "BK_Shock", "BK_ShockSkel", (xAnimTransitionConditionalCallback)VANM_dotran,
        (xAnimTransitionCallback)VANM_didtran, 0, 0, 0.0f, 0.0f, 10, 0, 0.1f, NULL);
    tmp_tran->Flags |= 0x20;
    tmp_tran = xAnimTableNewTransition(
        table, "BK_ShockSkel", "BK_Shock", (xAnimTransitionConditionalCallback)VANM_dotran,
        (xAnimTransitionCallback)VANM_didtran, 0, 0, 0.0f, 0.0f, 10, 0, 0.1f, NULL);
    tmp_tran->Flags |= 0x20;
    strptrz[0] = "BK_ScaryIdle";
    strptrz[1] = "BK_Walk";
    strptrz[2] = "BK_Turn";
    strptrz[3] = "BK_Patrol";
    strptrz[4] = "BK_PatrolFlip";
    strptrz[5] = "BK_Gloat";
    strptrz[6] = "BK_Gloat_1";
    strptrz[7] = "BK_Gloat_2";
    for (i = 0; i < 8; i++)
    {
        xAnimTableNewTransition(
            table, strptrz[i], "BK_ShockSkel", (xAnimTransitionConditionalCallback)VANM_dotran,
            (xAnimTransitionCallback)VANM_didtran, 0, 0, 0.0f, 0.0f, 10, 0, 0.1f, NULL);
        xAnimTableNewTransition(
            table, strptrz[i], "BK_Shock", (xAnimTransitionConditionalCallback)VANM_dotran,
            (xAnimTransitionCallback)VANM_didtran, 0, 0, 0.0f, 0.0f, 10, 0, 0.1f, NULL);
    }
    strptrz[0] = "BK_ScaryIdle";
    strptrz[1] = "BK_Walk";
    strptrz[2] = "BK_Turn";
    strptrz[3] = "BK_Patrol";
    strptrz[4] = "BK_PatrolFlip";
    strptrz[5] = "BK_Shock";
    strptrz[6] = "BK_ShockSkel";
    strptrz[7] = "BK_Attack";
    for (i = 0; i < 8; i++)
    {
        xAnimTableNewTransition(
            table, strptrz[i], "BK_Gloat", (xAnimTransitionConditionalCallback)VANM_dotran,
            (xAnimTransitionCallback)VANM_didtran, 0, 0, 0.0f, 0.0f, 10, 0, 0.1f, NULL);
        xAnimTableNewTransition(
            table, strptrz[i], "BK_Gloat_1", (xAnimTransitionConditionalCallback)VANM_dotran,
            (xAnimTransitionCallback)VANM_didtran, 0, 0, 0.0f, 0.0f, 10, 0, 0.1f, NULL);
        xAnimTableNewTransition(
            table, strptrz[i], "BK_Gloat_2", (xAnimTransitionConditionalCallback)VANM_dotran,
            (xAnimTransitionCallback)VANM_didtran, 0, 0, 0.0f, 0.0f, 10, 0, 0.1f, NULL);
    }
    strptrz[0] = "BK_ScaryIdle";
    strptrz[1] = "BK_Walk";
    strptrz[2] = "BK_Turn";
    strptrz[3] = "BK_Patrol";
    strptrz[4] = "BK_PatrolFlip";
    strptrz[5] = "BK_Shock";
    strptrz[6] = "BK_ShockSkel";
    strptrz[7] = "BK_Gloat";
    strptrz[8] = "BK_Gloat_1";
    strptrz[9] = "BK_Gloat_2";
    for (i = 0; i < 10; i++)
    {
        xAnimTableNewTransition(
            table, strptrz[i], "BK_Death", (xAnimTransitionConditionalCallback)VANM_dotran,
            (xAnimTransitionCallback)VANM_didtran, 0, 0, 0.0f, 0.0f, 10, 0, 0.1f, NULL);
    }

    return table;
}

// Builds the Green Ghost's animation table: chase, attack build-up, gloat and hit states.
static xAnimTable* VANM_mktable_GG()
{
    xAnimTable* table;
    char* strptrz[32] = {NULL};
    S32 i;

    table = xAnimTableNew("Villain_GreenGhost", NULL);
    xAnimTableNewState(table, "Idle", 0x10, 1, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "GG_Chase", 0x10, 0x2000004, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "GG_ChaseLeft", 0x10, 0x2000004, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "GG_ChaseRight", 0x10, 0x2000004, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "GG_AttackBuildBegin", 0x20, 1, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "GG_AttackBuildLoop", 0x10, 1, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "GG_Attack", 0x20, 1, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "GG_Gloat", 0x10, 1, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "GG_Gloat_1", 0x10, 1, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "GG_Hit", 0x10, 1, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "GG_Hit_1", 0x10, 1, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "GG_Hit_2", 0, 1, 1.0f, NULL, NULL, 0.0f, NULL);
    for (i = ZVIL_ANIM_GG_CHASE; i < ZVIL_ANIM_RB_IDLEWIND; i++)
    {
        xAnimTableNewTransition(
            table, g_animname[i], "Idle", (xAnimTransitionConditionalCallback)VANM_dodflt,
            (xAnimTransitionCallback)VANM_diddflt, 0x10, 0, 0.0f, 0.0f, 0, 0, 0.2f, NULL);
        xAnimTableNewTransition(
            table, g_animname[i], "Idle", (xAnimTransitionConditionalCallback)VANM_dotran,
            (xAnimTransitionCallback)VANM_didtran, 0, 0, 0.0f, 0.0f, 5, 0, 0.2f, NULL);
        xAnimTableNewTransition(
            table, "Idle", g_animname[i], (xAnimTransitionConditionalCallback)VANM_dotran,
            (xAnimTransitionCallback)VANM_didtran, 0, 0, 0.0f, 0.0f, 10, 0, 0.2f, NULL);
    }
    xAnimTableNewTransition(
        table, "GG_Chase", "GG_ChaseLeft", (xAnimTransitionConditionalCallback)VANM_dotran,
        (xAnimTransitionCallback)VANM_didtran, 0, 0, 0.0f, 0.0f, 10, 0, 0.2f, NULL);
    xAnimTableNewTransition(
        table, "GG_Chase", "GG_ChaseRight", (xAnimTransitionConditionalCallback)VANM_dotran,
        (xAnimTransitionCallback)VANM_didtran, 0, 0, 0.0f, 0.0f, 10, 0, 0.2f, NULL);
    xAnimTableNewTransition(
        table, "GG_ChaseLeft", "GG_Chase", (xAnimTransitionConditionalCallback)VANM_dotran,
        (xAnimTransitionCallback)VANM_didtran, 0, 0, 0.0f, 0.0f, 10, 0, 0.2f, NULL);
    xAnimTableNewTransition(
        table, "GG_ChaseLeft", "GG_ChaseRight", (xAnimTransitionConditionalCallback)VANM_dotran,
        (xAnimTransitionCallback)VANM_didtran, 0, 0, 0.0f, 0.0f, 10, 0, 0.2f, NULL);
    xAnimTableNewTransition(
        table, "GG_ChaseRight", "GG_Chase", (xAnimTransitionConditionalCallback)VANM_dotran,
        (xAnimTransitionCallback)VANM_didtran, 0, 0, 0.0f, 0.0f, 10, 0, 0.2f, NULL);
    xAnimTableNewTransition(
        table, "GG_ChaseRight", "GG_ChaseLeft", (xAnimTransitionConditionalCallback)VANM_dotran,
        (xAnimTransitionCallback)VANM_didtran, 0, 0, 0.0f, 0.0f, 10, 0, 0.2f, NULL);
    strptrz[0] = "GG_Chase";
    strptrz[1] = "GG_ChaseLeft";
    strptrz[2] = "GG_ChaseRight";
    strptrz[3] = "GG_Gloat";
    strptrz[4] = "GG_Gloat_1";
    strptrz[5] = "GG_Hit";
    strptrz[6] = "GG_Hit_1";
    strptrz[7] = "GG_Hit_2";
    for (i = 0; i < 8; i++)
    {
        xAnimTableNewTransition(table, strptrz[i], "GG_AttackBuildBegin",
                                (xAnimTransitionConditionalCallback)VANM_dotran,
                                (xAnimTransitionCallback)VANM_didtran, 0, 0, 0.0f, 0.0f, 10, 0,
                                0.2f, NULL);
    }
    strptrz[0] = "GG_Chase";
    strptrz[1] = "GG_ChaseLeft";
    strptrz[2] = "GG_ChaseRight";
    strptrz[3] = "GG_AttackBuildBegin";
    strptrz[4] = "GG_AttackBuildLoop";
    strptrz[5] = "GG_Attack";
    strptrz[6] = "GG_Hit";
    strptrz[7] = "GG_Hit_1";
    strptrz[8] = "GG_Hit_2";
    for (i = 0; i < 9; i++)
    {
        xAnimTableNewTransition(
            table, strptrz[i], "GG_Gloat", (xAnimTransitionConditionalCallback)VANM_dotran,
            (xAnimTransitionCallback)VANM_didtran, 0, 0, 0.0f, 0.0f, 10, 0, 0.2f, NULL);
        xAnimTableNewTransition(
            table, strptrz[i], "GG_Gloat_1", (xAnimTransitionConditionalCallback)VANM_dotran,
            (xAnimTransitionCallback)VANM_didtran, 0, 0, 0.0f, 0.0f, 10, 0, 0.2f, NULL);
    }
    xAnimTableNewTransition(table, "GG_AttackBuildBegin", "GG_AttackBuildLoop",
                            (xAnimTransitionConditionalCallback)VANM_dodflt,
                            (xAnimTransitionCallback)VANM_diddflt, 0x10, 0, 0.0f, 0.0f, 5, 0, 0.2f,
                            NULL);
    xAnimTableNewTransition(
        table, "GG_AttackBuildLoop", "GG_Attack", (xAnimTransitionConditionalCallback)VANM_dotran,
        (xAnimTransitionCallback)VANM_didtran, 0, 0, 0.0f, 0.0f, 10, 0, 0.2f, NULL);
    strptrz[0] = "GG_Chase";
    strptrz[1] = "GG_ChaseLeft";
    strptrz[2] = "GG_ChaseRight";
    strptrz[3] = "GG_AttackBuildBegin";
    strptrz[4] = "GG_AttackBuildLoop";
    strptrz[5] = "GG_Attack";
    strptrz[6] = "GG_Gloat";
    strptrz[7] = "GG_Gloat_1";
    for (i = 0; i < 8; i++)
    {
        xAnimTableNewTransition(
            table, strptrz[i], "GG_Hit", (xAnimTransitionConditionalCallback)VANM_dotran,
            (xAnimTransitionCallback)VANM_didtran, 0, 0, 0.0f, 0.0f, 10, 0, 0.2f, NULL);
        xAnimTableNewTransition(
            table, strptrz[i], "GG_Hit_1", (xAnimTransitionConditionalCallback)VANM_dotran,
            (xAnimTransitionCallback)VANM_didtran, 0, 0, 0.0f, 0.0f, 10, 0, 0.2f, NULL);
        xAnimTableNewTransition(
            table, strptrz[i], "GG_Hit_2", (xAnimTransitionConditionalCallback)VANM_dotran,
            (xAnimTransitionCallback)VANM_didtran, 0, 0, 0.0f, 0.0f, 10, 0, 0.2f, NULL);
    }

    return table;
}

// Builds Redbeard's animation table: walk, taunt, punch and throw attacks, look and death.
static xAnimTable* VANM_mktable_RB()
{
    xAnimTable* table;
    char* strptrz[32] = {NULL};
    S32 i;

    table = xAnimTableNew("Villain_RedBeard", NULL);
    xAnimTableNewState(table, "Idle", 0x110, 1, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "RB_IdleWind", 0x110, 1, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "RB_Walk", 0x10, 0x2000004, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "RB_Turn", 0x10, 0x2000001, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "RB_Chase", 0x10, 0x2000006, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "RB_TauntFeel", 0x10, 0x2000001, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "RB_TauntThroat", 0x10, 0x2000001, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "RB_TauntLaugh", 0x10, 0x2000001, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "RB_AttackPunch", 0x20, 0x2000001, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "RB_AttackThrow", 0x20, 1, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "RB_AttackThrowWait", 0x10, 1, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "RB_AttackCatch", 0x20, 1, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "RB_React", 0x10, 1, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "RB_LookBegin", 0x20, 1, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "RB_LookLoop", 0x10, 1, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "RB_LookEnd", 0x20, 1, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "RB_DeathLoop", 0x10, 1, 1.0f, NULL, NULL, 0.0f, NULL);
    for (i = ZVIL_ANIM_RB_IDLEWIND; i < ZVIL_ANIM_MM_FP_MOVE; i++)
    {
        xAnimTableNewTransition(
            table, g_animname[i], "Idle", (xAnimTransitionConditionalCallback)VANM_dodflt,
            (xAnimTransitionCallback)VANM_diddflt, 0x10, 0, 0.0f, 0.0f, 0, 0, 0.2f, NULL);
        xAnimTableNewTransition(
            table, g_animname[i], "Idle", (xAnimTransitionConditionalCallback)VANM_dotran,
            (xAnimTransitionCallback)VANM_didtran, 0, 0, 0.0f, 0.0f, 5, 0, 0.2f, NULL);
        xAnimTableNewTransition(
            table, "Idle", g_animname[i], (xAnimTransitionConditionalCallback)VANM_dotran,
            (xAnimTransitionCallback)VANM_didtran, 0, 0, 0.0f, 0.0f, 10, 0, 0.2f, NULL);
        if (i != ZVIL_ANIM_RB_DEATH_LOOP)
        {
            xAnimTableNewTransition(table, g_animname[i], "RB_DeathLoop",
                                    (xAnimTransitionConditionalCallback)VANM_dotran,
                                    (xAnimTransitionCallback)VANM_didtran, 0, 0, 0.0f, 0.0f, 10, 0,
                                    0.2f, NULL);
        }
    }
    xAnimTableNewTransition(
        table, "RB_IdleWind", "Idle", (xAnimTransitionConditionalCallback)VANM_dodflt,
        (xAnimTransitionCallback)VANM_diddflt, 0x10, 0, 0.0f, 0.0f, 1, 0, 0.2f, NULL);
    xAnimTableNewTransition(
        table, "RB_IdleWind", "RB_Turn", (xAnimTransitionConditionalCallback)VANM_dotran,
        (xAnimTransitionCallback)VANM_didtran, 0, 0, 0.0f, 0.0f, 10, 0, 0.2f, NULL);
    xAnimTableNewTransition(
        table, "RB_Turn", "RB_IdleWind", (xAnimTransitionConditionalCallback)VANM_dotran,
        (xAnimTransitionCallback)VANM_didtran, 0, 0, 0.0f, 0.0f, 10, 0, 0.2f, NULL);
    xAnimTableNewTransition(
        table, "RB_Chase", "RB_Walk", (xAnimTransitionConditionalCallback)VANM_dotran,
        (xAnimTransitionCallback)VANM_didtran, 0, 0, 0.0f, 0.0f, 10, 0, 0.2f, NULL);
    xAnimTableNewTransition(
        table, "RB_Walk", "RB_Chase", (xAnimTransitionConditionalCallback)VANM_dotran,
        (xAnimTransitionCallback)VANM_didtran, 0, 0, 0.0f, 0.0f, 10, 0, 0.2f, NULL);
    strptrz[0] = "RB_IdleWind";
    strptrz[1] = "RB_Turn";
    strptrz[2] = "RB_React";
    strptrz[3] = "RB_TauntFeel";
    strptrz[4] = "RB_TauntThroat";
    strptrz[5] = "RB_TauntLaugh";
    strptrz[6] = "RB_AttackPunch";
    strptrz[7] = "RB_AttackThrowWait";
    strptrz[8] = "RB_AttackThrow";
    strptrz[9] = "RB_AttackPunch";
    strptrz[10] = "RB_LookBegin";
    strptrz[11] = "RB_LookLoop";
    strptrz[12] = "RB_LookEnd";
    for (i = 0; i < 13; i++)
    {
        xAnimTableNewTransition(
            table, strptrz[i], "RB_Chase", (xAnimTransitionConditionalCallback)VANM_dotran,
            (xAnimTransitionCallback)VANM_didtran, 0, 0, 0.0f, 0.0f, 10, 0, 0.2f, NULL);
        xAnimTableNewTransition(
            table, strptrz[i], "RB_Walk", (xAnimTransitionConditionalCallback)VANM_dotran,
            (xAnimTransitionCallback)VANM_didtran, 0, 0, 0.0f, 0.0f, 10, 0, 0.2f, NULL);
    }
    strptrz[0] = "RB_IdleWind";
    strptrz[1] = "RB_React";
    strptrz[2] = "RB_AttackThrow";
    strptrz[3] = "RB_AttackCatch";
    strptrz[4] = "RB_AttackCatch";
    strptrz[5] = "RB_AttackPunch";
    strptrz[6] = "RB_LookBegin";
    strptrz[7] = "RB_LookLoop";
    strptrz[8] = "RB_LookEnd";
    strptrz[9] = "RB_Chase";
    strptrz[10] = "RB_Walk";
    strptrz[11] = "RB_Turn";
    for (i = 0; i < 12; i++)
    {
        xAnimTableNewTransition(
            table, strptrz[i], "RB_TauntFeel", (xAnimTransitionConditionalCallback)VANM_dotran,
            (xAnimTransitionCallback)VANM_didtran, 0, 0, 0.0f, 0.0f, 10, 0, 0.2f, NULL);
        xAnimTableNewTransition(
            table, strptrz[i], "RB_TauntThroat", (xAnimTransitionConditionalCallback)VANM_dotran,
            (xAnimTransitionCallback)VANM_didtran, 0, 0, 0.0f, 0.0f, 10, 0, 0.2f, NULL);
        xAnimTableNewTransition(
            table, strptrz[i], "RB_TauntLaugh", (xAnimTransitionConditionalCallback)VANM_dotran,
            (xAnimTransitionCallback)VANM_didtran, 0, 0, 0.0f, 0.0f, 10, 0, 0.2f, NULL);
    }
    xAnimTableNewTransition(table, "RB_AttackThrow", "RB_AttackThrowWait",
                            (xAnimTransitionConditionalCallback)VANM_dodflt,
                            (xAnimTransitionCallback)VANM_diddflt, 0x10, 0, 0.0f, 0.0f, 5, 0, 0.2f,
                            NULL);
    xAnimTableNewTransition(table, "RB_AttackThrowWait", "RB_AttackCatch",
                            (xAnimTransitionConditionalCallback)VANM_dodflt,
                            (xAnimTransitionCallback)VANM_diddflt, 0x10, 0, 0.0f, 0.0f, 1, 0, 0.2f,
                            NULL);
    strptrz[0] = "RB_IdleWind";
    strptrz[1] = "RB_React";
    strptrz[2] = "RB_TauntFeel";
    strptrz[3] = "RB_TauntThroat";
    strptrz[4] = "RB_TauntLaugh";
    strptrz[5] = "RB_AttackPunch";
    strptrz[6] = "RB_LookBegin";
    strptrz[7] = "RB_LookLoop";
    strptrz[8] = "RB_LookEnd";
    strptrz[9] = "RB_Chase";
    strptrz[10] = "RB_Walk";
    strptrz[11] = "RB_Turn";
    for (i = 0; i < 12; i++)
    {
        xAnimTableNewTransition(
            table, strptrz[i], "RB_AttackThrow", (xAnimTransitionConditionalCallback)VANM_dotran,
            (xAnimTransitionCallback)VANM_didtran, 0, 0, 0.0f, 0.0f, 10, 0, 0.2f, NULL);
        xAnimTableNewTransition(
            table, strptrz[i], "RB_AttackCatch", (xAnimTransitionConditionalCallback)VANM_dotran,
            (xAnimTransitionCallback)VANM_didtran, 0, 0, 0.0f, 0.0f, 10, 0, 0.2f, NULL);
        xAnimTableNewTransition(table, strptrz[i], "RB_AttackThrowWait",
                                (xAnimTransitionConditionalCallback)VANM_dotran,
                                (xAnimTransitionCallback)VANM_didtran, 0, 0, 0.0f, 0.0f, 10, 0,
                                0.2f, NULL);
    }
    xAnimTableNewTransition(
        table, "RB_LookBegin", "RB_LookLoop", (xAnimTransitionConditionalCallback)VANM_dodflt,
        (xAnimTransitionCallback)VANM_diddflt, 0x10, 0, 0.0f, 0.0f, 1, 0, 0.2f, NULL);
    xAnimTableNewTransition(
        table, "RB_LookLoop", "RB_LookEnd", (xAnimTransitionConditionalCallback)VANM_dodflt,
        (xAnimTransitionCallback)VANM_diddflt, 0x10, 0, 0.0f, 0.0f, 1, 0, 0.2f, NULL);
    strptrz[0] = "RB_IdleWind";
    strptrz[1] = "RB_React";
    strptrz[2] = "RB_TauntFeel";
    strptrz[3] = "RB_TauntThroat";
    strptrz[4] = "RB_TauntLaugh";
    strptrz[5] = "RB_AttackPunch";
    strptrz[6] = "RB_AttackThrowWait";
    strptrz[7] = "RB_AttackThrow";
    strptrz[8] = "RB_AttackCatch";
    strptrz[9] = "RB_Chase";
    strptrz[10] = "RB_Turn";
    strptrz[11] = "RB_Walk";
    for (i = 0; i < 12; i++)
    {
        xAnimTableNewTransition(
            table, strptrz[i], "RB_LookBegin", (xAnimTransitionConditionalCallback)VANM_dotran,
            (xAnimTransitionCallback)VANM_didtran, 0, 0, 0.0f, 0.0f, 10, 0, 0.2f, NULL);
        xAnimTableNewTransition(
            table, strptrz[i], "RB_LookLoop", (xAnimTransitionConditionalCallback)VANM_dotran,
            (xAnimTransitionCallback)VANM_didtran, 0, 0, 0.0f, 0.0f, 10, 0, 0.2f, NULL);
        xAnimTableNewTransition(
            table, strptrz[i], "RB_LookEnd", (xAnimTransitionConditionalCallback)VANM_dotran,
            (xAnimTransitionCallback)VANM_didtran, 0, 0, 0.0f, 0.0f, 10, 0, 0.2f, NULL);
    }

    return table;
}

// Builds the Mastermind's animation table: the opening ("foreplay") states and the boss battle.
static xAnimTable* VANM_mktable_MM()
{
    xAnimTable* table;
    char* strptrz[32] = {NULL};
    S32 i;

    table = xAnimTableNew("Villain_MasterMind", NULL);
    xAnimTableNewState(table, "Idle", 0x110, 1, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "MM_Foreplay_Move", 0x10, 0x2000004, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "MM_Foreplay_Action", 0x20, 1, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "MM_Foreplay_Taunt", 0x20, 0x2000001, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "MM_Foreplay_JumpDown", 0x20, 0x2000001, 1.0f, NULL, NULL, 0.0f,
                       NULL);
    xAnimTableNewState(table, "MM_Battle_Idle", 0x10, 1, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "MM_Battle_Move", 0x10, 0x2000006, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "MM_Battle_Backstep", 0x10, 0x2000006, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "MM_Battle_Turn", 0x10, 0x2000001, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "MM_Battle_Attack_start", 0x20, 0x2000001, 1.0f, NULL, NULL, 0.0f,
                       NULL);
    xAnimTableNewState(table, "MM_Battle_Attack_ready_loop", 0x10, 0x2000001, 1.0f, NULL, NULL,
                       0.0f, NULL);
    xAnimTableNewState(table, "MM_Battle_Attack", 0x20, 1, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "MM_Battle_Shield_start", 0x20, 0x2000001, 1.0f, NULL, NULL, 0.0f,
                       NULL);
    xAnimTableNewState(table, "MM_Battle_Shield_loop", 0x10, 0x2000001, 1.0f, NULL, NULL, 0.0f,
                       NULL);
    xAnimTableNewState(table, "MM_Battle_Shield_end", 0x20, 0x2000001, 1.0f, NULL, NULL, 0.0f,
                       NULL);
    xAnimTableNewState(table, "MM_Battle_Stunned_start", 0x20, 0x2000001, 1.0f, NULL, NULL, 0.0f,
                       NULL);
    xAnimTableNewState(table, "MM_Battle_Stunned_loop", 0x10, 0x2000001, 1.0f, NULL, NULL, 0.0f,
                       NULL);
    xAnimTableNewState(table, "MM_Battle_Stunned_end", 0x20, 0x2000001, 1.0f, NULL, NULL, 0.0f,
                       NULL);
    xAnimTableNewState(table, "MM_Battle_Knocked_begin", 0x20, 6, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "MM_Battle_Knocked_loop", 0x10, 6, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "MM_Battle_Knocked_recover", 0x20, 1, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "MM_Battle_Taunt", 0x10, 0x2000001, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "MM_AngerReact", 0x10, 0x2000001, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "MM_Death_start", 0x10, 1, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "MM_Death", 0, 1, 1.0f, NULL, NULL, 0.0f, NULL);
    for (i = ZVIL_ANIM_MM_FP_MOVE; i < ZVIL_ANIM_GANG_ALTIDLE1; i++)
    {
        if (i < ZVIL_ANIM_MM_BA_IDLE)
        {
            xAnimTableNewTransition(
                table, g_animname[i], "Idle", (xAnimTransitionConditionalCallback)VANM_dodflt,
                (xAnimTransitionCallback)VANM_diddflt, 0x10, 0, 0.0f, 0.0f, 0, 0, 0.1f, NULL);
            xAnimTableNewTransition(
                table, g_animname[i], "Idle", (xAnimTransitionConditionalCallback)VANM_dotran,
                (xAnimTransitionCallback)VANM_didtran, 0, 0, 0.0f, 0.0f, 5, 0, 0.1f, NULL);
            xAnimTableNewTransition(
                table, "Idle", g_animname[i], (xAnimTransitionConditionalCallback)VANM_dotran,
                (xAnimTransitionCallback)VANM_didtran, 0, 0, 0.0f, 0.0f, 5, 0, 0.1f, NULL);
        }
        else if (i != ZVIL_ANIM_MM_BA_IDLE)
        {
            xAnimTableNewTransition(table, g_animname[i], "MM_Battle_Idle",
                                    (xAnimTransitionConditionalCallback)VANM_dodflt,
                                    (xAnimTransitionCallback)VANM_diddflt, 0x10, 0, 0.0f, 0.0f, 0,
                                    0, 0.1f, NULL);
            xAnimTableNewTransition(table, g_animname[i], "MM_Battle_Idle",
                                    (xAnimTransitionConditionalCallback)VANM_dotran,
                                    (xAnimTransitionCallback)VANM_didtran, 0, 0, 0.0f, 0.0f, 5, 0,
                                    0.1f, NULL);
            xAnimTableNewTransition(table, "MM_Battle_Idle", g_animname[i],
                                    (xAnimTransitionConditionalCallback)VANM_dotran,
                                    (xAnimTransitionCallback)VANM_didtran, 0, 0, 0.0f, 0.0f, 5, 0,
                                    0.1f, NULL);
        }
    }
    strptrz[0] = "MM_Foreplay_Move";
    strptrz[1] = "MM_Foreplay_Action";
    for (i = 0; i < 2; i++)
    {
        xAnimTableNewTransition(
            table, strptrz[i], "MM_Foreplay_Taunt", (xAnimTransitionConditionalCallback)VANM_dotran,
            (xAnimTransitionCallback)VANM_didtran, 0, 0, 0.0f, 0.0f, 10, 0, 0.1f, NULL);
    }
    strptrz[0] = "MM_Foreplay_Move";
    strptrz[1] = "MM_Foreplay_Taunt";
    for (i = 0; i < 2; i++)
    {
        xAnimTableNewTransition(table, strptrz[i], "MM_Foreplay_Action",
                                (xAnimTransitionConditionalCallback)VANM_dotran,
                                (xAnimTransitionCallback)VANM_didtran, 0, 0, 0.0f, 0.0f, 10, 0,
                                0.1f, NULL);
    }
    strptrz[0] = "MM_Foreplay_Action";
    strptrz[1] = "MM_Foreplay_Taunt";
    for (i = 0; i < 2; i++)
    {
        xAnimTableNewTransition(
            table, strptrz[i], "MM_Foreplay_Move", (xAnimTransitionConditionalCallback)VANM_dotran,
            (xAnimTransitionCallback)VANM_didtran, 0, 0, 0.0f, 0.0f, 10, 0, 0.1f, NULL);
    }
    xAnimTableNewTransition(table, "MM_Foreplay_JumpDown", "MM_Battle_Idle",
                            (xAnimTransitionConditionalCallback)VANM_dodflt,
                            (xAnimTransitionCallback)VANM_diddflt, 0x10, 0, 0.0f, 0.0f, 1, 0, 0.1f,
                            NULL);
    strptrz[0] = "MM_Foreplay_Action";
    strptrz[1] = "MM_Foreplay_Taunt";
    strptrz[2] = "MM_Foreplay_Move";
    for (i = 0; i < 3; i++)
    {
        xAnimTableNewTransition(table, strptrz[i], "MM_Foreplay_JumpDown",
                                (xAnimTransitionConditionalCallback)VANM_dotran,
                                (xAnimTransitionCallback)VANM_didtran, 0, 0, 0.0f, 0.0f, 10, 0,
                                0.1f, NULL);
    }
    strptrz[0] = "Idle";
    strptrz[1] = "MM_Foreplay_Move";
    strptrz[2] = "MM_Foreplay_Taunt";
    strptrz[3] = "MM_Foreplay_JumpDown";
    for (i = 0; i < 4; i++)
    {
        xAnimTableNewTransition(
            table, strptrz[i], "MM_Battle_Idle", (xAnimTransitionConditionalCallback)VANM_dotran,
            (xAnimTransitionCallback)VANM_didtran, 0, 0, 0.0f, 0.0f, 10, 0, 0.1f, NULL);
        xAnimTableNewTransition(
            table, strptrz[i], "MM_Battle_Move", (xAnimTransitionConditionalCallback)VANM_dotran,
            (xAnimTransitionCallback)VANM_didtran, 0, 0, 0.0f, 0.0f, 10, 0, 0.1f, NULL);
    }
    xAnimTableNewTransition(
        table, "MM_Battle_Idle", "Idle", (xAnimTransitionConditionalCallback)VANM_dodflt,
        (xAnimTransitionCallback)VANM_diddflt, 0x10, 0, 0.0f, 0.0f, 0, 0, 0.1f, NULL);
    strptrz[0] = "MM_Battle_Turn";
    strptrz[1] = "MM_Battle_Backstep";
    strptrz[2] = "MM_Battle_Knocked_begin";
    strptrz[3] = "MM_Battle_Knocked_loop";
    strptrz[4] = "MM_Battle_Knocked_recover";
    strptrz[5] = "MM_Battle_Shield_start";
    strptrz[6] = "MM_Battle_Shield_loop";
    strptrz[7] = "MM_Battle_Shield_end";
    strptrz[8] = "MM_Battle_Stunned_start";
    strptrz[9] = "MM_Battle_Stunned_loop";
    strptrz[10] = "MM_Battle_Stunned_end";
    strptrz[11] = "MM_Battle_Attack_start";
    strptrz[12] = "MM_Battle_Attack_ready_loop";
    strptrz[13] = "MM_Battle_Attack";
    for (i = 0; i < 14; i++)
    {
        xAnimTableNewTransition(
            table, strptrz[i], "MM_Battle_Move", (xAnimTransitionConditionalCallback)VANM_dotran,
            (xAnimTransitionCallback)VANM_didtran, 0, 0, 0.0f, 0.0f, 10, 0, 0.1f, NULL);
    }
    xAnimTableNewTransition(table, "MM_Battle_Move", "MM_Battle_Backstep",
                            (xAnimTransitionConditionalCallback)VANM_dotran,
                            (xAnimTransitionCallback)VANM_didtran, 0, 0, 0.0f, 0.0f, 10, 0, 0.1f,
                            NULL);
    xAnimTableNewTransition(table, "MM_Battle_Shield_start", "MM_Battle_Shield_loop",
                            (xAnimTransitionConditionalCallback)VANM_dodflt,
                            (xAnimTransitionCallback)VANM_diddflt, 0x10, 0, 0.0f, 0.0f, 0, 0, 0.1f,
                            NULL);
    // Slot 0 is set twice and slot 10 never, so "MM_Battle_Move" is not added here and slot
    // 10 keeps "MM_Battle_Stunned_end" from the list above.
    strptrz[0] = "MM_Battle_Move";
    strptrz[1] = "MM_Battle_Turn";
    strptrz[2] = "MM_Battle_Backstep";
    strptrz[3] = "MM_Foreplay_Taunt";
    strptrz[4] = "MM_AngerReact";
    strptrz[5] = "MM_Battle_Taunt";
    strptrz[6] = "MM_Battle_Stunned_start";
    strptrz[7] = "MM_Battle_Stunned_loop";
    strptrz[8] = "MM_Battle_Stunned_end";
    strptrz[9] = "MM_Battle_Attack_start";
    strptrz[0] = "MM_Battle_Attack_ready_loop";
    strptrz[11] = "MM_Battle_Attack";
    for (i = 0; i < 12; i++)
    {
        xAnimTableNewTransition(table, strptrz[i], "MM_Battle_Shield_start",
                                (xAnimTransitionConditionalCallback)VANM_dotran,
                                (xAnimTransitionCallback)VANM_didtran, 0, 0, 0.0f, 0.0f, 10, 0,
                                0.1f, NULL);
    }
    strptrz[0] = "MM_Battle_Move";
    strptrz[1] = "MM_Battle_Turn";
    strptrz[2] = "MM_Battle_Backstep";
    strptrz[3] = "MM_Battle_Knocked_begin";
    strptrz[4] = "MM_Battle_Knocked_loop";
    strptrz[5] = "MM_Battle_Knocked_recover";
    strptrz[6] = "MM_Battle_Shield_start";
    strptrz[7] = "MM_Battle_Shield_loop";
    strptrz[8] = "MM_Battle_Shield_end";
    strptrz[9] = "MM_Battle_Stunned_start";
    strptrz[10] = "MM_Battle_Stunned_loop";
    strptrz[11] = "MM_Battle_Stunned_end";
    strptrz[12] = "MM_Battle_Attack_start";
    strptrz[13] = "MM_Battle_Attack_ready_loop";
    strptrz[14] = "MM_Battle_Attack";
    for (i = 0; i < 15; i++)
    {
        xAnimTableNewTransition(
            table, strptrz[i], "MM_Battle_Taunt", (xAnimTransitionConditionalCallback)VANM_dotran,
            (xAnimTransitionCallback)VANM_didtran, 0, 0, 0.0f, 0.0f, 10, 0, 0.1f, NULL);
        xAnimTableNewTransition(
            table, strptrz[i], "MM_Foreplay_Taunt", (xAnimTransitionConditionalCallback)VANM_dotran,
            (xAnimTransitionCallback)VANM_didtran, 0, 0, 0.0f, 0.0f, 9, 0, 0.1f, NULL);
        xAnimTableNewTransition(
            table, strptrz[i], "MM_AngerReact", (xAnimTransitionConditionalCallback)VANM_dotran,
            (xAnimTransitionCallback)VANM_didtran, 0, 0, 0.0f, 0.0f, 9, 0, 0.1f, NULL);
    }
    xAnimTableNewTransition(table, "MM_Battle_Attack_start", "MM_Battle_Attack_ready_loop",
                            (xAnimTransitionConditionalCallback)VANM_dodflt,
                            (xAnimTransitionCallback)VANM_diddflt, 0x10, 0, 0.0f, 0.0f, 5, 0, 0.1f,
                            NULL);
    xAnimTableNewTransition(table, "MM_Battle_Attack", "MM_Battle_Idle",
                            (xAnimTransitionConditionalCallback)VANM_dodflt,
                            (xAnimTransitionCallback)VANM_diddflt, 0x10, 0, 0.0f, 0.0f, 5, 0, 0.1f,
                            NULL);
    xAnimTableNewTransition(table, "MM_Battle_Attack_ready_loop", "MM_Battle_Attack",
                            (xAnimTransitionConditionalCallback)VANM_dotran,
                            (xAnimTransitionCallback)VANM_didtran, 0, 0, 0.0f, 0.0f, 9, 0, 0.1f,
                            NULL);
    strptrz[0] = "MM_Battle_Move";
    strptrz[1] = "MM_Battle_Turn";
    strptrz[2] = "MM_Battle_Backstep";
    strptrz[3] = "MM_Foreplay_Taunt";
    strptrz[4] = "MM_AngerReact";
    strptrz[5] = "MM_Battle_Taunt";
    strptrz[6] = "MM_Battle_Shield_start";
    strptrz[7] = "MM_Battle_Shield_loop";
    strptrz[8] = "MM_Battle_Shield_end";
    strptrz[9] = "MM_Battle_Stunned_start";
    strptrz[10] = "MM_Battle_Stunned_loop";
    strptrz[11] = "MM_Battle_Stunned_end";
    for (i = 0; i < 12; i++)
    {
        xAnimTableNewTransition(table, strptrz[i], "MM_Battle_Attack_start",
                                (xAnimTransitionConditionalCallback)VANM_dotran,
                                (xAnimTransitionCallback)VANM_didtran, 0, 0, 0.0f, 0.0f, 10, 0,
                                0.1f, NULL);
    }
    xAnimTableNewTransition(table, "MM_Battle_Stunned_start", "MM_Battle_Stunned_loop",
                            (xAnimTransitionConditionalCallback)VANM_dodflt,
                            (xAnimTransitionCallback)VANM_diddflt, 0x10, 0, 0.0f, 0.0f, 1, 0, 0.1f,
                            NULL);
    strptrz[0] = "MM_Battle_Shield_start";
    strptrz[1] = "MM_Battle_Shield_loop";
    strptrz[2] = "MM_Battle_Shield_end";
    strptrz[3] = "MM_Battle_Attack_start";
    strptrz[4] = "MM_Battle_Attack_ready_loop";
    strptrz[5] = "MM_Battle_Attack";
    for (i = 0; i < 6; i++)
    {
        xAnimTableNewTransition(table, strptrz[i], "MM_Battle_Stunned_start",
                                (xAnimTransitionConditionalCallback)VANM_dotran,
                                (xAnimTransitionCallback)VANM_didtran, 0, 0, 0.0f, 0.0f, 10, 0,
                                0.1f, NULL);
    }
    xAnimTableNewTransition(table, "MM_Battle_Knocked_begin", "MM_Battle_Knocked_loop",
                            (xAnimTransitionConditionalCallback)VANM_dodflt,
                            (xAnimTransitionCallback)VANM_diddflt, 0x10, 0, 0.0f, 0.0f, 1, 0, 0.1f,
                            NULL);
    strptrz[0] = "MM_Battle_Stunned_start";
    strptrz[1] = "MM_Battle_Stunned_loop";
    strptrz[2] = "MM_Battle_Stunned_end";
    for (i = 0; i < 3; i++)
    {
        xAnimTableNewTransition(table, strptrz[i], "MM_Battle_Knocked_begin",
                                (xAnimTransitionConditionalCallback)VANM_dotran,
                                (xAnimTransitionCallback)VANM_didtran, 0, 0, 0.0f, 0.0f, 10, 0,
                                0.1f, NULL);
        xAnimTableNewTransition(table, strptrz[i], "MM_Battle_Knocked_loop",
                                (xAnimTransitionConditionalCallback)VANM_dotran,
                                (xAnimTransitionCallback)VANM_didtran, 0, 0, 0.0f, 0.0f, 10, 0,
                                0.1f, NULL);
    }
    xAnimTableNewTransition(table, "MM_Battle_Knocked_loop", "MM_Battle_Knocked_recover",
                            (xAnimTransitionConditionalCallback)VANM_dotran,
                            (xAnimTransitionCallback)VANM_didtran, 0, 0, 0.0f, 0.0f, 10, 0, 0.1f,
                            NULL);
    strptrz[0] = "MM_Battle_Knocked_begin";
    strptrz[1] = "MM_Battle_Knocked_loop";
    strptrz[2] = "MM_Battle_Knocked_recover";
    for (i = 0; i < 3; i++)
    {
        xAnimTableNewTransition(
            table, strptrz[i], "MM_Death_start", (xAnimTransitionConditionalCallback)VANM_dotran,
            (xAnimTransitionCallback)VANM_didtran, 0, 0, 0.0f, 0.0f, 10, 0, 0.1f, NULL);
    }
    xAnimTableNewTransition(
        table, "MM_Death_start", "MM_Death", (xAnimTransitionConditionalCallback)VANM_dotran,
        (xAnimTransitionCallback)VANM_didtran, 0, 0, 0.0f, 0.0f, 10, 0, 0.1f, NULL);

    return table;
}

// Builds the Sham boss's animation table: idle, move and taunt.
static xAnimTable* VANM_mktable_SHAM()
{
    xAnimTable* table;
    S32 i;

    table = xAnimTableNew("Villain_BossSham", NULL);
    xAnimTableNewState(table, "Idle", 0x110, 1, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "SHAM_Move", 0x10, 0x2000004, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "SHAM_Taunt", 0x10, 1, 1.0f, NULL, NULL, 0.0f, NULL);
    for (i = ZVIL_ANIM_SHAM_MOVE; i < ZVIL_ANIM_CRIT_MOVE; i++)
    {
        xAnimTableNewTransition(
            table, g_animname[i], "Idle", (xAnimTransitionConditionalCallback)VANM_dodflt,
            (xAnimTransitionCallback)VANM_diddflt, 0x10, 0, 0.0f, 0.0f, 0, 0, 0.1f, NULL);
        xAnimTableNewTransition(
            table, g_animname[i], "Idle", (xAnimTransitionConditionalCallback)VANM_dotran,
            (xAnimTransitionCallback)VANM_didtran, 0, 0, 0.0f, 0.0f, 5, 0, 0.1f, NULL);
        xAnimTableNewTransition(
            table, "Idle", g_animname[i], (xAnimTransitionConditionalCallback)VANM_dotran,
            (xAnimTransitionCallback)VANM_didtran, 0, 0, 0.0f, 0.0f, 10, 0, 0.1f, NULL);
    }
    xAnimTableNewTransition(
        table, "SHAM_Taunt", "SHAM_Move", (xAnimTransitionConditionalCallback)VANM_dodflt,
        (xAnimTransitionCallback)VANM_diddflt, 0x10, 0, 0.0f, 0.0f, 5, 0, 0.1f, NULL);
    xAnimTableNewTransition(
        table, "SHAM_Move", "SHAM_Taunt", (xAnimTransitionConditionalCallback)VANM_dotran,
        (xAnimTransitionCallback)VANM_didtran, 0, 0, 0.0f, 0.0f, 10, 0, 0.1f, NULL);
    xAnimTableNewTransition(
        table, "SHAM_Taunt", "SHAM_Move", (xAnimTransitionConditionalCallback)VANM_dotran,
        (xAnimTransitionCallback)VANM_didtran, 0, 0, 0.0f, 0.0f, 10, 0, 0.1f, NULL);

    return table;
}

// Builds the shark's animation table: idle and bored states with their fidgets.
static xAnimTable* VANM_mktable_SHARK()
{
    xAnimTable* table;
    xAnimTransition* tmp_tran;
    S32 i;

    table = xAnimTableNew("Villain_Shark", NULL);
    xAnimTableNewState(table, "Idle", 0x10, 1, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "Bored", 0x10, 1, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "Idle_Fidget_1", 0x20, 1, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "Idle_Fidget_2", 0x20, 1, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "Bored_Fidget_1", 0x20, 1, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "Bored_Fidget_2", 0x20, 1, 1.0f, NULL, NULL, 0.0f, NULL);
    for (i = ZVIL_ANIM_IDLE; i < ZVIL_ANIM_MOVE; i++)
    {
        xAnimTableNewTransition(
            table, g_animname[i], "Idle", (xAnimTransitionConditionalCallback)VANM_dodflt,
            (xAnimTransitionCallback)VANM_diddflt, 0x10, 0, 0.0f, 0.0f, 0, 0, 0.0f, NULL);
        xAnimTableNewTransition(
            table, g_animname[i], "Idle", (xAnimTransitionConditionalCallback)VANM_dotran,
            (xAnimTransitionCallback)VANM_didtran, 0, 0, 0.0f, 0.0f, 5, 0, 0.0f, NULL);
        xAnimTableNewTransition(
            table, "Idle", g_animname[i], (xAnimTransitionConditionalCallback)VANM_dotran,
            (xAnimTransitionCallback)VANM_didtran, 0, 0, 0.0f, 0.0f, 10, 0, 0.0f, NULL);
    }
    tmp_tran = xAnimTableNewTransition(table, "Idle_Fidget_1 Idle_Fidget_2", "Idle",
                                       (xAnimTransitionConditionalCallback)VANM_dodflt,
                                       (xAnimTransitionCallback)VANM_diddflt, 0x10, 0, 0.0f, 0.0f,
                                       2, 0, 0.0f, NULL);
    tmp_tran->Flags |= 0x20;
    tmp_tran = xAnimTableNewTransition(table, "Idle_Fidget_1 Idle_Fidget_2", "Bored",
                                       (xAnimTransitionConditionalCallback)VANM_dodflt,
                                       (xAnimTransitionCallback)VANM_diddflt, 0x10, 0, 0.0f, 0.0f,
                                       1, 0, 0.0f, NULL);
    tmp_tran->Flags |= 0x20;
    tmp_tran = xAnimTableNewTransition(table, "Bored_Fidget_1 Bored_Fidget_2", "Bored",
                                       (xAnimTransitionConditionalCallback)VANM_dodflt,
                                       (xAnimTransitionCallback)VANM_diddflt, 0x10, 0, 0.0f, 0.0f,
                                       2, 0, 0.0f, NULL);
    tmp_tran->Flags |= 0x20;
    tmp_tran = xAnimTableNewTransition(table, "Bored_Fidget_1 Bored_Fidget_2", "Idle",
                                       (xAnimTransitionConditionalCallback)VANM_dodflt,
                                       (xAnimTransitionCallback)VANM_diddflt, 0x10, 0, 0.0f, 0.0f,
                                       1, 0, 0.0f, NULL);
    tmp_tran->Flags |= 0x20;
    tmp_tran = xAnimTableNewTransition(
        table, "Idle", "Idle_Fidget_1", (xAnimTransitionConditionalCallback)VANM_dotran,
        (xAnimTransitionCallback)VANM_didtran, 0, 0, 0.0f, 0.0f, 8, 0, 0.0f, NULL);
    tmp_tran->Flags |= 0x20;
    tmp_tran = xAnimTableNewTransition(
        table, "Idle", "Idle_Fidget_2", (xAnimTransitionConditionalCallback)VANM_dotran,
        (xAnimTransitionCallback)VANM_didtran, 0, 0, 0.0f, 0.0f, 8, 0, 0.0f, NULL);
    tmp_tran->Flags |= 0x20;
    tmp_tran = xAnimTableNewTransition(
        table, "Idle", "Bored_Fidget_1", (xAnimTransitionConditionalCallback)VANM_dotran,
        (xAnimTransitionCallback)VANM_didtran, 0, 0, 0.0f, 0.0f, 7, 0, 0.0f, NULL);
    tmp_tran->Flags |= 0x20;
    tmp_tran = xAnimTableNewTransition(
        table, "Idle", "Bored_Fidget_2", (xAnimTransitionConditionalCallback)VANM_dotran,
        (xAnimTransitionCallback)VANM_didtran, 0, 0, 0.0f, 0.0f, 7, 0, 0.0f, NULL);
    tmp_tran->Flags |= 0x20;
    tmp_tran = xAnimTableNewTransition(
        table, "Bored", "Bored_Fidget_1", (xAnimTransitionConditionalCallback)VANM_dotran,
        (xAnimTransitionCallback)VANM_didtran, 0, 0, 0.0f, 0.0f, 8, 0, 0.0f, NULL);
    tmp_tran->Flags |= 0x20;
    tmp_tran = xAnimTableNewTransition(
        table, "Bored", "Bored_Fidget_2", (xAnimTransitionConditionalCallback)VANM_dotran,
        (xAnimTransitionCallback)VANM_didtran, 0, 0, 0.0f, 0.0f, 8, 0, 0.0f, NULL);
    tmp_tran->Flags |= 0x20;
    tmp_tran = xAnimTableNewTransition(
        table, "Bored", "Idle_Fidget_1", (xAnimTransitionConditionalCallback)VANM_dotran,
        (xAnimTransitionCallback)VANM_didtran, 0, 0, 0.0f, 0.0f, 7, 0, 0.0f, NULL);
    tmp_tran->Flags |= 0x20;
    tmp_tran = xAnimTableNewTransition(
        table, "Bored", "Idle_Fidget_2", (xAnimTransitionConditionalCallback)VANM_dotran,
        (xAnimTransitionCallback)VANM_didtran, 0, 0, 0.0f, 0.0f, 7, 0, 0.0f, NULL);
    tmp_tran->Flags |= 0x20;

    return table;
}

// Builds the animation table for simple villains: idle and one action.
static xAnimTable* VANM_mktable_SIMP()
{
    xAnimTable* table;
    S32 i;

    table = xAnimTableNew("Villain_Simple", NULL);
    xAnimTableNewState(table, "Idle", 0x110, 1, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "SIMP_Action", 0x20, 1, 1.0f, NULL, NULL, 0.0f, NULL);
    for (i = ZVIL_ANIM_SIMP_ACTION; i < ZVIL_ANIM_NOMORE; i++)
    {
        xAnimTableNewTransition(
            table, g_animname[i], "Idle", (xAnimTransitionConditionalCallback)VANM_dodflt,
            (xAnimTransitionCallback)VANM_diddflt, 0x10, 0, 0.0f, 0.0f, 0, 0, 0.1f, NULL);
        xAnimTableNewTransition(
            table, g_animname[i], "Idle", (xAnimTransitionConditionalCallback)VANM_dotran,
            (xAnimTransitionCallback)VANM_didtran, 0, 0, 0.0f, 0.0f, 5, 0, 0.1f, NULL);
        xAnimTableNewTransition(
            table, "Idle", g_animname[i], (xAnimTransitionConditionalCallback)VANM_dotran,
            (xAnimTransitionCallback)VANM_didtran, 0, 0, 0.0f, 0.0f, 10, 0, 0.1f, NULL);
    }

    return table;
}

// Builds the gang's animation table: idles, fidgets, movement, actions and talking. Every state
// returns to Idle through one shared default transition, collected in deftran and intranz.
static xAnimTable* VANM_mktable_GANG()
{
    xAnimTable* table;
    char* strptrz[32] = {NULL};
    xAnimTransition* intranz[ZVIL_ANIM_NOMORE] = {NULL};
    xAnimTransition* deftran[ZVIL_ANIM_NOMORE] = {NULL};
    S32 i;
    S32 j;

    table = xAnimTableNew("Villain_Gang", NULL);
    xAnimTableNewState(table, "Idle", 0x110, 1, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "GANG_Idle_1", 0x110, 1, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "GANG_Idle_2", 0x110, 1, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "GANG_Idle_3", 0x110, 1, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "GANG_IdleFidget", 0x10, 1, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "GANG_IdleFidget_1", 0x10, 1, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "GANG_IdleFidget_2", 0x10, 1, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "GANG_IdleFidget_3", 0x10, 1, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "GANG_Turn", 0x10, 0x2000000, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "GANG_Walk", 0x10, 0x2000004, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "GANG_Run", 0x10, 0x2000006, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "GANG_RunChased", 0x10, 0x2000006, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "GANG_Action", 0x20, 1, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "GANG_BossAction", 0x20, 1, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "GANG_Talk", 0x10, 0x2000000, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "GANG_Talk_1", 0x10, 0x2000000, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "GANG_Talk_2", 0x10, 0x2000000, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "GANG_Talk_3", 0x10, 0x2000000, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "GANG_TalkAndTurn", 0x10, 0x2000000, 1.0f, NULL, NULL, 0.0f, NULL);
    for (i = ZVIL_ANIM_GANG_ALTIDLE1; i < ZVIL_ANIM_SHAM_MOVE; i++)
    {
        if (deftran[ZVIL_ANIM_IDLE] == NULL)
        {
            deftran[ZVIL_ANIM_IDLE] = xAnimTableNewTransition(
                table, g_animname[i], "Idle", (xAnimTransitionConditionalCallback)VANM_dodflt,
                (xAnimTransitionCallback)VANM_diddflt, 0x10, 0, 0.0f, 0.0f, 0, 0, 0.5f, NULL);
        }
        else
        {
            xAnimTableAddTransition(table, deftran[ZVIL_ANIM_IDLE], g_animname[i]);
        }
        if (intranz[ZVIL_ANIM_IDLE] == NULL)
        {
            intranz[ZVIL_ANIM_IDLE] = xAnimTableNewTransition(
                table, g_animname[i], "Idle", (xAnimTransitionConditionalCallback)VANM_dotran,
                (xAnimTransitionCallback)VANM_didtran, 0, 0, 0.0f, 0.0f, 5, 0, 0.5f, NULL);
        }
        else
        {
            xAnimTableAddTransition(table, intranz[ZVIL_ANIM_IDLE], g_animname[i]);
        }
        intranz[i] = xAnimTableNewTransition(
            table, "Idle", g_animname[i], (xAnimTransitionConditionalCallback)VANM_dotran,
            (xAnimTransitionCallback)VANM_didtran, 0, 0, 0.0f, 0.0f, 10, 0, 0.5f, NULL);
    }
    xAnimTableNewTransition(
        table, "GANG_IdleFidget_1", "GANG_Idle_1", (xAnimTransitionConditionalCallback)VANM_dodflt,
        (xAnimTransitionCallback)VANM_diddflt, 0x10, 0, 0.0f, 0.0f, 5, 0, 0.5f, NULL);
    xAnimTableNewTransition(
        table, "GANG_IdleFidget_2", "GANG_Idle_2", (xAnimTransitionConditionalCallback)VANM_dodflt,
        (xAnimTransitionCallback)VANM_diddflt, 0x10, 0, 0.0f, 0.0f, 5, 0, 0.5f, NULL);
    xAnimTableNewTransition(
        table, "GANG_IdleFidget_3", "GANG_Idle_3", (xAnimTransitionConditionalCallback)VANM_dodflt,
        (xAnimTransitionCallback)VANM_diddflt, 0x10, 0, 0.0f, 0.0f, 5, 0, 0.5f, NULL);
    xAnimTableAddTransition(table, intranz[ZVIL_ANIM_GANG_ALTIDLE1], "GANG_Idle_2 GANG_Idle_3");
    xAnimTableAddTransition(table, intranz[ZVIL_ANIM_GANG_ALTIDLE2], "GANG_Idle_1 GANG_Idle_3");
    xAnimTableAddTransition(table, intranz[ZVIL_ANIM_GANG_ALTIDLE3], "GANG_Idle_1 GANG_Idle_2");
    strptrz[0] = "GANG_Idle_1";
    strptrz[1] = "GANG_Idle_2";
    strptrz[2] = "GANG_Idle_3";
    for (i = 0; i < 3; i++)
    {
        xAnimTableAddTransition(table, intranz[ZVIL_ANIM_GANG_FIDGET], strptrz[i]);
        xAnimTableAddTransition(table, intranz[ZVIL_ANIM_GANG_FIDGET1], strptrz[i]);
        xAnimTableAddTransition(table, intranz[ZVIL_ANIM_GANG_FIDGET2], strptrz[i]);
        xAnimTableAddTransition(table, intranz[ZVIL_ANIM_GANG_FIDGET3], strptrz[i]);
    }
    strptrz[0] = "GANG_IdleFidget";
    strptrz[1] = "GANG_IdleFidget_1";
    strptrz[2] = "GANG_IdleFidget_2";
    strptrz[3] = "GANG_IdleFidget_3";
    strptrz[4] = "GANG_Turn";
    strptrz[5] = "GANG_Walk";
    strptrz[6] = "GANG_Run";
    strptrz[7] = "GANG_RunChased";
    strptrz[8] = "GANG_Talk";
    strptrz[9] = "GANG_Talk_1";
    strptrz[10] = "GANG_Talk_2";
    strptrz[11] = "GANG_Talk_3";
    strptrz[12] = "GANG_TalkAndTurn";
    for (i = 0; i < 13; i++)
    {
        xAnimTableAddTransition(table, intranz[ZVIL_ANIM_GANG_ALTIDLE1], strptrz[i]);
        xAnimTableAddTransition(table, intranz[ZVIL_ANIM_GANG_ALTIDLE2], strptrz[i]);
        xAnimTableAddTransition(table, intranz[ZVIL_ANIM_GANG_ALTIDLE3], strptrz[i]);
    }
    strptrz[0] = "GANG_Idle_1";
    strptrz[1] = "GANG_Idle_2";
    strptrz[2] = "GANG_Idle_3";
    strptrz[3] = "GANG_IdleFidget";
    strptrz[4] = "GANG_IdleFidget_1";
    strptrz[5] = "GANG_IdleFidget_2";
    strptrz[6] = "GANG_IdleFidget_3";
    strptrz[7] = "GANG_Turn";
    strptrz[8] = "GANG_Walk";
    strptrz[9] = "GANG_Run";
    strptrz[10] = "GANG_RunChased";
    strptrz[11] = "GANG_Talk";
    strptrz[12] = "GANG_Talk_1";
    strptrz[13] = "GANG_Talk_2";
    strptrz[14] = "GANG_Talk_3";
    strptrz[15] = "GANG_TalkAndTurn";
    for (i = 0; i < 16; i++)
    {
        xAnimTableAddTransition(table, intranz[ZVIL_ANIM_GANG_ACTION], strptrz[i]);
        xAnimTableAddTransition(table, intranz[ZVIL_ANIM_GANG_BOSSACTION], strptrz[i]);
    }
    xAnimTableAddTransition(table, intranz[ZVIL_ANIM_GANG_WALK], "GANG_Run GANG_RunChased");
    xAnimTableAddTransition(table, intranz[ZVIL_ANIM_GANG_RUN], "GANG_Walk GANG_RunChased");
    xAnimTableAddTransition(table, intranz[ZVIL_ANIM_GANG_RUNCHASED], "GANG_Walk GANG_Run");
    strptrz[0] = "GANG_Idle_1";
    strptrz[1] = "GANG_Idle_2";
    strptrz[2] = "GANG_Idle_3";
    strptrz[3] = "GANG_IdleFidget";
    strptrz[4] = "GANG_IdleFidget_1";
    strptrz[5] = "GANG_IdleFidget_2";
    strptrz[6] = "GANG_IdleFidget_3";
    strptrz[7] = "GANG_Action";
    strptrz[8] = "GANG_BossAction";
    strptrz[9] = "GANG_Talk";
    strptrz[10] = "GANG_Talk_1";
    strptrz[11] = "GANG_Talk_2";
    strptrz[12] = "GANG_Talk_3";
    strptrz[13] = "GANG_TalkAndTurn";
    for (i = 0; i < 14; i++)
    {
        xAnimTableAddTransition(table, intranz[ZVIL_ANIM_GANG_TURN], strptrz[i]);
        xAnimTableAddTransition(table, intranz[ZVIL_ANIM_GANG_WALK], strptrz[i]);
        xAnimTableAddTransition(table, intranz[ZVIL_ANIM_GANG_RUN], strptrz[i]);
        xAnimTableAddTransition(table, intranz[ZVIL_ANIM_GANG_RUNCHASED], strptrz[i]);
    }
    strptrz[0] = "GANG_Talk";
    strptrz[1] = "GANG_Talk_1";
    strptrz[2] = "GANG_Talk_2";
    strptrz[3] = "GANG_Talk_3";
    strptrz[4] = "GANG_TalkAndTurn";
    for (i = 0; i < 5; i++)
    {
        for (j = 0; j < 5; j++)
        {
            if (i != j)
            {
                xAnimTableNewTransition(
                    table, strptrz[i], strptrz[j], (xAnimTransitionConditionalCallback)VANM_dotran,
                    (xAnimTransitionCallback)VANM_didtran, 0, 0, 0.0f, 0.0f, 10, 0, 0.5f, NULL);
            }
        }
    }
    // Slot 0 keeps "GANG_Talk" from the list above, and slot 2 is set twice, so
    // "GANG_IdleFidget_1" is never added here.
    strptrz[1] = "GANG_IdleFidget";
    strptrz[2] = "GANG_IdleFidget_1";
    strptrz[2] = "GANG_IdleFidget_2";
    strptrz[3] = "GANG_IdleFidget_3";
    strptrz[4] = "GANG_Turn";
    strptrz[5] = "GANG_Walk";
    strptrz[6] = "GANG_Run";
    strptrz[7] = "GANG_RunChased";
    strptrz[8] = "GANG_Idle_1";
    strptrz[9] = "GANG_Idle_2";
    strptrz[10] = "GANG_Idle_3";
    for (i = 0; i < 11; i++)
    {
        xAnimTableAddTransition(table, intranz[ZVIL_ANIM_GANG_TALK], strptrz[i]);
        xAnimTableAddTransition(table, intranz[ZVIL_ANIM_GANG_TALK1], strptrz[i]);
        xAnimTableAddTransition(table, intranz[ZVIL_ANIM_GANG_TALK2], strptrz[i]);
        xAnimTableAddTransition(table, intranz[ZVIL_ANIM_GANG_TALK3], strptrz[i]);
        xAnimTableAddTransition(table, intranz[ZVIL_ANIM_GANG_TALKANDTURN], strptrz[i]);
    }

    return table;
}

// Builds the standard villain animation table, shared by most monsters: idles, movement,
// senses (hear, look, investigate), attacks, taunts, the gum and soap reactions, daze and death.
static xAnimTable* VANM_mktable_STD()
{
    xAnimTable* table;
    char* strptrz[32] = {NULL};
    xAnimTransition* intranz[ZVIL_ANIM_NOMORE] = {NULL};
    xAnimTransition* deftran[ZVIL_ANIM_NOMORE] = {NULL};
    S32 i;

    table = xAnimTableNew("Villain_Standard", NULL);
    xAnimTableNewState(table, "Idle", 0x110, 1, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "Bored", 0x110, 1, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "Idle_Fidget_1", 0x20, 1, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "Idle_Fidget_2", 0x20, 1, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "Bored_Fidget_1", 0x20, 1, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "Bored_Fidget_2", 0x20, 1, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "Fashion_Pose", 0x10, 1, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "Move", 0x110, 0x2000004, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "Turn", 0x10, 0x2000001, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "Chase", 0x110, 0x2000006, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "Action_OnceNoMove", 0x20, 1, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "Action_LoopAndMove", 0x110, 0x2000004, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "Surprize", 0x20, 0x2000001, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "Taunt", 0x10, 1, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "Taunt_1", 0x10, 1, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "Taunt_2", 0x10, 1, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "Hear_Start", 0x20, 1, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "Hear_Loop", 0x10, 0x2000001, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "Look_Start", 0x20, 1, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "Look_Loop", 0x10, 1, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "Investigate_Start", 0x20, 1, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "Investigate", 0x10, 1, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "AttackIdle", 0x10, 0x2000001, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "Attack", 0x20, 1, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "Attack_1", 0x20, 1, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "Attack_2", 0x20, 1, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "Combo", 0x20, 1, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "Combo_1", 0x20, 1, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "Combo_2", 0x20, 1, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "Lob_Attack", 0x20, 1, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "Lob_Combo", 0x20, 1, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "Extend_Attack_Start", 0x20, 1, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "Extend_Attack_Loop", 0x10, 1, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "Extend_Combo", 0x20, 1, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "Hit_By_Gum", 0x20, 1, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "Gummed", 0x10, 1, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "Gummed_Resist", 0x20, 1, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "Gummed_BreakFree", 0x20, 1, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "Hit_When_Gummed", 0x20, 1, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "Bounced_When_Gummed", 0x20, 1, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "Hit_By_Soap", 0x20, 1, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "Soaped", 0x10, 0x4000001, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "Soaped_Resist", 0x20, 1, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "Soaped_BreakFree", 0x20, 1, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "Hit_When_Soaped", 0x20, 1, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "Bounced_When_Soaped", 0x20, 1, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "Dazed_Begin", 0x20, 1, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "Dazed", 0x10, 1, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "Dazed_Recovery", 0x20, 1, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "Death", 0, 1, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "Death_From_Above", 0, 1, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "Death_From_Dazed", 0, 1, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "Death_Idle", 0x10, 1, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "Jump_Begin", 0x20, 0x200000a, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "Jump_Loop", 0x10, 0x200000a, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "Jump_Land", 0x20, 0x2000001, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "Fall_Loop", 0x20, 0x200000a, 1.0f, NULL, NULL, 0.0f, NULL);
    xAnimTableNewState(table, "Fall_Splat", 0x20, 0x2000001, 1.0f, NULL, NULL, 0.0f, NULL);
    for (i = ZVIL_ANIM_FIDGET_IDLE1; i < ZVIL_ANIM_BK_SCARYIDLE; i++)
    {
        if (deftran[ZVIL_ANIM_IDLE] == NULL)
        {
            deftran[ZVIL_ANIM_IDLE] = xAnimTableNewTransition(
                table, g_animname[i], "Idle", (xAnimTransitionConditionalCallback)VANM_dodflt,
                (xAnimTransitionCallback)VANM_diddflt, 0x10, 0, 0.0f, 0.0f, 0, 0, 0.1f, NULL);
        }
        else
        {
            xAnimTableAddTransition(table, deftran[ZVIL_ANIM_IDLE], g_animname[i]);
        }
        if (intranz[ZVIL_ANIM_IDLE] == NULL)
        {
            intranz[ZVIL_ANIM_IDLE] = xAnimTableNewTransition(
                table, g_animname[i], "Idle", (xAnimTransitionConditionalCallback)VANM_dotran,
                (xAnimTransitionCallback)VANM_didtran, 0, 0, 0.0f, 0.0f, 5, 0, 0.1f, NULL);
        }
        else
        {
            xAnimTableAddTransition(table, intranz[ZVIL_ANIM_IDLE], g_animname[i]);
        }
        intranz[i] = xAnimTableNewTransition(
            table, "Idle", g_animname[i], (xAnimTransitionConditionalCallback)VANM_dotran,
            (xAnimTransitionCallback)VANM_didtran, 0, 0, 0.0f, 0.0f, 10, 0, 0.1f, NULL);
    }
    xAnimTableAddTransition(table, deftran[ZVIL_ANIM_IDLE], "Idle_Fidget_1 Idle_Fidget_2");
    deftran[ZVIL_ANIM_BORED] = xAnimTableNewTransition(
        table, "Bored_Fidget_1 Bored_Fidget_2", "Bored",
        (xAnimTransitionConditionalCallback)VANM_dodflt, (xAnimTransitionCallback)VANM_diddflt,
        0x10, 0, 0.0f, 0.0f, 15, 0, 0.1f, NULL);
    strptrz[0] = "Idle";
    strptrz[1] = "Bored";
    for (i = 0; i < 2; i++)
    {
        xAnimTableAddTransition(table, intranz[ZVIL_ANIM_FIDGET_IDLE1], strptrz[i]);
        xAnimTableAddTransition(table, intranz[ZVIL_ANIM_FIDGET_IDLE2], strptrz[i]);
        xAnimTableAddTransition(table, intranz[ZVIL_ANIM_FIDGET_BORED1], strptrz[i]);
        xAnimTableAddTransition(table, intranz[ZVIL_ANIM_FIDGET_BORED2], strptrz[i]);
    }
    strptrz[0] = "Turn";
    strptrz[1] = "Surprize";
    strptrz[2] = "Investigate_Start";
    strptrz[3] = "Investigate";
    strptrz[4] = "Hear_Start";
    strptrz[5] = "Hear_Loop";
    strptrz[6] = "Look_Start";
    strptrz[7] = "Look_Loop";
    strptrz[8] = "Gummed";
    strptrz[9] = "Soaped";
    strptrz[10] = "Dazed";
    strptrz[11] = "Dazed_Recovery";
    strptrz[12] = "Attack";
    strptrz[13] = "Attack_1";
    strptrz[14] = "Attack_2";
    strptrz[15] = "Lob_Attack";
    strptrz[16] = "Extend_Attack_Loop";
    strptrz[17] = "Combo";
    strptrz[18] = "Combo_1";
    strptrz[19] = "Combo_2";
    strptrz[20] = "Taunt";
    strptrz[21] = "Taunt_1";
    strptrz[22] = "Taunt_2";
    strptrz[23] = "Bored";
    for (i = 0; i < 24; i++)
    {
        xAnimTableAddTransition(table, intranz[ZVIL_ANIM_MOVE], strptrz[i]);
        xAnimTableAddTransition(table, intranz[ZVIL_ANIM_CHASE], strptrz[i]);
    }
    strptrz[24] = "Move";
    strptrz[25] = "Chase";
    for (i = 0; i < 26; i++)
    {
        xAnimTableAddTransition(table, intranz[ZVIL_ANIM_ACTION_NOMOVE], strptrz[i]);
        xAnimTableNewTransition(
            table, "Action_OnceNoMove", strptrz[i], (xAnimTransitionConditionalCallback)VANM_dotran,
            (xAnimTransitionCallback)VANM_didtran, 0, 0, 0.0f, 0.0f, 10, 0, 0.1f, NULL);
        xAnimTableAddTransition(table, intranz[ZVIL_ANIM_ACTION_MOVING], strptrz[i]);
        xAnimTableNewTransition(table, "Action_LoopAndMove", strptrz[i],
                                (xAnimTransitionConditionalCallback)VANM_dotran,
                                (xAnimTransitionCallback)VANM_didtran, 0, 0, 0.0f, 0.0f, 10, 0,
                                0.1f, NULL);
    }
    xAnimTableAddTransition(table, intranz[ZVIL_ANIM_CHASE], "Move");
    xAnimTableAddTransition(table, intranz[ZVIL_ANIM_MOVE], "Chase");
    strptrz[0] = "Move";
    strptrz[1] = "Surprize";
    strptrz[2] = "Investigate_Start";
    strptrz[3] = "Investigate";
    strptrz[4] = "Hear_Start";
    strptrz[5] = "Hear_Loop";
    strptrz[6] = "Look_Start";
    strptrz[7] = "Look_Loop";
    strptrz[8] = "Gummed";
    strptrz[9] = "Soaped";
    strptrz[10] = "Dazed";
    strptrz[11] = "Dazed_Recovery";
    strptrz[12] = "Attack";
    strptrz[13] = "Attack_1";
    strptrz[14] = "Attack_2";
    strptrz[15] = "Lob_Attack";
    strptrz[16] = "Extend_Attack_Loop";
    strptrz[17] = "Combo";
    strptrz[18] = "Combo_1";
    strptrz[19] = "Combo_2";
    strptrz[20] = "Taunt";
    strptrz[21] = "Taunt_1";
    strptrz[22] = "Taunt_2";
    strptrz[23] = "Bored";
    strptrz[24] = "Move";
    strptrz[25] = "Chase";
    for (i = 0; i < 26; i++)
    {
        if (deftran[ZVIL_ANIM_TURN] == NULL)
        {
            deftran[ZVIL_ANIM_TURN] = xAnimTableNewTransition(
                table, strptrz[i], "Turn", (xAnimTransitionConditionalCallback)VANM_dodflt,
                (xAnimTransitionCallback)VANM_diddflt, 0x10, 0, 0.0f, 0.0f, 0, 0, 0.1f, NULL);
        }
        else
        {
            xAnimTableAddTransition(table, deftran[ZVIL_ANIM_TURN], strptrz[i]);
        }
        xAnimTableAddTransition(table, intranz[ZVIL_ANIM_TURN], strptrz[i]);
    }
    strptrz[0] = "Attack";
    strptrz[1] = "Attack_1";
    strptrz[2] = "Attack_2";
    strptrz[3] = "Combo";
    strptrz[4] = "Combo_1";
    strptrz[5] = "Combo_2";
    strptrz[6] = "Lob_Attack";
    strptrz[7] = "Extend_Attack_Loop";
    for (i = 0; i < 8; i++)
    {
        if (deftran[ZVIL_ANIM_ATTACKIDLE] == NULL)
        {
            deftran[ZVIL_ANIM_ATTACKIDLE] = xAnimTableNewTransition(
                table, strptrz[i], "AttackIdle", (xAnimTransitionConditionalCallback)VANM_dodflt,
                (xAnimTransitionCallback)VANM_diddflt, 0x10, 0, 0.0f, 0.0f, 0, 0, 0.1f, NULL);
        }
        else
        {
            xAnimTableAddTransition(table, deftran[ZVIL_ANIM_ATTACKIDLE], strptrz[i]);
        }
    }
    deftran[ZVIL_ANIM_EXT_ATTACK_LOOP] = xAnimTableNewTransition(
        table, "Extend_Attack_Start", "Extend_Attack_Loop",
        (xAnimTransitionConditionalCallback)VANM_dodflt, (xAnimTransitionCallback)VANM_diddflt,
        0x10, 0, 0.0f, 0.0f, 1, 0, 0.1f, NULL);
    strptrz[0] = "Move";
    strptrz[1] = "Turn";
    strptrz[2] = "Chase";
    strptrz[3] = "Surprize";
    strptrz[4] = "AttackIdle";
    strptrz[5] = "Taunt";
    strptrz[6] = "Taunt_1";
    strptrz[7] = "Taunt_2";
    for (i = 0; i < 8; i++)
    {
        xAnimTableAddTransition(table, intranz[ZVIL_ANIM_ATTACK], strptrz[i]);
        xAnimTableAddTransition(table, intranz[ZVIL_ANIM_ATTACK1], strptrz[i]);
        xAnimTableAddTransition(table, intranz[ZVIL_ANIM_ATTACK2], strptrz[i]);
        xAnimTableAddTransition(table, intranz[ZVIL_ANIM_LOB_ATTACK], strptrz[i]);
        xAnimTableAddTransition(table, intranz[ZVIL_ANIM_EXT_ATTACK_START], strptrz[i]);
        xAnimTableAddTransition(table, intranz[ZVIL_ANIM_EXT_ATTACK_LOOP], strptrz[i]);
    }
    strptrz[0] = "Attack";
    strptrz[1] = "Attack_1";
    strptrz[2] = "Attack_2";
    for (i = 0; i < 3; i++)
    {
        xAnimTableAddTransition(table, intranz[ZVIL_ANIM_COMBO], strptrz[i]);
        xAnimTableAddTransition(table, intranz[ZVIL_ANIM_COMBO1], strptrz[i]);
        xAnimTableAddTransition(table, intranz[ZVIL_ANIM_COMBO2], strptrz[i]);
    }
    xAnimTableAddTransition(table, intranz[ZVIL_ANIM_LOB_COMBO], "Lob_Attack");
    xAnimTableAddTransition(table, intranz[ZVIL_ANIM_EXT_COMBO], "Extend_Attack_Loop");
    strptrz[0] = "Move";
    strptrz[1] = "Chase";
    strptrz[2] = "Surprize";
    strptrz[3] = "Soaped";
    strptrz[4] = "Gummed";
    strptrz[5] = "Hear_Start";
    strptrz[6] = "Hear_Loop";
    strptrz[7] = "Look_Start";
    strptrz[8] = "Look_Loop";
    strptrz[9] = "Investigate_Start";
    strptrz[10] = "Investigate";
    strptrz[11] = "Attack";
    strptrz[12] = "Attack_1";
    strptrz[13] = "Attack_2";
    strptrz[14] = "Lob_Attack";
    strptrz[15] = "Extend_Attack_Start";
    strptrz[16] = "Extend_Attack_Loop";
    strptrz[17] = "Combo";
    strptrz[18] = "Combo_1";
    strptrz[19] = "Combo_2";
    strptrz[20] = "Lob_Combo";
    strptrz[21] = "Extend_Combo";
    strptrz[22] = "AttackIdle";
    strptrz[23] = "Turn";
    strptrz[24] = "Bored";
    for (i = 0; i < 25; i++)
    {
        xAnimTableAddTransition(table, intranz[ZVIL_ANIM_TAUNT], strptrz[i]);
        xAnimTableAddTransition(table, intranz[ZVIL_ANIM_TAUNT1], strptrz[i]);
        xAnimTableAddTransition(table, intranz[ZVIL_ANIM_TAUNT2], strptrz[i]);
        if (i != 22)
        {
            xAnimTableAddTransition(table, intranz[ZVIL_ANIM_ATTACKIDLE], strptrz[i]);
        }
    }
    deftran[ZVIL_ANIM_HEAR_LOOP] = xAnimTableNewTransition(
        table, "Hear_Start", "Hear_Loop", (xAnimTransitionConditionalCallback)VANM_dodflt,
        (xAnimTransitionCallback)VANM_diddflt, 0x10, 0, 0.0f, 0.0f, 1, 0, 0.1f, NULL);
    strptrz[0] = "Move";
    strptrz[1] = "Bored";
    strptrz[2] = "Turn";
    strptrz[3] = "Chase";
    strptrz[4] = "Gummed";
    strptrz[5] = "Soaped";
    strptrz[6] = "Dazed";
    strptrz[7] = "Dazed_Recovery";
    strptrz[8] = "Investigate_Start";
    strptrz[9] = "Investigate";
    strptrz[10] = "Look_Start";
    strptrz[11] = "Look_Loop";
    strptrz[12] = "Surprize";
    for (i = 0; i < 13; i++)
    {
        xAnimTableAddTransition(table, intranz[ZVIL_ANIM_HEAR_START], strptrz[i]);
        xAnimTableAddTransition(table, intranz[ZVIL_ANIM_HEAR_LOOP], strptrz[i]);
    }
    deftran[ZVIL_ANIM_LOOK_LOOP] = xAnimTableNewTransition(
        table, "Look_Start", "Look_Loop", (xAnimTransitionConditionalCallback)VANM_dodflt,
        (xAnimTransitionCallback)VANM_diddflt, 0x10, 0, 0.0f, 0.0f, 1, 0, 0.1f, NULL);
    strptrz[0] = "Move";
    strptrz[1] = "Bored";
    strptrz[2] = "Turn";
    strptrz[3] = "Chase";
    strptrz[4] = "Gummed";
    strptrz[5] = "Soaped";
    strptrz[6] = "Dazed";
    strptrz[7] = "Dazed_Recovery";
    strptrz[8] = "Investigate_Start";
    strptrz[9] = "Investigate";
    strptrz[10] = "Hear_Start";
    strptrz[11] = "Hear_Loop";
    strptrz[12] = "Surprize";
    for (i = 0; i < 13; i++)
    {
        xAnimTableAddTransition(table, intranz[ZVIL_ANIM_LOOK_START], strptrz[i]);
        xAnimTableAddTransition(table, intranz[ZVIL_ANIM_LOOK_LOOP], strptrz[i]);
    }
    strptrz[0] = "Move";
    strptrz[1] = "Bored";
    strptrz[2] = "Turn";
    strptrz[3] = "Chase";
    strptrz[4] = "Gummed";
    strptrz[5] = "Soaped";
    strptrz[6] = "Dazed";
    strptrz[7] = "Dazed_Recovery";
    strptrz[8] = "Investigate_Start";
    strptrz[9] = "Investigate";
    strptrz[10] = "Hear_Start";
    strptrz[11] = "Hear_Loop";
    strptrz[12] = "Look_Start";
    strptrz[13] = "Look_Loop";
    for (i = 0; i < 14; i++)
    {
        xAnimTableAddTransition(table, intranz[ZVIL_ANIM_SURPRIZE], strptrz[i]);
    }
    deftran[ZVIL_ANIM_INVESTIGATE] = xAnimTableNewTransition(
        table, "Investigate_Start", "Investigate", (xAnimTransitionConditionalCallback)VANM_dodflt,
        (xAnimTransitionCallback)VANM_diddflt, 0x10, 0, 0.0f, 0.0f, 1, 0, 0.1f, NULL);
    strptrz[0] = "Move";
    strptrz[1] = "Bored";
    strptrz[2] = "Turn";
    strptrz[3] = "Chase";
    strptrz[4] = "Gummed";
    strptrz[5] = "Soaped";
    strptrz[6] = "Dazed";
    // Slot 7 is skipped and keeps "Dazed_Recovery" from the list above.
    strptrz[8] = "Dazed_Recovery";
    strptrz[9] = "Hear_Start";
    strptrz[10] = "Hear_Loop";
    strptrz[11] = "Look_Start";
    strptrz[12] = "Look_Loop";
    strptrz[13] = "Surprize";
    for (i = 0; i < 14; i++)
    {
        xAnimTableAddTransition(table, intranz[ZVIL_ANIM_INVESTIGATE], strptrz[i]);
        xAnimTableAddTransition(table, intranz[ZVIL_ANIM_INVESTIGATE_START], strptrz[i]);
    }
    strptrz[0] = "Hit_By_Soap";
    strptrz[1] = "Soaped_Resist";
    strptrz[2] = "Hit_When_Soaped";
    strptrz[3] = "Bounced_When_Soaped";
    for (i = 0; i < 4; i++)
    {
        if (deftran[ZVIL_ANIM_SOAPED] == NULL)
        {
            deftran[ZVIL_ANIM_SOAPED] = xAnimTableNewTransition(
                table, strptrz[i], "Soaped", (xAnimTransitionConditionalCallback)VANM_dodflt,
                (xAnimTransitionCallback)VANM_diddflt, 0x10, 0, 0.0f, 0.0f, 1, 0, 0.1f, NULL);
        }
        else
        {
            xAnimTableAddTransition(table, deftran[ZVIL_ANIM_SOAPED], strptrz[i]);
        }
    }
    xAnimTableAddTransition(table, intranz[ZVIL_ANIM_SOAPED_RESIST], "Soaped");
    xAnimTableAddTransition(table, intranz[ZVIL_ANIM_SOAPED_BREAKFREE], "Soaped");
    xAnimTableAddTransition(table, intranz[ZVIL_ANIM_HIT_WHEN_SOAPED], "Soaped");
    xAnimTableAddTransition(table, intranz[ZVIL_ANIM_BOUNCED_WHEN_SOAPED], "Soaped");
    strptrz[0] = "Hit_By_Gum";
    strptrz[1] = "Gummed_Resist";
    strptrz[2] = "Hit_When_Gummed";
    strptrz[3] = "Bounced_When_Gummed";
    for (i = 0; i < 4; i++)
    {
        if (deftran[ZVIL_ANIM_GUMMED] == NULL)
        {
            deftran[ZVIL_ANIM_GUMMED] = xAnimTableNewTransition(
                table, strptrz[i], "Gummed", (xAnimTransitionConditionalCallback)VANM_dodflt,
                (xAnimTransitionCallback)VANM_diddflt, 0x10, 0, 0.0f, 0.0f, 1, 0, 0.1f, NULL);
        }
        else
        {
            xAnimTableAddTransition(table, deftran[ZVIL_ANIM_GUMMED], strptrz[i]);
        }
    }
    xAnimTableAddTransition(table, intranz[ZVIL_ANIM_GUMMED_RESIST], "Gummed");
    xAnimTableAddTransition(table, intranz[ZVIL_ANIM_GUMMED_BREAKFREE], "Gummed");
    xAnimTableAddTransition(table, intranz[ZVIL_ANIM_HIT_WHEN_GUMMED], "Gummed");
    xAnimTableAddTransition(table, intranz[ZVIL_ANIM_BOUNCED_WHEN_GUMMED], "Gummed");
    strptrz[0] = "Move";
    strptrz[1] = "Chase";
    strptrz[2] = "Hear_Start";
    strptrz[3] = "Hear_Loop";
    strptrz[4] = "Look_Start";
    strptrz[5] = "Look_Loop";
    strptrz[6] = "Investigate_Start";
    strptrz[7] = "Investigate";
    strptrz[8] = "Surprize";
    strptrz[9] = "Taunt";
    strptrz[10] = "Taunt_1";
    strptrz[11] = "Taunt_2";
    strptrz[12] = "AttackIdle";
    strptrz[13] = "Attack";
    strptrz[14] = "Attack_1";
    strptrz[15] = "Attack_2";
    strptrz[16] = "Lob_Attack";
    strptrz[17] = "Lob_Combo";
    strptrz[18] = "Extend_Attack_Start";
    strptrz[19] = "Extend_Attack_Loop";
    strptrz[20] = "Extend_Combo";
    strptrz[21] = "Combo";
    strptrz[22] = "Combo_1";
    strptrz[23] = "Combo_2";
    strptrz[24] = "Dazed";
    strptrz[25] = "Dazed_Begin";
    strptrz[26] = "Dazed_Recovery";
    strptrz[27] = "Bored";
    for (i = 0; i < 28; i++)
    {
        xAnimTableAddTransition(table, intranz[ZVIL_ANIM_HIT_BY_SOAP], strptrz[i]);
        xAnimTableAddTransition(table, intranz[ZVIL_ANIM_BOUNCED_WHEN_SOAPED], strptrz[i]);
        xAnimTableAddTransition(table, intranz[ZVIL_ANIM_SOAPED], strptrz[i]);
        xAnimTableAddTransition(table, intranz[ZVIL_ANIM_HIT_BY_GUM], strptrz[i]);
        xAnimTableAddTransition(table, intranz[ZVIL_ANIM_BOUNCED_WHEN_GUMMED], strptrz[i]);
        xAnimTableAddTransition(table, intranz[ZVIL_ANIM_GUMMED], strptrz[i]);
    }
    deftran[ZVIL_ANIM_DAZED] = xAnimTableNewTransition(
        table, "Dazed_Begin", "Dazed", (xAnimTransitionConditionalCallback)VANM_dodflt,
        (xAnimTransitionCallback)VANM_diddflt, 0x10, 0, 0.0f, 0.0f, 1, 0, 0.1f, NULL);
    strptrz[0] = "Move";
    strptrz[1] = "Turn";
    strptrz[2] = "Chase";
    strptrz[3] = "Hear_Start";
    strptrz[4] = "Hear_Loop";
    strptrz[5] = "Look_Start";
    strptrz[6] = "Look_Loop";
    strptrz[7] = "Investigate_Start";
    strptrz[8] = "Investigate";
    strptrz[9] = "Taunt";
    strptrz[10] = "Taunt_1";
    strptrz[11] = "Taunt_2";
    strptrz[12] = "Surprize";
    strptrz[13] = "Bored";
    strptrz[14] = "Lob_Attack";
    strptrz[15] = "Lob_Combo";
    strptrz[16] = "Extend_Attack_Start";
    strptrz[17] = "Extend_Attack_Loop";
    strptrz[18] = "AttackIdle";
    strptrz[19] = "Attack";
    strptrz[20] = "Attack_1";
    strptrz[21] = "Attack_2";
    for (i = 0; i < 22; i++)
    {
        xAnimTableAddTransition(table, intranz[ZVIL_ANIM_DAZED], strptrz[i]);
        xAnimTableAddTransition(table, intranz[ZVIL_ANIM_DAZED_BEGIN], strptrz[i]);
    }
    xAnimTableAddTransition(table, intranz[ZVIL_ANIM_DAZED_RECOVER], "Dazed");
    xAnimTableAddTransition(table, intranz[ZVIL_ANIM_DEATH_DAZED], "Dazed");
    xAnimTableAddTransition(table, intranz[ZVIL_ANIM_DEATH], "Dazed");
    xAnimTableAddTransition(table, intranz[ZVIL_ANIM_DEATH_ABOVE], "Dazed");
    for (i = ZVIL_ANIM_FIDGET_IDLE1; i < ZVIL_ANIM_BK_SCARYIDLE; i++)
    {
        if (i != ZVIL_ANIM_DEATH)
        {
            xAnimTableNewTransition(
                table, g_animname[i], "Death", (xAnimTransitionConditionalCallback)VANM_dotran,
                (xAnimTransitionCallback)VANM_didtran, 0, 0, 0.0f, 0.0f, 2, 0, 0.1f, NULL);
        }
        if (i != ZVIL_ANIM_DEATH_DAZED)
        {
            xAnimTableNewTransition(table, g_animname[i], "Death_From_Dazed",
                                    (xAnimTransitionConditionalCallback)VANM_dotran,
                                    (xAnimTransitionCallback)VANM_didtran, 0, 0, 0.0f, 0.0f, 2, 0,
                                    0.1f, NULL);
        }
        if (i != ZVIL_ANIM_DEATH_ABOVE)
        {
            xAnimTableNewTransition(table, g_animname[i], "Death_From_Above",
                                    (xAnimTransitionConditionalCallback)VANM_dotran,
                                    (xAnimTransitionCallback)VANM_didtran, 0, 0, 0.0f, 0.0f, 2, 0,
                                    0.1f, NULL);
        }
    }
    strptrz[0] = "Death";
    strptrz[1] = "Death_From_Dazed";
    strptrz[2] = "Death_From_Above";
    for (i = 0; i < 3; i++)
    {
        // The new transition is not kept in deftran, so each death state gets its own.
        if (deftran[ZVIL_ANIM_DEATH_IDLE] == NULL)
        {
            xAnimTableNewTransition(
                table, strptrz[i], "Death_Idle", (xAnimTransitionConditionalCallback)VANM_dodflt,
                (xAnimTransitionCallback)VANM_diddflt, 0x10, 0, 0.0f, 0.0f, 1, 0, 0.1f, NULL);
        }
        else
        {
            xAnimTableAddTransition(table, deftran[ZVIL_ANIM_DEATH_IDLE], strptrz[i]);
        }
        xAnimTableAddTransition(table, intranz[ZVIL_ANIM_DEATH_IDLE], strptrz[i]);
    }
    xAnimTableAddTransition(table, intranz[ZVIL_ANIM_JUMP_BEGIN], "Fashion_Pose");
    xAnimTableAddTransition(table, intranz[ZVIL_ANIM_JUMP_LOOP], "Fashion_Pose");
    xAnimTableNewTransition(
        table, "Jump_Begin", "Jump_Loop", (xAnimTransitionConditionalCallback)VANM_dodflt,
        (xAnimTransitionCallback)VANM_diddflt, 0x10, 0, 0.0f, 0.0f, 1, 0, 0.1f, NULL);
    xAnimTableNewTransition(
        table, "Jump_Loop", "Jump_Land", (xAnimTransitionConditionalCallback)VANM_dodflt,
        (xAnimTransitionCallback)VANM_diddflt, 0x10, 0, 0.0f, 0.0f, 1, 0, 0.1f, NULL);
    xAnimTableAddTransition(table, intranz[ZVIL_ANIM_JUMP_LAND], "Jump_Loop");
    xAnimTableAddTransition(table, intranz[ZVIL_ANIM_FALL_SPLAT], "Fall_Loop");

    return table;
}

// Plays the given animation, falling back to Idle when the villain's table lacks it.
static inline en_NPC_ANIMSTATES VANM_setone(st_zEntVillain* ent, en_NPC_ANIMSTATES anim)
{
    en_NPC_ANIMSTATES best[2] = { anim, ZVIL_ANIM_IDLE };
    return VANM_set(ent, best, 2, 0);
}

static inline en_NPC_ANIMSTATES VANM_hashToAnim(U32 hash)
{
    en_NPC_ANIMSTATES anim = ZVIL_ANIM_UNKNOWN;
    for (S32 i = 0; i < ZVIL_ANIM_NOMORE; i++)
    {
        if (hash == g_anamhash[i])
        {
            anim = (en_NPC_ANIMSTATES)i;
            break;
        }
    }
    return anim;
}

// Length of an animation state (with no state given, the villain's current one), scaled by
// mult; the unscaled length goes to tym when asked.
static inline F32 VANM_duration(xAnimState* astate, st_zEntVillain* ent, F32 mult, F32* tym)
{
    F32 dur = 0.0f;
    if (astate != NULL)
    {
        dur = astate->Data->Duration;
    }
    else if (ent != NULL)
    {
        xAnimSingle* single = VANM_entSingle(ent);
        xAnimState* cur = single ? single->State : NULL;
        if (cur != NULL)
        {
            dur = cur->Data->Duration;
        }
    }
    if (tym != NULL)
    {
        *tym = dur;
    }
    dur *= mult;
    return dur;
}

static U32 VANM_dotran(xAnimTransition* tran, xAnimSingle* single, st_zEntVillain* ent)
{
    U32 dotran = 0;

    if (ent->anim_playnext == ZVIL_ANIM_UNKNOWN)
    {
        dotran = 0;
    }
    else if (ent->anim_playing)
    {
        dotran = 0;
    }
    else if (tran->Dest == NULL)
    {
        dotran = 0;
    }
    else if (tran->Dest->Name == NULL || tran->Dest->ID == 0)
    {
        dotran = 0;
    }
    else if (tran->Dest->ID == g_anamhash[ent->anim_playnext])
    {
        dotran = 1;
        if (ent->anim_playsync)
        {
            tran->SrcTime = single->State->Data->Duration;
            ent->tmr_list[ZVIL_TYMR_MINANIMPLAY] = single->State->Data->Duration;
            ent->tmr_list[ZVIL_TYMR_MINANIMPLAY] =
                (ent->tmr_list[ZVIL_TYMR_MINANIMPLAY] - (single->Time - 0.05f) < -1.0f)
                    ? -1.0f
                    : ent->tmr_list[ZVIL_TYMR_MINANIMPLAY] - (single->Time - 0.05f);
        }

        if (ent->vhf->animtran != NULL)
        {
            ent->vhf->animtran(ent, tran, single);
        }
    }

    return dotran;
}

static U32 VANM_didtran(xAnimTransition* tran, xAnimSingle* single, st_zEntVillain* ent)
{
    F32 tym = 0.0f;

    ent->anim_playing = 1;
    ent->anim_playnext = VANM_hashToAnim(single->State->ID);
    ent->tmr_list[ZVIL_TYMR_MINANIMPLAY] =
        VANM_duration(single->State, ent, ent->anim_loopmult, &tym);
    if (single->State->Flags & 0x10)
    {
        ent->anim_loopmult = 0.97f;
    }
    ent->anim_playsync = 0;

    return 1;
}

static U32 VANM_dodflt(xAnimTransition* tran, xAnimSingle* single, st_zEntVillain* ent)
{
    return 1;
}

static U32 VANM_diddflt(xAnimTransition* tran, xAnimSingle* single, st_zEntVillain* ent)
{
    F32 tym = 0.0f;

    ent->anim_playing = 1;
    ent->anim_playnext = VANM_hashToAnim(single->State->ID);
    ent->tmr_list[ZVIL_TYMR_MINANIMPLAY] =
        VANM_duration(single->State, ent, ent->anim_loopmult, &tym);
    if (single->State->Flags & 0x10)
    {
        ent->anim_loopmult = 0.97f;
    }
    ent->anim_playsync = 0;

    return 1;
}

// Starts the transition from one state to another, making a temporary one when the table
// has none.
static inline void VANM_tranto(st_zEntVillain* ent, xAnimState* from, xAnimState* to)
{
    xAnimTable* table = ent->model->Anim->Table;
    xAnimTransition* newtran = NULL;

    if (from != NULL && to != NULL && from->ID != to->ID)
    {
        if (VANM_findtran(ent, table, from, to, 1, &newtran) == NULL && newtran != NULL)
        {
            xAnimPlayStartTransition(ent->model->Anim, newtran);
        }
    }
}

static en_NPC_ANIMSTATES VANM_set(st_zEntVillain* ent, en_NPC_ANIMSTATES* best, S32 cnt,
                                  S32 sync)
{
    xAnimState* astate = NULL;
    S32 i;

    if (best == NULL)
    {
        return ent->anim_playnext;
    }
    if (cnt < 1)
    {
        return ent->anim_playnext;
    }

    xAnimSingle* single = VANM_entSingle(ent);
    xAnimState* curstate = single ? single->State : NULL;

    for (i = 0; i < cnt; i++)
    {
        astate = xAnimTableGetStateID(ent->model->Anim->Table, g_anamhash[best[i]]);
        if (astate == NULL)
        {
            continue;
        }
        if (astate->UserFlags & 0x40000000)
        {
            astate = NULL;
            continue;
        }
        if (astate != NULL)
        {
            if (ent->anim_playnext != best[i])
            {
                ent->anim_playnext = best[i];
                ent->anim_playing = 0;
                ent->anim_playsync = sync;
                ent->anim_loopmult = 0.97f;
                ent->tmr_list[ZVIL_TYMR_MINANIMPLAY] =
                    VANM_duration(astate, ent, 0.97f, NULL);
                break;
            }
            astate = NULL;
            break;
        }
    }

    if (curstate != NULL && astate != NULL && astate->ID != curstate->ID)
    {
        VANM_tranto(ent, curstate, astate);
    }

    return ent->anim_playnext;
}

static en_NPC_ANIMSTATES VANM_randset(st_zEntVillain* ent, F32* wts,
                                      en_NPC_ANIMSTATES* choice_list, S32 cnt)
{
    en_NPC_ANIMSTATES fnl_anim;
    S32 i;
    xAnimState* chk_ast;
    S32* act_list;
    S32 use_anim;
    F32* act_wts = NULL;
    S32 act_cnt = 0;

    if (choice_list == NULL)
    {
        return ent->anim_playnext;
    }
    if (cnt < 1)
    {
        return ent->anim_playnext;
    }

    act_list = (S32*)xMemPushTemp(cnt * sizeof(S32));
    memset(act_list, 0, cnt * sizeof(S32));
    if (wts != NULL)
    {
        act_wts = (F32*)xMemPushTemp(cnt * sizeof(F32));
        memset(act_wts, 0, cnt * sizeof(F32));
    }

    for (i = 0; i < cnt; i++)
    {
        chk_ast = xAnimTableGetStateID(ent->model->Anim->Table, g_anamhash[choice_list[i]]);
        if (chk_ast == NULL)
        {
            continue;
        }
        if (chk_ast->UserFlags & 0x40000000)
        {
            continue;
        }
        act_list[act_cnt] = choice_list[i];
        if (act_wts != NULL)
        {
            act_wts[act_cnt] = wts[i];
        }
        act_cnt++;
    }

    if (act_cnt == 0)
    {
        fnl_anim = ent->anim_playnext;
    }
    else if (act_cnt == 1)
    {
        fnl_anim = VANM_setone(ent, (en_NPC_ANIMSTATES)act_list[0]);
    }
    else
    {
        if (act_wts != NULL)
        {
            VSUP_wtadjust(act_wts, act_cnt, 1.0f);
        }
        use_anim = VSUP_choose(act_list, act_wts, act_cnt);
        fnl_anim = VANM_setone(ent, (en_NPC_ANIMSTATES)use_anim);
    }

    xMemPopTemp(act_list);
    if (act_wts != NULL)
    {
        xMemPopTemp(act_wts);
    }

    return fnl_anim;
}

static xAnimTransition* VANM_findtran(st_zEntVillain* ent, xAnimTable* table, xAnimState* from,
                                      xAnimState* to, S32 autoslamit, xAnimTransition** newtran)
{
    xAnimTransition* da_tran = NULL;
    xAnimTransition* smack;
    xAnimTransitionList* tmp_tranlist;
    xAnimTransition* tmp_tran;

    if (from == NULL || to == NULL)
    {
        return NULL;
    }
    if (from->ID == to->ID)
    {
        return NULL;
    }

    if (from->Default == NULL)
    {
        if (from->ID != g_anamhash[ZVIL_ANIM_IDLE])
        {
            xSceneID2Name(globals.sceneCur, ent->id);
        }
    }
    else if (from->Default->T == NULL)
    {
        if (from->ID != g_anamhash[ZVIL_ANIM_IDLE])
        {
            xSceneID2Name(globals.sceneCur, ent->id);
        }
    }
    else if (to->ID == from->Default->T->Dest->ID)
    {
        da_tran = from->Default->T;
    }

    if (da_tran == NULL)
    {
        tmp_tranlist = from->List;
        while (tmp_tranlist != NULL)
        {
            tmp_tran = tmp_tranlist->T;
            if (tmp_tran == NULL)
            {
                tmp_tranlist = tmp_tranlist->Next;
                continue;
            }
            if (to->ID == tmp_tran->Dest->ID)
            {
                da_tran = tmp_tran;
                break;
            }
            tmp_tranlist = tmp_tranlist->Next;
        }
    }

    if (da_tran == NULL)
    {
        xSceneID2Name(globals.sceneCur, ent->id);
        if (autoslamit && newtran != NULL)
        {
            smack = xAnimTempTransitionAlloc();
            smack->Dest = to;
            smack->Callback = NULL;
            smack->SrcTime = 0.0f;
            smack->DestTime = 0.0f;
            smack->BlendRecip = 10.0f;
            smack->BlendOffset = NULL;
            *newtran = smack;
        }
    }

    return da_tran;
}

static xAnimSingle* VANM_entSingle(const _zEnt* ent)
{
    if (ent->model == NULL)
    {
        return NULL;
    }
    if (ent->model->Anim == NULL)
    {
        return NULL;
    }
    return ent->model->Anim->Single;
}

static S32 VSUP_choose(S32* list, F32* wts, S32 cnt)
{
    S32 idx = 0;
    F32 dlta = 1.0f / cnt;
    F32 pt = iurand();
    F32 lo;
    F32 hi = 0.0f;

    for (S32 i = 0; i < cnt; i++)
    {
        lo = hi;
        if (wts != NULL)
        {
            hi += wts[i];
        }
        else
        {
            hi += dlta;
        }
        if (pt >= lo && pt <= hi)
        {
            idx = i;
            break;
        }
    }

    return list[idx];
}

static void VSUP_wtadjust(F32* wts, S32 cnt, F32 arbref)
{
    F32 sum = 0.0f;
    S32 i;

    for (i = 0; i < cnt; i++)
    {
        if (wts[i] < 0.0f)
        {
            wts[i] = -wts[i];
        }
        sum += wts[i];
    }

    F32 fac = arbref / sum;
    for (i = 0; i < cnt; i++)
    {
        wts[i] *= fac;
    }
}

// Flips each component of out that points against the matching component of alter.
static void VSUP_cheap_reflect(_xVec3* out, _xVec3* alter)
{
    if (alter->x < 0.0f && out->x > 0.0f)
        out->x *= -1.0f;
    else if (alter->x > 0.0f && out->x < 0.0f)
        out->x *= -1.0f;

    if (alter->y < 0.0f && out->y > 0.0f)
        out->y *= -1.0f;
    else if (alter->y > 0.0f && out->y < 0.0f)
        out->y *= -1.0f;

    if (alter->z < 0.0f && out->z > 0.0f)
        out->z *= -1.0f;
    else if (alter->z > 0.0f && out->z < 0.0f)
        out->z *= -1.0f;
}
