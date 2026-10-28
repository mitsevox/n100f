#include "zEntProjectile.h"
#include "zEntPlayer.h"
#include "zGlobals.h"
#include "zScene.h"
#include "zParEmitter.h"
#include "zGust.h"
#include "xLinkList.h"
#include "xMath.h"
#include "xMath3.h"
#include "xMemMgr.h"
#include "xBound.h"
#include "xFFX.h"
#include "xShadow.h"
#include "iShadow.h"
#include "xModel.h"
#include "xString.h"
#include "xstransvc.h"
#include "xutil.h"

#include <math.h>
#include <string.h>

struct zSFX;

static S32 ZPRJ_solv_velvec(xVec3 vec_tgtOrig, F32 grav, F32 arc_factor, F32 spd_limit, F32 dst_range,
                            xVec3* vec_vel);
static S32 ZPRJ_bounce_calcmove(zEntProjectile* prj, F32 dt, zScene*, _xEntFrame* frame);
static S32 ZPRJ_bounce_logix(zEntProjectile* prj, F32 dt, zScene*, _xEntFrame* frame);
static S32 ZPRJ_bounce_attach(zEntProjectile* prj);
static S32 ZPRJ_grav_calcmove(zEntProjectile* prj, F32 dt, zScene*, _xEntFrame* frame);
static S32 ZPRJ_grav_logix(zEntProjectile* prj, F32 dt, zScene*, _xEntFrame*);
static S32 ZPRJ_grav_attach(zEntProjectile* prj);
static S32 ZPRJ_arrow_logix(zEntProjectile* prj, F32 dt, zScene*, _xEntFrame*);
static S32 ZPRJ_arrow_attach(zEntProjectile* prj);
void ZPRJ_wrap_bupdate(_xEnt* prjent, xVec3* pos);
zEntProjectile* ZPRJ_getNextAvail();
static st_ZPRJ_TYPERECORD* ZPRJ_findType(U32 aid);
static void ZPRJ_insertType(zProjectileTypeAsset* ptype);

static st_PROJECTILE_DEPOT g_prjdepot = {};
static st_ZPRJ_CATEGORY_FUNCS g_catFuncs[ZPRJ_CAT_NOMORE] = {};

static inline st_ZPRJ_CATEGORY_FUNCS* ZPRJ_catFuncs(zEntProjectile* prj)
{
    return &g_catFuncs[prj->cat_proj];
}
static st_ZPRJ_CATEGORY_PROPS g_iniProps[ZPRJ_CAT_NOMORE] = {};
static _zParEmitter* g_paremit[32] = {};
static char* g_parname[32] = {
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    "PAREMIT_PROJFLIGHT_SMOKE",
    "PAREMIT_PROJFLIGHT_WOOD",
    "PAREMIT_PROJFLIGHT_TAR",
    NULL,
    "PAREMIT_PROJFLIGHT_FIREBALL",
    "PAREMIT_PROJFLIGHT_NO_DAMAGE_FIREBALL",
    "PAREMIT_PROJFLIGHT_WITCHDOCTOR",
    "PAREMIT_PROJFLIGHT_MASTERMIND",
};
static zSFX* g_sounds[1] = {};
static char* g_sfxnames[1] = {};
static S32 g_prj_init;

void zEntProjectile_Startup()
{
    st_ZPRJ_CATEGORY_FUNCS fmap_gen = {
        1,    zEntProjectile_gen_attach, NULL, zEntProjectile_gen_logix, NULL, NULL, zEntProjectile_gen_render,
        zEntProjectile_gen_aimer,        NULL, NULL
    };
    st_ZPRJ_CATEGORY_FUNCS fmap_arrow = {
        1, ZPRJ_arrow_attach, NULL, ZPRJ_arrow_logix, NULL, NULL, zEntProjectile_gen_render, zEntProjectile_gen_aimer,
        NULL, NULL
    };
    st_ZPRJ_CATEGORY_FUNCS fmap_grav = {
        1,    ZPRJ_grav_attach, NULL, ZPRJ_grav_logix, ZPRJ_grav_calcmove, NULL, zEntProjectile_gen_render,
        zEntProjectile_gen_aimer, NULL, NULL
    };
    st_ZPRJ_CATEGORY_FUNCS fmap_bounce = {
        1,    ZPRJ_bounce_attach, NULL, ZPRJ_bounce_logix, ZPRJ_bounce_calcmove, NULL, zEntProjectile_gen_render,
        zEntProjectile_gen_aimer, NULL, NULL
    };
    st_ZPRJ_CATEGORY_PROPS prop_gen = { 0x80000001, 0x03, 0x00, 0x00, 0, 0 };
    st_ZPRJ_CATEGORY_PROPS prop_arrow = { 0xB0000021, 0x03, 0x3F, 0x00, 0, 0 };
    st_ZPRJ_CATEGORY_PROPS prop_grav = { 0xB0000021, 0x03, 0x3F, 0x7E, 0, 0 };
    st_ZPRJ_CATEGORY_PROPS prop_bounce = { 0xB0000021, 0x03, 0x3F, 0x7E, 0, 0x02000000 };

    zEntProjectile_addCat(ZPRJ_CAT_UNKNOWN, &fmap_gen);
    zEntProjectile_addProps(ZPRJ_CAT_UNKNOWN, &prop_gen);

    zEntProjectile_addCat(ZPRJ_CAT_ARROW, &fmap_arrow);
    zEntProjectile_addProps(ZPRJ_CAT_ARROW, &prop_arrow);

    zEntProjectile_addCat(ZPRJ_CAT_GRAVBALL, &fmap_grav);
    zEntProjectile_addProps(ZPRJ_CAT_GRAVBALL, &prop_grav);

    zEntProjectile_addCat(ZPRJ_CAT_BOUNCEBALL, &fmap_bounce);
    zEntProjectile_addProps(ZPRJ_CAT_BOUNCEBALL, &prop_bounce);
}

void zEntProjectile_Shutdown()
{
}

S32 zEntProjectile_addCat(en_ZPRJ_CATEGORY cat, st_ZPRJ_CATEGORY_FUNCS* funcs)
{
    S32 isok = 1;
    st_ZPRJ_CATEGORY_FUNCS* catmap = &g_catFuncs[cat];

    if (cat >= ZPRJ_CAT_NOMORE)
    {
        isok = 0;
    }
    else
    {
        if (catmap->flg_cback & 1)
        {
            isok = -1;
        }
        memcpy(catmap, funcs, sizeof(st_ZPRJ_CATEGORY_FUNCS));
    }

    if (catmap->render == NULL)
    {
        catmap->render = zEntProjectile_gen_render;
    }

    return isok;
}

S32 zEntProjectile_addProps(en_ZPRJ_CATEGORY cat, st_ZPRJ_CATEGORY_PROPS* props)
{
    S32 isok = 1;

    if (cat >= ZPRJ_CAT_NOMORE)
    {
        isok = 0;
    }
    else
    {
        memcpy(&g_iniProps[cat], props, sizeof(st_ZPRJ_CATEGORY_PROPS));
    }

    return isok;
}

void zEntProjectile_registerType(zProjectileTypeAsset* ptype)
{
    ZPRJ_insertType(ptype);
}

void zEntProjectile_scenePrepare()
{
    st_PROJECTILE_DEPOT* depot = &g_prjdepot;

    if (g_prj_init++ == 0)
    {
        memset(depot, 0, sizeof(st_PROJECTILE_DEPOT));
        depot->rawcnt = 0;
        depot->rawpool = NULL;
        depot->typetable = NULL;
        NewList(&depot->prjpool);
        NewList(&depot->prjinuse);
    }
}

