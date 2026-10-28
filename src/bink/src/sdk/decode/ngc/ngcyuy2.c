#include "bink/yuvstate.h"

inline void YUY2_4x2Helper(S32 n, U32* d, U32* y, U16* u, U16* v)
{
    U32 i = n >> 1;
    U32 yy, uu, vv;

    while (i--)
    {
        yy = *y++;
        uu = *(U32*)u;
        u += 2;
        vv = *(U32*)v;
        v += 2;
        *d++ = (yy & 0xFF000000) + ((uu >> 8) & 0xFF0000) + ((yy >> 8) & 0xFF00) + (vv >> 24);
        *d++ = ((yy << 16) & 0xFF000000) + (uu & 0xFF0000) + ((yy << 8) & 0xFF00) + ((vv >> 16) & 0xFF);
        yy = *y++;
        *d++ = (yy & 0xFF000000) + ((uu << 8) & 0xFF0000) + ((yy >> 8) & 0xFF00) + ((vv >> 8) & 0xFF);
        *d++ = ((yy << 16) & 0xFF000000) + ((uu << 16) & 0xFF0000) + ((yy << 8) & 0xFF00) + (vv & 0xFF);
    }

    if (n & 1)
    {
        U32 yw = *y;
        U16 uh = *u;
        U16 vh = *v;
        d[0] = (yw & 0xFF000000) + ((uh << 8) & 0xFF0000) + ((yw >> 8) & 0xFF00) + (vh >> 8);
        d[1] = ((yw << 16) & 0xFF000000) + ((uh << 16) & 0xFF0000) + ((yw << 8) & 0xFF00) + (vh & 0xFF);
    }
}

inline void YUY2_x2_4x2Helper(S32 n, U32* d, U32* y, U16* u, U16* v)
{
    U32 i = n >> 1;
    U32 yy, uu, vv, uv;

    while (i--)
    {
        uu = *(U32*)u;
        u += 2;
        vv = *(U32*)v;
        v += 2;
        yy = *y++;
        uv = ((uu >> 8) & 0xFF0000) + (vv >> 24);
        *d++ = (yy & 0xFF000000) + ((yy & 0xFF000000) >> 16) + uv;
        *d++ = ((yy & 0xFF0000) << 8) + ((yy & 0xFF0000) >> 8) + uv;
        uv = (uu & 0xFF0000) + ((vv >> 16) & 0xFF);
        *d++ = ((yy & 0xFF00) << 16) + (yy & 0xFF00) + uv;
        *d++ = ((yy & 0xFF) << 24) + ((yy & 0xFF) << 8) + uv;
        yy = *y++;
        uv = ((uu << 8) & 0xFF0000) + ((vv >> 8) & 0xFF);
        *d++ = (yy & 0xFF000000) + ((yy & 0xFF000000) >> 16) + uv;
        *d++ = ((yy & 0xFF0000) << 8) + ((yy & 0xFF0000) >> 8) + uv;
        uv = ((uu << 16) & 0xFF0000) + (vv & 0xFF);
        *d++ = ((yy & 0xFF00) << 16) + (yy & 0xFF00) + uv;
        *d++ = ((yy & 0xFF) << 24) + ((yy & 0xFF) << 8) + uv;
    }

    if (n & 1)
    {
        U32 yw = *y;
        U16 uh = *u;
        U16 vh = *v;
        U32 huv;

        huv = ((uh << 8) & 0xFF0000) + (vh >> 8);
        *d++ = (yw & 0xFF000000) + ((yw & 0xFF000000) >> 16) + huv;
        *d++ = ((yw & 0xFF0000) << 8) + ((yw & 0xFF0000) >> 8) + huv;
        huv = ((uh << 16) & 0xFF0000) + (vh & 0xFF);
        *d++ = ((yw & 0xFF00) << 16) + (yw & 0xFF00) + huv;
        *d = ((yw & 0xFF) << 24) + ((yw & 0xFF) << 8) + huv;
    }
}

inline void YUY2_m_4x2Helper(U32 n, U32* d, U32* y)
{
    U32 yy;

    while (n--)
    {
        yy = *y++;
        *d++ = (yy & 0xFF000000) + ((yy >> 8) & 0xFF00) + 0x00800080;
        *d++ = ((yy << 16) & 0xFF000000) + ((yy << 8) & 0xFF00) + 0x00800080;
    }
}

inline void YUY2_mx2_4x2Helper(U32 n, U32* d, U32* y)
{
    U32 yy;

    while (n--)
    {
        yy = *y++;
        *d++ = (yy & 0xFF000000) + ((yy & 0xFF000000) >> 16) + 0x00800080;
        *d++ = ((yy & 0xFF0000) << 8) + ((yy & 0xFF0000) >> 8) + 0x00800080;
        *d++ = ((yy & 0xFF00) << 16) + (yy & 0xFF00) + 0x00800080;
        *d++ = ((yy & 0xFF) << 24) + ((yy & 0xFF) << 8) + 0x00800080;
    }
}

void YUY2_4x2(S32 n)
{
    YUY2_4x2Helper(n, S.dst0, S.y0, S.u, S.v);
    YUY2_4x2Helper(n, S.dst1, S.y1, S.u, S.v);
    S.dst0 += n * 8;
    S.dst1 += n * 8;
    S.y0 += n * 4;
    S.y1 += n * 4;
    S.u += n * 2;
    S.v += n * 2;
}

void YUY2_x2_4x2(S32 n)
{
    YUY2_x2_4x2Helper(n, S.dst0, S.y0, S.u, S.v);
    YUY2_x2_4x2Helper(n, S.dst1, S.y1, S.u, S.v);
    S.dst0 += n * 16;
    S.dst1 += n * 16;
    S.y0 += n * 4;
    S.y1 += n * 4;
    S.u += n * 2;
    S.v += n * 2;
}

void YUY2_m_4x2(S32 n)
{
    YUY2_m_4x2Helper(n, S.dst0, S.y0);
    YUY2_m_4x2Helper(n, S.dst1, S.y1);
    S.dst0 += n * 8;
    S.dst1 += n * 8;
    S.y0 += n * 4;
    S.y1 += n * 4;
}

void YUY2_mx2_4x2(S32 n)
{
    YUY2_mx2_4x2Helper(n, S.dst0, S.y0);
    YUY2_mx2_4x2Helper(n, S.dst1, S.y1);
    S.dst0 += n * 16;
    S.dst1 += n * 16;
    S.y0 += n * 4;
    S.y1 += n * 4;
}
