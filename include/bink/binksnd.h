#ifndef BINK_BINKSND_H
#define BINK_BINKSND_H

#include "types.h"

typedef struct BinkSnd BinkSnd;

typedef S32 (*BinkSndOpen)(BinkSnd* snd, U32 freq, S32 bits, S32 chans);

struct BinkSnd {
    S32 (*ready)(BinkSnd* snd);
    S32 (*lock)(BinkSnd* snd, U8** addr, U32* len);
    S32 (*unlock)(BinkSnd* snd, U32 len);
    void (*volume)(BinkSnd* snd, S32 volume);
    void (*pan)(BinkSnd* snd, S32 pan);
    S32 (*pause)(BinkSnd* snd, S32 status);
    S32 (*set_onoff)(BinkSnd* snd, S32 status);
    void (*close)(BinkSnd* snd);
    U8 unk20[0x34];
    U32 best_size;
    S32 size_mask;
    S32 starved;
    S32 on;
    S32 unk64;
    S32 unk68;
    U32 freq;
    S32 bits;
    S32 chans;
    U8 priv[256];
};

#endif