void zEntProjectile_sceneReset()
{
    st_PROJECTILE_DEPOT* depot = &g_prjdepot;
    st_ZPRJ_LINKNODE* link;

    while (depot->prjinuse.head->next != NULL)
    {
        link = (st_ZPRJ_LINKNODE*)RemHead(&depot->prjinuse);
        link->backref->Recycle();
    }
}

void zEntProjectile_sceneFinish()
{
    st_PROJECTILE_DEPOT* depot = &g_prjdepot;

    if (--g_prj_init == 0)
    {
        depot->rawcnt = 0;
        depot->rawpool = NULL;
        NewList(&depot->prjpool);
        NewList(&depot->prjinuse);
    }
}

void zEntProjectile_prepSystem(S32 num_ptype, zScene*)
{
    S32 i;
    st_PROJECTILE_DEPOT* depot = &g_prjdepot;

    if (depot->typetable != NULL)
    {
        return;
    }

    if (depot->rawpool != NULL)
    {
        return;
    }

    if (num_ptype != 0)
    {
        depot->typetable =
            (num_ptype < 1) ? NULL
                            : (st_ZPRJ_TYPERECORD*)memset(xMemAlloc(gActiveHeap, num_ptype * sizeof(st_ZPRJ_TYPERECORD)),
                                                          0, num_ptype * sizeof(st_ZPRJ_TYPERECORD));
        if (depot->typetable != NULL)
        {
            depot->typecnt = num_ptype;
        }
    }

    if (depot->typetable != NULL)
    {
        depot->rawpool = (zEntProjectile*)memset(xMemAlloc(gActiveHeap, 100 * sizeof(zEntProjectile)), 0x33,
                                                 100 * sizeof(zEntProjectile));
        if (depot->rawpool != NULL)
        {
            depot->rawcnt = 100;
            xModelPoolInit(100, 2);
        }
    }

    for (i = 0; i < depot->rawcnt; i++)
    {
        memset(&depot->rawpool[i], 0, sizeof(zEntProjectile));
        depot->rawpool[i].linknode.backref = &depot->rawpool[i];
        AddTail(&depot->prjpool, &depot->rawpool[i].linknode.node);
    }

    if (g_sfxnames[0] == NULL)
    {
        g_sounds[0] = NULL;
    }
    else
    {
        g_sounds[0] = (zSFX*)zSceneFindObject(xStrHash(g_sfxnames[0]));
    }

    for (i = 0; i < 32; i++)
    {
        if (g_parname[i] == NULL)
        {
            g_paremit[i] = NULL;
        }
        else
        {
            g_paremit[i] = (_zParEmitter*)zSceneFindObject(xStrHash(g_parname[i]));
        }
    }
}

S32 zEntProjectile_poolUpdate(en_ZPRJ_CATEGORY, zScene* scn, F32 dt)
{
    st_ELIST holdlist;
    S32 cnt = 0;
    zEntProjectile* tmpprj;
    st_ZPRJ_LINKNODE* tmplink;
    st_PROJECTILE_DEPOT* depot = &g_prjdepot;

    for (tmplink = (st_ZPRJ_LINKNODE*)depot->prjinuse.head; tmplink->node.next != NULL;
         tmplink = (st_ZPRJ_LINKNODE*)tmplink->node.next)
    {
        tmplink->backref->ZPRJ_update(scn, dt);
        cnt++;
    }

    NewList(&holdlist);
    MoveList(&holdlist, &depot->prjinuse);

    while (holdlist.head->next != NULL)
    {
        tmplink = (st_ZPRJ_LINKNODE*)RemHead(&holdlist);
        tmpprj = tmplink->backref;

        if (tmpprj->flg_stat & 0x80000000)
        {
            tmpprj->Recycle();
        }
        else
        {
            AddTail(&depot->prjinuse, &tmpprj->linknode.node);
        }
    }

    return cnt;
}

void zEntProjectile_poolRender(en_ZPRJ_CATEGORY cat)
{
    zEntProjectile* tmpprj;
    st_ZPRJ_LINKNODE* tmplink;

    for (tmplink = (st_ZPRJ_LINKNODE*)g_prjdepot.prjinuse.head; tmplink->node.next != NULL;
         tmplink = (st_ZPRJ_LINKNODE*)tmplink->node.next)
    {
        tmpprj = tmplink->backref;

        if (cat == ZPRJ_CAT_UNKNOWN || cat == tmpprj->cat_proj)
        {
            if (!(tmpprj->flg_stat & 0x40000000))
            {
                tmpprj->ZPRJ_render();
                tmpprj->flg_stat |= 0x40000000;
            }
        }
    }
}

static void ZPRJ_insertType(zProjectileTypeAsset* ptype)
{
    S32 idx;
    st_PROJECTILE_DEPOT* depot = &g_prjdepot;

    if (depot->typeidx < depot->typecnt)
    {
        idx = depot->typeidx++;
        depot->typetable[idx].src_ptype = ptype;
        depot->typetable[idx].aid_ptype = ptype->id;
        depot->typetable[idx].mdlFile[ZPRJ_MDLIDX_STD] = (RpAtomic*)xSTFindAsset(ptype->modelID, NULL);
        if (ptype->atRestModelID != 0)
        {
            depot->typetable[idx].mdlFile[ZPRJ_MDLIDX_POSTHIT] = (RpAtomic*)xSTFindAsset(ptype->atRestModelID, NULL);
        }
        else
        {
            depot->typetable[idx].mdlFile[ZPRJ_MDLIDX_POSTHIT] = NULL;
        }
    }
}

static st_ZPRJ_TYPERECORD* ZPRJ_findType(U32 aid)
{
    st_ZPRJ_TYPERECORD* da_type = NULL;
    S32 i;
    st_PROJECTILE_DEPOT* depot = &g_prjdepot;

    for (i = 0; i < depot->typeidx; i++)
    {
        if (depot->typetable[i].aid_ptype == aid)
        {
            da_type = &depot->typetable[i];
            break;
        }
    }

    return da_type;
}

en_ZPRJ_CATEGORY zEntProjectile_catFromPrjType(U32 ptypeID, en_ZPRJ_CATEGORY prefer)
{
    st_ZPRJ_TYPERECORD* ptype = ZPRJ_findType(ptypeID);

    if (ptype == NULL)
    {
        return prefer;
    }

    if (ptype->categ != ZPRJ_CAT_UNKNOWN)
    {
        return ptype->categ;
    }

    return prefer;
}

zEntProjectile* zEntProjectile_acquire(en_ZPRJ_CATEGORY cat, U32 ptypeID)
{
    zEntProjectile* prj;
    S32 status;
    S32 rc;

    status = 1;
    prj = ZPRJ_getNextAvail();

    if (prj == NULL)
    {
        return prj;
    }

    AddHead(&g_prjdepot.prjinuse, &prj->linknode.node);

    rc = prj->Init(cat, ptypeID);
    if (!rc)
    {
        status = 0;
    }

    if (!status)
    {
        prj->flg_stat |= 0x10000000;
        prj->flg_stat |= 0x80000000;
        prj->state = ZPRJ_STATE_DEAD;
        prj = NULL;
    }

    return prj;
}

