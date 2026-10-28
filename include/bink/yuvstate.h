#ifndef BINK_YUVSTATE_H
#define BINK_YUVSTATE_H

#include "types.h"

typedef struct YUVSTATE
{
    U8* dst0;
    U8* dst1;
    U8* y0;
    U8* y1;
    U8* u;
    U8* v;
    U8* a0;
    U8* a1;
    S32 g;
    S32 b;
    S32 r;
    U8 unk2C[0x18];
    S32 pitch;
    U8* base;
} YUVSTATE;

extern YUVSTATE S;

#endif
