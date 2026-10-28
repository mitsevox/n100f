#include <stdlib.h>
#include "bink/bitplane.h"
#include "bink/dct.h"
#include "bink/varbits.h"

void* memset(void* dst, int val, U32 n);

#define VARBITSGET(vb, v, n)                                                \
    if ((vb).bitlen >= (n))                                                 \
    {                                                                       \
        v = (vb).bits & (0xFFFFFFFF >> (32 - (n)));                         \
        (vb).bits >>= (n);                                                  \
        (vb).bitlen -= (n);                                                 \
    }                                                                       \
    else                                                                    \
    {                                                                       \
        U32 _w = *(vb).cur;                                                 \
        v = ((vb).bits | (_w << (vb).bitlen)) & (0xFFFFFFFF >> (32 - (n))); \
        (vb).bits = _w >> ((n) - (vb).bitlen);                              \
        (vb).bitlen = 32 + (vb).bitlen - (n);                               \
        (vb).cur++;                                                         \
    }

#define VARBITSGET1(vb, t)                                                                   \
    ((vb).bitlen ? (t = (vb).bits & 1, (vb).bitlen--, (vb).bits >>= 1, t)                    \
                 : (t = *(vb).cur, (vb).cur++, (vb).bits = t >> 1, (vb).bitlen = 31, t & 1))

#define CNTLZW(x) ({ U32 _r; __asm__("cntlzw %0,%1" : "=r"(_r) : "r"(x)); _r; })

#define LENQUAD(CMP, ADD)                       \
    if ((U8)(*lp + ((x << 10) + 0x300)) != CMP) \
        *--start = *lp + ((x << 10) + 0x300);   \
    else                                        \
        count += ADD;

#define LENPASS(CMP, ADD)                                                     \
    p = start;                                                                \
    while (p < end)                                                           \
    {                                                                         \
        U16 x = *p;                                                           \
                                                                              \
        if (x)                                                                \
        {                                                                     \
            count++;                                                          \
            if ((U8)x == CMP)                                                 \
            {                                                                 \
                switch (x & 0x300)                                            \
                {                                                             \
                case 0x000:                                                   \
                    x >>= 10;                                                 \
                    *p = len16[x >> 4] + (((x + 4) << 10) + 0x100);           \
                    goto quad##ADD;                                           \
                case 0x100:                                                   \
                    x >>= 10;                                                 \
                    *p = len4[(x >> 2) - 1] + ((x << 10) + 0x200);            \
                    *end++ = len4[x >> 2] + (((x + 4) << 10) + 0x200);        \
                    *end++ = len4[(x >> 2) + 1] + (((x + 8) << 10) + 0x200);  \
                    *end++ = len4[(x >> 2) + 2] + (((x + 12) << 10) + 0x200); \
                    continue;                                                 \
                case 0x200:                                                   \
                    *p = 0;                                                   \
                    x >>= 10;                                                 \
                    p++;                                                      \
                quad##ADD:                                                    \
                    count += 4;                                               \
                    lp = &len[x];                                             \
                    LENQUAD(CMP, ADD)                                         \
                    x++;                                                      \
                    lp++;                                                     \
                    LENQUAD(CMP, ADD)                                         \
                    x++;                                                      \
                    lp++;                                                     \
                    LENQUAD(CMP, ADD)                                         \
                    x++;                                                      \
                    lp++;                                                     \
                    LENQUAD(CMP, ADD)                                         \
                    continue;                                                 \
                case 0x300:                                                   \
                    *p = 0;                                                   \
                    count += ADD;                                             \
                    break;                                                    \
                }                                                             \
            }                                                                 \
        }                                                                     \
        p++;                                                                  \
    }

