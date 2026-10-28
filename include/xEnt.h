#ifndef XENT_H
#define XENT_H

#include "xBase.h"
#include "xMath3.h"
#include "xModel.h"

struct p2EntAsset : p2BaseAsset
{
    U8 flags;
    U8 subtype;
    U8 pflags;
    U8 moreFlags;
    U32 surfaceID;
    xVec3 ang;
    xVec3 pos;
    xVec3 scale;
    F32 redMult;
    F32 greenMult;
    F32 blueMult;
    F32 seeThru;
    F32 seeThruSpeed;
    U32 modelInfoID;
    U32 animListID;
};

struct _iCollis
{
    S32 unknown;
};

struct _xCollis
{
    U32 flags;
    U32 oid;
    void* optr;
    F32 dist;
    xVec3 norm;
    xVec3 tohit;
    xVec3 depen;
    xVec3 hdng;
    _iCollis ixtra;
};

struct _xBox
{
    xVec3 upper;
    xVec3 lower;
};

struct _xBBox
{
    xVec3 center;
    _xBox box;
};

struct _xCylinder
{
    xVec3 center;
    F32 r;
    F32 h;
};

struct xQCData
{
    U32 xmask;
    U32 ymask;
    U32 zmask;
};

struct _xBound
{
    U8 type;
    U8 pad[3];
    union
    {
        xSphere sph;
        _xBBox box;
        _xCylinder cyl;
    };
    xMat4x3* mat;
    xQCData qcd;
};

struct iShadowCache
{
    xVec3 pos;
    xVec3 right;
    xVec3 corner[4];
};

struct _xScene;

struct _xEnt;

struct _xEntDrive
{
    U32 flags;
    F32 otm;
    F32 otmr;
    F32 os;
    F32 tm;
    F32 tmr;
    F32 s;
    _xEnt* odriver;
    _xEnt* driver;
    _xEnt* driven;
    xVec3 op;
    xVec3 p;
    xVec3 q;
    F32 yaw;
};

struct _xEntFrame
{
    xMat4x3 mat;
    xMat4x3 dmat;
    xVec3 oldpos;
    xVec3 oldvel;
    xRot oldrot;
    xRot drot;
    xRot rot;
    xVec3 dpos;
    xVec3 dvel;
    xVec3 vel;
    U32 mode;
};

struct _xEntCollis
{
    U8 chk;
    U8 pen;
    U8 env_sidx;
    U8 env_eidx;
    U8 npc_sidx;
    U8 npc_eidx;
    U8 dyn_sidx;
    U8 dyn_eidx;
    U8 stat_sidx;
    U8 stat_eidx;
    U8 trig_idx;
    U8 idx;
    _xCollis colls[18];
    void (*post)(_xEnt*, _xScene*, F32, _xEntCollis*);
    U32 (*depenq)(_xEnt*, _xEnt*, _xScene*, F32, _xCollis*);
    U8 trigs_cur[8];
    U8 trigs_prev[8];
    U8 trigs_entered[8];
    U8 trigs_exited[8];
};

struct _xEntShadow
{
    xVec3 pos;
    xVec3 vec;
};

struct xModelInstance;
struct xSurface;
struct xFFX;

struct _xEnt : xBase
{
    p2EntAsset* asset;
    U8 flags;
    U8 subType;
    U8 pflags;
    U8 moreFlags;
    U8 pad0[3];
    U8 num_ffx;
    U16 idx;
    U16 num_updates;
    F32 material_alpha;
    F32 fade_tmr;
    U8 collType;
    U8 collLev;
    U8 chkby;
    U8 penby;
    xModelInstance* model;
    void (*update)(_xEnt*, _xScene*, F32);
    void (*bupdate)(_xEnt*, xVec3*);
    void (*move)(_xEnt*, _xScene*, F32, _xEntFrame*);
    void (*render)(_xEnt*);
    _xEntFrame* frame;
    _xEntCollis* collis;
    _xBound bound;
    F32 updateTimer;
    F32 updateAcc;
    void (*updateTimerFunc)(_xEnt*);
    void (*transl)(_xEnt*, xVec3*);
    iShadowCache ishadow;
    xSurface* surf;
    xFFX* ffx;
    _xEnt* driver;
    _xEntShadow* ent_shadow;
};

void xEntCollide(_xEnt* ent, _xScene* sc, F32 dt);
void xEntApplyPhysics(_xEnt* ent, _xScene* sc, F32 dt);
void xEntMove(_xEnt* ent, _xScene* sc, F32 dt);
void xEntUpdate(_xEnt* ent, _xScene* sc, F32 dt);
void xEntReset(_xEnt* ent);
U32 xEntIsVisible(_xEnt* ent);
void xEntDefaultBoundUpdate(_xEnt* ent, xVec3* pos);
void xEntSetMaterialMul(_xEnt* ent, F32 r, F32 g, F32 b);
void xEntBeginUpdate(_xEnt* ent, _xScene* sc, F32 dt);
void xEntRender(_xEnt* ent);
void xEntMotionToMatrix(_xEnt* ent, _xEntFrame* frame);
void xEntBeginCollide(_xEnt* ent, _xScene* sc, F32 dt);
void xEntEndCollide(_xEnt* ent, _xScene* sc, F32 dt);
void xEntEndUpdate(_xEnt* ent, _xScene* sc, F32 dt);

inline void xEntShow(_xEnt* ent)
{
    ent->flags |= 0x1;
}

inline xVec3* xEntGetPos(const _xEnt* ent)
{
    return (xVec3*)&ent->model->Mat->pos;
}

inline void xEntHide(_xEnt* ent)
{
    ent->flags &= ~0x1;
}

#endif
