#include <string.h>
#include "bink/binkngc.h"
#include "bink/radmem.h"
#include "bink/yuv.h"
#include "bink/yuvstate.h"

typedef void (*BLITFN)(long n);
typedef void (*ROWFN)(unsigned long x, unsigned long count);
typedef long (*COLFN)(unsigned long count, long x);

typedef struct BLITALIGN
{
    U32 align;
    BLITFN fn;
} BLITALIGN;

typedef struct BLITS
{
    U32 bpp;
    BLITALIGN even;
    BLITALIGN odd;
    BLITALIGN m;
    BLITALIGN x2even;
    BLITALIGN x2odd;
    BLITALIGN mx2;
    ROWFN row;
    COLFN col;
    ROWFN rowm;
    COLFN colm;
    ROWFN row2w;
    COLFN col2w;
    ROWFN rowm2w;
    COLFN colm2w;
    ROWFN row2h;
    COLFN col2h;
    ROWFN rowm2h;
    COLFN colm2h;
    ROWFN row2wh;
    COLFN col2wh;
    ROWFN rowm2wh;
    COLFN colm2wh;
} BLITS;

#define ALIGN32 __attribute__((aligned(32)))

extern "C" {
void YUV_32_4x2_even(long n);
void YUV_32_4x2_odd(long n);
void YUV_32m_4x2(long n);
void YUV_32x2_4x2_even(long n);
void YUV_32x2_4x2_odd(long n);
void YUV_32mx2_4x2(long n);
void YUV_32a_4x2_even(long n);
void YUV_32a_4x2_odd(long n);
void YUV_32am_4x2(long n);
void YUV_32ax2_4x2_even(long n);
void YUV_32ax2_4x2_odd(long n);
void YUV_32amx2_4x2(long n);
void YUV_16_4x2_even(long n);
void YUV_16_4x2_odd(long n);
void YUV_16m_4x2(long n);
void YUV_16x2_4x2_even(long n);
void YUV_16x2_4x2_odd(long n);
void YUV_16mx2_4x2(long n);
void YUV_16a4_4x2_even(long n);
void YUV_16a4_4x2_odd(long n);
void YUV_16a4m_4x2(long n);
void YUV_16a4x2_4x2_even(long n);
void YUV_16a4x2_4x2_odd(long n);
void YUV_16a4mx2_4x2(long n);
void YUY2_4x2(long n);
void YUY2_m_4x2(long n);
void YUY2_x2_4x2(long n);
void YUY2_mx2_4x2(long n);
}


extern "C" {
S32 ytable[260] ALIGN32 = { 0 };
S32 ytable_x4[260] ALIGN32 = { 0 };
U32* clamp_ytable[260] ALIGN32 = { 0 };
U32 clamptable[772] ALIGN32 = { 0 };
U32 clamp_a4[260] ALIGN32 = { 0 };
U32 clamp_r[772] ALIGN32 = { 0 };
U32 clamp_g[772] ALIGN32 = { 0 };
U32 clamp_b[772] ALIGN32 = { 0 };
U32 clamp_rh[772] ALIGN32 = { 0 };
U32 clamp_gh[772] ALIGN32 = { 0 };
U32 clamp_bh[772] ALIGN32 = { 0 };
U32 clamp_rr[772] ALIGN32 = { 0 };
U32 clamp_gg[772] ALIGN32 = { 0 };
U32 clamp_bb[772] ALIGN32 = { 0 };
U32 mono16[256] ALIGN32 = { 0 };
U32 mono16x2[256] ALIGN32 = { 0 };
U32 mono32[256] ALIGN32 = { 0 };
YUVSTATE S ALIGN32 = { 0 };
S32 YUVTables[4][256] ALIGN32 = { 0 };
U32 RGBshift[6][2] ALIGN32 = { 0 };
}

static S32 whichyuv = 0;
S32 rgb_layout = -1;
static U32 z2hsize = 0;
static U8* z2hbuf1 = 0;
static U8* z2hbuf2 = 0;
static S32 donetables = 0;

static BLITFN EVEN;
static BLITFN ODD;
static BLITFN EVENx;
static BLITFN ODDx;
static ROWFN dounalignedrow;
static COLFN dounalignedcol;
static U32 align;
static U32 alignm1;
static U32 alignshift;
static U32 testing;

static S32 origYUVTables[4][256];

static void checkzoombufs(unsigned long size)
{
    unsigned long len = (size + 31) & ~31;

    if (len * 2 > z2hsize)
    {
        if (z2hbuf1)
        {
            radfree(z2hbuf1);
            z2hbuf1 = 0;
        }
        z2hbuf1 = (U8*)radmalloc(len * 2);
        z2hsize = len * 2;
    }
    z2hbuf2 = z2hbuf1 + len;
}

#define ZOOM2H(name, FN)                    \
    static void name(long n)                \
    {                                       \
        U8* dst0 = S.dst0;                  \
        U8* dst1 = S.dst1;                  \
        U8* y0 = S.y0;                      \
        U8* y1 = S.y1;                      \
        U8* a0 = S.a0;                      \
        U8* a1 = S.a1;                      \
        U8* u = S.u;                        \
        U8* v = S.v;                        \
        long d;                             \
                                            \
        FN(n);                              \
        S.y0 = y0;                          \
        S.y1 = y1;                          \
        S.a0 = a0;                          \
        S.a1 = a1;                          \
        S.u = u;                            \
        S.v = v;                            \
        d = S.dst1 - dst1;                  \
        S.dst0 = dst0 + S.pitch;            \
        S.dst1 = dst1 + S.pitch;            \
        FN(n);                              \
        S.dst0 = dst0 + d;                  \
        S.dst1 = dst1 + d;                  \
    }

ZOOM2H(zoom2heven, EVENx)
ZOOM2H(zoom2hodd, ODDx)

