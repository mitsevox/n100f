#ifndef ZENTPICKUP_H
#define ZENTPICKUP_H

#include "zEnt.h"

struct xPar;
struct _zParEmitter;
struct _xScene;
struct xSerial;

// One entry of the master pickup table (boot.HIP's PickupTable asset).
struct zAssetPickup
{
    U32 pickupHash;
    U8 pickupType;
    U8 pickupIndex;
    U16 pickupFlags;
    U32 quantity;
    U32 modelID;
    U32 animID;
};

// Header of the master pickup table; Count entries follow it.
struct zAssetPickupTable
{
    U32 Magic;
    U32 Count;
};

// Follows the p2EntAsset of each placed pickup.
struct p2EntPickupAsset
{
    U32 pickupHash;
    U16 pickupFlags;
    U16 pickupValue;
};

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

struct zEntPickupWarpPoint
{
    S32 id;
    U32 status;
    U32 sceneID;
};

void zEntPickup_SceneUpdate(F32 dt);
void zEntPickupResetWarps();
void zEntPickupCheat_allWarps();
void zEntPickup_UnlockSnackGate(_xEnt* ent);
U32 zEntPickup_lastSnackInArea();
U32 zEntPickup_WarpIsOpen(U32 index);
void enableWarpPoint(U32 index);
void zEntPickup_Drop(_zEntPickup* ent);
void zEntPickup_MakeDroppable(_zEntPickup* ent, _xVec3* pos, xBase* parent);
void zEntPickup_Load(_zEntPickup* ent, xSerial* s);
void zEntPickup_Save(_zEntPickup* ent, xSerial* s);
void zEntPickup_Reset(_zEntPickup* ent);
void zEntPickup_hideSnackGateInfo(_zEntPickup* ent);
void zEntPickup_Update(_zEntPickup* ent, _xScene* sc, F32 dt);
S32 zEntPickupEventCB(xBase* from, xBase* to, U32 toEvent, F32* toParam, xBase* toParamWidget);
void zEntPickup_Setup();
void zEntPickup_Setup(_zEntPickup* p);
void zEntPickupInit(_zEntPickup* ent, p2EntAsset* asset);
void zEntPickupInit(void* ent, void* asset);

#endif