en_ZPRJ_STATES zEntProjectile::StateSet(en_ZPRJ_STATES state)
{
    en_ZPRJ_STATES oldstate;

    if (this->state == state)
    {
        return this->state;
    }

    switch (this->state)
    {
    case ZPRJ_STATE_DEAD:
        break;
    case ZPRJ_STATE_ACTIVE:
    case ZPRJ_STATE_STANDBY:
        flg_stat &= 0xFFFF;
        flg_stat |= 0x10000;
        break;
    case ZPRJ_STATE_UNUSED:
        break;
    }

    oldstate = this->state;
    this->state = state;
    tmr_curstate = 0.0f;

    switch (this->state)
    {
    case ZPRJ_STATE_ACTIVE:
        tmr_alive = 0.0f;
        tmr_emit = 0.0f;
        xVec3Copy(&pos_home, (xVec3*)&model->Mat->pos);
        flg_stat |= 0x20000;
        SendEvent(ZPRJ_EVNT_ACTIVE, globals.update_dt, NULL, NULL);
        break;
    case ZPRJ_STATE_DEAD:
        flg_stat |= 0x80000000;
        this->state = ZPRJ_STATE_DEAD;
        break;
    case ZPRJ_STATE_STANDBY:
        break;
    case ZPRJ_STATE_UNUSED:
        break;
    }

    if (oldstate != state)
    {
        SendEvent(ZPRJ_EVNT_TRANSTATE, globals.update_dt, (void*)oldstate, NULL);
    }

    return this->state;
}

void zEntProjectile::ColDelaySet(F32 tym, S32 addto)
{
    if (tmr_coldelay < 0.0f)
    {
        tmr_coldelay = tym;
    }
    else if (addto)
    {
        tmr_coldelay += tym;
    }
    else
    {
        tmr_coldelay = tym;
    }
}

void zEntProjectile::Abort()
{
    flg_stat |= 0x40000;
    SendEvent(ZPRJ_EVNT_ABORT, globals.update_dt, NULL, NULL);
    StateSet(ZPRJ_STATE_DEAD);
}

void zEntProjectile::SyncFrame(S32 applyToModel)
{
    _xEnt* ent = this;
    xVec3* mpos;
    xVec3* upos;

    if (ent->frame->mode != 0)
    {
        xEntMotionToMatrix(ent, ent->frame);

        if (applyToModel)
        {
            if (!(ent->frame->mode & 0x20000))
            {
                memcpy(ent->model->Mat, &ent->frame->mat, sizeof(xMat3x3));
            }

            if (!(ent->frame->mode & 0x10000))
            {
                upos = &ent->frame->mat.pos;
                mpos = (xVec3*)&ent->model->Mat->pos;
                xVec3Copy(mpos, upos);
            }

            xModelEval(ent->model);

            if (ent->bupdate != NULL)
            {
                ent->bupdate(ent, (xVec3*)&ent->model->Mat->pos);
            }
        }

        ent->frame->mode = 0;
    }
}

S32 zEntProjectile::SendEvent(en_PROJECTILE_EVENT event)
{
    return SendEvent(event, globals.update_dt);
}

S32 zEntProjectile::SendEvent(en_PROJECTILE_EVENT event, F32 dt)
{
    S32 rc = SendEvent(event, dt, NULL, NULL);

    if (flg_prop & 0x20)
    {
        DoEffects(event, dt);
    }

    return rc;
}

S32 zEntProjectile::SendEvent(en_PROJECTILE_EVENT event, F32 dt, void* edata, S32* res_user)
{
    S32 rc = 0x45;
    S32 result;

    if (ZPRJ_catFuncs(this)->notify != NULL)
    {
        rc = ZPRJ_catFuncs(this)->notify(this, event, edata);
    }

    if (ownerNotify != NULL)
    {
        result = ownerNotify(this, event, edata);
        if (res_user != NULL)
        {
            *res_user = result;
        }
    }

    return rc;
}

void zEntProjectile::FlexVecSet(xVec3* vec_flex)
{
    xVec3Copy(&common.vec_flex, vec_flex);
}

void zEntProjectile::FlexMatSet(xVec3* rot_euler, F32 dt)
{
    xVec3 ang = {};

    if (dt > 0.0f)
    {
        xVec3SMul(&ang, rot_euler, dt);
    }
    else
    {
        xVec3Copy(&ang, rot_euler);
    }

    xMat3x3Euler(&common.mat_flex, &ang);
}

S32 zEntProjectile::AssignModel(en_ZPRJ_MODEL_INDEX midx)
{
    st_ZPRJ_TYPERECORD* ptype = this->ptype;
    xMat4x3 mat_copy = {};
    S32 have_copy = 0;

    if (ptype->mdlFile[midx] == NULL)
    {
        return 0;
    }

    if (model != NULL)
    {
        memcpy(&mat_copy, model->Mat, sizeof(xMat4x3));
        have_copy = 1;
        xModelInstanceFree(model);
    }

    model = NULL;
    model = xModelInstanceAlloc(ptype->mdlFile[midx], this, 0, 0, NULL);

    if (have_copy)
    {
        memcpy(model->Mat, &mat_copy, sizeof(xMat4x3));
    }

    return 1;
}

S32 zEntProjectile::AimAtTarget(ST_CAT_AIMDATA* info, S32 apply)
{
    S32 result;

    if (ZPRJ_catFuncs(this)->aimer != NULL)
    {
        result = ZPRJ_catFuncs(this)->aimer(this, info, apply);
    }
    else
    {
        result = 0;
    }

    return result;
}

zEntProjectile* ZPRJ_getNextAvail()
{
    st_PROJECTILE_DEPOT* depot = &g_prjdepot;
    st_ZPRJ_LINKNODE* link;
    zEntProjectile* prj;
    static S32 warned = 0;

    if (depot->prjpool.head->next == NULL)
    {
        if (!warned)
        {
            warned = 1;
        }
        prj = NULL;
    }
    else
    {
        warned = 0;
        link = (st_ZPRJ_LINKNODE*)RemHead(&depot->prjpool);
        prj = link->backref;
        memset(prj, 0, sizeof(zEntProjectile));
        prj->linknode.backref = prj;
    }

    return prj;
}

void zEntProjectile::Recycle()
{
    if (!(flg_stat & 0x10000000) && !(flg_stat & 0x40000))
    {
        SendEvent(ZPRJ_EVNT_DEATH, globals.update_dt, NULL, NULL);
    }

    if (!(flg_stat & 0x8000000))
    {
        if (ZPRJ_catFuncs(this)->detach != NULL)
        {
            ZPRJ_catFuncs(this)->detach(this);
        }
    }

    flg_stat = 0;

    if (model != NULL)
    {
        xModelInstanceFree(model);
    }

    model = NULL;
    AddTail(&g_prjdepot.prjpool, &linknode.node);
}

S32 zEntProjectile::Init(en_ZPRJ_CATEGORY cat, U32 ptypeID)
{
    S32 rc;
    S32 status = 1;
    st_ZPRJ_CATEGORY_FUNCS* catf = NULL;

    rc = SetupPrelim(cat, ptypeID);
    if (!rc)
    {
        status = 0;
    }

    if (status)
    {
        catf = &g_catFuncs[cat_proj];
        if (!(catf->flg_cback & 1))
        {
            status = 0;
        }
    }

    if (status)
    {
        asset = &raw_asset;
        asset->flags = 1;
        asset->redMult = 1.0f;
        asset->greenMult = 1.0f;
        asset->blueMult = 1.0f;
        asset->seeThru = 1.0f;
        asset->seeThruSpeed = 0.5f;
        asset->modelInfoID = 0;
        asset->animListID = 0;
    }

    if (status)
    {
        rc = Setup_xEnt();
        if (!rc)
        {
            status = 0;
        }
    }

    if (status)
    {
        rc = SetupFrame(NULL, NULL);
        if (!rc)
        {
            status = 0;
        }
    }

    if (status)
    {
        xEntBeginUpdate(this, globals.sceneCur, globals.update_dt);
    }

    if (status)
    {
        static F32 pct_docol_dflt = 1.0f;

        pct_docol = 0.75f;
        pct_docol = pct_docol_dflt;
        tmr_coldelay = 0.1f;
    }

    if (status)
    {
        rc = catf->attach(this);
        if (!rc)
        {
            flg_stat |= 0x8000000;
            status = 0;
        }
    }

    return status;
}

