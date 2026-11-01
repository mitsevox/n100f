#include "xPad.h"
#include "iPad.h"
#include "zGame.h"
#include "zGlobals.h"
#include "zMenu.h"
#include "zScene.h"

#include <string.h>

void iPadStartRumble(_tagxPad* pad, _tagxRumble* rumble);

// iPad.cpp has no source yet; these are its rumble effect step and its pad read.
// zScene.cpp
#if VERSION_GIHE78_REV0
#define PAD_RUMBLE_STEP fn_800CD24C
#define PAD_SWAP_BUTTONS fn_800CD170
#elif VERSION_GIHE78_REV1
#define PAD_RUMBLE_STEP fn_800CCE40
#define PAD_SWAP_BUTTONS fn_800CCD64
#endif
extern "C" U32 PAD_RUMBLE_STEP();
extern "C" S32 PAD_SWAP_BUTTONS();

// mPad (0x80200008) lies outside this unit's .bss range: gc/iFMV.cpp, which the split tool does
// not write, loads it too, so the range ends before it and the array is defined there.
extern _tagxPad mPad[4];
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

// Analog-as-digital makes the sticks repeat as d-pad presses, for the menus.
static inline void xPadAnalogIsDigital(S32 idx, S32 enable)
{
    if (idx != 0)
    {
        return;
    }

    _tagxPad* p = mPad + idx;
    if (enable)
    {
        p->flags |= 0x10;
    }
    else
    {
        p->flags &= ~0x10;
    }

    if (p->al2d_timer >= 0.35f)
    {
        p->al2d_timer = 0.35f;
    }
    if (p->ar2d_timer >= 0.35f)
    {
        p->ar2d_timer = 0.35f;
    }
    if (p->d_timer >= 0.35f)
    {
        p->d_timer = 0.35f;
    }
}

