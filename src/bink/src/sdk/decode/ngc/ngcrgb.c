#include "bink/yuvstate.h"

extern U32* clamp_ytable[260];
extern S32 YUVTables[4][256];
extern U32 clamp_a4[260];
extern U32 mono16[256];
extern U32 mono32[256];
extern S32 ytable[260];
extern U32 clamp_r[772];
extern U32 clamp_g[772];
extern U32 clamp_b[772];

inline U32* GetTiledRgbLoc(U32* addr, U32 tpitch)
{
    U32 row = ((U8*)addr - S.base) / S.pitch;

    return (U32*)(S.base + tpitch * (row >> 2) + (row & 3) * 8 + ((U8*)addr - (S.base + row * S.pitch)) * 4);
}

void YUV_32_4x2_even(S32 n)
{
    U32* d0;
    U32* d1;
    U16* u;
    U16* v;
    U32* y0;
    U32* y1;
    U32 tpitch;

    tpitch = (S.pitch * 4 + 63) & ~63;
    d0 = GetTiledRgbLoc(S.dst0, tpitch);
    S.dst0 += n * 4;
    d1 = GetTiledRgbLoc(S.dst1, tpitch);
    S.dst1 += n * 4;
    u = S.u;
    v = S.v;
    y0 = S.y0;
    y1 = S.y1;

    do {
        U32 uu = *u++;
        U32 vv = *v++;
        U32 yy0 = *y0++;
        U32 yy1 = *y1++;
        S32 r;
        S32 g;
        S32 b;
        U32* c0;
        U32* c1;

        r = YUVTables[3][vv >> 8];
        b = YUVTables[0][uu >> 8];
        g = YUVTables[2][uu >> 8] + YUVTables[1][vv >> 8];
        c0 = clamp_ytable[yy0 >> 24];
        c1 = clamp_ytable[(yy0 >> 16) & 0xFF];
        d0[0] = (c0[r] << 16) | c1[r];
        d0[8] = (c0[g] << 24) | (c0[b] << 16) | (c1[g] << 8) | c1[b];
        c0 = clamp_ytable[yy1 >> 24];
        c1 = clamp_ytable[(yy1 >> 16) & 0xFF];
        d1[0] = (c0[r] << 16) | c1[r];
        d1[8] = (c0[g] << 24) | (c0[b] << 16) | (c1[g] << 8) | c1[b];
        r = YUVTables[3][vv & 0xFF];
        b = YUVTables[0][uu & 0xFF];
        g = YUVTables[2][uu & 0xFF] + YUVTables[1][vv & 0xFF];
        c0 = clamp_ytable[(yy0 >> 8) & 0xFF];
        c1 = clamp_ytable[yy0 & 0xFF];
        d0[1] = (c0[r] << 16) | c1[r];
        d0[9] = (c0[g] << 24) | (c0[b] << 16) | (c1[g] << 8) | c1[b];
        d0 += 16;
        c0 = clamp_ytable[(yy1 >> 8) & 0xFF];
        c1 = clamp_ytable[yy1 & 0xFF];
        d1[1] = (c0[r] << 16) | c1[r];
        d1[9] = (c0[g] << 24) | (c0[b] << 16) | (c1[g] << 8) | c1[b];
        d1 += 16;
    } while (--n);

    S.u = u;
    S.v = v;
    S.y0 = y0;
    S.y1 = y1;
}

void YUV_32x2_4x2_even(S32 n)
{
    U32* d0;
    U32* d1;
    U16* u;
    U16* v;
    U32* y0;
    U32* y1;
    U32 tpitch;

    tpitch = (S.pitch * 4 + 63) & ~63;
    d0 = GetTiledRgbLoc(S.dst0, tpitch);
    S.dst0 += n * 8;
    d1 = GetTiledRgbLoc(S.dst1, tpitch);
    S.dst1 += n * 8;
    u = S.u;
    v = S.v;
    y0 = S.y0;
    y1 = S.y1;

    do {
        U32 uu = *u++;
        U32 vv = *v++;
        U32 yy0 = *y0++;
        U32 yy1 = *y1++;
        S32 r;
        S32 g;
        S32 b;
        U32* c0;
        U32* c1;

        r = YUVTables[3][vv >> 8];
        b = YUVTables[0][uu >> 8];
        g = YUVTables[2][uu >> 8] + YUVTables[1][vv >> 8];
        c0 = clamp_ytable[yy0 >> 24];
        c1 = clamp_ytable[(yy0 >> 16) & 0xFF];
        d0[0] = (c0[r] << 16) | c0[r];
        d0[1] = (c1[r] << 16) | c1[r];
        d0[8] = (c0[g] << 24) | (c0[b] << 16) | (c0[g] << 8) | c0[b];
        d0[9] = (c1[g] << 24) | (c1[b] << 16) | (c1[g] << 8) | c1[b];
        c0 = clamp_ytable[yy1 >> 24];
        c1 = clamp_ytable[(yy1 >> 16) & 0xFF];
        d1[0] = (c0[r] << 16) | c0[r];
        d1[1] = (c1[r] << 16) | c1[r];
        d1[8] = (c0[g] << 24) | (c0[b] << 16) | (c0[g] << 8) | c0[b];
        d1[9] = (c1[g] << 24) | (c1[b] << 16) | (c1[g] << 8) | c1[b];
        r = YUVTables[3][vv & 0xFF];
        b = YUVTables[0][uu & 0xFF];
        g = YUVTables[2][uu & 0xFF] + YUVTables[1][vv & 0xFF];
        c0 = clamp_ytable[(yy0 >> 8) & 0xFF];
        c1 = clamp_ytable[yy0 & 0xFF];
        d0[16] = (c0[r] << 16) | c0[r];
        d0[17] = (c1[r] << 16) | c1[r];
        d0[24] = (c0[g] << 24) | (c0[b] << 16) | (c0[g] << 8) | c0[b];
        d0[25] = (c1[g] << 24) | (c1[b] << 16) | (c1[g] << 8) | c1[b];
        d0 += 32;
        c0 = clamp_ytable[(yy1 >> 8) & 0xFF];
        c1 = clamp_ytable[yy1 & 0xFF];
        d1[16] = (c0[r] << 16) | c0[r];
        d1[17] = (c1[r] << 16) | c1[r];
        d1[24] = (c0[g] << 24) | (c0[b] << 16) | (c0[g] << 8) | c0[b];
        d1[25] = (c1[g] << 24) | (c1[b] << 16) | (c1[g] << 8) | c1[b];
        d1 += 32;
    } while (--n);

    S.u = u;
    S.v = v;
    S.y0 = y0;
    S.y1 = y1;
}