S32 zEntProjectile::SetupPrelim(en_ZPRJ_CATEGORY cat, U32 ptaid)
{
    st_ZPRJ_CATEGORY_PROPS* prop;
    st_ZPRJ_TYPERECORD* ptype;
    S32 isok = 1;

    flg_stat |= 0x10000;
    state = ZPRJ_STATE_STANDBY;
    cat_proj = cat;

    prop = &g_iniProps[cat_proj];
    flg_prop = prop->flg_props;
    flg_chk = g_iniProps[cat_proj].flg_chk;
    flg_pen = g_iniProps[cat_proj].flg_pen;
    flg_categ = prop->flg_categ;

    if (ptaid == 0)
    {
        return 0;
    }

    ptype = ZPRJ_findType(ptaid);
    if (ptype == NULL)
    {
        return 0;
    }

    this->ptype = ptype;
    tym_maxlife = this->ptype->src_ptype->destructTime;
    ds2_maxlife = this->ptype->src_ptype->destructDist * this->ptype->src_ptype->destructDist;

    if (this->ptype->src_ptype->oriented)
    {
        OrientSet(0x30000000);
    }

    return isok;
}

S32 zEntProjectile::Setup_xEnt()
{
    id = 0;
    baseType = 0x19;
    baseFlags = 5;
    linkCount = 0;
    link = NULL;
    flags = 1;
    moreFlags = 0;
    collType = 4;
    collLev = 4;
    pflags = g_iniProps[cat_proj].flg_physics;
    chkby = 0;
    penby = 0;
    idx = 0;
    material_alpha = 255.0f;
    updateAcc = 0.0f;
    ent_shadow = &raw_shadow;
    xVec3Copy(&ent_shadow->vec, &g_NY3);
    collis = NULL;
    frame = &raw_frame;
    eventFunc = NULL;
    update = NULL;
    bupdate = ZPRJ_wrap_bupdate;
    move = NULL;
    transl = NULL;
    bound.type = 1;

    AssignModel(ZPRJ_MDLIDX_STD);

    if (globals.sceneCur->baseCount[28] != 0 && (g_iniProps[cat_proj].flg_props & 0x2))
    {
        xFFXAddEffect(this, zGustUpdateEnt, &raw_gust);
    }

    return 1;
}

S32 zEntProjectile::SetupFrame(xVec3* pos, xVec3* veloc)
{
    xMat4x3 initmat = {};
    xVec3 vel_dflt = { 0.0f, 0.0f, 0.0f };
    xVec3 pos_dflt = { 0.0f, 0.0f, 0.0f };

    if (pos == NULL)
    {
        pos = &pos_dflt;
    }

    if (veloc == NULL)
    {
        veloc = &vel_dflt;
    }

    xMat3x3Euler((xMat3x3*)&initmat, 0.0f, 0.0f, 0.0f);
    xVec3SMul(&initmat.right, &initmat.right, 1.0f);
    xVec3SMul(&initmat.up, &initmat.up, 1.0f);
    xVec3SMul(&initmat.at, &initmat.at, 1.0f);
    initmat.flags = 0;
    xVec3Copy(&initmat.pos, pos);
    xVec3Copy(&frame->oldpos, pos);

    bupdate(this, pos);

    memcpy(model->Mat, &initmat, sizeof(xMat4x3));
    memcpy(&frame->mat, &initmat, sizeof(xMat4x3));

    xVec3Copy(&frame->rot.axis, &g_Y3);
    frame->rot.angle = 0.0f;
    frame->oldrot = frame->rot;
    xVec3Copy(&frame->vel, veloc);
    xVec3Copy(&frame->oldvel, veloc);

    return 1;
}

void zEntProjectile::OrientSet(S32 orient)
{
    flg_prop &= ~0x70000000;
    flg_prop |= orient;
}

void zEntProjectile::OrientToPath(_xEntFrame* frame, S32 flg_orient)
{
    xVec3* camera_pos;
    S32 opt_orient;
    xVec3 vec_tmp = {};

    opt_orient = flg_orient & 0x70000000;

    switch (opt_orient)
    {
    case 0x30000000:
        xVec3SMul(&vec_tmp, &frame->vel, -1.0f);
        xMat3x3LookVec((xMat3x3*)&frame->mat, &vec_tmp);
        break;
    case 0x10000000:
        xVec3Sub(&vec_tmp, &frame->mat.pos, xBoundCenter(&globals.player.ent.bound));
        xMat3x3LookVec((xMat3x3*)&frame->mat, &vec_tmp);
        break;
    case 0x20000000:
        camera_pos = &globals.camera.hi_cam.mat.pos;
        xVec3Sub(&vec_tmp, &frame->mat.pos, camera_pos);
        xMat3x3LookVec((xMat3x3*)&frame->mat, &vec_tmp);
        break;
    case 0:
    default:
        break;
    }
}

S32 zEntProjectile::ZPRJ_update(zScene* scn, F32 dt)
{
    S32 rc;
    _xCollis lcl_xcollis_plyr = {};
    _xEntCollis lcl_xentcollis = {};
    st_ZPRJ_CATEGORY_FUNCS* catf;

    if (state == ZPRJ_STATE_STANDBY)
    {
        return 1;
    }

    if (state == ZPRJ_STATE_DEAD)
    {
        return 1;
    }

    if (!(flg_stat & 0x20000))
    {
        return 0;
    }

    plyr_colrec = &lcl_xcollis_plyr;
    collis = &lcl_xentcollis;
    catf = &g_catFuncs[cat_proj];
    collis->chk = flg_chk;
    collis->pen = flg_pen;

    if (frame != NULL)
    {
        if (frame->mode != 0)
        {
            SyncFrame(1);
        }
        frame = NULL;
    }

    frame = &raw_frame;

    xEntEndUpdate(this, scn, dt);
    xEntBeginUpdate(this, scn, dt);
    xEntBeginCollide(this, scn, dt);

    memset(plyr_colrec, 0, sizeof(_xCollis));
    plyr_colrec->optr = &globals.player.ent;
    plyr_colrec->oid = globals.player.ent.id;
    plyr_colrec->flags = 0x1F00;

    if (collis->chk & 0x10)
    {
        xBoundHitsBound(&bound, &globals.player.ent.bound, plyr_colrec);
    }

    if (DoColCheck(dt))
    {
        xEntCollide(this, scn, dt);

        if (collis->idx > 6 && !(collis->colls[0].flags & 1) && !(collis->colls[1].flags & 1))
        {
            LookForFloorRoof(scn, dt);
        }
    }

    xEntEndCollide(this, scn, dt);

    tmr_alive += dt;
    tmr_curstate += dt;
    tmr_coldelay = MAX(-1.0f, tmr_coldelay - dt);

    flg_stat &= ~0x60000000;
    SendEvent(ZPRJ_EVNT_NEWTIME, dt, NULL, NULL);

    if (plyr_colrec->flags & 1)
    {
        SendEvent(ZPRJ_EVNT_HITPLYR, dt, plyr_colrec, NULL);
    }

    catf->logix(this, dt, scn, frame);

    if (frame->mode != 0)
    {
        SyncFrame(0);
    }

    OrientToPath(frame, flg_prop);

    if (pflags & 0x2)
    {
        xEntApplyPhysics(this, scn, dt);
    }

    if ((pflags & 0x1) && catf->calcmove != NULL)
    {
        catf->calcmove(this, dt, scn, frame);

        if (frame->mode != 0)
        {
            SyncFrame(0);
        }
    }

    if (flg_prop & 0x2)
    {
        xFFXApply(this, scn, dt);
    }

    SendEvent(ZPRJ_EVNT_ENDTIME, dt, NULL, NULL);

    frame = NULL;
    collis = NULL;
    plyr_colrec = NULL;

    if (tmr_alive > tym_maxlife && state != ZPRJ_STATE_DEAD)
    {
        SendEvent(ZPRJ_EVNT_SELFDESTRUCT, dt, NULL, NULL);
        StateSet(ZPRJ_STATE_DEAD);
    }

    return 1;
}

