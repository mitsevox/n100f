#include <math.h>
#include "bink/fft.h"
#include "bink/popmal.h"
#include "bink/radmem.h"
#include "bink/varbits.h"

typedef struct BinkAudioDecomp
{
    U32 size;
    F32 scale;
    U32 total;
    U32 overlap;
    U32 chans;
    S16* samples;
    U32 first;
    U32 numbands;
    U32* bands;
    int* ip;
    F32* w;
    S16* prev;
    U32 flags;
} BinkAudioDecomp;

static U8 rlelens[16] = { 2, 3, 4, 5, 6, 8, 9, 10, 11, 12, 13, 14, 15, 16, 32, 64 };

static U32 bandtopfreq[25] = {
    0,    100,  200,  300,  400,  510,  630,  770,  920,  1080, 1270,  1480,  1720,
    2000, 2320, 2700, 3150, 3700, 4400, 5300, 6400, 7700, 9500, 12000, 15500,
};

static F64 invertbins[25] = {
    1.0 / 8388608.0, 1.0 / 4194304.0, 1.0 / 2097152.0, 1.0 / 1048576.0, 1.0 / 524288.0,
    1.0 / 262144.0,  1.0 / 131072.0,  1.0 / 65536.0,   1.0 / 32768.0,   1.0 / 16384.0,
    1.0 / 8192.0,    1.0 / 4096.0,    1.0 / 2048.0,    1.0 / 1024.0,    1.0 / 512.0,
    1.0 / 256.0,     1.0 / 128.0,     1.0 / 64.0,      1.0 / 32.0,      1.0 / 16.0,
    1.0 / 8.0,       1.0 / 4.0,       1.0 / 2.0,       1.0,
};

static inline F32 __frsqrte(F32 x)
{
    F32 r;
    __asm__("frsqrte %0,%1" : "=f"(r) : "f"(x));
    return r;
}

static inline F32 fastsqrt(F32 x)
{
    if (x > 0.0f)
    {
        F64 guess = __frsqrte(x);
        guess = 0.5 * guess * (3.0 - guess * guess * x);
        guess = 0.5 * guess * (3.0 - guess * guess * x);
        guess = 0.5 * guess * (3.0 - guess * guess * x);
        return (F32)(guess * x);
    }
    return x;
}

#define VARBITSGET(vb, v, n)                                              \
    if ((vb).bitlen >= (n))                                               \
    {                                                                     \
        v = (vb).bits & (0xFFFFFFFF >> (32 - (n)));                       \
        (vb).bitlen -= (n);                                               \
        (vb).bits >>= (n);                                                \
    }                                                                     \
    else                                                                  \
    {                                                                     \
        U32 _w = *(vb).cur;                                               \
        v = ((vb).bits | (_w << (vb).bitlen)) & (0xFFFFFFFF >> (32 - (n))); \
        (vb).bits = _w >> ((n) - (vb).bitlen);                            \
        (vb).bitlen = 32 + (vb).bitlen - (n);                             \
        (vb).cur++;                                                       \
    }

#define VARBITSGET1(vb, t)                                                  \
    ((vb).bitlen ? (t = (vb).bits & 1, (vb).bitlen--, (vb).bits >>= 1, t) \
                 : (t = *(vb).cur, (vb).cur++, (vb).bits = t >> 1, (vb).bitlen = 31, t & 1))

#define VARBITSPOS(vb) ((((U8*)(vb).cur - (U8*)(vb).start) * 8) - (vb).bitlen)

#define CLAMP16(v) (((v) >= 32767) ? 32767 : ((v) <= -32768) ? -32768 : (S16)(v))

static F32 fxptof(U32 v)
{
    F32 f = (F32)(((v & ~0x10000000) >> 5) * invertbins[v & 31]);
    if (v & 0x10000000)
        f = -f;
    return f;
}

static void quanttos16s(S16* out, F32* in, F32 scale, U32 n)
{
    for (; n; n--)
    {
        S32 v = (S32)(*in * scale);
        *out++ = CLAMP16(v);
        in++;
    }
}

static void quanttos16chans2(S16* out, F32* in, F32 scale, U32 n)
{
    U32 i = n;
    while (i--)
    {
        S32 v = (S32)(in[0] * scale);
        *out++ = CLAMP16(v);
        v = (S32)(in[n] * scale);
        *out++ = CLAMP16(v);
        in++;
    }
}