U32 LenBPLossless(S16* coefs)
{
    U16 buf[132];
    U8 len[64];
    U8 len4[16];
    U8 len16[4];
    U16* start;
    U16* end;
    U16* p;
    U32 count;
    S32 i;
    U8 max;
    U8 n;
    U8 l;
    U8* lp;

    max = 0;
    for (i = 1; i < 64; i++)
    {
        U8 b = 32 - CNTLZW((U32)(U16)abs(coefs[zigzag[i]]));

        if (b > max)
            max = b;
        len[i] = b;
    }

    for (i = 4; i < 64; i += 4)
    {
        l = len[i];
        if (l < len[i + 1])
            l = len[i + 1];
        if (l < len[i + 2])
            l = len[i + 2];
        if (l < len[i + 3])
            l = len[i + 3];
        len4[i / 4 - 1] = l;
    }

    l = len4[1];
    if (l < len4[2])
        l = len4[2];
    if (l < len4[3])
        l = len4[3];
    if (l < len4[4])
        l = len4[4];
    len16[0] = l;
    l = len4[6];
    if (l < len4[7])
        l = len4[7];
    if (l < len4[8])
        l = len4[8];
    if (l < len4[9])
        l = len4[9];
    len16[1] = l;
    l = len4[11];
    if (l < len4[12])
        l = len4[12];
    if (l < len4[13])
        l = len4[13];
    if (l < len4[14])
        l = len4[14];
    len16[2] = l;

    count = 4;
    start = buf + 68;
    start[0] = ((len16[0] > len4[0]) ? len16[0] : len4[0]) | 0x1000;
    start[1] = ((len16[1] > len4[5]) ? len16[1] : len4[5]) | 0x6000;
    start[2] = ((len16[2] > len4[10]) ? len16[2] : len4[10]) | 0xB000;
    start[3] = len[1] + 0x700;
    start[4] = len[2] + 0xB00;
    start[5] = len[3] + 0xF00;
    end = start + 6;

    for (n = max; n > 1; n--)
    {
        LENPASS(n, n)
    }

    if (max)
    {
        LENPASS(1, 1)
    }

    return count;
}

#define VARBITSPUT(vb, v, n)                            \
    {                                                   \
        U32 _n = (n);                                   \
        U32 _m = (v) & VarBitsLens[_n];                 \
        (vb)->bits |= _m << (vb)->bitlen;               \
        (vb)->bitlen += _n;                             \
        if ((vb)->bitlen > 31)                          \
        {                                               \
            *(vb)->cur = (vb)->bits;                    \
            (vb)->cur++;                                \
            (vb)->bitlen -= 32;                         \
            if ((vb)->bitlen)                           \
                (vb)->bits = _m >> (_n - (vb)->bitlen); \
            else                                        \
                (vb)->bits = 0;                         \
        }                                               \
    }

#define VARBITSPUT1(vb, b)                   \
    {                                        \
        if (b)                               \
            (vb)->bits |= 1 << (vb)->bitlen; \
        (vb)->bitlen++;                      \
        if ((vb)->bitlen == 32)              \
        {                                    \
            *(vb)->cur = (vb)->bits;         \
            (vb)->bitlen = 0;                \
            (vb)->bits = 0;                  \
            (vb)->cur++;                     \
        }                                    \
    }

#define WRITECOEF()                \
    VARBITSPUT(vb, mag[x], n - 1); \
    VARBITSPUT1(vb, zz[x] < 0);

#define WRITEQUAD()                           \
    VARBITSPUT1(vb, *lp != n);                \
    if ((U8)(*lp + ((x << 10) + 0x300)) != n) \
        *--start = *lp + ((x << 10) + 0x300); \
    else                                      \
    {                                         \
        WRITECOEF();                          \
    }

