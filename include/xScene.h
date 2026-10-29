#ifndef XSCENE_H
#define XSCENE_H

#include "types.h"
#include "xBase.h"
#include "xMemMgr.h"

struct _xEnt;
struct _xEnv;
struct _xRay3;
struct _xCollis;

// Partial: members up to mempool.
struct _xScene
{
    U16 flags;
    U16 num_ents;
    U16 num_trigs;
    U16 num_stats;
    U16 num_dyns;
    U16 num_npcs;
    U16 num_act_ents;
    U16 num_nact_ents;
    F32 gravity;
    F32 drag;
    F32 friction;
    U16 num_ents_allocd;
    U16 num_trigs_allocd;
    U16 num_stats_allocd;
    U16 num_dyns_allocd;
    U16 num_npcs_allocd;
    _xEnt** trigs;
    _xEnt** stats;
    _xEnt** dyns;
    _xEnt** npcs;
    _xEnt** act_ents;
    _xEnt** nact_ents;
    _xEnv* env;
    xMemPool mempool;
};

extern _xScene* g_xSceneCur;

xBase* xSceneResolvID(_xScene* sc, U32 id);
char* xSceneID2Name(_xScene* sc, U32 id);
void xSceneForAllNPCs(_xScene* sc, _xEnt* (*func)(_xEnt*, _xScene*, void*), void* data);
void xSceneForAllDynamics(_xScene* sc, _xEnt* (*func)(_xEnt*, _xScene*, void*), void* data);
void xSceneForAllStatics(_xScene* sc, _xEnt* (*func)(_xEnt*, _xScene*, void*), void* data);
void xSceneForAllTriggers(_xScene* sc, _xEnt* (*func)(_xEnt*, _xScene*, void*), void* data);
void xRayHitsScene(_xScene* sc, _xRay3* r, _xCollis* coll);

#endif