static void setevenodd(unsigned long flags, unsigned long* pitch, unsigned long width,
                       unsigned long height, BLITS* b, unsigned long* skip)
{
    unsigned long mode;

    testing = 0;
    S.pitch = *pitch;

    if (flags & 0x10000)
    {
        if (whichyuv != 1)
        {
            whichyuv = 1;
            memcpy(YUVTables[3], origYUVTables[0], sizeof(YUVTables[3]));
            memcpy(YUVTables[1], origYUVTables[2], sizeof(YUVTables[1]));
            memcpy(YUVTables[2], origYUVTables[1], sizeof(YUVTables[2]));
            memcpy(YUVTables[0], origYUVTables[3], sizeof(YUVTables[0]));
        }
    }
    else
    {
        if (whichyuv != 0)
        {
            whichyuv = 0;
            memcpy(YUVTables, origYUVTables, sizeof(YUVTables));
        }
    }

    mode = flags & 0x70000000;
    if (mode == 0x10000000)
    {
        checkzoombufs(width * b->bpp);
        *pitch *= 2;
        *skip = *pitch - width * b->bpp;
        if (flags & 0x20000)
        {
            EVEN = zoom2heven;
            ODD = zoom2hodd;
            dounalignedrow = b->rowm2h;
            dounalignedcol = b->colm2h;
            ODDx = EVENx = b->m.fn;
            alignshift = b->m.align;
        }
        else
        {
            EVEN = zoom2heven;
            ODD = zoom2hodd;
            dounalignedrow = b->row2h;
            dounalignedcol = b->col2h;
            EVENx = b->even.fn;
            ODDx = b->odd.fn;
            alignshift = (b->odd.align >= b->even.align) ? b->odd.align : b->even.align;
        }
    }
    else if (mode == 0x30000000 || mode == 0x50000000)
    {
        *skip = *pitch - width * b->bpp * 2;
        if (flags & 0x20000)
        {
            dounalignedrow = b->rowm2w;
            dounalignedcol = b->colm2w;
            ODD = EVEN = b->mx2.fn;
            alignshift = b->mx2.align;
        }
        else
        {
            dounalignedrow = b->row2w;
            dounalignedcol = b->col2w;
            EVEN = b->x2even.fn;
            ODD = b->x2odd.fn;
            alignshift = (b->x2odd.align >= b->x2even.align) ? b->x2odd.align : b->x2even.align;
        }
    }
    else if (mode == 0x40000000)
    {
        checkzoombufs(width * b->bpp * 2);
        *pitch *= 2;
        *skip = *pitch - width * b->bpp * 2;
        if (flags & 0x20000)
        {
            EVEN = zoom2heven;
            ODD = zoom2hodd;
            dounalignedrow = b->rowm2wh;
            dounalignedcol = b->colm2wh;
            ODDx = EVENx = b->mx2.fn;
            alignshift = b->mx2.align;
        }
        else
        {
            EVEN = zoom2heven;
            ODD = zoom2hodd;
            dounalignedrow = b->row2wh;
            dounalignedcol = b->col2wh;
            EVENx = b->x2even.fn;
            ODDx = b->x2odd.fn;
            alignshift = (b->x2odd.align >= b->x2even.align) ? b->x2odd.align : b->x2even.align;
        }
    }
    else
    {
        *skip = *pitch - width * b->bpp;
        if (flags & 0x20000)
        {
            dounalignedrow = b->rowm;
            dounalignedcol = b->colm;
            ODD = EVEN = b->m.fn;
            alignshift = b->m.align;
        }
        else
        {
            dounalignedrow = b->row;
            dounalignedcol = b->col;
            EVEN = b->even.fn;
            ODD = b->odd.fn;
            alignshift = (b->odd.align >= b->even.align) ? b->odd.align : b->even.align;
        }
    }

    align = 1 << alignshift;
    alignm1 = align - 1;
}

#define NEXTROWS(n)                         \
    S.dst0 = S.dst1 + skip;                 \
    S.dst1 = S.dst0 + pitch;               \
    S.y0 = S.y1 + yskip;                    \
    S.y1 = S.y0 + width;                    \
    S.a0 = S.a1 + yskip;                    \
    S.a1 = S.a0 + width;                    \
    S.u += uvskip;                          \
    S.v += uvskip;

static void YUV_blit(void* dest, unsigned long destx, unsigned long desty, unsigned long pitch,
                     void* src, unsigned long srcx, unsigned long srcy, unsigned long srcw,
                     unsigned long srch, unsigned long width, unsigned long height,
                     unsigned long flags, void* alpha, BLITS* b)
{
    unsigned long opitch = pitch;
    unsigned long skip;
    unsigned long yskip, uvskip;
    unsigned long uvw;
    S32 y, last;
    unsigned long lead;

    if ((flags & 0x70000000) == 0x60000000)
    {
        pitch *= 2;
        srcy >>= 1;
        srch >>= 1;
        height >>= 1;
        width *= 2;
    }

    S.base = (U8*)dest;
    if (b->bpp == 2)
    {
        if (((U32)dest & 3) == 2)
        {
            destx++;
            dest = (void*)((U32)dest & ~3);
        }
    }
    else if (b->bpp == 3)
    {
        if ((((U32)dest - 3) & 3) == 0)
        {
            dest = (U8*)dest - 3;
            destx += 1;
        }
        else if ((((U32)dest - 6) & 3) == 0)
        {
            dest = (U8*)dest - 6;
            destx += 2;
        }
        else if ((((U32)dest - 9) & 3) == 0)
        {
            dest = (U8*)dest - 9;
            destx += 3;
        }
    }

    S.dst0 = (U8*)dest + (desty * opitch + destx * b->bpp);

    if ((flags & 0x70000000) == 0x20000000 || (flags & 0x70000000) == 0x50000000)
        pitch *= 2;

    setevenodd(flags, &pitch, srcw, srch, b, &skip);

    S.y0 = (U8*)src + srcx + srcy * width;
    S.y1 = S.y0 + width;
    S.a0 = (U8*)alpha + srcx + srcy * width;
    S.a1 = S.a0 + width;
    S.dst1 = S.dst0 + pitch;

    uvw = width >> 1;
    yskip = width - srcw;
    uvskip = uvw - (srcw >> 1);
    if ((srcw & 1) && (srcx & 1))
        uvskip--;

    if (flags & 0x10000)
    {
        S.u = (U8*)src + width * height;
        S.v = S.u + uvw * (height >> 1);
    }
    else
    {
        S.v = (U8*)src + width * height;
        S.u = S.v + uvw * (height >> 1);
    }
    S.u = S.u + (srcx >> 1) + (srcy >> 1) * uvw;
    S.v = S.v + (srcx >> 1) + (srcy >> 1) * uvw;

    y = srcy;
    last = srcy + srch - 1;

    if ((y & 1) && y <= last)
    {
        dounalignedrow(srcx, srcw);
        y++;
        S.dst0 += skip;
        S.dst1 = S.dst0 + pitch;
        S.y0 += yskip;
        S.y1 = S.y0 + width;
        S.a0 += yskip;
        S.a1 = S.a0 + width;
        S.u += uvskip;
        S.v += uvskip;
    }

    lead = (4 - (destx & 3)) & 3;
    if (lead > srcw)
        lead = srcw;

    while (y < last)
    {
        unsigned long w = srcw;
        long x = srcx;
        unsigned long n, groups;

        if (lead)
        {
            w = srcw - lead;
            x = dounalignedcol(lead, srcx);
        }

        n = w;
        groups = (n & ~alignm1) >> 2;
        n -= groups * 4;
        if (groups)
        {
            if (x & 1)
                ODD(groups);
            else
                EVEN(groups);
        }
        x += groups * 4;
        if (n)
            dounalignedcol(n, x);

        y += 2;
        NEXTROWS(n)
    }

    if (y <= last)
        dounalignedrow(srcx, srcw);
}

