#ifndef XSCENE_H
#define XSCENE_H

#include "types.h"
#include "xBase.h"

// Partial: members up to gravity.
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
};

xBase* xSceneResolvID(_xScene* sc, U32 id);

#endif
