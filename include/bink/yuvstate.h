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
} YUVSTATE;

extern YUVSTATE S;

#endif