void YUV_32m_4x2(S32 n)
{
    U32* d0;
    U32* d1;
    U32* y0;
    U32* y1;
    U32 tpitch;

    tpitch = (S.pitch * 4 + 63) & ~63;
    d0 = GetTiledRgbLoc(S.dst0, tpitch);
    S.dst0 += n * 4;
    d1 = GetTiledRgbLoc(S.dst1, tpitch);
    S.dst1 += n * 4;
    y0 = S.y0;
    y1 = S.y1;

    do {
        U32 yy0 = *y0++;
        U32 yy1 = *y1++;
        U32 a;
        U32 b;
        U32 c;
        U32 e;

        a = mono32[yy0 >> 24];
        b = mono32[(yy0 >> 16) & 0xFF];
        c = mono32[(yy0 >> 8) & 0xFF];
        e = mono32[yy0 & 0xFF];
        d0[0] = (a & 0xFFFF0000) | (b >> 16);
        d0[1] = (c & 0xFFFF0000) | (e >> 16);
        d0[8] = (a << 16) | (b & 0xFFFF);
        d0[9] = (c << 16) | (e & 0xFFFF);
        d0 += 16;
        a = mono32[yy1 >> 24];
        b = mono32[(yy1 >> 16) & 0xFF];
        c = mono32[(yy1 >> 8) & 0xFF];
        e = mono32[yy1 & 0xFF];
        d1[0] = (a & 0xFFFF0000) | (b >> 16);
        d1[1] = (c & 0xFFFF0000) | (e >> 16);
        d1[8] = (a << 16) | (b & 0xFFFF);
        d1[9] = (c << 16) | (e & 0xFFFF);
        d1 += 16;
    } while (--n);

    S.y0 = y0;
    S.y1 = y1;
}

void YUV_32mx2_4x2(S32 n)
{
    U32* d0;
    U32* d1;
    U32* y0;
    U32* y1;
    U32 tpitch;

    tpitch = (S.pitch * 4 + 63) & ~63;
    d0 = GetTiledRgbLoc(S.dst0, tpitch);
    S.dst0 += n * 8;
    d1 = GetTiledRgbLoc(S.dst1, tpitch);
    S.dst1 += n * 8;
    y0 = S.y0;
    y1 = S.y1;

    do {
        U32 yy0 = *y0++;
        U32 yy1 = *y1++;
        U32 a;
        U32 b;

        a = mono32[yy0 >> 24];
        b = mono32[(yy0 >> 16) & 0xFF];
        d0[0] = (a & 0xFFFF0000) | (a >> 16);
        d0[1] = (b & 0xFFFF0000) | (b >> 16);
        d0[8] = (a << 16) | (a & 0xFFFF);
        d0[9] = (b << 16) | (b & 0xFFFF);
        a = mono32[(yy0 >> 8) & 0xFF];
        b = mono32[yy0 & 0xFF];
        d0[16] = (a & 0xFFFF0000) | (a >> 16);
        d0[17] = (b & 0xFFFF0000) | (b >> 16);
        d0[24] = (a << 16) | (a & 0xFFFF);
        d0[25] = (b << 16) | (b & 0xFFFF);
        d0 += 32;
        a = mono32[(yy1 >> 16) & 0xFF];
        b = mono32[(yy1 >> 8) & 0xFF];
        d1[0] = (a & 0xFFFF0000) | (a >> 16);
        d1[1] = (b & 0xFFFF0000) | (b >> 16);
        d1[8] = (a << 16) | (a & 0xFFFF);
        d1[9] = (b << 16) | (b & 0xFFFF);
        a = mono32[(yy1 >> 8) & 0xFF];
        b = mono32[yy1 & 0xFF];
        d1[16] = (a & 0xFFFF0000) | (a >> 16);
        d1[17] = (b & 0xFFFF0000) | (b >> 16);
        d1[24] = (a << 16) | (a & 0xFFFF);
        d1[25] = (b << 16) | (b & 0xFFFF);
        d1 += 32;
    } while (--n);

    S.y0 = y0;
    S.y1 = y1;
}

