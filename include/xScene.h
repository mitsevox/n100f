#ifndef XSCENE_H
#define XSCENE_H

#include "types.h"
#include "xBase.h"

struct _xEnt;

// Partial: members up to npcs.
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
};

xBase* xSceneResolvID(_xScene* sc, U32 id);

#endif
