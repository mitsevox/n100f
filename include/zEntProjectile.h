#ifndef ZENTPROJECTILE_H
#define ZENTPROJECTILE_H

#include "types.h"
#include "xEnt.h"
#include "xMath3.h"
#include "xLinkList.h"

struct zScene;
struct zEntProjectile;
struct _zGust;
struct _xCollis;
struct RpAtomic;
struct zProjectileTypeAsset;

enum en_ZPRJ_CATEGORY
{
    ZPRJ_CAT_UNKNOWN,
    ZPRJ_CAT_ARROW,
    ZPRJ_CAT_GRAVBALL,
    ZPRJ_CAT_BOUNCEBALL,
    ZPRJ_CAT_BUBSOAP,
    ZPRJ_CAT_BUBGUM,
    ZPRJ_CAT_BKAXE,
    ZPRJ_CAT_GGSHOCK,
    ZPRJ_CAT_RBSWORD,
    ZPRJ_CAT_MMFLASK,
    ZPRJ_CAT_PICKUP,
    ZPRJ_CAT_STDDEBRIS,
    ZPRJ_CAT_BANGDEBRIS,
    ZPRJ_CAT_NOMORE
};

enum en_PROJECTILE_EVENT
{
    ZPRJ_EVNT_ACTIVE,
    ZPRJ_EVNT_TRANSTATE,
    ZPRJ_EVNT_NEWTIME,
    ZPRJ_EVNT_ENDTIME,
    ZPRJ_EVNT_ABORT,
    ZPRJ_EVNT_HITPLYR,
    ZPRJ_EVNT_LAUNCH,
    ZPRJ_EVNT_INFLIGHT,
    ZPRJ_EVNT_ATREST,
    ZPRJ_EVNT_HITSTUFF,
    ZPRJ_EVNT_DETONATE,
    ZPRJ_EVNT_SELFDESTRUCT,
    ZPRJ_EVNT_DEATH,
    ZPRJ_EVNT_STARTRESTING,
    ZPRJ_EVNT_BOUNCE,
    ZPRJ_EVNT_VIL_BUILDUP,
    ZPRJ_EVNT_VIL_AFTERMATH,
    ZPRJ_EVNT_NOMORE
};

enum en_ZPRJ_STATES
{
    ZPRJ_STATE_UNUSED,
    ZPRJ_STATE_STANDBY,
    ZPRJ_STATE_ACTIVE,
    ZPRJ_STATE_DEAD
};

enum en_ZPRJ_CATAIM_FORM
{
    ZPRJ_CAIM_ARROW,
    ZPRJ_CAIM_GRAVBALL
};

struct st_ZPRJ_CAT_GRAVBALL
{
    F32 acc_gravity;
};

struct st_ZPRJ_CAT_BOUNCEBALL : st_ZPRJ_CAT_GRAVBALL
{
    S32 atRest;
    F32 tym_atrest;
    S32 cnt_curBounce;
    S32 cnt_maxBounce;
    F32 fac_elastic;
    F32 pct_randKick;
    S32 skip_next_bounce;
};

struct st_CAT_AIMARROW
{
    xVec3 pos_src;
    xVec3 pos_tgt;
};

struct st_CAT_AIMGRAV
{
    xVec3 pos_src;
    xVec3 pos_tgt;
    F32 fac_arcness;
    F32 acc_gravity;
};

struct ST_CAT_AIMDATA
{
    en_ZPRJ_CATAIM_FORM idx_caim;
    F32 spd_base;
    xVec3 dir_base;
    union
    {
        st_CAT_AIMARROW arrow;
        st_CAT_AIMGRAV grav;
    };
};

struct st_ZPRJ_CATEGORY_PROPS
{
    S32 flg_props;
    U8 flg_physics;
    U8 flg_chk;
    U8 flg_pen;
    U8 pad;
    S32 flg_categ;
};

struct st_ZPRJ_CATEGORY_FUNCS
{
    S32 flg_cback;
    S32 (*attach)(zEntProjectile*);
    void (*detach)(zEntProjectile*);
    S32 (*logix)(zEntProjectile*, F32, zScene*, _xEntFrame*);
    S32 (*calcmove)(zEntProjectile*, F32, zScene*, _xEntFrame*);
    S32 (*notify)(zEntProjectile*, en_PROJECTILE_EVENT, void*);
    S32 (*render)(zEntProjectile*);
    S32 (*aimer)(zEntProjectile*, ST_CAT_AIMDATA*, S32);
    S32 (*collide)(zEntProjectile*, zScene*, F32);
    void (*dbgUpdate)(zEntProjectile*, S32);
};

enum en_ZPRJ_CATDATA_FORM
{
    ZPRJ_CDAT_ARROW,
    ZPRJ_CDAT_GRAVBALL,
    ZPRJ_CDAT_BOUNCE
};

struct st_ZPRJ_CAT_ARROW
{
    F32 acc_gravity;
    S32 amStuck;
    _xEnt* ent_stuck;
    xVec3 pos_stuck;
};

