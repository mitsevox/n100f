#ifndef BINK_BINKIO_H
#define BINK_BINKIO_H

#include "types.h"
#include "bink/radcb.h"

typedef struct Bink Bink;
typedef struct BinkIO BinkIO;

typedef S32 (*BinkIOOpen)(BinkIO* io, const char* name, U32 flags);

struct BinkIO {
    U32 (*read_header)(BinkIO* io, S32 offset, void* dest, U32 size);
    U32 (*read_frame)(BinkIO* io, U32 frame, S32 offset, void* dest, U32 size);
    U32 (*get_buffer_size)(BinkIO* io, U32 size);
    void (*set_info)(BinkIO* io, void* buf, U32 size, U32 filesize, U32 simulate);
    U32 (*idle)(BinkIO* io);
    void (*close)(BinkIO* io);
    Bink* bink;
    volatile U32 error;
    volatile U32 reading;
    volatile U32 bytes_read;
    volatile U32 busy;
    volatile U32 total_time;
    volatile U32 fg_time;
    volatile U32 idle_time;
    volatile U32 bg_time;
    volatile U32 buf_size;
    volatile U32 buf_peak;
    volatile U32 cur_size;
    volatile U32 cur_used;
    U8 priv[160];
    void (*suspend)(BinkIO* io);
    S32 (*try_suspend)(BinkIO* io);
    void (*resume)(BinkIO* io);
    void (*idle_on)(BinkIO* io);
    RADCB cb;
    U8 unk118[36];
};

S32 BinkFileOpen(BinkIO* io, const char* name, U32 flags);

#endif