void YUV_16_4x2_even(S32 n)
{
    U32* d0;
    U32* d1;
    U16* u;
    U16* v;
    U32* y0;
    U32* y1;
    U32 tpitch;

    tpitch = (S.pitch * 4 + 31) & ~31;
    d0 = GetTiledRgbLoc(S.dst0, tpitch);
    S.dst0 += n * 2;
    d1 = GetTiledRgbLoc(S.dst1, tpitch);
    S.dst1 += n * 2;
    u = S.u;
    v = S.v;
    y0 = S.y0;
    y1 = S.y1;

    do {
        U32 uu = *u++;
        U32 vv = *v++;
        U32 yy0 = *y0++;
        U32 yy1 = *y1++;
        U32* r;
        U32* g;
        U32* b;
        U16 y;
        S32 ya;
        S32 yb;

        r = clamp_b + 256 + YUVTables[3][vv >> 8];
        b = clamp_r + 256 + YUVTables[0][uu >> 8];
        g = clamp_g + 256 + (YUVTables[2][uu >> 8] + YUVTables[1][vv >> 8]);
        y = yy0 >> 16;
        ya = ytable[y >> 8];
        yb = ytable[y & 0xFF];
        d0[0] = ((U16)(b[ya] | g[ya] | r[ya]) << 16) |
                (U16)(b[yb] | g[yb] | r[yb]);
        y = yy1 >> 16;
        ya = ytable[y >> 8];
        yb = ytable[y & 0xFF];
        d1[0] = ((U16)(b[ya] | g[ya] | r[ya]) << 16) |
                (U16)(b[yb] | g[yb] | r[yb]);
        r = clamp_b + 256 + YUVTables[3][vv & 0xFF];
        b = clamp_r + 256 + YUVTables[0][uu & 0xFF];
        g = clamp_g + 256 + (YUVTables[2][uu & 0xFF] + YUVTables[1][vv & 0xFF]);
        y = yy0 & 0xFFFF;
        ya = ytable[y >> 8];
        yb = ytable[y & 0xFF];
        d0[1] = ((U16)(b[ya] | g[ya] | r[ya]) << 16) |
                (U16)(b[yb] | g[yb] | r[yb]);
        d0 += 8;
        y = yy1 & 0xFFFF;
        ya = ytable[y >> 8];
        yb = ytable[y & 0xFF];
        d1[1] = ((U16)(b[ya] | g[ya] | r[ya]) << 16) |
                (U16)(b[yb] | g[yb] | r[yb]);
        d1 += 8;
    } while (--n);

    S.u = u;
    S.v = v;
    S.y0 = y0;
    S.y1 = y1;
}

void YUV_16x2_4x2_even(S32 n)
{
    U32* d0;
    U32* d1;
    U16* u;
    U16* v;
    U32* y0;
    U32* y1;
    U32 tpitch;

    tpitch = (S.pitch * 4 + 31) & ~31;
    d0 = GetTiledRgbLoc(S.dst0, tpitch);
    S.dst0 += n * 4;
    d1 = GetTiledRgbLoc(S.dst1, tpitch);
    S.dst1 += n * 4;
    u = S.u;
    v = S.v;
    y0 = S.y0;
    y1 = S.y1;

    do {
        U32 uu = *u++;
        U32 vv = *v++;
        U32 yy0 = *y0++;
        U32 yy1 = *y1++;
        U32* r;
        U32* g;
        U32* b;
        U16 y;
        S32 ya;
        S32 yb;
        U16 p;
        U16 q;

        r = clamp_b + 256 + YUVTables[3][vv >> 8];
        b = clamp_r + 256 + YUVTables[0][uu >> 8];
        g = clamp_g + 256 + (YUVTables[2][uu >> 8] + YUVTables[1][vv >> 8]);
        y = yy0 >> 16;
        ya = ytable[y >> 8];
        yb = ytable[y & 0xFF];
        p = (U16)(b[ya] | g[ya] | r[ya]);
        q = (U16)(b[yb] | g[yb] | r[yb]);
        d0[0] = (p << 16) | p;
        d0[1] = (q << 16) | q;
        y = yy1 >> 16;
        ya = ytable[y >> 8];
        yb = ytable[y & 0xFF];
        p = (U16)(b[ya] | g[ya] | r[ya]);
        q = (U16)(b[yb] | g[yb] | r[yb]);
        d1[0] = (p << 16) | p;
        d1[1] = (q << 16) | q;
        r = clamp_b + 256 + YUVTables[3][vv & 0xFF];
        b = clamp_r + 256 + YUVTables[0][uu & 0xFF];
        g = clamp_g + 256 + (YUVTables[2][uu & 0xFF] + YUVTables[1][vv & 0xFF]);
        y = yy0 & 0xFFFF;
        ya = ytable[y >> 8];
        yb = ytable[y & 0xFF];
        p = (U16)(b[ya] | g[ya] | r[ya]);
        q = (U16)(b[yb] | g[yb] | r[yb]);
        d0[8] = (p << 16) | p;
        d0[9] = (q << 16) | q;
        d0 += 16;
        y = yy1 & 0xFFFF;
        ya = ytable[y >> 8];
        yb = ytable[y & 0xFF];
        p = (U16)(b[ya] | g[ya] | r[ya]);
        q = (U16)(b[yb] | g[yb] | r[yb]);
        d1[8] = (p << 16) | p;
        d1[9] = (q << 16) | q;
        d1 += 16;
    } while (--n);

    S.u = u;
    S.v = v;
    S.y0 = y0;
    S.y1 = y1;
}