struct st_ZPRJ_CAT_PICKUP
{
    S32 nada;
};

struct st_ZPRJ_CATDATA
{
    en_ZPRJ_CATDATA_FORM idx_cdat;
    union
    {
        st_ZPRJ_CAT_ARROW arrow;
        st_ZPRJ_CAT_GRAVBALL gravball;
        st_ZPRJ_CAT_BOUNCEBALL bounce;
        st_ZPRJ_CAT_PICKUP pickup;
    };
};

struct st_ZPRJ_COMMON
{
    xVec3 vec_flex;
    xMat3x3 mat_flex;
};

struct st_ZPRJ_LINKNODE
{
    st_ENODE node;
    zEntProjectile* backref;
};

struct st_ZPRJ_TYPERECORD
{
    U32 aid_ptype;
    zProjectileTypeAsset* src_ptype;
    en_ZPRJ_CATEGORY categ;
    RpAtomic* mdlFile[2];
};

struct zProjectileTypeAsset : p2BaseAsset
{
    S32 efxType;
    U32 modelID;
    U32 animID;
    U32 atRestModelID;
    U32 atRestAnimID;
    S32 destructEnabled;
    F32 destructTime;
    F32 destructDist;
    S32 oriented;
    S32 extra_space[6];
};

struct st_PROJECTILE_DEPOT
{
    S32 typecnt;
    st_ZPRJ_TYPERECORD* typetable;
    S32 typeidx;
    S32 rawcnt;
    zEntProjectile* rawpool;
    st_ELIST prjpool;
    st_ELIST prjinuse;
};

enum en_ZPRJ_FX_TYPE
{
    ZPRJ_FXTYPE_NONE,
    ZPRJ_FXTYPE_SMOKE,
    ZPRJ_FXTYPE_WOOD,
    ZPRJ_FXTYPE_TAR,
    ZPRJ_FXTYPE_FIREBALL,
    ZPRJ_FXTYPE_NODAMAGE_FIREBALL,
    ZPRJ_FXTYPE_ICE,
    ZPRJ_FXTYPE_BLACKKNIGHT,
    ZPRJ_FXTYPE_WITCHDOCTOR,
    ZPRJ_FXTYPE_REDBEARD,
    ZPRJ_FXTYPE_MASTERMIND_RED,
    ZPRJ_FXTYPE_MASTERMIND_GREEN,
    ZPRJ_FXTYPE_MASTERMIND_BLACK,
    ZPRJ_FXTYPE_UNUSED3,
    ZPRJ_FXTYPE_UNUSED4,
    ZPRJ_FXTYPE_UNUSED5,
    ZPRJ_FXTYPE_TOTAL
};

enum en_ZPRJ_MODEL_INDEX
{
    ZPRJ_MDLIDX_STD,
    ZPRJ_MDLIDX_POSTHIT,
    ZPRJ_MDLIDX_NOMORE
};

enum en_ZPRJ_DOCOL_HEURISTICS
{
    ZPRJ_DOCOL_CUSTOWNER,
    ZPRJ_DOCOL_CUSTCAT,
    ZPRJ_DOCOL_ALWAYS,
    ZPRJ_DOCOL_RANDPCT,
    ZPRJ_DOCOL_SINE,
    ZPRJ_DOCOL_TOGGLE,
    ZPRJ_DOCOL_VELBOUND,
    ZPRJ_DOCOL_NOMORE
};

enum en_ZPRJ_SOLVER_METHODS
{
    ZPRJ_ATGTSOLV_METHA,
    ZPRJ_ATGTSOLV_METHB,
    ZPRJ_ATGTSOLV_METHC
};

struct st_ZPRJ_SOLV_METHOD_A
{
    xVec3 vel_launch;
    xVec3 dir_launch;
    F32 spd_muzzle;
    xVec3 pos_launch;
    xVec3 pos_target;
    F32 acc_gravity;
    F32 fac_arcness;
    F32 dst_range;
};

struct st_NEWT_USERDATA
{
    F32 g;
    F32 g_2;
    F32 y_y0;
    F32 v0;
    S32 cnt_iter;
};

struct st_ZPRJ_SOLV_METHOD_B
{
    S32 flg_stat;
    xVec3 vel_launch;
    xVec3 acc_gravity;
    xVec3 pos_base;
    F32 spd_base;
    xVec3 dir_base;
    xVec3 pos_tgt;
    xVec3 vel_tgt;
    F32 dst_tgtXZ;
    F32 spd_max;
    F32 spd_min;
    S32 cnt_subspd;
    F32 ang_thmax;
    F32 ang_thmin;
    S32 cnt_subang;
    F32 acc_maxgrav;
    F32 acc_mingrav;
    S32 cnt_subgrav;
    S32 is_winner;
    S32 cnt_pause;
    F32 last_speed;
    F32 last_angle;
    F32 last_grav;
    st_NEWT_USERDATA newtdata;
};

struct st_ZPRJ_SOLV_METHOD_C
{
    xVec3 vel_launch;
    F32 spd_muzzle;
    F32 acc_gravity;
    xVec3 pos_launch;
    xVec3 pos_target;
};

