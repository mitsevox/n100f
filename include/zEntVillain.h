#ifndef ZENTVILLAIN_H
#define ZENTVILLAIN_H

#include "iModel.h"
#include "zEnt.h"
#include "zFeet.h"

struct _zEntBubble;
struct _zMovePoint;
struct xAnimSingle;
struct xAnimTransition;
struct zEntProjectile;
struct zSFX;
struct st_zEntVillain;

enum en_zEntVillainSubTypeID
{
    eEntVillainType_Undefined = -1,
    eEntVillainTypeBat,
    eEntVillainTypeBlackKnight,
    eEntVillainTypeBuckMasters,
    eEntVillainTypeCreeper,
    eEntVillainTypeSeaCreature,
    eEntVillainTypeGargoyle,
    eEntVillainTypeGhost,
    eEntVillainTypeGhostDiver,
    eEntVillainTypeGreenGhost,
    eEntVillainTypeHeadlessSpecter,
    eEntVillainTypePhantom,
    eEntVillainTypeRedbeard,
    eEntVillainTypeRobot,
    eEntVillainTypeScarecrow,
    eEntVillainTypeShark,
    eEntVillainTypeSpaceKook,
    eEntVillainTypeTarMonster,
    eEntVillainTypeWitch,
    eEntVillainTypeWitchDoctor,
    eEntVillainTypeWolfman,
    eEntVillainTypeZombie,
    eEntVillainTypeCrab,
    eEntVillainTypeRat,
    eEntVillainTypeFlyingFish,
    eEntVillainTypeSpider,
    eEntVillainTypeHand,
    eEntVillainTypeKillerPlant,
    eEntVillainTypeBird,
    eEntVillainTypeShaggy0,
    eEntVillainTypeShaggy1,
    eEntVillainTypeShaggy2,
    eEntVillainTypeShaggy3,
    eEntVillainTypeShaggy4,
    eEntVillainTypeShaggy5,
    eEntVillainTypeShaggy6,
    eEntVillainTypeShaggy7,
    eEntVillainTypeShaggy8,
    eEntVillainTypeShaggy9,
    eEntVillainTypeFred,
    eEntVillainTypeDaphne,
    eEntVillainTypeVelma,
    eEntVillainBossBlackKnight,
    eEntVillainBossGreenGhost,
    eEntVillainBossRedbeard,
    eEntVillainBossMastermind,
    eEntVillainCaptainMoody,
    eEntVillainCaveMan,
    eEntVillainHolly,
    eEntVillainGroundsKeeper,
    eEntVillainProfessor,
    eEntVillainType_NoMore,
};

enum en_NPC_PERSONALITIES
{
    ZVIL_PERSON_BOGUS = -1,
    ZVIL_PERSON_NORMAL,
    ZVIL_PERSON_WIMP,
    ZVIL_PERSON_WIRED,
    ZVIL_PERSON_BULLY,
    ZVIL_PERSON_LEADER,
    ZVIL_PERSON_AGGRESSIVE,
    ZVIL_PERSON_BOSS,
    ZVIL_PERSON_SCOOBYGANG,
};

enum en_NPC_MODE
{
    ZVIL_MODE_NOCHANGE,
    ZVIL_MODE_SHUTDOWN,
    ZVIL_MODE_READY,
    ZVIL_MODE_NORMAL,
    ZVIL_MODE_ALERT,
    ZVIL_MODE_BATTLE,
    ZVIL_MODE_RETREAT,
    ZVIL_MODE_DEATH,
    ZVIL_MODE_NOMORE,
};

enum en_BEHAVIOUR_GOAL
{
    ZVIL_GOAL_NONE = -1,
    ZVIL_GOAL_AFTERLIFE,
    ZVIL_GOAL_BASEINIT,
    ZVIL_GOAL_STARTUP,
    ZVIL_GOAL_GOONLINE,
    ZVIL_GOAL_IDLE,
    ZVIL_GOAL_BORED,
    ZVIL_GOAL_PATROL,
    ZVIL_GOAL_MINIPATROL,
    ZVIL_GOAL_FOLLOW,
    ZVIL_GOAL_GUARD,
    ZVIL_GOAL_AMBUSH,
    ZVIL_GOAL_LEAP,
    ZVIL_GOAL_TALK,
    ZVIL_GOAL_LOOKFORHERO,
    ZVIL_GOAL_LISTEN,
    ZVIL_GOAL_SURPRIZE,
    ZVIL_GOAL_INVESTIGATE,
    ZVIL_GOAL_ATTACKREADY,
    ZVIL_GOAL_BULLREADY,
    ZVIL_GOAL_CHASE,
    ZVIL_GOAL_CQCATTACK,
    ZVIL_GOAL_LOBBING,
    ZVIL_GOAL_SPELLCAST,
    ZVIL_GOAL_TAUNT,
    ZVIL_GOAL_DAZED,
    ZVIL_GOAL_SOAPED,
    ZVIL_GOAL_GUMMED,
    ZVIL_GOAL_HURT,
    ZVIL_GOAL_BLOCKHEAD,
    ZVIL_GOAL_TELEPORT,
    ZVIL_GOAL_KNOCKBACK,
    ZVIL_GOAL_ACTION,
    ZVIL_GOAL_DIE,
    ZVIL_GOAL_NOMORE,
};

