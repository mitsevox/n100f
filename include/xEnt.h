#ifndef XENT_H
#define XENT_H

#include "xBase.h"
#include "xMath3.h"

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
struct _xEntFrame;
struct _xEntCollis;
struct _xEntShadow;
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

void xEntUpdate(_xEnt* ent, _xScene* sc, F32 dt);
void xEntReset(_xEnt* ent);

inline void xEntShow(_xEnt* ent)
{
    ent->flags |= 0x1;
}

inline void xEntHide(_xEnt* ent)
{
    ent->flags &= ~0x1;
}

#endif
