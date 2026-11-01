#include <string.h>
#include <dolphin/types.h>

// The runtime's memory fill and copy, linked into .init so that __init_data can use them to copy
// the ROM sections and clear .bss before anything else runs. Follows bfbb's
// src/PowerPC_EABI_Support/src/Runtime/__mem.c (CREDITS.md).

__declspec(section ".init") void __fill_mem(void* dst, int c, size_t n);

__declspec(section ".init") void* memset(void* dst, int c, size_t n)
{
    __fill_mem(dst, c, n);
    return dst;
}

// Fills n bytes at dst with the byte c: byte stores up to a word boundary, then eight words at a
// time, then single words, then the trailing bytes.
__declspec(section ".init") void __fill_mem(void* dst, int c, size_t n)
{
    u8* cdest;
    u32* idest;
    u32 i;
    u32 cval;

    cval = (u8)c;
    cdest = (u8*)dst - 1;

    if (n >= 32)
    {
        i = ~(u32)cdest & 3;
        if (i != 0)
        {
            n = n - i;
            do
            {
                *(++cdest) = (u8)cval;
            } while (--i);
        }
        if (cval != 0)
        {
            cval = cval << 24 | cval << 16 | cval << 8 | cval;
        }

        idest = (u32*)(cdest - 3);
        i = n >> 5;
        if (i != 0)
        {
            do
            {
                idest[1] = cval;
                idest[2] = cval;
                idest[3] = cval;
                idest[4] = cval;
                idest[5] = cval;
                idest[6] = cval;
                idest[7] = cval;
                *(idest += 8) = cval;
            } while (--i);
        }

        i = (n >> 2) & 7;

        if (i != 0)
        {
            do
            {
                *++idest = cval;
            } while (--i);
        }

        cdest = (u8*)idest + 3;
        n &= 3;
    }
    if (n != 0)
    {
        do
        {
            *++cdest = (u8)cval;
        } while (--n);
    }
}

// Copies n bytes, forwards when the source is at or above the destination, backwards otherwise,
// so overlapping copies are safe.
__declspec(section ".init") void* memcpy(void* dst, const void* src, size_t n)
{
    u8* __src;
    u8* __dst;
    int i;

    if (src >= dst)
    {
        __src = ((u8*)src) - 1;
        __dst = ((u8*)dst) - 1;
        i = n + 1;
        while (--i)
        {
            *((u8*)++__dst) = *((u8*)++__src);
        }
        return dst;
    }
    else
    {
        __src = ((u8*)src) + n;
        __dst = ((u8*)dst) + n;
        i = n + 1;
        while (--i)
        {
            *((u8*)--__dst) = *((u8*)--__src);
        }
        return dst;
    }
}
