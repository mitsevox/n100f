#include "zLobMaster.h"
#include "zEntProjectile.h"
#include "zMovePoint.h"
#include "zScene.h"
#include "zGlobals.h"
#include "xScene.h"
#include "xMath.h"
#include "xMath3.h"
#include "xMemMgr.h"
#include "xutil.h"
#include "xBound.h"
#include "iMath.h"

#include <string.h>

static S32 LPRJ_debris_prjmove(zEntProjectile* prj, F32 dt, zScene* sc, _xEntFrame* frame);
static S32 LPRJ_debris_logix(zEntProjectile* prj, F32 dt, zScene* sc, _xEntFrame* frame);
static S32 LPRJ_debris_attach(zEntProjectile* prj);
static S32 ZLOB_elcb_event(xBase* from, xBase* xb, U32 toEvent, F32* toParam, xBase* toParamWidget);
static S32 LCFG_getParam(st_LOBMASTER_CONFIG* cfg, en_LOBMASTER_PARAMID pid, void* vv);
static void ZLOB_HEB_to_DIR(xVec3* dir_result, xVec3* rot_HPB);
static st_LOBMASTER_DATA* ZLOB_find_lobber(U32 aid_lobm);
static void ZLOB_grab_nextBull(st_LOBMASTER_DATA* lob);
static S32 ZLOB_doLaunch(st_LOBMASTER_DATA* lob);
static S32 ZLOB_shoot_one(st_LOBMASTER_DATA* lob, S32 nth, S32 total);
static S32 ZLOB_legacy_notify(zEntProjectile* prj, en_PROJECTILE_EVENT event, void* data);
static S32 ZLOB_shoot_legacy(st_LOBMASTER_DATA* lob, S32 nth, S32 total);
static S32 ZLOB_shoot_arrow(st_LOBMASTER_DATA* lob, S32 nth, S32 total);
static S32 ZLOB_shoot_debris(st_LOBMASTER_DATA* lob, S32 nth, S32 total);
static S32 ZLOB_randpos_fromBound(_xBound* bnd, S32, S32, S32 flg_ropts, xVec3* pos_launch);

st_LOBMASTER_QUEUE g_zlob_queue[5] = {};
static st_LOBMASTER_DEPOT g_zlob_depot = {};
static S32 g_zlob_init;

void zLobMaster_Startup()
{
    st_ZPRJ_CATEGORY_FUNCS fmap_debris = {
        1,    LPRJ_debris_attach, NULL, LPRJ_debris_logix, LPRJ_debris_prjmove, NULL, zEntProjectile_gen_render,
        NULL, NULL,               NULL
    };
    st_ZPRJ_CATEGORY_PROPS prop_debris = { 1, 0x03, 0x20, 0x20, 0, 0 };
    st_ZPRJ_CATEGORY_PROPS prop_bang = { 1, 0x03, 0x3E, 0x7E, 0, 0 };

    zEntProjectile_addCat(ZPRJ_CAT_STDDEBRIS, &fmap_debris);
    zEntProjectile_addProps(ZPRJ_CAT_STDDEBRIS, &prop_debris);

    zEntProjectile_addCat(ZPRJ_CAT_BANGDEBRIS, &fmap_debris);
    zEntProjectile_addProps(ZPRJ_CAT_BANGDEBRIS, &prop_bang);
}

void zLobMaster_Shutdown()
{
}

void zLobMaster_scenePrepare()
{
    st_LOBMASTER_DEPOT* depot = &g_zlob_depot;

    memset(g_zlob_queue, 0, sizeof(g_zlob_queue));

    if (g_zlob_init++ == 0)
    {
        memset(depot, 0, sizeof(st_LOBMASTER_DEPOT));
    }
}

void zLobMaster_sceneFinish()
{
    st_LOBMASTER_DEPOT* depot = &g_zlob_depot;

    if (--g_zlob_init == 0)
    {
        memset(depot, 0, sizeof(st_LOBMASTER_DEPOT));
    }
}

st_LOBMASTER_DATA* zLobMaster_memPool(S32 cnt)
{
    st_LOBMASTER_DATA* pool;
    st_LOBMASTER_DEPOT* depot = &g_zlob_depot;

    if (cnt < 1)
    {
        return NULL;
    }

    pool = (st_LOBMASTER_DATA*)xMemAlloc(gActiveHeap, cnt * sizeof(st_LOBMASTER_DATA));
    memset(pool, 0, cnt * sizeof(st_LOBMASTER_DATA));

    depot->raw_pool = pool;
    depot->raw_cnt = cnt;

    return pool;
}

st_LOBMASTER_DATA* zLobMaster_getInst(st_LOBMASTER_DATA* pool, S32 idx)
{
    return &pool[idx];
}

static inline void LCFG_setup(st_LOBMASTER_CONFIG* cfg, zLobMasterAsset* rawass)
{
    xVec3 rot_launch = { 0.0f, 0.0f, 0.0f };
    cfg->rawass = rawass;
    xVec3SMul(&rot_launch, &rawass->rot_launch, PI / 180.0f);
    ZLOB_HEB_to_DIR(&cfg->dir_launch, &rot_launch);
}

void zLobMaster_Init(st_LOBMASTER_DATA* lob, p2BaseAsset* basset)
{
    zLobMasterAsset* tmp_rawass = (zLobMasterAsset*)basset;

    xBaseInit(lob, basset);

    lob->eventFunc = ZLOB_elcb_event;

    if (lob->linkCount)
    {
        lob->link = (p2LinkAsset*)(tmp_rawass + 1);
    }
    else
    {
        lob->link = NULL;
    }

    LCFG_setup(&lob->cfg, tmp_rawass);

    xSceneID2Name(globals.sceneCur, lob->id);
}