void YUV_16m_4x2(S32 n)
{
    U32* d0;
    U32* d1;
    U32* y0;
    U32* y1;
    U32 tpitch;

    tpitch = (S.pitch * 4 + 31) & ~31;
    d0 = GetTiledRgbLoc(S.dst0, tpitch);
    S.dst0 += n * 2;
    d1 = GetTiledRgbLoc(S.dst1, tpitch);
    S.dst1 += n * 2;
    y1 = S.y1;
    y0 = S.y0;

    do {
        U32 yy0 = *y0++;
        U32 yy1 = *y1++;
        U16 y;

        y = yy0 >> 16;
        d0[0] = ((U16)mono16[y >> 8] << 16) | (U16)mono16[y & 0xFF];
        y = yy1 >> 16;
        d1[0] = ((U16)mono16[y >> 8] << 16) | (U16)mono16[y & 0xFF];
        y = yy0;
        d0[1] = ((U16)mono16[y >> 8] << 16) | (U16)mono16[y & 0xFF];
        d0 += 8;
        y = yy1;
        d1[1] = ((U16)mono16[y >> 8] << 16) | (U16)mono16[y & 0xFF];
        d1 += 8;
    } while (--n);

    S.y0 = y0;
    S.y1 = y1;
}

void YUV_16mx2_4x2(S32 n)
{
    U32* d0;
    U32* d1;
    U32* y0;
    U32* y1;
    U32 tpitch;

    tpitch = (S.pitch * 4 + 31) & ~31;
    d0 = GetTiledRgbLoc(S.dst0, tpitch);
    S.dst0 += n * 4;
    d1 = GetTiledRgbLoc(S.dst1, tpitch);
    S.dst1 += n * 4;
    y1 = S.y1;
    y0 = S.y0;

    do {
        U32 yy0 = *y0++;
        U32 yy1 = *y1++;
        U16 y;
        U16 a;
        U16 b;

        y = yy0 >> 16;
        a = mono16[y >> 8];
        b = mono16[y & 0xFF];
        d0[0] = (a << 16) | a;
        d0[1] = (b << 16) | b;
        y = yy1 >> 16;
        a = mono16[y >> 8];
        b = mono16[y & 0xFF];
        d1[0] = (a << 16) | a;
        d1[1] = (b << 16) | b;
        y = yy0;
        a = mono16[y >> 8];
        b = mono16[y & 0xFF];
        d0[8] = (a << 16) | a;
        d0[9] = (b << 16) | b;
        d0 += 16;
        y = yy1;
        a = mono16[y >> 8];
        b = mono16[y & 0xFF];
        d1[8] = (a << 16) | a;
        d1[9] = (b << 16) | b;
        d1 += 16;
    } while (--n);

    S.y0 = y0;
    S.y1 = y1;
}

void YUV_32a_4x2_even(S32 n)
{
    U32* d0;
    U32* d1;
    U16* u;
    U16* v;
    U32* y0;
    U32* y1;
    U32* a0;
    U32* a1;
    U32 tpitch;

    tpitch = (S.pitch * 4 + 63) & ~63;
    d0 = GetTiledRgbLoc(S.dst0, tpitch);
    S.dst0 += n * 4;
    d1 = GetTiledRgbLoc(S.dst1, tpitch);
    S.dst1 += n * 4;
    a0 = S.a0;
    a1 = S.a1;
    u = S.u;
    v = S.v;
    y0 = S.y0;
    y1 = S.y1;

    do {
        U32 uu = *u++;
        U32 vv = *v++;
        U32 yy0 = *y0++;
        U32 yy1 = *y1++;
        U32 aa0 = *a0++;
        U32 aa1 = *a1++;
        S32 r;
        S32 g;
        S32 b;
        U32* c0;
        U32* c1;

        r = YUVTables[3][vv >> 8];
        b = YUVTables[0][uu >> 8];
        g = YUVTables[2][uu >> 8] + YUVTables[1][vv >> 8];
        c0 = clamp_ytable[yy0 >> 24];
        c1 = clamp_ytable[(yy0 >> 16) & 0xFF];
        d0[0] = (aa0 & 0xFF000000) | (c0[r] << 16) | ((aa0 >> 8) & 0xFF00) | c1[r];
        d0[8] = (c0[g] << 24) | (c0[b] << 16) | (c1[g] << 8) | c1[b];
        c0 = clamp_ytable[yy1 >> 24];
        c1 = clamp_ytable[(yy1 >> 16) & 0xFF];
        d1[0] = (aa1 & 0xFF000000) | (c0[r] << 16) | ((aa1 >> 8) & 0xFF00) | c1[r];
        d1[8] = (c0[g] << 24) | (c0[b] << 16) | (c1[g] << 8) | c1[b];
        r = YUVTables[3][vv & 0xFF];
        b = YUVTables[0][uu & 0xFF];
        g = YUVTables[2][uu & 0xFF] + YUVTables[1][vv & 0xFF];
        c0 = clamp_ytable[(yy0 >> 8) & 0xFF];
        c1 = clamp_ytable[yy0 & 0xFF];
        d0[1] = ((aa0 << 16) & 0xFF000000) | (c0[r] << 16) | ((aa0 << 8) & 0xFF00) | c1[r];
        d0[9] = (c0[g] << 24) | (c0[b] << 16) | (c1[g] << 8) | c1[b];
        d0 += 16;
        c0 = clamp_ytable[(yy1 >> 8) & 0xFF];
        c1 = clamp_ytable[yy1 & 0xFF];
        d1[1] = ((aa1 << 16) & 0xFF000000) | (c0[r] << 16) | ((aa1 << 8) & 0xFF00) | c1[r];
        d1[9] = (c0[g] << 24) | (c0[b] << 16) | (c1[g] << 8) | c1[b];
        d1 += 16;
    } while (--n);

    S.u = u;
    S.v = v;
    S.y0 = y0;
    S.y1 = y1;
    S.a0 = a0;
    S.a1 = a1;
}

