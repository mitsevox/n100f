#ifndef BINK_RADCB_H
#define BINK_RADCB_H

#include <dolphin/os.h>
#include "types.h"

typedef struct RADCB RADCB;
typedef S32 (*RADCBFUNC)(RADCB* cb, U32 ticks);

struct RADCB
{
    RADCB* next;
    U8 unk04[4];
    S32 result;
    volatile S32 state;
    volatile S32 suspends;
    RADCBFUNC timer;
    RADCBFUNC handler;
};

typedef struct RADCBHANDLER
{
    volatile S32 state;
    volatile S32 suspends;
    OSAlarm alarm;
    RADCB* list;
    U32 ticks;
    S32 count;
} RADCBHANDLER;

RADCBHANDLER* RADCB_allocate_handler(U32 hz);
void RADCB_register_callback(RADCBHANDLER* h, RADCB* cb, RADCBFUNC timer, RADCBFUNC handler);
S32 RADCB_unregister_2_callbacks(RADCBHANDLER* h1, RADCB* cb1, RADCBHANDLER* h2, RADCB* cb2, U32 flags);
void RADCB_resume_handler(RADCBHANDLER* h);
S32 RADCB_try_to_suspend_callback(RADCBHANDLER* h, RADCB* cb);
void RADCB_suspend_callback(RADCBHANDLER* h, RADCB* cb);
void RADCB_suspend_2_callbacks(RADCBHANDLER* h1, RADCB* cb1, RADCBHANDLER* h2, RADCB* cb2);
void RADCB_resume_callback(RADCBHANDLER* h, RADCB* cb);
void RADCB_free_handler(RADCBHANDLER* h);
void RADCB_idle_on_callbacks(void);

#endif