void zLobMaster_InitDep(st_LOBMASTER_DATA* lob, zScene*)
{
    LCFG_getParam(&lob->cfg, ZLOB_PARM_LOBFLAGS, &lob->flg_lmopts);
    LCFG_getParam(&lob->cfg, ZLOB_PARM_DEBRISANGLE, &lob->ang_debrisCone);
    LCFG_getParam(&lob->cfg, ZLOB_PARM_MVPTBULL, &lob->nav_bullbase);
    LCFG_getParam(&lob->cfg, ZLOB_PARM_AMMO, &lob->cnt_ammo);

    if (lob->nav_bullbase)
    {
        ZLOB_grab_nextBull(lob);
    }

    xSceneID2Name(globals.sceneCur, lob->id);
}

void zLobMaster_timestep(zScene*, F32 dt)
{
    S32 i;
    st_LOBMASTER_DATA* lobber;
    st_LOBMASTER_QUEUE* que;

    for (i = 0; i < 5; i++)
    {
        que = &g_zlob_queue[i];
        if (!(que->flg_stat & 1))
        {
            continue;
        }

        que->tmr_delay = MAX(-1.0f, que->tmr_delay - dt);
        if (que->tmr_delay > 0.0f)
        {
            continue;
        }

        lobber = ZLOB_find_lobber(que->aid_lobber);
        if (!lobber)
        {
            // lobber is NULL here, so this memset writes to address 0.
            memset(lobber, 0, sizeof(st_LOBMASTER_QUEUE));
            continue;
        }

        lobber->pos_altLaunch = que->pos_launch;
        lobber->flg_suitcase |= 1;

        lobber->dir_altLaunch = que->dir_launch;
        lobber->flg_suitcase |= 2;

        que->idx_current = que->cnt_salvo;

        if (que->idx_current >= que->cnt_salvo)
        {
            memset(que, 0, sizeof(st_LOBMASTER_QUEUE));
        }
    }
}

static void ZLOB_HEB_to_DIR(xVec3* dir_result, xVec3* rot_HPB)
{
    xVec3 vec_aim = { 0.0f, 0.0f, 1.0f };
    xMat3x3 mat_rot = {};

    if (rot_HPB->z)
    {
        xMat3x3Euler(&mat_rot, 0.0f, 0.0f, rot_HPB->z);
        xMat3x3LMulVec(&vec_aim, &mat_rot, &vec_aim);
    }

    xMat3x3Euler(&mat_rot, 0.0f, rot_HPB->y, 0.0f);
    xMat3x3LMulVec(&vec_aim, &mat_rot, &vec_aim);

    xMat3x3Euler(&mat_rot, rot_HPB->x, 0.0f, 0.0f);
    xMat3x3LMulVec(&vec_aim, &mat_rot, &vec_aim);

    xVec3Normalize(dir_result, &vec_aim);
}

static S32 LCFG_getParam(st_LOBMASTER_CONFIG* cfg, en_LOBMASTER_PARAMID pid, void* vv)
{
    S32 iv;
    S32 found = 1;
    zLobMasterAsset* lmass = cfg->rawass;

    switch (pid)
    {
    case ZLOB_PARM_RAWASS:
        *(zLobMasterAsset**)vv = lmass;
        break;
    case ZLOB_PARM_LOBFLAGS:
        iv = 0;
        if (lmass->flg_enablers & 0x1)
        {
            iv |= 0x1;
        }
        if (lmass->flg_enablers & 0x2)
        {
            iv |= 0x4;
        }
        if (lmass->flg_enablers & 0x4)
        {
            iv |= 0x8;
        }
        if (lmass->flg_enablers & 0x8)
        {
            iv |= 0x10;
        }
        if (lmass->flg_enablers & 0x10)
        {
            iv |= 0x20;
        }
        if (lmass->flg_enablers & 0x20)
        {
            iv |= 0x40;
        }
        if (lmass->flg_enablers & 0x40)
        {
            iv |= 0x80;
        }
        if (lmass->cnt_ammo > 0)
        {
            iv |= 0x2;
        }
        *(S32*)vv = iv;
        break;
    case ZLOB_PARM_LOBMASTERTYPE:
        switch (lmass->typ_powerup)
        {
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
            *(S32*)vv = LOBMASTER_PICKUP;
            break;
        default:
            *(S32*)vv = lmass->mode;
            break;
        }
        break;
    case ZLOB_PARM_PRJTYPEID:
        *(U32*)vv = lmass->projectileTypeID;
        break;
    case ZLOB_PARM_POS_LAUNCH:
        *(xVec3*)vv = lmass->pos_launch;
        break;
    case ZLOB_PARM_DIR_LAUNCH:
        *(xVec3*)vv = cfg->dir_launch;
        break;
    case ZLOB_PARM_SPD_LAUNCH:
        *(F32*)vv = lmass->spd_launch;
        break;
    case ZLOB_PARM_SPD_RANDPCT:
        *(F32*)vv = 0.01f * lmass->spd_randPct;
        break;
    case ZLOB_PARM_SCL_MODEL:
        *(xVec3*)vv = lmass->scl_model;
        break;
    case ZLOB_PARM_TYM_MAXLIFE:
        if (lmass->flg_enablers & 0x1)
        {
            *(F32*)vv = lmass->tym_maxlife;
        }
        else
        {
            *(F32*)vv = 10.0f;
        }
        break;
    case ZLOB_PARM_DST_MAXDIST:
        if (lmass->flg_enablers & 0x1)
        {
            *(F32*)vv = lmass->dst_maxdist;
        }
        else
        {
            *(F32*)vv = 15.0f;
        }
        break;
    case ZLOB_PARM_DEBRISANGLE:
        *(F32*)vv = PI * lmass->ang_debrisCone / 180.0f;
        break;
    case ZLOB_PARM_NUMBOUNCE:
        *(S32*)vv = lmass->numBounce;
        break;
    case ZLOB_PARM_SALVO:
        *(S32*)vv = lmass->cnt_salvo;
        break;
    case ZLOB_PARM_AMMO:
        *(S32*)vv = lmass->cnt_ammo;
        break;
    case ZLOB_PARM_ARCFACTOR:
        *(F32*)vv = lmass->fac_arccoeff;
        break;
    case ZLOB_PARM_HEAVYFACTOR:
        *(F32*)vv = lmass->heavyFactor;
        break;
    case ZLOB_PARM_PAYLOAD:
        *(S32*)vv = lmass->typ_powerup;
        break;
    case ZLOB_PARM_MVPTBULL:
        *(_zMovePoint**)vv = zMovePoint_From_xAssetID(lmass->aid_mvptID);
        break;
    case ZLOB_PARM_TUMBLE:
        ((xVec3*)vv)->x = PI * lmass->tumbleRotation.x / 180.0f;
        ((xVec3*)vv)->y = PI * lmass->tumbleRotation.y / 180.0f;
        ((xVec3*)vv)->z = PI * lmass->tumbleRotation.z / 180.0f;
        break;
    case ZLOB_PARM_COLLIDEDELAY:
        *(F32*)vv = lmass->collideDelay;
        break;
    case ZLOB_PARM_RESTPERIOD:
        *(F32*)vv = lmass->atRestPeriod;
        break;
    default:
        found = 0;
        break;
    }

    return found;
}

