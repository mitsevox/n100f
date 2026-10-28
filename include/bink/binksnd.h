#ifndef BINK_BINKSND_H
#define BINK_BINKSND_H

#include "types.h"

struct Bink;
typedef struct BinkSnd BinkSnd;

typedef S32 (*BinkSndOpen)(BinkSnd* snd, U32 freq, S32 bits, S32 chans, U32 flags, struct Bink* bink);
typedef BinkSndOpen (*BinkSndSysOpen)(U32 param);

struct BinkSnd {
    S32 (*ready)(BinkSnd* snd);
    S32 (*lock)(BinkSnd* snd, U8** addr, U32* len);
    S32 (*unlock)(BinkSnd* snd, U32 len);
    void (*volume)(BinkSnd* snd, S32 volume);
    void (*pan)(BinkSnd* snd, S32 pan);
    S32 (*pause)(BinkSnd* snd, S32 status);
    S32 (*set_onoff)(BinkSnd* snd, S32 status);
    void (*close)(BinkSnd* snd);
    void (*mix_bins)(BinkSnd* snd, U32* bins, U32 total);
    void (*mix_bin_vols)(BinkSnd* snd, U32* bins, S32* vols, U32 total);
    U32 buf_size;
    U8* buf;
    U8* buf_end;
    U8* write_ptr;
    U8* read_ptr;
    void* decomp;
    U32 buffered;
    S32 convert8;
    S32 end_frame;
    U32 prime;
    U32 unk50;
    U32 best_size;
    S32 size_mask;
    S32 starved;
    S32 on;
    U32 latency;
    U32 speed;
    U32 freq;
    S32 bits;
    S32 chans;
    U8 priv[256];
};

BinkSndOpen BinkOpenNGCSound(U32 param);

#endif