void WriteBPLossless(VARBITS* vb, S16* coefs)
{
    U16 buf[132];
    U8 len[64];
    U8 len4[16];
    U16 mag[64];
    S16 zz[64];
    U8 len16[4];
    U16* start;
    U16* end;
    U16* p;
    S32 i;
    U8 max;
    U8 n;
    U8 l;
    U8* lp;

    for (i = 0; i < 64; i++)
        zz[i] = coefs[zigzag[i]];

    for (i = 0; i < 64; i++)
        mag[i] = abs(zz[i]);

    max = 0;
    for (i = 1; i < 64; i++)
    {
        U8 b = 32 - CNTLZW(mag[i]);

        if (b > max)
            max = b;
        len[i] = b;
    }

    for (i = 4; i < 64; i += 4)
    {
        l = len[i];
        if (l < len[i + 1])
            l = len[i + 1];
        if (l < len[i + 2])
            l = len[i + 2];
        if (l < len[i + 3])
            l = len[i + 3];
        len4[i / 4 - 1] = l;
    }

    l = len4[1];
    if (l < len4[2])
        l = len4[2];
    if (l < len4[3])
        l = len4[3];
    if (l < len4[4])
        l = len4[4];
    len16[0] = l;
    l = len4[6];
    if (l < len4[7])
        l = len4[7];
    if (l < len4[8])
        l = len4[8];
    if (l < len4[9])
        l = len4[9];
    len16[1] = l;
    l = len4[11];
    if (l < len4[12])
        l = len4[12];
    if (l < len4[13])
        l = len4[13];
    if (l < len4[14])
        l = len4[14];
    len16[2] = l;

    VARBITSPUT(vb, max, 4);

    start = buf + 68;
    start[0] = ((len16[0] > len4[0]) ? len16[0] : len4[0]) | 0x1000;
    start[1] = ((len16[1] > len4[5]) ? len16[1] : len4[5]) | 0x6000;
    start[2] = ((len16[2] > len4[10]) ? len16[2] : len4[10]) | 0xB000;
    start[3] = len[1] + 0x700;
    start[4] = len[2] + 0xB00;
    start[5] = len[3] + 0xF00;
    end = start + 6;

    for (n = max; n; n--)
    {
        p = start;
        while (p < end)
        {
            U16 x = *p;

            if (x)
            {
                VARBITSPUT1(vb, (U8)x == n);
                if ((U8)x == n)
                {
                    switch (x & 0x300)
                    {
                    case 0x000:
                        x >>= 10;
                        *p = len16[x >> 4] + (((x + 4) << 10) + 0x100);
                        goto quad;
                    case 0x100:
                        x >>= 10;
                        *p = len4[(x >> 2) - 1] + ((x << 10) + 0x200);
                        *end++ = len4[x >> 2] + (((x + 4) << 10) + 0x200);
                        *end++ = len4[(x >> 2) + 1] + (((x + 8) << 10) + 0x200);
                        *end++ = len4[(x >> 2) + 2] + (((x + 12) << 10) + 0x200);
                        continue;
                    case 0x200:
                        *p = 0;
                        x >>= 10;
                        p++;
                    quad:
                        lp = &len[x];
                        WRITEQUAD();
                        x++;
                        lp++;
                        WRITEQUAD();
                        x++;
                        lp++;
                        WRITEQUAD();
                        x++;
                        lp++;
                        WRITEQUAD();
                        continue;
                    case 0x300:
                        x >>= 10;
                        WRITECOEF();
                        *p = 0;
                        break;
                    }
                }
            }
            p++;
        }
    }
}

#define LOSSLESSCOEF(x)                               \
    if (VARBITSGET1(vb, t))                           \
    {                                                 \
        *--start = (x << 2) + 3;                      \
    }                                                 \
    else                                              \
    {                                                 \
        VARBITSGET(vb, c, n);                         \
        c |= bit;                                     \
        ((S16*)tmp)[x] = VARBITSGET1(vb, t) ? -c : c; \
    }

#define LASTCOEF(x)                                   \
    if (VARBITSGET1(vb, t))                           \
    {                                                 \
        *--start = (x << 2) + 3;                      \
    }                                                 \
    else                                              \
    {                                                 \
        ((S16*)tmp)[x] = VARBITSGET1(vb, t) ? -1 : 1; \
    }