void YUV_32ax2_4x2_even(S32 n)
{
    U32* d0;
    U32* d1;
    U16* u;
    U16* v;
    U32* y0;
    U32* y1;
    U32* a0;
    U32* a1;
    U32 tpitch;

    tpitch = (S.pitch * 4 + 63) & ~63;
    d0 = GetTiledRgbLoc(S.dst0, tpitch);
    S.dst0 += n * 8;
    d1 = GetTiledRgbLoc(S.dst1, tpitch);
    S.dst1 += n * 8;
    a0 = S.a0;
    a1 = S.a1;
    u = S.u;
    v = S.v;
    y0 = S.y0;
    y1 = S.y1;

    do {
        U32 uu = *u++;
        U32 vv = *v++;
        U32 yy0 = *y0++;
        U32 yy1 = *y1++;
        U32 aa0 = *a0++;
        U32 aa1 = *a1++;
        S32 r;
        S32 g;
        S32 b;
        U32* c0;
        U32* c1;

        r = YUVTables[3][vv >> 8];
        b = YUVTables[0][uu >> 8];
        g = YUVTables[2][uu >> 8] + YUVTables[1][vv >> 8];
        c0 = clamp_ytable[yy0 >> 24];
        c1 = clamp_ytable[(yy0 >> 16) & 0xFF];
        d0[0] = ((aa0 >> 24) << 24) | (c0[r] << 16) | ((aa0 >> 24) << 8) | c0[r];
        d0[1] = (((aa0 >> 16) & 0xFF) << 24) | (c1[r] << 16) | (((aa0 >> 16) & 0xFF) << 8) | c1[r];
        d0[8] = (c0[g] << 24) | (c0[b] << 16) | (c0[g] << 8) | c0[b];
        d0[9] = (c1[g] << 24) | (c1[b] << 16) | (c1[g] << 8) | c1[b];
        c0 = clamp_ytable[yy1 >> 24];
        c1 = clamp_ytable[(yy1 >> 16) & 0xFF];
        d1[0] = ((aa1 >> 24) << 24) | (c0[r] << 16) | ((aa1 >> 24) << 8) | c0[r];
        d1[1] = (((aa1 >> 16) & 0xFF) << 24) | (c1[r] << 16) | (((aa1 >> 16) & 0xFF) << 8) | c1[r];
        d1[8] = (c0[g] << 24) | (c0[b] << 16) | (c0[g] << 8) | c0[b];
        d1[9] = (c1[g] << 24) | (c1[b] << 16) | (c1[g] << 8) | c1[b];
        r = YUVTables[3][vv & 0xFF];
        b = YUVTables[0][uu & 0xFF];
        g = YUVTables[2][uu & 0xFF] + YUVTables[1][vv & 0xFF];
        c0 = clamp_ytable[(yy0 >> 8) & 0xFF];
        c1 = clamp_ytable[yy0 & 0xFF];
        d0[16] = (((aa0 >> 8) & 0xFF) << 24) | (c0[r] << 16) | (((aa0 >> 8) & 0xFF) << 8) | c0[r];
        d0[17] = ((aa0 & 0xFF) << 24) | (c1[r] << 16) | ((aa0 & 0xFF) << 8) | c1[r];
        d0[24] = (c0[g] << 24) | (c0[b] << 16) | (c0[g] << 8) | c0[b];
        d0[25] = (c1[g] << 24) | (c1[b] << 16) | (c1[g] << 8) | c1[b];
        d0 += 32;
        c0 = clamp_ytable[(yy1 >> 8) & 0xFF];
        c1 = clamp_ytable[yy1 & 0xFF];
        d1[16] = (((aa1 >> 8) & 0xFF) << 24) | (c0[r] << 16) | (((aa1 >> 8) & 0xFF) << 8) | c0[r];
        d1[17] = ((aa1 & 0xFF) << 24) | (c1[r] << 16) | ((aa1 & 0xFF) << 8) | c1[r];
        d1[24] = (c0[g] << 24) | (c0[b] << 16) | (c0[g] << 8) | c0[b];
        d1[25] = (c1[g] << 24) | (c1[b] << 16) | (c1[g] << 8) | c1[b];
        d1 += 32;
    } while (--n);

    S.u = u;
    S.v = v;
    S.y0 = y0;
    S.y1 = y1;
    S.a0 = a0;
    S.a1 = a1;
}

