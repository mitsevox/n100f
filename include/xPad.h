#ifndef XPAD_H
#define XPAD_H

#include "types.h"

struct _tagPadAnalog
{
    S8 x;
    S8 y;
};

// Partial: the state and rumble members after the analog sticks are left opaque.
// The object is 0x68 bytes, as mPad[4] spans 0x1A0.
struct _tagxPad
{
    U8 value[22];
    U8 last_value[22];
    U32 on;
    U32 pressed;
    U32 released;
    _tagPadAnalog analog1;
    _tagPadAnalog analog2;
    U8 opaque_3C[0x68 - 0x3C];
};

extern _tagxPad mPad[4];
extern _tagxPad* gDebugPad;

S32 xPadUpdate(S32 idx, F32 time_passed);

#endif