#define LOSSLESSPASS(COEF)                    \
    p = start;                                \
    while (p < end)                           \
    {                                         \
        U8 x = *p;                            \
                                              \
        if (x && VARBITSGET1(vb, t))          \
        {                                     \
            switch (x & 3)                    \
            {                                 \
            case 0:                           \
                x >>= 2;                      \
                *p = ((x + 4) << 2) + 1;      \
                goto coefs##COEF;             \
            case 1:                           \
                x >>= 2;                      \
                *p = (x << 2) + 2;            \
                *end++ = ((x + 4) << 2) + 2;  \
                *end++ = ((x + 8) << 2) + 2;  \
                *end++ = ((x + 12) << 2) + 2; \
                continue;                     \
            case 2:                           \
                *p = 0;                       \
                x >>= 2;                      \
                p++;                          \
            coefs##COEF:                      \
                COEF(x);                      \
                x++;                          \
                COEF(x);                      \
                x++;                          \
                COEF(x);                      \
                x++;                          \
                COEF(x);                      \
                continue;                     \
            case 3:                           \
                x >>= 2;                      \
                COEF##3(x);                   \
                *p = 0;                       \
                break;                        \
            }                                 \
        }                                     \
        p++;                                  \
    }

#define LOSSLESSCOEF3(x)                          \
    VARBITSGET(vb, c, n);                         \
    c |= bit;                                     \
    ((S16*)tmp)[x] = VARBITSGET1(vb, t) ? -c : c;

#define LASTCOEF3(x) ((S16*)tmp)[x] = VARBITSGET1(vb, t) ? -1 : 1;

void ReadBPLossless(S16* coefs, VARBITS* vbp)
{
    U8 buf[136];
    U32 tmp[32];
    VARBITS vb;
    U8* start;
    U8* end;
    U8* p;
    U32 t;
    S16 c;
    S16 bit;
    U8 n;
    U8 v;

    vb = *vbp;
    ((S16*)tmp)[1] = 0;
    memset(&tmp[1], 0, 124);
    VARBITSGET(vb, v, 4);
    start = buf + 68;
    start[0] = 16;
    start[1] = 96;
    start[2] = 176;
    start[3] = 7;
    start[4] = 11;
    start[5] = 15;
    end = start + 6;
    bit = 1 << (v - 1);
    n = v;

    while (n > 1)
    {
        n--;
        LOSSLESSPASS(LOSSLESSCOEF)
        bit >>= 1;
    }

    if (v)
    {
        LOSSLESSPASS(LASTCOEF)
    }

    *vbp = vb;
    coefs[1] = ((S16*)tmp)[1];
    ((U32*)coefs)[1] = tmp[2];
    ((U32*)coefs)[2] = tmp[4];
    ((U32*)coefs)[3] = tmp[6];
    ((U32*)coefs)[4] = tmp[1];
    ((U32*)coefs)[5] = tmp[3];
    ((U32*)coefs)[6] = tmp[5];
    ((U32*)coefs)[7] = tmp[7];
    ((U32*)coefs)[8] = tmp[12];
    ((U32*)coefs)[9] = tmp[22];
    ((U32*)coefs)[10] = tmp[8];
    ((U32*)coefs)[11] = tmp[10];
    ((U32*)coefs)[12] = tmp[13];
    ((U32*)coefs)[13] = tmp[23];
    ((U32*)coefs)[14] = tmp[9];
    ((U32*)coefs)[15] = tmp[11];
    ((U32*)coefs)[16] = tmp[14];
    ((U32*)coefs)[17] = tmp[16];
    ((U32*)coefs)[18] = tmp[24];
    ((U32*)coefs)[19] = tmp[26];
    ((U32*)coefs)[20] = tmp[15];
    ((U32*)coefs)[21] = tmp[17];
    ((U32*)coefs)[22] = tmp[25];
    ((U32*)coefs)[23] = tmp[27];
    ((U32*)coefs)[24] = tmp[18];
    ((U32*)coefs)[25] = tmp[20];
    ((U32*)coefs)[26] = tmp[28];
    ((U32*)coefs)[27] = tmp[30];
    ((U32*)coefs)[28] = tmp[19];
    ((U32*)coefs)[29] = tmp[21];
    ((U32*)coefs)[30] = tmp[29];
    ((U32*)coefs)[31] = tmp[31];
}