static inline void ZLOB_reset(st_LOBMASTER_DATA* lob)
{
    zLobMasterAsset* rawass = NULL;
    LCFG_getParam(&lob->cfg, ZLOB_PARM_RAWASS, &rawass);
    xBaseReset(lob, rawass);
    memset(&lob->flg_lmopts, 0, sizeof(st_LOBMASTER_DATA) - sizeof(xBase));
    zLobMaster_Init(lob, rawass);
    zLobMaster_InitDep(lob, NULL);
}

static S32 ZLOB_elcb_event(xBase* from, xBase* xb, U32 toEvent, F32*, xBase*)
{
    st_LOBMASTER_DATA* lob = (st_LOBMASTER_DATA*)xb;
    S32 do_fromWidge = 0;

    switch (toEvent)
    {
    case 10:
        ZLOB_reset(lob);
        break;
    case 92:
        LCFG_getParam(&lob->cfg, ZLOB_PARM_AMMO, &lob->cnt_ammo);
        break;
    case 91:
        ZLOB_doLaunch(lob);
        break;
    case 167:
        switch (from->baseType)
        {
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
            lob->pos_altLaunch = *xEntGetPos((_xEnt*)from);
            lob->flg_suitcase = 0;
            lob->flg_suitcase |= 1;
            memcpy(&lob->bnd_altLaunch, &((_xEnt*)from)->bound, sizeof(_xBound));
            lob->flg_suitcase |= 4;
            do_fromWidge = 1;
            break;
        case 13:
            lob->pos_altLaunch = *zMovePointGetPos((_zMovePoint*)from);
            lob->flg_suitcase = 0;
            lob->flg_suitcase |= 1;
            do_fromWidge = 1;
            break;
        case 36:
            LCFG_getParam(&((st_LOBMASTER_DATA*)from)->cfg, ZLOB_PARM_POS_LAUNCH, &lob->pos_altLaunch);
            lob->flg_suitcase = 0;
            lob->flg_suitcase |= 1;
            do_fromWidge = 1;
            break;
        case 1:
        case 5:
        case 7:
        case 9:
        case 10:
        case 14:
        case 15:
        case 16:
        case 17:
        case 19:
        case 20:
        case 21:
        case 22:
        case 25:
        case 26:
        case 28:
        case 29:
        case 30:
        case 31:
        case 32:
        case 33:
        case 34:
        case 35:
        case 37:
            xSceneID2Name(globals.sceneCur, from->id);
            break;
        default:
            xSceneID2Name(globals.sceneCur, from->id);
            break;
        }

        if (do_fromWidge)
        {
            ZLOB_doLaunch(lob);
        }

        lob->flg_suitcase = 0;
        break;
    case 87:
    case 88:
    case 89:
    case 90:
        break;
    default:
        xSceneID2Name(globals.sceneCur, xb->id);
        break;
    }

    return 1;
}

static S32 ZLOB_doLaunch(st_LOBMASTER_DATA* lob)
{
    S32 salvo = 0;
    en_LOB_MASTER_TYPE type = LOBMASTER_CUSTOM;
    S32 cnt = 0;
    S32 rc;
    S32 i;

    LCFG_getParam(&lob->cfg, ZLOB_PARM_LOBMASTERTYPE, &type);
    LCFG_getParam(&lob->cfg, ZLOB_PARM_SALVO, &salvo);

    if (type == LOBMASTER_DEBRIS)
    {
        salvo = MIN(salvo, 10);
    }

    for (i = 0; i < salvo; i++)
    {
        if (lob->flg_lmopts & 0x2)
        {
            if (lob->cnt_ammo-- < 0)
            {
                break;
            }
        }

        rc = ZLOB_shoot_one(lob, i, salvo);
        if (!rc)
        {
            break;
        }

        cnt++;
    }

    ZLOB_grab_nextBull(lob);

    return cnt;
}