S32 zEntProjectile::DoColCheck(F32 dt)
{
    st_ZPRJ_CATEGORY_FUNCS* catf;
    S32 doit = 0;
    static en_ZPRJ_DOCOL_HEURISTICS use_heur = ZPRJ_DOCOL_ALWAYS;

    if (tmr_coldelay > 0.0f)
    {
        return 0;
    }

    catf = &g_catFuncs[cat_proj];

    if (ownerDoCollide != NULL)
    {
        use_heur = ZPRJ_DOCOL_CUSTOWNER;
    }
    else if (catf->collide != NULL)
    {
        use_heur = ZPRJ_DOCOL_CUSTCAT;
    }

    switch (use_heur)
    {
    case ZPRJ_DOCOL_CUSTOWNER:
        doit = ownerDoCollide(this, globals.sceneCur, dt);
        break;
    case ZPRJ_DOCOL_CUSTCAT:
        doit = catf->collide(this, globals.sceneCur, dt);
        break;
    case ZPRJ_DOCOL_ALWAYS:
        doit = 1;
        break;
    case ZPRJ_DOCOL_RANDPCT:
        doit = xUtil_yesno(pct_docol);
        break;
    case ZPRJ_DOCOL_SINE:
        doit = fabsf(cos(tmr_alive)) < pct_docol;
        break;
    case ZPRJ_DOCOL_VELBOUND:
        doit = dt * sqrtf(xVec3Length2(&frame->vel)) > 0.49f * bound.sph.r;
        break;
    case ZPRJ_DOCOL_TOGGLE:
        if (pct_docol < 0.0f)
        {
            pct_docol = 1.0f;
        }
        else
        {
            pct_docol = -1.0f;
            doit = 1;
        }
        break;
    }

    return doit;
}

void ZPRJ_wrap_bupdate(_xEnt* prjent, xVec3* pos)
{
    zEntProjectile* prj = (zEntProjectile*)prjent;

    xVec3Copy(&prj->bound.sph.center, pos);
    prj->bound.sph.r = 0.5f;
    xBoundUpdate(&prj->bound);
}

S32 zEntProjectile::ZPRJ_render()
{
    xVec3 vec_tmp = {};
    F32 dst;
    F32 dst_plyr;

    if (!(flg_stat & 0x20000))
    {
        return 1;
    }

    if (state <= ZPRJ_STATE_STANDBY)
    {
        return 1;
    }

    ZPRJ_catFuncs(this)->render(this);

    if (flg_prop & 0x8)
    {
        xShadowRender(this);
    }
    else if (xEntIsVisible(this) && (flg_prop & 0x4))
    {
        xVec3Sub(&vec_tmp, &globals.camera.hi_cam.mat.pos, (xVec3*)&model->Mat->pos);
        dst = xVec3Length2(&vec_tmp);
        xVec3Sub(&vec_tmp, (xVec3*)&globals.player.ent.model->Mat->pos, (xVec3*)&model->Mat->pos);
        dst_plyr = xVec3Length2(&vec_tmp);
        dst = (dst < dst_plyr) ? dst : dst_plyr;

        xVec3Copy(&ent_shadow->pos, &bound.sph.center);
        ent_shadow->pos.y += 2.0f * bound.sph.r;

        if (dst < 100.0f)
        {
            xShadowRender(this);
        }
        else
        {
            iShadowCollide(&ishadow, this, 0.75f, 1.0f);
            iShadowAdd(&ishadow, 0x50);
        }
    }

    return 1;
}

S32 zEntProjectile::LookForFloorRoof(_xScene*, F32)
{
    S32 i;
    _xCollis* best = NULL;
    _xCollis* cur_rec;
    _xCollis* ceil = &collis->colls[0];
    _xCollis* flor = &collis->colls[1];

    if (!(ceil->flags & 1))
    {
        for (i = collis->stat_sidx; i < collis->stat_eidx; i++)
        {
            cur_rec = &collis->colls[i];
            if (!(cur_rec->hdng.y > -0.9f))
            {
                if (best == NULL)
                {
                    best = cur_rec;
                }
                else if (!(cur_rec->hdng.y > best->hdng.y))
                {
                    best = cur_rec;
                }
            }
        }

        for (i = collis->env_sidx; i < collis->env_eidx; i++)
        {
            cur_rec = &collis->colls[i];
            if (!(cur_rec->hdng.y > -0.9f))
            {
                if (best == NULL)
                {
                    best = cur_rec;
                }
                else if (!(cur_rec->hdng.y > best->hdng.y))
                {
                    best = cur_rec;
                }
            }
        }

        if (best != NULL)
        {
            memcpy(ceil, best, sizeof(_xCollis));
            cur_rec->flags |= 0x6;
        }
    }

    if (!(flor->flags & 1))
    {
        best = NULL;

        for (i = collis->stat_sidx; i < collis->stat_eidx; i++)
        {
            cur_rec = &collis->colls[i];
            if (!(cur_rec->hdng.y < 0.9f))
            {
                if (best == NULL)
                {
                    best = cur_rec;
                }
                else if (!(cur_rec->hdng.y > best->hdng.y))
                {
                    best = cur_rec;
                }
            }
        }

        for (i = collis->env_sidx; i < collis->env_eidx; i++)
        {
            cur_rec = &collis->colls[i];
            if (!(cur_rec->hdng.y < 0.9f))
            {
                if (best == NULL)
                {
                    best = cur_rec;
                }
                else if (!(cur_rec->hdng.y > best->hdng.y))
                {
                    best = cur_rec;
                }
            }
        }

        if (best != NULL)
        {
            memcpy(flor, best, sizeof(_xCollis));
            cur_rec->flags |= 0xA;
        }
    }

    return (ceil->flags & 1) || (flor->flags & 1);
}

S32 zEntProjectile_gen_attach(zEntProjectile* prj)
{
    st_ZPRJ_CATDATA* catd = &prj->cat_data;

    switch (prj->cat_proj)
    {
    case ZPRJ_CAT_UNKNOWN:
        break;
    case ZPRJ_CAT_ARROW:
        catd->arrow.amStuck = 0;
        break;
    case ZPRJ_CAT_GRAVBALL:
    case ZPRJ_CAT_BOUNCEBALL:
        catd->gravball.acc_gravity = 0.0f;
        break;
    }

    return 1;
}

S32 zEntProjectile_gen_logix(zEntProjectile* prj, F32 dt, zScene*, _xEntFrame* frame)
{
    xVec3 vec_home = {};

    if (prj->tmr_alive > prj->tym_maxlife)
    {
        prj->SendEvent(ZPRJ_EVNT_SELFDESTRUCT, dt, NULL, NULL);
        prj->StateSet(ZPRJ_STATE_DEAD);
    }
    else
    {
        xVec3Sub(&vec_home, &frame->mat.pos, &prj->pos_home);
        if (xVec3Length2(&vec_home) > prj->ds2_maxlife)
        {
            prj->StateSet(ZPRJ_STATE_DEAD);
        }
    }

    return 1;
}

