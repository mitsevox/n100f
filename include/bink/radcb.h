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

void RADCB_free_handler(RADCBHANDLER* h);

#endif