static S32 ZLOB_shoot_one(st_LOBMASTER_DATA* lob, S32 nth, S32 total)
{
    en_LOB_MASTER_TYPE type = LOBMASTER_CUSTOM;

    LCFG_getParam(&lob->cfg, ZLOB_PARM_LOBMASTERTYPE, &type);

    switch (type)
    {
    case LOBMASTER_CUSTOM:
        ZLOB_shoot_legacy(lob, nth, total);
        break;
    case LOBMASTER_ARROW:
        ZLOB_shoot_arrow(lob, nth, total);
        break;
    case LOBMASTER_DEBRIS:
        ZLOB_shoot_debris(lob, nth, total);
        break;
    case LOBMASTER_PICKUP:
        break;
    default:
        ZLOB_shoot_legacy(lob, nth, total);
        break;
    }

    return 1;
}

static S32 ZLOB_legacy_notify(zEntProjectile* prj, en_PROJECTILE_EVENT event, void*)
{
    st_LOBMASTER_DATA* lob = (st_LOBMASTER_DATA*)prj->owner;
    F32 fval = 0.0f;

    switch (event)
    {
    case ZPRJ_EVNT_NEWTIME:
        if (lob->flg_lmopts & 0x20)
        {
            LCFG_getParam(&lob->cfg, ZLOB_PARM_COLLIDEDELAY, &fval);
            if (prj->tmr_alive > fval)
            {
                prj->flg_chk |= 0x10;
                prj->flg_pen |= 0x10;
            }
        }
        break;
    }

    return 1;
}

static inline void ZLOB_prj_setup(st_LOBMASTER_DATA* lob, zEntProjectile* prj)
{
    S32 payload;

    prj->owner = lob;
    LCFG_getParam(&lob->cfg, ZLOB_PARM_TYM_MAXLIFE, &prj->tym_maxlife);
    LCFG_getParam(&lob->cfg, ZLOB_PARM_DST_MAXDIST, &prj->ds2_maxlife);
    prj->ds2_maxlife = prj->ds2_maxlife * prj->ds2_maxlife;

    if (lob->flg_lmopts & 0x10)
    {
        prj->flg_prop |= 0x2;
    }

    payload = 0;
    LCFG_getParam(&lob->cfg, ZLOB_PARM_PAYLOAD, &payload);
    if (payload == 1)
    {
        prj->flg_prop |= 0x10;
    }
    else
    {
        prj->flg_prop &= ~0x10;
    }
}

static S32 ZLOB_shoot_legacy(st_LOBMASTER_DATA* lob, S32, S32)
{
    S32 result;
    zEntProjectile* prj;
    U32 prjTypeID = 0;
    F32 fval = 0.0f;
    st_ZPRJ_CAT_BOUNCEBALL* bounce;

    LCFG_getParam(&lob->cfg, ZLOB_PARM_PRJTYPEID, &prjTypeID);
    prj = zEntProjectile_acquire(ZPRJ_CAT_BOUNCEBALL, prjTypeID);
    if (!prj)
    {
        return 0;
    }

    bounce = &prj->cat_data.bounce;
    ZLOB_prj_setup(lob, prj);
    prj->ownerNotify = ZLOB_legacy_notify;

    LCFG_getParam(&lob->cfg, ZLOB_PARM_HEAVYFACTOR, &fval);
    if (fval > 0.0f)
    {
        bounce->acc_gravity = fval;
    }
    else
    {
        bounce->acc_gravity = 1.5f;
    }

    if (lob->flg_lmopts & 8)
    {
        prj->flg_categ |= 0x2000000;
        LCFG_getParam(&lob->cfg, ZLOB_PARM_NUMBOUNCE, &bounce->cnt_maxBounce);
        bounce->cnt_curBounce = 0;
        bounce->fac_elastic = 0.8f;
        bounce->pct_randKick = 0.0f;
    }
    else
    {
        prj->flg_categ &= ~0x2000000;
    }

    if (lob->flg_lmopts & 0x40)
    {
        prj->flg_categ |= 0x1000000;
        LCFG_getParam(&lob->cfg, ZLOB_PARM_RESTPERIOD, &bounce->tym_atrest);
    }
    else
    {
        prj->flg_categ &= ~0x1000000;
    }

    xVec3 rot_tumble = { 0.0f, 0.0f, 0.0f };
    LCFG_getParam(&lob->cfg, ZLOB_PARM_TUMBLE, &rot_tumble);
    if (xVec3Dot(&rot_tumble, &rot_tumble))
    {
        prj->flg_categ |= 0x4000000;
        prj->OrientSet(0);
        prj->FlexVecSet(&rot_tumble);
        prj->FlexMatSet(&rot_tumble, globals.update_dt);
    }
    else
    {
        prj->OrientSet(0x30000000);
        prj->flg_categ &= ~0x4000000;
    }

    if (lob->flg_lmopts & 0x20)
    {
        LCFG_getParam(&lob->cfg, ZLOB_PARM_COLLIDEDELAY, &fval);
        if (fval > 0.0f)
        {
            prj->ColDelaySet(fval, 0);
        }
        else
        {
            prj->ColDelaySet(-1.0f, 0);
        }
        prj->flg_chk = 0x2E;
        prj->flg_pen = 0x6E;
    }
    else
    {
        prj->ColDelaySet(-1.0f, 0);
        prj->flg_chk = 0x20;
        prj->flg_pen = 0x60;
    }

    F32 spd_launch = 0.0f;
    F32 spd_randPct = 0.0f;
    F32 tmp_rand;
    xVec3 pos_launch = { 0.0f, 0.0f, 0.0f };
    xVec3 dir_launch = { 0.0f, 0.0f, 0.0f };
    xVec3 pos_tgt = { 0.0f, 0.0f, 0.0f };

    if (lob->flg_suitcase & 1)
    {
        xVec3Copy(&pos_launch, &lob->pos_altLaunch);
    }
    else
    {
        LCFG_getParam(&lob->cfg, ZLOB_PARM_POS_LAUNCH, &pos_launch);
    }

    LCFG_getParam(&lob->cfg, ZLOB_PARM_SPD_LAUNCH, &spd_launch);

    if (lob->nav_nextbull)
    {
        F32 fac_arc = 0.0f;

        xVec3Copy(&pos_tgt, zMovePointGetPos(lob->nav_nextbull));
        xVec3Sub(&dir_launch, &pos_tgt, &pos_launch);
        xVec3Normalize(&dir_launch, &dir_launch);

        LCFG_getParam(&lob->cfg, ZLOB_PARM_ARCFACTOR, &fac_arc);
        if (fac_arc != 0.0f || bounce->acc_gravity != 0.0f)
        {
            ST_CAT_AIMDATA aim = { ZPRJ_CAIM_GRAVBALL };
            st_CAT_AIMGRAV* grav = &aim.grav;

            aim.spd_base = spd_launch;
            xVec3Copy(&aim.dir_base, &dir_launch);
            xVec3Copy(&grav->pos_src, &pos_launch);
            xVec3Copy(&grav->pos_tgt, zMovePointGetPos(lob->nav_nextbull));
            grav->acc_gravity = bounce->acc_gravity;
            grav->fac_arcness = (fac_arc < 0.0f) ? 1.0f : fac_arc;

            prj->AimAtTarget(&aim, 0);

            xVec3Copy(&dir_launch, &aim.dir_base);
        }
    }
    else if (lob->flg_suitcase & 2)
    {
        xVec3Copy(&dir_launch, &lob->dir_altLaunch);
    }
    else
    {
        LCFG_getParam(&lob->cfg, ZLOB_PARM_DIR_LAUNCH, &dir_launch);
    }

    LCFG_getParam(&lob->cfg, ZLOB_PARM_SPD_RANDPCT, &spd_randPct);
    if (spd_randPct > 0.0f)
    {
        F32 spd_base = spd_launch;
        tmp_rand = 2.0f * (iurand() - 0.5f);
        spd_launch += tmp_rand * spd_randPct;
        if (spd_randPct > 1.0f)
        {
            spd_launch = MAX(0.5f * spd_base, spd_launch);
        }
        else
        {
            spd_launch = MAX(1.0f, spd_launch);
        }
    }

    if (lob->flg_lmopts & 4)
    {
        xVec3 ang = { 0.0f, 0.0f, 0.0f };
        xMat3x3 mat_rot = {};

        ang.x = lob->ang_debrisCone * (iurand() - 0.5f);
        ang.y = lob->ang_debrisCone * (iurand() - 0.5f);
        ang.z = lob->ang_debrisCone * (iurand() - 0.5f);
        xMat3x3Euler(&mat_rot, &ang);
        xMat3x3LMulVec(&dir_launch, &mat_rot, &dir_launch);
        xVec3Normalize(&dir_launch, &dir_launch);
    }

    xVec3Copy((xVec3*)&prj->frame->mat.pos, &pos_launch);
    xVec3SMul(&prj->frame->vel, &dir_launch, spd_launch);
    prj->frame->mode |= 4;
    prj->SyncFrame(1);

    result = prj->StateSet(ZPRJ_STATE_ACTIVE);

    return result;
}

