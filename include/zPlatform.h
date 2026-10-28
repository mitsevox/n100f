#ifndef ZPLATFORM_H
#define ZPLATFORM_H

#include "zEnt.h"
#include "xEntMotion.h"

struct p2PlatformAsset;

// Partial: members up to moving. The members between motion and moving that no
// reconstructed function uses are left opaque.
struct _zPlatform : _zEnt
{
    p2PlatformAsset* passet;
    _xEntMotion motion;
    U8 opaque_motion_end[0x174 - 0xF0 - sizeof(_xEntMotion)];
    S32 moving;
};

struct _xScene;

void zPlatform_Update(_xEnt* entplat, _xScene* sc, F32 dt);
S32 zPlatformEventCB(xBase* from, xBase* to, U32 toEvent, F32* toParam, xBase* toParamWidget);

#endif