enum en_NPC_ANIMSTATES
{
    ZVIL_ANIM_UNKNOWN,
    ZVIL_ANIM_IDLE,
    ZVIL_ANIM_FIDGET_IDLE1,
    ZVIL_ANIM_FIDGET_IDLE2,
    ZVIL_ANIM_BORED,
    ZVIL_ANIM_FIDGET_BORED1,
    ZVIL_ANIM_FIDGET_BORED2,
    ZVIL_ANIM_MOVE,
    ZVIL_ANIM_POSE,
    ZVIL_ANIM_TURN,
    ZVIL_ANIM_CHASE,
    ZVIL_ANIM_ACTION_NOMOVE,
    ZVIL_ANIM_ACTION_MOVING,
    ZVIL_ANIM_SURPRIZE,
    ZVIL_ANIM_TAUNT,
    ZVIL_ANIM_TAUNT1,
    ZVIL_ANIM_TAUNT2,
    ZVIL_ANIM_HEAR_START,
    ZVIL_ANIM_HEAR_LOOP,
    ZVIL_ANIM_LOOK_START,
    ZVIL_ANIM_LOOK_LOOP,
    ZVIL_ANIM_INVESTIGATE_START,
    ZVIL_ANIM_INVESTIGATE,
    ZVIL_ANIM_ATTACKIDLE,
    ZVIL_ANIM_ATTACK,
    ZVIL_ANIM_ATTACK1,
    ZVIL_ANIM_ATTACK2,
    ZVIL_ANIM_COMBO,
    ZVIL_ANIM_COMBO1,
    ZVIL_ANIM_COMBO2,
    ZVIL_ANIM_LOB_ATTACK,
    ZVIL_ANIM_LOB_COMBO,
    ZVIL_ANIM_EXT_ATTACK_START,
    ZVIL_ANIM_EXT_ATTACK_LOOP,
    ZVIL_ANIM_EXT_COMBO,
    ZVIL_ANIM_HIT_BY_GUM,
    ZVIL_ANIM_GUMMED,
    ZVIL_ANIM_GUMMED_RESIST,
    ZVIL_ANIM_GUMMED_BREAKFREE,
    ZVIL_ANIM_HIT_WHEN_GUMMED,
    ZVIL_ANIM_BOUNCED_WHEN_GUMMED,
    ZVIL_ANIM_HIT_BY_SOAP,
    ZVIL_ANIM_SOAPED,
    ZVIL_ANIM_SOAPED_RESIST,
    ZVIL_ANIM_SOAPED_BREAKFREE,
    ZVIL_ANIM_HIT_WHEN_SOAPED,
    ZVIL_ANIM_BOUNCED_WHEN_SOAPED,
    ZVIL_ANIM_DAZED_BEGIN,
    ZVIL_ANIM_DAZED,
    ZVIL_ANIM_DAZED_RECOVER,
    ZVIL_ANIM_DEATH,
    ZVIL_ANIM_DEATH_ABOVE,
    ZVIL_ANIM_DEATH_DAZED,
    ZVIL_ANIM_DEATH_IDLE,
    ZVIL_ANIM_JUMP_BEGIN,
    ZVIL_ANIM_JUMP_LOOP,
    ZVIL_ANIM_JUMP_LAND,
    ZVIL_ANIM_FALL_LOOP,
    ZVIL_ANIM_FALL_SPLAT,
    ZVIL_ANIM_BK_SCARYIDLE,
    ZVIL_ANIM_BK_WALK,
    ZVIL_ANIM_BK_TURN,
    ZVIL_ANIM_BK_PATROL,
    ZVIL_ANIM_BK_PATROLFLIP,
    ZVIL_ANIM_BK_ATTACK,
    ZVIL_ANIM_BK_SHOCK,
    ZVIL_ANIM_BK_SHOCKSKEL,
    ZVIL_ANIM_BK_GLOAT,
    ZVIL_ANIM_BK_GLOAT1,
    ZVIL_ANIM_BK_GLOAT2,
    ZVIL_ANIM_BK_DEATH,
    ZVIL_ANIM_GG_CHASE,
    ZVIL_ANIM_GG_CHASELEFT,
    ZVIL_ANIM_GG_CHASERIGHT,
    ZVIL_ANIM_GG_BUILDBEGIN,
    ZVIL_ANIM_GG_BUILDLOOP,
    ZVIL_ANIM_GG_ATTACK,
    ZVIL_ANIM_GG_TAUNT,
    ZVIL_ANIM_GG_TAUNT1,
    ZVIL_ANIM_GG_HURT,
    ZVIL_ANIM_GG_HURT1,
    ZVIL_ANIM_GG_HURT2,
    ZVIL_ANIM_RB_IDLEWIND,
    ZVIL_ANIM_RB_PATROL,
    ZVIL_ANIM_RB_TURN,
    ZVIL_ANIM_RB_CHASE,
    ZVIL_ANIM_RB_TAUNTFEEL,
    ZVIL_ANIM_RB_TAUNTTHROAT,
    ZVIL_ANIM_RB_TAUNTLAUGH,
    ZVIL_ANIM_RB_ATTACKPUNCH,
    ZVIL_ANIM_RB_ATTACKTHROW,
    ZVIL_ANIM_RB_ATTACKWAIT,
    ZVIL_ANIM_RB_ATTACKCATCH,
    ZVIL_ANIM_RB_REACT,
    ZVIL_ANIM_RB_LOOK_BEGIN,
    ZVIL_ANIM_RB_LOOK_LOOP,
    ZVIL_ANIM_RB_LOOK_END,
    ZVIL_ANIM_RB_DEATH_LOOP,
    ZVIL_ANIM_MM_FP_MOVE,
    ZVIL_ANIM_MM_FP_TAUNT,
    ZVIL_ANIM_MM_FP_ACTION,
    ZVIL_ANIM_MM_FP_JUMP,
    ZVIL_ANIM_MM_BA_IDLE,
    ZVIL_ANIM_MM_BA_MOVE,
    ZVIL_ANIM_MM_BA_TURN,
    ZVIL_ANIM_MM_BA_BACKSTEP,
    ZVIL_ANIM_MM_BA_TAUNT,
    ZVIL_ANIM_MM_BA_REACT,
    ZVIL_ANIM_MM_BA_ATTACK_BEGIN,
    ZVIL_ANIM_MM_BA_ATTACK_LOOP,
    ZVIL_ANIM_MM_BA_ATTACK,
    ZVIL_ANIM_MM_BA_SHIELD_BEGIN,
    ZVIL_ANIM_MM_BA_SHIELD_LOOP,
    ZVIL_ANIM_MM_BA_SHIELD_END,
    ZVIL_ANIM_MM_BA_STUNNED_BEGIN,
    ZVIL_ANIM_MM_BA_STUNNED_LOOP,
    ZVIL_ANIM_MM_BA_STUNNED_END,
    ZVIL_ANIM_MM_BA_KNOCKED_BEGIN,
    ZVIL_ANIM_MM_BA_KNOCKED_LOOP,
    ZVIL_ANIM_MM_BA_KNOCKED_END,
    ZVIL_ANIM_MM_BA_DEATH_BEGIN,
    ZVIL_ANIM_MM_BA_DEATH,
    ZVIL_ANIM_GANG_ALTIDLE1,
    ZVIL_ANIM_GANG_ALTIDLE2,
    ZVIL_ANIM_GANG_ALTIDLE3,
    ZVIL_ANIM_GANG_FIDGET,
    ZVIL_ANIM_GANG_FIDGET1,
    ZVIL_ANIM_GANG_FIDGET2,
    ZVIL_ANIM_GANG_FIDGET3,
    ZVIL_ANIM_GANG_TURN,
    ZVIL_ANIM_GANG_WALK,
    ZVIL_ANIM_GANG_RUN,
    ZVIL_ANIM_GANG_RUNCHASED,
    ZVIL_ANIM_GANG_ACTION,
    ZVIL_ANIM_GANG_BOSSACTION,
    ZVIL_ANIM_GANG_TALK,
    ZVIL_ANIM_GANG_TALK1,
    ZVIL_ANIM_GANG_TALK2,
    ZVIL_ANIM_GANG_TALK3,
    ZVIL_ANIM_GANG_TALKANDTURN,
    ZVIL_ANIM_SHAM_MOVE,
    ZVIL_ANIM_SHAM_TAUNT,
    ZVIL_ANIM_CRIT_MOVE,
    ZVIL_ANIM_CRIT_BATSLEEP,
    ZVIL_ANIM_CRIT_SPIDERDROP,
    ZVIL_ANIM_CRIT_SMASHED,
    ZVIL_ANIM_SIMP_ACTION,
    ZVIL_ANIM_NOMORE,
};

