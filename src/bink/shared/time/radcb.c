#include <dolphin/os.h>
#include "bink/radcb.h"
#include "bink/radmem.h"

typedef struct OSLowMemory
{
    U8 pad[0xF8];
    U32 busClock;
} OSLowMemory;

#define BUS_CLOCK (((OSLowMemory*)OS_BASE_CACHED)->busClock)

static S32 remove_cb_from_list(RADCBHANDLER* h, RADCB* cb)
{
    RADCB* p;

    if (h == 0)
        return 0;

    p = h->list;
    if (p == cb)
    {
        h->list = cb->next;
        return 1;
    }

    while (p->next)
    {
        if (p->next == cb)
        {
            p->next = cb->next;
            return 1;
        }
        p = p->next;
    }

    return 0;
}

static void call_callbacks(RADCBHANDLER* h)
{
    RADCB* cb;

    h->ticks++;
    if (h->state == 1)
    {
        for (cb = h->list; cb; cb = cb->next)
        {
            if (cb->state == 1)
            {
                cb->result = cb->timer(cb, h->ticks);
                if (cb->result)
                    cb->handler(cb, h->ticks);
            }
        }
    }
}

static void RAD_callback_timer(OSAlarm* alarm, OSContext* context)
{
    RADCBHANDLER* h = (RADCBHANDLER*)((char*)alarm - 8);

    if (h && h->list)
    {
        BOOL enabled = OSEnableInterrupts();
        call_callbacks(h);
        OSRestoreInterrupts(enabled);
    }
}

RADCBHANDLER* RADCB_allocate_handler(U32 hz)
{
    RADCBHANDLER* h = radmalloc(sizeof(RADCBHANDLER));

    if (h)
    {
        memset(h, 0, sizeof(RADCBHANDLER));
        h->state = 0;
        h->suspends = 1;
        h->ticks = 1;
        h->list = 0;
        h->count = 0;
        OSSetPeriodicAlarm(&h->alarm, OSGetTime(), (1000 / hz) * (BUS_CLOCK / 4 / 1000), RAD_callback_timer);
    }

    return h;
}

void RADCB_register_callback(RADCBHANDLER* h, RADCB* cb, RADCBFUNC timer, RADCBFUNC handler)
{
    BOOL enabled = OSDisableInterrupts();

    cb->state = 1;
    cb->timer = timer;
    cb->handler = handler;
    cb->suspends = 0;
    cb->next = h->list;
    h->list = cb;
    h->count++;

    OSRestoreInterrupts(enabled);
}

S32 RADCB_unregister_callback(RADCBHANDLER* h, RADCB* cb, U32 flags)
{
    S32 freed = 0;

    if (h && cb && cb->handler && cb->timer)
    {
        BOOL enabled = OSDisableInterrupts();

        if (remove_cb_from_list(h, cb))
        {
            h->count--;
            if ((flags & 1) && h->list == 0)
            {
                freed = 1;
                RADCB_free_handler(h);
            }
        }

        OSRestoreInterrupts(enabled);
    }

    return freed;
}

S32 RADCB_unregister_2_callbacks(RADCBHANDLER* h1, RADCB* cb1, RADCBHANDLER* h2, RADCB* cb2, U32 flags)
{
    S32 freed = RADCB_unregister_callback(h1, cb1, flags);
    return freed | (RADCB_unregister_callback(h2, cb2, flags) << 1);
}

S32 RADCB_registered_count(RADCBHANDLER* h)
{
    return h->count;
}

S32 RADCB_try_to_suspend_handler(RADCBHANDLER* h)
{
    if (h)
    {
        BOOL enabled = OSDisableInterrupts();

        if (h->state == 1)
            h->state = 2;
        h->suspends++;

        OSRestoreInterrupts(enabled);
        return h->state != 1;
    }

    return 1;
}

void RADCB_suspend_handler(RADCBHANDLER* h)
{
    while (!RADCB_try_to_suspend_handler(h))
        OSYieldThread();
}

void RADCB_resume_handler(RADCBHANDLER* h)
{
    if (h)
    {
        BOOL enabled = OSDisableInterrupts();

        h->suspends--;
        if (h->suspends == 0)
            h->state = 1;

        OSRestoreInterrupts(enabled);
    }
}

S32 RADCB_try_to_suspend_callback(RADCBHANDLER* h, RADCB* cb)
{
    if (h && cb && cb->handler && cb->timer)
    {
        BOOL enabled = OSDisableInterrupts();

        if (cb->state == 1)
            cb->state = 2;
        cb->suspends++;

        OSRestoreInterrupts(enabled);
        return cb->state != 1;
    }

    return 1;
}

void RADCB_suspend_callback(RADCBHANDLER* h, RADCB* cb)
{
    if (h && cb && cb->handler && cb->timer)
    {
        while (!RADCB_try_to_suspend_callback(h, cb))
            OSYieldThread();
    }
}

void RADCB_suspend_2_callbacks(RADCBHANDLER* h1, RADCB* cb1, RADCBHANDLER* h2, RADCB* cb2)
{
    RADCB_suspend_callback(h1, cb1);
    RADCB_suspend_callback(h2, cb2);
}

void RADCB_resume_callback(RADCBHANDLER* h, RADCB* cb)
{
    if (h && cb && cb->handler && cb->timer)
    {
        BOOL enabled = OSDisableInterrupts();

        cb->suspends--;
        if (cb->suspends == 0)
            cb->state = 1;

        OSRestoreInterrupts(enabled);
    }
}

void RADCB_suspend_2_handlers(RADCBHANDLER* h1, RADCBHANDLER* h2)
{
    RADCB_suspend_handler(h1);
    RADCB_suspend_handler(h2);
}

void RADCB_free_handler(RADCBHANDLER* h)
{
    if (h)
    {
        RADCB_suspend_handler(h);
        OSCancelAlarm(&h->alarm);
        radfree(h);
    }
}

void RADCB_free_2_handlers(RADCBHANDLER* h1, RADCBHANDLER* h2)
{
    RADCB_free_handler(h1);
    RADCB_free_handler(h2);
}

void RADCB_idle_on_callbacks(void)
{
}

U32 RADCB_callback_size(void)
{
    return sizeof(RADCB);
}