#define NEXTBAND(yskip, uvskip, skip)    \
    S.y0 = S.y1 + yskip;                \
    S.y1 = S.y0 + width;                \
    S.a0 = S.a1 + yskip;                \
    S.a1 = S.a0 + width;                \
    S.u += uvskip;                      \
    S.v += uvskip;                      \
    S.dst0 = S.dst1 + skip;             \
    S.dst1 = S.dst0 + pitch;

#define MASKBLOCK(n, yskip, uvskip, skip)   \
    for (i = 0; i < 8; i++)                 \
    {                                       \
        EVEN(n);                            \
        NEXTBAND(yskip, uvskip, skip)       \
    }

#define ADVANCE(w, d)                       \
    S.y0 += w;                              \
    S.y1 += w;                              \
    S.a0 += w;                              \
    S.a1 += w;                              \
    S.u += w / 2;                           \
    S.v += w / 2;                           \
    S.dst0 += d;                            \
    S.dst1 += d;

static void YUV_blit_mask(void* dest, unsigned long destx, unsigned long desty,
                          unsigned long pitch, U8* mask, unsigned long mpitch, void* src,
                          unsigned long srcx, unsigned long srcy, unsigned long srcw,
                          unsigned long srch, unsigned long width, unsigned long height,
                          unsigned long flags, void* alpha, BLITS* b)
{
    unsigned long tmp, skip16, skip32;
    unsigned long mskip;
    unsigned long uvskip32;
    unsigned long ybandskip, uvbandskip, dbandskip;
    unsigned long opitch = pitch;
    unsigned long owidth = width;
    unsigned long osrch = srch;
    unsigned long oheight = height;
    unsigned long xs;
    S32 lasty;
    unsigned long mrow2;
    unsigned long bx;
    unsigned long uvw;
    unsigned long yskip16, yskip32, uvskip16;
    S32 x, y, lastx;
    S32 i;

    if ((flags & 0x70000000) == 0x60000000)
    {
        pitch *= 2;
        srch >>= 1;
        srcy >>= 1;
        height >>= 1;
        width *= 2;
        mrow2 = mpitch;
        mpitch *= 2;
    }
    else
        mrow2 = 0;

    S.base = (U8*)dest;
    S.dst0 = (U8*)dest + (desty * opitch + destx * b->bpp);

    if ((flags & 0x70000000) == 0x20000000 || (flags & 0x70000000) == 0x50000000)
        pitch *= 2;

    mskip = mpitch - (srcw >> 4);
    tmp = pitch;
    setevenodd(flags, &tmp, 16, srch, b, &skip16);
    setevenodd(flags, &pitch, 32, srch, b, &skip32);

    if ((flags & 0x70000000) == 0x30000000 || (flags & 0x70000000) == 0x50000000 ||
        (flags & 0x70000000) == 0x40000000)
        xs = 2;
    else
        xs = 1;

    bx = (b->bpp * xs) << 4;
    uvw = width >> 1;
    S.y0 = (U8*)src + srcx + srcy * width;
    S.y1 = S.y0 + width;
    S.a0 = (U8*)alpha + srcx + srcy * width;
    S.a1 = S.a0 + width;
    S.dst1 = S.dst0 + pitch;

    yskip16 = width - 16;
    yskip32 = width - 32;
    uvskip16 = uvw - 8;
    uvskip32 = uvw - 16;
    tmp = width - srcw;
    ybandskip = tmp + (srcw & 15) + width * 14;
    uvbandskip = uvw - (srcw >> 1) + ((srcw & 15) >> 1) + uvw * 7;
    dbandskip = pitch - srcw * (bx / 16) + (srcw & 15) * (bx / 16) + pitch * 14;

    if (flags & 0x10000)
    {
        S.u = (U8*)src + width * height;
        S.v = S.u + uvw * (height >> 1);
    }
    else
    {
        S.v = (U8*)src + width * height;
        S.u = S.v + uvw * (height >> 1);
    }
    S.u = S.u + (srcx >> 1) + (srcy >> 1) * uvw;
    S.v = S.v + (srcx >> 1) + (srcy >> 1) * uvw;

    lasty = srch + srcy - 1;
    lastx = srcw + srcx - 1;
    mask += mpitch * (srcy >> 4) + (S32)srcx / 16;

    for (y = srcy; y + 15 <= lasty; y += 16)
    {
        x = srcx;
        while (x + 31 <= lastx)
        {
            U32 bits = (mask[0] ? 1 : 0) + (mask[1] ? 2 : 0);
            YUVSTATE save;

            if (mrow2)
                bits |= (mask[mrow2] ? 1 : 0) + (mask[mrow2 + 1] ? 2 : 0);

            switch (bits)
            {
                case 1:
                    save = S;
                    MASKBLOCK(4, yskip16, uvskip16, skip16)
                    S = save;
                    break;

                case 2:
                    save = S;
                    ADVANCE(16, bx)
                    MASKBLOCK(4, yskip16, uvskip16, skip16)
                    S = save;
                    break;

                case 3:
                    save = S;
                    MASKBLOCK(8, yskip32, uvskip32, skip32)
                    S = save;
                    break;
            }

            ADVANCE(32, bx * 2)
            x += 32;
            mask += 2;
        }

        if (x + 15 <= lastx)
        {
            U32 bits = mask[0];

            if (mrow2)
                bits |= mask[mrow2];
            mask++;
            if (bits)
            {
                YUVSTATE save = S;

                MASKBLOCK(4, yskip16, uvskip16, skip16)
                S = save;
            }
            ADVANCE(16, bx)
        }

        S.y0 = S.y1 + ybandskip;
        S.y1 = S.y0 + width;
        S.a0 = S.a1 + ybandskip;
        S.a1 = S.a0 + width;
        S.u += uvbandskip;
        S.v += uvbandskip;
        S.dst0 = S.dst1 + dbandskip;
        S.dst1 = S.dst0 + pitch;
        mask += mskip;
    }

    if (srcw & 15)
        YUV_blit(dest, destx + (srcw & ~15) * xs, desty, opitch, src, srcw & ~15, 0, srcw - (srcw & ~15),
                 osrch, owidth, oheight, flags, alpha, b);

    if (srch & 15)
    {
        unsigned long olast = osrch - 1;
        unsigned long sy = mult64anddiv(srch & ~15, osrch, srch);

        YUV_blit(dest, destx, desty + mult64anddiv(sy, pitch * owidth, opitch * width), opitch, src, 0,
                 sy, srcw, olast - sy + 1, owidth, oheight, flags, alpha, b);
    }
}