static S32 ZLOB_shoot_arrow(st_LOBMASTER_DATA* lob, S32, S32)
{
    S32 result;
    zEntProjectile* prj;
    U32 prjTypeID = 0;

    LCFG_getParam(&lob->cfg, ZLOB_PARM_PRJTYPEID, &prjTypeID);
    prj = zEntProjectile_acquire(ZPRJ_CAT_ARROW, prjTypeID);
    if (!prj)
    {
        return 0;
    }

    ZLOB_prj_setup(lob, prj);

    F32 spd_base = 0.0f;
    F32 spd_randPct = 0.0f;
    xVec3 pos_launch = { 0.0f, 0.0f, 0.0f };
    xVec3 dir_launch = { 0.0f, 0.0f, 0.0f };
    xVec3 pos_tgt = { 0.0f, 0.0f, 0.0f };
    LCFG_getParam(&lob->cfg, ZLOB_PARM_SPD_LAUNCH, &spd_base);
    LCFG_getParam(&lob->cfg, ZLOB_PARM_SPD_RANDPCT, &spd_randPct);

    F32 spd_launch = spd_base;
    if (spd_randPct > 0.0f)
    {
        F32 tmp_rand = 2.0f * (iurand() - 0.5f);
        spd_launch += tmp_rand * spd_randPct;
        if (spd_randPct > 1.0f)
        {
            spd_launch = MAX(0.5f * spd_base, spd_launch);
        }
        else
        {
            spd_launch = MAX(1.0f, spd_launch);
        }
    }

    if (lob->flg_suitcase & 1)
    {
        xVec3Copy(&pos_launch, &lob->pos_altLaunch);
    }
    else
    {
        LCFG_getParam(&lob->cfg, ZLOB_PARM_POS_LAUNCH, &pos_launch);
    }

    if (lob->nav_nextbull)
    {
        xVec3Copy(&pos_tgt, zMovePointGetPos(lob->nav_nextbull));
        xVec3Sub(&dir_launch, &pos_tgt, &pos_launch);
        xVec3Normalize(&dir_launch, &dir_launch);
    }

    if (lob->flg_suitcase & 2)
    {
        xVec3Copy(&dir_launch, &lob->dir_altLaunch);
    }
    else
    {
        LCFG_getParam(&lob->cfg, ZLOB_PARM_DIR_LAUNCH, &dir_launch);
    }

    xVec3Copy((xVec3*)&prj->frame->mat.pos, &pos_launch);
    xVec3SMul(&prj->frame->vel, &dir_launch, spd_launch);
    prj->frame->mode |= 4;
    prj->SyncFrame(1);

    result = prj->StateSet(ZPRJ_STATE_ACTIVE);

    return result;
}