enum en_NPC_SOUND
{
    VSND_TYP_BOGUS = -1,
    VSND_TYP_ENCOUNTER,
    VSND_TYP_SPOOKYROOM,
    VSND_TYP_DIALOGUE,
    VSND_TYP_MOVE,
    VSND_TYP_MOVE_ALT,
    VSND_TYP_IDLESLOP,
    VSND_TYP_SNORE,
    VSND_TYP_MUMBLE,
    VSND_TYP_SPLISH,
    VSND_TYP_SPLASH,
    VSND_TYP_CLANKING,
    VSND_TYP_LOOK,
    VSND_TYP_LISTEN,
    VSND_TYP_CURIOUS,
    VSND_TYP_EXCLAIM,
    VSND_TYP_AMBUSH,
    VSND_TYP_ATTACK,
    VSND_TYP_ATTACKSWIPE,
    VSND_TYP_KICK,
    VSND_TYP_PUNCH,
    VSND_TYP_WEPLAUNCH,
    VSND_TYP_WEPFLIGHT,
    VSND_TYP_WEPEXPLODE,
    VSND_TYP_LAUGH,
    VSND_TYP_DIZZY,
    VSND_TYP_SOAPBEGIN,
    VSND_TYP_SOAPSTOP,
    VSND_TYP_SOAPED,
    VSND_TYP_GUMBEGIN,
    VSND_TYP_GUMSTOP,
    VSND_TYP_GUMMED,
    VSND_TYP_GUMBOUNCE,
    VSND_TYP_GUMSTRETCH,
    VSND_TYP_OUCH,
    VSND_TYP_OUCHCRASH,
    VSND_TYP_FALL,
    VSND_TYP_DIE,
    VSND_TYP_HOLOFADE,
    VSND_TYP_PLAYERDEATH,
    VSND_TYP_PRJ_LAUNCH,
    VSND_TYP_PRJ_FLIGHT,
    VSND_TYP_PRJ_EXPLODE,
    VSND_TYP_PRJ_SPLAT,
    VSND_TYP_SPELL_CAST,
    VSND_TYP_SPELL_PLAY,
    VSND_TYP_SPELL_LOOP,
    VSND_TYP_ZSFXTALK,
    VSND_TYP_CHEERING,
    VSND_TYP_ONELINER,
    VSND_TYP_ONELINERTOO,
    VSND_TYP_NEEDTARBOOTS,
    VSND_TYP_EXTRASPRING,
    VSND_TYP_NEEDFLOAT,
    VSND_TYP_HAVEFLOAT,
    VSND_TYP_BKZAPPED,
    VSND_TYP_BKDIE,
    VSND_TYP_BLOCKING,
    VSND_TYP_ESCAPING,
    VSND_TYP_TRASHTALK,
    VSND_TYP_FOREPLAYHURT,
    VSND_TYP_BUTTONPRESS,
    VSND_TYP_ATTACKBUILD,
    VSND_TYP_ALMOSTKILLED,
    VSND_TYP_DISAPPEAR,
    VSND_TYP_REAPPEAR,
    VSND_TYP_MMDIE,
    VSND_TYP_MMLAUGH,
    ZVIL_SND_NOMORE,
    VSND_LISTEND = 32768,
};

