#include "TRK_MINNOW_DOLPHIN/trk.h"

// Writes back and invalidates the data and instruction caches over [start, start + length).
asm void TRK_flush_cache(register u32 start, register int length) {
    nofralloc

    lis r5, 0xFFFF
    ori r5, r5, 0xFFF1
    and r5, r5, start
    subf r3, r5, start
    add r4, length, r3

loop:
    dcbst 0, r5
    dcbf 0, r5
    sync
    icbi 0, r5
    addic r5, r5, 8
    addic. r4, r4, -8
    bge loop

    isync
    blr
}