S32 zEntProjectile_gen_render(zEntProjectile* prj)
{
    if (xEntIsVisible(prj))
    {
        xEntRender(prj);
    }

    return 1;
}

static S32 ZPRJ_arrow_attach(zEntProjectile* prj)
{
    prj->cat_data.idx_cdat = ZPRJ_CDAT_ARROW;
    return 1;
}

static S32 ZPRJ_arrow_logix(zEntProjectile* prj, F32 dt, zScene*, _xEntFrame*)
{
    st_ZPRJ_CAT_ARROW* arrow = &prj->cat_data.arrow;

    if (arrow->amStuck)
    {
        prj->SendEvent(ZPRJ_EVNT_ATREST, dt, NULL, NULL);
    }
    else
    {
        prj->SendEvent(ZPRJ_EVNT_INFLIGHT, dt);
    }

    if (prj->tmr_alive > prj->tym_maxlife)
    {
        prj->SendEvent(ZPRJ_EVNT_SELFDESTRUCT, dt, NULL, NULL);
        prj->StateSet(ZPRJ_STATE_DEAD);
        return 1;
    }

    if (prj->plyr_colrec->flags & 1)
    {
        if (prj->flg_prop & 0x10)
        {
            zEntPlayer_Damage(prj, 1);
        }

        if (prj->flg_categ & 0x1000000)
        {
            if (!arrow->amStuck)
            {
                arrow->amStuck = 1;
                arrow->ent_stuck = &globals.player.ent;
                xVec3Sub(&arrow->pos_stuck, (xVec3*)&arrow->ent_stuck->model->Mat->pos,
                         (xVec3*)&prj->model->Mat->pos);
                xVec3Init(&prj->frame->dvel, 0.0f, 0.0f, 0.0f);
                xVec3Init(&prj->frame->vel, 0.0f, 0.0f, 0.0f);
                prj->frame->mode |= 0xC;
                prj->OrientSet(0x10000000);
            }
        }
        else
        {
            prj->SendEvent(ZPRJ_EVNT_DETONATE, dt, NULL, NULL);
            prj->StateSet(ZPRJ_STATE_DEAD);
        }
    }
    else if (prj->collis->colls[0].flags & 1)
    {
        prj->SendEvent(ZPRJ_EVNT_DETONATE, dt, NULL, NULL);
        prj->StateSet(ZPRJ_STATE_DEAD);
    }
    else if (prj->collis->colls[1].flags & 1)
    {
        if (prj->flg_categ & 0x2000000)
        {
            arrow->amStuck = 2;
            prj->pflags |= 0x4;
            prj->frame->vel.x = 0.0f;
            prj->frame->vel.z = 0.0f;
            prj->frame->mode |= 0x4;
        }
        else
        {
            prj->SendEvent(ZPRJ_EVNT_DETONATE, dt, NULL, NULL);
            prj->StateSet(ZPRJ_STATE_DEAD);
        }
    }
    else if (prj->collis->env_sidx < prj->collis->env_eidx || prj->collis->npc_sidx < prj->collis->npc_eidx ||
             prj->collis->stat_sidx < prj->collis->stat_eidx || prj->collis->dyn_sidx < prj->collis->dyn_eidx)
    {
        if (prj->flg_categ & 0x2000000)
        {
            arrow->amStuck = 2;
            prj->pflags |= 0x4;
            prj->frame->vel.x = 0.0f;
            prj->frame->vel.z = 0.0f;
            prj->frame->mode |= 0x4;
        }
        else if (prj->flg_categ & 0x1000000)
        {
            arrow->amStuck = 1;
            prj->pflags |= 0x4;
            xVec3Init(&prj->frame->dvel, 0.0f, 0.0f, 0.0f);
            xVec3Init(&prj->frame->vel, 0.0f, 0.0f, 0.0f);
            prj->frame->mode |= 0xC;
            prj->OrientSet(0);
        }
        else
        {
            prj->SendEvent(ZPRJ_EVNT_DETONATE, dt, NULL, NULL);
            prj->StateSet(ZPRJ_STATE_DEAD);
        }
    }

    return 1;
}

static S32 ZPRJ_grav_attach(zEntProjectile* prj)
{
    prj->cat_data.idx_cdat = ZPRJ_CDAT_GRAVBALL;
    prj->cat_data.gravball.acc_gravity = 1.5f;
    return 1;
}

static S32 ZPRJ_grav_logix(zEntProjectile* prj, F32 dt, zScene*, _xEntFrame*)
{
    prj->SendEvent(ZPRJ_EVNT_INFLIGHT, dt);

    if (prj->tmr_alive > prj->tym_maxlife)
    {
        prj->SendEvent(ZPRJ_EVNT_SELFDESTRUCT, dt, NULL, NULL);
        prj->StateSet(ZPRJ_STATE_DEAD);
        return 1;
    }

    if ((prj->flg_prop & 0x10) && (prj->plyr_colrec->flags & 1))
    {
        zEntPlayer_Damage(prj, 1);
    }

    if ((prj->plyr_colrec->flags & 1) || (prj->collis->colls[0].flags & 1) || (prj->collis->colls[1].flags & 1) ||
        prj->collis->env_sidx < prj->collis->env_eidx || prj->collis->npc_sidx < prj->collis->npc_eidx ||
        prj->collis->stat_sidx < prj->collis->stat_eidx || prj->collis->dyn_sidx < prj->collis->dyn_eidx)
    {
        prj->SendEvent(ZPRJ_EVNT_DETONATE, dt, NULL, NULL);
        prj->StateSet(ZPRJ_STATE_DEAD);
    }

    return 1;
}

static S32 ZPRJ_grav_calcmove(zEntProjectile* prj, F32 dt, zScene*, _xEntFrame* frame)
{
    st_ZPRJ_CAT_GRAVBALL* grav = &prj->cat_data.gravball;

    frame->dvel.x = 0.0f;
    if (prj->flg_categ & 0x2)
    {
        frame->dvel.y -= dt * grav->acc_gravity;
    }
    else
    {
        frame->dvel.y = -(dt * grav->acc_gravity);
        frame->dvel.y = -(dt * grav->acc_gravity);
    }
    frame->dvel.z = 0.0f;
    frame->mode |= 0x8;

    return 1;
}

static S32 ZPRJ_bounce_attach(zEntProjectile* prj)
{
    st_ZPRJ_CAT_BOUNCEBALL* bounce = &prj->cat_data.bounce;

    prj->cat_data.idx_cdat = ZPRJ_CDAT_BOUNCE;
    bounce->tym_atrest = 3.0f;
    bounce->acc_gravity = 1.5f;
    bounce->cnt_maxBounce = 5;
    bounce->fac_elastic = 0.6f;
    bounce->pct_randKick = 0.0f;

    return 1;
}