static S32 ZLOB_shoot_debris(st_LOBMASTER_DATA* lob, S32 nth, S32 total)
{
    S32 result;
    zEntProjectile* prj;
    U32 prjTypeID = 0;
    st_ZPRJ_CAT_BOUNCEBALL* bounce;
    F32 fval;
    F32 ang_cone;
    S32 flg_player;
    F32 spd_base;
    F32 spd_launch;
    xVec3 pos_launch = { 0.0f, 0.0f, 0.0f };
    xVec3 dir_launch = { 0.0f, 0.0f, 0.0f };
    xVec3 pos_tgt = { 0.0f, 0.0f, 0.0f };
    xVec3 rot_tumble = { 4.0f * PI, 4.0f * PI, 4.0f * PI };
    xVec3 ang = { 0.0f, 0.0f, 0.0f };
    xMat3x3 mat_rot = {};

    LCFG_getParam(&lob->cfg, ZLOB_PARM_PRJTYPEID, &prjTypeID);
    prj = zEntProjectile_acquire(ZPRJ_CAT_STDDEBRIS, prjTypeID);
    if (!prj)
    {
        return 0;
    }

    bounce = &prj->cat_data.bounce;

    ZLOB_prj_setup(lob, prj);
    prj->flg_prop &= ~0x10;

    prj->tym_maxlife = MIN(prj->tym_maxlife, 5.0f);
    prj->ds2_maxlife = MIN(prj->ds2_maxlife, 25.0f);

    prj->OrientSet(0);

    flg_player = zEntPlayer_MoveInfo();

    prj->flags |= 0x4;
    prj->fade_tmr = 1.25f;
    if ((gTransitionSceneID ^ 0x20000000) == zSceneIDFromName("o004") &&
        (prj->id == 0xF933F89E || prj->id == 0xFFF2B7AA))
    {
        prj->fade_tmr = 0.25f;
    }
    prj->fade_tmr = 0.2f + prj->tym_maxlife;

    bounce->acc_gravity = 1.5f;
    prj->flg_categ |= 0x2;
    prj->flg_categ |= 0x2000000;
    bounce->cnt_maxBounce = 4;
    bounce->cnt_curBounce = 0;
    bounce->fac_elastic = 0.8f;
    bounce->pct_randKick = 0.0f;

    prj->flg_categ |= 0x4000000;
    xVec3SMul(&rot_tumble, &rot_tumble, 2.0f * (iurand() - 0.5f));
    prj->FlexMatSet(&rot_tumble, globals.update_dt);
    prj->FlexVecSet(&rot_tumble);

    prj->ColDelaySet(0.2f, 0);
    prj->flg_chk = 0x3E;
    prj->flg_pen = 0x7E;

    if (flg_player & 0x8)
    {
        spd_base = 5.0f;
    }
    else if (flg_player & 0x10)
    {
        spd_base = 8.0f;
    }
    else
    {
        spd_base = 6.0f;
    }

    spd_launch = spd_base + 1.8f * (2.0f * (iurand() - 0.5f));
    spd_launch = MAX(0.5f * spd_base, spd_launch);

    if (lob->flg_suitcase & 4)
    {
        ZLOB_randpos_fromBound(&lob->bnd_altLaunch, nth, total, 0xA4A, &pos_launch);
    }
    else if (lob->flg_suitcase & 1)
    {
        xVec3Copy(&pos_launch, &lob->pos_altLaunch);
    }
    else
    {
        LCFG_getParam(&lob->cfg, ZLOB_PARM_POS_LAUNCH, &pos_launch);
    }

    ang_cone = lob->ang_debrisCone;
    if (lob->flg_suitcase & 4)
    {
        xVec3Copy(&dir_launch, &g_Y3);
        if (flg_player & 0x8)
        {
            ang_cone = PI * 90.0f / 180.0f;
            dir_launch.x = 2.0f * (iurand() - 0.5f);
            dir_launch.y = 0.0f;
            dir_launch.z = 2.0f * (iurand() - 0.5f);
        }
        else if (flg_player & 0x10)
        {
            ang_cone = PI * 75.0f / 180.0f;
            xVec3Sub(&dir_launch, xBoundCenter(&lob->bnd_altLaunch), xEntGetPos(&globals.player.ent));
            if (xVec3Normalize(&dir_launch, &dir_launch) > 5.0f)
            {
                xVec3Copy(&dir_launch, &g_Y3);
            }
        }
        else
        {
            ang_cone = PI * 170.0f / 180.0f;
        }
    }
    else if (lob->flg_suitcase & 2)
    {
        xVec3Copy(&dir_launch, &lob->dir_altLaunch);
    }
    else
    {
        LCFG_getParam(&lob->cfg, ZLOB_PARM_DIR_LAUNCH, &dir_launch);
    }

    ang.x = ang.y = ang.z = 0.5f * ang_cone;
    ang.x *= 2.0f * (iurand() - 0.5f);
    ang.y *= 2.0f * (iurand() - 0.5f);
    ang.z *= 2.0f * (iurand() - 0.5f);
    xMat3x3Euler(&mat_rot, &ang);

    xMat3x3LMulVec(&dir_launch, &mat_rot, &dir_launch);
    xVec3Normalize(&dir_launch, &dir_launch);

    xVec3Copy((xVec3*)&prj->frame->mat.pos, &pos_launch);
    xVec3SMul(&prj->frame->vel, &dir_launch, spd_launch);
    prj->frame->mode |= 5;
    prj->SyncFrame(1);

    result = prj->StateSet(ZPRJ_STATE_ACTIVE);

    return result;
}

