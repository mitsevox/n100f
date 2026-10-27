#ifndef ZPLATFORM_H
#define ZPLATFORM_H

#include "zEnt.h"

// Partial: no members past _zEnt are declared.
struct _zPlatform : _zEnt
{
};

S32 zPlatformEventCB(xBase* from, xBase* to, U32 toEvent, F32* toParam, xBase* toParamWidget);

#endif
