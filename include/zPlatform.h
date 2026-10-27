#ifndef ZPLATFORM_H
#define ZPLATFORM_H

#include "zEnt.h"

// Partial: members up to moving. The members before it that no reconstructed function uses
// are left opaque.
struct _zPlatform : _zEnt
{
    U8 opaque_EC[0x174 - 0xEC];
    S32 moving;
};

struct _xScene;

void zPlatform_Update(_xEnt* entplat, _xScene* sc, F32 dt);
S32 zPlatformEventCB(xBase* from, xBase* to, U32 toEvent, F32* toParam, xBase* toParamWidget);

#endif
