#include "xPad.h"
#include "iPad.h"

#include <string.h>

_tagxPad mPad[4];
static _tagxRumble mRumbleList[32];
_tagxPad* gDebugPad;
_tagxPad* gPlayerPad;

S32 xPadInit()
{
    memset(mPad, 0, sizeof(mPad));
    memset(mRumbleList, 0, sizeof(mRumbleList));
    if (!iPadInit())
    {
        return 0;
    }
    gPlayerPad = mPad;
    return 1;
}

// Only the first pad is ever enabled; it gets rumble switched on when it supports it.
_tagxPad* xPadEnable(S32 idx)
{
    _tagxPad* p = mPad + idx;
    if (p->state != ePad_Disabled)
    {
        return p;
    }
    if (idx != 0)
    {
        return p;
    }

    p = iPadEnable(p, idx);
    xPadRumbleEnable(idx, 1);
    return p;
}

void xPadRumbleEnable(S32 idx, S32 enable)
{
    _tagxPad* p = mPad + idx;
    if (enable)
    {
        if (p->flags & 4)
        {
            p->flags |= 8;
        }
    }
    else
    {
        if (p->flags & 8)
        {
            p->flags ^= 8;
            xPadDestroyRumbleChain(p);
        }
    }
}

void xPadKill()
{
    iPadKill();
}

// Stops the motor and frees every queued rumble of the pad.
void xPadDestroyRumbleChain(_tagxPad* pad)
{
    iPadStopRumble(pad);
    _tagxRumble* curr = pad->rumble_head.next;
    while (curr != NULL)
    {
        memset(curr, 0, sizeof(_tagxRumble));
        curr = curr->next;
    }
    pad->rumble_head.next = NULL;
}

void xPadDestroyRumbleChain(S32 idx)
{
    xPadDestroyRumbleChain(mPad + idx);
}