// Reads the pad and works out this frame's presses and releases. In the menus, while paused and
// in the gallery the sticks and the d-pad repeat every 0.35 seconds while held.
S32 xPadUpdate(S32 idx, F32 time_passed)
{
    U32 new_on;
    _tagxPad* p;

    if (idx != 0)
    {
        return 0;
    }

    if (PAD_RUMBLE_STEP())
    {
        xPadAnalogIsDigital(idx, 0);
    }
    else if (zMenuRunning() || zGameIsPaused() ||
             (globals.sceneCur && globals.sceneCur->sceneID == zSceneIDFromName("h003")))
    {
        xPadAnalogIsDigital(idx, 1);
    }
    else
    {
        xPadAnalogIsDigital(idx, 0);
    }

    p = mPad + idx;
    if ((p->flags & 4) && (p->flags & 8))
    {
        _tagxRumble* r = p->rumble_head.next;
        while (r)
        {
            r->seconds -= time_passed;
            if (r->seconds <= 0.0f)
            {
                r->active = 0;
                if (r->next == NULL)
                {
                    p->rumble_head.next = NULL;
                    r = NULL;
                    iPadStopRumble(p);
                }
                else
                {
                    p->rumble_head.next = r->next;
                    r = p->rumble_head.next;
                    iPadStartRumble(p, r);
                }
            }
            else
            {
                iPadRumbleFx(p, r, time_passed);
                break;
            }
        }
    }

    if (!iPadUpdate(p, &new_on))
    {
        return 1;
    }

    // Swaps the two buttons at 0x40000 and 0x80000 on the first port while PAD_SWAP_BUTTONS says so.
    if (p->port == 0 && PAD_SWAP_BUTTONS())
    {
        U32 swapped = new_on & ~0xC0000;
        if (new_on & 0x40000)
        {
            swapped |= 0x80000;
        }
        if (new_on & 0x80000)
        {
            swapped |= 0x40000;
        }
        new_on = swapped;
    }

    if (p->flags & 0x10)
    {
        if (p->flags & 1)
        {
            U32 fake_dpad = 0;
            if (p->analog1.x >= 50)
            {
                fake_dpad |= 0x20;
            }
            else if (p->analog1.x <= -50)
            {
                fake_dpad |= 0x80;
            }
            if (p->analog1.y >= 50)
            {
                fake_dpad |= 0x40;
            }
            else if (p->analog1.y <= -50)
            {
                fake_dpad |= 0x10;
            }

            if (fake_dpad == 0)
            {
                p->al2d_timer = 0.0f;
            }
            else
            {
                p->al2d_timer -= time_passed;
                if (p->al2d_timer <= 0.0f)
                {
                    new_on |= fake_dpad;
                    p->al2d_timer = 0.35f;
                }
            }
        }

        if (p->flags & 2)
        {
            if (p->analog2.x > -50 && p->analog2.x < 50 && p->analog2.y > -50 && p->analog2.y < 50)
            {
                p->ar2d_timer = 0.0f;
            }
            else
            {
                p->ar2d_timer -= time_passed;
                if (p->ar2d_timer <= 0.0f)
                {
                    p->ar2d_timer = 0.35f;
                    if (p->analog2.x >= 50)
                    {
                        new_on |= 0x20;
                    }
                    else if (p->analog2.x <= -50)
                    {
                        new_on |= 0x80;
                    }
                    if (p->analog2.y >= 50)
                    {
                        new_on |= 0x40;
                    }
                    else if (p->analog2.y <= -50)
                    {
                        new_on |= 0x10;
                    }
                }
            }
        }
    }

    p->pressed = new_on & ~p->on;
    p->released = p->on & ~new_on;
    p->on = new_on;

    if (p->flags & 0x10)
    {
        if (!(p->on & 0x10) && !(p->on & 0x40) && !(p->on & 0x80) && !(p->on & 0x20))
        {
            p->d_timer = 0.0f;
        }
        else
        {
            p->d_timer -= time_passed;
            if (p->d_timer <= 0.0f)
            {
                p->d_timer = 0.35f;
                if (p->on & 0x10)
                {
                    p->pressed |= 0x10;
                }
                else if (p->on & 0x40)
                {
                    p->pressed |= 0x40;
                }
                if (p->on & 0x80)
                {
                    p->pressed |= 0x80;
                }
                else if (p->on & 0x20)
                {
                    p->pressed |= 0x20;
                }
            }
        }
    }

    return 1;
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

static inline _tagxRumble* xPadGetRumbleSlot()
{
    for (S32 i = 0; i < 32; i++)
    {
        if (!mRumbleList[i].active)
        {
            memset(&mRumbleList[i], 0, sizeof(_tagxRumble));
            return &mRumbleList[i];
        }
    }
    return NULL;
}

// Queues a rumble on the pad, or replaces the queue when replace is set. Only the head of the
// queue runs; xPadUpdate starts the next one when it runs out.
S32 xPadAddRumble(S32 idx, _tagRumbleType type, F32 time, S32 replace, U32 fxflags)
{
    _tagxPad* pad = mPad + idx;
    S32 appended;
    _tagxRumble* r;
    _tagxRumble* last_r;

    if (!(pad->flags & 4))
    {
        return 0;
    }
    if (!(pad->flags & 8))
    {
        return 0;
    }

    appended = 1;
    if (replace)
    {
        xPadDestroyRumbleChain(pad);
        appended = 0;
    }

    r = &pad->rumble_head;
    last_r = r;
    if (r->next == NULL)
    {
        appended = 0;
    }
    while (r != NULL)
    {
        last_r = r;
        r = r->next;
    }

    r = xPadGetRumbleSlot();
    if (r == NULL)
    {
        return 0;
    }

    r->type = type;
    r->seconds = time;
    r->active = 1;
    r->fxflags = fxflags;
    last_r->next = r;
    last_r->next->next = NULL;
    if (!appended)
    {
        iPadStartRumble(pad, r);
    }
    return 1;
}