enum en_NPC_STIMULUS
{
    ZVIL_STIM_NONE,
    ZVIL_STIM_OFTHEGODS,
    ZVIL_STIM_MAKEREADY,
    ZVIL_STIM_EXITGOAL,
    ZVIL_STIM_ENTERGOAL,
    ZVIL_STIM_TRANGOAL,
    ZVIL_STIM_WAKEUP,
    ZVIL_STIM_GOTOSLEEP,
    ZVIL_STIM_GOTOPOS,
    ZVIL_STIM_GOTOWIDGET,
    ZVIL_STIM_FOLLOWWIDGET,
    ZVIL_STIM_STARTPATROL,
    ZVIL_STIM_STOPPATROL,
    ZVIL_STIM_STARTGUARD,
    ZVIL_STIM_STOPGUARD,
    ZVIL_STIM_TRESPASS,
    ZVIL_STIM_ENDTRESPASS,
    ZVIL_STIM_STARTTALK,
    ZVIL_STIM_STOPTALK,
    ZVIL_STIM_LOOKFORHERO,
    ZVIL_STIM_SAWHERO,
    ZVIL_STIM_HEARDHERO,
    ZVIL_STIM_SHOCKED,
    ZVIL_STIM_ATTACKHERO,
    ZVIL_STIM_DOACTION,
    ZVIL_STIM_DOTAUNT,
    ZVIL_STIM_COLLIDE,
    ZVIL_STIM_HEADBUTT,
    ZVIL_STIM_BOUNCED,
    ZVIL_STIM_BUMPED,
    ZVIL_STIM_STUNNING,
    ZVIL_STIM_KILLED,
    ZVIL_STIM_SOAPED,
    ZVIL_STIM_GUMMED,
    ZVIL_STIM_KILLSELF,
    ZVIL_STIM_GOTOAFTERLIFE,
    ZVIL_STIM_GUARDWIDGET,
    ZVIL_STIM_GANGBOSSACTION,
    ZVIL_STIM_HURTBOSS,
    ZVIL_STIM_MMPUSHBUTTON,
    ZVIL_STIM_NOMORE,
};