static void ZLOB_grab_nextBull(st_LOBMASTER_DATA* lob)
{
    _zMovePoint* nav_tmp = NULL;
    xVec3 hdng = { 0.0f, 0.0f, 0.0f };

    if (!lob->nav_prevbull)
    {
        lob->nav_prevbull = lob->nav_bullbase;
        lob->nav_nextbull = lob->nav_bullbase;
    }
    else
    {
        zMovePointGetNext(lob->nav_nextbull, lob->nav_prevbull, &nav_tmp, &hdng);
        lob->nav_prevbull = lob->nav_nextbull;
        lob->nav_nextbull = nav_tmp;
    }

    if (!lob->nav_nextbull)
    {
        lob->nav_nextbull = lob->nav_bullbase;
    }
}

static st_LOBMASTER_DATA* ZLOB_find_lobber(U32 aid_lobm)
{
    S32 i;
    st_LOBMASTER_DEPOT* depot = &g_zlob_depot;
    st_LOBMASTER_DATA* lob = NULL;
    st_LOBMASTER_DATA* tmp_lob;

    for (i = 0; i < depot->raw_cnt; i++)
    {
        tmp_lob = &depot->raw_pool[i];
        if (!tmp_lob->id)
        {
            continue;
        }

        if (tmp_lob->id == aid_lobm)
        {
            lob = tmp_lob;
            break;
        }
    }

    return lob;
}

static S32 ZLOB_randpos_fromBound(_xBound* bnd, S32, S32, S32 flg_ropts, xVec3* pos_launch)
{
    xVec3 delta = { 0 };
    xVec3 vec_tmp = { 0 };
    xVec3 dirvec = { 0.0f, 0.0f, 1.0f };
    xMat3x3 mat_rot = {};
    xVec3 rot_range = { 2 * PI, 2 * PI, 2 * PI };
    S32 result = 1;
    F32 rad;
    F32 hyt;

    if (bnd->type == 2 || bnd->type == 4)
    {
        xVec3Sub(&vec_tmp, &bnd->box.box.upper, &bnd->box.box.lower);
        if (flg_ropts & 0x20)
        {
            xVec3SMul(&delta, &vec_tmp, 0.5f);
            delta.x *= 2.0f * (iurand() - 0.5f);
            delta.y *= 2.0f * (iurand() - 0.5f);
            delta.z *= 2.0f * (iurand() - 0.5f);
        }
        else
        {
            xVec3SMul(&vec_tmp, &vec_tmp, 0.5f);
            xVec3Copy(&delta, &vec_tmp);
            delta.x *= 2.0f * (iurand() - 0.5f);
            delta.y *= 2.0f * (iurand() - 0.5f);
            delta.z *= 2.0f * (iurand() - 0.5f);
            if (xUtil_yesno(0.333f))
            {
                if (xUtil_yesno())
                {
                    delta.x = vec_tmp.x;
                }
                else
                {
                    delta.x = -vec_tmp.x;
                }
            }
            else if (xUtil_yesno(0.333f))
            {
                if (xUtil_yesno(0.75f))
                {
                    delta.y = vec_tmp.y;
                }
                else
                {
                    delta.y = -vec_tmp.y;
                }
            }
            else
            {
                if (xUtil_yesno())
                {
                    delta.z = vec_tmp.z;
                }
                else
                {
                    delta.z = -vec_tmp.z;
                }
            }
        }

        if (bnd->type == 4)
        {
            xMat3x3LMulVec(&delta, (xMat3x3*)bnd->mat, &delta);
        }
        xVec3Add(pos_launch, &delta, &bnd->box.center);
    }
    else if (bnd->type == 1)
    {
        if (flg_ropts & 1)
        {
            rot_range.x *= iurand();
            rot_range.y *= iurand();
            rot_range.z *= iurand();
        }
        else if (flg_ropts & 2)
        {
            rot_range.y = PI;
            rot_range.z = 0.0f;
            rot_range.x *= iurand();
            rot_range.y *= iurand();
            rot_range.z *= iurand();
        }
        else if (flg_ropts & 4)
        {
            rot_range.y = PI;
            rot_range.z = 0.0f;
            rot_range.x *= iurand();
            rot_range.y *= iurand();
            rot_range.z *= iurand();
            rot_range.y += PI;
        }

        xMat3x3Euler(&mat_rot, &rot_range);
        xMat3x3LMulVec(&dirvec, &mat_rot, &dirvec);
        if (flg_ropts & 8)
        {
            xVec3SMul(&delta, &dirvec, bnd->sph.r);
        }
        else
        {
            xVec3SMul(&delta, &dirvec, bnd->sph.r * iurand());
        }
        xVec3Add(pos_launch, &delta, &bnd->sph.center);
    }
    else if (bnd->type == 3)
    {
        rad = bnd->cyl.r;
        hyt = 0.5f * bnd->cyl.h;
        if (flg_ropts & 0x800)
        {
            if (xUtil_yesno(0.25f))
            {
                rad = bnd->cyl.r * iurand();
            }
            else
            {
                hyt = 0.5f * bnd->cyl.h * (2.0f * (iurand() - 0.5f));
            }
        }
        else
        {
            hyt = 0.5f * bnd->cyl.h * (2.0f * (iurand() - 0.5f));
            rad = bnd->cyl.r * iurand();
        }

        if (!(flg_ropts & 0x100))
        {
            if ((flg_ropts & 0x200) && hyt < 0.0f)
            {
                hyt = -hyt;
            }
            else if ((flg_ropts & 0x400) && hyt > 0.0f)
            {
                hyt = -hyt;
            }
        }

        delta.x = rad * (2.0f * (iurand() - 0.5f));
        delta.y = hyt;
        delta.z = rad * (2.0f * (iurand() - 0.5f));
        xVec3Add(pos_launch, &delta, &bnd->cyl.center);
    }
    else
    {
        result = 0;
    }

    return result;
}