#define LOSSYWRITECOEF()        \
    sig[nsig++] = mag[x];       \
    VARBITSPUT1(vb, zz[x] < 0);

#define LOSSYWRITEQUAD()                      \
    VARBITSPUT1(vb, *lp != n);                \
    if ((U8)(*lp + ((x << 10) + 0x300)) != n) \
        *--start = *lp + ((x << 10) + 0x300); \
    else                                      \
    {                                         \
        LOSSYWRITECOEF();                     \
    }

U32 WriteBPLossy(VARBITS* vb, S8* coefs)
{
    U16 buf[132];
    U8 len[64];
    U8 len4[16];
    S8 zz[64];
    U8 mag[64];
    U8 sig[64];
    U8 len16[4];
    U16* start;
    U16* end;
    U16* p;
    S32 i;
    S32 nsig;
    S16 bit;
    U8 max;
    U8 n;
    U8 l;
    U8* lp;

    for (i = 0; i < 64; i++)
        zz[i] = coefs[zigzag[i]];

    for (i = 0; i < 64; i++)
        mag[i] = abs(zz[i]);

    max = 0;
    for (i = 0; i < 64; i++)
    {
        U8 b = 32 - CNTLZW(mag[i]);

        if (b > max)
            max = b;
        len[i] = b;
    }

    if (max == 0)
        return 0;

    for (i = 0; i < 64; i += 4)
    {
        l = len[i];
        if (l < len[i + 1])
            l = len[i + 1];
        if (l < len[i + 2])
            l = len[i + 2];
        if (l < len[i + 3])
            l = len[i + 3];
        len4[i / 4] = l;
    }

    l = len4[2];
    if (l < len4[3])
        l = len4[3];
    if (l < len4[4])
        l = len4[4];
    if (l < len4[5])
        l = len4[5];
    len16[0] = l;
    l = len4[7];
    if (l < len4[8])
        l = len4[8];
    if (l < len4[9])
        l = len4[9];
    if (l < len4[10])
        l = len4[10];
    len16[1] = l;
    l = len4[12];
    if (l < len4[13])
        l = len4[13];
    if (l < len4[14])
        l = len4[14];
    if (l < len4[15])
        l = len4[15];
    len16[2] = l;

    VARBITSPUT(vb, max - 1, 3);

    start = buf + 68;
    start[0] = ((len16[0] > len4[1]) ? len16[0] : len4[1]) | 0x1000;
    start[1] = ((len16[1] > len4[6]) ? len16[1] : len4[6]) | 0x6000;
    start[2] = ((len16[2] > len4[11]) ? len16[2] : len4[11]) | 0xB000;
    start[3] = len4[0] + 0x200;
    end = start + 4;
    nsig = 0;
    bit = 1 << (max - 1);

    for (n = max; n; n--)
    {
        i = 0;
        while (i < nsig)
            VARBITSPUT1(vb, sig[i++] & bit);

        p = start;
        while (p < end)
        {
            U16 x = *p;

            if (x)
            {
                VARBITSPUT1(vb, (U8)x == n);
                if ((U8)x == n)
                {
                    switch (x & 0x300)
                    {
                    case 0x000:
                        x >>= 10;
                        *p = len16[x >> 4] + (((x + 4) << 10) + 0x100);
                        goto quad;
                    case 0x100:
                        x >>= 10;
                        *p = len4[x >> 2] + ((x << 10) + 0x200);
                        *end++ = len4[(x >> 2) + 1] + (((x + 4) << 10) + 0x200);
                        *end++ = len4[(x >> 2) + 2] + (((x + 8) << 10) + 0x200);
                        *end++ = len4[(x >> 2) + 3] + (((x + 12) << 10) + 0x200);
                        continue;
                    case 0x200:
                        *p = 0;
                        x >>= 10;
                        p++;
                    quad:
                        lp = &len[x];
                        LOSSYWRITEQUAD();
                        x++;
                        lp++;
                        LOSSYWRITEQUAD();
                        x++;
                        lp++;
                        LOSSYWRITEQUAD();
                        x++;
                        lp++;
                        LOSSYWRITEQUAD();
                        continue;
                    case 0x300:
                        x >>= 10;
                        LOSSYWRITECOEF();
                        *p = 0;
                        break;
                    }
                }
            }
            p++;
        }

        bit >>= 1;
    }

    return 1;
}