void YUV_init(S32 layout)
{
    S32 i;

    if (!donetables)
    {
        for (i = 0; i < 256; i++)
        {
            S32 c;

            if (i > 16)
            {
                if (i <= 234)
                    ytable[i] = ((i - 16) * 38154) / 32768;
                else
                    ytable[i] = 254;
            }
            else
                ytable[i] = 0;
            ytable_x4[i] = ytable[i] << 2;
            c = i - 128;
            YUVTables[1][i] = -((c * 26639) / 32768);
            YUVTables[2][i] = -((c * 12837) / 32768);
            YUVTables[3][i] = (c * 52299) / 32768;
            YUVTables[0][i] = (c * 66101) / 32768;
        }
        for (i = 0; i < 256; i++)
        {
            clamptable[i] = 0;
            clamptable[i + 256] = i;
            clamptable[i + 512] = 255;
            clamp_ytable[i] = &clamptable[256] + ytable[i];
        }
        donetables = 1;
        memcpy(origYUVTables, YUVTables, sizeof(YUVTables));
    }

    if (rgb_layout != layout && layout != -1)
    {
        S32 rbits, gbits, bbits;
        S32 rshift, gshift, bshift;
        S32 rr, gr, br;
        U32 white, white2;
        U32 rmax, gmax, bmax;

        rgb_layout = layout;
        switch (layout)
        {
            case 10:
                rbits = 5; rshift = 0;
                gbits = 6; gshift = 5;
                bbits = 5; bshift = 11;
                break;
            case 8:
            case 9:
                rbits = 5; rshift = 0;
                gbits = 5; gshift = 5;
                bbits = 5; bshift = 10;
                break;
            case 7:
                rbits = 4; rshift = 0;
                gbits = 4; gshift = 4;
                bbits = 4; bshift = 8;
                break;
            case 12:
                rbits = 6; rshift = 0;
                gbits = 6; gshift = 6;
                bbits = 4; bshift = 12;
                break;
            case 11:
                rbits = 6; rshift = 0;
                gbits = 5; gshift = 6;
                bbits = 5; bshift = 11;
                break;
            default:
                rbits = 0; rshift = 0;
                gbits = 0; gshift = 0;
                bbits = 0; bshift = 0;
                break;
        }

        RGBshift[0][0] = rshift;
        RGBshift[0][1] = 0;
        RGBshift[1][0] = gshift;
        RGBshift[1][1] = 0;
        RGBshift[2][0] = bshift;
        RGBshift[2][1] = 0;
        RGBshift[3][0] = 8 - rbits;
        RGBshift[3][1] = 0;
        RGBshift[4][0] = 8 - gbits;
        RGBshift[4][1] = 0;
        RGBshift[5][0] = 8 - bbits;
        RGBshift[5][1] = 0;

        rr = 8 - rbits;
        gr = 8 - gbits;
        br = 8 - bbits;

        white = (0xff >> rr) | ((0xff >> gr) << gshift) | ((0xff >> br) << bshift);
        white2 = white | (white << 16);
        rmax = (1 << rbits) - 1;
        gmax = ((1 << gbits) - 1) << gshift;
        bmax = ((1 << bbits) - 1) << bshift;

        for (i = 0; i < 256; i++)
        {
            if (i <= 16)
            {
                mono16[i] = 0;
                mono16x2[i] = 0;
                mono32[i] = 0;
            }
            else if (i > 234)
            {
                mono16[i] = white;
                mono16x2[i] = white2;
                mono32[i] = 0xffffff;
            }
            else
            {
                S32 y = ((i - 16) * 255) / 219;
                U32 m = (y >> rr) | ((y >> gr) << gshift) | ((y >> br) << bshift);

                mono16x2[i] = m | (m << 16);
                mono32[i] = (y << 16) | (y << 8) | y;
                mono16[i] = m;
            }

            clamp_r[i] = 0;
            clamp_g[i] = 0;
            clamp_b[i] = 0;
            clamp_r[i + 256] = i >> rr;
            clamp_g[i + 256] = (i >> gr) << gshift;
            clamp_b[i + 256] = (i >> br) << bshift;
            clamp_r[i + 512] = rmax;
            clamp_g[i + 512] = gmax;
            clamp_b[i + 512] = bmax;

            clamp_rh[i] = clamp_r[i] << 16;
            clamp_gh[i] = clamp_g[i] << 16;
            clamp_bh[i] = clamp_b[i] << 16;
            clamp_rh[i + 256] = clamp_r[i + 256] << 16;
            clamp_gh[i + 256] = clamp_g[i + 256] << 16;
            clamp_bh[i + 256] = clamp_b[i + 256] << 16;
            clamp_rh[i + 512] = rmax << 16;
            clamp_gh[i + 512] = gmax << 16;
            clamp_bh[i + 512] = bmax << 16;

            clamp_rr[i] = 0;
            clamp_gg[i] = 0;
            clamp_bb[i] = 0;
            clamp_rr[i + 256] = (i >> rr) + ((i >> rr) << 16);
            clamp_gg[i + 256] = ((i >> gr) << gshift) + (((i >> gr) << gshift) << 16);
            clamp_bb[i + 256] = ((i >> br) << bshift) + (((i >> br) << bshift) << 16);
            clamp_rr[i + 512] = rmax + (rmax << 16);
            clamp_gg[i + 512] = gmax + (gmax << 16);
            clamp_bb[i + 512] = bmax + (bmax << 16);

            clamp_a4[i] = (i << 7) & 0xf000;
        }
    }
}

