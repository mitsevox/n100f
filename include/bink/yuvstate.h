#ifndef BINK_YUVSTATE_H
#define BINK_YUVSTATE_H

#include "types.h"

typedef struct YUVSTATE
{
    U32* dst0;
    U32* dst1;
    U32* y0;
    U32* y1;
    U16* u;
    U16* v;
    U32* a0;
    U32* a1;
    U8 unk20[0x24];
    S32 pitch;
    U8* base;
} YUVSTATE;

extern YUVSTATE S;

#endif
