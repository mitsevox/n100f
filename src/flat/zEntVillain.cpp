#include "zEntVillain.h"

#include "iMath.h"
#include "iModel.h"
#include "iSnd.h"
#include "xAnim.h"
#include "xBound.h"
#include "xEvent.h"
#include "xFFX.h"
#include "xMath.h"
#include "xMathInlines.h"
#include "xMemMgr.h"
#include "xordarray.h"
#include "xScene.h"
#include "xSnd.h"
#include "xString.h"
#include "xutil.h"
#include "xVec3Inlines.h"
#include "zEntPickup.h"
#include "zEntPlayer.h"
#include "zEntShaggy.h"
#include "zEntProjectile.h"
#include "zGlobals.h"
#include "zLaugh.h"
#include "zLightning.h"
#include "zMovePoint.h"
#include "zParEmitter.h"
#include "zScene.h"

#include <math.h>
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

enum en_BGG_PHASE
{
    BGG_PHAZ_UNKNOWN,
    BGG_PHAZ_BUILDUP,
    BGG_PHAZ_FLY,
    BGG_PHAZ_FALL,
    BGG_PHAZ_SHOCKWAVE
};

// A Green Ghost quake ball in flight.
struct st_BGG_QUAKEBALL
{
    S32 flg_stat;
    en_BGG_PHASE phase;
    F32 rad_build;
    F32 spd_flight;
    _xVec3 pos_tgt;
    F32 tym_flightMax;
    F32 rad_shock;
    F32 hyt_shock;
    F32 tym_shock;
};

// The lightning around a thrown Black Knight axe.
struct st_BKAXE_DATA
{
    S32 flg_stat;
    _tagLightning* strk[8];
    F32 rad_zap;
    F32 dst_zapdrop;
    F32 tym_zapdur;
    _xVec3 planes;
};

enum en_ZVMM_FLASK_STAGE
{
    MAST_STAGE_UNKNOWN,
    MAST_STAGE_TEASE,
    MAST_STAGE_TOSS,
    MAST_STAGE_AFTERMATH
};

enum en_ZVMM_FLASK_TYPE
{
    MAST_FLSK_FIRE,
    MAST_FLSK_ACID,
    MAST_FLSK_SMOG,
    MAST_FLSK_NOMORE
};

// A flask thrown by the Mastermind.
struct st_ZVMM_FLASK_DATA
{
    S32 flg_stat;
    en_ZVMM_FLASK_STAGE stage;
    en_ZVMM_FLASK_TYPE typ_flask;
    _xVec3 pos_tgt;
    F32 spd_flight;
    F32 arcness;
    F32 heavy;
    F32 tym_aftermath;
    F32 hyt_aftermath;
    F32 rad_aftermath;
};

enum en_VPAR_EMITTER_ROLE
{
    VPAR_EMIT_BOGUS = -1,
    VPAR_EMIT_WALLPRINT,
    VPAR_EMIT_EXTATTACK,
    VPAR_EMIT_SOAPY,
    VPAR_EMIT_POPSOAP,
    VPAR_EMIT_GUMMY,
    VPAR_EMIT_POPGUM,
    VPAR_EMIT_BUTTBOUNCE,
    VPAR_EMIT_HEADBUTT,
    VPAR_EMIT_STUN,
    VPAR_EMIT_DYING,
    VPAR_EMIT_DECAY,
    VPAR_EMIT_POOF,
    VPAR_EMIT_RESPAWN,
    VPAR_EMIT_TELEPORT,
    VPAR_EMIT_BORED,
    VPAR_EMIT_GUARD,
    VPAR_EMIT_QUESTMARK,
    VPAR_EMIT_EXCLAIM
};

// One particle emitter a villain type uses, and the role it plays.
struct st_zEntVillain_ParEmitRecord
{
    en_VPAR_EMITTER_ROLE emitrole;
    en_zEntVillainSubTypeID viltype;
    S32 priority;
    U32 vilowner;
    U32 emitterID;
    _zParEmitter* emitctxt;
    S32 flg_need;
    S32 flg_omit;
};


// One bonus item a villain can drop; bit 0 of flg_info marks it as dropped.
struct st_NPC_BONUS
{
    S32 flg_info;
    _zEntPickup* pickup;
};

