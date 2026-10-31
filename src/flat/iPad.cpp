#include "iPad.h"

#include <dolphin/pad.h>

#include "xPad.h"
#include "xTRC.h"

#define PAD_ERR_NO_CONTROLLER -1
#define PAD_ERR_NOT_READY -2
#define PAD_ERR_TRANSFER -3

// The GC-only xTRC.cpp pad and disc helpers (see their rows in evidence.tsv).
extern "C" S32 fn_800443F8();
extern "C" void fn_80044458(S32 pad_id, S32 state);

PADStatus sPadData[4];

S32 iPadInit()
{
    PADInit();
    return 1;
}

// Attach a pad to a port; it can rumble and starts in the inserted TRC state.
_tagxPad* iPadEnable(_tagxPad* pad, S16 port)
{
    pad->port = port;
    pad->slot = 0;
    lbl_802289B8[pad->port].state = TRC_PadInserted;
    pad->flags |= 3;
    pad->flags |= 4;
    return pad;
}

// GC button bits to the game's xPad bits.
static inline U32 iPadConvFromGCN(U32 buttons, U32 mask, U32 bit)
{
    return (buttons & mask) ? bit : 0;
}

// A stick axis (about -40 to 40 after PADClamp) scaled by 3.2 into -127 to 127.
static inline S32 iPadConvStick(F32 value)
{
    if (value > 40.0f)
    {
        value = 40.0f;
    }
    else if (value < -40.0f)
    {
        value = -40.0f;
    }

    value = 3.2f * value;

    if (value > 127.0f)
    {
        value = 127.0f;
    }
    else if (value < -127.0f)
    {
        value = -127.0f;
    }

    return value;
}

// Read the pads (once per frame, from pad 0), report a missing pad to the TRC layer and reset
// it, and fill in the pad's button bits and both sticks. Returns 0 while a TRC message holds
// the game or when the pad gave no data.
S32 iPadUpdate(_tagxPad* pad, U32* on)
{
    U32 temp_on;
    U16 buttons;

    if (fn_800443F8())
    {
        return 0;
    }

    if (pad->port == 0 && pad->slot == 0)
    {
        PADRead(sPadData);
        PADClamp(sPadData);
    }

    switch (sPadData[pad->port].err)
    {
    case PAD_ERR_NO_CONTROLLER:
        fn_80044458(pad->port, TRC_PadMissing);
        PADReset(0x80000000 >> pad->port);
        return 0;
    case PAD_ERR_NOT_READY:
    case PAD_ERR_TRANSFER:
        return 0;
    default:
        return 0;
    case PAD_ERR_NONE:
        break;
    }

    buttons = sPadData[pad->port].button;
    temp_on = iPadConvFromGCN(buttons, 0x1, 0x80);
    temp_on |= iPadConvFromGCN(buttons, 0x2, 0x20);
    temp_on |= iPadConvFromGCN(buttons, 0x4, 0x40);
    temp_on |= iPadConvFromGCN(buttons, 0x8, 0x10);

    if (buttons & 0x10)
    {
        temp_on |= iPadConvFromGCN(buttons, 0x20, 0x2000);
        temp_on |= iPadConvFromGCN(buttons, 0x40, 0x200);
        if (sPadData[pad->port].triggerLeft >= 0x18)
        {
            temp_on |= 0x200;
        }
        if (sPadData[pad->port].triggerRight >= 0x18)
        {
            temp_on |= 0x2000;
        }
        temp_on |= 0x100000;
    }
    else
    {
        temp_on |= iPadConvFromGCN(buttons, 0x20, 0x1000);
        temp_on |= iPadConvFromGCN(buttons, 0x40, 0x100);
        if (sPadData[pad->port].triggerLeft >= 0x18)
        {
            temp_on |= 0x100;
        }
        if (sPadData[pad->port].triggerRight >= 0x18)
        {
            temp_on |= 0x1000;
        }
    }

    temp_on |= iPadConvFromGCN(buttons, 0x100, 0x10000);
    temp_on |= iPadConvFromGCN(buttons, 0x200, 0x80000);
    temp_on |= iPadConvFromGCN(buttons, 0x800, 0x40000);
    temp_on |= iPadConvFromGCN(buttons, 0x400, 0x20000);
    temp_on |= iPadConvFromGCN(buttons, 0x1000, 0x1);
    *on = temp_on;

    pad->analog1.x = iPadConvStick(sPadData[pad->port].stickX);
    pad->analog1.y = -iPadConvStick(sPadData[pad->port].stickY);
    pad->analog2.x = iPadConvStick(sPadData[pad->port].substickX);
    pad->analog2.y = -iPadConvStick(sPadData[pad->port].substickY);

    if (lbl_802289B8[pad->port].state != TRC_PadInserted)
    {
        fn_80044458(pad->port, TRC_PadInserted);
    }

    return 1;
}

// Rumble effects are not timed on GC.
void iPadRumbleFx(_tagxPad* p, _tagxRumble* r, F32 time_passed)
{
}

void iPadStopRumble(_tagxPad* pad)
{
    PADControlMotor(pad->port, 0);
}

// Rumble is all or nothing on GC: the motor turns on for any rumble type.
void iPadStartRumble(_tagxPad* pad, _tagxRumble* rumble)
{
    PADControlMotor(pad->port, 1);
}

// Pad shutdown needs nothing on GC.
void iPadKill()
{
}