struct st_ZPRJ_AUTOCALC_INFO
{
    en_ZPRJ_SOLVER_METHODS method;
    union
    {
        st_ZPRJ_SOLV_METHOD_A metha;
        st_ZPRJ_SOLV_METHOD_B methb;
        st_ZPRJ_SOLV_METHOD_C methc;
    };
};

struct zGustData
{
    U32 gust_on;
    _zGust* g[4];
    F32 lerp[4];
};

struct zEntProjectile : _xEnt
{
    S32 flg_prop;
    S32 flg_categ;
    U8 flg_chk;
    U8 flg_pen;
    U8 pad[2];
    void* owner;
    void* ownerContext;
    S32 (*ownerNotify)(zEntProjectile*, en_PROJECTILE_EVENT, void*);
    S32 (*ownerDoCollide)(zEntProjectile*, zScene*, F32);
    F32 tmr_alive;
    F32 tmr_curstate;
    xVec3 pos_home;
    F32 tym_maxlife;
    F32 ds2_maxlife;
    st_ZPRJ_COMMON common;
    st_ZPRJ_CATDATA cat_data;
    void* xtradata;
    _xCollis* plyr_colrec;
    st_ZPRJ_LINKNODE linknode;
    en_ZPRJ_STATES state;
    S32 flg_stat;
    F32 pct_docol;
    F32 tmr_coldelay;
    en_ZPRJ_CATEGORY cat_proj;
    st_ZPRJ_TYPERECORD* ptype;
    F32 tmr_emit;
    _xEntFrame raw_frame;
    p2EntAsset raw_asset;
    _xEntShadow raw_shadow;
    zGustData raw_gust;

    void PEFX_flight(en_ZPRJ_FX_TYPE fxtyp, F32 dt);
    void DoEffects(en_PROJECTILE_EVENT event, F32 dt);
    S32 LookForFloorRoof(_xScene*, F32);
    S32 ZPRJ_render();
    S32 DoColCheck(F32 dt);
    S32 ZPRJ_update(zScene* scn, F32 dt);
    void OrientToPath(_xEntFrame* frame, S32 flg_orient);
    void OrientSet(S32 orient);
    S32 SetupFrame(xVec3* pos, xVec3* veloc);
    S32 Setup_xEnt();
    S32 SetupPrelim(en_ZPRJ_CATEGORY cat, U32 ptaid);
    S32 Init(en_ZPRJ_CATEGORY cat, U32 ptypeID);
    void Recycle();
    S32 AimAtTarget(ST_CAT_AIMDATA* info, S32 apply);
    S32 AssignModel(en_ZPRJ_MODEL_INDEX midx);
    void FlexMatSet(xVec3* rot_euler, F32 dt);
    void FlexVecSet(xVec3* vec_flex);
    S32 SendEvent(en_PROJECTILE_EVENT event, F32 dt, void* edata, S32* res_user);
    S32 SendEvent(en_PROJECTILE_EVENT event, F32 dt);
    S32 SendEvent(en_PROJECTILE_EVENT event);
    void SyncFrame(S32 applyToModel);
    void Abort();
    void ColDelaySet(F32 tym, S32 addto);
    en_ZPRJ_STATES StateSet(en_ZPRJ_STATES state);
};

S32 zEntProjectile_gen_aimer(zEntProjectile* prj, ST_CAT_AIMDATA* info, S32 apply);
void zEntProjectile_softReflect(xVec3* dir_new, xVec3* dir_cur, xVec3* oppose, F32 scaleMag);
F32 zEntProjectile_colAccum(zEntProjectile* prj, xVec3* dir_away);
S32 zEntProjectile_gen_render(zEntProjectile* prj);
S32 zEntProjectile_gen_logix(zEntProjectile* prj, F32 dt, zScene*, _xEntFrame* frame);
S32 zEntProjectile_gen_attach(zEntProjectile* prj);
zEntProjectile* zEntProjectile_acquire(en_ZPRJ_CATEGORY cat, U32 ptypeID);
en_ZPRJ_CATEGORY zEntProjectile_catFromPrjType(U32 ptypeID, en_ZPRJ_CATEGORY prefer);
void zEntProjectile_poolRender(en_ZPRJ_CATEGORY cat);
S32 zEntProjectile_poolUpdate(en_ZPRJ_CATEGORY, zScene* scn, F32 dt);
void zEntProjectile_prepSystem(S32 num_ptype, zScene*);
void zEntProjectile_sceneFinish();
void zEntProjectile_sceneReset();
void zEntProjectile_scenePrepare();
void zEntProjectile_registerType(zProjectileTypeAsset* ptype);
S32 zEntProjectile_addProps(en_ZPRJ_CATEGORY cat, st_ZPRJ_CATEGORY_PROPS* props);
S32 zEntProjectile_addCat(en_ZPRJ_CATEGORY cat, st_ZPRJ_CATEGORY_FUNCS* funcs);
void zEntProjectile_Shutdown();
void zEntProjectile_Startup();

#endif