// How many of one villain type the player has gummed, soaped and beaten, and how many there are.
struct st_NPC_TALLY_RECORD
{
    S32 cnt_hitgum;
    S32 cnt_hitsoap;
    S32 cnt_killed;
    S32 cnt_mortals;
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
static inline en_NPC_ANIMSTATES VANM_setone(st_zEntVillain* ent, en_NPC_ANIMSTATES anim);
static inline en_NPC_ANIMSTATES VANM_randthree(st_zEntVillain* ent, en_NPC_ANIMSTATES a,
                                               en_NPC_ANIMSTATES b, en_NPC_ANIMSTATES c);
static xAnimTransition* VANM_findtran(st_zEntVillain* ent, xAnimTable* table, xAnimState* from,
                                      xAnimState* to, S32 autoslamit, xAnimTransition** newtran);
static xAnimSingle* VANM_entSingle(const _zEnt* ent);
static inline xAnimState* VANM_entState(const _zEnt* ent);
static S32 VSUP_accum_collide(st_zEntVillain* vil, _xVec3* vec_accum);
static void VSUP_cheap_reflect(_xVec3* out, _xVec3* alter);
static F32 VSUP_ds2_toPos(const _zEnt* ent, _xVec3 pos);
static F32 VSUP_dir_toPos(const _zEnt* ent, _xVec3 pos, _xVec3* dir);
static inline F32 VSUP_ds2_toEnt(const _zEnt* ent, const _xEnt* tgt);
static inline F32 VSUP_dir_toEnt(const _zEnt* ent, const _xEnt* tgt, _xVec3* dir);
static inline F32 VSUP_ent_AOI(const st_zEntVillain* vil, const _zEnt* ent,
                               en_NPC_QUADRANT* qdrant);
static S32 VSUP_ent_inFOV(const st_zEntVillain* vil, const _zEnt* ent);
static S32 VSUP_ent_inLOS_timz(const st_zEntVillain* vil, const _zEnt* ent, S32 prev, F32 rand);
static S32 VSUP_ent_inLOS(const st_zEntVillain* vil, const _zEnt* ent, S32 prev, F32 rand);
static S32 zVIL_chk_pitfall(st_zEntVillain* vil, S32 prev, F32 rand);
static inline S32 VSUP_vertPos(const st_zEntVillain* vil, en_ZVIL_MODEL_VERTEX vert, _xVec3* pos);
static S32 VOUT_patrol_move(st_zEntVillain* vil, _xScene* scn, F32 dt, const _xEntFrame* frame);
static S32 VOUT_chkdo_patrolArrived(st_zEntVillain* vil);
static S32 VOUT_chkdo_minipatrolArrived(st_zEntVillain* vil);
static S32 VOUT_chase_move(st_zEntVillain* vil, _xScene* scn, F32 dt, const _xEntFrame* frame);
static S32 VOUT_follow_move(st_zEntVillain* vil, _xScene* scn, F32 dt, const _xEntFrame* frame);
static S32 VOUT_chkdo_followArrived(st_zEntVillain* vil);

static S32 VSUP_choose(S32* list, F32* wts, S32 cnt);
static void VSUP_wtadjust(F32* wts, S32 cnt, F32 arbref);

static void zVIL_tag_verts(st_zEntVillain* ent);
static S32 zVIL_trangoal(st_zEntVillain* ent, st_STIMULUS_CONTEXT_DATA* stimctxt, F32 dt);
static void zVIL_apply_move(st_zEntVillain* ent, _xScene* scn, F32 dt, _xEntFrame* frame);
static void zVIL_upd_timers(st_zEntVillain* ent, F32 dt);
static S32 VCFG_getParm(st_ZVIL_CONFIG_REFERENCE* cfgdata, en_NPC_INI_PARAMETERS pid, void* vv);
static S32 VSND_procQueue(st_zEntVillain* vil, F32 dt);
static U32 VSND_goPlay(st_zEntVillain* vil, en_NPC_SOUND sndEvent, U32 sndExplicit, S32 flg_vsnd,
                       F32 dist);
static U32 VSND_select_sound(st_zEntVillain* vil, en_NPC_SOUND sndEvent);
static void zVIL_input_stim(st_zEntVillain* ent, en_NPC_STIMULUS, st_STIMULUS_CONTEXT_DATA* stimctxt);
static void zVIL_precalc(st_zEntVillain* vil, S32 flg_needinfo);
static void VPRJ_zapTheAxe(zEntProjectile* prj, S32 cleanup);
static en_BEHAVIOUR_GOAL VGOL_set(st_zEntVillain* ent, en_BEHAVIOUR_GOAL goal);
static S32 VSAV_load(st_zEntVillain* ent, xSerial* xser);
static S32 VSAV_save(st_zEntVillain* ent, xSerial* xser);
static void VEFX_plop_gum(st_zEntVillain* ent);
static void VPAR_emit_partype(st_zEntVillain* ent, en_VPAR_EMITTER_ROLE emittype, F32 dt);
static void VBON_drop_bonusItem(st_zEntVillain* vil);
static st_NPC_BONUS* VBON_choose_bonusItem(en_zEntVillainSubTypeID vtyp);
static S32 VSND_findProps(en_NPC_SOUND sndEvent, st_NPC_SOUND_PROPERTIES** sprop_caller);
static S32 VSND_addQueueEntry(st_zEntVillain* vil, en_NPC_SOUND sndEvent, S32 flg_vsnd, F32 radius,
                              F32 delay, F32 repeatDelay, S32 repeatCount, U32 useThisSound);
static inline S32 VSND_queueSound(st_zEntVillain* vil, en_NPC_SOUND sndEvent);
static inline S32 VSND_chanIsBusy(st_zEntVillain* vil, S32 flg_chan);
static inline F32 VANM_entTime(const _zEnt* ent);
static inline F32 VCFG_tymref(st_zEntVillain* vil, S32 which);
static inline bool VCFG_inDamageWindow(st_zEntVillain* vil, F32 tym);
static inline void VPRJ_BMM_toss(zEntProjectile* prj, const _xVec3* pos_tgt);
static S32 VNAV_setup_telepoint(st_zEntVillain* vil, S32 force);
static S32 VNAV_get_nav(st_zEntVillain* ent);
static F32 VNAV_get_movespeed(st_zEntVillain* ent, F32 base);
static inline U32 VANM_entStateType(const _zEnt* ent);
static inline U32 VANM_entMoveType(const _zEnt* ent);
static inline void VANM_setSpeed(st_zEntVillain* ent, F32 spd, _xVec3 sanim);
static zEntProjectile* VWEP_lobReservePRJ(st_zEntVillain* vil);
static inline S32 VWEP_lobInitArrow(st_zEntVillain* vil, _xVec3* pos_tgt, _xVec3* vec_off,
                                    _xVec3* vec_spin, F32 arcness, F32 heavy);
static S32 VWEP_lobInitArcing(st_zEntVillain* vil, _xVec3* pos_tgt, _xVec3* vec_off,
                              _xVec3* vec_spin, F32 arcness, F32 heavy);
static S32 VWEP_lobActivatePRJ(zEntProjectile* prj, st_zEntVillain* vil, _xVec3* pos_tgt,
                               _xVec3* vec_off, _xVec3* vec_spin, F32 arcness, F32 heavy);
static void VEFX_mm_zapper(st_zEntVillain* vil, S32 cleanup);
static inline S32 VWEP_lobAtPlayer(zEntProjectile* prj, st_zEntVillain* vil);
static S32 VOUT_dogoto_widget(st_zEntVillain* vil, xBase* widge, F32 dst_offset, S32 follow);
static inline void VOUT_dogoto_pos(st_zEntVillain* vil, const _xVec3* pos, F32 dst_offset);
static void VWEP_mm_zapPlayer(st_zEntVillain* vil);
static inline void VEFX_mm_teleportHide(F32 dt);
static inline void VEFX_mm_teleportShow(st_zEntVillain* vil, F32 dt);
static inline void VSUP_haltFrame(st_zEntVillain* vil);
static inline void VSUP_haltEntFrame(const _xEntFrame* frame);

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

// Play flags and start delay of each villain sound type, ended by VSND_LISTEND.
static st_NPC_SOUND_PROPERTIES g_sndProps[36] = {
    { VSND_TYP_ENCOUNTER, 0x4003, 0.0f },
    { VSND_TYP_SPOOKYROOM, 0x4003, 0.0f },
    { VSND_TYP_IDLESLOP, 0xC001, -1.0f },
    { VSND_TYP_MOVE, 0x4000, 0.2f },
    { VSND_TYP_MOVE_ALT, 0x4000, 0.5f },
    { VSND_TYP_CLANKING, 0x4000, 0.2f },
    { VSND_TYP_SPLISH, 0x4000, 0.1f },
    { VSND_TYP_SPLASH, 0x4000, 0.1f },
    { VSND_TYP_SNORE, 0x4000, 0.4f },
    { VSND_TYP_CHEERING, 0x1202, -1.0f },
    { VSND_TYP_ZSFXTALK, 0x1202, -1.0f },
    { VSND_TYP_ONELINER, 0x1202, -1.0f },
    { VSND_TYP_ONELINERTOO, 0x1202, -1.0f },
    { VSND_TYP_NEEDTARBOOTS, 0x1202, 0.1f },
    { VSND_TYP_EXTRASPRING, 0x1202, 0.1f },
    { VSND_TYP_NEEDFLOAT, 0x1202, 0.1f },
    { VSND_TYP_HAVEFLOAT, 0x1202, 0.1f },
    { VSND_TYP_BLOCKING, 0x9201, 0.1f },
    { VSND_TYP_ESCAPING, 0x9201, 0.1f },
    { VSND_TYP_TRASHTALK, 0x9201, 0.1f },
    { VSND_TYP_FOREPLAYHURT, 0x9201, 0.1f },
    { VSND_TYP_BUTTONPRESS, 0x9201, 0.1f },
    { VSND_TYP_ATTACKBUILD, 0x9201, 0.1f },
    { VSND_TYP_ALMOSTKILLED, 0x9201, 0.1f },
    { VSND_TYP_MMDIE, 0x9201, 5.1f },
    { VSND_TYP_MMLAUGH, 0x9201, 0.1f },
    { VSND_TYP_BKZAPPED, 0x1201, -1.0f },
    { VSND_TYP_BKDIE, 0x1201, -1.0f },
    { VSND_TYP_LOOK, 0xA001, -1.0f },
    { VSND_TYP_LISTEN, 0xA001, -1.0f },
    { VSND_TYP_CURIOUS, 0xA001, -1.0f },
    { VSND_TYP_HOLOFADE, 0x1000A001, 0.1f },
    { VSND_TYP_SPELL_CAST, 0x1000, 0.0f },
    { VSND_TYP_SPELL_PLAY, 0x2000, 0.0f },
    { VSND_TYP_SPELL_LOOP, 0x2803, 0.0f },
    { VSND_LISTEND, 0x2000, 0.1f },
};

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

// Where the Mastermind stood when he last looked for a teleport point.
static _xVec3 sMMPreTeleportPos;

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

// The bonus items villains can drop.
static st_NPC_BONUS g_npcBonus[11] = { 0 };

// The player's tally per villain type; the last row holds the totals.
static st_NPC_TALLY_RECORD g_stattally[eEntVillainType_NoMore + 1] = { 0 };

// Lightning data of the thrown Black Knight axes.
static st_BKAXE_DATA g_axfxpool[10] = { 0 };

// Green Ghost quake balls.
static st_BGG_QUAKEBALL g_quakepool[4] = { 0 };

// Names of the Mastermind's teleport points: eight safe ones and four dangerous ones.
static char* g_strz_mmtp[12] = {
    "MM_TP_SAFE_A", "MM_TP_SAFE_B", "MM_TP_SAFE_C", "MM_TP_SAFE_D",
    "MM_TP_SAFE_E", "MM_TP_SAFE_F", "MM_TP_SAFE_G", "MM_TP_SAFE_H",
    "MM_TP_DNGR_A", "MM_TP_DNGR_B", "MM_TP_DNGR_C", "MM_TP_DNGR_D",
};

// The Mastermind's teleport points, found by name when he is prepared.
static _zMovePoint* g_mmtp[12] = { 0 };

// Mastermind flasks.
static st_ZVMM_FLASK_DATA g_mmflask[10] = { 0 };

// The particle emitters the scene's villains use.
static st_XORDEREDARRAY g_emitlist = { 0 };

// The particle emitters of the Mastermind's teleport out and in.
static _zParEmitter* sEmitMMTeleportHide;
static _zParEmitter* sEmitMMTeleportShow;

// How many times the villain system has been started without being shut down.
static S32 g_npcinit;

// How many villains the scene has.
static S32 g_npccnt;

// All villains of the scene, in one block.
static st_zEntVillain* g_npcpool;

// The goal and the stimulus a villain type's stimulus or goal-change step is handling.
static en_BEHAVIOUR_GOAL g_pgol;
static en_NPC_STIMULUS g_pstm;

// Records of the particle emitters the villains use, how many are in use, and whether they
// are set up.
static st_zEntVillain_ParEmitRecord* g_emitpool;
static S32 g_emitnext;
static S32 g_emitready;

// Which boss the current scene's battle is against (1 to 4), or 0 when it is not a boss battle.
static S32 g_scene_is_a_boss_battle;

// A block of scratch memory the villain system keeps from startup.
static char* g_datablock;

// Counts down one start of the villain system and returns how many remain.
S32 zEntVillain_Shutdown()
{
    if (--g_npcinit == 0)
    {
        return g_npcinit;
    }
    return g_npcinit;
}

// Gets the villains ready for a new scene: discards the old villain block (filled with 0x33),
// clears the emitter list and allocates a fresh, empty block of 32 emitter records.
S32 zEntVillain_scenePrepare()
{
    zEntVillain_MemPool(0);
    XOrdInit(&g_emitlist, 32);
    g_emitpool = (st_zEntVillain_ParEmitRecord*)xMemAlloc(gActiveHeap, 32 * sizeof(st_zEntVillain_ParEmitRecord));
    memset(g_emitpool, 0, 32 * sizeof(st_zEntVillain_ParEmitRecord));
    g_emitnext = 0;
    g_emitready = 0;
    return 1;
}

// Discards the villain block (filled with 0x33) when a scene ends.
S32 zEntVillain_sceneFinish()
{
    zEntVillain_MemPool(0);
    return g_npcinit;
}

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

// Allocates a zeroed block for npccnt villains and returns it. With 0, fills the old block with
// 0x33 bytes and forgets it.
st_zEntVillain* zEntVillain_MemPool(U32 npccnt)
{
    if (npccnt)
    {
        g_npccnt = npccnt;
        g_npcpool = (st_zEntVillain*)xMemAlloc(gActiveHeap, g_npccnt * sizeof(st_zEntVillain));
        memset(g_npcpool, 0, npccnt * sizeof(st_zEntVillain));
    }
    else
    {
        if (g_npcpool && g_npccnt > 0)
        {
            memset(g_npcpool, 0x33, g_npccnt * sizeof(st_zEntVillain));
        }
        g_npccnt = 0;
        g_npcpool = NULL;
    }

    return g_npcpool;
}

// Returns the villain in slot npcidx of the pool.
st_zEntVillain* zEntVillain_NPCInst(U32 npcidx)
{
    return &g_npcpool[npcidx];
}

// Writes the villain's state to a saved game.
void zEntVillain_Save(st_zEntVillain* ent, xSerial* xser)
{
    VSAV_save(ent, xser);
}

// Reads the villain's state from a saved game.
void zEntVillain_Load(st_zEntVillain* ent, xSerial* xser)
{
    VSAV_load(ent, xser);
}

// Draws the extras of every villain: the gum blob on a gummed villain, and whatever its type
// draws on top once it is active.
void zEntVillain_BonusRender()
{
    S32 npccnt;
    st_zEntVillain* vil;
    S32 i;

    npccnt = g_npccnt;
    for (i = 0; i < npccnt; i++)
    {
        vil = &g_npcpool[i];

        if (vil->goal == ZVIL_GOAL_GUMMED)
        {
            VEFX_plop_gum(vil);
        }

        if (vil->mode >= ZVIL_MODE_NORMAL && vil->vhf->xtrarend)
        {
            xSceneID2Name(globals.sceneCur, vil->id);
            vil->vhf->xtrarend(vil);
        }
    }
}

// Tells the villain the player bounced on it and returns how its type responded. Bounce particles
// go up unless the type ignored it or took damage; if it killed a villain that counts for the
// tally, the tally goes up, and a villain that drops a bonus item drops it.
en_ZVILLAIN_BOUNCE_RESPONSE zEntVillain_Bounce(st_zEntVillain* ent)
{
    st_STIMULUS_CONTEXT_DATA stimctxt = { ZVIL_STIM_BOUNCED };

    stimctxt.res_buttbounce = ZVIL_BOUNCE_NONE;
    zVIL_input_stim(ent, ZVIL_STIM_BOUNCED, &stimctxt);

    if (stimctxt.res_buttbounce != ZVIL_BOUNCE_NONE && stimctxt.res_buttbounce != ZVIL_BOUNCE_DAMAGE)
    {
        VPAR_emit_partype(ent, VPAR_EMIT_BUTTBOUNCE, globals.update_dt);
    }

    if (ent->ability & 0x4)
    {
        zEntPlayer_Notify(31);
    }

    if (ent->goal == ZVIL_GOAL_DIE)
    {
        if ((ent->ability & 0x200000) && (g_npcTable[ent->asset->subtype].ability & 0x200000) &&
            (g_npcTable[ent->asset->subtype].abl_vuln & 0x4))
        {
            g_stattally[ent->asset->subtype].cnt_killed++;
            g_stattally[eEntVillainType_NoMore].cnt_killed++;
        }

        if (ent->allow & 0x800)
        {
            VBON_drop_bonusItem(ent);
        }
    }

    return stimctxt.res_buttbounce;
}

// Tells the villain something bumped into it and returns how its type responded.
en_ZVILLAIN_BUMPED_RESPONSE zEntVillain_Bumped(st_zEntVillain* ent, const _xCollis* colrec)
{
    st_STIMULUS_CONTEXT_DATA stimctxt = { ZVIL_STIM_BUMPED };

    stimctxt.stim = ZVIL_STIM_BUMPED;
    stimctxt.colrec = (_xCollis*)colrec;
    stimctxt.res_bumped = ZVIL_BUMPED_NONE;
    zVIL_input_stim(ent, ZVIL_STIM_BUMPED, &stimctxt);
    return stimctxt.res_bumped;
}

// Called when the player is hurt; villains do nothing with it.
void zEntVillain_HeroWasHurt()
{
}

// Stuns the villain with the given strength (at least 0.1), and lets the laugh track know.
void zEntVillain_Stun(st_zEntVillain* ent, F32 strength)
{
    st_STIMULUS_CONTEXT_DATA stimctxt = { ZVIL_STIM_STUNNING };

    stimctxt.amt_stun = MAX(strength, 0.1f);
    zVIL_input_stim(ent, ZVIL_STIM_STUNNING, &stimctxt);
    zLaughNotify(12);
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


// The standard movement step, by goal:
// - lifeless or starting up: the frame stops;
// - idle or bored: the frame stops if move ability bit 0x8000 is set;
// - action: with that bit set, the villain's move target is one unit below where he stands, at
//   speed 1;
// - ambush: he patrols if he has a path point, otherwise heads for the player; his target height
//   bobs by 0.25 dt sin(time in the goal), his speed is 6, his frame stops, and with a path point
//   he checks for arrival;
// - follow, patrol, chase: the shared helpers;
// - mini patrol: he heads for his anchor if he has one, otherwise for his guard spot;
// - listen: he turns to his listening heading;
// - surprised, attack, lob, taunt: he turns to face the player, and when readying an attack only
//   if precalc plyr_mvinf bits 0xFF00 are clear;
// - soaped: while flg_misc bit 0x100000 is set and his knockback velocity's squared length is over
//   0.01, that velocity shrinks to 0.99 each frame and he moves with it; otherwise it is cleared
//   and his target height bobs by 0.5 dt sin(5 times the time in the goal);
// - gummed: with move ability bit 0x100000 and both a next and a previous path point, he heads
//   for the next point and on arrival takes the following one and sends an arrive event;
// - dying: with ability bit 0x80000 and flg_misc bit 0x100000, he moves at half his bowling
//   direction.
static S32 VILN_std_calcmove(st_zEntVillain* vil, _xScene* scn, F32 dt, const _xEntFrame* frame)
{
    _zMovePoint* nav_tmp;
    _zMovePoint* nav_old;
    st_ZVIL_PRECALC* prec = &vil->precalc;
    F32 dy;
    F32 dist;
    _xVec3 entpos = { 0 };

    entpos = *xEntGetPos(vil);

    switch (vil->goal)
    {
    case ZVIL_GOAL_AFTERLIFE:
    case ZVIL_GOAL_BASEINIT:
    case ZVIL_GOAL_STARTUP:
    case ZVIL_GOAL_GOONLINE:
        VSUP_haltEntFrame(frame);
        break;
    case ZVIL_GOAL_IDLE:
    case ZVIL_GOAL_BORED:
        if (vil->abl_move & 0x8000)
        {
            VSUP_haltEntFrame(frame);
        }
        break;
    case ZVIL_GOAL_ACTION:
        if (vil->abl_move & 0x8000)
        {
            vil->pos_moveto = *xEntGetPos(vil);
            vil->pos_moveto.y -= 1.0f;
            vil->spd_moveto = 1.0f;
            vil->domove |= 0x8000;
            vil->domove |= 0x800;
            vil->domove |= 0x4;
            vil->domove |= 0x80000000;
        }
        break;
    case ZVIL_GOAL_AMBUSH:
        if (vil->nav_next)
        {
            VOUT_patrol_move(vil, scn, dt, frame);
        }
        else
        {
            xVec3Copy(&vil->pos_moveto, xEntGetPos(&globals.player.ent));
            vil->domove |= 0x800;
            vil->domove |= 0xA;
            vil->domove |= 0x80000000;
        }

        vil->pos_moveto.y += 0.25f * dt * (F32)sin(vil->tmr_list[ZVIL_TYMR_CURGOAL]);
        vil->domove |= 0x4;
        vil->spd_moveto = 6.0f;
        vil->domove |= 0x8000;
        VSUP_haltEntFrame(frame);

        if (vil->nav_next)
        {
            VOUT_chkdo_patrolArrived(vil);
        }
        break;
    case ZVIL_GOAL_FOLLOW:
        VOUT_follow_move(vil, scn, dt, frame);
        VOUT_chkdo_followArrived(vil);
        break;
    case ZVIL_GOAL_PATROL:
        VOUT_patrol_move(vil, scn, dt, frame);
        VOUT_chkdo_patrolArrived(vil);
        break;
    case ZVIL_GOAL_MINIPATROL:
        if (vil->vhf->query(vil, ZVIL_Q_HAVEANCHOR))
        {
            xVec3Copy(&vil->pos_moveto, &vil->pos_pursue_home);
        }
        else
        {
            xVec3Copy(&vil->pos_moveto, &vil->pos_guard_home);
        }
        vil->ang_moveto = VSUP_dir_toPos(vil, vil->pos_moveto, &vil->dir_moveto);
        if (vil->abl_move & 0x1000 || vil->abl_move & 0x400)
        {
            vil->domove |= 0x4;
        }
        vil->domove |= 0x800;
        vil->domove |= 0x1;
        vil->domove |= 0xA;
        vil->domove |= 0x80000000;
        VOUT_chkdo_minipatrolArrived(vil);
        break;
    case ZVIL_GOAL_LISTEN:
        vil->ang_moveto = vil->ang_listen;
        vil->domove |= 0x40;
        vil->domove |= 0x1;
        vil->domove |= 0x10;
        vil->domove |= 0x80000000;
        break;
    case ZVIL_GOAL_SURPRIZE:
        vil->ang_moveto = prec->ang_VtoP;
        vil->domove |= 0x40;
        vil->domove |= 0x1;
        vil->domove |= 0x10;
        vil->domove |= 0x80000000;
        break;
    case ZVIL_GOAL_ATTACKREADY:
        if (!(prec->plyr_mvinf & 0xFF00))
        {
            vil->ang_moveto = prec->ang_VtoP;
            vil->domove |= 0x40;
            vil->domove |= 0x1;
            vil->domove |= 0x10;
            vil->domove |= 0x80000000;
        }
        break;
    case ZVIL_GOAL_CHASE:
        VOUT_chase_move(vil, scn, dt, frame);
        break;
    case ZVIL_GOAL_CQCATTACK:
    case ZVIL_GOAL_LOBBING:
        vil->ang_moveto = prec->ang_VtoP;
        vil->domove |= 0x40;
        vil->domove |= 0x1;
        vil->domove |= 0x10;
        vil->domove |= 0x80000000;
        break;
    case ZVIL_GOAL_TAUNT:
        vil->ang_moveto = prec->ang_VtoP;
        vil->domove |= 0x40;
        vil->domove |= 0x1;
        vil->domove |= 0x10;
        vil->domove |= 0x80000000;
        break;
    case ZVIL_GOAL_SOAPED:
        if (vil->flg_misc & 0x100000 && xVec3Length2(&vil->vec_knocked) > 0.01f)
        {
            xVec3SMulBy(&vil->vec_knocked, 0.99f);
            xVec3Copy(&vil->vel_moveto, &vil->vec_knocked);
            vil->domove |= 0x1000;
            vil->domove |= 0x80000000;
        }
        else
        {
            xVec3SMulBy(&vil->vec_knocked, 0.0f);
            vil->pos_moveto.y += 0.5f * dt * (F32)sin(5.0f * vil->tmr_list[ZVIL_TYMR_CURGOAL]);
            vil->domove |= 0x800;
            vil->domove |= 0x4;
            vil->domove |= 0x80000000;
        }
        break;
    case ZVIL_GOAL_GUMMED:
        if (!(vil->abl_move & 0x100000))
        {
            break;
        }
        if (!vil->nav_next)
        {
            break;
        }
        if (!vil->nav_prev)
        {
            break;
        }

        xVec3Copy(&vil->pos_moveto, zMovePointGetPos(vil->nav_next));
        dist = VSUP_ds2_toPos(vil, vil->pos_moveto);
        vil->ang_moveto = VSUP_dir_toPos(vil, vil->pos_moveto, &vil->dir_moveto);

        if (vil->abl_move & 0x400)
        {
            vil->domove |= 0x4;
            dy = SQR(vil->pos_moveto.y - entpos.y);
            dist = MAX(dist, dy);
        }

        vil->domove |= 0x800;
        vil->domove |= 0x1;
        vil->domove |= 0xA;
        vil->domove |= 0x80000000;

        if (dist < g_npcTable[vil->asset->subtype].ds2_atpos)
        {
            nav_old = vil->nav_next;
            nav_tmp = NULL;
            zMovePointGetNext(vil->nav_next, vil->nav_prev, &nav_tmp, NULL);
            if (vil->nav_next)
            {
                vil->nav_prev = vil->nav_next;
            }
            vil->nav_next = nav_tmp;
            zEntEvent(vil, nav_old, eEventArrive);
        }
        break;
    case ZVIL_GOAL_DIE:
        if (vil->ability & 0x80000 && vil->flg_misc & 0x100000)
        {
            xVec3SMul(&vil->vel_moveto, &vil->dir_bowling, 0.5f);
            vil->domove |= 0x1000;
            vil->domove |= 0x80000000;
        }
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

// Shaggy villain type 5 moves by goal: idle he turns to face the player, and he follows and
// patrols like other villains.
static S32 VILN_shagM_calcmove(st_zEntVillain* vil, _xScene* scn, F32 dt, const _xEntFrame* frame)
{
    switch (vil->goal)
    {
    case ZVIL_GOAL_AFTERLIFE:
    case ZVIL_GOAL_BASEINIT:
    case ZVIL_GOAL_STARTUP:
    case ZVIL_GOAL_GOONLINE:
        break;
    case ZVIL_GOAL_IDLE:
        vil->ang_moveto = vil->precalc.ang_VtoP;
        vil->domove |= 0x40;
        vil->domove |= 0x1;
        vil->domove |= 0x10;
        vil->domove |= 0x80000000;
        break;
    case ZVIL_GOAL_ACTION:
        break;
    case ZVIL_GOAL_FOLLOW:
        VOUT_follow_move(vil, scn, dt, frame);
        VOUT_chkdo_followArrived(vil);
        break;
    case ZVIL_GOAL_PATROL:
        VOUT_patrol_move(vil, scn, dt, frame);
        VOUT_chkdo_patrolArrived(vil);
        break;
    case ZVIL_GOAL_DIE:
        break;
    }

    return 1;
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

// Fred, Daphne, Velma, Holly, the groundskeeper and the professor move by goal: idle or talking
// they turn to face the player, and they follow and patrol like villains.
static S32 VILN_gang_calcmove(st_zEntVillain* vil, _xScene* scn, F32 dt, const _xEntFrame* frame)
{
    switch (vil->goal)
    {
    case ZVIL_GOAL_AFTERLIFE:
    case ZVIL_GOAL_BASEINIT:
    case ZVIL_GOAL_STARTUP:
    case ZVIL_GOAL_GOONLINE:
        break;
    case ZVIL_GOAL_IDLE:
        vil->ang_moveto = vil->precalc.ang_VtoP;
        vil->domove |= 0x40;
        vil->domove |= 0x1;
        vil->domove |= 0x10;
        vil->domove |= 0x80000000;
        break;
    case ZVIL_GOAL_TALK:
        vil->ang_moveto = vil->precalc.ang_VtoP;
        vil->domove |= 0x40;
        vil->domove |= 0x1;
        vil->domove |= 0x10;
        vil->domove |= 0x80000000;
        break;
    case ZVIL_GOAL_ACTION:
        break;
    case ZVIL_GOAL_FOLLOW:
        VOUT_follow_move(vil, scn, dt, frame);
        VOUT_chkdo_followArrived(vil);
        break;
    case ZVIL_GOAL_PATROL:
        VOUT_patrol_move(vil, scn, dt, frame);
        VOUT_chkdo_patrolArrived(vil);
        break;
    case ZVIL_GOAL_DIE:
        break;
    }

    return 1;
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

// The Black Knight, Green Ghost, Phantom and Redbeard outside their boss fights move by goal: idle
// or taunting they turn to face the player, and they follow and patrol like other villains.
static S32 VILN_sham_calcmove(st_zEntVillain* vil, _xScene* scn, F32 dt, const _xEntFrame* frame)
{
    switch (vil->goal)
    {
    case ZVIL_GOAL_AFTERLIFE:
    case ZVIL_GOAL_BASEINIT:
    case ZVIL_GOAL_STARTUP:
    case ZVIL_GOAL_GOONLINE:
        break;
    case ZVIL_GOAL_IDLE:
    case ZVIL_GOAL_TAUNT:
        vil->ang_moveto = vil->precalc.ang_VtoP;
        vil->domove |= 0x40;
        vil->domove |= 0x1;
        vil->domove |= 0x10;
        vil->domove |= 0x80000000;
        break;
    case ZVIL_GOAL_FOLLOW:
        VOUT_follow_move(vil, scn, dt, frame);
        VOUT_chkdo_followArrived(vil);
        break;
    case ZVIL_GOAL_PATROL:
        VOUT_patrol_move(vil, scn, dt, frame);
        VOUT_chkdo_patrolArrived(vil);
        break;
    case ZVIL_GOAL_DIE:
        break;
    }

    return 1;
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

// The Black Knight boss moves by goal: idle, readying an attack, lobbing or taunting he turns to
// face the player, and he follows and patrols like other villains. Hurt or dying he keeps turning:
// to a fixed heading of 1.5 pi plus 0.2 once his hit points are gone, otherwise to his last
// heading.
static S32 VILN_BBK_calcmove(st_zEntVillain* vil, _xScene* scn, F32 dt, const _xEntFrame* frame)
{
    switch (vil->goal)
    {
    case ZVIL_GOAL_AFTERLIFE:
    case ZVIL_GOAL_BASEINIT:
    case ZVIL_GOAL_STARTUP:
    case ZVIL_GOAL_GOONLINE:
        break;
    case ZVIL_GOAL_IDLE:
    case ZVIL_GOAL_ATTACKREADY:
    case ZVIL_GOAL_LOBBING:
    case ZVIL_GOAL_TAUNT:
        vil->ang_moveto = vil->precalc.ang_VtoP;
        vil->domove |= 0x40;
        vil->domove |= 0x1;
        vil->domove |= 0x10;
        vil->domove |= 0x80000000;
        break;
    case ZVIL_GOAL_FOLLOW:
        VOUT_follow_move(vil, scn, dt, frame);
        VOUT_chkdo_followArrived(vil);
        break;
    case ZVIL_GOAL_PATROL:
        VOUT_patrol_move(vil, scn, dt, frame);
        VOUT_chkdo_patrolArrived(vil);
        break;
    case ZVIL_GOAL_HURT:
    case ZVIL_GOAL_DIE:
        if (vil->hitpts <= 0)
        {
            vil->ang_moveto = 1.5f * PI + 0.2f;
        }
        vil->domove |= 0x40;
        vil->domove |= 0x1;
        vil->domove |= 0x10;
        vil->domove |= 0x80000000;
        break;
    }

    return 1;
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

// The Green Ghost boss moves by goal: idle, lobbing or taunting he turns to face the player. He
// follows and patrols like other villains, with domove bit 0x80000 set when his move target is on
// the +x side of him and 0x40000 otherwise. Hurt or dying, his move target circles his vortex
// point: the angle turns pi per second, the radius shrinks to 0.95 each frame, and once his hit
// points are zero the vortex point sinks 0.6 per second.
static S32 VILN_BGG_calcmove(st_zEntVillain* vil, _xScene* scn, F32 dt, const _xEntFrame* frame)
{
    F32 dx;
    _xVec3 entpos = { 0 };

    entpos = *xEntGetPos(vil);

    switch (vil->goal)
    {
    case ZVIL_GOAL_AFTERLIFE:
    case ZVIL_GOAL_BASEINIT:
    case ZVIL_GOAL_STARTUP:
    case ZVIL_GOAL_GOONLINE:
        break;
    case ZVIL_GOAL_IDLE:
    case ZVIL_GOAL_LOBBING:
    case ZVIL_GOAL_TAUNT:
        vil->ang_moveto = vil->precalc.ang_VtoP;
        vil->domove |= 0x40;
        vil->domove |= 0x1;
        vil->domove |= 0x10;
        vil->domove |= 0x80000000;
        break;
    case ZVIL_GOAL_FOLLOW:
        VOUT_follow_move(vil, scn, dt, frame);
        dx = vil->pos_moveto.x - entpos.x;
        vil->domove &= ~0xC0000;
        if (dx > 0.0f)
        {
            vil->domove |= 0x80000;
        }
        else
        {
            vil->domove |= 0x40000;
        }
        VOUT_chkdo_followArrived(vil);
        break;
    case ZVIL_GOAL_PATROL:
        VOUT_patrol_move(vil, scn, dt, frame);
        dx = vil->pos_moveto.x - entpos.x;
        vil->domove &= ~0xC0000;
        if (dx > 0.0f)
        {
            vil->domove |= 0x80000;
        }
        else
        {
            vil->domove |= 0x40000;
        }
        VOUT_chkdo_patrolArrived(vil);
        break;
    case ZVIL_GOAL_HURT:
    case ZVIL_GOAL_DIE:
    {
        _xVec3 vecoff = { 0 };

        vecoff.x = sin(vil->ang_spiral);
        vecoff.z = cos(vil->ang_spiral);
        xVec3SMulBy(&vecoff, vil->rad_spiral);
        xVec3Add(&vil->pos_moveto, &vil->pos_vortex, &vecoff);
        vil->domove |= 0x800;
        vil->domove |= 0x40000000;
        vil->domove |= 0xA;
        vil->domove |= 0x80000000;

        vil->ang_spiral = xAngleClamp(vil->ang_spiral + PI * dt);
        vil->rad_spiral *= 0.95f;
        if (!vil->hitpts)
        {
            vil->pos_vortex.y -= 0.6f * dt;
        }
        break;
    }
    }

    return 1;
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

// The Redbeard boss moves by goal: idle, casting or taunting his move target is the fixed point
// (-0.5, 0, 0) and his heading is toward the player (domove bit 0x10 is not set), and he follows
// and patrols like other villains.
static S32 VILN_BRB_calcmove(st_zEntVillain* vil, _xScene* scn, F32 dt, const _xEntFrame* frame)
{
    _xVec3 pos_hardwired = { -0.5f, 0.0f, 0.0f };

    switch (vil->goal)
    {
    case ZVIL_GOAL_AFTERLIFE:
    case ZVIL_GOAL_BASEINIT:
    case ZVIL_GOAL_STARTUP:
    case ZVIL_GOAL_GOONLINE:
        break;
    case ZVIL_GOAL_IDLE:
        xVec3Copy(&vil->pos_moveto, &pos_hardwired);
        vil->domove |= 0x800;
        vil->domove |= 0x40000000;
        vil->domove |= 0xA;
        vil->ang_moveto = vil->precalc.ang_VtoP;
        vil->domove |= 0x40;
        vil->domove |= 0x1;
        vil->domove |= 0x80000000;
        break;
    case ZVIL_GOAL_SPELLCAST:
    case ZVIL_GOAL_TAUNT:
        xVec3Copy(&vil->pos_moveto, &pos_hardwired);
        vil->domove |= 0x800;
        vil->domove |= 0x40000000;
        vil->domove |= 0xA;
        vil->ang_moveto = vil->precalc.ang_VtoP;
        vil->domove |= 0x40;
        vil->domove |= 0x1;
        vil->domove |= 0x80000000;
        break;
    case ZVIL_GOAL_FOLLOW:
        VOUT_follow_move(vil, scn, dt, frame);
        VOUT_chkdo_followArrived(vil);
        break;
    case ZVIL_GOAL_PATROL:
        VOUT_patrol_move(vil, scn, dt, frame);
        VOUT_chkdo_patrolArrived(vil);
        break;
    case ZVIL_GOAL_HURT:
    case ZVIL_GOAL_DIE:
        break;
    }

    return 1;
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

// Handles the Mastermind's notices. On prepare, it marks the scene as his boss battle (4), lets him
// attack, sets his hit points from his configuration (at least 1), sets his lob spin to one turn
// and his hurt time to none, finds his twelve teleport points by name, and sets the first three
// flag bits on his first two models and clears them on the next six. Then, like the standard
// notice, it sets up his vertex tags on prepare and, on precalc, refreshes his precalculated data
// once his mode is normal or later.
static S32 VILN_BMM_notice(st_zEntVillain* vil, en_ZVIL_NOTICE note, st_STIMULUS_CONTEXT_DATA*)
{
    S32 i;
    xModelInstance* minst = vil->model;
    S32 idx = 0;

    if (note == ZVIL_NOTE_PREPARE)
    {
        g_scene_is_a_boss_battle = 4;
        vil->allow |= 0x8;
        vil->hitpts = (vil->vilcfg.hitpts < 1) ? 1 : vil->vilcfg.hitpts;
        vil->vec_lobSpin.x = 6.2831855f;
        vil->vilcfg.tymref_list[ZVIL_TDUR_HURT] = -1.0f;
    }

    if (note == ZVIL_NOTE_PREPARE)
    {
        for (i = 0; i < 12; i++)
        {
            g_mmtp[i] = zMovePoint_From_xAssetID(xStrHash(g_strz_mmtp[i]));
        }
    }

    if (note == ZVIL_NOTE_PREPARE)
    {
        while (minst)
        {
            switch (idx)
            {
            case 0:
            case 1:
                minst->Flags |= 0x1;
                minst->Flags |= 0x2;
                minst->Flags |= 0x4;
                break;
            case 2:
            case 3:
                minst->Flags &= (U16)~0x1;
                minst->Flags &= (U16)~0x2;
                minst->Flags &= (U16)~0x4;
                break;
            case 4:
            case 5:
                minst->Flags &= (U16)~0x1;
                minst->Flags &= (U16)~0x2;
                minst->Flags &= (U16)~0x4;
                break;
            case 6:
            case 7:
                minst->Flags &= (U16)~0x1;
                minst->Flags &= (U16)~0x2;
                minst->Flags &= (U16)~0x4;
                break;
            default:
                break;
            }

            minst = minst->Next;
            idx++;
        }
    }

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

// The Mastermind's decision step: for his current goal, picks the goal to change to, if any.
static S32 VILN_BMM_aware(st_zEntVillain* vil, _xScene*, F32 dt, st_STIMULUS_CONTEXT_DATA* resp)
{
    st_ZVIL_PRECALC* prec = &vil->precalc;

    switch (vil->goal)
    {
    case ZVIL_GOAL_AFTERLIFE:
    case ZVIL_GOAL_BASEINIT:
        break;
    case ZVIL_GOAL_STARTUP:
        resp->trangoal = ZVIL_GOAL_GOONLINE;
        resp->tranmode = ZVIL_MODE_READY;
        resp->stim = ZVIL_STIM_TRANGOAL;
        break;
    case ZVIL_GOAL_GOONLINE:
        resp->trangoal = vil->goaldflt;
        resp->tranmode = ZVIL_MODE_NORMAL;
        resp->stim = ZVIL_STIM_TRANGOAL;
        break;
    case ZVIL_GOAL_IDLE:
        if (vil->allow & 0x10)
        {
            resp->trangoal = ZVIL_GOAL_ATTACKREADY;
            resp->tranmode = ZVIL_MODE_NOCHANGE;
            resp->stim = ZVIL_STIM_TRANGOAL;
            break;
        }
        if (vil->allow & 0x4)
        {
            S32 rc = VNAV_setup_telepoint(vil, 1);
            if (rc)
            {
                resp->trangoal = ZVIL_GOAL_TELEPORT;
                resp->tranmode = ZVIL_MODE_NOCHANGE;
                resp->stim = ZVIL_STIM_TRANGOAL;
                resp->mm_teleport_hurtz_doggie = 0;
                break;
            }
        }
        if (vil->vhf->query(vil, ZVIL_Q_CANPATROL))
        {
            if (vil->vhf->query(vil, ZVIL_Q_NEEDNAV))
            {
                VNAV_get_nav(vil);
            }
            if (vil->vhf->query(vil, ZVIL_Q_HAVENAV))
            {
                resp->trangoal = ZVIL_GOAL_PATROL;
                resp->tranmode = ZVIL_MODE_NOCHANGE;
                resp->stim = ZVIL_STIM_TRANGOAL;
            }
        }
        break;
    case ZVIL_GOAL_ACTION:
        if (vil->tmr_list[ZVIL_TYMR_MINANIMPLAY] <= 0.0f)
        {
            if (vil->allow & 0x10)
            {
                resp->trangoal = ZVIL_GOAL_ATTACKREADY;
                resp->tranmode = ZVIL_MODE_NOCHANGE;
                resp->stim = ZVIL_STIM_TRANGOAL;
                break;
            }
            if (vil->allow & 0x4)
            {
                S32 rc = VNAV_setup_telepoint(vil, 1);
                if (rc)
                {
                    resp->trangoal = ZVIL_GOAL_TELEPORT;
                    resp->tranmode = ZVIL_MODE_NOCHANGE;
                    resp->stim = ZVIL_STIM_TRANGOAL;
                    resp->mm_teleport_hurtz_doggie = 0;
                    break;
                }
            }
            resp->trangoal = ZVIL_GOAL_IDLE;
            resp->tranmode = ZVIL_MODE_NOCHANGE;
            resp->stim = ZVIL_STIM_TRANGOAL;
        }
        break;
    case ZVIL_GOAL_FOLLOW:
        if (vil->allow & 0x10)
        {
            resp->trangoal = ZVIL_GOAL_ATTACKREADY;
            resp->tranmode = ZVIL_MODE_NOCHANGE;
            resp->stim = ZVIL_STIM_TRANGOAL;
            break;
        }
        if (vil->allow & 0x4)
        {
            S32 rc = VNAV_setup_telepoint(vil, 1);
            if (rc)
            {
                resp->trangoal = ZVIL_GOAL_TELEPORT;
                resp->tranmode = ZVIL_MODE_NOCHANGE;
                resp->stim = ZVIL_STIM_TRANGOAL;
                resp->mm_teleport_hurtz_doggie = 0;
                break;
            }
        }
        if (!vil->vhf->query(vil, ZVIL_Q_DOFOLLOW))
        {
            resp->trangoal = ZVIL_GOAL_IDLE;
            resp->tranmode = ZVIL_MODE_NOCHANGE;
            resp->stim = ZVIL_STIM_TRANGOAL;
        }
        break;
    case ZVIL_GOAL_PATROL:
        if (vil->allow & 0x10)
        {
            resp->trangoal = ZVIL_GOAL_ATTACKREADY;
            resp->tranmode = ZVIL_MODE_NOCHANGE;
            resp->stim = ZVIL_STIM_TRANGOAL;
            break;
        }
        if (vil->allow & 0x4)
        {
            S32 rc = VNAV_setup_telepoint(vil, 1);
            if (rc)
            {
                resp->trangoal = ZVIL_GOAL_TELEPORT;
                resp->tranmode = ZVIL_MODE_NOCHANGE;
                resp->stim = ZVIL_STIM_TRANGOAL;
                resp->mm_teleport_hurtz_doggie = 0;
                break;
            }
        }
        if (vil->vhf->query(vil, ZVIL_Q_NEEDNAV))
        {
            VNAV_get_nav(vil);
        }
        if (!vil->vhf->query(vil, ZVIL_Q_HAVENAV))
        {
            resp->trangoal = ZVIL_GOAL_IDLE;
            resp->tranmode = ZVIL_MODE_NOCHANGE;
            resp->stim = ZVIL_STIM_TRANGOAL;
        }
        break;
    case ZVIL_GOAL_TELEPORT:
        if (vil->tmr_list[ZVIL_TYMR_CURGOAL] > 0.2f)
        {
            VEFX_mm_zapper(vil, 1);
        }
        if (vil->allow & 0x10)
        {
            if (vil->tmr_list[ZVIL_TYMR_GENERIC] <= 0.0f && !VSND_chanIsBusy(vil, 1))
            {
                resp->trangoal = ZVIL_GOAL_ATTACKREADY;
                resp->tranmode = ZVIL_MODE_NOCHANGE;
                resp->stim = ZVIL_STIM_TRANGOAL;
            }
        }
        break;
    case ZVIL_GOAL_LEAP:
        if (vil->tmr_list[ZVIL_TYMR_MINANIMPLAY] <= 0.0f && (vil->allow & 0x10))
        {
            resp->trangoal = ZVIL_GOAL_ATTACKREADY;
            resp->tranmode = ZVIL_MODE_NOCHANGE;
            resp->stim = ZVIL_STIM_TRANGOAL;
        }
        break;
    case ZVIL_GOAL_TAUNT:
        if (vil->tmr_list[ZVIL_TYMR_MINANIMPLAY] <= 0.0f)
        {
            if (vil->allow & 0x10)
            {
                resp->trangoal = ZVIL_GOAL_ATTACKREADY;
                resp->tranmode = ZVIL_MODE_NOCHANGE;
                resp->stim = ZVIL_STIM_TRANGOAL;
            }
            else
            {
                resp->trangoal = ZVIL_GOAL_IDLE;
                resp->tranmode = ZVIL_MODE_NOCHANGE;
                resp->stim = ZVIL_STIM_TRANGOAL;
            }
        }
        break;
    case ZVIL_GOAL_ATTACKREADY:
        if (!(vil->allow & 0x14))
        {
            resp->trangoal = ZVIL_GOAL_IDLE;
            resp->tranmode = ZVIL_MODE_NOCHANGE;
            resp->stim = ZVIL_STIM_TRANGOAL;
            break;
        }
        if (vil->vhf->query(vil, ZVIL_Q_PLYRTOOCLOSE))
        {
            resp->trangoal = ZVIL_GOAL_BLOCKHEAD;
            resp->tranmode = ZVIL_MODE_NOCHANGE;
            resp->stim = ZVIL_STIM_TRANGOAL;
            break;
        }
        if (vil->tmr_list[ZVIL_TYMR_GENERIC] <= 0.0f)
        {
            if (fabsf(vil->frame->rot.angle - prec->ang_VtoP) <= 0.2f)
            {
                vil->anim_loopmult = 0.97f;
                vil->vhf->pickanim(vil, ZVIL_GOAL_ATTACKREADY, ZVIL_GSPOT_START);
            }
            else
            {
                vil->anim_loopmult = 0.97f;
                vil->vhf->pickanim(vil, ZVIL_GOAL_ATTACKREADY, ZVIL_GSPOT_MOVERIGHT);
            }
            vil->tmr_list[ZVIL_TYMR_GENERIC] = 0.2f;
        }
        if (vil->vhf->query(vil, ZVIL_Q_CANATTACK) &&
            vil->tmr_list[ZVIL_TDUR_LOBRELOAD] <= 0.0f)
        {
            vil->prj_reserved = VWEP_lobReservePRJ(vil);
            if (vil->prj_reserved)
            {
                resp->trangoal = ZVIL_GOAL_LOBBING;
                resp->tranmode = ZVIL_MODE_NOCHANGE;
                resp->stim = ZVIL_STIM_TRANGOAL;
            }
        }
        break;
    case ZVIL_GOAL_LOBBING:
        if (vil->vhf->query(vil, ZVIL_Q_PLYRTOOCLOSE))
        {
            resp->trangoal = ZVIL_GOAL_BLOCKHEAD;
            resp->tranmode = ZVIL_MODE_NOCHANGE;
            resp->stim = ZVIL_STIM_TRANGOAL;
            break;
        }
        if (vil->anim_playnext == ZVIL_ANIM_MM_BA_ATTACK_BEGIN)
        {
        }
        else if (vil->anim_playnext == ZVIL_ANIM_MM_BA_ATTACK_LOOP)
        {
            if (vil->tmr_list[ZVIL_TDUR_BUILDUP] <= 0.0f && !VSND_chanIsBusy(vil, 1))
            {
                vil->anim_loopmult = 0.97f;
                vil->vhf->pickanim(vil, ZVIL_GOAL_LOBBING, ZVIL_GSPOT_KICKATTACK);
                vil->tmr_list[ZVIL_TYMR_MINANIMPLAY] = 32000000.0f;
            }
        }
        else if (vil->anim_playnext == ZVIL_ANIM_MM_BA_ATTACK && vil->anim_playing)
        {
            if (VCFG_inDamageWindow(vil, VANM_entTime(vil)) && vil->prj_reserved &&
                vil->cnt_lobSalvo > 0)
            {
                _xVec3 vec_tmp = { 0 };
                if (!(vil->cnt_lobSalvo & 1))
                {
                    xVec3Copy(&vec_tmp, &prec->vec_VtoP);
                    vec_tmp.x *= 0.6f;
                    vec_tmp.y = 0.0f;
                    vec_tmp.z *= 0.6f;
                }

                xVec3Add(&vec_tmp, &vec_tmp, xBoundCenter(&globals.player.ent.bound));
                VPRJ_BMM_toss(vil->prj_reserved, &vec_tmp);
                vil->prj_reserved = NULL;

                vil->cnt_lobSalvo--;
                if (vil->cnt_lobSalvo > 0 && vil->prj_reserved == NULL)
                {
                    vil->prj_reserved = VWEP_lobReservePRJ(vil);
                    if (vil->prj_reserved)
                    {
                        VWEP_lobAtPlayer(vil->prj_reserved, vil);
                    }
                }
            }
        }
        else if (vil->tmr_list[ZVIL_TYMR_MINANIMPLAY] <= 0.0f)
        {
            S32 rc = VNAV_setup_telepoint(vil, 1);
            if (rc)
            {
                resp->trangoal = ZVIL_GOAL_TELEPORT;
                resp->tranmode = ZVIL_MODE_NOCHANGE;
                resp->stim = ZVIL_STIM_TRANGOAL;
                resp->mm_teleport_hurtz_doggie = 0;
            }
        }
        break;
    case ZVIL_GOAL_DAZED:
        VPAR_emit_partype(vil, VPAR_EMIT_STUN, dt);
        if (vil->tmr_list[ZVIL_TYMR_MINANIMPLAY] <= 0.0f && !VSND_chanIsBusy(vil, 1))
        {
            if (vil->tmr_list[ZVIL_TTIL_RECOVERY] <= 0.0f)
            {
                vil->anim_loopmult = 0.97f;
                vil->vhf->pickanim(vil, ZVIL_GOAL_DAZED, ZVIL_GSPOT_STATERECOVER);
                vil->tmr_list[ZVIL_TTIL_RECOVERY] = 32000000.0f;
            }
            else if (vil->tmr_list[ZVIL_TDUR_DAZED] <= 0.0f)
            {
                resp->trangoal = ZVIL_GOAL_ATTACKREADY;
                resp->tranmode = ZVIL_MODE_NOCHANGE;
                resp->stim = ZVIL_STIM_TRANGOAL;
            }
        }
        break;
    case ZVIL_GOAL_BLOCKHEAD:
        if (VSND_chanIsBusy(vil, 1))
        {
            vil->tmr_list[ZVIL_TYMR_GENERIC] = 3.0f + 0.25f * (iurand() - 0.5f) * 3.0f;
        }
        else if (vil->tmr_list[ZVIL_TYMR_MINANIMPLAY] <= 0.0f)
        {
            if (vil->tmr_list[ZVIL_TYMR_GENERIC] <= 0.0f)
            {
                S32 rc = VNAV_setup_telepoint(vil, 1);
                if (rc)
                {
                    resp->trangoal = ZVIL_GOAL_TELEPORT;
                    resp->tranmode = ZVIL_MODE_NOCHANGE;
                    resp->stim = ZVIL_STIM_TRANGOAL;
                    resp->mm_teleport_hurtz_doggie = 1;
                    break;
                }
            }

            if (vil->vhf->query(vil, ZVIL_Q_PLYRTOOCLOSE))
            {
                vil->anim_loopmult = 0.97f;
                vil->vhf->pickanim(vil, ZVIL_GOAL_BLOCKHEAD, ZVIL_GSPOT_RESUME);
            }
            else if (vil->anim_playnext == ZVIL_ANIM_MM_BA_SHIELD_LOOP)
            {
                vil->anim_loopmult = 0.97f;
                vil->vhf->pickanim(vil, ZVIL_GOAL_BLOCKHEAD, ZVIL_GSPOT_STATERECOVER);
                vil->tmr_list[ZVIL_TYMR_MINANIMPLAY] = 32000000.0f;
            }
            else if (vil->anim_playnext == ZVIL_ANIM_MM_BA_SHIELD_END ||
                     vil->anim_playnext == ZVIL_ANIM_MM_BA_IDLE)
            {
                resp->trangoal = ZVIL_GOAL_ATTACKREADY;
                resp->tranmode = ZVIL_MODE_NOCHANGE;
                resp->stim = ZVIL_STIM_TRANGOAL;
            }
        }
        break;
    case ZVIL_GOAL_KNOCKBACK:
        if (vil->tmr_list[ZVIL_TYMR_MINANIMPLAY] <= 0.0f)
        {
            if (vil->anim_playnext == ZVIL_ANIM_MM_BA_KNOCKED_BEGIN)
            {
            }
            else if (vil->anim_playnext == ZVIL_ANIM_MM_BA_KNOCKED_LOOP && vil->anim_playing)
            {
                if (vil->tmr_list[ZVIL_TYMR_GENERIC] <= 0.0f)
                {
                    vil->anim_loopmult = 0.97f;
                    vil->vhf->pickanim(vil, ZVIL_GOAL_KNOCKBACK, ZVIL_GSPOT_STATERECOVER);
                    VSND_queueSound(vil, VSND_TYP_ALMOSTKILLED);
                    vil->tmr_list[ZVIL_TYMR_MINANIMPLAY] = 32000000.0f;
                    vil->pflags |= 0x4;
                }
            }
            else if (vil->anim_playnext == ZVIL_ANIM_MM_BA_KNOCKED_END && vil->anim_playing)
            {
                if (!VSND_chanIsBusy(vil, 1))
                {
                    S32 rc = VNAV_setup_telepoint(vil, 1);
                    if (rc)
                    {
                        resp->trangoal = ZVIL_GOAL_TELEPORT;
                        resp->tranmode = ZVIL_MODE_NOCHANGE;
                        resp->stim = ZVIL_STIM_TRANGOAL;
                        resp->mm_teleport_hurtz_doggie = 0;
                    }
                    else
                    {
                        resp->trangoal = ZVIL_GOAL_ATTACKREADY;
                        resp->tranmode = ZVIL_MODE_NOCHANGE;
                        resp->stim = ZVIL_STIM_TRANGOAL;
                    }
                }
            }
            else
            {
                resp->trangoal = ZVIL_GOAL_ATTACKREADY;
                resp->tranmode = ZVIL_MODE_NOCHANGE;
                resp->stim = ZVIL_STIM_TRANGOAL;
            }
        }
        break;
    case ZVIL_GOAL_HURT:
        if (vil->tmr_list[ZVIL_TYMR_MINANIMPLAY] <= 0.0f && vil->tmr_list[ZVIL_TDUR_HURT] <= 0.0f)
        {
            if (vil->hitpts < 1)
            {
                resp->trangoal = ZVIL_GOAL_DIE;
                resp->tranmode = ZVIL_MODE_DEATH;
                resp->stim = ZVIL_STIM_TRANGOAL;
            }
            else
            {
                resp->trangoal = ZVIL_GOAL_ATTACKREADY;
                resp->tranmode = ZVIL_MODE_NOCHANGE;
                resp->stim = ZVIL_STIM_TRANGOAL;
            }
        }
        break;
    case ZVIL_GOAL_DIE:
        if (vil->tmr_list[ZVIL_TYMR_MINANIMPLAY] <= 0.0f)
        {
            zEntEvent(vil, vil, eEventVilReport_DecayComplete);
            vil->tmr_list[ZVIL_TYMR_MINANIMPLAY] = 32000000.0f;
        }
        break;
    default:
        resp->trangoal = ZVIL_GOAL_IDLE;
        resp->tranmode = ZVIL_MODE_NOCHANGE;
        resp->stim = ZVIL_STIM_TRANGOAL;
        break;
    }

    return 1;
}

// The Mastermind's stimulus step: how he answers a stimulus in his current goal. While dead or
// starting up, the go-ahead starts him up. In his battle goals he dies when told to kill himself,
// goes to the afterlife, acts on the push of a button, is hurt by a hurt-boss stimulus, breaks
// soap and gum bubbles (laughing), takes damage from headbutts and bumps, is dazed by a bounce,
// lobs a flask at an attack order when he may attack and has one, taunts when he can, and goes
// to a position or a widget, patrols or idles when told to. Dazed, a headbutt knocks him back,
// soap and gum bubbles still break (without the laugh) and a bounce does nothing.
static S32 VILN_BMM_stimulus(st_zEntVillain* vil, st_STIMULUS_CONTEXT_DATA* stimctxt,
                             st_STIMULUS_CONTEXT_DATA* resp)
{
    S32 std_mvpt_nav;
    en_NPC_STIMULUS stim = stimctxt->stim;

    switch (g_pgol = vil->goal)
    {
    case ZVIL_GOAL_NONE:
        break;
    case ZVIL_GOAL_AFTERLIFE:
        switch (g_pstm = stim)
        {
        case ZVIL_STIM_NONE:
            break;
        case ZVIL_STIM_TRANGOAL:
            break;
        case ZVIL_STIM_MAKEREADY:
            resp->trangoal = ZVIL_GOAL_STARTUP;
            resp->tranmode = ZVIL_MODE_SHUTDOWN;
            resp->stim = ZVIL_STIM_TRANGOAL;
            break;
        }
        break;
    case ZVIL_GOAL_BASEINIT:
        switch (g_pstm = stim)
        {
        case ZVIL_STIM_NONE:
            break;
        case ZVIL_STIM_TRANGOAL:
            break;
        case ZVIL_STIM_MAKEREADY:
            resp->trangoal = ZVIL_GOAL_STARTUP;
            resp->tranmode = ZVIL_MODE_SHUTDOWN;
            resp->stim = ZVIL_STIM_TRANGOAL;
            break;
        }
        break;
    case ZVIL_GOAL_STARTUP:
    case ZVIL_GOAL_GOONLINE:
    case ZVIL_GOAL_DAZED:
    case ZVIL_GOAL_HURT:
    case ZVIL_GOAL_DIE:
    case ZVIL_GOAL_LEAP:
    case ZVIL_GOAL_TELEPORT:
        g_pstm = stim;
        break;
    case ZVIL_GOAL_IDLE:
    case ZVIL_GOAL_FOLLOW:
    case ZVIL_GOAL_PATROL:
    case ZVIL_GOAL_TAUNT:
    case ZVIL_GOAL_LOBBING:
    case ZVIL_GOAL_ACTION:
    case ZVIL_GOAL_BLOCKHEAD:
    case ZVIL_GOAL_KNOCKBACK:
        switch (g_pstm = stim)
        {
        case ZVIL_STIM_NONE:
            break;
        case ZVIL_STIM_TRANGOAL:
            break;
        case ZVIL_STIM_KILLSELF:
            vil->hitpts = 0;
            resp->trangoal = ZVIL_GOAL_DIE;
            resp->tranmode = ZVIL_MODE_DEATH;
            resp->stim = ZVIL_STIM_TRANGOAL;
            break;
        case ZVIL_STIM_GOTOAFTERLIFE:
            resp->trangoal = ZVIL_GOAL_AFTERLIFE;
            resp->tranmode = ZVIL_MODE_SHUTDOWN;
            resp->stim = ZVIL_STIM_TRANGOAL;
            break;
        case ZVIL_STIM_MMPUSHBUTTON:
            resp->trangoal = ZVIL_GOAL_ACTION;
            resp->tranmode = ZVIL_MODE_NOCHANGE;
            resp->stim = ZVIL_STIM_TRANGOAL;
            break;
        case ZVIL_STIM_HURTBOSS:
            resp->trangoal = ZVIL_GOAL_HURT;
            resp->tranmode = ZVIL_MODE_NOCHANGE;
            resp->stim = ZVIL_STIM_TRANGOAL;
            break;
        case ZVIL_STIM_SOAPED:
            stimctxt->res_bubble = ZVIL_BUBBLE_BREAK;
            VSND_queueSound(vil, VSND_TYP_LAUGH);
            break;
        case ZVIL_STIM_GUMMED:
            stimctxt->res_bubble = ZVIL_BUBBLE_BREAK;
            VSND_queueSound(vil, VSND_TYP_LAUGH);
            break;
        case ZVIL_STIM_HEADBUTT:
            stimctxt->res_headbutt = ZVIL_HEADBUTT_DAMAGE;
            break;
        case ZVIL_STIM_BOUNCED:
            stimctxt->res_buttbounce = ZVIL_BOUNCE_NORMAL;
            resp->trangoal = ZVIL_GOAL_DAZED;
            resp->tranmode = ZVIL_MODE_BATTLE;
            resp->stim = ZVIL_STIM_TRANGOAL;
            break;
        case ZVIL_STIM_BUMPED:
            stimctxt->res_bumped = ZVIL_BUMPED_DAMAGE;
            break;
        case ZVIL_STIM_KILLED:
            break;
        case ZVIL_STIM_ATTACKHERO:
            if (vil->allow & 0x8 && vil->abl_atak & 0x1)
            {
                vil->prj_reserved = VWEP_lobReservePRJ(vil);
                if (vil->prj_reserved)
                {
                    resp->trangoal = ZVIL_GOAL_LOBBING;
                    resp->tranmode = ZVIL_MODE_BATTLE;
                    resp->stim = ZVIL_STIM_TRANGOAL;
                }
            }
            break;
        case ZVIL_STIM_DOTAUNT:
            if (vil->ability & 0x100)
            {
                resp->trangoal = ZVIL_GOAL_TAUNT;
                resp->tranmode = ZVIL_MODE_NOCHANGE;
                resp->stim = ZVIL_STIM_TRANGOAL;
            }
            break;
        case ZVIL_STIM_GOTOPOS:
            VOUT_dogoto_pos(vil, &stimctxt->gotopos, stimctxt->off_goto);
            resp->trangoal = ZVIL_GOAL_FOLLOW;
            resp->tranmode = ZVIL_MODE_NOCHANGE;
            resp->stim = ZVIL_STIM_TRANGOAL;
            break;
        case ZVIL_STIM_GOTOWIDGET:
            if (stimctxt->widge && vil->abl_move & 0x100)
            {
                VOUT_dogoto_widget(vil, stimctxt->widge, stimctxt->off_goto, 0);
                vil->tmr_list[ZVIL_TDUR_NAVDELAY] = -1.0f;
                resp->trangoal = ZVIL_GOAL_FOLLOW;
                resp->tranmode = ZVIL_MODE_NOCHANGE;
                resp->stim = ZVIL_STIM_TRANGOAL;
            }
            break;
        case ZVIL_STIM_FOLLOWWIDGET:
            if (stimctxt->widge && vil->abl_move & 0x100)
            {
                std_mvpt_nav = VOUT_dogoto_widget(vil, stimctxt->widge, stimctxt->off_goto, 1);
                vil->tmr_list[ZVIL_TDUR_NAVDELAY] = -1.0f;
                if (std_mvpt_nav)
                {
                    resp->trangoal = ZVIL_GOAL_PATROL;
                    resp->tranmode = ZVIL_MODE_NOCHANGE;
                    resp->stim = ZVIL_STIM_TRANGOAL;
                }
                else
                {
                    resp->trangoal = ZVIL_GOAL_FOLLOW;
                    resp->tranmode = ZVIL_MODE_NOCHANGE;
                    resp->stim = ZVIL_STIM_TRANGOAL;
                }
            }
            break;
        case ZVIL_STIM_STARTPATROL:
            vil->tmr_list[ZVIL_TDUR_NAVDELAY] = -1.0f;
            if (vil->vhf->query(vil, ZVIL_Q_NEEDNAV))
            {
                VNAV_get_nav(vil);
            }
            if (vil->vhf->query(vil, ZVIL_Q_HAVENAV))
            {
                resp->trangoal = ZVIL_GOAL_PATROL;
                resp->tranmode = ZVIL_MODE_NOCHANGE;
                resp->stim = ZVIL_STIM_TRANGOAL;
            }
            else
            {
                resp->trangoal = ZVIL_GOAL_IDLE;
                resp->tranmode = ZVIL_MODE_NOCHANGE;
                resp->stim = ZVIL_STIM_TRANGOAL;
            }
            break;
        case ZVIL_STIM_STOPPATROL:
            resp->trangoal = ZVIL_GOAL_IDLE;
            resp->tranmode = ZVIL_MODE_NOCHANGE;
            resp->stim = ZVIL_STIM_TRANGOAL;
            break;
        case ZVIL_STIM_STARTGUARD:
            resp->trangoal = ZVIL_GOAL_IDLE;
            resp->tranmode = ZVIL_MODE_NOCHANGE;
            resp->stim = ZVIL_STIM_TRANGOAL;
            break;
        case ZVIL_STIM_STOPGUARD:
            resp->trangoal = ZVIL_GOAL_IDLE;
            resp->tranmode = ZVIL_MODE_NOCHANGE;
            resp->stim = ZVIL_STIM_TRANGOAL;
            break;
        }
        break;
    }

    switch (g_pgol = vil->goal)
    {
    case ZVIL_GOAL_NONE:
        break;
    case ZVIL_GOAL_AFTERLIFE:
    case ZVIL_GOAL_BASEINIT:
    case ZVIL_GOAL_STARTUP:
    case ZVIL_GOAL_GOONLINE:
    case ZVIL_GOAL_IDLE:
    case ZVIL_GOAL_FOLLOW:
    case ZVIL_GOAL_PATROL:
    case ZVIL_GOAL_TAUNT:
    case ZVIL_GOAL_ACTION:
    case ZVIL_GOAL_LOBBING:
    case ZVIL_GOAL_BLOCKHEAD:
    case ZVIL_GOAL_TELEPORT:
    case ZVIL_GOAL_KNOCKBACK:
        g_pstm = stim;
        break;
    case ZVIL_GOAL_DAZED:
        switch (g_pstm = stim)
        {
        case ZVIL_STIM_NONE:
            break;
        case ZVIL_STIM_TRANGOAL:
            break;
        case ZVIL_STIM_KILLSELF:
            vil->hitpts = 0;
            resp->trangoal = ZVIL_GOAL_DIE;
            resp->tranmode = ZVIL_MODE_DEATH;
            resp->stim = ZVIL_STIM_TRANGOAL;
            break;
        case ZVIL_STIM_GOTOAFTERLIFE:
            resp->trangoal = ZVIL_GOAL_AFTERLIFE;
            resp->tranmode = ZVIL_MODE_SHUTDOWN;
            resp->stim = ZVIL_STIM_TRANGOAL;
            break;
        case ZVIL_STIM_HURTBOSS:
            resp->trangoal = ZVIL_GOAL_HURT;
            resp->tranmode = ZVIL_MODE_NOCHANGE;
            resp->stim = ZVIL_STIM_TRANGOAL;
            break;
        case ZVIL_STIM_SOAPED:
            stimctxt->res_bubble = ZVIL_BUBBLE_BREAK;
            break;
        case ZVIL_STIM_GUMMED:
            stimctxt->res_bubble = ZVIL_BUBBLE_BREAK;
            break;
        case ZVIL_STIM_HEADBUTT:
            stimctxt->res_headbutt = ZVIL_HEADBUTT_NORMAL;
            resp->trangoal = ZVIL_GOAL_KNOCKBACK;
            resp->tranmode = ZVIL_MODE_NOCHANGE;
            resp->stim = ZVIL_STIM_TRANGOAL;
            break;
        case ZVIL_STIM_BOUNCED:
            stimctxt->res_buttbounce = ZVIL_BOUNCE_NONE;
            break;
        case ZVIL_STIM_BUMPED:
            stimctxt->res_bumped = ZVIL_BUMPED_DAMAGE;
            break;
        case ZVIL_STIM_KILLED:
            break;
        case ZVIL_STIM_DOTAUNT:
            if (vil->ability & 0x100)
            {
                resp->trangoal = ZVIL_GOAL_TAUNT;
                resp->tranmode = ZVIL_MODE_NOCHANGE;
                resp->stim = ZVIL_STIM_TRANGOAL;
            }
            break;
        }
        break;
    }

    return 1;
}

// The Mastermind's goal changes. The standard handler takes the lifeless, starting-up and dying
// goals, and he queues his death sound when he enters the dying goal. On entering a goal he picks
// its animation and sets it up: patrolling without a next navigation point asks for one;
// teleporting hides him and turns off his collisions, zaps the player and shouts when he is
// teleporting away from a blockhead, otherwise trash-talks if his allow bit 0x10 is set, and
// while that bit is set he vanishes in particles with a sound; readying an attack, being blockheaded, dazed
// (vulnerable), knocked back (towards where the player stands), lobbing (lobs the reserved
// flask, one per salvo), taunting and being hurt set their timers and sounds; being hurt costs
// a hit point. Leaving the teleport goal shows him again in particles; leaving the dazed goal
// ends his vulnerability; leaving the knockback goal stops his frame; leaving the lobbing goal
// drops the reserved flask.
static S32 VILN_BMM_trangoal(st_zEntVillain* vil, st_STIMULUS_CONTEXT_DATA* stimctxt, F32 dt)
{
    en_BEHAVIOUR_GOAL procgoal;
    S32 result = 1;
    en_NPC_STIMULUS stim = stimctxt->stim;

    if (stim == ZVIL_STIM_ENTERGOAL || stim == ZVIL_STIM_EXITGOAL)
    {
        procgoal = stimctxt->trangoal;
    }
    else
    {
        procgoal = vil->goal;
    }

    switch (procgoal)
    {
    case ZVIL_GOAL_AFTERLIFE:
    case ZVIL_GOAL_BASEINIT:
    case ZVIL_GOAL_STARTUP:
    case ZVIL_GOAL_GOONLINE:
    case ZVIL_GOAL_DIE:
        result = VILN_std_trangoal(vil, stimctxt, dt);
        if (procgoal == ZVIL_GOAL_DIE && stim == ZVIL_STIM_ENTERGOAL)
        {
            VSND_queueSound(vil, VSND_TYP_MMDIE);
        }
        break;
    }

    switch (g_pgol = procgoal)
    {
    case ZVIL_GOAL_NONE:
        break;
    case ZVIL_GOAL_IDLE:
        switch (g_pstm = stim)
        {
        case ZVIL_STIM_NONE:
            break;
        case ZVIL_STIM_TRANGOAL:
            break;
        case ZVIL_STIM_ENTERGOAL:
            vil->anim_loopmult = 0.97f;
            vil->vhf->pickanim(vil, ZVIL_GOAL_IDLE, ZVIL_GSPOT_START);
            vil->tgt_stat = 0;
            break;
        case ZVIL_STIM_EXITGOAL:
            break;
        }
        break;
    case ZVIL_GOAL_ACTION:
        switch (g_pstm = stim)
        {
        case ZVIL_STIM_NONE:
            break;
        case ZVIL_STIM_TRANGOAL:
            break;
        case ZVIL_STIM_ENTERGOAL:
            vil->anim_loopmult = 0.97f;
            vil->vhf->pickanim(vil, ZVIL_GOAL_ACTION, ZVIL_GSPOT_START);
            break;
        case ZVIL_STIM_EXITGOAL:
            break;
        }
        break;
    case ZVIL_GOAL_FOLLOW:
        switch (g_pstm = stim)
        {
        case ZVIL_STIM_NONE:
            break;
        case ZVIL_STIM_TRANGOAL:
            break;
        case ZVIL_STIM_ENTERGOAL:
            vil->anim_loopmult = 0.97f;
            vil->vhf->pickanim(vil, ZVIL_GOAL_FOLLOW, ZVIL_GSPOT_START);
            break;
        case ZVIL_STIM_EXITGOAL:
            break;
        }
        break;
    case ZVIL_GOAL_PATROL:
        switch (g_pstm = stim)
        {
        case ZVIL_STIM_NONE:
            break;
        case ZVIL_STIM_TRANGOAL:
            break;
        case ZVIL_STIM_ENTERGOAL:
            vil->anim_loopmult = 0.97f;
            vil->vhf->pickanim(vil, ZVIL_GOAL_PATROL, ZVIL_GSPOT_START);
            vil->tgt_stat = 0;
            if (!vil->nav_next)
            {
                vil->need_navpt = 1;
            }
            break;
        case ZVIL_STIM_EXITGOAL:
            break;
        }
        break;
    case ZVIL_GOAL_LEAP:
        switch (g_pstm = stim)
        {
        case ZVIL_STIM_NONE:
            break;
        case ZVIL_STIM_TRANGOAL:
            break;
        case ZVIL_STIM_ENTERGOAL:
            vil->anim_loopmult = 0.97f;
            vil->vhf->pickanim(vil, ZVIL_GOAL_LEAP, ZVIL_GSPOT_START);
            break;
        case ZVIL_STIM_EXITGOAL:
            break;
        }
        break;
    case ZVIL_GOAL_TELEPORT:
        switch (g_pstm = stim)
        {
        case ZVIL_STIM_NONE:
            break;
        case ZVIL_STIM_TRANGOAL:
            break;
        case ZVIL_STIM_ENTERGOAL:
            vil->anim_loopmult = 0.97f;
            vil->vhf->pickanim(vil, ZVIL_GOAL_TELEPORT, ZVIL_GSPOT_START);
            vil->flags &= ~0x1;
            vil->chkby &= 0xEF;
            vil->penby &= 0xEF;
            vil->collis->chk &= 0xEF;
            vil->collis->pen &= 0xEF;
            if (stimctxt->mm_teleport_hurtz_doggie > 0)
            {
                VEFX_mm_zapper(vil, 0);
                VWEP_mm_zapPlayer(vil);
                VSND_queueSound(vil, VSND_TYP_ESCAPING);
            }
            else if (vil->allow & 0x10)
            {
                VSND_queueSound(vil, VSND_TYP_TRASHTALK);
            }
            if (vil->allow & 0x10)
            {
                vil->tmr_list[ZVIL_TYMR_GENERIC] = 0.75f + 0.25f * (iurand() - 0.5f) * 0.75f;
                VSND_queueSound(vil, VSND_TYP_DISAPPEAR);
                VPAR_emit_partype(vil, VPAR_EMIT_TELEPORT, dt);
                VEFX_mm_teleportHide(dt);
            }
            else
            {
                vil->tmr_list[ZVIL_TYMR_GENERIC] = 0.5f;
            }
            vil->tmr_list[ZVIL_TDUR_LOBRELOAD] =
                VCFG_tymref(vil, ZVIL_TDUR_LOBRELOAD) +
                0.25f * (iurand() - 0.5f) * VCFG_tymref(vil, ZVIL_TDUR_LOBRELOAD);
            break;
        case ZVIL_STIM_EXITGOAL:
            VEFX_mm_teleportShow(vil, dt);
            VEFX_mm_zapper(vil, 1);
            VPAR_emit_partype(vil, VPAR_EMIT_TELEPORT, dt);
            VSND_queueSound(vil, VSND_TYP_REAPPEAR);
            vil->flags |= 0x1;
            vil->chkby |= 0x10;
            vil->penby |= 0x10;
            vil->collis->chk |= 0x10;
            vil->collis->pen |= 0x10;
            break;
        }
        break;
    case ZVIL_GOAL_ATTACKREADY:
        switch (g_pstm = stim)
        {
        case ZVIL_STIM_NONE:
            break;
        case ZVIL_STIM_TRANGOAL:
            break;
        case ZVIL_STIM_ENTERGOAL:
            vil->anim_loopmult = 0.97f;
            vil->vhf->pickanim(vil, ZVIL_GOAL_ATTACKREADY, ZVIL_GSPOT_START);
            vil->tmr_list[ZVIL_TYMR_GENERIC] = 0.2f;
            break;
        case ZVIL_STIM_EXITGOAL:
            break;
        }
        break;
    case ZVIL_GOAL_BLOCKHEAD:
        switch (g_pstm = stim)
        {
        case ZVIL_STIM_NONE:
            break;
        case ZVIL_STIM_TRANGOAL:
            break;
        case ZVIL_STIM_ENTERGOAL:
            vil->anim_loopmult = 0.97f;
            vil->vhf->pickanim(vil, ZVIL_GOAL_BLOCKHEAD, ZVIL_GSPOT_START);
            VSND_queueSound(vil, VSND_TYP_BLOCKING);
            vil->tmr_list[ZVIL_TYMR_GENERIC] = 3.0f + 0.25f * (iurand() - 0.5f) * 3.0f;
            break;
        case ZVIL_STIM_EXITGOAL:
            break;
        }
        break;
    case ZVIL_GOAL_DAZED:
        switch (g_pstm = stim)
        {
        case ZVIL_STIM_NONE:
            break;
        case ZVIL_STIM_TRANGOAL:
            break;
        case ZVIL_STIM_ENTERGOAL:
            vil->anim_loopmult = 0.97f;
            vil->vhf->pickanim(vil, ZVIL_GOAL_DAZED, ZVIL_GSPOT_START);
            vil->tmr_list[ZVIL_TDUR_DAZED] =
                VCFG_tymref(vil, ZVIL_TDUR_DAZED) +
                0.25f * (iurand() - 0.5f) * VCFG_tymref(vil, ZVIL_TDUR_DAZED);
            vil->tmr_list[ZVIL_TTIL_RECOVERY] = vil->tmr_list[ZVIL_TDUR_DAZED];
            vil->tmr_list[ZVIL_TTIL_RECOVERY] =
                (vil->tmr_list[ZVIL_TTIL_RECOVERY] + -1.0f < -1.0f)
                    ? -1.0f
                    : vil->tmr_list[ZVIL_TTIL_RECOVERY] + -1.0f;
            vil->abl_vuln |= 0x80;
            break;
        case ZVIL_STIM_EXITGOAL:
            vil->abl_vuln &= ~0x80;
            break;
        }
        break;
    case ZVIL_GOAL_KNOCKBACK:
        switch (g_pstm = stim)
        {
        case ZVIL_STIM_NONE:
            break;
        case ZVIL_STIM_TRANGOAL:
            break;
        case ZVIL_STIM_ENTERGOAL:
            vil->anim_loopmult = 0.97f;
            vil->vhf->pickanim(vil, ZVIL_GOAL_KNOCKBACK, ZVIL_GSPOT_START);
            vil->dst_navTween = 7.5f;
            vil->tmr_list[ZVIL_TYMR_GENERIC] = 1.5f;
            xVec3Copy(&vil->pos_bowling, &xModelGetFrame(globals.player.ent.model)->pos);
            vil->pflags &= 0xFB;
            break;
        case ZVIL_STIM_EXITGOAL:
            VSUP_haltFrame(vil);
            vil->pflags |= 0x4;
            break;
        }
        break;
    case ZVIL_GOAL_LOBBING:
        switch (g_pstm = stim)
        {
        case ZVIL_STIM_NONE:
            break;
        case ZVIL_STIM_TRANGOAL:
            break;
        case ZVIL_STIM_ENTERGOAL:
            vil->anim_loopmult = 0.97f;
            vil->vhf->pickanim(vil, ZVIL_GOAL_LOBBING, ZVIL_GSPOT_START);
            vil->tmr_list[ZVIL_TDUR_BUILDUP] =
                VCFG_tymref(vil, ZVIL_TDUR_BUILDUP) +
                0.25f * (iurand() - 0.5f) * VCFG_tymref(vil, ZVIL_TDUR_BUILDUP);
            VWEP_lobAtPlayer(vil->prj_reserved, vil);
            VSND_queueSound(vil, VSND_TYP_ATTACKBUILD);
            vil->cnt_lobSalvo = 1;
            vil->goodattack = 0;
            break;
        case ZVIL_STIM_EXITGOAL:
            vil->goodattack = 0;
            vil->tmr_list[ZVIL_TDUR_LOBRELOAD] =
                VCFG_tymref(vil, ZVIL_TDUR_LOBRELOAD) +
                0.25f * (iurand() - 0.5f) * VCFG_tymref(vil, ZVIL_TDUR_LOBRELOAD);
            if (vil->prj_reserved)
            {
                vil->prj_reserved->Abort();
            }
            vil->prj_reserved = NULL;
            break;
        }
        break;
    case ZVIL_GOAL_TAUNT:
        switch (g_pstm = stim)
        {
        case ZVIL_STIM_NONE:
            break;
        case ZVIL_STIM_TRANGOAL:
            break;
        case ZVIL_STIM_ENTERGOAL:
            vil->anim_loopmult = 0.97f;
            vil->vhf->pickanim(vil, ZVIL_GOAL_TAUNT, ZVIL_GSPOT_START);
            vil->tmr_list[ZVIL_TDUR_TAUNT] = vil->vilcfg.tymref_list[ZVIL_TDUR_TAUNT];
            VSND_queueSound(vil, VSND_TYP_MMLAUGH);
            break;
        case ZVIL_STIM_EXITGOAL:
            break;
        }
        break;
    case ZVIL_GOAL_HURT:
        switch (g_pstm = stim)
        {
        case ZVIL_STIM_NONE:
            break;
        case ZVIL_STIM_TRANGOAL:
            break;
        case ZVIL_STIM_ENTERGOAL:
            vil->tmr_list[ZVIL_TDUR_HURT] =
                VCFG_tymref(vil, ZVIL_TDUR_HURT) +
                0.25f * (iurand() - 0.5f) * VCFG_tymref(vil, ZVIL_TDUR_HURT);
            if (vil->hitpts > 1)
            {
                vil->anim_loopmult = 0.97f;
                vil->vhf->pickanim(vil, ZVIL_GOAL_HURT, ZVIL_GSPOT_HURT_ALTA);
                VSND_queueSound(vil, VSND_TYP_FOREPLAYHURT);
            }
            else
            {
                vil->anim_loopmult = 0.97f;
                vil->vhf->pickanim(vil, ZVIL_GOAL_HURT, ZVIL_GSPOT_START);
            }
            vil->hitpts--;
            break;
        case ZVIL_STIM_EXITGOAL:
            break;
        }
        break;
    }

    return result;
}

// The Mastermind's movement step, by goal. Idle or taunting, he turns to face the player only if
// allow bit 0x10 is set; dazed, readying an attack or lobbing, he always does. He follows and
// patrols like other villains. Leaping, his move target is his next path point; teleporting, it is
// too and he also faces the player. As a blockhead he faces the player. Knocked back, while the
// knocked animation plays he is carried to his bowling spot at speed 5 and his target height
// changes by 0.35 dt sin(2 pi (1 - t)), t being his time in the goal over 1.5 s capped at 1;
// otherwise his frame stops.
static S32 VILN_BMM_calcmove(st_zEntVillain* vil, _xScene* scn, F32 dt, const _xEntFrame* frame)
{
    F32 fv;

    switch (vil->goal)
    {
    case ZVIL_GOAL_AFTERLIFE:
    case ZVIL_GOAL_BASEINIT:
    case ZVIL_GOAL_STARTUP:
    case ZVIL_GOAL_GOONLINE:
    case ZVIL_GOAL_ACTION:
        break;
    case ZVIL_GOAL_IDLE:
    case ZVIL_GOAL_TAUNT:
        if (!(vil->allow & 0x10))
        {
            break;
        }
    case ZVIL_GOAL_DAZED:
    case ZVIL_GOAL_ATTACKREADY:
    case ZVIL_GOAL_LOBBING:
        vil->ang_moveto = vil->precalc.ang_VtoP;
        vil->domove |= 0x40;
        vil->domove |= 0x1;
        vil->domove |= 0x10;
        vil->domove |= 0x80000000;
        break;
    case ZVIL_GOAL_FOLLOW:
        VOUT_follow_move(vil, scn, dt, frame);
        VOUT_chkdo_followArrived(vil);
        break;
    case ZVIL_GOAL_PATROL:
        VOUT_patrol_move(vil, scn, dt, frame);
        VOUT_chkdo_patrolArrived(vil);
        break;
    case ZVIL_GOAL_LEAP:
        xVec3Copy(&vil->pos_moveto, zMovePointGetPos(vil->nav_next));
        vil->domove |= 0x800;
        vil->domove |= 0xA;
        vil->domove |= 0x4;
        vil->domove |= 0x80000000;
        break;
    case ZVIL_GOAL_TELEPORT:
        xVec3Copy(&vil->pos_moveto, zMovePointGetPos(vil->nav_next));
        vil->ang_moveto = vil->precalc.ang_VtoP;
        vil->domove |= 0x800;
        vil->domove |= 0xA;
        vil->domove |= 0x4;
        vil->domove |= 0x40;
        vil->domove |= 0x40000000;
        vil->domove |= 0x20000000;
        vil->domove |= 0x80000000;
        break;
    case ZVIL_GOAL_BLOCKHEAD:
        vil->ang_moveto = vil->precalc.ang_VtoP;
        vil->domove |= 0x40;
        vil->domove |= 0x1;
        vil->domove |= 0x10;
        vil->domove |= 0x20000000;
        vil->domove |= 0x80000000;
        break;
    case ZVIL_GOAL_KNOCKBACK:
        if (vil->anim_playnext == ZVIL_ANIM_MM_BA_KNOCKED_BEGIN ||
            vil->anim_playnext == ZVIL_ANIM_MM_BA_KNOCKED_LOOP)
        {
            xVec3Copy(&vil->pos_moveto, &vil->pos_bowling);
            vil->spd_moveto = 5.0f;

            fv = vil->tmr_list[ZVIL_TYMR_CURGOAL] / 1.5f;
            fv = 1.0f - MIN(fv, 1.0f);
            fv *= 2 * PI;
            fv = sin(fv);
            vil->pos_moveto.y += 0.35f * dt * fv;

            vil->domove |= 0x8000;
            vil->domove |= 0x4;
            vil->domove |= 0x800;
            vil->domove |= 0xA;
            vil->domove |= 0x20;
            vil->domove |= 0x80000000;
        }
        else
        {
            VSUP_haltFrame(vil);
        }
        break;
    case ZVIL_GOAL_HURT:
    case ZVIL_GOAL_DIE:
        break;
    }

    return 1;
}

// The Mastermind's respond step adds up the push-out vectors of his NPC, environment and static
// collisions that hit an object; the sum is not used.
static S32 VILN_BMM_respond(st_zEntVillain* vil, st_STIMULUS_CONTEXT_DATA*)
{
    _xVec3 vec_sum = { 0 };
    _xCollis* colrec;
    S32 i;

    for (i = vil->collis->npc_sidx; i < vil->collis->npc_eidx; i++)
    {
        colrec = &vil->collis->colls[i];
        if (colrec->optr)
        {
            xVec3Add(&vec_sum, &vec_sum, &colrec->depen);
        }
    }

    for (i = vil->collis->env_sidx; i < vil->collis->env_eidx; i++)
    {
        colrec = &vil->collis->colls[i];
        if (colrec->optr)
        {
            xVec3Add(&vec_sum, &vec_sum, &colrec->depen);
        }
    }

    for (i = vil->collis->stat_sidx; i < vil->collis->stat_eidx; i++)
    {
        colrec = &vil->collis->colls[i];
        if (colrec->optr)
        {
            xVec3Add(&vec_sum, &vec_sum, &colrec->depen);
        }
    }

    return 1;
}

// Picks the Mastermind's animation for a goal and where he is in it. He is in battle mode when he
// may teleport (allow bit 0x4) or attack from range (allow bit 0x10), and then idles, walks, acts
// and taunts with his battle animations.
static S32 VILN_BMM_pickanim(st_zEntVillain* vil, en_BEHAVIOUR_GOAL goal, en_ZVIL_GOAL_SPOT gspot)
{
    S32 battle_mode;

    battle_mode = (vil->allow & 0x4) ? 1 : 0;
    battle_mode |= (vil->allow & 0x10) ? 1 : 0;

    switch (goal)
    {
    case ZVIL_GOAL_AFTERLIFE:
    case ZVIL_GOAL_BASEINIT:
    case ZVIL_GOAL_STARTUP:
    case ZVIL_GOAL_GOONLINE:
        VANM_setone(vil, ZVIL_ANIM_IDLE);
        break;
    case ZVIL_GOAL_IDLE:
        if (battle_mode)
        {
            VANM_setone(vil, ZVIL_ANIM_MM_BA_IDLE);
        }
        else
        {
            VANM_setone(vil, ZVIL_ANIM_IDLE);
        }
        break;
    case ZVIL_GOAL_ACTION:
        if (battle_mode)
        {
            VANM_setone(vil, ZVIL_ANIM_MM_BA_TAUNT);
        }
        else
        {
            VANM_setone(vil, ZVIL_ANIM_MM_FP_ACTION);
        }
        break;
    case ZVIL_GOAL_FOLLOW:
        if (battle_mode)
        {
            VANM_setone(vil, ZVIL_ANIM_MM_BA_MOVE);
        }
        else
        {
            VANM_setone(vil, ZVIL_ANIM_MM_FP_MOVE);
        }
        break;
    case ZVIL_GOAL_PATROL:
        if (battle_mode)
        {
            VANM_setone(vil, ZVIL_ANIM_MM_BA_MOVE);
        }
        else
        {
            VANM_setone(vil, ZVIL_ANIM_MM_FP_MOVE);
        }
        break;
    case ZVIL_GOAL_LEAP:
        VANM_setone(vil, ZVIL_ANIM_MM_FP_JUMP);
        break;
    case ZVIL_GOAL_TELEPORT:
        if (battle_mode)
        {
            VANM_setone(vil, ZVIL_ANIM_MM_BA_IDLE);
        }
        else
        {
            VANM_setone(vil, ZVIL_ANIM_MM_BA_IDLE);
        }
        break;
    case ZVIL_GOAL_TAUNT:
        if (battle_mode)
        {
            VANM_randthree(vil, ZVIL_ANIM_MM_BA_TAUNT, ZVIL_ANIM_MM_FP_TAUNT,
                           ZVIL_ANIM_MM_BA_REACT);
        }
        else
        {
            VANM_setone(vil, ZVIL_ANIM_MM_FP_TAUNT);
        }
        break;
    case ZVIL_GOAL_ATTACKREADY:
        if (gspot == ZVIL_GSPOT_START)
        {
            VANM_setone(vil, ZVIL_ANIM_MM_BA_IDLE);
        }
        else if (gspot == ZVIL_GSPOT_MOVERIGHT)
        {
            VANM_setone(vil, ZVIL_ANIM_MM_BA_TURN);
        }
        break;
    case ZVIL_GOAL_BLOCKHEAD:
        if (gspot == ZVIL_GSPOT_START)
        {
            VANM_setone(vil, ZVIL_ANIM_MM_BA_SHIELD_BEGIN);
        }
        else if (gspot == ZVIL_GSPOT_RESUME)
        {
            VANM_setone(vil, ZVIL_ANIM_MM_BA_SHIELD_LOOP);
        }
        else if (gspot == ZVIL_GSPOT_STATERECOVER)
        {
            VANM_setone(vil, ZVIL_ANIM_MM_BA_SHIELD_END);
        }
        break;
    case ZVIL_GOAL_DAZED:
        if (gspot == ZVIL_GSPOT_START)
        {
            VANM_setone(vil, ZVIL_ANIM_MM_BA_STUNNED_BEGIN);
        }
        else if (gspot == ZVIL_GSPOT_RESUME)
        {
            VANM_setone(vil, ZVIL_ANIM_MM_BA_STUNNED_LOOP);
        }
        else if (gspot == ZVIL_GSPOT_STATERECOVER)
        {
            VANM_setone(vil, ZVIL_ANIM_MM_BA_STUNNED_END);
        }
        break;
    case ZVIL_GOAL_LOBBING:
        if (gspot == ZVIL_GSPOT_START)
        {
            VANM_setone(vil, ZVIL_ANIM_MM_BA_ATTACK_BEGIN);
        }
        else if (gspot == ZVIL_GSPOT_ATTACKLINGER)
        {
            VANM_setone(vil, ZVIL_ANIM_MM_BA_ATTACK_LOOP);
        }
        else if (gspot == ZVIL_GSPOT_KICKATTACK)
        {
            VANM_setone(vil, ZVIL_ANIM_MM_BA_ATTACK);
        }
        break;
    case ZVIL_GOAL_KNOCKBACK:
        if (gspot == ZVIL_GSPOT_START)
        {
            VANM_setone(vil, ZVIL_ANIM_MM_BA_KNOCKED_BEGIN);
        }
        else if (gspot == ZVIL_GSPOT_RESUME)
        {
            VANM_setone(vil, ZVIL_ANIM_MM_BA_KNOCKED_LOOP);
        }
        else if (gspot == ZVIL_GSPOT_STATERECOVER)
        {
            VANM_setone(vil, ZVIL_ANIM_MM_BA_KNOCKED_END);
        }
        break;
    case ZVIL_GOAL_HURT:
        if (gspot == ZVIL_GSPOT_START)
        {
            VANM_setone(vil, ZVIL_ANIM_MM_BA_DEATH_BEGIN);
        }
        else if (gspot == ZVIL_GSPOT_HURT_ALTA)
        {
            VANM_setone(vil, ZVIL_ANIM_IDLE);
        }
        break;
    case ZVIL_GOAL_DIE:
        VANM_setone(vil, ZVIL_ANIM_MM_BA_DEATH);
        break;
    }

    return 1;
}

// Per-frame update callback. Updates the villain's timers. If it is playing an animation and is
// neither Shaggy nor a Witch, it compares its model's current animation state with the one it asked
// for: if they differ it stops counting as playing and bit 0x100 of the model's flags is set,
// otherwise that bit is cleared; in every other case the bit is set. It then calls the type's
// update query (the answer is not used), gives the type the precalc notice, lets it look at the
// scene, runs the entity update (physics and movement when its flags allow them, force fields, and
// collision when it has a collision record) and lets the type respond; when the precalc, aware or
// respond step answers with a stimulus, the villain changes goal. Finally it sets the precalc status
// to 0x40000000, reads the base model scale from its configuration, gives the type the old-time
// notice, sets the status to 0x20000000 and processes its sound queue.
static void zVIL_elcb_update(_xEnt* xe, _xScene* scn, F32 dt)
{
    st_STIMULUS_CONTEXT_DATA stimresp = { ZVIL_STIM_NONE };
    st_zEntVillain* ent = (st_zEntVillain*)xe;

    zVIL_upd_timers(ent, dt);

    if (ent->anim_playing && !zEntIsShaggy(ent) && ent->subType != eEntVillainTypeWitch)
    {
        xAnimState* state = VANM_entState(ent);
        if (state->ID != g_anamhash[ent->anim_playnext])
        {
            ent->anim_playing = 0;
            ent->model->Flags |= 0x100;
        }
        else
        {
            ent->model->Flags &= (U16)~0x100;
        }
    }
    else
    {
        ent->model->Flags |= 0x100;
    }

    ent->vhf->query(ent, ZVIL_Q_DOUPDATE);

    memset(&stimresp, 0, sizeof(stimresp));
    stimresp.stim = ZVIL_STIM_NONE;
    ent->vhf->notice(ent, ZVIL_NOTE_PRECALC, &stimresp);
    if (stimresp.stim)
    {
        zVIL_trangoal(ent, &stimresp, dt);
    }

    memset(&stimresp, 0, sizeof(stimresp));
    stimresp.stim = ZVIL_STIM_NONE;
    ent->vhf->aware(ent, scn, dt, &stimresp);
    if (stimresp.stim)
    {
        zVIL_trangoal(ent, &stimresp, dt);
    }

    xEntBeginUpdate(ent, scn, dt);
    if (ent->pflags & 0x2)
    {
        xEntApplyPhysics(ent, scn, dt);
    }
    if (ent->pflags & 0x1)
    {
        xEntMove(ent, scn, dt);
    }
    xFFXApply(ent, scn, dt);
    if (ent->collis)
    {
        xEntCollide(ent, scn, dt);
    }
    xEntEndUpdate(ent, scn, dt);

    memset(&stimresp, 0, sizeof(stimresp));
    stimresp.stim = ZVIL_STIM_NONE;
    ent->vhf->respond(ent, &stimresp);
    if (stimresp.stim)
    {
        zVIL_trangoal(ent, &stimresp, dt);
    }

    ent->precalc.flg_status = 0x40000000;
    VCFG_getParm(&ent->vilcfg, ZVIL_PARM_MDLSCL_BASE, &ent->model->Scale);

    memset(&stimresp, 0, sizeof(stimresp));
    stimresp.stim = ZVIL_STIM_NONE;
    ent->vhf->notice(ent, ZVIL_NOTE_OLDTIME, &stimresp);

    ent->precalc.flg_status = 0x20000000;
    VSND_procQueue(ent, dt);
}

// Event callback: the villain's type handles the event, and if it answers with a stimulus, that
// stimulus is passed on to the villain. Always returns 1.
static S32 zVIL_elcb_event(xBase* from, xBase* to, U32 toEvent, F32* toParam, xBase* toParamWidget)
{
    st_zEntVillain* ent = (st_zEntVillain*)to;
    st_STIMULUS_CONTEXT_DATA npcresp = { ZVIL_STIM_NONE };

    ent->vhf->sysevent(ent, from, toEvent, toParam, toParamWidget, &npcresp);
    if (npcresp.stim)
    {
        zVIL_input_stim(ent, npcresp.stim, &npcresp);
    }

    return 1;
}

// Bound callback: centres the villain's collision sphere on the given position plus its type's
// offset, turned with the model, and sets the radius from its type. The second sphere's radius is
// always set from its type too; when it is above 0, that sphere's centre is placed the same way with
// its own offset (its bound is not updated).
static void zVIL_elcb_bupdate(_xEnt* xe, _xVec3* pos_bound)
{
    _xVec3 offset = { 0 };
    st_zEntVillain* ent = (st_zEntVillain*)xe;

    offset = g_npcTable[ent->asset->subtype].vec_bndoff;
    xMat3x3RMulVec(&offset, (xMat3x3*)ent->model->Mat, &offset);
    xVec3Add(&ent->bound.sph.center, &offset, pos_bound);
    ent->bound.sph.r = g_npcTable[ent->asset->subtype].rad_bound;
    xBoundUpdate(&ent->bound);

    ent->bound2.sph.r = g_npcTable[ent->asset->subtype].rad_bound2;
    if (ent->bound2.sph.r > 0.0f)
    {
        offset = g_npcTable[ent->asset->subtype].vec_bnd2off;
        xMat3x3RMulVec(&offset, (xMat3x3*)ent->model->Mat, &offset);
        xVec3Add(&ent->bound2.sph.center, &offset, pos_bound);
    }
}

// Movement callback: the villain's type works out its move, then the villain makes it if it has a
// move to make.
static void zVIL_elcb_move(_xEnt* xe, _xScene* scn, F32 dt, _xEntFrame* frame)
{
    st_zEntVillain* ent = (st_zEntVillain*)xe;

    ent->vhf->calcmove(ent, scn, dt, frame);
    if (ent->domove)
    {
        zVIL_apply_move(ent, scn, dt, frame);
    }
}

// Sets the villain's pursuit home from the widget it protects. A protected entity's position
// becomes home. A move point's position is stored but then replaced, as with no widget: the
// villain's own position becomes home and the keep-away distance is cleared. Any other kind of
// widget is dropped (its and the villain's names are looked up and not used) before that.
// Always returns 1.
static S32 VOUT_apply_anchor(st_zEntVillain* vil)
{
    S32 have_anchor = 0;

    if (vil->tgt_protect)
    {
        switch (vil->tgt_protect->baseType)
        {
        case 13:
            xVec3Copy(&vil->pos_pursue_home, zMovePointGetPos((_zMovePoint*)vil->tgt_protect));
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
            have_anchor = 1;
            xVec3Copy(&vil->pos_pursue_home, &xModelGetFrame(vil->tgt_protect->model)->pos);
            break;
        default:
            xSceneID2Name(globals.sceneCur, vil->id);
            xSceneID2Name(globals.sceneCur, vil->tgt_protect->id);
            vil->tgt_protect = NULL;
            have_anchor = 0;
            vil->ds2_goto_keepaway = 0.0f;
            break;
        }
    }

    if (!have_anchor)
    {
        have_anchor = 1;
        xVec3Copy(&vil->pos_pursue_home, &xModelGetFrame(vil->model)->pos);
        vil->ds2_goto_keepaway = 0.0f;
    }
    return have_anchor;
}

// Headbutt response for a villain with spikes: none unless spike ability bit 0x1 is set.
// Otherwise the player's quadrant around the villain picks the first spike bit to test: front 0x2,
// back 0x4, left 0x8, right 0x10; that bit or any later one in this list gives
// ZVIL_HEADBUTT_DAMAGE.
static en_ZVILLAIN_HEADBUTT_RESPONSE VOUT_spikey_header(st_zEntVillain* ent)
{
    en_NPC_QUADRANT quad = ZVIL_QUAD_UNKNOWN;
    en_ZVILLAIN_HEADBUTT_RESPONSE result = ZVIL_HEADBUTT_NONE;

    if (!(ent->abl_spyk & 1))
    {
        return ZVIL_HEADBUTT_NONE;
    }

    VSUP_ent_AOI(ent, &globals.player.ent, &quad);
    switch (quad)
    {
    case ZVIL_QUAD_FRONT:
        if (ent->abl_spyk & 2)
        {
            result = ZVIL_HEADBUTT_DAMAGE;
        }
    case ZVIL_QUAD_BACK:
        if (ent->abl_spyk & 4)
        {
            result = ZVIL_HEADBUTT_DAMAGE;
        }
    case ZVIL_QUAD_LEFT:
        if (ent->abl_spyk & 8)
        {
            result = ZVIL_HEADBUTT_DAMAGE;
        }
    case ZVIL_QUAD_RIGHT:
        if (ent->abl_spyk & 0x10)
        {
            result = ZVIL_HEADBUTT_DAMAGE;
        }
    }
    return result;
}

// Picks the villain's next navigation point and returns whether it has one. It restarts the
// check-for-nav timer, and picks none while the nav delay runs; otherwise, with no previous point
// it starts at its first point (when that point is on), and from there follows the path from its
// current point (or its previous one). A new point also clears need_navpt and records the
// distance to it from the previous point, or from the villain when the point is on a spline.
static S32 VNAV_get_nav(st_zEntVillain* ent)
{
    _zMovePoint* nav_tmp = NULL;

    ent->tmr_list[ZVIL_TTIL_CHKFORNAV] = ent->vilcfg.tymref_list[ZVIL_TTIL_CHKFORNAV];
    if (!(ent->tmr_list[ZVIL_TDUR_NAVDELAY] <= 0.0f))
    {
        nav_tmp = NULL;
    }
    else if (ent->nav_prev == NULL)
    {
        if (zMovePointIsOn(ent->nav_mvptlist))
        {
            ent->nav_prev = ent->nav_mvptlist;
            nav_tmp = ent->nav_mvptlist;
        }
        else
        {
            nav_tmp = NULL;
        }
    }
    else if (ent->nav_prev && ent->nav_next)
    {
        zMovePointGetNext(ent->nav_next, ent->nav_prev, &nav_tmp, NULL);
    }
    else if (ent->nav_prev)
    {
        zMovePointGetNext(ent->nav_prev, NULL, &nav_tmp, NULL);
    }

    if (nav_tmp)
    {
        ent->need_navpt = 0;
        _xVec3 vec_trav = { 0 };
        if (ent->nav_prev)
        {
            xVec3Sub(&vec_trav, zMovePointGetPos(nav_tmp), zMovePointGetPos(ent->nav_prev));
            ent->dst_navTween = xVec3Length(&vec_trav);
        }
        else if (nav_tmp->spl)
        {
            xVec3Sub(&vec_trav, zMovePointGetPos(nav_tmp), (_xVec3*)&ent->model->Mat->pos);
            ent->dst_navTween = xVec3Length(&vec_trav);
        }
    }

    if (ent->nav_next)
    {
        ent->nav_prev = ent->nav_next;
    }
    ent->nav_next = nav_tmp;
    return ent->nav_next ? 1 : 0;
}

// Navigation line-of-sight test: whether nothing in the scene blocks a ray from mypos to the move
// point, starting just outside the villain's (usrdata) bound and ending at the point.
static S32 VNAV_chklos_timz(void* usrdata, _xVec3* mypos, const _zMovePoint* mvpt)
{
    S32 result;
    _xRay3 r;

    r.min_t = 0.001f + ((st_zEntVillain*)usrdata)->bound.sph.r;
    xVec3Sub(&r.dir, zMovePointGetPos(mvpt), mypos);
    r.max_t = xVec3Normalize(&r.dir, &r.dir);
    xVec3SMul(&r.origin, &r.dir, r.min_t);
    xVec3AddTo(&r.origin, mypos);
    r.max_t -= r.min_t;
    r.flags = 0x800;
    result = !xRayHitsSceneFast(&r, globals.sceneCur);
    return result;
}

// The villain's move speed for the frame, base times the speed that fits its current animation:
// 5 flat for an animation with type bit 0x4000000, the move speed while gummed with move ability
// 0x100000, five times base for a move type of 10, the chase speed for 6, and the move speed for
// 4, where the boss Black Knight adds the gap between his chase and move speeds, times 0.66,
// 0.33 or 0 at 2, 3 and 4 hit points (in full otherwise). An idle animation (1) keeps base and
// anything else gives 0. With misc flags 0x2 (move), 0x4 (chase) and 0x1 (idle) the animation's
// playback speed follows.
static F32 VNAV_get_movespeed(st_zEntVillain* ent, F32 base)
{
    F32 rate = base;
    F32 fv;

    if (VANM_entStateType(ent) & 0x4000000)
    {
        rate = 5.0f;
    }
    else if (ent->goal == ZVIL_GOAL_GUMMED && (ent->abl_move & 0x100000))
    {
        rate *= ent->spd_move;
        if (ent->flg_misc & 0x2)
        {
            VANM_setSpeed(ent, rate, ent->sanim_move);
        }
    }
    else if (VANM_entMoveType(ent) == 10)
    {
        rate *= 5.0f;
    }
    else if (VANM_entMoveType(ent) == 6)
    {
        rate *= ent->spd_chase;
        if (ent->flg_misc & 0x4)
        {
            VANM_setSpeed(ent, rate, ent->sanim_chase);
        }
    }
    else if (VANM_entMoveType(ent) == 4)
    {
        if (ent->asset->subtype == eEntVillainBossBlackKnight)
        {
            fv = fabsf(ent->spd_chase - ent->spd_move);
            switch (ent->hitpts)
            {
            case 0:
            case 1:
                break;
            case 2:
                fv *= 0.66f;
                break;
            case 3:
                fv *= 0.33f;
                break;
            case 4:
                fv *= 0.0f;
                break;
            }
            rate *= ent->spd_move + fv;
        }
        else
        {
            rate *= ent->spd_move;
        }
        if (ent->flg_misc & 0x2)
        {
            VANM_setSpeed(ent, rate, ent->sanim_move);
        }
    }
    else if (VANM_entMoveType(ent) == 1)
    {
        if (ent->flg_misc & 0x1 && ent->anim_playing && !(ent->sanim_idle.x < 0.001f))
        {
            xAnimSingle* single = VANM_entSingle(ent);
            if (single)
            {
                single->CurrentSpeed = ent->sanim_idle.x;
            }
        }
    }
    else
    {
        rate = 0.0f;
    }

    return rate;
}

// Picks the Mastermind's next teleport point. Unless forced, he picks none while the player is more
// than 4 units away. For each point it measures the player's squared distance to the point and,
// meant as the villain's own distance to it, the squared flat distance between the villain and a
// zero vector, so the same value for every point; it takes the point farthest from the player
// when that second value is above 1. The random choice then always replaces it: one time in five
// a dangerous point (weights 0.2, 0.3, 0.3, 0.2), otherwise a safe one (weights 1, 2, 2, 1, 2, 1, 1, 2).
// When forced with no point found, it falls back to his first navigation point and clears his
// next one; when forced and the point is his next navigation point, that is cleared.
static _zMovePoint* VNAV_find_telepoint(st_zEntVillain* vil, S32 force)
{
    _zMovePoint* da_tp = NULL;
    S32 idx_best = -1;
    S32 i;
    F32 ds2_toScoob[12] = { 0 };
    F32 ds2_toSelf[12] = { 0 };
    _xVec3 vec_a = { 0 };
    _xVec3 vec_b = { 0 };

    if (!force && vil->precalc.ds2_VtoP > 16.0f)
    {
        return NULL;
    }

    for (i = 0; i < 12; i++)
    {
        if (g_mmtp[i] == NULL)
        {
            ds2_toScoob[i] = -1.0f;
            ds2_toSelf[i] = -1.0f;
            continue;
        }

        ds2_toSelf[i] = VSUP_ds2_toPos(vil, vec_a);

        xVec3Copy(&vec_b, &xModelGetFrame(globals.player.ent.model)->pos);
        xVec3Sub(&vec_b, &vec_b, zMovePointGetPos(g_mmtp[i]));
        ds2_toScoob[i] = xVec3Length2(&vec_b);
    }

    for (i = 0; i < 12; i++)
    {
        if (ds2_toScoob[i] < 0.0f) continue;
        if (ds2_toSelf[i] < 0.0f) continue;

        if (idx_best < 0) idx_best = i;
        if (ds2_toScoob[i] > ds2_toScoob[idx_best]) idx_best = i;
    }

    if (idx_best >= 0 && ds2_toSelf[idx_best] > 1.0f)
    {
        da_tp = g_mmtp[idx_best];
    }

    static S32 do_rand_select = 1;
    if (do_rand_select)
    {
        S32 choice;
        S32 dngrlist[4] = { 8, 9, 10, 11 };
        S32 safelist[8] = { 0, 1, 2, 3, 4, 5, 6, 7 };
        F32 dngr_wt[4] = { 0.2f, 0.3f, 0.3f, 0.2f };
        F32 safe_wt[12] = { 1.0f, 2.0f, 2.0f, 1.0f, 2.0f, 1.0f, 1.0f, 2.0f };

        VSUP_wtadjust(safe_wt, 8, 1.0f);
        if (xUtil_yesno(0.2f))
        {
            choice = VSUP_choose(dngrlist, dngr_wt, 4);
            da_tp = g_mmtp[choice];
        }
        else
        {
            choice = VSUP_choose(safelist, safe_wt, 8);
            da_tp = g_mmtp[choice];
        }
    }

    if (da_tp == NULL && force)
    {
        for (i = 0; i < 12; i++)
        {
            if (g_mmtp[i]) break;
        }

        da_tp = vil->nav_mvptlist;
        vil->nav_next = NULL;
    }
    else if (force && da_tp && da_tp == vil->nav_next)
    {
        vil->nav_next = NULL;
    }

    return da_tp;
}

// Gets the Mastermind ready to teleport: remembers where he stands, makes his next navigation
// point his previous one, and looks for a teleport point. A point other than his next one becomes
// his next one, and he teleports; otherwise he needs a navigation point. Either way his
// navigation check timer restarts at its configured time, plus or minus 12.5 percent.
static S32 VNAV_setup_telepoint(st_zEntVillain* vil, S32 force)
{
    S32 do_teleport = 0;
    _zMovePoint* telepoint;

    xVec3Copy(&sMMPreTeleportPos, &xModelGetFrame(vil->model)->pos);

    if (vil->nav_next && vil->nav_next != vil->nav_prev)
    {
        vil->nav_prev = vil->nav_next;
    }

    telepoint = VNAV_find_telepoint(vil, force);
    if (telepoint && telepoint != vil->nav_next)
    {
        vil->nav_next = telepoint;
        do_teleport = 1;
        vil->need_navpt = 0;
        vil->tmr_list[ZVIL_TTIL_CHKFORNAV] =
            VCFG_tymref(vil, ZVIL_TTIL_CHKFORNAV) +
            0.25f * (iurand() - 0.5f) * VCFG_tymref(vil, ZVIL_TTIL_CHKFORNAV);
    }
    else
    {
        vil->need_navpt = 1;
        vil->tmr_list[ZVIL_TTIL_CHKFORNAV] =
            VCFG_tymref(vil, ZVIL_TTIL_CHKFORNAV) +
            0.25f * (iurand() - 0.5f) * VCFG_tymref(vil, ZVIL_TTIL_CHKFORNAV);
    }

    return do_teleport;
}

// Passes a stimulus to the villain's type, unless it is on standby and its standby time has not
// run out. If the type answers with a stimulus of its own, the villain changes goal to suit it.
static void zVIL_input_stim(st_zEntVillain* ent, en_NPC_STIMULUS, st_STIMULUS_CONTEXT_DATA* stimctxt)
{
    st_STIMULUS_CONTEXT_DATA stimresp = { ZVIL_STIM_NONE };

    if (!ent->standby || (ent->standby && ent->tmr_list[ZVIL_TDUR_STDBY] <= 0.0f))
    {
        stimresp = *stimctxt;
        stimresp.stim = ZVIL_STIM_NONE;
        ent->vhf->stimulus(ent, stimctxt, &stimresp);
        if (stimresp.stim)
        {
            zVIL_trangoal(ent, &stimresp, globals.update_dt);
        }
    }
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


// Counts a villain timer down by dt, stopping at -1.
static inline void VTMR_countDown(F32* tmr, F32 dt)
{
    *tmr = (*tmr - dt < -1.0f) ? -1.0f : *tmr - dt;
}

// Runs the villain's timers for a frame. The standby, animation, generic, reload, navigation,
// sound, voice and sneak timers always count down, and the goal timer counts up. The rest count
// down only in the goals that use them: guarding the look timer and then the idle ones; idle the
// boredom timer and then the bored ones; bored the fidget, navigation-check, navigation-delay and
// quip timers; patrolling, mini-patrolling and following the navigation timers; talking the
// fidget timer; looking, listening and investigating their own durations; taunting the taunt
// duration; readying an attack the alert, taunt and next-chase timers; chasing the
// navigation-check timer; lobbing and casting the extend and build-up timers and then the
// post-attack window, which a close attack also counts; dazed, soaped and gummed their durations
// with the fidget and recovery timers (gummed also the minimum-gum window); hurt the hurt
// duration; dying the decay duration. Every count-down stops at -1.
static void zVIL_upd_timers(st_zEntVillain* ent, F32 dt)
{
    VTMR_countDown(&ent->tmr_list[ZVIL_TDUR_STDBY], dt);
    VTMR_countDown(&ent->tmr_list[ZVIL_TYMR_MINANIMPLAY], dt);
    ent->tmr_list[ZVIL_TYMR_CURGOAL] = (ent->tmr_list[ZVIL_TYMR_CURGOAL] + dt < -1.0f) ?
                                           -1.0f :
                                           ent->tmr_list[ZVIL_TYMR_CURGOAL] + dt;
    VTMR_countDown(&ent->tmr_list[ZVIL_TYMR_GENERIC], dt);
    VTMR_countDown(&ent->tmr_list[ZVIL_TDUR_LOBRELOAD], dt);
    VTMR_countDown(&ent->tmr_list[ZVIL_TDUR_EXTRELOAD], dt);
    VTMR_countDown(&ent->tmr_list[ZVIL_TDUR_NAVDELAY], dt);
    VTMR_countDown(&ent->tmr_list[ZVIL_TTIL_ANIMSOUND], dt);
    VTMR_countDown(&ent->tmr_list[ZVIL_TTIL_NEXTVOICE], dt);
    VTMR_countDown(&ent->tmr_list[ZVIL_TWIN_IGNORESNEAK], dt);

    switch (ent->goal)
    {
    case ZVIL_GOAL_AFTERLIFE:
    case ZVIL_GOAL_BASEINIT:
    case ZVIL_GOAL_STARTUP:
    case ZVIL_GOAL_GOONLINE:
        break;
    case ZVIL_GOAL_GUARD:
        VTMR_countDown(&ent->tmr_list[ZVIL_TDUR_LOOK], dt);
    case ZVIL_GOAL_IDLE:
        VTMR_countDown(&ent->tmr_list[ZVIL_TTIL_BORED], dt);
    case ZVIL_GOAL_BORED:
        VTMR_countDown(&ent->tmr_list[ZVIL_TTIL_FIDGET], dt);
        VTMR_countDown(&ent->tmr_list[ZVIL_TTIL_CHKFORNAV], dt);
        VTMR_countDown(&ent->tmr_list[ZVIL_TDUR_NAVDELAY], dt);
        VTMR_countDown(&ent->tmr_list[ZVIL_TTIL_QUIP_OK], dt);
        break;
    case ZVIL_GOAL_PATROL:
    case ZVIL_GOAL_MINIPATROL:
    case ZVIL_GOAL_FOLLOW:
        VTMR_countDown(&ent->tmr_list[ZVIL_TTIL_CHKFORNAV], dt);
        VTMR_countDown(&ent->tmr_list[ZVIL_TDUR_NAVDELAY], dt);
        break;
    case ZVIL_GOAL_TALK:
        VTMR_countDown(&ent->tmr_list[ZVIL_TTIL_FIDGET], dt);
        break;
    case ZVIL_GOAL_LOOKFORHERO:
        VTMR_countDown(&ent->tmr_list[ZVIL_TDUR_LOOK], dt);
        break;
    case ZVIL_GOAL_LISTEN:
        VTMR_countDown(&ent->tmr_list[ZVIL_TDUR_HEAR], dt);
        break;
    case ZVIL_GOAL_INVESTIGATE:
        VTMR_countDown(&ent->tmr_list[ZVIL_TDUR_SEARCH], dt);
        break;
    case ZVIL_GOAL_TAUNT:
        VTMR_countDown(&ent->tmr_list[ZVIL_TDUR_TAUNT], dt);
        break;
    case ZVIL_GOAL_ATTACKREADY:
    case ZVIL_GOAL_BULLREADY:
        VTMR_countDown(&ent->tmr_list[ZVIL_TDUR_ALERT], dt);
        VTMR_countDown(&ent->tmr_list[ZVIL_TTIL_TAUNT], dt);
        VTMR_countDown(&ent->tmr_list[ZVIL_TTIL_NEXTCHASE], dt);
        break;
    case ZVIL_GOAL_CHASE:
        VTMR_countDown(&ent->tmr_list[ZVIL_TTIL_CHKFORNAV], dt);
        break;
    case ZVIL_GOAL_LOBBING:
    case ZVIL_GOAL_SPELLCAST:
        VTMR_countDown(&ent->tmr_list[ZVIL_TDUR_EXTEND], dt);
        VTMR_countDown(&ent->tmr_list[ZVIL_TDUR_BUILDUP], dt);
    case ZVIL_GOAL_CQCATTACK:
        VTMR_countDown(&ent->tmr_list[ZVIL_TWIN_POSTATTACK], dt);
        break;
    case ZVIL_GOAL_DAZED:
        VTMR_countDown(&ent->tmr_list[ZVIL_TDUR_DAZED], dt);
        VTMR_countDown(&ent->tmr_list[ZVIL_TTIL_FIDGET], dt);
        VTMR_countDown(&ent->tmr_list[ZVIL_TTIL_RECOVERY], dt);
        break;
    case ZVIL_GOAL_GUMMED:
        VTMR_countDown(&ent->tmr_list[ZVIL_TDUR_GUM], dt);
        VTMR_countDown(&ent->tmr_list[ZVIL_TTIL_FIDGET], dt);
        VTMR_countDown(&ent->tmr_list[ZVIL_TWIN_MINGUM], dt);
        VTMR_countDown(&ent->tmr_list[ZVIL_TTIL_RECOVERY], dt);
        break;
    case ZVIL_GOAL_SOAPED:
        VTMR_countDown(&ent->tmr_list[ZVIL_TDUR_SOAP], dt);
        VTMR_countDown(&ent->tmr_list[ZVIL_TTIL_FIDGET], dt);
        VTMR_countDown(&ent->tmr_list[ZVIL_TTIL_RECOVERY], dt);
        break;
    case ZVIL_GOAL_HURT:
        VTMR_countDown(&ent->tmr_list[ZVIL_TDUR_HURT], dt);
        break;
    case ZVIL_GOAL_DIE:
        VTMR_countDown(&ent->tmr_list[ZVIL_TDUR_DECAY], dt);
        break;
    }
}

// Works out the villain's per-frame facts about itself and the player (prec). flg_needinfo picks
// what: 0x1 the distance (squared and plain), heading, direction and offset to the player and the
// player's move info, whose sneak bits (0xFF00) are cleared in a boss battle and while the
// ignore-sneak timer runs; outside a boss battle, whenever the player is not sneaking the timer
// restarts at 0.875 to 1.125 of its length. 0x4 the heights of both bound centres over their
// feet. 0x8 the squared flat distances to the anchor and guard positions and the cosine and sine
// of the heading. 0x2, which implies 0x1, detection: a villain that can detect flags the player
// seen (0x1) inside its detection range, which is (ds2_detect / 16) squared while the player
// sneaks, and between the heights of its detection band; a seen player is then flagged in view
// (0x4) and in sight (0x8, always with allow flag 0x200). A villain that can detect also flags a
// player within 1.1 (0x10), the lob distance (0x20) and the extend distance (0x40). With misc
// flag 0x80000000 an unseen player is still checked for view and sight, and a player outside the
// pursuit range around the anchor (when that range is wider than the type's arrival distance)
// gets an intercept point on that range, flagged 0x400 when it is farther from the villain than
// the arrival distance. Past the normal mode the villain checks for a pitfall (flg_xtra 0x1).
// When the distance in prec is under 5 and the height offset under 3 (both 0 without 0x1), the
// player is told the villain type and the length of the offset. flg_status records each part
// worked out, and 0x80000000 once done.
static void zVIL_precalc(st_zEntVillain* vil, S32 flg_needinfo)
{
    st_ZVIL_PRECALC* prec;
    _xVec3 pos_vil = { 0 };
    _xVec3 ctr_vil = { 0 };
    _xVec3 pos_plyr = { 0 };
    _xVec3 ctr_plyr = { 0 };
    _xVec3 vec_tmp = { 0 };
    F32 upr_detect;
    F32 low_detect;
    F32 tmp_detect_radiusSqr;
    S32 was_pitfall;
    S32 was_inlos;

    if (flg_needinfo & 0x2)
    {
        flg_needinfo |= 0x1;
    }

    prec = &vil->precalc;
    was_inlos = (prec->flg_plyr & 0x8) ? 1 : 0;
    was_pitfall = (prec->flg_xtra & 0x1) ? 1 : 0;
    memset(prec, 0, sizeof(st_ZVIL_PRECALC));

    xVec3Copy(&pos_vil, xEntGetPos(vil));
    xVec3Copy(&ctr_vil, xBoundCenter(&vil->bound));
    xVec3Copy(&pos_plyr, xEntGetPos(&globals.player.ent));
    xVec3Copy(&ctr_plyr, xBoundCenter(&globals.player.ent.bound));

    if (flg_needinfo & 0x1)
    {
        prec->ds2_VtoP = VSUP_ds2_toEnt(vil, &globals.player.ent);
        prec->dst_VtoP = xsqrt(prec->ds2_VtoP);
        prec->ang_VtoP = VSUP_dir_toEnt(vil, &globals.player.ent, &prec->dir_VtoP);
        xVec3Sub(&prec->vec_VtoP, &pos_plyr, &pos_vil);
        prec->plyr_mvinf = zEntPlayer_MoveInfo();
        if (g_scene_is_a_boss_battle)
        {
            prec->plyr_mvinf &= 0xFFFF00FF;
        }
        else if (!(prec->plyr_mvinf & 0xFF00))
        {
            vil->tmr_list[ZVIL_TWIN_IGNORESNEAK] =
                VCFG_tymref(vil, ZVIL_TWIN_IGNORESNEAK) +
                0.25f * (iurand() - 0.5f) * VCFG_tymref(vil, ZVIL_TWIN_IGNORESNEAK);
        }
        else if (!(vil->tmr_list[ZVIL_TWIN_IGNORESNEAK] <= 0.0f))
        {
            prec->plyr_mvinf &= 0xFFFF00FF;
        }
        prec->flg_status |= 0x1;
    }

    if (flg_needinfo & 0x4)
    {
        prec->dif_plyrCtr = ctr_plyr.y - pos_plyr.y;
        prec->hyt_plyr = 2.0f * prec->dif_plyrCtr;
        prec->dif_vilCtr = ctr_vil.y - pos_vil.y;
        prec->hyt_vil = 2.0f * prec->dif_vilCtr;
        prec->flg_status |= 0x4;
    }

    if (vil->mode > ZVIL_MODE_NORMAL)
    {
        if (zVIL_chk_pitfall(vil, was_pitfall, 0.25f))
        {
            prec->flg_xtra |= 0x1;
        }
    }

    if (flg_needinfo & 0x8)
    {
        prec->ds2_anchor = VSUP_ds2_toPos(vil, vil->pos_pursue_home);
        prec->ds2_grdpos = VSUP_ds2_toPos(vil, vil->pos_guard_home);
        F32 ang_face = vil->frame->rot.angle;
        prec->cos_faceang = cos(ang_face);
        prec->sin_faceang = sin(ang_face);
        prec->flg_status |= 0x8;
    }

    if (flg_needinfo & 0x2)
    {
        upr_detect = 0.5f * vil->hyt_detect + vil->hyt_dtctoff;
        low_detect = upr_detect - vil->hyt_detect;
        if (vil->vhf->query(vil, ZVIL_Q_CANDETECT))
        {
            tmp_detect_radiusSqr = vil->ds2_detect;
            if (prec->plyr_mvinf & 0xFF00)
            {
                tmp_detect_radiusSqr = SQR(0.0625 * tmp_detect_radiusSqr);
            }
            if (prec->ds2_VtoP < tmp_detect_radiusSqr && prec->vec_VtoP.y < upr_detect &&
                prec->vec_VtoP.y > low_detect)
            {
                prec->flg_plyr |= 0x1;
                if (prec->ds2_VtoP < SQR(0.0625f * vil->ds2_detect))
                {
                    prec->flg_plyr |= 0x1;
                }
                if (VSUP_ent_inFOV(vil, &globals.player.ent))
                {
                    prec->flg_plyr |= 0x4;
                }
                if (vil->allow & 0x200)
                {
                    prec->flg_plyr |= 0x8;
                }
                else if (VSUP_ent_inLOS(vil, &globals.player.ent, was_inlos, 0.1f))
                {
                    prec->flg_plyr |= 0x8;
                }
            }
            if (prec->dst_VtoP < 1.1f)
            {
                prec->flg_plyr |= 0x10;
            }
            if (prec->dst_VtoP < vil->dst_lob)
            {
                prec->flg_plyr |= 0x20;
            }
            if (prec->dst_VtoP < vil->dst_extend)
            {
                prec->flg_plyr |= 0x40;
            }
        }

        if (vil->flg_misc & 0x80000000)
        {
            if (!(prec->flg_plyr & 0x1))
            {
                if (VSUP_ent_inFOV(vil, &globals.player.ent))
                {
                    prec->flg_plyr |= 0x4;
                }
                if (vil->allow & 0x200)
                {
                    prec->flg_plyr |= 0x8;
                }
                else if (VSUP_ent_inLOS(vil, &globals.player.ent, was_inlos, 0.1f))
                {
                    prec->flg_plyr |= 0x8;
                }
            }

            xVec3Sub(&vec_tmp, &ctr_plyr, &vil->pos_pursue_home);
            prec->ds2_plyrToAnchor = SQR(vec_tmp.x) + SQR(vec_tmp.z);
            if (prec->ds2_plyrToAnchor > vil->ds2_pursue &&
                vil->ds2_pursue > g_npcTable[vil->asset->subtype].ds2_atpos)
            {
                xVec3Normalize(&prec->pos_intercept, &vec_tmp);
                xVec3SMulBy(&prec->pos_intercept, xsqrt(vil->ds2_pursue));
                xVec3AddTo(&prec->pos_intercept, &vil->pos_pursue_home);
                xVec3Sub(&vec_tmp, &pos_vil, &prec->pos_intercept);
                if (g_npcTable[vil->asset->subtype].ds2_atpos < SQR(vec_tmp.x) + SQR(vec_tmp.z))
                {
                    prec->flg_plyr |= 0x400;
                }
            }
        }
        prec->flg_status |= 0x2;
    }

    if (prec->dst_VtoP < 5.0f && fabsf(prec->vec_VtoP.y) < 3.0f)
    {
        zEntPlayer_Notify(vil->asset->subtype, xVec3Length(&prec->vec_VtoP));
    }

    prec->flg_status |= 0x80000000;
}

// Turns the villain's move request (domove) into this frame's motion and then clears it. The
// first of bits 0x40, 0x80, 0x100, 0x800 and 0x400 picks the turn: toward ang_moveto, toward
// dir_moveto, by dlt_faceang, toward pos_moveto or toward dlt_moveto. The turn happens only with
// bit 0x1, and only for a villain with move ability 0x1000 or 0x400 or whose current animation
// has type bit 0x2000000; its rate is dt times wiz_moveto with bit 0x10000, otherwise 0.85 to 1
// times dt times the turn speed, and a turn also moves. The step is dt times spd_moveto with bit
// 0x8000, otherwise dt times the move speed for a random 0.9 to 1 (0.6 to 1 after a turn of more
// than half the field of view). Bits 0x2 and 0x8 move
// in x and z along the new heading (half speed backwards with 0x20); 0x10 cancels the move.
// Within a squared flat distance of 0.25 (0.1 for spiders) the villain moves a random share, up
// to half, of the gap instead; the exact-snap branch for the smaller threshold is never reached.
// Bit 0x4 moves toward the target height: all the way within a step, a full step within a flat
// 0.25, otherwise the step times the slope (at most 1). Bit 0x20000000 turns straight to
// ang_moveto and 0x40000000 puts the villain at pos_moveto. Bit 0x1000 sets the velocity to
// vel_moveto and 0x4000 adds it to the velocity change. Unless move ability 0x1000 or 0x400 or
// physics flag 0x4 is set or the villain is gummed or soaped, a step that rises less than 0.01
// drops by 0.01. Move ability 0x2000 records the facing in ang_mdlface; ability 0x4000 and
// domove bits 0x80000 and 0x40000 offset the facing by a quarter turn while the turn is worked
// out and undo it when the villain turns.
static void zVIL_apply_move(st_zEntVillain* ent, _xScene* scn, F32 dt, _xEntFrame* frame)
{
    F32 dy;
    F32 rate;
    F32 hrate;
    _xVec3 npcpos = { 0 };
    F32 ang_diff = 0.0f;
    F32 ang_travel;
    S32 apply_rot = 0;
    S32 apply_move = 0;
    S32 apply_veloc = 0;
    S32 slowmove = 0;
    F32 cos_ang;
    F32 sin_ang;
    F32 distSqr;
    F32 ds2_outersnap;
    F32 ds2_innersnap;
    F32 slope;

    xVec3Copy(&npcpos, &xModelGetFrame(ent->model)->pos);

    if (ent->domove)
    {
        if (ent->abl_move & 0x2000)
        {
            ent->ang_mdlface = frame->rot.angle;
        }
        if (ent->abl_move & 0x4000)
        {
            frame->rot.angle -= PI / 2;
        }
        if (ent->domove & 0x80000)
        {
            frame->rot.angle += PI / 2;
        }
        else if (ent->domove & 0x40000)
        {
            frame->rot.angle -= PI / 2;
        }

        if (ent->domove & 0x40)
        {
            ang_diff = ent->ang_moveto - frame->rot.angle;
            apply_rot = 1;
        }
        else if (ent->domove & 0x80)
        {
            ang_diff = xAngleClamp(atan2(ent->dir_moveto.x, ent->dir_moveto.z));
            ang_diff -= frame->rot.angle;
            apply_rot = 1;
        }
        else if (ent->domove & 0x100)
        {
            ang_diff = ent->dlt_faceang;
            apply_rot = 1;
        }
        else if (ent->domove & 0x800)
        {
            ang_diff = xAngleClamp(
                atan2(ent->pos_moveto.x - npcpos.x, ent->pos_moveto.z - npcpos.z));
            ang_diff -= frame->rot.angle;
            apply_rot = 1;
        }
        else if (ent->domove & 0x400)
        {
            ang_diff = xAngleClamp(atan2(ent->dlt_moveto.x, ent->dlt_moveto.z));
            ang_diff -= frame->rot.angle;
            apply_rot = 1;
        }

        if (!(ent->domove & 0x1)) apply_rot = 0;
        if (ent->domove & 0x2) apply_move = 1;
        if (ent->domove & 0x4) apply_move = 1;
        if (ent->domove & 0x8) apply_move = 1;
        if (ent->domove & 0x5000) apply_veloc = 1;

        if (ent->domove & 0x20000000)
        {
            frame->drot.angle = ent->ang_moveto - frame->rot.angle;
            frame->mode |= 0x20;
            apply_rot = 0;
        }
        if (ent->domove & 0x40000000)
        {
            xVec3Copy(&frame->mat.pos, &ent->pos_moveto);
            frame->mode |= 0x1;
            apply_move = 0;
        }

        ang_travel = frame->rot.angle;
        if (apply_rot && ((ent->abl_move & 0x1000) || (ent->abl_move & 0x400) ||
                          (VANM_entStateType(ent) & 0x2000000)))
        {
            rate = 0.15f * iurand() + 0.85f;
            if (ent->domove & 0x10000)
            {
                rate = dt * ent->wiz_moveto;
            }
            else
            {
                rate *= dt * ent->spd_turn;
            }

            if (ang_diff > PI) ang_diff -= 2 * PI;
            if (ang_diff < -PI) ang_diff += 2 * PI;

            if (ent->abl_move & 0x4000)
            {
                frame->rot.angle += PI / 2;
            }
            if (ent->domove & 0x80000)
            {
                frame->rot.angle -= PI / 2;
            }
            else if (ent->domove & 0x40000)
            {
                frame->rot.angle += PI / 2;
            }

            frame->drot.angle = rate * ang_diff;
            frame->mode |= 0x20;
            ang_travel += frame->drot.angle;
            if (fabsf(ang_diff) > 0.5f * ent->fov_detect) slowmove = 1;
            apply_move = 1;
        }

        if (ent->domove & 0x10) apply_move = 0;
        if (ent->domove & 0x40000000) apply_move = 0;

        if (apply_move)
        {
            rate = slowmove ? 0.4f * iurand() + 0.6f : 0.1f * iurand() + 0.9f;
            if (ent->domove & 0x8000)
            {
                rate = ent->spd_moveto;
            }
            else
            {
                rate = VNAV_get_movespeed(ent, rate);
            }
            rate *= dt;

            distSqr = SQR(ent->pos_moveto.x - npcpos.x) + SQR(ent->pos_moveto.z - npcpos.z);
            if (distSqr < SQR(rate))
            {
                hrate = xsqrt(distSqr);
            }
            else
            {
                hrate = rate;
            }

            frame->dpos.x = 0.0f;
            frame->dpos.y = 0.0f;
            frame->dpos.z = 0.0f;

            ds2_outersnap = 0.25f;
            ds2_innersnap = 0.1f;
            if (ent->asset->subtype == eEntVillainTypeSpider)
            {
                ds2_outersnap = 0.1f;
                ds2_innersnap = 0.01f;
            }

            if (distSqr < ds2_outersnap)
            {
                frame->dpos.x = (ent->pos_moveto.x - npcpos.x) * (0.5f * iurand());
                frame->dpos.z = (ent->pos_moveto.z - npcpos.z) * (0.5f * iurand());
            }
            else if (distSqr < ds2_innersnap)
            {
                frame->dpos.x = ent->pos_moveto.x - npcpos.x;
                frame->dpos.z = ent->pos_moveto.z - npcpos.z;
            }
            else
            {
                cos_ang = cos(ang_travel);
                sin_ang = sin(ang_travel);
                if (ent->domove & 0x2)
                {
                    if (ent->domove & 0x20)
                    {
                        frame->dpos.x = 0.5f * -hrate * sin_ang;
                    }
                    else
                    {
                        frame->dpos.x = hrate * sin_ang;
                    }
                }
                else
                {
                    frame->dpos.x = 0.0f;
                }
                if (ent->domove & 0x8)
                {
                    if (ent->domove & 0x20)
                    {
                        frame->dpos.z = 0.5f * -hrate * cos_ang;
                    }
                    else
                    {
                        frame->dpos.z = hrate * cos_ang;
                    }
                }
                else
                {
                    frame->dpos.z = 0.0f;
                }
            }

            if (ent->domove & 0x4)
            {
                dy = ent->pos_moveto.y - npcpos.y;
                if (SQR(dy) < SQR(rate))
                {
                    frame->dpos.y = dy;
                }
                else if (distSqr < 0.0625f)
                {
                    if (dy < 0.0f)
                    {
                        frame->dpos.y = -1.0f * rate;
                    }
                    else
                    {
                        frame->dpos.y = rate;
                    }
                }
                else
                {
                    slope = dy / xsqrt(distSqr);
                    slope = CLAMP(slope, -1.0f, 1.0f);
                    frame->dpos.y = rate * slope;
                }
            }
            frame->mode |= 0x2;
        }

        if (apply_veloc)
        {
            if (ent->domove & 0x1000)
            {
                frame->vel.x = ent->vel_moveto.x;
                frame->vel.y = ent->vel_moveto.y;
                frame->vel.z = ent->vel_moveto.z;
                frame->mode |= 0x4;
            }
            else if (ent->domove & 0x4000)
            {
                frame->dvel.x += ent->vel_moveto.x;
                frame->dvel.y += ent->vel_moveto.y;
                frame->dvel.z += ent->vel_moveto.z;
                frame->mode |= 0x8;
            }
        }

        if (!(ent->abl_move & 0x1000) && !(ent->abl_move & 0x400) &&
            ent->goal != ZVIL_GOAL_GUMMED && ent->goal != ZVIL_GOAL_SOAPED &&
            !(ent->pflags & 0x4) && frame->dpos.y < 0.01f)
        {
            frame->dpos.y -= 0.01f;
            frame->mode |= 0x2;
        }

        ent->domove = 0;
    }
}

// Whether the villain stands before a drop: with chance rand (otherwise prev stands) it casts a
// ray from its eyeball vertex (its bound centre without one) toward a point 1.5 bound radii ahead
// and 3.5 radii below, offset sideways by 0.3 of the model's right axis, and if that ray hits
// nothing it is a pit; otherwise a second ray goes toward the point offset to the other side. The
// bound radius is set to 0 while the rays are cast and restored after.
static S32 zVIL_chk_pitfall(st_zEntVillain* vil, S32 prev, F32 rand)
{
    S32 ispit;
    F32 sbr;
    _xRay3 ray_los = { 0 };
    _xVec3 vilpos = { 0 };
    _xVec3 tgt = { 0 };
    _xVec3 xos = { 0 };

    if (!xUtil_yesno(rand))
    {
        return prev;
    }

    sbr = vil->bound.sph.r;
    vil->bound.sph.r = 0.0f;

    if (!VSUP_vertPos(vil, ZVIL_MDLVERT_LOSEYEBALL, &vilpos))
    {
        xVec3Copy(&vilpos, xBoundCenter(&vil->bound));
    }

    xVec3SMul(&tgt, (_xVec3*)&vil->model->Mat->at, 1.5f * sbr);
    tgt.y -= 3.5f * sbr;
    xVec3AddTo(&tgt, (_xVec3*)&vil->model->Mat->pos);
    xVec3SMul(&xos, (_xVec3*)&vil->model->Mat->right, 0.3f);

    ray_los.min_t = 0.001f;
    ray_los.flags = 0x800;

    xVec3AddTo(&tgt, &xos);
    xVec3Sub(&ray_los.dir, &tgt, &vilpos);
    ray_los.max_t = xVec3Normalize(&ray_los.dir, &ray_los.dir);
    xVec3SMul(&ray_los.origin, &ray_los.dir, ray_los.min_t);
    xVec3AddTo(&ray_los.origin, &vilpos);
    ispit = !xRayHitsSceneFast(&ray_los, globals.sceneCur);

    if (!ispit)
    {
        xVec3Sub(&tgt, &tgt, &xos);
        xVec3Sub(&tgt, &tgt, &xos);
        xVec3Sub(&ray_los.dir, &tgt, &vilpos);
        ray_los.max_t = xVec3Normalize(&ray_los.dir, &ray_los.dir);
        xVec3SMul(&ray_los.origin, &ray_los.dir, ray_los.min_t);
        xVec3AddTo(&ray_los.origin, &vilpos);
        ispit = !xRayHitsSceneFast(&ray_los, globals.sceneCur);
    }

    vil->bound.sph.r = sbr;
    return ispit;
}

// Copies the villain asset's settings into its config: speeds, detection and pursuit ranges
// (squared), the field of view and turn speed in radians, lobbing and extender settings, and the
// timer lengths, where the asset gives the dazed, taunt, gum, bubble, extend and reload times and
// the rest get fixed defaults (-1 for timers with no default).
static void VCFG_readAsset(st_ZVIL_CONFIG_REFERENCE* cfgdata)
{
    p2EntVillainAsset* rawass = cfgdata->vilp2ass;

    cfgdata->hitpts = rawass->hitpoints;

    cfgdata->fov_detect = 3.1415927f * rawass->activateFOV / 180.0f;
    cfgdata->ds2_detect = rawass->activateRadius * rawass->activateRadius;
    cfgdata->hyt_detect = rawass->detectHeight;
    cfgdata->hyt_dtctoff = rawass->detectHeightOffset;
    cfgdata->ds2_pursue = rawass->pursuitRange * rawass->pursuitRange;

    cfgdata->spd_move = rawass->speedMovement;
    cfgdata->spd_chase = rawass->speedPursue;
    cfgdata->spd_turn = 3.1415927f * rawass->speedTurn / 180.0f;

    cfgdata->spd_lob = rawass->lobSpeed;
    cfgdata->dst_lob = rawass->lobRange;
    cfgdata->cnt_lobSalvo = rawass->lobSalvo;
    cfgdata->fac_lobarc = rawass->lobArcness;
    cfgdata->fac_lobheavy = rawass->lobHeavy;

    cfgdata->dst_extend = rawass->extenderRange;
    cfgdata->wid_extend = rawass->extenderWidth;
    cfgdata->spd_extend = rawass->extenderRate;

    cfgdata->tymref_list[ZVIL_TTIL_FIDGET] = 8.0f;
    cfgdata->tymref_list[ZVIL_TTIL_BORED] = 20.0f;
    cfgdata->tymref_list[ZVIL_TTIL_TAUNT] = 5.0f;
    cfgdata->tymref_list[ZVIL_TTIL_CHKFORNAV] = 7.0f;
    cfgdata->tymref_list[ZVIL_TTIL_RECOVERY] = 100.0f;
    cfgdata->tymref_list[ZVIL_TTIL_NEXTCHASE] = 0.3f;
    cfgdata->tymref_list[ZVIL_TTIL_ANIMSOUND] = -1.0f;
    cfgdata->tymref_list[ZVIL_TTIL_NEXTVOICE] = 5.0f;
    cfgdata->tymref_list[ZVIL_TTIL_QUIP_OK] = 10.0f;
    cfgdata->tymref_list[ZVIL_TDUR_STDBY] = 10.0f;
    cfgdata->tymref_list[ZVIL_TDUR_ALERT] = 2.0f;
    cfgdata->tymref_list[ZVIL_TDUR_TAUNT] = rawass->durGloatState;
    cfgdata->tymref_list[ZVIL_TDUR_DAZED] = rawass->durDazedState;
    cfgdata->tymref_list[ZVIL_TDUR_GUM] = rawass->durGummedState;
    cfgdata->tymref_list[ZVIL_TDUR_SOAP] = rawass->durBubbleState;
    cfgdata->tymref_list[ZVIL_TDUR_LOOK] = 3.0f;
    cfgdata->tymref_list[ZVIL_TDUR_HEAR] = 3.0f;
    cfgdata->tymref_list[ZVIL_TDUR_SEARCH] = 4.0f;
    cfgdata->tymref_list[ZVIL_TDUR_EXTEND] = rawass->extenderDuration;
    cfgdata->tymref_list[ZVIL_TDUR_LOBRELOAD] = rawass->lobDurReload;
    cfgdata->tymref_list[ZVIL_TDUR_EXTRELOAD] = rawass->extenderReloadTime;
    cfgdata->tymref_list[ZVIL_TDUR_BUILDUP] = 3.0f;
    cfgdata->tymref_list[ZVIL_TDUR_HURT] = -1.0f;
    cfgdata->tymref_list[ZVIL_TDUR_DECAY] = -1.0f;
    cfgdata->tymref_list[ZVIL_TDUR_NAVDELAY] = -1.0f;
    cfgdata->tymref_list[ZVIL_TWIN_MINGUM] = 2.0f;
    cfgdata->tymref_list[ZVIL_TWIN_POSTATTACK] = 0.2f;
    cfgdata->tymref_list[ZVIL_TWIN_IGNORESNEAK] = 0.25f;
    cfgdata->tymref_list[ZVIL_TYMR_MINANIMPLAY] = -1.0f;
    cfgdata->tymref_list[ZVIL_TYMR_CURGOAL] = -1.0f;
    cfgdata->tymref_list[ZVIL_TYMR_GENERIC] = -1.0f;
}

// Writes the villain's saved state as two bits: bit 0 when its goal is the afterlife or base
// setup, bit 1 when it is active and its asset says it starts unborn (prebirth).
static S32 VSAV_save(st_zEntVillain* ent, xSerial* xser)
{
    S32 iv;
    en_BEHAVIOUR_GOAL gol;

    if (!(ent->baseFlags & 0x2))
    {
        return 0;
    }

    zEntSave(ent, xser);

    iv = 0;
    gol = ent->goal;
    if (gol == ZVIL_GOAL_AFTERLIFE || gol == ZVIL_GOAL_BASEINIT)
    {
        iv |= 0x1;
    }
    else if (ent->mode > ZVIL_MODE_SHUTDOWN && ent->mode < ZVIL_MODE_DEATH)
    {
        switch (ent->vilcfg.vilp2ass->behaviorState)
        {
        case ZVIL_START_PREBIRTH:
            iv |= 0x2;
            break;
        }
    }

    xser->Write_b2(iv);
    return 1;
}

// Reads the villain's saved state back. Bit 0 shuts it down at base setup, hides it and stops
// anything from colliding with it; bit 1 shuts it down at startup.
static S32 VSAV_load(st_zEntVillain* ent, xSerial* xser)
{
    S32 iv = 0;

    if (!(ent->baseFlags & 0x2))
    {
        return 1;
    }

    zEntLoad(ent, xser);

    xser->Read_b2(&iv);
    if (iv & 0x1)
    {
        ent->goaldflt = ZVIL_GOAL_IDLE;
        ent->mode = ZVIL_MODE_SHUTDOWN;
        VGOL_set(ent, ZVIL_GOAL_BASEINIT);
        ent->chkby = 0;
        ent->penby = 0;
        ent->flags &= ~0x1;
    }
    else if (iv & 0x2)
    {
        ent->goaldflt = ZVIL_GOAL_IDLE;
        ent->mode = ZVIL_MODE_SHUTDOWN;
        VGOL_set(ent, ZVIL_GOAL_STARTUP);
    }

    return 1;
}

// Aims the villain's reserved projectile as a gravity ball from his attack vertex, or else from
// half a unit ahead of and above his bound centre (looking up his name, unused); the boss Black
// Knight throws from 2 units up and 1.5 to his left of that. It goes toward pos_tgt plus vec_off
// (when given) at his lob speed, with an arc of arcness and gravity of heavy (1 and 1.5 when not
// above 0), spins with vec_spin when given and cannot collide for 0.4 seconds. Returns whether
// the aim worked.
static S32 VWEP_lobInitArcing(st_zEntVillain* vil, _xVec3* pos_tgt, _xVec3* vec_off,
                              _xVec3* vec_spin, F32 arcness, F32 heavy)
{
    S32 result = 1;
    zEntProjectile* prj = vil->prj_reserved;
    _xVec3 pos_launch = { 0 };
    ST_CAT_AIMDATA aiminf = { ZPRJ_CAIM_GRAVBALL };
    _xVec3 tmp_vec = { 0 };
    S32 rc;

    prj->ColDelaySet(0.4f, 1);
    rc = VSUP_vertPos(vil, ZVIL_MDLVERT_ATTACK, &pos_launch);
    if (!rc)
    {
        xSceneID2Name(globals.sceneCur, vil->id);
        xVec3Copy(&pos_launch, xBoundCenter(&vil->bound));
        xVec3SMul(&tmp_vec, (_xVec3*)&vil->model->Mat->at, 0.5f);
        tmp_vec.y += 0.5f;
        xVec3AddTo(&pos_launch, &tmp_vec);
    }
    if (vil->asset->subtype == eEntVillainBossBlackKnight)
    {
        xVec3SMul(&tmp_vec, (_xVec3*)&vil->model->Mat->up, 2.0f);
        xVec3AddTo(&pos_launch, &tmp_vec);
        xVec3SMul(&tmp_vec, (_xVec3*)&vil->model->Mat->right, -1.5f);
        xVec3AddTo(&pos_launch, &tmp_vec);
    }

    aiminf.spd_base = vil->spd_lob;
    xVec3Copy(&aiminf.dir_base, (_xVec3*)&vil->model->Mat->at);
    xVec3Copy(&aiminf.grav.pos_src, &pos_launch);
    if (vec_off)
    {
        xVec3Add(&aiminf.grav.pos_tgt, pos_tgt, vec_off);
    }
    else
    {
        xVec3Copy(&aiminf.grav.pos_tgt, pos_tgt);
    }
    if (arcness <= 0.0f)
    {
        aiminf.grav.fac_arcness = 1.0f;
    }
    else
    {
        aiminf.grav.fac_arcness = arcness;
    }
    if (heavy <= 0.0f)
    {
        aiminf.grav.acc_gravity = 1.5f;
    }
    else
    {
        aiminf.grav.acc_gravity = heavy;
    }

    if (vec_spin)
    {
        prj->FlexMatSet(vec_spin, globals.update_dt);
        prj->flg_categ |= 0x4000000;
    }
    else
    {
        prj->flg_categ &= ~0x4000000;
    }
    rc = prj->AimAtTarget(&aiminf, 1);
    if (!rc)
    {
        result = 0;
    }
    return result;
}

// Readies the Black Knight's reserved axe to throw: it is aimed as an arcing throw (see
// VWEP_lobInitArcing), rolled a quarter circle (about z), then pitched (about x) by the clamped
// heading from pos_tgt to the axe (atan2 of the x and z differences), and spins with vec_spin when
// given. Its lightning reaches his extend distance, drops by his extend width and lasts his extend
// speed in time; its planes are 46, his lob distance and -268.5. It lives until 10000 units
// squared away. Always returns 1.
static S32 VWEP_lobInitBKAxe(st_zEntVillain* vil, _xVec3* pos_tgt, _xVec3* vec_off,
                             _xVec3* vec_spin, F32 arcness, F32 heavy)
{
    zEntProjectile* prj = vil->prj_reserved;
    _xVec3 vec_initrot = { 0.0f, 0.0f, PI / 2 };
    _xMat3x3 mat_rot = { 0 };
    _xVec3 vec_tmp = { 0 };
    _xVec3 ang = { 0 };
    st_BKAXE_DATA* axfx = (st_BKAXE_DATA*)prj->xtradata;

    VWEP_lobInitArcing(vil, pos_tgt, vec_off, vec_spin, arcness, heavy);
    xMat3x3Euler(&mat_rot, &vec_initrot);
    xMat4x3RMul3x3(&prj->frame->mat, &mat_rot, &prj->frame->mat);
    xVec3Sub(&vec_tmp, (_xVec3*)&prj->frame->mat.pos, pos_tgt);
    ang.y = xAngleClamp(atan2(vec_tmp.x, vec_tmp.z));
    xMat3x3Euler(&mat_rot, &ang);
    xMat4x3RMul3x3(&prj->frame->mat, &mat_rot, &prj->frame->mat);
    prj->frame->mode |= 0x101;
    prj->SyncFrame(1);

    axfx->rad_zap = vil->dst_extend;
    axfx->dst_zapdrop = vil->wid_extend;
    axfx->tym_zapdur = vil->spd_extend;
    axfx->planes.y = vil->dst_lob;
    axfx->planes.z = -268.5f;
    axfx->planes.x = 46.0f;
    if (vec_spin)
    {
        prj->FlexMatSet(vec_spin, globals.update_dt);
        prj->flg_categ |= 0x4000000;
    }
    else
    {
        prj->flg_categ &= ~0x4000000;
    }
    prj->ds2_maxlife = 10000.0f;
    return 1;
}

// Readies the Green Ghost's reserved quake ball to throw, aimed like an arrow (see
// VWEP_lobInitArrow). It falls with a gravity of heavy (1.5 when not above 0). The ball builds to
// his lob distance, flies at his lob speed (at least 0.2), and its shockwave reaches his extend
// distance, as high as his extend width, for his extend speed in time. Always returns 1.
static S32 VWEP_lobInitGGQuaker(st_zEntVillain* vil, _xVec3* pos_tgt, _xVec3* vec_off,
                                _xVec3* vec_spin, F32 arcness, F32 heavy)
{
    zEntProjectile* prj = vil->prj_reserved;
    st_BGG_QUAKEBALL* quake = (st_BGG_QUAKEBALL*)prj->xtradata;

    VWEP_lobInitArrow(vil, pos_tgt, vec_off, vec_spin, arcness, heavy);
    if (heavy <= 0.0f)
    {
        prj->cat_data.arrow.acc_gravity = 1.5f;
    }
    else
    {
        prj->cat_data.arrow.acc_gravity = heavy;
    }

    quake->rad_build = vil->dst_lob;
    quake->spd_flight = MAX(vil->spd_lob, 0.2f);
    quake->hyt_shock = vil->wid_extend;
    quake->rad_shock = vil->dst_extend;
    quake->tym_shock = vil->spd_extend;
    return 1;
}

// Aims the villain's reserved projectile like an arrow from his attack vertex, or else from half
// a unit ahead of and above his bound centre (looking up his name, unused), toward pos_tgt plus
// vec_off (when given), at his lob speed. It cannot collide for the first 0.4 seconds. Always
// returns 1.
static inline S32 VWEP_lobInitArrow(st_zEntVillain* vil, _xVec3* pos_tgt, _xVec3* vec_off,
                                    _xVec3* vec_spin, F32 arcness, F32 heavy)
{
    zEntProjectile* prj = vil->prj_reserved;
    _xVec3 pos_launch = { 0 };
    ST_CAT_AIMDATA aiminf = { ZPRJ_CAIM_ARROW };
    _xVec3 tmp_vec = { 0 };
    S32 rc;

    prj->ColDelaySet(0.4f, 1);
    rc = VSUP_vertPos(vil, ZVIL_MDLVERT_ATTACK, &pos_launch);
    if (!rc)
    {
        xSceneID2Name(globals.sceneCur, vil->id);
        xVec3Copy(&pos_launch, xBoundCenter(&vil->bound));
        xVec3SMul(&tmp_vec, (_xVec3*)&vil->model->Mat->at, 0.5f);
        tmp_vec.y += 0.5f;
        xVec3AddTo(&pos_launch, &tmp_vec);
    }

    aiminf.spd_base = vil->spd_lob;
    xVec3Copy(&aiminf.dir_base, (_xVec3*)&vil->model->Mat->at);
    xVec3Copy(&aiminf.arrow.pos_src, &pos_launch);
    if (vec_off)
    {
        xVec3Add(&aiminf.arrow.pos_tgt, pos_tgt, vec_off);
    }
    else
    {
        xVec3Copy(&aiminf.arrow.pos_tgt, pos_tgt);
    }
    prj->AimAtTarget(&aiminf, 1);
    return 1;
}

// Readies Redbeard's reserved sword to throw: it starts at his sword handle vertex, or else his
// attack, weapon or eye vertex, or else his bound centre, turned half a circle about z. It flies
// his extend distance toward the player at two fifths of that distance per second (with the
// constant 1.999999), and its end point is that far along the direction to the player. Always
// returns 1.
static S32 VWEP_lobInitRBSword(st_zEntVillain* vil, _xVec3* pos_tgt, _xVec3* vec_off,
                               _xVec3* vec_spin, F32 arcness, F32 heavy)
{
    zEntProjectile* prj = vil->prj_reserved;
    S32 rc;
    _xVec3 tmp_vec = { 0 };
    st_REDBEARD_SWORD* sword = (st_REDBEARD_SWORD*)prj->xtradata;

    rc = VSUP_vertPos(vil, ZVIL_MDLVERT_SWORDHANDLE, &tmp_vec);
    if (!rc)
    {
        rc = VSUP_vertPos(vil, ZVIL_MDLVERT_ATTACK, &tmp_vec);
    }
    if (!rc)
    {
        rc = VSUP_vertPos(vil, ZVIL_MDLVERT_WEAPON, &tmp_vec);
    }
    if (!rc)
    {
        rc = VSUP_vertPos(vil, ZVIL_MDLVERT_LOSEYEBALL, &tmp_vec);
    }
    if (!rc)
    {
        xVec3Copy(&tmp_vec, xBoundCenter(&vil->bound));
    }

    xVec3Copy((_xVec3*)&prj->frame->mat.pos, &tmp_vec);
    xMat3x3Euler((xMat3x3*)&prj->frame->mat, 0.0f, 0.0f, PI);
    prj->frame->mode |= 0x101;
    prj->SyncFrame(1);

    sword->dst_far = vil->dst_extend;
    sword->spd_fly = (1.999999f * vil->dst_extend) / 5.0f;
    xVec3Copy(&sword->dir_end, &vil->precalc.dir_VtoP);
    xVec3SMul(&sword->pos_end, &sword->dir_end, sword->dst_far);
    return 1;
}

// Readies the Mastermind's reserved flask to throw: it starts at his attack vertex (when tagged),
// tipped 45 degrees about z, and spins with vec_spin when given. The flask enters the tease stage
// with a flight speed of his lob speed (at least 0.2), its aftermath time, height and radius all
// his extend width, and an arc of arcness and weight of heavy, which default to 1 and 1.5 when not
// above 0. Always returns 1.
static S32 VWEP_lobInitMMFlask(st_zEntVillain* vil, _xVec3* pos_tgt, _xVec3* vec_off,
                               _xVec3* vec_spin, F32 arcness, F32 heavy)
{
    zEntProjectile* prj = vil->prj_reserved;
    _xMat3x3 mat_rot = { 0 };
    st_ZVMM_FLASK_DATA* flask = (st_ZVMM_FLASK_DATA*)prj->xtradata;
    static _xVec3 vec_initwobble = { 0.0f, 0.0f, PI / 4 };

    VSUP_vertPos(vil, ZVIL_MDLVERT_ATTACK, (_xVec3*)&prj->frame->mat.pos);
    xMat3x3Euler(&mat_rot, &vec_initwobble);
    xMat4x3RMul3x3(&prj->frame->mat, &mat_rot, &prj->frame->mat);
    prj->frame->mode |= 0x101;
    prj->SyncFrame(1);
    if (vec_spin)
    {
        prj->FlexMatSet(vec_spin, globals.update_dt);
        prj->flg_categ |= 0x4000000;
    }
    else
    {
        prj->flg_categ &= ~0x4000000;
    }

    flask->stage = MAST_STAGE_TEASE;
    flask->spd_flight = MAX(vil->spd_lob, 0.2f);
    flask->hyt_aftermath = vil->wid_extend;
    flask->tym_aftermath = vil->wid_extend;
    flask->rad_aftermath = vil->wid_extend;
    if (arcness <= 0.0f)
    {
        flask->arcness = 1.0f;
    }
    else
    {
        flask->arcness = arcness;
    }
    if (heavy <= 0.0f)
    {
        flask->heavy = 1.5f;
    }
    else
    {
        flask->heavy = heavy;
    }
    return 1;
}

// Keeps the Black Knight's axe's collision sphere (radius 0.75) on its position.
static void VPRJ_BBK_bupdate(_xEnt* prjent, _xVec3* pos)
{
    prjent->bound.sph.center = *pos;
    prjent->bound.sph.r = 0.75f;
    xBoundUpdate(&prjent->bound);
}

// Sets up a thrown Black Knight axe: it keeps its own collision sphere, gets a collision delay,
// lives at least 30 seconds and takes a free slot of the axe pool. Returns 1 if a slot was free.
static S32 VPRJ_BBK_attach(zEntProjectile* prj)
{
    S32 i;

    prj->bupdate = VPRJ_BBK_bupdate;
    prj->cat_data.idx_cdat = ZPRJ_CDAT_BOUNCE;
    prj->OrientSet(0);
    prj->ColDelaySet(50.0f, 1);
    prj->tym_maxlife = MAX(30.0f, prj->tym_maxlife);

    for (i = 0; i < 10; i++)
    {
        if (!(g_axfxpool[i].flg_stat & 1))
        {
            memset(&g_axfxpool[i], 0, sizeof(st_BKAXE_DATA));
            g_axfxpool[i].flg_stat |= 1;
            prj->xtradata = &g_axfxpool[i];
            break;
        }
    }

    if (prj->xtradata)
    {
        return 1;
    }
    return 0;
}

// Releases a Black Knight axe's pool slot, cleaning up its lightning first.
static void VPRJ_BBK_detach(zEntProjectile* prj)
{
    st_BKAXE_DATA* axfx = (st_BKAXE_DATA*)prj->xtradata;

    if (axfx)
    {
        VPRJ_zapTheAxe(prj, 1);
        memset(axfx, 0, sizeof(st_BKAXE_DATA));
        prj->xtradata = NULL;
    }
}

// Moves a thrown Black Knight axe. In flight it falls under the bounce gravity and, when its
// category flag 0x4000000 is set, turns by its flex matrix. At rest it keeps turning only while
// it does not lie flat, and flag bit 2 of its lightning data is cleared once it stops. Returns 1.
static S32 VPRJ_BBK_prjmove(zEntProjectile* prj, F32 dt, zScene*, _xEntFrame* frame)
{
    st_BKAXE_DATA* axfx = (st_BKAXE_DATA*)prj->xtradata;
    st_ZPRJ_CAT_BOUNCEBALL* bounce = &prj->cat_data.bounce;

    switch (bounce->atRest)
    {
    default:
        frame->dvel.x = 0.0f;
        frame->dvel.y = -(dt * bounce->acc_gravity);
        frame->dvel.z = 0.0f;
        frame->mode |= 0x8;

        if (prj->flg_categ & 0x4000000)
        {
            xMat4x3RMul3x3(&frame->mat, &prj->common.mat_flex, &frame->mat);
            prj->frame->mode |= 0x100;
        }
        break;
    case 1:
        if (frame->mat.at.y > -0.25f)
        {
            xMat4x3RMul3x3(&frame->mat, &prj->common.mat_flex, &frame->mat);
            prj->frame->mode |= 0x100;
        }

        if ((axfx->flg_stat & 0x2) && !(prj->frame->mode & 0x100))
        {
            axfx->flg_stat &= ~0x2;
        }
        break;
    }

    return 1;
}

// Sets up a Green Ghost quake ball: it is drawn as an arrow, starts building up and takes a free
// slot of the quake ball pool. Returns 1 if a slot was free.
static S32 VPRJ_BGG_attach(zEntProjectile* prj)
{
    S32 i;

    prj->cat_data.idx_cdat = ZPRJ_CDAT_ARROW;
    prj->flags &= ~0x1;
    prj->flg_prop &= ~0x8;

    for (i = 0; i < 4; i++)
    {
        st_BGG_QUAKEBALL* quake = &g_quakepool[i];
        if (!(quake->flg_stat & 1))
        {
            memset(quake, 0, sizeof(st_BGG_QUAKEBALL));
            quake->flg_stat |= 1;
            quake->phase = BGG_PHAZ_BUILDUP;
            prj->xtradata = quake;
            break;
        }
    }

    return prj->xtradata ? 1 : 0;
}

// Releases a Green Ghost quake ball's pool slot.
static void VPRJ_BGG_detach(zEntProjectile* prj)
{
    st_BGG_QUAKEBALL* quake = (st_BGG_QUAKEBALL*)prj->xtradata;

    if (quake)
    {
        memset(quake, 0, sizeof(st_BGG_QUAKEBALL));
        prj->xtradata = NULL;
    }
}

// Sets up a Redbeard sword projectile and takes a free sword. Returns 1 if one was free.
static S32 VPRJ_BRB_attach(zEntProjectile* prj)
{
    S32 i;

    prj->cat_data.idx_cdat = ZPRJ_CDAT_ARROW;

    for (i = 0; i < 4; i++)
    {
        st_REDBEARD_SWORD* sword = &g_rbsword[i];
        if (!(sword->flg_stat & 1))
        {
            memset(sword, 0, sizeof(st_REDBEARD_SWORD));
            sword->flg_stat |= 1;
            prj->xtradata = sword;
            break;
        }
    }

    return prj->xtradata ? 1 : 0;
}

// Releases a Redbeard sword.
static void VPRJ_BRB_detach(zEntProjectile* prj)
{
    st_REDBEARD_SWORD* sword = (st_REDBEARD_SWORD*)prj->xtradata;

    if (sword)
    {
        memset(sword, 0, sizeof(st_REDBEARD_SWORD));
        prj->xtradata = NULL;
    }
}

// Redbeard sword projectiles take every event as handled.
static S32 VPRJ_BRB_notify(zEntProjectile*, en_PROJECTILE_EVENT, void*)
{
    return 1;
}

// Keeps the Mastermind's flask's collision sphere (radius 0.5) on its position.
static void VPRJ_BMM_bupdate(_xEnt* prjent, _xVec3* pos)
{
    prjent->bound.sph.center = *pos;
    prjent->bound.sph.r = 0.5f;
    xBoundUpdate(&prjent->bound);
}

// Sends a reserved Mastermind flask at a target: sets its target, moves it to the toss stage and
// sets its flag bit 0x2.
static inline void VPRJ_BMM_toss(zEntProjectile* prj, const _xVec3* pos_tgt)
{
    st_ZVMM_FLASK_DATA* flask = (st_ZVMM_FLASK_DATA*)prj->xtradata;
    xVec3Copy(&flask->pos_tgt, pos_tgt);
    flask->stage = MAST_STAGE_TOSS;
    flask->flg_stat |= 0x2;
}

// Sends the villain to a position: sets allow bit 0x1, drops the need for a navigation point,
// clears flg_misc bits 0x20000000, 0x40000000 and 0x80000000 and sets 0x10000000, and keeps the
// square of the distance he should stop short by.
static inline void VOUT_dogoto_pos(st_zEntVillain* vil, const _xVec3* pos, F32 dst_offset)
{
    vil->allow |= 0x1;
    vil->need_navpt = 0;
    vil->flg_misc &= ~0x20000000;
    vil->flg_misc &= ~0x40000000;
    vil->flg_misc &= ~0x80000000;
    vil->flg_misc |= 0x10000000;
    vil->ds2_goto_keepaway = dst_offset * dst_offset;
    xVec3Copy(&vil->pos_goto, pos);
}

// Puffs the Mastermind's teleport-out particles where he stood before teleporting.
static inline void VEFX_mm_teleportHide(F32 dt)
{
    _zParEmitterCustomSettings info;
    info.custom_flags = 0x20;
    xVec3Copy(&info.pos, &sMMPreTeleportPos);
    zParEmitterEmitCustom(sEmitMMTeleportHide, dt, &info);
}

// Puffs the Mastermind's teleport-in particles one unit above where he stands.
static inline void VEFX_mm_teleportShow(st_zEntVillain* vil, F32 dt)
{
    _zParEmitterCustomSettings info;
    info.custom_flags = 0x20;
    xVec3Copy(&info.pos, &xModelGetFrame(vil->model)->pos);
    info.pos.y += 1.0f;
    zParEmitterEmitCustom(sEmitMMTeleportShow, dt, &info);
}

// Stops the villain's frame: clears its velocity and its velocity change and sets mode bits 0x4
// and 0x8.
static inline void VSUP_haltFrame(st_zEntVillain* vil)
{
    vil->frame->dvel.x = 0.0f;
    vil->frame->dvel.y = 0.0f;
    vil->frame->dvel.z = 0.0f;
    vil->frame->vel.x = 0.0f;
    vil->frame->vel.y = 0.0f;
    vil->frame->vel.z = 0.0f;
    vil->frame->mode |= 0xC;
}

// Stops a movement frame: clears its velocity and its velocity change and sets mode bits 0x4 and
// 0x8.
static inline void VSUP_haltEntFrame(const _xEntFrame* frame)
{
    _xEntFrame* frm = (_xEntFrame*)frame;
    frm->dvel.x = 0.0f;
    frm->dvel.y = 0.0f;
    frm->dvel.z = 0.0f;
    frm->vel.x = 0.0f;
    frm->vel.y = 0.0f;
    frm->vel.z = 0.0f;
    frm->mode |= 0xC;
}

// Readies a reserved projectile to be lobbed at the player's bound centre, with the villain's lob
// spin, arc and heaviness.
static inline S32 VWEP_lobAtPlayer(zEntProjectile* prj, st_zEntVillain* vil)
{
    return VWEP_lobActivatePRJ(prj, vil, xBoundCenter(&globals.player.ent.bound), NULL,
                               &vil->vec_lobSpin, vil->fac_lobarc, vil->fac_lobheavy);
}

// Sets up a thrown Mastermind flask: it keeps its own collision sphere and takes a free slot of
// the flask pool. Returns 1 if a slot was free.
static S32 VPRJ_BMM_attach(zEntProjectile* prj)
{
    S32 i;

    prj->cat_data.idx_cdat = ZPRJ_CDAT_BOUNCE;
    prj->bupdate = VPRJ_BMM_bupdate;
    prj->OrientSet(0);

    for (i = 0; i < 10; i++)
    {
        if (!(g_mmflask[i].flg_stat & 1))
        {
            memset(&g_mmflask[i], 0, sizeof(st_ZVMM_FLASK_DATA));
            g_mmflask[i].flg_stat |= 1;
            prj->xtradata = &g_mmflask[i];
            break;
        }
    }

    return prj->xtradata ? 1 : 0;
}

// Releases a Mastermind flask's pool slot.
static void VPRJ_BMM_detach(zEntProjectile* prj)
{
    st_ZVMM_FLASK_DATA* flask = (st_ZVMM_FLASK_DATA*)prj->xtradata;

    if (flask)
    {
        memset(flask, 0, sizeof(st_ZVMM_FLASK_DATA));
        prj->xtradata = NULL;
    }
}

// Keeps a Redbeard sword's collision sphere (radius 0.75) on its position.
static void VWEP_rbs_bupdate(_xEnt* ent_rbs, _xVec3* pos)
{
    ent_rbs->bound.type = 1;
    ent_rbs->bound.sph.r = 0.75f;
    ent_rbs->bound.sph.center = *pos;
    xBoundUpdate(&ent_rbs->bound);
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

// Plays one of three animations, picked at random, without weights, among those the villain's
// animation table has.
static inline en_NPC_ANIMSTATES VANM_randthree(st_zEntVillain* ent, en_NPC_ANIMSTATES a,
                                               en_NPC_ANIMSTATES b, en_NPC_ANIMSTATES c)
{
    en_NPC_ANIMSTATES choice_list[3] = { a, b, c };
    return VANM_randset(ent, NULL, choice_list, 3);
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

// The animation state the villain's model is in, or NULL if it has none.
static inline xAnimState* VANM_entState(const _zEnt* ent)
{
    xAnimSingle* single = VANM_entSingle(ent);
    xAnimState* state = single ? single->State : NULL;
    return state;
}

// The type bits (0x1E000000) of the user flags of the villain's current animation state, or 0 if
// it has none.
static inline U32 VANM_entStateType(const _zEnt* ent)
{
    xAnimState* state = VANM_entState(ent);
    return state ? state->UserFlags & 0x1E000000 : 0;
}

// The move kind bits (0x1E) of the user flags of the villain's current animation state, or 0 if
// it has none: 1 idle, 4 move, 6 chase, 10 a fixed fast move.
static inline U32 VANM_entMoveType(const _zEnt* ent)
{
    xAnimState* state = VANM_entState(ent);
    return state ? state->UserFlags & 0x1E : 0;
}

// While an animation plays, sets its playback speed to spd over the animation's own speed
// (sanim.x), kept between sanim.y and sanim.z; does nothing when sanim.x is under 0.001.
static inline void VANM_setSpeed(st_zEntVillain* ent, F32 spd, _xVec3 sanim)
{
    F32 fv;
    xAnimSingle* single;

    if (!ent->anim_playing)
    {
        return;
    }
    if (sanim.x < 0.001f)
    {
        return;
    }

    fv = spd / sanim.x;
    fv = CLAMP(fv, sanim.y, sanim.z);
    single = VANM_entSingle(ent);
    if (single)
    {
        single->CurrentSpeed = fv;
    }
}

// How far the villain's model is into its current animation, or 0 if it has none.
static inline F32 VANM_entTime(const _zEnt* ent)
{
    xAnimSingle* single = VANM_entSingle(ent);
    return single ? single->Time : 0.0f;
}

// The reference time a villain type's config gives one of its timers.
static inline F32 VCFG_tymref(st_zEntVillain* vil, S32 which)
{
    return vil->vilcfg.tymref_list[which];
}

// Whether a time into the attack animation is inside the villain's damage window, from its
// configured start and end times (-1 and -2 when it has none).
static inline bool VCFG_inDamageWindow(st_zEntVillain* vil, F32 tym)
{
    F32 tym_beg = -1.0f;
    F32 tym_end = -2.0f;
    VCFG_getParm(&vil->vilcfg, ZVIL_PARM_DMG_BEGTIME, &tym_beg);
    VCFG_getParm(&vil->vilcfg, ZVIL_PARM_DMG_ENDTIME, &tym_end);
    bool inwin = tym >= tym_beg;
    if (inwin)
    {
        inwin = tym <= tym_end;
    }
    return inwin;
}

// Adds a sound to the first free slot of the villain's sound queue (a slot whose sound is -1),
// cleared first. A sound to play directly is queued as VSND_LISTEND. Returns 1 if a slot was
// free, 0 if not or if the sound is VSND_TYP_BOGUS.
static S32 VSND_addQueueEntry(st_zEntVillain* vil, en_NPC_SOUND sndEvent, S32 flg_vsnd, F32 radius,
                              F32 delay, F32 repeatDelay, S32 repeatCount, U32 useThisSound)
{
    S32 added = 0;
    S32 i;
    st_NPC_SOUND_QUEUE* que;

    if (sndEvent == VSND_TYP_BOGUS)
    {
        return 0;
    }

    for (i = 0; i < 4; i++)
    {
        que = &vil->snd_queue[i];
        if (que->sndEvent == VSND_TYP_BOGUS)
        {
            memset(que, 0, sizeof(st_NPC_SOUND_QUEUE));
            if (useThisSound)
            {
                que->sndEvent = VSND_LISTEND;
            }
            else
            {
                que->sndEvent = sndEvent;
            }
            que->sndDirect = useThisSound;
            que->flg_frolic = flg_vsnd;
            que->tmr_delay = delay;
            que->cnt_repeat = repeatCount;
            que->radius = radius;
            que->tym_repeatDelay = repeatDelay;
            added++;
            break;
        }
    }

    return added;
}

// Runs the villain's sound queue for a frame. A queued direct sound (VSND_LISTEND) with no sound
// to play is dropped; any other entry forgets its direct sound. Each entry's delay counts down by
// dt, and when it runs out the sound plays with the low 24 bits of its flags. The top four flag
// bits then set the repeat count: 0x1 sets it to 1, so the sound repeats until its slot is freed
// elsewhere, 0x2 counts the repeats down and anything else ends it. An entry with repeats left
// waits its repeat delay again; the others are freed. Returns how many entries played this frame
// and stay queued.
static S32 VSND_procQueue(st_zEntVillain* vil, F32 dt)
{
    st_NPC_SOUND_QUEUE* que;
    S32 i;
    S32 cnt = 0;

    for (i = 0; i < 4; i++)
    {
        que = &vil->snd_queue[i];
        if (que->sndEvent == VSND_TYP_BOGUS)
        {
            continue;
        }

        if (que->sndEvent == VSND_LISTEND)
        {
            if (!que->sndDirect)
            {
                que->sndEvent = VSND_TYP_BOGUS;
                continue;
            }
        }
        else
        {
            que->sndDirect = 0;
        }

        que->tmr_delay -= dt;
        if (que->tmr_delay > 0.0f)
        {
            continue;
        }

        VSND_goPlay(vil, que->sndEvent, que->sndDirect, que->flg_frolic & 0xFFFFFF, que->radius);

        switch (que->flg_frolic & 0xF0000000)
        {
        case 0x10000000:
            que->cnt_repeat = 1;
            break;
        case 0x20000000:
            que->cnt_repeat--;
            break;
        default:
            que->cnt_repeat = 0;
            break;
        }

        if (que->cnt_repeat > 0)
        {
            que->tmr_delay = que->tym_repeatDelay;
        }
        else
        {
            que->sndEvent = VSND_TYP_BOGUS;
        }

        if (que->sndEvent != VSND_TYP_BOGUS)
        {
            cnt++;
        }
    }

    return cnt;
}

// Plays a villain sound: the given sound, or one VSND_select_sound picks for the sound type.
// Flag 0x8000 marks a voice line: it is skipped while the next-voice timer runs, and while the
// voice channel is busy, when it restarts that timer at its configured length give or take an
// eighth. Flag 0x800 skips it while the effects channel is busy. Flags 0x1000, 0x2000 and 0x4000
// pick volume 1, 0.9 or 0.8 and priority 150, 100 or 50 (otherwise 0.75 and 100); in a boss
// battle the volume halves unless the villain has ability bit 0x1. With flag 0x200 it plays flat,
// otherwise in 3D at half volume out to dist. The sound's owner is the villain's address plus the
// low two flag bits, so each villain has four sound channels; the 3D call passes it as the
// entity. Returns the played sound's id, or 0.
static U32 VSND_goPlay(st_zEntVillain* vil, en_NPC_SOUND sndEvent, U32 sndExplicit, S32 flg_vsnd,
                       F32 dist)
{
    F32 vol;
    U32 xsid = 0;
    U32 owner;
    U32 priority;
    U32 aid_toplay;

    aid_toplay = sndExplicit ? sndExplicit : VSND_select_sound(vil, sndEvent);

    if (flg_vsnd & 0x8000 && !(vil->tmr_list[ZVIL_TTIL_NEXTVOICE] <= 0.0f))
    {
        return 0;
    }

    if (flg_vsnd & 0x8000 && VSND_chanIsBusy(vil, 1))
    {
        vil->tmr_list[ZVIL_TTIL_NEXTVOICE] =
            VCFG_tymref(vil, ZVIL_TTIL_NEXTVOICE) +
            0.25f * (iurand() - 0.5f) * VCFG_tymref(vil, ZVIL_TTIL_NEXTVOICE);
        return 0;
    }

    if (flg_vsnd & 0x800 && VSND_chanIsBusy(vil, 3))
    {
        return 0;
    }

    if (flg_vsnd & 0x1000)
    {
        vol = 1.0f;
        priority = 150;
    }
    else if (flg_vsnd & 0x2000)
    {
        vol = 0.9f;
        priority = 100;
    }
    else if (flg_vsnd & 0x4000)
    {
        vol = 0.8f;
        priority = 50;
    }
    else
    {
        vol = 0.75f;
        priority = 100;
    }

    if (g_scene_is_a_boss_battle && !(vil->ability & 0x1))
    {
        vol *= 0.5f;
    }

    owner = (U32)vil + (flg_vsnd & 3);
    if (aid_toplay && flg_vsnd & 0x200)
    {
        xsid = xSndPlay(aid_toplay, vol, vol, 0.0f, priority, 0x202, owner);
    }
    else if (aid_toplay)
    {
        xsid = xSndPlay3D(aid_toplay, 0.5f * vol, 0.0f, priority, 0x202, (_xEnt*)owner, dist);
    }

    return xsid;
}

// Finds a sound type's entry in g_sndProps (the VSND_LISTEND entry if it has none), hands it back
// through sprop_caller when that is given, and returns its play flags.
static S32 VSND_findProps(en_NPC_SOUND sndEvent, st_NPC_SOUND_PROPERTIES** sprop_caller)
{
    st_NPC_SOUND_PROPERTIES* sprop = g_sndProps;

    while (sprop->sndEvent != VSND_LISTEND)
    {
        if (sprop->sndEvent == sndEvent)
        {
            break;
        }
        sprop++;
    }

    if (sprop_caller)
    {
        *sprop_caller = sprop;
    }

    return sprop->flg_vsnd;
}

// Queues a sound type once, with its play flags (without the top four bits) and start delay from
// g_sndProps and the villain type's noise distance as its radius.
static inline S32 VSND_queueSound(st_zEntVillain* vil, en_NPC_SOUND sndEvent)
{
    st_NPC_SOUND_PROPERTIES* sprop = NULL;
    S32 flg_vsnd = VSND_findProps(sndEvent, &sprop);
    flg_vsnd &= 0x0FFFFFFF;
    return VSND_addQueueEntry(vil, sndEvent, flg_vsnd, g_npcTable[vil->asset->subtype].dst_noisy,
                              sprop->tym_delayStart, -1.0f, 0, 0);
}

// Whether a sound is playing on one of the villain's four sound channels (the low two bits of
// flg_chan pick the channel).
static inline S32 VSND_chanIsBusy(st_zEntVillain* vil, S32 flg_chan)
{
    return iSndIsPlaying(0, (U32)vil + (flg_chan & 3));
}

// Drops a bonus item at the villain, if its allow flags let it. Outside boss battles nothing drops
// while a dropped bonus item that is still out is within 2 units of the villain.
static void VBON_drop_bonusItem(st_zEntVillain* vil)
{
    S32 overlap = 0;
    S32 skip_distchk = 0;
    _xVec3 vec_tmp = { 0 };
    S32 i;
    st_NPC_BONUS* bon;

    if (!(vil->allow & 0x800))
    {
        return;
    }

    if (g_scene_is_a_boss_battle)
    {
        skip_distchk = 1;
    }

    for (i = 0; i < 11; i++)
    {
        if (skip_distchk)
        {
            break;
        }

        bon = &g_npcBonus[i];
        if (bon->pickup == NULL)
        {
            continue;
        }
        if (!(bon->flg_info & 0x1))
        {
            continue;
        }
        if (bon->pickup->state & 0x8)
        {
            continue;
        }

        xVec3Sub(&vec_tmp, xEntGetPos(bon->pickup), xEntGetPos(vil));
        if (xVec3Length2(&vec_tmp) < 4.0f)
        {
            overlap = 1;
            break;
        }
    }

    if (overlap)
    {
        return;
    }

    bon = VBON_choose_bonusItem((en_zEntVillainSubTypeID)vil->asset->subtype);
    if (bon == NULL)
    {
        return;
    }

    bon->flg_info |= 0x1;
    bon->pickup->flg_opts |= 0x3;
    bon->pickup->useThisEntPos = vil;
    zEntPickup_MakeDroppable(bon->pickup, xBoundCenter(&vil->bound), NULL);
    zEntPickup_Drop(bon->pickup);
}

// Heads the villain for his next path point: sets his move target to the point, his heading and
// direction toward it, domove bit 0x4 if move ability bit 0x1000 or 0x400 is set, and domove bits
// 0x800, 0x1, 0xA and 0x80000000. Without a next point it only looks up his name (nothing is done
// with it) and returns 0; otherwise it returns 1.
static S32 VOUT_patrol_move(st_zEntVillain* vil, _xScene*, F32, const _xEntFrame*)
{
    if (!vil->nav_next)
    {
        xSceneID2Name(globals.sceneCur, vil->id);
        return 0;
    }

    xVec3Copy(&vil->pos_moveto, zMovePointGetPos(vil->nav_next));
    vil->ang_moveto = VSUP_dir_toPos(vil, vil->pos_moveto, &vil->dir_moveto);

    if (vil->abl_move & 0x1000 || vil->abl_move & 0x400)
    {
        vil->domove |= 0x4;
    }

    vil->domove |= 0x800;
    vil->domove |= 0x1;
    vil->domove |= 0xA;
    vil->domove |= 0x80000000;
    return 1;
}

// Checks whether the villain has reached his move target: his squared flat distance to it, or the
// squared height difference if that is larger and move ability bit 0x1000 or 0x400 is set, must
// not exceed his type's squared arrival distance. On arrival his position becomes his guard spot
// unless flg_misc bit 0x40000000 is set, he gets the point's wait time on his path delay timer,
// asks for a new path point, sets his path-check timer to -1 and sends the point an arrive event.
// Returns 1 on arrival, 0 otherwise.
static S32 VOUT_chkdo_patrolArrived(st_zEntVillain* vil)
{
    _xVec3 entpos = { 0 };
    F32 dy;
    F32 dist = VSUP_ds2_toPos(vil, vil->pos_moveto);

    entpos = *xEntGetPos(vil);

    if (vil->abl_move & 0x1000 || vil->abl_move & 0x400)
    {
        dy = SQR(vil->pos_moveto.y - entpos.y);
        dist = MAX(dist, dy);
    }

    if (dist > g_npcTable[vil->asset->subtype].ds2_atpos)
    {
        return 0;
    }

    if (!(vil->flg_misc & 0x40000000))
    {
        xVec3Copy(&vil->pos_guard_home, xEntGetPos(vil));
    }

    vil->tmr_list[ZVIL_TDUR_NAVDELAY] = zMovePointGetDelay(vil->nav_next);
    vil->need_navpt = 1;
    vil->tmr_list[ZVIL_TTIL_CHKFORNAV] = -1.0f;
    zEntEvent(vil, vil->nav_next, eEventArrive);
    return 1;
}

// Checks whether the villain has reached his mini patrol target: the larger of his squared flat
// distance to it and (if move ability bit 0x1000 or 0x400 is set) the squared height difference
// must not exceed his type's squared arrival distance plus his squared stand-off. On arrival it
// clears flg_misc bit 0x80000000 if he has an anchor, otherwise bit 0x40000000. Returns 1 on
// arrival, 0 otherwise.
static S32 VOUT_chkdo_minipatrolArrived(st_zEntVillain* vil)
{
    _xVec3 entpos = { 0 };
    F32 dy = 0.0f;
    S32 amthere = 0;
    F32 ds2;

    entpos = *xEntGetPos(vil);

    if (vil->abl_move & 0x1000 || vil->abl_move & 0x400)
    {
        dy = SQR(vil->pos_moveto.y - entpos.y);
    }

    ds2 = VSUP_ds2_toPos(vil, vil->pos_moveto);
    if (MAX(ds2, dy) > vil->ds2_goto_keepaway + g_npcTable[vil->asset->subtype].ds2_atpos)
    {
        return amthere;
    }

    if (vil->vhf->query(vil, ZVIL_Q_HAVEANCHOR))
    {
        vil->flg_misc &= ~0x80000000;
    }
    else
    {
        vil->flg_misc &= ~0x40000000;
    }

    amthere = 1;
    return amthere;
}

// Heads the villain for what he follows: the spot he was sent to (flg_misc bit 0x10000000), the
// entity he trails (bit 0x20000000), or else his next path point. It sets his heading and
// direction toward it and the same domove bits as a patrol move. With none of the three it returns
// 0; otherwise 1.
static S32 VOUT_follow_move(st_zEntVillain* vil, _xScene*, F32, const _xEntFrame*)
{
    if (!vil->nav_next && !(vil->flg_misc & 0x10000000) && !(vil->flg_misc & 0x20000000))
    {
        return 0;
    }

    if (vil->flg_misc & 0x10000000)
    {
        xVec3Copy(&vil->pos_moveto, &vil->pos_goto);
    }
    else if (vil->flg_misc & 0x20000000)
    {
        xVec3Copy(&vil->pos_moveto, &xModelGetFrame(vil->tgt_follow->model)->pos);
    }
    else
    {
        xVec3Copy(&vil->pos_moveto, zMovePointGetPos(vil->nav_next));
    }

    vil->ang_moveto = VSUP_dir_toPos(vil, vil->pos_moveto, &vil->dir_moveto);

    if (vil->abl_move & 0x1000 || vil->abl_move & 0x400)
    {
        vil->domove |= 0x4;
    }

    vil->domove |= 0x800;
    vil->domove |= 0x1;
    vil->domove |= 0xA;
    vil->domove |= 0x80000000;
    return 1;
}

// Checks whether the villain has reached what he follows, as a patrol arrival does but with his
// squared stand-off added to the arrival distance. On arrival his position becomes his guard spot
// unless flg_misc bit 0x40000000 is set. If he was sent to a spot or trails an entity, the
// stand-off and those two flg_misc bits are cleared along with allow bit 0x1; otherwise he waits
// at the path point as on patrol. Returns 1 on arrival, 0 otherwise.
static S32 VOUT_chkdo_followArrived(st_zEntVillain* vil)
{
    _xVec3 entpos = { 0 };
    F32 dy;
    F32 dist = VSUP_ds2_toPos(vil, vil->pos_moveto);

    entpos = *xEntGetPos(vil);

    if (vil->abl_move & 0x1000 || vil->abl_move & 0x400)
    {
        dy = SQR(vil->pos_moveto.y - entpos.y);
        dist = MAX(dist, dy);
    }

    if (dist > vil->ds2_goto_keepaway + g_npcTable[vil->asset->subtype].ds2_atpos)
    {
        return 0;
    }

    if (!(vil->flg_misc & 0x40000000))
    {
        xVec3Copy(&vil->pos_guard_home, xEntGetPos(vil));
    }

    if (vil->flg_misc & 0x10000000 || vil->flg_misc & 0x20000000)
    {
        vil->ds2_goto_keepaway = 0.0f;
        vil->flg_misc &= ~0x10000000;
        vil->flg_misc &= ~0x20000000;
        vil->allow &= ~0x1;
    }
    else
    {
        vil->tmr_list[ZVIL_TDUR_NAVDELAY] = zMovePointGetDelay(vil->nav_next);
        vil->need_navpt = 1;
        vil->tmr_list[ZVIL_TTIL_CHKFORNAV] = -1.0f;
        zEntEvent(vil, vil->nav_next, eEventArrive);
    }
    return 1;
}

// Heads the villain for the player: his move target is the player's intercept point if precalc
// flg_plyr bit 0x400 is set, otherwise the centre of the player's bound. His heading is the one
// toward the player, and it sets domove bits 0x800, 0x1, 0xA, 0x4 (if move ability bit 0x1000 or
// 0x400 is set) and 0x80000000. Returns 1.
static S32 VOUT_chase_move(st_zEntVillain* vil, _xScene*, F32, const _xEntFrame*)
{
    if (vil->precalc.flg_plyr & 0x400)
    {
        xVec3Copy(&vil->pos_moveto, &vil->precalc.pos_intercept);
    }
    else
    {
        xVec3Copy(&vil->pos_moveto, xBoundCenter(&globals.player.ent.bound));
    }

    VSUP_ds2_toPos(vil, vil->pos_moveto);
    vil->ang_moveto = vil->precalc.ang_VtoP;

    vil->domove |= 0x800;
    vil->domove |= 0x1;
    vil->domove |= 0xA;

    if (vil->abl_move & 0x1000 || vil->abl_move & 0x400)
    {
        vil->domove |= 0x4;
    }

    vil->domove |= 0x80000000;
    return 1;
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
        if (wts)
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

// Heading (around the y axis) of a direction, from its x and z.
static inline F32 VSUP_ang_ofXZDir(_xVec3 dir)
{
    return atan2(dir.x, dir.z);
}

// Heading (around the y axis) from the entity to a position. If dir is given, it gets the unit
// direction to the position.
static F32 VSUP_dir_toPos(const _zEnt* ent, _xVec3 pos, _xVec3* dir)
{
    _xVec3 delta = { 0 };
    F32 ang;

    xVec3Sub(&delta, &pos, &xModelGetFrame(ent->model)->pos);
    xVec3Normalize(&delta, &delta);
    ang = xAngleClamp(VSUP_ang_ofXZDir(delta));
    if (dir)
    {
        xVec3Normalize(dir, &delta);
    }
    return ang;
}

// Squared flat (x and z) distance from the entity to a position.
static F32 VSUP_ds2_toPos(const _zEnt* ent, _xVec3 pos)
{
    _xVec3 delta = { 0 };

    xVec3Sub(&delta, &pos, &xModelGetFrame(ent->model)->pos);
    return delta.x * delta.x + delta.z * delta.z;
}

// Squared flat (x and z) distance from the villain to another entity's position.
static inline F32 VSUP_ds2_toEnt(const _zEnt* ent, const _xEnt* tgt)
{
    _xVec3 pos = { 0 };

    xVec3Copy(&pos, xEntGetPos(tgt));
    return VSUP_ds2_toPos(ent, pos);
}

// Heading from the entity to the centre of another entity's bound; see VSUP_dir_toPos.
static inline F32 VSUP_dir_toEnt(const _zEnt* ent, const _xEnt* tgt, _xVec3* dir)
{
    _xVec3 ctr = { 0 };

    xVec3Copy(&ctr, xBoundCenter(&tgt->bound));
    return VSUP_dir_toPos(ent, ctr, dir);
}

// Whether the entity's bound centre is within half the villain's field of view either side of
// its heading. A villain with no field of view sees nothing.
static S32 VSUP_ent_inFOV(const st_zEntVillain* vil, const _zEnt* ent)
{
    S32 result = 0;
    F32 ang = VSUP_dir_toEnt(vil, ent, NULL);
    F32 ang_face = vil->frame->rot.angle;

    if (vil->fov_detect <= 0.0f)
    {
        result = 0;
    }
    else if (fabsf(ang_face - ang) < 0.5f * vil->fov_detect)
    {
        result = 1;
    }
    return result;
}

// Heading from the villain's facing to the entity, clamped. If qdrant is given, it gets the
// entity's quadrant: front within 45 degrees either side of the facing, then right, back and
// left. The left test repeats the front range's bounds, so left is never given and qdrant stays
// as it was for an angle there.
static inline F32 VSUP_ent_AOI(const st_zEntVillain* vil, const _zEnt* ent, en_NPC_QUADRANT* qdrant)
{
    F32 ang_diff = VSUP_dir_toEnt(vil, ent, NULL) - vil->frame->rot.angle;
    F32 ang_aoi;

    if (qdrant)
    {
        ang_aoi = xAngleClamp(ang_diff + PI / 4);
        if (ang_aoi > 0.0f && ang_aoi <= PI / 2)
        {
            *qdrant = ZVIL_QUAD_FRONT;
        }
        else if (ang_aoi > PI / 2 && ang_aoi <= PI)
        {
            *qdrant = ZVIL_QUAD_RIGHT;
        }
        else if (ang_aoi > PI && ang_aoi < 3 * PI / 2)
        {
            *qdrant = ZVIL_QUAD_BACK;
        }
        else if (ang_aoi > 0.0f && ang_aoi < PI / 2)
        {
            *qdrant = ZVIL_QUAD_LEFT;
        }
    }
    return xAngleClamp(ang_diff);
}

// Whether nothing in the scene blocks a ray from the villain's bound centre toward the entity's,
// starting just outside the villain's bound and reaching no farther than its detection range.
// The ray is cast only with chance rand; otherwise the previous answer (prev) stands.
static S32 VSUP_ent_inLOS_timz(const st_zEntVillain* vil, const _zEnt* ent, S32 prev, F32 rand)
{
    S32 result;

    if (!xUtil_yesno(rand))
    {
        return prev;
    }

    _xRay3 r = { 0 };
    _xVec3 mypos = { 0 };

    xVec3Copy(&mypos, xBoundCenter(&vil->bound));
    r.min_t = 0.001f + vil->bound.sph.r;
    xVec3Sub(&r.dir, xBoundCenter(&ent->bound), &mypos);
    r.max_t = xVec3Normalize(&r.dir, &r.dir);
    r.max_t = MIN(xsqrt(vil->ds2_detect), r.max_t);
    xVec3SMul(&r.origin, &r.dir, r.min_t);
    xVec3AddTo(&r.origin, &mypos);
    r.flags = 0x800;
    result = !xRayHitsSceneFast(&r, globals.sceneCur);
    return result;
}

// Puts the world position of one of the villain's model vertices in pos and returns 1, or returns 0
// when that vertex is not set up.
static inline S32 VSUP_vertPos(const st_zEntVillain* vil, en_ZVIL_MODEL_VERTEX vert, _xVec3* pos)
{
    if (!(vil->flg_vert & (1 << vert)))
    {
        return 0;
    }

    iModelTagEval(vil->model->Data, (iModelTag*)&vil->tag_vert[vert], vil->model->Mat, pos);
    return 1;
}

// Line of sight from the villain to the entity; see VSUP_ent_inLOS_timz.
static S32 VSUP_ent_inLOS(const st_zEntVillain* vil, const _zEnt* ent, S32 prev, F32 rand)
{
    return VSUP_ent_inLOS_timz(vil, ent, prev, rand);
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