void YUV_32am_4x2(S32 n)
{
    U32* d0;
    U32* d1;
    U32* y0;
    U32* y1;
    U32* a0;
    U32* a1;
    U32 tpitch;

    tpitch = (S.pitch * 4 + 63) & ~63;
    d0 = GetTiledRgbLoc(S.dst0, tpitch);
    S.dst0 += n * 4;
    d1 = GetTiledRgbLoc(S.dst1, tpitch);
    S.dst1 += n * 4;
    a0 = S.a0;
    a1 = S.a1;
    y0 = S.y0;
    y1 = S.y1;

    do {
        U32 yy0 = *y0++;
        U32 yy1 = *y1++;
        U32 aa0 = *a0++;
        U32 aa1 = *a1++;
        U32 a;
        U32 b;
        U32 c;
        U32 e;

        a = mono32[yy0 >> 24];
        b = mono32[(yy0 >> 16) & 0xFF];
        c = mono32[(yy0 >> 8) & 0xFF];
        e = mono32[yy0 & 0xFF];
        d0[0] = (aa0 & 0xFF000000) | (a & 0xFFFF0000) | (b >> 16) | ((aa0 >> 8) & 0xFF00);
        d0[1] = ((aa0 << 16) & 0xFF000000) | (c & 0xFFFF0000) | (e >> 16) | ((aa0 << 8) & 0xFF00);
        d0[8] = (a << 16) | (b & 0xFFFF);
        d0[9] = (c << 16) | (e & 0xFFFF);
        d0 += 16;
        a = mono32[yy1 >> 24];
        b = mono32[(yy1 >> 16) & 0xFF];
        c = mono32[(yy1 >> 8) & 0xFF];
        e = mono32[yy1 & 0xFF];
        d1[0] = (aa1 & 0xFF000000) | (a & 0xFFFF0000) | (b >> 16) | ((aa1 >> 8) & 0xFF00);
        d1[1] = ((aa1 << 16) & 0xFF000000) | (c & 0xFFFF0000) | (e >> 16) | ((aa1 << 8) & 0xFF00);
        d1[8] = (a << 16) | (b & 0xFFFF);
        d1[9] = (c << 16) | (e & 0xFFFF);
        d1 += 16;
    } while (--n);

    S.y0 = y0;
    S.y1 = y1;
    S.a0 = a0;
    S.a1 = a1;
}

void YUV_32amx2_4x2(S32 n)
{
    U32* d0;
    U32* d1;
    U32* y0;
    U32* y1;
    U32* a0;
    U32* a1;
    U32 tpitch;

    tpitch = (S.pitch * 4 + 63) & ~63;
    d0 = GetTiledRgbLoc(S.dst0, tpitch);
    S.dst0 += n * 8;
    d1 = GetTiledRgbLoc(S.dst1, tpitch);
    S.dst1 += n * 8;
    a0 = S.a0;
    a1 = S.a1;
    y0 = S.y0;
    y1 = S.y1;

    do {
        U32 yy0 = *y0++;
        U32 yy1 = *y1++;
        U32 aa0 = *a0++;
        U32 aa1 = *a1++;
        U32 a;
        U32 b;

        a = mono32[yy0 >> 24];
        b = mono32[(yy0 >> 16) & 0xFF];
        d0[0] = ((aa0 >> 24) << 24) | (a & 0xFFFF0000) | (a >> 16) | ((aa0 >> 24) << 8);
        d0[1] = (((aa0 >> 16) & 0xFF) << 24) | (b & 0xFFFF0000) | (b >> 16) | (((aa0 >> 16) & 0xFF) << 8);
        d0[8] = (a << 16) | (a & 0xFFFF);
        d0[9] = (b << 16) | (b & 0xFFFF);
        a = mono32[(yy0 >> 8) & 0xFF];
        b = mono32[yy0 & 0xFF];
        d0[16] = ((aa0 >> 24) << 24) | (a & 0xFFFF0000) | (a >> 16) | ((aa0 >> 24) << 8);
        d0[17] = (((aa0 >> 16) & 0xFF) << 24) | (b & 0xFFFF0000) | (b >> 16) | (((aa0 >> 16) & 0xFF) << 8);
        d0[24] = (a << 16) | (a & 0xFFFF);
        d0[25] = (b << 16) | (b & 0xFFFF);
        d0 += 32;
        a = mono32[(yy1 >> 16) & 0xFF];
        b = mono32[(yy1 >> 8) & 0xFF];
        d1[0] = ((aa1 >> 24) << 24) | (a & 0xFFFF0000) | (a >> 16) | ((aa1 >> 24) << 8);
        d1[1] = (((aa1 >> 16) & 0xFF) << 24) | (b & 0xFFFF0000) | (b >> 16) | (((aa1 >> 16) & 0xFF) << 8);
        d1[8] = (a << 16) | (a & 0xFFFF);
        d1[9] = (b << 16) | (b & 0xFFFF);
        a = mono32[(yy1 >> 8) & 0xFF];
        b = mono32[yy1 & 0xFF];
        d1[16] = ((aa1 >> 24) << 24) | (a & 0xFFFF0000) | (a >> 16) | ((aa1 >> 24) << 8);
        d1[17] = (((aa1 >> 16) & 0xFF) << 24) | (b & 0xFFFF0000) | (b >> 16) | (((aa1 >> 16) & 0xFF) << 8);
        d1[24] = (a << 16) | (a & 0xFFFF);
        d1[25] = (b << 16) | (b & 0xFFFF);
        d1 += 32;
    } while (--n);

    S.y0 = y0;
    S.y1 = y1;
    S.a0 = a0;
    S.a1 = a1;
}

