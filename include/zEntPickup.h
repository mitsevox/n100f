#ifndef ZENTPICKUP_H
#define ZENTPICKUP_H

#include "zEnt.h"

struct zAssetPickup;
struct xPar;
struct _zParEmitter;

struct _zEntPickup : _zEnt
{
    U32 state;
    zAssetPickup* p;
    void* anim;
    F32 animTime;
    F32 timer;
    F32 shake_timer;
    _xVec3 shake_pos;
    S32 snackGateInfoDisplayed;
    F32 fx_timer;
    xPar* fx_par;
    _zParEmitter* fx_emit;
    F32 fx_scale;
    _xVec3 grab_pos;
    U32 flyflags;
    S32 flg_opts;
    _xVec3 vel;
    _xVec3 droppos;
    xBase* followTarget;
    _xVec3 followOffset;
    xBase* dropParent;
    _xEnt* useThisEntPos;
};

void zEntPickup_Drop(_zEntPickup* ent);
void zEntPickup_MakeDroppable(_zEntPickup* ent, _xVec3* pos, xBase* parent);

#endif