#define LOSSYCOEF()                               \
    if (VARBITSGET1(vb, t))                       \
    {                                             \
        *--start = (v << 2) + 3;                  \
    }                                             \
    else                                          \
    {                                             \
        sig[nsig++] = v;                          \
        out[v] = VARBITSGET1(vb, t) ? -bit : bit; \
        if (n++ == count)                         \
            goto done;                            \
    }

static void readlossy(S8* out, VARBITS* vbp, S32 count)
{
    U8 buf[136];
    U8 sig[64];
    VARBITS vb;
    U8* start;
    U8* end;
    U8* p;
    U8 v;
    U32 t;
    S32 i;
    S32 nsig;
    S32 n;
    S8 bit;
    U8 planes;

    vb = *vbp;
    n = 0;
    memset(out, 0, 64);
    VARBITSGET(vb, v, 3);
    v++;
    start = buf + 68;
    start[0] = 16;
    start[1] = 96;
    start[2] = 176;
    start[3] = 2;
    bit = 1 << (v - 1);
    end = start + 4;
    nsig = 0;

    for (planes = v; planes; planes--)
    {
        for (i = 0; i < nsig; i++)
        {
            if (VARBITSGET1(vb, t))
            {
                out[sig[i]] += (out[sig[i]] < 0) ? -bit : bit;
                if (n++ == count)
                    goto done;
            }
        }

        p = start;
        while (p < end)
        {
            v = *p;
            if (v && VARBITSGET1(vb, t))
            {
                switch (v & 3)
                {
                case 0:
                    v >>= 2;
                    *p = ((v + 4) << 2) + 1;
                    goto coefs;
                case 1:
                    v >>= 2;
                    *p = (v << 2) + 2;
                    *end++ = ((v + 4) << 2) + 2;
                    *end++ = ((v + 8) << 2) + 2;
                    *end++ = ((v + 12) << 2) + 2;
                    continue;
                case 2:
                    *p = 0;
                    v >>= 2;
                    p++;
                coefs:
                    LOSSYCOEF();
                    v++;
                    LOSSYCOEF();
                    v++;
                    LOSSYCOEF();
                    v++;
                    LOSSYCOEF();
                    continue;
                case 3:
                    v >>= 2;
                    sig[nsig++] = v;
                    out[v] = VARBITSGET1(vb, t) ? -bit : bit;
                    if (n++ == count)
                        goto done;
                    *p = 0;
                    break;
                }
            }
            p++;
        }

        bit >>= 1;
    }

done:
    *vbp = vb;
}

void ReadBPLossy(S8* out, VARBITS* vb, U32 count)
{
    U16 tmp[32];

    readlossy((S8*)tmp, vb, count);
    ((U16*)out)[0] = tmp[0];
    ((U16*)out)[1] = tmp[2];
    ((U16*)out)[2] = tmp[4];
    ((U16*)out)[3] = tmp[6];
    ((U16*)out)[4] = tmp[1];
    ((U16*)out)[5] = tmp[3];
    ((U16*)out)[6] = tmp[5];
    ((U16*)out)[7] = tmp[7];
    ((U16*)out)[8] = tmp[12];
    ((U16*)out)[9] = tmp[22];
    ((U16*)out)[10] = tmp[8];
    ((U16*)out)[11] = tmp[10];
    ((U16*)out)[12] = tmp[13];
    ((U16*)out)[13] = tmp[23];
    ((U16*)out)[14] = tmp[9];
    ((U16*)out)[15] = tmp[11];
    ((U16*)out)[16] = tmp[14];
    ((U16*)out)[17] = tmp[16];
    ((U16*)out)[18] = tmp[24];
    ((U16*)out)[19] = tmp[26];
    ((U16*)out)[20] = tmp[15];
    ((U16*)out)[21] = tmp[17];
    ((U16*)out)[22] = tmp[25];
    ((U16*)out)[23] = tmp[27];
    ((U16*)out)[24] = tmp[18];
    ((U16*)out)[25] = tmp[20];
    ((U16*)out)[26] = tmp[28];
    ((U16*)out)[27] = tmp[30];
    ((U16*)out)[28] = tmp[19];
    ((U16*)out)[29] = tmp[21];
    ((U16*)out)[30] = tmp[29];
    ((U16*)out)[31] = tmp[31];
}

