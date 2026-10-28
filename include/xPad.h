#ifndef XPAD_H
#define XPAD_H

#include "types.h"

// Partial: the analog, state and rumble members after the button masks are left opaque.
// The object is 0x68 bytes, as mPad[4] spans 0x1A0.
struct _tagxPad
{
    U8 value[22];
    U8 last_value[22];
    U32 on;
    U32 pressed;
    U32 released;
    U8 opaque_38[0x68 - 0x38];
};

extern _tagxPad mPad[4];

S32 xPadUpdate(S32 idx, F32 time_passed);

#endif