static void read_rle_samples(F32* coeffs, U32 n, VARBITS* vb, F32* quant, U32* bands)
{
    F32 q = 0.0f;
    U32 k = 0;
    U32 i, j, v, t, width;
    F32* out;

    while (bands[k] * 2 < 2)
        q = quant[k++];

    i = 2;
    out = coeffs + 2;
    while (i < n)
    {
        if (VARBITSGET1(*vb, t))
        {
            VARBITSGET(*vb, j, 4);
            j = i + rlelens[j] * 8;
        }
        else
            j = i + 8;

        if (j > n)
            j = n;

        VARBITSGET(*vb, width, 4);
        if (width == 0)
        {
            t = j - i;
            memset(out, 0, t * 4);
            out += t;
            i = j;
            while (i > bands[k] * 2)
                q = quant[k++];
        }
        else
        {
            for (; i < j; i++)
            {
                if (i == bands[k] * 2)
                    q = quant[k++];

                VARBITSGET(*vb, v, width);
                if (v)
                {
                    S32 s = -(S32)VARBITSGET1(*vb, t);
                    v = (v ^ s) - s;
                    *out++ = (F32)(S32)v * q;
                }
                else
                    *out++ = 0.0f;
            }
        }
    }
}

static U32 Unquant(U32 n, F32 scale, U32 chans, U32 flags, int* ip, F32* w, S16* out, void* src, U32 numbands,
                   U32* bands)
{
    F32 quant[26];
    VARBITS vb;
    F32 coeffs[4096];
    F32* c;
    U32 ch, i, v;

    vb.start = src;
    vb.cur = (U32*)src;
    vb.bitlen = 0;
    vb.bits = 0;
    if (flags & 1)
    {
        VARBITSGET(vb, v, 2);
    }

    c = coeffs;
    for (ch = 0; ch < chans; ch++)
    {
        VARBITSGET(vb, i, 29);
        c[0] = fxptof(i);
        VARBITSGET(vb, i, 29);
        c[1] = fxptof(i);

        for (i = 0; i < numbands; i++)
        {
            VARBITSGET(vb, v, 8);
            quant[i] = (F32)pow(10.0, (F32)(S32)v * 0.664f * 0.1f);
        }

        read_rle_samples(c, n, &vb, quant, bands);

        if (flags & 1)
            ddct(n, 1, c, ip, w);
        else
            rdft(n, -1, c, ip, w);

        c += n;
    }

    if (chans == 1)
        quanttos16s(out, coeffs, scale, n);
    else
        quanttos16chans2(out, coeffs, scale, n);

    vb.bitlen = 0;
    return VARBITSPOS(vb) / 8;
}

void* BinkAudioDecompressOpen(U32 rate, U32 chans, U32 flags)
{
    BinkAudioDecomp* ac;
    U32* bands;
    int* ip;
    F32* w;
    S16* prev;
    S16* samples;
    U32 size, total, nyq, half, nb, i;

    if (rate > 44099)
        size = 2048;
    else if (rate > 22049)
        size = 1024;
    else
        size = 512;

    total = size * chans * 2;
    if (!(flags & 1))
    {
        rate *= chans;
        size *= chans;
        chans = 1;
    }

    nyq = (rate + 1) >> 1;
    half = size >> 1;

    for (i = 0; i < 25 && bandtopfreq[i] < nyq; i++)
        ;
    nb = i;

    pushmalloc((void**)&bands, (nb + 1) * 4);
    pushmalloc((void**)&ip, (2 + (U32)fastsqrt((F32)half)) * 4);
    if (flags & 1)
        pushmalloc((void**)&w, size * 5);
    else
        pushmalloc((void**)&w, half * 4 - 1);
    pushmalloc((void**)&prev, total >> 1);
    pushmalloc((void**)&samples, total);

    ac = popmalloc(sizeof(BinkAudioDecomp));
    if (ac == 0)
        return 0;

    memset(ac, 0, sizeof(BinkAudioDecomp));
    ac->bands = bands;
    ac->ip = ip;
    ac->w = w;
    ac->prev = prev;
    ac->samples = samples;
    ac->flags = flags;
    ac->chans = chans;
    ac->numbands = nb;
    ac->size = size;
    ac->total = total;
    ac->overlap = total >> 4;
    ac->scale = 2.0f / fastsqrt((F32)size);

    for (i = 0; i < nb; i++)
    {
        ac->bands[i] = bandtopfreq[i] * half / nyq;
        if (ac->bands[i] == 0)
            ac->bands[i] = 1;
    }
    ac->bands[i] = half;
    ac->ip[0] = 0;
    ac->first = 1;

    return ac;
}

void BinkAudioDecompress(BinkAudioDecomp* ac, S16** outp, U32* outlen, void* src, void** srcend)
{
    U32 used = Unquant(ac->size, ac->scale, ac->chans, ac->flags, ac->ip, ac->w, ac->samples, src, ac->numbands,
                       ac->bands);

    if (ac->first)
        ac->first = 0;
    else
    {
        U32 n = ac->overlap >> 1;
        U32 i;

        for (i = 0; i < n; i++)
            ac->samples[i] = (ac->samples[i] * i + ac->prev[i] * (n - i)) / n;
    }

    memcpy(ac->prev, (U8*)ac->samples + (ac->total - ac->overlap), ac->overlap);

    if (outlen)
        *outlen = ac->total - ac->overlap;
    if (outp)
        *outp = ac->samples;
    if (srcend)
        *srcend = (U8*)src + used;
}

void BinkAudioDecompressClose(void* ac)
{
    radfree(ac);
}