static S32 LPRJ_debris_attach(zEntProjectile* prj)
{
    st_ZPRJ_CAT_BOUNCEBALL* bounce = &prj->cat_data.bounce;

    prj->cat_data.idx_cdat = ZPRJ_CDAT_BOUNCE;
    bounce->tym_atrest = 3.0f;
    bounce->acc_gravity = 15.0f;
    bounce->cnt_maxBounce = 5;
    bounce->fac_elastic = 0.3f;
    bounce->pct_randKick = 0.0f;

    return 1;
}

static S32 LPRJ_debris_logix(zEntProjectile* prj, F32 dt, zScene*, _xEntFrame* frame)
{
    st_ZPRJ_CAT_BOUNCEBALL* bounce = &prj->cat_data.bounce;
    xVec3 vec_accum = { 0 };

    if (bounce->atRest)
    {
        prj->SendEvent(ZPRJ_EVNT_ATREST, dt);
    }
    else
    {
        prj->SendEvent(ZPRJ_EVNT_INFLIGHT, dt);
    }

    if (prj->tmr_alive > prj->tym_maxlife)
    {
        prj->SendEvent(ZPRJ_EVNT_SELFDESTRUCT, dt);
        prj->StateSet(ZPRJ_STATE_DEAD);
        return 1;
    }

    xVec3 tmp_vec = { 0 };
    xVec3Sub(&tmp_vec, xEntGetPos(prj), &prj->pos_home);
    if (xVec3Dot(&tmp_vec, &tmp_vec) > prj->ds2_maxlife)
    {
        prj->SendEvent(ZPRJ_EVNT_SELFDESTRUCT, dt);
        prj->StateSet(ZPRJ_STATE_DEAD);
        return 1;
    }

    if (bounce->atRest == 1)
    {
        bounce->tym_atrest -= dt;
    }

    if ((prj->plyr_colrec->flags & 1) && (prj->flg_prop & 0x10))
    {
        zEntPlayer_Damage(prj, 1);
    }

    if (bounce->atRest == 1 && bounce->tym_atrest < 0.0f)
    {
        prj->SendEvent(ZPRJ_EVNT_DETONATE, dt);
        prj->StateSet(ZPRJ_STATE_DEAD);
    }
    else if ((prj->plyr_colrec->flags & 1) || (prj->collis->colls[0].flags & 1) ||
        (prj->collis->colls[1].flags & 1) || prj->collis->env_sidx < prj->collis->env_eidx ||
        prj->collis->npc_sidx < prj->collis->npc_eidx ||
        prj->collis->stat_sidx < prj->collis->stat_eidx ||
        prj->collis->dyn_sidx < prj->collis->dyn_eidx)
    {
        if ((prj->flg_categ & 0x2000000) && bounce->skip_next_bounce &&
            bounce->cnt_curBounce < bounce->cnt_maxBounce)
        {
            bounce->skip_next_bounce = 0;
            xVec3Copy(&prj->frame->vel, &prj->frame->oldvel);
            prj->frame->mode |= 4;
        }
        else if ((prj->flg_categ & 0x2000000) && bounce->cnt_curBounce < bounce->cnt_maxBounce)
        {
            bounce->cnt_curBounce++;
            bounce->skip_next_bounce = 1;
            bounce->atRest = 2;

            F32 elastic = bounce->fac_elastic;
            if (prj->flg_categ & 2)
            {
                elastic *= 0.5f;
            }

            zEntProjectile_colAccum(prj, &vec_accum);
            zEntProjectile_softReflect(&prj->frame->vel, &prj->frame->oldvel, &vec_accum, elastic);
            prj->frame->mode |= 4;
            prj->SendEvent(ZPRJ_EVNT_BOUNCE, dt);

            if (xVec3Dot(&frame->vel, &frame->vel) < 0.2f * 0.2f)
            {
                bounce->cnt_curBounce = bounce->cnt_maxBounce;
            }
        }
        else
        {
            prj->SendEvent(ZPRJ_EVNT_DETONATE, dt);
            prj->StateSet(ZPRJ_STATE_DEAD);
        }
    }

    return 1;
}

static S32 LPRJ_debris_prjmove(zEntProjectile* prj, F32 dt, zScene*, _xEntFrame* frame)
{
    st_ZPRJ_CAT_BOUNCEBALL* bounce = &prj->cat_data.bounce;

    if (bounce->atRest == 1)
    {
        prj->frame->dvel.x = 0.0f;
        prj->frame->dvel.y = 0.0f;
        prj->frame->dvel.z = 0.0f;
        prj->frame->vel.x = 0.0f;
        prj->frame->vel.y = 0.0f;
        prj->frame->vel.z = 0.0f;
        prj->frame->mode |= 0xC;
    }
    else
    {
        frame->dvel.x = 0.0f;
        if (prj->flg_categ & 0x2)
        {
            frame->dvel.y -= dt * bounce->acc_gravity;
        }
        else
        {
            frame->dvel.y = -(dt * bounce->acc_gravity);
        }
        frame->dvel.z = 0.0f;
        frame->mode |= 0x8;
    }

    if ((prj->flg_categ & 0x4000000) && bounce->atRest != 1)
    {
        xMat4x3RMul3x3(&frame->mat, &prj->common.mat_flex, &frame->mat);
        frame->mode |= 0x100;
    }

    return 1;
}
