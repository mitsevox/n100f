#ifndef XPAD_H
#define XPAD_H

#include "types.h"

struct _tagPadAnalog
{
    S8 x;
    S8 y;
};

enum _tagPadState
{
    ePad_Disabled,
    ePad_DisabledError,
    ePad_Enabled,
    ePad_Missing,
    ePad_Total
};

enum _tagRumbleType
{
    eRumble_Off,
    eRumble_VeryLightHi,
    eRumble_VeryLight,
    eRumble_LightHi,
    eRumble_Light,
    eRumble_MediumHi,
    eRumble_Medium,
    eRumble_HeavyHi,
    eRumble_Heavy,
    eRumble_VeryHeavyHi,
    eRumble_VeryHeavy,
    eRumble_Total,
    eRumbleForceU32 = 0x7FFFFFFF
};

struct _tagxRumble
{
    _tagRumbleType type;
    F32 seconds;
    _tagxRumble* next;
    S16 active;
    U16 fxflags;
};

struct _tagiPad
{
    S32 port;
};

// The object is 0x68 bytes, as mPad[4] spans 0x1A0. Flag 0x4 marks a pad that can rumble
// and 0x8 one with rumble switched on.
struct _tagxPad
{
    U8 value[22];
    U8 last_value[22];
    U32 on;
    U32 pressed;
    U32 released;
    _tagPadAnalog analog1;
    _tagPadAnalog analog2;
    _tagPadState state;
    U32 flags;
    _tagxRumble rumble_head;
    S16 port;
    S16 slot;
    _tagiPad context;
    F32 al2d_timer;
    F32 ar2d_timer;
    F32 d_timer;
};

extern _tagxPad mPad[4];
extern _tagxPad* gDebugPad;
extern _tagxPad* gPlayerPad;

S32 xPadInit();
_tagxPad* xPadEnable(S32 idx);
void xPadRumbleEnable(S32 idx, S32 enable);
S32 xPadUpdate(S32 idx, F32 time_passed);
void xPadKill();
S32 xPadAddRumble(S32 idx, _tagRumbleType type, F32 time, S32 replace, U32 fxflags);
void xPadDestroyRumbleChain(_tagxPad* pad);
void xPadDestroyRumbleChain(S32 idx);

#endif