enum en_NPC_TIMERS
{
    ZVIL_TTIL_FIDGET,
    ZVIL_TTIL_BORED,
    ZVIL_TTIL_TAUNT,
    ZVIL_TTIL_CHKFORNAV,
    ZVIL_TTIL_RECOVERY,
    ZVIL_TTIL_NEXTCHASE,
    ZVIL_TTIL_ANIMSOUND,
    ZVIL_TTIL_NEXTVOICE,
    ZVIL_TTIL_QUIP_OK,
    ZVIL_TDUR_STDBY,
    ZVIL_TDUR_ALERT,
    ZVIL_TDUR_TAUNT,
    ZVIL_TDUR_DAZED,
    ZVIL_TDUR_GUM,
    ZVIL_TDUR_SOAP,
    ZVIL_TDUR_LOOK,
    ZVIL_TDUR_HEAR,
    ZVIL_TDUR_SEARCH,
    ZVIL_TDUR_EXTEND,
    ZVIL_TDUR_LOBRELOAD,
    ZVIL_TDUR_EXTRELOAD,
    ZVIL_TDUR_BUILDUP,
    ZVIL_TDUR_HURT,
    ZVIL_TDUR_DECAY,
    ZVIL_TDUR_NAVDELAY,
    ZVIL_TWIN_MINGUM,
    ZVIL_TWIN_POSTATTACK,
    ZVIL_TWIN_IGNORESNEAK,
    ZVIL_TYMR_MINANIMPLAY,
    ZVIL_TYMR_CURGOAL,
    ZVIL_TYMR_GENERIC,
    ZVIL_TYMR_WEPLIFE,
    ZVIL_TYMR_NOMORE,
};

enum en_NPC_QUADRANT
{
    ZVIL_QUAD_UNKNOWN,
    ZVIL_QUAD_FRONT,
    ZVIL_QUAD_BACK,
    ZVIL_QUAD_LEFT,
    ZVIL_QUAD_RIGHT,
};

enum en_ZVIL_MODEL_VERTEX
{
    ZVIL_MDLVERT_GUM_LFOOT,
    ZVIL_MDLVERT_GUM_RFOOT,
    ZVIL_MDLVERT_GUM_ELBOW,
    ZVIL_MDLVERT_GUM_CUSTA,
    ZVIL_MDLVERT_GUM_CUSTB,
    ZVIL_MDLVERT_WEAPON,
    ZVIL_MDLVERT_ATTACK,
    ZVIL_MDLVERT_ATTACKALT,
    ZVIL_MDLVERT_DMG_BOUNCE,
    ZVIL_MDLVERT_LOSEYEBALL,
    ZVIL_MDLVERT_SWORDHANDLE,
    ZVIL_MDLVERT_SWORDPIVOT,
    ZVIL_MDLVERT_NOMORE,
};

enum en_NPC_INI_PARAMETERS
{
    ZVIL_PARM_NONE = -1,
    ZVIL_PARM_HIT_POINTS,
    ZVIL_PARM_BUBBLED_TIME,
    ZVIL_PARM_GUMMED_TIME,
    ZVIL_PARM_DAZED_TIME,
    ZVIL_PARM_EXTEND_RANGE,
    ZVIL_PARM_EXTEND_RATE,
    ZVIL_PARM_EXTEND_WIDTH,
    ZVIL_PARM_EXTEND_TIME,
    ZVIL_PARM_FOV_ANGLE,
    ZVIL_PARM_GLOAT_TIME,
    ZVIL_PARM_LOB_RANGE,
    ZVIL_PARM_LOB_SPEED,
    ZVIL_PARM_LOB_SALVO,
    ZVIL_PARM_LOB_PRJTYPE,
    ZVIL_PARM_LOB_RELOAD_TIME,
    ZVIL_PARM_LOB_ARC,
    ZVIL_PARM_LOB_HEAVY,
    ZVIL_PARM_DETECT_RADIUS,
    ZVIL_PARM_DETECT_HEIGHT,
    ZVIL_PARM_DETECT_HYTOFF,
    ZVIL_PARM_PURSUE_RADIUS,
    ZVIL_PARM_MOVE_RATE,
    ZVIL_PARM_CHASE_RATE,
    ZVIL_PARM_TURN_RATE,
    ZVIL_PARM_MDLSCL_BASE,
    ZVIL_PARM_SANIM_IDLE,
    ZVIL_PARM_SANIM_MOVE,
    ZVIL_PARM_SANIM_CHASE,
    ZVIL_PARM_WANDER_ARC,
    ZVIL_PARM_VERT_GUM_LFOOT,
    ZVIL_PARM_VERT_GUM_RFOOT,
    ZVIL_PARM_VERT_GUM_ELBOW,
    ZVIL_PARM_VERT_GUM_CUSTA,
    ZVIL_PARM_VERT_GUM_CUSTB,
    ZVIL_PARM_VERT_WEAPON,
    ZVIL_PARM_VERT_ATTACK,
    ZVIL_PARM_VERT_ATTACKALT,
    ZVIL_PARM_VERT_DMG_BOUNCE,
    ZVIL_PARM_VERT_LOSEYEBALL,
    ZVIL_PARM_VERT_RBSWORDHANDLE,
    ZVIL_PARM_VERT_RBSWORDPIVOT,
    ZVIL_PARM_BND_SPH_CENTER,
    ZVIL_PARM_BND_SPH_RADIUS,
    ZVIL_PARM_BND2_SPH_CENTER,
    ZVIL_PARM_BND2_SPH_RADIUS,
    ZVIL_PARM_SPAWNMASK,
    ZVIL_PARM_MINDIFFICULTY,
    ZVIL_PARM_DMG_BEGTIME,
    ZVIL_PARM_DMG_ENDTIME,
    ZVIL_PARM_NOMORE,
};

