#include "bink/varbits.h"

const U32 VarBitsLens[33] = {
    0x00000000, 0x00000001, 0x00000003, 0x00000007, 0x0000000F, 0x0000001F, 0x0000003F,
    0x0000007F, 0x000000FF, 0x000001FF, 0x000003FF, 0x000007FF, 0x00000FFF, 0x00001FFF,
    0x00003FFF, 0x00007FFF, 0x0000FFFF, 0x0001FFFF, 0x0003FFFF, 0x0007FFFF, 0x000FFFFF,
    0x001FFFFF, 0x003FFFFF, 0x007FFFFF, 0x00FFFFFF, 0x01FFFFFF, 0x03FFFFFF, 0x07FFFFFF,
    0x0FFFFFFF, 0x1FFFFFFF, 0x3FFFFFFF, 0x7FFFFFFF, 0xFFFFFFFF,
};

const U8 _bitlevels[129] __attribute__((aligned(64))) = {
    0, 1, 2, 2, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4,
    5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5,
    6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
    6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
    7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
    7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
    7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
    7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
    8,
};

void VarBitsCopy(VARBITS* dest, VARBITS* src, U32 size)
{
    U32 len = 8;
    U32 v;

    while (size > 7)
    {
        U32 m;

        if (src->bitlen > 7)
        {
            v = src->bits & 255;
            src->bitlen -= 8;
            src->bits >>= 8;
        }
        else
        {
            U32 w = *src->cur;
            v = (src->bits | (w << src->bitlen)) & 255;
            src->bits = w >> (8 - src->bitlen);
            src->bitlen += 24;
            src->cur++;
        }

        m = v & VarBitsLens[len];
        dest->bits |= m << dest->bitlen;
        dest->bitlen += 8;
        if (dest->bitlen > 31)
        {
            *dest->cur = dest->bits;
            dest->cur++;
            dest->bitlen -= 32;
            if (dest->bitlen)
                dest->bits = m >> (8 - dest->bitlen);
            else
                dest->bits = 0;
        }
        size -= 8;
    }

    if (size)
    {
        U32 m;

        if (src->bitlen >= size)
        {
            v = src->bits & (0xFFFFFFFF >> (32 - size));
            src->bits >>= size;
            src->bitlen -= size;
        }
        else
        {
            U32 w = *src->cur;
            v = (src->bits | (w << src->bitlen)) & (0xFFFFFFFF >> (32 - size));
            src->bits = w >> (size - src->bitlen);
            src->bitlen = src->bitlen + 32 - size;
            src->cur++;
        }

        m = v & VarBitsLens[size];
        dest->bits |= m << dest->bitlen;
        dest->bitlen += size;
        if (dest->bitlen > 31)
        {
            *dest->cur = dest->bits;
            dest->cur++;
            dest->bitlen -= 32;
            if (dest->bitlen)
                dest->bits = m >> (size - dest->bitlen);
            else
                dest->bits = 0;
        }
    }
}