static S32 ZPRJ_bounce_logix(zEntProjectile* prj, F32 dt, zScene*, _xEntFrame* frame)
{
    F32 elastic;
    xVec3 vec_accum = {};
    st_ZPRJ_CAT_BOUNCEBALL* bounce = &prj->cat_data.bounce;

    if (bounce->atRest == 1)
    {
        prj->SendEvent(ZPRJ_EVNT_ATREST, dt, NULL, NULL);
    }
    else if (bounce->atRest == 2)
    {
        prj->SendEvent(ZPRJ_EVNT_INFLIGHT, dt);
    }
    else
    {
        prj->SendEvent(ZPRJ_EVNT_INFLIGHT, dt);
    }

    if (prj->tmr_alive > prj->tym_maxlife)
    {
        prj->SendEvent(ZPRJ_EVNT_SELFDESTRUCT, dt, NULL, NULL);
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
        prj->SendEvent(ZPRJ_EVNT_DETONATE, dt, NULL, NULL);
        prj->StateSet(ZPRJ_STATE_DEAD);
    }
    else if ((prj->plyr_colrec->flags & 1) || (prj->collis->colls[0].flags & 1) ||
             (prj->collis->colls[1].flags & 1) || prj->collis->env_sidx < prj->collis->env_eidx ||
             prj->collis->npc_sidx < prj->collis->npc_eidx || prj->collis->stat_sidx < prj->collis->stat_eidx ||
             prj->collis->dyn_sidx < prj->collis->dyn_eidx)
    {
        if ((prj->flg_categ & 0x2000000) && bounce->skip_next_bounce &&
            bounce->cnt_curBounce < bounce->cnt_maxBounce)
        {
            bounce->skip_next_bounce = 0;
            xVec3Copy(&prj->frame->vel, &prj->frame->oldvel);
            prj->frame->mode |= 0x4;
        }
        else if ((prj->flg_categ & 0x2000000) && bounce->cnt_curBounce < bounce->cnt_maxBounce)
        {
            bounce->cnt_curBounce++;
            bounce->skip_next_bounce = 1;
            bounce->atRest = 2;

            elastic = bounce->fac_elastic;
            if (prj->flg_categ & 0x2)
            {
                elastic *= 0.5f;
            }

            zEntProjectile_colAccum(prj, &vec_accum);
            zEntProjectile_softReflect(&prj->frame->vel, &prj->frame->oldvel, &vec_accum, elastic);
            prj->frame->mode |= 0x4;

            prj->SendEvent(ZPRJ_EVNT_BOUNCE, dt, NULL, NULL);

            if (xVec3Length2(&frame->vel) < 0.0625f)
            {
                bounce->cnt_curBounce = bounce->cnt_maxBounce;
            }
        }
        else if (prj->flg_categ & 0x1000000)
        {
            if (bounce->atRest != 1)
            {
                bounce->atRest = 1;
                prj->SendEvent(ZPRJ_EVNT_STARTRESTING, dt, NULL, NULL);
                prj->AssignModel(ZPRJ_MDLIDX_POSTHIT);
            }
        }
        else if (bounce->atRest != 2)
        {
            prj->SendEvent(ZPRJ_EVNT_DETONATE, dt, NULL, NULL);
            prj->StateSet(ZPRJ_STATE_DEAD);
        }
    }

    return 1;
}

static S32 ZPRJ_bounce_calcmove(zEntProjectile* prj, F32 dt, zScene*, _xEntFrame* frame)
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
        xMat4x3RMul3x3((xMat4x3*)frame, &prj->common.mat_flex, (xMat4x3*)frame);
        frame->mode |= 0x100;
    }

    return 1;
}

F32 zEntProjectile_colAccum(zEntProjectile* prj, xVec3* dir_away)
{
    xVec3 vec_accum = {};
    S32 hit_something = 0;
    F32 mag_hit = 0.0f;
    _xCollis* colrec;
    S32 i;

    if (prj->collis == NULL)
    {
        dir_away->x = dir_away->y = dir_away->z = mag_hit;
        return mag_hit;
    }

    if (prj->collis->colls[0].flags & 1)
    {
        hit_something++;
        xVec3Add(&vec_accum, &vec_accum, &prj->collis->colls[0].depen);
    }
    else if (prj->collis->colls[1].flags & 1)
    {
        hit_something++;
        xVec3Add(&vec_accum, &vec_accum, &prj->collis->colls[1].depen);
    }

    for (i = prj->collis->npc_sidx; i < prj->collis->npc_eidx; i++)
    {
        colrec = &prj->collis->colls[i];
        if ((colrec->flags & 1) && !(colrec->flags & 2))
        {
            hit_something++;
            xVec3Add(&vec_accum, &vec_accum, &colrec->depen);
        }
    }

    for (i = prj->collis->env_sidx; i < prj->collis->env_eidx; i++)
    {
        colrec = &prj->collis->colls[i];
        hit_something++;
        if ((colrec->flags & 1) && !(colrec->flags & 2))
        {
            xVec3Add(&vec_accum, &vec_accum, &colrec->depen);
        }
    }

    for (i = prj->collis->stat_sidx; i < prj->collis->stat_eidx; i++)
    {
        colrec = &prj->collis->colls[i];
        if ((colrec->flags & 1) && !(colrec->flags & 2))
        {
            hit_something++;
            xVec3Add(&vec_accum, &vec_accum, &colrec->depen);
        }
    }

    for (i = prj->collis->dyn_sidx; i <= prj->collis->dyn_eidx; i++)
    {
        colrec = &prj->collis->colls[i];
        if ((colrec->flags & 1) && !(colrec->flags & 2))
        {
            hit_something++;
            xVec3Add(&vec_accum, &vec_accum, &colrec->depen);
        }
    }

    if (fabsf(vec_accum.x) < 0.00001f)
    {
        vec_accum.x = 0.0f;
    }

    if (fabsf(vec_accum.y) < 0.00001f)
    {
        vec_accum.y = 0.0f;
    }

    if (fabsf(vec_accum.z) < 0.00001f)
    {
        vec_accum.z = 0.0f;
    }

    if (hit_something)
    {
        mag_hit = xVec3Normalize(dir_away, &vec_accum);
    }
    else
    {
        xVec3SMul(dir_away, dir_away, 0.0f);
    }

    return mag_hit;
}

void zEntProjectile_softReflect(xVec3* dir_new, xVec3* dir_cur, xVec3* oppose, F32 scaleMag)
{
    if (scaleMag != 0.0f && scaleMag != 1.0f)
    {
        xVec3SMul(dir_new, dir_cur, scaleMag);
    }
    else
    {
        xVec3Copy(dir_new, dir_cur);
    }

    if (oppose->x < 0.0f && dir_cur->x > 0.0f)
    {
        dir_new->x *= -1.0f;
    }
    else if (oppose->x > 0.0f && dir_cur->x < 0.0f)
    {
        dir_new->x *= -1.0f;
    }

    if (oppose->y < 0.0f && dir_cur->y > 0.0f)
    {
        dir_new->y *= -1.0f;
    }
    else if (oppose->y > 0.0f && dir_cur->y < 0.0f)
    {
        dir_new->y *= -1.0f;
    }

    if (oppose->z < 0.0f && dir_cur->z > 0.0f)
    {
        dir_new->z *= -1.0f;
    }
    else if (oppose->z > 0.0f && dir_cur->z < 0.0f)
    {
        dir_new->z *= -1.0f;
    }
}