enum en_zEntVillain_StartBehaviour
{
    ZVIL_START_PREBIRTH,
    ZVIL_START_DEFAULT,
    ZVIL_START_SLEEPING,
    ZVIL_START_AMBUSH,
};

enum en_ZVIL_FLAG_GROUPS
{
    ZVIL_FGRP_PHYS,
    ZVIL_FGRP_CHK,
    ZVIL_FGRP_PEN,
    ZVIL_FGRP_CHKBY,
    ZVIL_FGRP_PENBY,
    ZVIL_FGRP_ABLE,
    ZVIL_FGRP_ATAK,
    ZVIL_FGRP_DTCT,
    ZVIL_FGRP_MOVE,
    ZVIL_FGRP_SPYK,
    ZVIL_FGRP_VULN,
    ZVIL_FGRP_ALOW,
    ZVIL_FGRP_NOMORE,
};

enum en_ZVIL_QUERY
{
    ZVIL_Q_DOUPDATE,
    ZVIL_Q_ALIVE,
    ZVIL_Q_ASLEEP,
    ZVIL_Q_PITFALL,
    ZVIL_Q_HAVEANCHOR,
    ZVIL_Q_HAVEGUARDPOS,
    ZVIL_Q_DOFOLLOW,
    ZVIL_Q_NEEDNAV,
    ZVIL_Q_HAVENAV,
    ZVIL_Q_CANGUARD,
    ZVIL_Q_CANPATROL,
    ZVIL_Q_CANCHASE,
    ZVIL_Q_RUNAWAY,
    ZVIL_Q_CANPURSUE,
    ZVIL_Q_CANATTACK,
    ZVIL_Q_SHOULDATTACK,
    ZVIL_Q_CANDETECT,
    ZVIL_Q_PLYRDETECTED,
    ZVIL_Q_PLYRTOOCLOSE,
    ZVIL_Q_PLYRNEAR,
    ZVIL_Q_PLYRINFOV,
    ZVIL_Q_PLYRINLOS,
    ZVIL_Q_CANSEEPLYR,
    ZVIL_Q_CANHEARPLYR,
    ZVIL_Q_PLYRSEEN,
    ZVIL_Q_PLYRHEARD,
    ZVIL_Q_NOMORE,
};

enum en_ZVIL_NOTICE
{
    ZVIL_NOTE_PREPARE,
    ZVIL_NOTE_PRECALC,
    ZVIL_NOTE_OLDTIME,
    ZVIL_NOTE_NOMORE,
};

enum en_ZVIL_GOAL_SPOT
{
    ZVIL_GSPOT_START,
    ZVIL_GSPOT_RESUME,
    ZVIL_GSPOT_FIDGET,
    ZVIL_GSPOT_ATTACKLINGER,
    ZVIL_GSPOT_KICKATTACK,
    ZVIL_GSPOT_BUBHIT,
    ZVIL_GSPOT_BUBBOUNCE,
    ZVIL_GSPOT_BUBRESIST,
    ZVIL_GSPOT_BUBBREAKFREE,
    ZVIL_GSPOT_STATERECOVER,
    ZVIL_GSPOT_ACTION_ALTA,
    ZVIL_GSPOT_MOVELEFT,
    ZVIL_GSPOT_MOVERIGHT,
    ZVIL_GSPOT_TAUNT_ALTA,
    ZVIL_GSPOT_TAUNT_ALTB,
    ZVIL_GSPOT_HURT_ALTA,
    ZVIL_GSPOT_HURT_ALTB,
};

enum en_ZVILLAIN_BUBBLE_RESPONSE
{
    ZVIL_BUBBLE_NONE,
    ZVIL_BUBBLE_ENVELOP,
    ZVIL_BUBBLE_BREAK,
    ZVIL_BUBBLE_BOUNCE,
};

enum en_ZVILLAIN_HEADBUTT_RESPONSE
{
    ZVIL_HEADBUTT_NONE,
    ZVIL_HEADBUTT_NORMAL,
    ZVIL_HEADBUTT_CONTINUE,
    ZVIL_HEADBUTT_HITGANG,
    ZVIL_HEADBUTT_DAMAGE,
};

enum en_ZVILLAIN_BOUNCE_RESPONSE
{
    ZVIL_BOUNCE_NONE,
    ZVIL_BOUNCE_NORMAL,
    ZVIL_BOUNCE_GUM,
    ZVIL_BOUNCE_DAMAGE,
    ZVIL_BOUNCE_LAND,
};

enum en_ZVILLAIN_BUMPED_RESPONSE
{
    ZVIL_BUMPED_NONE,
    ZVIL_BUMPED_SOAP,
    ZVIL_BUMPED_GUM,
    ZVIL_BUMPED_DAMAGE,
    ZVIL_BUMPED_ATTACK,
};

