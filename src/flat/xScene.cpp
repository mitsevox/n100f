#include "xScene.h"
#include "xAnim.h"
#include "xEnt.h"
#include "xModel.h"

struct _xEnv;
struct xSerial;

void xAnimTempTransitionInit(U32 count);
void xAnimPoolInit(xMemPool* pool, U32 count, U32 singles, U32 blendFlags, U32 effectMax);
void xCollideInit(_xScene* sc);
void xCollideFastInit(_xScene* sc);
void xEnvSetup(_xEnv* env);

// Allocates the scene's entity lists and sets up the animation, model and collision pools.
// The model pools grow with the scene's NPC, dynamic and static counts.
void xSceneInit(_xScene* sc, U16 num_trigs, U16 num_stats, U16 num_dyns, U16 num_npcs)
{
    sc->flags = 0;
    sc->num_trigs_allocd = num_trigs;
    sc->trigs = (_xEnt**)xMemAlloc(gActiveHeap, num_trigs * sizeof(_xEnt*));
    sc->num_stats_allocd = num_stats;
    sc->stats = (_xEnt**)xMemAlloc(gActiveHeap, num_stats * sizeof(_xEnt*));
    sc->num_dyns_allocd = num_dyns;
    sc->dyns = (_xEnt**)xMemAlloc(gActiveHeap, num_dyns * sizeof(_xEnt*));
    sc->num_npcs_allocd = num_npcs;
    sc->npcs = (_xEnt**)xMemAlloc(gActiveHeap, num_npcs * sizeof(_xEnt*));
    sc->num_ents_allocd = (U32)num_trigs + (U32)num_stats + (U32)num_dyns + (U32)num_npcs;
    sc->num_act_ents = 0;
    sc->act_ents = (_xEnt**)xMemAlloc(gActiveHeap, sc->num_ents_allocd * sizeof(_xEnt*));
    sc->num_nact_ents = 0;
    sc->nact_ents = (_xEnt**)xMemAlloc(gActiveHeap, sc->num_ents_allocd * sizeof(_xEnt*));
    sc->num_ents = 0;
    sc->num_trigs = 0;
    sc->num_stats = 0;
    sc->num_dyns = 0;
    sc->num_npcs = 0;
    sc->resolvID = NULL;

    xAnimInit();
    xModelInit();
    xAnimPoolInit(&sc->mempool, 50, 1, 0x1, 4);
    xModelPoolInit(num_npcs + 24, 64);
    xModelPoolInit(num_npcs + 24 + num_dyns, 8);
    xModelPoolInit(num_npcs + 24 + num_dyns + num_stats, 1);
    xAnimTempTransitionInit(16);
    xCollideInit(sc);
    xCollideFastInit(sc);
}

void xSceneSave(_xScene* sc, xSerial* s)
{
}

void xSceneLoad(_xScene* sc, xSerial* s)
{
}

void xSceneSetup(_xScene* sc)
{
    xEnvSetup(sc->env);
}

// Files the entity under its collision type (trigger 1, static 2, dynamic 4, NPC 8) and
// in the active list.
void xSceneAddEnt(_xScene* sc, _xEnt* ent)
{
    if (ent->collType == 1)
    {
        sc->trigs[sc->num_trigs++] = ent;
    }
    else if (ent->collType == 2)
    {
        sc->stats[sc->num_stats++] = ent;
    }
    else if (ent->collType == 4)
    {
        sc->dyns[sc->num_dyns++] = ent;
    }
    else if (ent->collType == 8)
    {
        sc->npcs[sc->num_npcs++] = ent;
    }

    sc->act_ents[sc->num_act_ents++] = ent;
}

xBase* xSceneResolvID(_xScene* sc, U32 id)
{
    if (sc->resolvID)
    {
        return sc->resolvID(id);
    }

    return NULL;
}

char* xSceneID2Name(_xScene* sc, U32 id)
{
    if (sc->id2Name)
    {
        return sc->id2Name(id);
    }

    return NULL;
}

// Each walk stops early when the callback returns NULL.
void xSceneForAllTriggers(_xScene* sc, _xEnt* (*func)(_xEnt*, _xScene*, void*), void* data)
{
    for (U16 i = 0; i < sc->num_trigs; i++)
    {
        if (!func(sc->trigs[i], sc, data))
        {
            break;
        }
    }
}

void xSceneForAllStatics(_xScene* sc, _xEnt* (*func)(_xEnt*, _xScene*, void*), void* data)
{
    for (U16 i = 0; i < sc->num_stats; i++)
    {
        if (!func(sc->stats[i], sc, data))
        {
            break;
        }
    }
}

void xSceneForAllDynamics(_xScene* sc, _xEnt* (*func)(_xEnt*, _xScene*, void*), void* data)
{
    for (U16 i = 0; i < sc->num_dyns; i++)
    {
        if (!func(sc->dyns[i], sc, data))
        {
            break;
        }
    }
}

void xSceneForAllNPCs(_xScene* sc, _xEnt* (*func)(_xEnt*, _xScene*, void*), void* data)
{
    for (U16 i = 0; i < sc->num_npcs; i++)
    {
        if (!func(sc->npcs[i], sc, data))
        {
            break;
        }
    }
}
