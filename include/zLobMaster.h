#ifndef ZLOBMASTER_H
#define ZLOBMASTER_H

#include "types.h"
#include "xBase.h"
#include "xEnt.h"
#include "xMath3.h"

struct zScene;
struct _zMovePoint;

enum en_LOB_MASTER_TYPE
{
    LOBMASTER_CUSTOM,
    LOBMASTER_ARROW,
    LOBMASTER_DEBRIS,
    LOBMASTER_PICKUP,
    LOBMASTER_NOMORE
};

enum en_LOBMASTER_PARAMID
{
    ZLOB_PARM_RAWASS,
    ZLOB_PARM_LOBFLAGS,
    ZLOB_PARM_LOBMASTERTYPE,
    ZLOB_PARM_PRJTYPEID,
    ZLOB_PARM_POS_LAUNCH,
    ZLOB_PARM_DIR_LAUNCH,
    ZLOB_PARM_SPD_LAUNCH,
    ZLOB_PARM_SPD_RANDPCT,
    ZLOB_PARM_SCL_MODEL,
    ZLOB_PARM_TYM_MAXLIFE,
    ZLOB_PARM_DST_MAXDIST,
    ZLOB_PARM_DEBRISANGLE,
    ZLOB_PARM_SALVO,
    ZLOB_PARM_AMMO,
    ZLOB_PARM_ARCFACTOR,
    ZLOB_PARM_HEAVYFACTOR,
    ZLOB_PARM_PAYLOAD,
    ZLOB_PARM_MVPTBULL,
    ZLOB_PARM_NUMBOUNCE,
    ZLOB_PARM_TUMBLE,
    ZLOB_PARM_COLLIDEDELAY,
    ZLOB_PARM_RESTPERIOD,
    ZLOB_PARM_NOMORE
};

struct zLobMasterAsset : p2BaseAsset
{
    S32 lobMasterType;
    U32 projectileTypeID;
    xVec3 pos_launch;
    xVec3 rot_launch;
    F32 spd_launch;
    F32 spd_randPct;
    xVec3 scl_model;
    S32 flg_enablers;
    F32 tym_maxlife;
    F32 dst_maxdist;
    U32 aid_mvptID;
    S32 cnt_salvo;
    S32 cnt_ammo;
    F32 fac_arccoeff;
    S32 ang_debrisCone;
    S32 numBounce;
    S32 typ_powerup;
    F32 heavyFactor;
    xVec3 tumbleRotation;
    F32 collideDelay;
    F32 atRestPeriod;
    U32 mode;
};

struct st_LOBMASTER_CONFIG
{
    zLobMasterAsset* rawass;
    xVec3 dir_launch;
};

struct st_LOBMASTER_DATA : xBase
{
    S32 flg_lmopts;
    F32 ang_debrisCone;
    S32 cnt_ammo;
    S32 cnt_salvo;
    _zMovePoint* nav_bullbase;
    _zMovePoint* nav_nextbull;
    _zMovePoint* nav_prevbull;
    S32 flg_suitcase;
    xVec3 pos_altLaunch;
    xVec3 dir_altLaunch;
    _xBound bnd_altLaunch;
    st_LOBMASTER_CONFIG cfg;
};

struct st_LOBMASTER_DEPOT
{
    st_LOBMASTER_DATA* raw_pool;
    S32 raw_cnt;
};

struct st_LOBMASTER_QUEUE
{
    S32 flg_stat;
    F32 tmr_delay;
    U32 aid_lobber;
    xVec3 pos_launch;
    xVec3 dir_launch;
    S32 cnt_salvo;
    S32 idx_current;
};

void zLobMaster_Startup();
void zLobMaster_Shutdown();
void zLobMaster_scenePrepare();
void zLobMaster_sceneFinish();
st_LOBMASTER_DATA* zLobMaster_memPool(S32 cnt);
st_LOBMASTER_DATA* zLobMaster_getInst(st_LOBMASTER_DATA* pool, S32 idx);
void zLobMaster_Init(st_LOBMASTER_DATA* lob, p2BaseAsset* basset);
void zLobMaster_InitDep(st_LOBMASTER_DATA* lob, zScene* sc);
void zLobMaster_timestep(zScene* sc, F32 dt);

#endif