struct st_ZVIL_PRECALC
{
    S32 flg_status;
    F32 ds2_VtoP;
    F32 dst_VtoP;
    F32 ang_VtoP;
    _xVec3 dir_VtoP;
    _xVec3 vec_VtoP;
    U32 plyr_mvinf;
    S32 flg_plyr;
    S32 flg_xtra;
    F32 dif_plyrCtr;
    F32 dif_vilCtr;
    F32 hyt_vil;
    F32 hyt_plyr;
    F32 ds2_anchor;
    F32 ds2_grdpos;
    F32 cos_faceang;
    F32 sin_faceang;
    F32 ds2_plyrToAnchor;
    _xVec3 pos_intercept;
};

struct st_NPC_SOUND_PROPERTIES
{
    en_NPC_SOUND sndEvent;
    S32 flg_vsnd;
    F32 tym_delayStart;
};

struct st_NPC_SOUND_QUEUE
{
    en_NPC_SOUND sndEvent;
    U32 sndDirect;
    S32 flg_frolic;
    F32 tmr_delay;
    S32 cnt_repeat;
    F32 radius;
    F32 tym_repeatDelay;
};

struct p2EntVillainAsset
{
    F32 activateRadius;
    F32 activateFOV;
    F32 detectHeight;
    F32 detectHeightOffset;
    F32 speedMovement;
    F32 speedPursue;
    F32 speedTurn;
    F32 pursuitRange;
    U16 durDazedState;
    U16 durGloatState;
    U16 durGummedState;
    U16 durBubbleState;
    U8 hitpoints;
    U8 behaviorState;
    U16 pad;
    U32 villFlags;
    F32 lobSpeed;
    F32 lobDurReload;
    F32 lobRange;
    U32 lobSalvo;
    U32 projectileTypeID;
    U32 mvptBullseyeID;
    F32 lobArcness;
    F32 lobHeavy;
    F32 extenderRange;
    F32 extenderWidth;
    F32 extenderDuration;
    F32 extenderRate;
    F32 extenderReloadTime;
    U32 movePointAssetID;
    U32 pathAssetID;
    S32 minPlayerPowerups;
    S32 minGameDifficulty;
};

struct st_ZVIL_CONFIG_REFERENCE
{
    p2EntVillainAsset* vilp2ass;
    p2EntAsset* asset;
    F32 spd_move;
    F32 spd_chase;
    F32 spd_turn;
    F32 ds2_detect;
    F32 hyt_detect;
    F32 hyt_dtctoff;
    F32 fov_detect;
    F32 ds2_pursue;
    F32 ds2_fromhome;
    F32 spd_lob;
    F32 dst_lob;
    S32 cnt_lobSalvo;
    U32 prjTypeID;
    F32 dst_extend;
    F32 wid_extend;
    F32 spd_extend;
    F32 ang_wander;
    F32 fac_lobarc;
    F32 fac_lobheavy;
    F32 tymref_list[32];
    S32 msk_condspawn;
    S32 minDifficulty;
    S32 hitpts;
    S32 misc_flags;
    _xVec3 scl_modelBase;
    _xVec3 sanim_idle;
    _xVec3 sanim_move;
    _xVec3 sanim_chase;
    F32 rad_bound;
    _xVec3 vec_bndoffset;
    F32 rad_bound2;
    _xVec3 vec_bnd2offset;
    F32 tym_attack_begin;
    F32 tym_attack_end;
    _xVec3 pos_verts[12];
};

struct st_STIMULUS_CONTEXT_DATA
{
    en_NPC_STIMULUS stim;
    _zEnt* tgt_ent;
    union
    {
        en_BEHAVIOUR_GOAL gods_goal;
        _zEntBubble* bubble;
        _xVec3 dir_headbutt;
        _xCollis* colrec;
        _xVec3 pos_guard;
        _xVec3 gotopos;
        xBase* widge;
        S32 use_alt_action;
        S32 mm_teleport_hurtz_doggie;
        zSFX* talkSFX;
    };
    union
    {
        F32 off_goto;
        F32 mag_headbutt;
        F32 amt_stun;
    };
    union
    {
        S32 res_gods;
        en_ZVILLAIN_BUBBLE_RESPONSE res_bubble;
        en_ZVILLAIN_HEADBUTT_RESPONSE res_headbutt;
        en_ZVILLAIN_BOUNCE_RESPONSE res_buttbounce;
        en_ZVILLAIN_BUMPED_RESPONSE res_bumped;
    };
    S32 res_bogus;
    en_BEHAVIOUR_GOAL trangoal;
    en_NPC_MODE tranmode;
};

