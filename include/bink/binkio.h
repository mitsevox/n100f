#ifndef BINK_BINKIO_H
#define BINK_BINKIO_H

#include "types.h"

typedef struct BinkIO BinkIO;

struct BinkIO {
    U32 (*read_header)(BinkIO* io, S32 offset, void* dest, U32 size);
    U32 (*read_frame)(BinkIO* io, U32 frame, S32 offset, void* dest, U32 size);
    U32 (*get_buffer_size)(BinkIO* io, U32 size);
    void (*set_info)(BinkIO* io, void* buf, U32 size, U32 filesize, U32 simulate);
    U32 (*idle)(BinkIO* io);
    void (*close)(BinkIO* io);
    void* unk18;
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
    U8 priv[240];
};

#endif