void ReadBPLossyWithMotion(U8* dest, U32 pitch, VARBITS* vb, U32 q, U8* motion)
{
    U8 tmp[64];
    U8* d = dest;

    readlossy((S8*)tmp, vb, q);
    d[0] = tmp[0] + motion[0];
    d[1] = tmp[1] + motion[1];
    d[2] = tmp[4] + motion[2];
    d[3] = tmp[5] + motion[3];
    d[4] = tmp[8] + motion[4];
    d[5] = tmp[9] + motion[5];
    d[6] = tmp[12] + motion[6];
    d[7] = tmp[13] + motion[7];
    d += pitch;
    d[0] = tmp[2] + motion[8];
    d[1] = tmp[3] + motion[9];
    d[2] = tmp[6] + motion[10];
    d[3] = tmp[7] + motion[11];
    d[4] = tmp[10] + motion[12];
    d[5] = tmp[11] + motion[13];
    d[6] = tmp[14] + motion[14];
    d[7] = tmp[15] + motion[15];
    d += pitch;
    d[0] = tmp[24] + motion[16];
    d[1] = tmp[25] + motion[17];
    d[2] = tmp[44] + motion[18];
    d[3] = tmp[45] + motion[19];
    d[4] = tmp[16] + motion[20];
    d[5] = tmp[17] + motion[21];
    d[6] = tmp[20] + motion[22];
    d[7] = tmp[21] + motion[23];
    d += pitch;
    d[0] = tmp[26] + motion[24];
    d[1] = tmp[27] + motion[25];
    d[2] = tmp[46] + motion[26];
    d[3] = tmp[47] + motion[27];
    d[4] = tmp[18] + motion[28];
    d[5] = tmp[19] + motion[29];
    d[6] = tmp[22] + motion[30];
    d[7] = tmp[23] + motion[31];
    d += pitch;
    d[0] = tmp[28] + motion[32];
    d[1] = tmp[29] + motion[33];
    d[2] = tmp[32] + motion[34];
    d[3] = tmp[33] + motion[35];
    d[4] = tmp[48] + motion[36];
    d[5] = tmp[49] + motion[37];
    d[6] = tmp[52] + motion[38];
    d[7] = tmp[53] + motion[39];
    d += pitch;
    d[0] = tmp[30] + motion[40];
    d[1] = tmp[31] + motion[41];
    d[2] = tmp[34] + motion[42];
    d[3] = tmp[35] + motion[43];
    d[4] = tmp[50] + motion[44];
    d[5] = tmp[51] + motion[45];
    d[6] = tmp[54] + motion[46];
    d[7] = tmp[55] + motion[47];
    d += pitch;
    d[0] = tmp[36] + motion[48];
    d[1] = tmp[37] + motion[49];
    d[2] = tmp[40] + motion[50];
    d[3] = tmp[41] + motion[51];
    d[4] = tmp[56] + motion[52];
    d[5] = tmp[57] + motion[53];
    d[6] = tmp[60] + motion[54];
    d[7] = tmp[61] + motion[55];
    d += pitch;
    d[0] = tmp[38] + motion[56];
    d[1] = tmp[39] + motion[57];
    d[2] = tmp[42] + motion[58];
    d[3] = tmp[43] + motion[59];
    d[4] = tmp[58] + motion[60];
    d[5] = tmp[59] + motion[61];
    d[6] = tmp[62] + motion[62];
    d[7] = tmp[63] + motion[63];
}