void YUV_16a4_4x2_even(S32 n)
{
    U32* d0;
    U32* d1;
    U16* u;
    U16* v;
    U32* y0;
    U32* y1;
    U32* a0;
    U32* a1;
    U32 tpitch;

    tpitch = (S.pitch * 4 + 31) & ~31;
    d0 = GetTiledRgbLoc(S.dst0, tpitch);
    S.dst0 += n * 2;
    d1 = GetTiledRgbLoc(S.dst1, tpitch);
    S.dst1 += n * 2;
    a0 = S.a0;
    a1 = S.a1;
    u = S.u;
    v = S.v;
    y0 = S.y0;
    y1 = S.y1;

    do {
        U32 uu = *u++;
        U32 vv = *v++;
        U32 yy0 = *y0++;
        U32 yy1 = *y1++;
        U32 aa0 = *a0++;
        U32 aa1 = *a1++;
        U32* r;
        U32* g;
        U32* b;
        U16 y;
        U16 a;
        S32 ya;
        S32 yb;

        r = clamp_b + 256 + YUVTables[3][vv >> 8];
        b = clamp_r + 256 + YUVTables[0][uu >> 8];
        g = clamp_g + 256 + (YUVTables[2][uu >> 8] + YUVTables[1][vv >> 8]);
        y = yy0 >> 16;
        a = aa0 >> 16;
        ya = ytable[y >> 8];
        yb = ytable[y & 0xFF];
        d0[0] = ((U16)(clamp_a4[a >> 8] | b[ya] | g[ya] | r[ya]) << 16) |
                (U16)(clamp_a4[a & 0xFF] | b[yb] | g[yb] | r[yb]);
        y = yy1 >> 16;
        a = aa1 >> 16;
        ya = ytable[y >> 8];
        yb = ytable[y & 0xFF];
        d1[0] = ((U16)(clamp_a4[a >> 8] | b[ya] | g[ya] | r[ya]) << 16) |
                (U16)(clamp_a4[a & 0xFF] | b[yb] | g[yb] | r[yb]);
        r = clamp_b + 256 + YUVTables[3][vv & 0xFF];
        b = clamp_r + 256 + YUVTables[0][uu & 0xFF];
        g = clamp_g + 256 + (YUVTables[2][uu & 0xFF] + YUVTables[1][vv & 0xFF]);
        y = yy0 & 0xFFFF;
        a = aa0;
        ya = ytable[y >> 8];
        yb = ytable[y & 0xFF];
        d0[1] = ((U16)(clamp_a4[a >> 8] | b[ya] | g[ya] | r[ya]) << 16) |
                (U16)(clamp_a4[a & 0xFF] | b[yb] | g[yb] | r[yb]);
        d0 += 8;
        y = yy1 & 0xFFFF;
        a = aa1;
        ya = ytable[y >> 8];
        yb = ytable[y & 0xFF];
        d1[1] = ((U16)(clamp_a4[a >> 8] | b[ya] | g[ya] | r[ya]) << 16) |
                (U16)(clamp_a4[a & 0xFF] | b[yb] | g[yb] | r[yb]);
        d1 += 8;
    } while (--n);

    S.u = u;
    S.v = v;
    S.y0 = y0;
    S.y1 = y1;
    S.a0 = a0;
    S.a1 = a1;
}

void YUV_16a4x2_4x2_even(S32 n)
{
    U32* d0;
    U32* d1;
    U16* u;
    U16* v;
    U32* y0;
    U32* y1;
    U32* a0;
    U32* a1;
    U32 tpitch;

    tpitch = (S.pitch * 4 + 31) & ~31;
    d0 = GetTiledRgbLoc(S.dst0, tpitch);
    S.dst0 += n * 4;
    d1 = GetTiledRgbLoc(S.dst1, tpitch);
    S.dst1 += n * 4;
    a0 = S.a0;
    a1 = S.a1;
    u = S.u;
    v = S.v;
    y0 = S.y0;
    y1 = S.y1;

    do {
        U32 uu = *u++;
        U32 vv = *v++;
        U32 yy0 = *y0++;
        U32 yy1 = *y1++;
        U32 aa0 = *a0++;
        U32 aa1 = *a1++;
        U32* r;
        U32* g;
        U32* b;
        U16 y;
        U16 a;
        S32 ya;
        S32 yb;
        U16 p;
        U16 q;

        r = clamp_b + 256 + YUVTables[3][vv >> 8];
        b = clamp_r + 256 + YUVTables[0][uu >> 8];
        g = clamp_g + 256 + (YUVTables[2][uu >> 8] + YUVTables[1][vv >> 8]);
        y = yy0 >> 16;
        a = aa0 >> 16;
        ya = ytable[y >> 8];
        yb = ytable[y & 0xFF];
        p = (U16)(clamp_a4[a >> 8] | b[ya] | g[ya] | r[ya]);
        q = (U16)(clamp_a4[a & 0xFF] | b[yb] | g[yb] | r[yb]);
        d0[0] = (p << 16) | p;
        d0[1] = (q << 16) | q;
        y = yy1 >> 16;
        a = aa1 >> 16;
        ya = ytable[y >> 8];
        yb = ytable[y & 0xFF];
        p = (U16)(clamp_a4[a >> 8] | b[ya] | g[ya] | r[ya]);
        q = (U16)(clamp_a4[a & 0xFF] | b[yb] | g[yb] | r[yb]);
        d1[0] = (p << 16) | p;
        d1[1] = (q << 16) | q;
        r = clamp_b + 256 + YUVTables[3][vv & 0xFF];
        b = clamp_r + 256 + YUVTables[0][uu & 0xFF];
        g = clamp_g + 256 + (YUVTables[2][uu & 0xFF] + YUVTables[1][vv & 0xFF]);
        y = yy0 & 0xFFFF;
        a = aa0;
        ya = ytable[y >> 8];
        yb = ytable[y & 0xFF];
        p = (U16)(clamp_a4[a >> 8] | b[ya] | g[ya] | r[ya]);
        q = (U16)(clamp_a4[a & 0xFF] | b[yb] | g[yb] | r[yb]);
        d0[8] = (p << 16) | p;
        d0[9] = (q << 16) | q;
        d0 += 16;
        y = yy1 & 0xFFFF;
        a = aa1;
        ya = ytable[y >> 8];
        yb = ytable[y & 0xFF];
        p = (U16)(clamp_a4[a >> 8] | b[ya] | g[ya] | r[ya]);
        q = (U16)(clamp_a4[a & 0xFF] | b[yb] | g[yb] | r[yb]);
        d1[8] = (p << 16) | p;
        d1[9] = (q << 16) | q;
        d1 += 16;
    } while (--n);

    S.u = u;
    S.v = v;
    S.y0 = y0;
    S.y1 = y1;
    S.a0 = a0;
    S.a1 = a1;
}

