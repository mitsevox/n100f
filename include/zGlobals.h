#ifndef ZGLOBALS_H
#define ZGLOBALS_H

#include "types.h"
#include "xBase.h"
#include "xMath3.h"
#include "zEntPlayer.h"
#include "iCamera.h"
#include "iTime.h"

struct zScene;
struct zAssetPickupTable;
struct _zCutsceneMgr;
struct _tagxPad;

// Partial: only the header and the matrix are declared. The GC object is 0xA0 bytes; the
// PS2 copy is 0xB0 because its matrices are 16-byte aligned.
struct _tagxCam : xBase
{
    U32 valid_flags;
    S32 cam_type;
    U32 flags;
    xMat4x3 mat;
    U8 opaque_5C[0xA0 - 0x5C];
};

// Partial: only the RenderWare camera and the two cameras are declared. The GC object is
// 0x1F0 bytes.
struct _tagxCamFrame
{
    RwCamera* lo_cam;
    _tagxCam orig_cam;
    _tagxCam hi_cam;
    U8 opaque_144[0x1F0 - 0x144];
};

// Partial: only camera, the pads, player, pickupTable, profile, cmgr, sceneCur, the fog state
// and update_dt are declared. The members between them are left opaque.
struct zGlobals
{
    _tagxCamFrame camera;
    _tagxPad* pad0;
    _tagxPad* pad1;
    zPlayerGlobals player;
    zAssetPickupTable* pickupTable;
    S32 profile;
    _zCutsceneMgr* cmgr;
    U8 opaque_864[0xB64 - 0x864];
    zScene* sceneCur;
    U8 opaque_B68[0xB90 - 0xB68];
    iFogParams fog;
    iFogParams fogA;
    iFogParams fogB;
    iTime fog_t0;
    iTime fog_t1;
    U8 opaque_BF8[0x32FC - 0xBF8];
    F32 update_dt;
};

extern zGlobals globals;

#endif
