#ifndef ZSCENE_H
#define ZSCENE_H

#include "types.h"
#include "xBase.h"
#include "xScene.h"

struct _zEnt;

// Partial: members up to ents. The _xScene members past gravity are left opaque.
struct zScene : _xScene
{
    U8 opaque_14[0x74 - 0x14];
    U32 num_ents;
    _zEnt** ents;
};

xBase* zSceneFindObject(U32 gameID);

#endif