void YUV_16a4m_4x2(S32 n)
{
    U32* d0;
    U32* d1;
    U32* y0;
    U32* y1;
    U32* a0;
    U32* a1;
    U32 tpitch;

    tpitch = (S.pitch * 4 + 31) & ~31;
    d0 = GetTiledRgbLoc(S.dst0, tpitch);
    S.dst0 += n * 2;
    d1 = GetTiledRgbLoc(S.dst1, tpitch);
    S.dst1 += n * 2;
    a0 = S.a0;
    a1 = S.a1;
    y0 = S.y0;
    y1 = S.y1;

    do {
        U32 aa0 = *a0++;
        U32 yy0 = *y0++;
        U32 aa1 = *a1++;
        U32 yy1 = *y1++;
        U16 y;
        U16 a;

        y = yy0 >> 16;
        a = aa0 >> 16;
        d0[0] = (((U16)clamp_a4[a >> 8] | (U16)mono16[y >> 8]) << 16) | ((U16)clamp_a4[a & 0xFF] | (U16)mono16[y & 0xFF]);
        y = yy1 >> 16;
        a = aa1 >> 16;
        d1[0] = (((U16)clamp_a4[a >> 8] | (U16)mono16[y >> 8]) << 16) | ((U16)clamp_a4[a & 0xFF] | (U16)mono16[y & 0xFF]);
        y = yy0;
        a = aa0;
        d0[1] = (((U16)clamp_a4[a >> 8] | (U16)mono16[y >> 8]) << 16) | ((U16)clamp_a4[a & 0xFF] | (U16)mono16[y & 0xFF]);
        d0 += 8;
        y = yy1;
        a = aa1;
        d1[1] = (((U16)clamp_a4[a >> 8] | (U16)mono16[y >> 8]) << 16) | ((U16)clamp_a4[a & 0xFF] | (U16)mono16[y & 0xFF]);
        d1 += 8;
    } while (--n);

    S.y0 = y0;
    S.y1 = y1;
    S.a0 = a0;
    S.a1 = a1;
}

void YUV_16a4mx2_4x2(S32 n)
{
    U32* d0;
    U32* d1;
    U32* y0;
    U32* y1;
    U32* a0;
    U32* a1;
    U32 tpitch;

    tpitch = (S.pitch * 4 + 31) & ~31;
    d0 = GetTiledRgbLoc(S.dst0, tpitch);
    S.dst0 += n * 4;
    d1 = GetTiledRgbLoc(S.dst1, tpitch);
    S.dst1 += n * 4;
    a0 = S.a0;
    a1 = S.a1;
    y0 = S.y0;
    y1 = S.y1;

    do {
        U32 aa0 = *a0++;
        U32 yy0 = *y0++;
        U32 aa1 = *a1++;
        U32 yy1 = *y1++;
        U16 y;
        U16 a;
        U16 p;
        U16 q;

        y = yy0 >> 16;
        a = aa0 >> 16;
        p = (U16)clamp_a4[a >> 8] | (U16)mono16[y >> 8];
        q = (U16)clamp_a4[a & 0xFF] | (U16)mono16[y & 0xFF];
        d0[0] = (p << 16) | p;
        d0[1] = (q << 16) | q;
        y = yy1 >> 16;
        a = aa1 >> 16;
        p = (U16)clamp_a4[a >> 8] | (U16)mono16[y >> 8];
        q = (U16)clamp_a4[a & 0xFF] | (U16)mono16[y & 0xFF];
        d1[0] = (p << 16) | p;
        d1[1] = (q << 16) | q;
        y = yy0;
        a = aa0;
        p = (U16)clamp_a4[a >> 8] | (U16)mono16[y >> 8];
        q = (U16)clamp_a4[a & 0xFF] | (U16)mono16[y & 0xFF];
        d0[8] = (p << 16) | p;
        d0[9] = (q << 16) | q;
        d0 += 16;
        y = yy1;
        a = aa1;
        p = (U16)clamp_a4[a >> 8] | (U16)mono16[y >> 8];
        q = (U16)clamp_a4[a & 0xFF] | (U16)mono16[y & 0xFF];
        d1[8] = (p << 16) | p;
        d1[9] = (q << 16) | q;
        d1 += 16;
    } while (--n);

    S.y0 = y0;
    S.y1 = y1;
    S.a0 = a0;
    S.a1 = a1;
}

void YUV_16_4x2_odd(S32 n)
{
}

void YUV_32_4x2_odd(S32 n)
{
}

void YUV_16x2_4x2_odd(S32 n)
{
}

void YUV_16a4x2_4x2_odd(S32 n)
{
}

void YUV_32x2_4x2_odd(S32 n)
{
}

void YUV_16a4_4x2_odd(S32 n)
{
}

void YUV_32a_4x2_odd(S32 n)
{
}

void YUV_32ax2_4x2_odd(S32 n)
{
}