S32 zEntProjectile_gen_aimer(zEntProjectile* prj, ST_CAT_AIMDATA* info, S32 apply)
{
    st_ZPRJ_SOLV_METHOD_A* metha;
    st_CAT_AIMGRAV* grav;
    st_CAT_AIMARROW* arrow;
    S32 didit;

    switch (info->idx_caim)
    {
    case ZPRJ_CAIM_ARROW:
        arrow = &info->arrow;
        xVec3Sub(&info->dir_base, &arrow->pos_tgt, &arrow->pos_src);
        xVec3Normalize(&info->dir_base, &info->dir_base);

        if (apply)
        {
            xVec3Copy(&prj->frame->mat.pos, &arrow->pos_src);
            xVec3SMul(&prj->frame->vel, &info->dir_base, info->spd_base);
            prj->frame->mode |= 0x5;
            prj->SyncFrame(1);
        }

        didit = 1;
        break;
    case ZPRJ_CAIM_GRAVBALL:
    {
        st_ZPRJ_AUTOCALC_INFO autinf = {};

        grav = &info->grav;
        metha = &autinf.metha;
        didit = 0;

        xVec3Sub(&info->dir_base, &grav->pos_tgt, &grav->pos_src);
        metha->spd_muzzle = info->spd_base;
        xVec3Copy(&metha->dir_launch, &info->dir_base);
        xVec3Copy(&metha->pos_launch, &grav->pos_src);
        xVec3Copy(&metha->pos_target, &grav->pos_tgt);
        metha->fac_arcness = grav->fac_arcness;
        metha->acc_gravity = grav->acc_gravity;
        xVec3SMul(&metha->vel_launch, &metha->dir_launch, metha->spd_muzzle);

        switch (autinf.method)
        {
        case ZPRJ_ATGTSOLV_METHA:
        {
            xVec3 vec_tgt = {};

            xVec3Sub(&vec_tgt, &metha->pos_target, &metha->pos_launch);
            didit = ZPRJ_solv_velvec(vec_tgt, metha->acc_gravity, metha->fac_arcness, metha->spd_muzzle,
                                     metha->dst_range, &metha->vel_launch) +
                    1;
            break;
        }
        case ZPRJ_ATGTSOLV_METHB:
            didit = 0;
            break;
        case ZPRJ_ATGTSOLV_METHC:
            didit = 0;
            break;
        }

        if (didit)
        {
            xVec3Copy(&info->dir_base, &metha->vel_launch);
            info->spd_base = xVec3Normalize(&info->dir_base, &info->dir_base);
        }

        if (didit && apply)
        {
            prj->cat_data.gravball.acc_gravity = grav->acc_gravity;
            xVec3Copy(&prj->frame->mat.pos, &grav->pos_src);
            xVec3SMul(&prj->frame->vel, &info->dir_base, info->spd_base);
            prj->frame->mode |= 0x5;
            prj->SyncFrame(1);
        }
        break;
    }
    default:
        didit = 0;
        break;
    }

    return didit;
}

static S32 ZPRJ_solv_velvec(xVec3 vec_tgtOrig, F32 grav, F32 arc_factor, F32 spd_limit, F32 dst_range,
                            xVec3* vec_vel)
{
    xVec3 vec_tgt = {};
    F32 tym_limit = -1.0f;
    F32 tym_ariv;
    S32 limit_tally = 0;
    U32 solcnt;
    F32 sol[2] = {};
    F32 c;
    F32 b;
    xVec3 vec_tmp = {};
    F32 dst_tgt;

    vec_tgt = vec_tgtOrig;
    dst_tgt = sqrtf(vec_tgtOrig.x * vec_tgtOrig.x + vec_tgtOrig.z * vec_tgtOrig.z);

    if (dst_range > 0.0f && dst_tgt > dst_range)
    {
        limit_tally = 1;
        xVec3Copy(&vec_tgt, &vec_tgtOrig);
        vec_tgt.y = 0.0f;
        xVec3Normalize(&vec_tgt, &vec_tgt);
        xVec3SMul(&vec_tgt, &vec_tgt, dst_range);
        vec_tgt.y = vec_tgtOrig.y;
    }

    if (dst_range > 0.0f)
    {
        tym_limit = dst_range / spd_limit;
    }

    c = -0.5f * grav;
    b = sqrtf(2.0f * grav * (1.5f * arc_factor));

    xVec3Init(vec_vel, 0.0f, 0.0f, 0.0f);

    solcnt = xMathSolveQuadratic(c, b, -vec_tgt.y, &sol[0], &sol[1]);
    if (solcnt == 2)
    {
        tym_ariv = MAX(sol[0], sol[1]);
    }
    else if (solcnt == 1)
    {
        tym_ariv = sol[0];
    }
    else
    {
        tym_ariv = 1.0f;
        limit_tally++;
    }

    if (tym_ariv <= 0.0f)
    {
        limit_tally++;
        if (tym_limit <= 0.0f)
        {
            tym_ariv = 1.0f;
        }
        else
        {
            tym_ariv = tym_limit;
        }
    }

    vec_tmp.x = vec_tgt.x / tym_ariv;
    vec_tmp.y = 0.0f;
    vec_tmp.z = vec_tgt.z / tym_ariv;

    if (vec_tmp.x * vec_tmp.x + vec_tmp.z * vec_tmp.z > spd_limit * spd_limit)
    {
        limit_tally++;
        xVec3Normalize(&vec_tmp, &vec_tmp);
        xVec3SMul(&vec_tmp, &vec_tmp, spd_limit);
    }

    vec_tmp.y = b;
    xVec3Copy(vec_vel, &vec_tmp);

    return limit_tally;
}

void zEntProjectile::DoEffects(en_PROJECTILE_EVENT event, F32 dt)
{
    en_ZPRJ_FX_TYPE fxtyp = (en_ZPRJ_FX_TYPE)ptype->src_ptype->efxType;

    switch (event)
    {
    case ZPRJ_EVNT_HITPLYR:
        break;
    case ZPRJ_EVNT_LAUNCH:
        break;
    case ZPRJ_EVNT_INFLIGHT:
        PEFX_flight(fxtyp, dt);
        break;
    case ZPRJ_EVNT_ATREST:
        break;
    case ZPRJ_EVNT_HITSTUFF:
        break;
    case ZPRJ_EVNT_DETONATE:
        break;
    case ZPRJ_EVNT_SELFDESTRUCT:
        break;
    case ZPRJ_EVNT_DEATH:
        break;
    }
}

void zEntProjectile::PEFX_flight(en_ZPRJ_FX_TYPE fxtyp, F32 dt)
{
    _zParEmitterCustomSettings info;
    _zParEmitter* emit = NULL;

    tmr_emit -= dt;
    if (tmr_emit > 0.0f)
    {
        return;
    }

    tmr_emit = 0.1f;
    info.custom_flags = 0x20;
    xVec3Copy(&info.pos, (xVec3*)&model->Mat->pos);

    switch (fxtyp)
    {
    case ZPRJ_FXTYPE_SMOKE:
        emit = g_paremit[6];
        break;
    case ZPRJ_FXTYPE_WOOD:
        emit = g_paremit[7];
        break;
    case ZPRJ_FXTYPE_TAR:
        emit = g_paremit[8];
        tmr_emit = 0.0f;
        break;
    case ZPRJ_FXTYPE_ICE:
        emit = g_paremit[9];
        break;
    case ZPRJ_FXTYPE_FIREBALL:
        emit = g_paremit[10];
        tmr_emit = 0.0f;
        break;
    case ZPRJ_FXTYPE_NODAMAGE_FIREBALL:
        emit = g_paremit[11];
        tmr_emit = 0.0f;
        break;
    case ZPRJ_FXTYPE_WITCHDOCTOR:
        emit = g_paremit[12];
        tmr_emit = 0.0f;
        break;
    case ZPRJ_FXTYPE_MASTERMIND_RED:
    case ZPRJ_FXTYPE_MASTERMIND_GREEN:
    case ZPRJ_FXTYPE_MASTERMIND_BLACK:
        emit = g_paremit[13];
        tmr_emit = 0.0f;
        info.pos.y += 0.35f;
        break;
    case ZPRJ_FXTYPE_BLACKKNIGHT:
    case ZPRJ_FXTYPE_REDBEARD:
        emit = NULL;
        break;
    case ZPRJ_FXTYPE_NONE:
    default:
        break;
    }

    if (emit != NULL)
    {
        zParEmitterEmitCustom(emit, dt, &info);
    }
}