#define PUT(T, d, v) *(T*)S.d = v;

#define PUTC(T, d, v)  \
    c = v;              \
    *(T*)S.d = c;

#define PUT2W(T, d, v)      \
    c = v;                  \
    ((T*)S.d)[0] = c;       \
    ((T*)S.d)[1] = c;

#define PUT2H(T, d, v)                 \
    c = v;                             \
    ((T*)S.d)[0] = c;                  \
    *(T*)(S.d + S.pitch) = c;

#define PUT2WH(T, d, v)                \
    c = v;                             \
    ((T*)S.d)[0] = c;                  \
    ((T*)S.d)[1] = c;                  \
    *(T*)(S.d + S.pitch) = c;          \
    ((T*)(S.d + S.pitch))[1] = c;

#define UV32()                                              \
    S.b = YUVTables[0][*S.u];                               \
    S.g = YUVTables[2][*S.u] + YUVTables[1][*S.v];          \
    S.r = YUVTables[3][*S.v];

#define NEXTUV()       \
    if (!(x & 1))      \
    {                  \
        S.u++;         \
        S.v++;         \
    }

#define Y32(y) cy = clamp_ytable[*S.y++];
#define Y16(y) yv = ytable[*S.y++];

#define M32(y, a) mono32[*S.y++]
#define M32A(y, a) (mono32[*S.y++] | (*S.a++ << 24))
#define M16(y, a) mono16[*S.y++]
#define M16A4(y, a) (mono16[*S.y++] | clamp_a4[*S.a++])

#define C32(a) (cy[S.b] | (cy[S.g] << 8) | (cy[S.r] << 16))
#define C32A(a) (cy[S.b] | (cy[S.g] << 8) | (cy[S.r] << 16) | (*S.a++ << 24))
#define C16(a) (clamp_r[yv + S.b + 256] | clamp_g[yv + S.g + 256] | clamp_b[yv + S.r + 256])
#define C16A4(a)                                                                          \
    (clamp_r[yv + S.b + 256] | clamp_g[yv + S.g + 256] | clamp_b[yv + S.r + 256] |       \
     clamp_a4[*S.a++])

#define ROWM(name, T, PUTF, step, PIX)                      \
    static void name(unsigned long x, unsigned long count)  \
    {                                                       \
        while (count--)                                     \
        {                                                   \
            T c;                                            \
                                                            \
            PUTF(T, dst0, PIX(y0, a0))                      \
            S.dst0 += step;                                 \
        }                                                   \
    }

#define COLM(name, T, PUTF, step, PIX)                      \
    static long name(unsigned long count, long x)           \
    {                                                       \
        unsigned long n = count;                            \
                                                            \
        do                                                  \
        {                                                   \
            T c;                                            \
                                                            \
            PUTF(T, dst0, PIX(y0, a0))                      \
            PUTF(T, dst1, PIX(y1, a1))                      \
            S.dst0 += step;                                 \
            S.dst1 += step;                                 \
        } while (--n);                                      \
                                                            \
        return x + count;                                   \
    }

#define ROW(name, T, PUTF, step, DECL, Y, PIX)              \
    static void name(unsigned long x, unsigned long count)  \
    {                                                       \
        while (count--)                                     \
        {                                                   \
            T c;                                            \
            DECL;                                           \
                                                            \
            x++;                                            \
            UV32()                                          \
            Y(y0)                                           \
            PUTF(T, dst0, PIX(a0))                          \
            S.dst0 += step;                                 \
            NEXTUV()                                        \
        }                                                   \
    }

#define COL(name, T, PUTF, step, DECL, Y, PIX)              \
    static long name(unsigned long count, long x)           \
    {                                                       \
        unsigned long n = count;                            \
                                                            \
        do                                                  \
        {                                                   \
            T c;                                            \
            DECL;                                           \
                                                            \
            x++;                                            \
            UV32()                                          \
            Y(y0)                                           \
            PUTF(T, dst0, PIX(a0))                          \
            Y(y1)                                           \
            PUTF(T, dst1, PIX(a1))                          \
            S.dst0 += step;                                 \
            S.dst1 += step;                                 \
            NEXTUV()                                        \
        } while (--n);                                      \
                                                            \
        return x;                                           \
    }