struct st_ZVIL_NPC_HANDLERFUNCS
{
    S32 flg_funcs;
    S32 (*flags)(st_zEntVillain*, en_ZVIL_FLAG_GROUPS, en_BEHAVIOUR_GOAL);
    S32 (*query)(st_zEntVillain*, en_ZVIL_QUERY);
    S32 (*sysevent)(st_zEntVillain*, xBase*, U32, F32*, xBase*, st_STIMULUS_CONTEXT_DATA*);
    S32 (*notice)(st_zEntVillain*, en_ZVIL_NOTICE, st_STIMULUS_CONTEXT_DATA*);
    S32 (*aware)(st_zEntVillain*, _xScene*, F32, st_STIMULUS_CONTEXT_DATA*);
    S32 (*stimulus)(st_zEntVillain*, st_STIMULUS_CONTEXT_DATA*, st_STIMULUS_CONTEXT_DATA*);
    S32 (*trangoal)(st_zEntVillain*, st_STIMULUS_CONTEXT_DATA*, F32);
    S32 (*calcmove)(st_zEntVillain*, _xScene*, F32, const _xEntFrame*);
    S32 (*respond)(st_zEntVillain*, st_STIMULUS_CONTEXT_DATA*);
    S32 (*pickanim)(st_zEntVillain*, en_BEHAVIOUR_GOAL, en_ZVIL_GOAL_SPOT);
    S32 (*animtran)(st_zEntVillain*, xAnimTransition*, xAnimSingle*);
    S32 (*xtrarend)(st_zEntVillain*);
};

struct st_zEntVillain : _zEnt
{
    st_ZVIL_NPC_HANDLERFUNCS* vhf;
    S32 ability;
    S32 abl_atak;
    S32 abl_dtct;
    S32 abl_move;
    S32 abl_spyk;
    S32 abl_vuln;
    en_NPC_PERSONALITIES typ_person;
    S32 flg_misc;
    en_NPC_MODE mode;
    en_BEHAVIOUR_GOAL goal;
    en_BEHAVIOUR_GOAL goaldflt;
    S32 standby;
    S32 anim_playing;
    S32 anim_playsync;
    en_NPC_ANIMSTATES anim_playnext;
    F32 anim_loopmult;
    _xVec3 sanim_idle;
    _xVec3 sanim_move;
    _xVec3 sanim_chase;
    F32 spd_move;
    F32 spd_chase;
    F32 spd_turn;
    F32 ds2_detect;
    F32 fov_detect;
    F32 hyt_detect;
    F32 hyt_dtctoff;
    F32 ds2_pursue;
    F32 spd_lob;
    F32 dst_lob;
    S32 cnt_lobSalvo;
    U32 prjTypeID;
    _xVec3 vec_lobSpin;
    F32 dst_extend;
    F32 wid_extend;
    F32 spd_extend;
    _xVec3 pos_exttgt;
    F32 ang_wander;
    F32 fac_lobarc;
    F32 fac_lobheavy;
    iModelTag tag_vert[12];
    S32 flg_vert;
    tagzFeet feet;
    zSFX* sfx_curTalk;
    _xBound bound2;
    S32 allow;
    _zEntBubble* bubble;
    F32 scl_nombub;
    xModelInstance* mdl_gumBlob;
    S32 hitpts;
    U32 flinch;
    S32 goodattack;
    _zMovePoint* nav_mvptlist;
    _zMovePoint* nav_next;
    _zMovePoint* nav_prev;
    S32 need_navpt;
    F32 dst_navTween;
    S32 domove;
    _xVec3 pos_moveto;
    _xVec3 dlt_moveto;
    _xVec3 dir_moveto;
    F32 ang_moveto;
    _xVec3 vel_moveto;
    F32 dlt_faceang;
    F32 ang_mdlface;
    F32 spd_moveto;
    F32 wiz_moveto;
    F32 ang_spiral;
    F32 rad_spiral;
    _xVec3 pos_vortex;
    _xVec3 vec_knocked;
    _xVec3 pos_bowling;
    _xVec3 dir_bowling;
    _xVec3 pos_pursue_home;
    _xVec3 pos_guard_home;
    _xVec3 pos_goto;
    F32 ds2_goto_keepaway;
    _xEnt* tgt_follow;
    _zMovePoint* tgt_bullseye;
    _zEnt* tgt_protect;
    S32 tgt_stat;
    F32 ang_listen;
    zEntProjectile* prj_reserved;
    F32 tmr_list[32];
    st_ZVIL_PRECALC precalc;
    st_NPC_SOUND_QUEUE snd_queue[4];
    st_ZVIL_CONFIG_REFERENCE vilcfg;
};

en_ZVILLAIN_BUBBLE_RESPONSE zEntVillain_Bubble(st_zEntVillain* ent, _zEntBubble* bub);
en_ZVILLAIN_BUMPED_RESPONSE zEntVillain_Bumped(st_zEntVillain* ent, const _xCollis* colrec);
void zEntVillain_HeroWasHurt();
void zEntVillain_Stun(st_zEntVillain* ent, F32 strength);
st_zEntVillain* zEntVillain_NPCInst(U32 npcidx);
en_ZVILLAIN_BOUNCE_RESPONSE zEntVillain_Bounce(st_zEntVillain* ent);
void zEntVillain_BonusRender();
void zEntVillain_Load(st_zEntVillain* ent, xSerial* xser);
void zEntVillain_Save(st_zEntVillain* ent, xSerial* xser);
st_zEntVillain* zEntVillain_MemPool(U32 npccnt);
S32 zEntVillain_sceneFinish();
S32 zEntVillain_scenePrepare();
S32 zEntVillain_Shutdown();

#endif