#define FAMILY32(p, T, M, C, DECL, Y, s)                        \
    ROWM(dounaligned##p##rowm2w, T, PUT2W, 2 * s, M)            \
    COLM(dounaligned##p##colm2w, T, PUT2W, 2 * s, M)            \
    ROWM(dounaligned##p##rowm2h, T, PUT2H, s, M)                \
    COLM(dounaligned##p##colm2h, T, PUT2H, s, M)                \
    ROWM(dounaligned##p##rowm2wh, T, PUT2WH, 2 * s, M)          \
    COLM(dounaligned##p##colm2wh, T, PUT2WH, 2 * s, M)          \
    ROW(dounaligned##p##row2w, T, PUT2W, 2 * s, DECL, Y, C)     \
    COL(dounaligned##p##col2w, T, PUT2W, 2 * s, DECL, Y, C)     \
    ROW(dounaligned##p##row2h, T, PUT2H, s, DECL, Y, C)         \
    COL(dounaligned##p##col2h, T, PUT2H, s, DECL, Y, C)         \
    ROW(dounaligned##p##row2wh, T, PUT2WH, 2 * s, DECL, Y, C)   \
    COL(dounaligned##p##col2wh, T, PUT2WH, 2 * s, DECL, Y, C)   \
    ROWM(dounaligned##p##rowm, T, PUT, s, M)                    \
    COLM(dounaligned##p##colm, T, PUT, s, M)                    \
    ROW(dounaligned##p##row, T, PUT, s, DECL, Y, C)             \
    COL(dounaligned##p##col, T, PUT, s, DECL, Y, C)

#define FAMILY16(p, T, M, C, DECL, Y, s)                        \
    ROWM(dounaligned##p##rowm2h, T, PUT2H, s, M)                \
    COLM(dounaligned##p##colm2h, T, PUT2H, s, M)                \
    ROWM(dounaligned##p##rowm2w, T, PUT2W, 2 * s, M)            \
    COLM(dounaligned##p##colm2w, T, PUT2W, 2 * s, M)            \
    ROWM(dounaligned##p##rowm2wh, T, PUT2WH, 2 * s, M)          \
    COLM(dounaligned##p##colm2wh, T, PUT2WH, 2 * s, M)          \
    ROW(dounaligned##p##row2h, T, PUT2H, s, DECL, Y, C)         \
    COL(dounaligned##p##col2h, T, PUT2H, s, DECL, Y, C)         \
    ROW(dounaligned##p##row2w, T, PUT2W, 2 * s, DECL, Y, C)     \
    COL(dounaligned##p##col2w, T, PUT2W, 2 * s, DECL, Y, C)     \
    ROW(dounaligned##p##row2wh, T, PUT2WH, 2 * s, DECL, Y, C)   \
    COL(dounaligned##p##col2wh, T, PUT2WH, 2 * s, DECL, Y, C)   \
    ROWM(dounaligned##p##rowm, T, PUTC, s, M)                    \
    COLM(dounaligned##p##colm, T, PUTC, s, M)                    \
    ROW(dounaligned##p##row, T, PUT, s, DECL, Y, C)             \
    COL(dounaligned##p##col, T, PUT, s, DECL, Y, C)

FAMILY32(32, U32, M32, C32, U32* cy, Y32, 4)

static BLITS blits32 ALIGN32 = {
    4,
    { 2, YUV_32_4x2_even },
    { 2, YUV_32_4x2_odd },
    { 2, YUV_32m_4x2 },
    { 2, YUV_32x2_4x2_even },
    { 2, YUV_32x2_4x2_odd },
    { 2, YUV_32mx2_4x2 },
    dounaligned32row,
    dounaligned32col,
    dounaligned32rowm,
    dounaligned32colm,
    dounaligned32row2w,
    dounaligned32col2w,
    dounaligned32rowm2w,
    dounaligned32colm2w,
    dounaligned32row2h,
    dounaligned32col2h,
    dounaligned32rowm2h,
    dounaligned32colm2h,
    dounaligned32row2wh,
    dounaligned32col2wh,
    dounaligned32rowm2wh,
    dounaligned32colm2wh
};

extern "C" void YUV_blit_32bpp(void* dest, U32 destx, U32 desty, S32 pitch, U8* src, U32 srcx, U32 srcy,
                    U32 srcw, U32 srch, U32 width, U32 height, U32 flags)
{
    YUV_blit(dest, destx, desty, pitch, src, srcx, srcy, srcw, srch, width, height, flags, 0, &blits32);
}

FAMILY32(32a, U32, M32A, C32A, U32* cy, Y32, 4)

static BLITS blits32a ALIGN32 = {
    4,
    { 2, YUV_32a_4x2_even },
    { 2, YUV_32a_4x2_odd },
    { 2, YUV_32am_4x2 },
    { 2, YUV_32ax2_4x2_even },
    { 2, YUV_32ax2_4x2_odd },
    { 2, YUV_32amx2_4x2 },
    dounaligned32arow,
    dounaligned32acol,
    dounaligned32arowm,
    dounaligned32acolm,
    dounaligned32arow2w,
    dounaligned32acol2w,
    dounaligned32arowm2w,
    dounaligned32acolm2w,
    dounaligned32arow2h,
    dounaligned32acol2h,
    dounaligned32arowm2h,
    dounaligned32acolm2h,
    dounaligned32arow2wh,
    dounaligned32acol2wh,
    dounaligned32arowm2wh,
    dounaligned32acolm2wh
};

extern "C" void YUV_blit_32abpp(void* dest, U32 destx, U32 desty, S32 pitch, U8* src, U32 srcx, U32 srcy,
                    U32 srcw, U32 srch, U32 width, U32 height, U8* alpha, U32 flags)
{
    YUV_blit(dest, destx, desty, pitch, src, srcx, srcy, srcw, srch, width, height, flags, alpha, &blits32a);
}

FAMILY16(16, U16, M16, C16, S32 yv, Y16, 2)

static BLITS blits16 ALIGN32 = {
    2,
    { 2, YUV_16_4x2_even },
    { 2, YUV_16_4x2_odd },
    { 2, YUV_16m_4x2 },
    { 2, YUV_16x2_4x2_even },
    { 2, YUV_16x2_4x2_odd },
    { 2, YUV_16mx2_4x2 },
    dounaligned16row,
    dounaligned16col,
    dounaligned16rowm,
    dounaligned16colm,
    dounaligned16row2w,
    dounaligned16col2w,
    dounaligned16rowm2w,
    dounaligned16colm2w,
    dounaligned16row2h,
    dounaligned16col2h,
    dounaligned16rowm2h,
    dounaligned16colm2h,
    dounaligned16row2wh,
    dounaligned16col2wh,
    dounaligned16rowm2wh,
    dounaligned16colm2wh
};

extern "C" void YUV_blit_16bpp(void* dest, U32 destx, U32 desty, S32 pitch, U8* src, U32 srcx, U32 srcy,
                    U32 srcw, U32 srch, U32 width, U32 height, U32 flags)
{
    YUV_blit(dest, destx, desty, pitch, src, srcx, srcy, srcw, srch, width, height, flags, 0, &blits16);
}

FAMILY16(16a4, U16, M16A4, C16A4, S32 yv, Y16, 2)

static BLITS blits16a4 ALIGN32 = {
    2,
    { 2, YUV_16a4_4x2_even },
    { 2, YUV_16a4_4x2_odd },
    { 2, YUV_16a4m_4x2 },
    { 2, YUV_16a4x2_4x2_even },
    { 2, YUV_16a4x2_4x2_odd },
    { 2, YUV_16a4mx2_4x2 },
    dounaligned16a4row,
    dounaligned16a4col,
    dounaligned16a4rowm,
    dounaligned16a4colm,
    dounaligned16a4row2w,
    dounaligned16a4col2w,
    dounaligned16a4rowm2w,
    dounaligned16a4colm2w,
    dounaligned16a4row2h,
    dounaligned16a4col2h,
    dounaligned16a4rowm2h,
    dounaligned16a4colm2h,
    dounaligned16a4row2wh,
    dounaligned16a4col2wh,
    dounaligned16a4rowm2wh,
    dounaligned16a4colm2wh
};

extern "C" void YUV_blit_16a4bpp(void* dest, U32 destx, U32 desty, S32 pitch, U8* src, U32 srcx, U32 srcy,
                    U32 srcw, U32 srch, U32 width, U32 height, U8* alpha, U32 flags)
{
    YUV_blit(dest, destx, desty, pitch, src, srcx, srcy, srcw, srch, width, height, flags, alpha, &blits16a4);
}


#define YC(y, i, j) (S.y[i] | (*S.u++ << 8) | (S.y[j] << 16) | (*S.v++ << 24))
#define YCN(w, y, i, j) ((w & 0xFF00FF00) | S.y[i] | (S.y[j] << 16))

#define YROWM(name, BODY)                                   \
    static void name(unsigned long x, unsigned long count)  \
    {                                                       \
        long n = count;                                     \
                                                            \
        while ((n -= 2) >= 0)                               \
        {                                                   \
            BODY                                            \
        }                                                   \
    }

#define YCOL(name, BODY)                                    \
    static long name(unsigned long count, long x)           \
    {                                                       \
        long n = count;                                     \
                                                            \
        do                                                  \
        {                                                   \
            BODY                                            \
        } while ((n -= 2) > 0);                             \
                                                            \
        return x + count;                                   \
    }

// The masked writers put each output word in a variable the caller declares once for the
// whole loop body (a column function reuses it for both rows), and each shifted half in a
// temporary of its own. Both change GCC's register allocation (source-form rows).
#define YMW(w, y, hi, lo)                       \
    {                                           \
        U32 t = (S.y[hi] << 16) | 0x80008000;   \
        w = S.y[lo] | t;                        \
    }

#define YPUT2WH(d, w0, w1)                   \
    ((U32*)S.d)[0] = w0;                    \
    ((U32*)S.d)[1] = w1;                    \
    *(U32*)(S.d + S.pitch) = w0;            \
    ((U32*)(S.d + S.pitch))[1] = w1;

#define YMPUT2WH(d, y)                      \
    YMW(w0, y, 0, 0)                        \
    YMW(w1, y, 1, 1)                        \
    YPUT2WH(d, w0, w1)

#define YMPUT2W(d, y)                       \
    YMW(w, y, 0, 0)                         \
    ((U32*)S.d)[0] = w;                     \
    YMW(w, y, 1, 1)                         \
    S.y += 2;                               \
    ((U32*)S.d)[1] = w;

#define YMPUT2H(d, y)                       \
    YMW(w, y, 1, 0)                         \
    S.y += 2;                               \
    ((U32*)S.d)[0] = w;                     \
    *(U32*)(S.d + S.pitch) = w;

#define YMPUT(d, y)                         \
    YMW(w, y, 1, 0)                         \
    S.y += 2;                               \
    *(U32*)S.d = w;

YROWM(dounalignedYUY2rowm2wh, U32 w0; U32 w1; YMPUT2WH(dst0, y0) S.y0 += 2; S.dst0 += 8;)
YCOL(dounalignedYUY2colm2wh, U32 w0; U32 w1; YMPUT2WH(dst0, y0) YMPUT2WH(dst1, y1) S.y0 += 2; S.y1 += 2; S.dst0 += 8; S.dst1 += 8;)
YROWM(dounalignedYUY2rowm2w, U32 w; YMPUT2W(dst0, y0) S.dst0 += 8;)
YCOL(dounalignedYUY2colm2w, U32 w; YMPUT2W(dst0, y0) YMPUT2W(dst1, y1) S.dst0 += 8; S.dst1 += 8;)
YROWM(dounalignedYUY2rowm2h, U32 w; YMPUT2H(dst0, y0) S.dst0 += 4;)
YCOL(dounalignedYUY2colm2h, U32 w; YMPUT2H(dst0, y0) YMPUT2H(dst1, y1) S.dst0 += 4; S.dst1 += 4;)

YROWM(dounalignedYUY2row2wh,
      U32 w0 = YC(y0, 0, 0);
      U32 w1 = YCN(w0, y0, 1, 1);
      YPUT2WH(dst0, w0, w1)
      S.y0 += 2;
      S.dst0 += 8;)
YCOL(dounalignedYUY2col2wh,
     U32 w0 = YC(y0, 0, 0);
     U32 w1 = YCN(w0, y0, 1, 1);
     YPUT2WH(dst0, w0, w1)
     w0 = YCN(w0, y1, 0, 0);
     w1 = YCN(w1, y1, 1, 1);
     YPUT2WH(dst1, w0, w1)
     S.y0 += 2;
     S.y1 += 2;
     S.dst0 += 8;
     S.dst1 += 8;)
YROWM(dounalignedYUY2row2w,
      U32 w = YC(y0, 0, 0);
      ((U32*)S.dst0)[0] = w;
      w = YCN(w, y0, 1, 1);
      S.y0 += 2;
      ((U32*)S.dst0)[1] = w;
      S.dst0 += 8;)
YCOL(dounalignedYUY2col2w,
     U32 w = YC(y0, 0, 0);
     ((U32*)S.dst0)[0] = w;
     w = YCN(w, y0, 1, 1);
     S.y0 += 2;
     ((U32*)S.dst0)[1] = w;
     w = YCN(w, y1, 0, 0);
     ((U32*)S.dst1)[0] = w;
     w = YCN(w, y1, 1, 1);
     S.y1 += 2;
     ((U32*)S.dst1)[1] = w;
     S.dst0 += 8;
     S.dst1 += 8;)
YROWM(dounalignedYUY2row2h,
      U32 w = YC(y0, 0, 1);
      S.y0 += 2;
      ((U32*)S.dst0)[0] = w;
      *(U32*)(S.dst0 + S.pitch) = w;
      S.dst0 += 4;)
YCOL(dounalignedYUY2col2h,
     U32 w = YC(y0, 0, 1);
     S.y0 += 2;
     ((U32*)S.dst0)[0] = w;
     *(U32*)(S.dst0 + S.pitch) = w;
     w = YCN(w, y1, 0, 1);
     S.y1 += 2;
     ((U32*)S.dst1)[0] = w;
     *(U32*)(S.dst1 + S.pitch) = w;
     S.dst0 += 4;
     S.dst1 += 4;)
YROWM(dounalignedYUY2rowm, U32 w; YMPUT(dst0, y0) S.dst0 += 4;)
YCOL(dounalignedYUY2colm, U32 w; YMPUT(dst0, y0) YMPUT(dst1, y1) S.dst0 += 4; S.dst1 += 4;)
YROWM(dounalignedYUY2row,
      U32 w = YC(y0, 0, 1);
      S.y0 += 2;
      *(U32*)S.dst0 = w;
      S.dst0 += 4;)
YCOL(dounalignedYUY2col,
     U32 w = YC(y0, 0, 1);
     S.y0 += 2;
     *(U32*)S.dst0 = w;
     w = YCN(w, y1, 0, 1);
     S.y1 += 2;
     *(U32*)S.dst1 = w;
     S.dst0 += 4;
     S.dst1 += 4;)

static BLITS blitsyuy2 ALIGN32 = {
    2,
    { 2, YUY2_4x2 },
    { 2, YUY2_4x2 },
    { 2, YUY2_m_4x2 },
    { 2, YUY2_x2_4x2 },
    { 2, YUY2_x2_4x2 },
    { 2, YUY2_mx2_4x2 },
    dounalignedYUY2row,
    dounalignedYUY2col,
    dounalignedYUY2rowm,
    dounalignedYUY2colm,
    dounalignedYUY2row2w,
    dounalignedYUY2col2w,
    dounalignedYUY2rowm2w,
    dounalignedYUY2colm2w,
    dounalignedYUY2row2h,
    dounalignedYUY2col2h,
    dounalignedYUY2rowm2h,
    dounalignedYUY2colm2h,
    dounalignedYUY2row2wh,
    dounalignedYUY2col2wh,
    dounalignedYUY2rowm2wh,
    dounalignedYUY2colm2wh
};

extern "C" void YUV_blit_YUY2(void* dest, U32 destx, U32 desty, S32 pitch, U8* src, U32 srcx, U32 srcy,
                    U32 srcw, U32 srch, U32 width, U32 height, U32 flags)
{
    if (((U32)dest & 3) == 2)
    {
        destx++;
        dest = (void*)((U32)dest & ~3);
    }
    if (destx & 1)
    {
        if (srcx & 1)
            srcx++;
        destx++;
        srcw--;
    }
    else if (srcx & 1)
    {
        srcx++;
        srcw--;
    }
    if (srcw & 1)
        srcw--;

    YUV_blit(dest, destx, desty, pitch, src, srcx, srcy, srcw, srch, width, height, flags, 0, &blitsyuy2);
}

extern "C" void YUV_blit_32bpp_mask(void* dest, U32 destx, U32 desty, S32 pitch, U8* mask, U32 mpitch,
                    U8* src, U32 srcx, U32 srcy, U32 srcw, U32 srch, U32 width, U32 height,
                    U32 flags)
{
    YUV_blit_mask(dest, destx, desty, pitch, mask, mpitch, src, srcx, srcy, srcw, srch, width, height,
                  flags, 0, &blits32);
}

extern "C" void YUV_blit_32abpp_mask(void* dest, U32 destx, U32 desty, S32 pitch, U8* mask, U32 mpitch,
                    U8* src, U32 srcx, U32 srcy, U32 srcw, U32 srch, U32 width, U32 height, U8* alpha,
                    U32 flags)
{
    YUV_blit_mask(dest, destx, desty, pitch, mask, mpitch, src, srcx, srcy, srcw, srch, width, height,
                  flags, alpha, &blits32a);
}

extern "C" void YUV_blit_16bpp_mask(void* dest, U32 destx, U32 desty, S32 pitch, U8* mask, U32 mpitch,
                    U8* src, U32 srcx, U32 srcy, U32 srcw, U32 srch, U32 width, U32 height,
                    U32 flags)
{
    YUV_blit_mask(dest, destx, desty, pitch, mask, mpitch, src, srcx, srcy, srcw, srch, width, height,
                  flags, 0, &blits16);
}

extern "C" void YUV_blit_16a4bpp_mask(void* dest, U32 destx, U32 desty, S32 pitch, U8* mask, U32 mpitch,
                    U8* src, U32 srcx, U32 srcy, U32 srcw, U32 srch, U32 width, U32 height, U8* alpha,
                    U32 flags)
{
    YUV_blit_mask(dest, destx, desty, pitch, mask, mpitch, src, srcx, srcy, srcw, srch, width, height,
                  flags, alpha, &blits16a4);
}

extern "C" void YUV_blit_YUY2_mask(void* dest, U32 destx, U32 desty, S32 pitch, U8* mask, U32 mpitch,
                    U8* src, U32 srcx, U32 srcy, U32 srcw, U32 srch, U32 width, U32 height,
                    U32 flags)
{
    YUV_blit_mask(dest, destx, desty, pitch, mask, mpitch, src, srcx, srcy, srcw, srch, width, height,
                  flags, 0, &blitsyuy2);
}
