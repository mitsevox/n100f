#include "bink/expand.h"
#include "bink/binkngc.h"
#include "bink/bitplane.h"
#include "bink/dct.h"
#include "bink/varbits.h"

#define CNTLZW(x) ({ U32 _r; __asm__("cntlzw %0,%1" : "=r"(_r) : "r"(x)); _r; })

#define VARBITSGET(vb, v, n)                                              \
    if ((vb).bitlen >= (n))                                               \
    {                                                                     \
        v = (vb).bits & (0xFFFFFFFF >> (32 - (n)));                       \
        (vb).bits >>= (n);                                                \
        (vb).bitlen -= (n);                                               \
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

#define HUFF4GET(vb, v, tbl, reads, syms)                                   \
    {                                                                       \
        U32 _e, _w;                                                         \
        if ((vb).bitlen >= (reads))                                         \
        {                                                                   \
            _e = (tbl)[(vb).bits & (0xFFFFFFFF >> (32 - (reads)))];         \
            v = (syms)[_e & 15];                                            \
            _e >>= 4;                                                       \
            (vb).bits >>= _e;                                               \
            (vb).bitlen -= _e;                                              \
        }                                                                   \
        else                                                                \
        {                                                                   \
            _w = *(vb).cur;                                                 \
            _e = (tbl)[((vb).bits | (_w << (vb).bitlen)) & (0xFFFFFFFF >> (32 - (reads)))]; \
            v = (syms)[_e & 15];                                            \
            _e >>= 4;                                                       \
            if ((vb).bitlen >= _e)                                          \
            {                                                               \
                (vb).bits >>= _e;                                           \
                (vb).bitlen -= _e;                                          \
            }                                                               \
            else                                                            \
            {                                                               \
                (vb).bits = _w >> (_e - (vb).bitlen);                       \
                (vb).bitlen = 32 + (vb).bitlen - _e;                        \
                (vb).cur++;                                                 \
            }                                                               \
        }                                                                   \
    }

#define ROWF(d, s) *(F64*)(d) = *(F64*)(s)
#define ROWW(d, s)                     \
    ((U32*)(d))[0] = ((U32*)(s))[0]; \
    ((U32*)(d))[1] = ((U32*)(s))[1]

#define COPY8(ROW, d0, dp, s0, sp, d1, s1)  \
    ROW(d0, s0);                            \
    d0 = d1 + (dp);                         \
    s0 = s1 + (sp);                         \
    ROW(d1, s1);                            \
    d1 = d0 + (dp);                         \
    s1 = s0 + (sp);                         \
    ROW(d0, s0);                            \
    d0 = d1 + (dp);                         \
    s0 = s1 + (sp);                         \
    ROW(d1, s1);                            \
    d1 = d0 + (dp);                         \
    s1 = s0 + (sp);                         \
    ROW(d0, s0);                            \
    d0 = d1 + (dp);                         \
    s0 = s1 + (sp);                         \
    ROW(d1, s1);                            \
    d1 = d0 + (dp);                         \
    s1 = s0 + (sp);                         \
    ROW(d0, s0);                            \
    ROW(d1, s1)

#define ROWFC(d, s, k) *(F64*)(d) = ((F64*)(s))[k]
#define ROWWC(d, s, k)                         \
    ((U32*)(d))[0] = ((U32*)(s))[(k) * 2];     \
    ((U32*)(d))[1] = ((U32*)(s))[(k) * 2 + 1]

#define ROWS8C(ROW, d0, d1, dp, s)  \
    ROW(d0, s, 0);                  \
    d0 = d1 + (dp);                 \
    ROW(d1, s, 1);                  \
    d1 = d0 + (dp);                 \
    ROW(d0, s, 2);                  \
    d0 = d1 + (dp);                 \
    ROW(d1, s, 3);                  \
    d1 = d0 + (dp);                 \
    ROW(d0, s, 4);                  \
    d0 = d1 + (dp);                 \
    ROW(d1, s, 5);                  \
    d1 = d0 + (dp);                 \
    ROW(d0, s, 6);                  \
    ROW(d1, s, 7)

#define ROWS8(ROW, d0, d1, dp)  \
    ROW(d0);                    \
    d0 = d1 + (dp);             \
    ROW(d1);                    \
    d1 = d0 + (dp);             \
    ROW(d0);                    \
    d0 = d1 + (dp);             \
    ROW(d1);                    \
    d1 = d0 + (dp);             \
    ROW(d0);                    \
    d0 = d1 + (dp);             \
    ROW(d1);                    \
    d1 = d0 + (dp);             \
    ROW(d0);                    \
    ROW(d1)

#define FILLROW(d)          \
    ((U32*)(d))[0] = w;     \
    ((U32*)(d))[1] = w

#define FILLROW16(d)        \
    ((U32*)(d))[0] = w;     \
    ((U32*)(d))[1] = w;     \
    ((U32*)(d))[2] = w;     \
    ((U32*)(d))[3] = w

#define PATROW(d)                                                \
    pat = *pats.cur++;                                           \
    ((U32*)(d))[0] = (c1 & mask1[pat & 15]) | (c2 & mask2[pat & 15]); \
    ((U32*)(d))[1] = (c1 & mask1[pat >> 4]) | (c2 & mask2[pat >> 4])

typedef struct BUNDLE {
    U8* cur;
    U8* end;
    U32 bits;
    U32 bias;
    U8 syms[16];
    U32 reads;
    U8* table;
    U32 lenbits;
    U8* base;
} BUNDLE;

typedef struct HUFF8 {
    U8 syms[16][16];
    U32 reads[16];
    U8* tables[16];
    U32 prev;
} HUFF8;

static const F64 huff4decode00align = 0;

static const U8 huff4decode00[16] = {
    0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4A, 0x4B, 0x4C, 0x4D, 0x4E, 0x4F,
};

static const U8 huff4decode01[32] = {
    0x10, 0x41, 0x10, 0x52, 0x10, 0x53, 0x10, 0x54, 0x10, 0x55, 0x10, 0x56, 0x10, 0x57, 0x10, 0x58,
    0x10, 0x41, 0x10, 0x59, 0x10, 0x5A, 0x10, 0x5B, 0x10, 0x5C, 0x10, 0x5D, 0x10, 0x5E, 0x10, 0x5F,
};

static const U8 huff4decode02[32] = {
    0x20, 0x42, 0x21, 0x58, 0x20, 0x54, 0x21, 0x5C, 0x20, 0x43, 0x21, 0x5A, 0x20, 0x56, 0x21, 0x5E,
    0x20, 0x42, 0x21, 0x59, 0x20, 0x55, 0x21, 0x5D, 0x20, 0x43, 0x21, 0x5B, 0x20, 0x57, 0x21, 0x5F,
};

static const U8 huff4decode03[32] = {
    0x20, 0x43, 0x31, 0x58, 0x20, 0x45, 0x32, 0x5C, 0x20, 0x44, 0x31, 0x5A, 0x20, 0x56, 0x32, 0x5E,
    0x20, 0x43, 0x31, 0x59, 0x20, 0x45, 0x32, 0x5D, 0x20, 0x44, 0x31, 0x5B, 0x20, 0x57, 0x32, 0x5F,
};

static const U8 huff4decode04[32] = {
    0x30, 0x44, 0x32, 0x58, 0x31, 0x46, 0x33, 0x5C, 0x30, 0x45, 0x32, 0x5A, 0x31, 0x47, 0x33, 0x5E,
    0x30, 0x44, 0x32, 0x59, 0x31, 0x46, 0x33, 0x5D, 0x30, 0x45, 0x32, 0x5B, 0x31, 0x47, 0x33, 0x5F,
};

static const U8 huff4decode05[32] = {
    0x30, 0x46, 0x42, 0x4A, 0x31, 0x48, 0x44, 0x5C, 0x30, 0x47, 0x43, 0x4B, 0x31, 0x49, 0x45, 0x5E,
    0x30, 0x46, 0x42, 0x4A, 0x31, 0x48, 0x44, 0x5D, 0x30, 0x47, 0x43, 0x4B, 0x31, 0x49, 0x45, 0x5F,
};

static const U8 huff4decode06[32] = {
    0x20, 0x45, 0x41, 0x49, 0x20, 0x47, 0x43, 0x5C, 0x20, 0x46, 0x42, 0x5A, 0x20, 0x48, 0x44, 0x5E,
    0x20, 0x45, 0x41, 0x49, 0x20, 0x47, 0x43, 0x5D, 0x20, 0x46, 0x42, 0x5B, 0x20, 0x48, 0x44, 0x5F,
};

static const U8 huff4decode07[64] = {
    0x10, 0x31, 0x10, 0x53, 0x10, 0x32, 0x10, 0x68, 0x10, 0x31, 0x10, 0x55, 0x10, 0x32, 0x10, 0x6C,
    0x10, 0x31, 0x10, 0x54, 0x10, 0x32, 0x10, 0x6A, 0x10, 0x31, 0x10, 0x66, 0x10, 0x32, 0x10, 0x6E,
    0x10, 0x31, 0x10, 0x53, 0x10, 0x32, 0x10, 0x69, 0x10, 0x31, 0x10, 0x55, 0x10, 0x32, 0x10, 0x6D,
    0x10, 0x31, 0x10, 0x54, 0x10, 0x32, 0x10, 0x6B, 0x10, 0x31, 0x10, 0x67, 0x10, 0x32, 0x10, 0x6F,
};

static const U8 huff4decode08[64] = {
    0x10, 0x21, 0x10, 0x52, 0x10, 0x21, 0x10, 0x68, 0x10, 0x21, 0x10, 0x64, 0x10, 0x21, 0x10, 0x6C,
    0x10, 0x21, 0x10, 0x53, 0x10, 0x21, 0x10, 0x6A, 0x10, 0x21, 0x10, 0x66, 0x10, 0x21, 0x10, 0x6E,
    0x10, 0x21, 0x10, 0x52, 0x10, 0x21, 0x10, 0x69, 0x10, 0x21, 0x10, 0x65, 0x10, 0x21, 0x10, 0x6D,
    0x10, 0x21, 0x10, 0x53, 0x10, 0x21, 0x10, 0x6B, 0x10, 0x21, 0x10, 0x67, 0x10, 0x21, 0x10, 0x6F,
};

static const U8 huff4decode09[64] = {
    0x10, 0x31, 0x10, 0x54, 0x10, 0x42, 0x10, 0x68, 0x10, 0x31, 0x10, 0x56, 0x10, 0x43, 0x10, 0x6C,
    0x10, 0x31, 0x10, 0x55, 0x10, 0x42, 0x10, 0x6A, 0x10, 0x31, 0x10, 0x57, 0x10, 0x43, 0x10, 0x6E,
    0x10, 0x31, 0x10, 0x54, 0x10, 0x42, 0x10, 0x69, 0x10, 0x31, 0x10, 0x56, 0x10, 0x43, 0x10, 0x6D,
    0x10, 0x31, 0x10, 0x55, 0x10, 0x42, 0x10, 0x6B, 0x10, 0x31, 0x10, 0x57, 0x10, 0x43, 0x10, 0x6F,
};

static const U8 huff4decode10[64] = {
    0x20, 0x32, 0x21, 0x55, 0x20, 0x43, 0x21, 0x59, 0x20, 0x32, 0x21, 0x57, 0x20, 0x44, 0x21, 0x6C,
    0x20, 0x32, 0x21, 0x56, 0x20, 0x43, 0x21, 0x6A, 0x20, 0x32, 0x21, 0x58, 0x20, 0x44, 0x21, 0x6E,
    0x20, 0x32, 0x21, 0x55, 0x20, 0x43, 0x21, 0x59, 0x20, 0x32, 0x21, 0x57, 0x20, 0x44, 0x21, 0x6D,
    0x20, 0x32, 0x21, 0x56, 0x20, 0x43, 0x21, 0x6B, 0x20, 0x32, 0x21, 0x58, 0x20, 0x44, 0x21, 0x6F,
};

static const U8 huff4decode11[64] = {
    0x10, 0x41, 0x10, 0x55, 0x10, 0x43, 0x10, 0x59, 0x10, 0x42, 0x10, 0x57, 0x10, 0x44, 0x10, 0x6C,
    0x10, 0x41, 0x10, 0x56, 0x10, 0x43, 0x10, 0x6A, 0x10, 0x42, 0x10, 0x58, 0x10, 0x44, 0x10, 0x6E,
    0x10, 0x41, 0x10, 0x55, 0x10, 0x43, 0x10, 0x59, 0x10, 0x42, 0x10, 0x57, 0x10, 0x44, 0x10, 0x6D,
    0x10, 0x41, 0x10, 0x56, 0x10, 0x43, 0x10, 0x6B, 0x10, 0x42, 0x10, 0x58, 0x10, 0x44, 0x10, 0x6F,
};

static const U8 huff4decode12[64] = {
    0x20, 0x22, 0x21, 0x53, 0x20, 0x22, 0x21, 0x68, 0x20, 0x22, 0x21, 0x55, 0x20, 0x22, 0x21, 0x6C,
    0x20, 0x22, 0x21, 0x54, 0x20, 0x22, 0x21, 0x6A, 0x20, 0x22, 0x21, 0x66, 0x20, 0x22, 0x21, 0x6E,
    0x20, 0x22, 0x21, 0x53, 0x20, 0x22, 0x21, 0x69, 0x20, 0x22, 0x21, 0x55, 0x20, 0x22, 0x21, 0x6D,
    0x20, 0x22, 0x21, 0x54, 0x20, 0x22, 0x21, 0x6B, 0x20, 0x22, 0x21, 0x67, 0x20, 0x22, 0x21, 0x6F,
};

static const U8 huff4decode13[128] = {
    0x10, 0x31, 0x10, 0x33, 0x10, 0x32, 0x10, 0x64, 0x10, 0x31, 0x10, 0x33, 0x10, 0x32, 0x10, 0x78,
    0x10, 0x31, 0x10, 0x33, 0x10, 0x32, 0x10, 0x66, 0x10, 0x31, 0x10, 0x33, 0x10, 0x32, 0x10, 0x7C,
    0x10, 0x31, 0x10, 0x33, 0x10, 0x32, 0x10, 0x65, 0x10, 0x31, 0x10, 0x33, 0x10, 0x32, 0x10, 0x7A,
    0x10, 0x31, 0x10, 0x33, 0x10, 0x32, 0x10, 0x67, 0x10, 0x31, 0x10, 0x33, 0x10, 0x32, 0x10, 0x7E,
    0x10, 0x31, 0x10, 0x33, 0x10, 0x32, 0x10, 0x64, 0x10, 0x31, 0x10, 0x33, 0x10, 0x32, 0x10, 0x79,
    0x10, 0x31, 0x10, 0x33, 0x10, 0x32, 0x10, 0x66, 0x10, 0x31, 0x10, 0x33, 0x10, 0x32, 0x10, 0x7D,
    0x10, 0x31, 0x10, 0x33, 0x10, 0x32, 0x10, 0x65, 0x10, 0x31, 0x10, 0x33, 0x10, 0x32, 0x10, 0x7B,
    0x10, 0x31, 0x10, 0x33, 0x10, 0x32, 0x10, 0x67, 0x10, 0x31, 0x10, 0x33, 0x10, 0x32, 0x10, 0x7F,
};

static const U8 huff4decode14[128] = {
    0x10, 0x31, 0x10, 0x33, 0x10, 0x32, 0x10, 0x54, 0x10, 0x31, 0x10, 0x33, 0x10, 0x32, 0x10, 0x78,
    0x10, 0x31, 0x10, 0x33, 0x10, 0x32, 0x10, 0x65, 0x10, 0x31, 0x10, 0x33, 0x10, 0x32, 0x10, 0x7C,
    0x10, 0x31, 0x10, 0x33, 0x10, 0x32, 0x10, 0x54, 0x10, 0x31, 0x10, 0x33, 0x10, 0x32, 0x10, 0x7A,
    0x10, 0x31, 0x10, 0x33, 0x10, 0x32, 0x10, 0x76, 0x10, 0x31, 0x10, 0x33, 0x10, 0x32, 0x10, 0x7E,
    0x10, 0x31, 0x10, 0x33, 0x10, 0x32, 0x10, 0x54, 0x10, 0x31, 0x10, 0x33, 0x10, 0x32, 0x10, 0x79,
    0x10, 0x31, 0x10, 0x33, 0x10, 0x32, 0x10, 0x65, 0x10, 0x31, 0x10, 0x33, 0x10, 0x32, 0x10, 0x7D,
    0x10, 0x31, 0x10, 0x33, 0x10, 0x32, 0x10, 0x54, 0x10, 0x31, 0x10, 0x33, 0x10, 0x32, 0x10, 0x7B,
    0x10, 0x31, 0x10, 0x33, 0x10, 0x32, 0x10, 0x77, 0x10, 0x31, 0x10, 0x33, 0x10, 0x32, 0x10, 0x7F,
};

static const U8 huff4decode15[128] = {
    0x20, 0x32, 0x21, 0x34, 0x20, 0x33, 0x21, 0x65, 0x20, 0x32, 0x21, 0x34, 0x20, 0x33, 0x21, 0x69,
    0x20, 0x32, 0x21, 0x34, 0x20, 0x33, 0x21, 0x67, 0x20, 0x32, 0x21, 0x34, 0x20, 0x33, 0x21, 0x7C,
    0x20, 0x32, 0x21, 0x34, 0x20, 0x33, 0x21, 0x66, 0x20, 0x32, 0x21, 0x34, 0x20, 0x33, 0x21, 0x7A,
    0x20, 0x32, 0x21, 0x34, 0x20, 0x33, 0x21, 0x68, 0x20, 0x32, 0x21, 0x34, 0x20, 0x33, 0x21, 0x7E,
    0x20, 0x32, 0x21, 0x34, 0x20, 0x33, 0x21, 0x65, 0x20, 0x32, 0x21, 0x34, 0x20, 0x33, 0x21, 0x69,
    0x20, 0x32, 0x21, 0x34, 0x20, 0x33, 0x21, 0x67, 0x20, 0x32, 0x21, 0x34, 0x20, 0x33, 0x21, 0x7D,
    0x20, 0x32, 0x21, 0x34, 0x20, 0x33, 0x21, 0x66, 0x20, 0x32, 0x21, 0x34, 0x20, 0x33, 0x21, 0x7B,
    0x20, 0x32, 0x21, 0x34, 0x20, 0x33, 0x21, 0x68, 0x20, 0x32, 0x21, 0x34, 0x20, 0x33, 0x21, 0x7F,
};

static const U8 huff4reads[16] = {
    0x04, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x07, 0x07, 0x07,
};

static const U32 mask2[16] = {
    0x00000000, 0xFF000000, 0x00FF0000, 0xFFFF0000,
    0x0000FF00, 0xFF00FF00, 0x00FFFF00, 0xFFFFFF00,
    0x000000FF, 0xFF0000FF, 0x00FF00FF, 0xFFFF00FF,
    0x0000FFFF, 0xFF00FFFF, 0x00FFFFFF, 0xFFFFFFFF,
};

static const U32 mask1[16] = {
    0xFFFFFFFF, 0x00FFFFFF, 0xFF00FFFF, 0x0000FFFF,
    0xFFFF00FF, 0x00FF00FF, 0xFF0000FF, 0x000000FF,
    0xFFFFFF00, 0x00FFFF00, 0xFF00FF00, 0x0000FF00,
    0xFFFF0000, 0x00FF0000, 0xFF000000, 0x00000000,
};

static const U32 mask4[4] = {
    0x00000000, 0xFFFF0000, 0x0000FFFF, 0xFFFFFFFF,
};

static const U32 mask3[4] = {
    0xFFFFFFFF, 0x0000FFFF, 0xFFFF0000, 0x00000000,
};

static const F64 huff4decodesalign = 0;

static const U8* huff4decodes[16] = {
    huff4decode00, huff4decode01, huff4decode02, huff4decode03, huff4decode04, huff4decode05,
    huff4decode06, huff4decode07, huff4decode08, huff4decode09, huff4decode10, huff4decode11,
    huff4decode12, huff4decode13, huff4decode14, huff4decode15,
};

static const U8 rlelens[4] = {4, 8, 12, 32};

static void OpenReadBundle(U8* base, BUNDLE* b, U32 start, U32 width, U32 bits, U32 mult, S32 sign)
{
    S32 last = (width >> 3) * mult - 1;

    b->cur = 0;
    b->end = 0;
    b->bits = bits;
    b->lenbits = 32 - CNTLZW((start + last) & 0xFFFF);
    b->bias = sign ? 1 << (bits - 1) : 0;
    b->base = base;
}

static void mergesort(VARBITS* vb, U8* out, U8* a, U8* b, U32 n)
{
    U32 na = n;
    U32 nb = n;
    U32 t;

    do {
        if (VARBITSGET1(*vb, t)) {
            *out = *b++;
            nb--;
        } else {
            *out = *a++;
            na--;
        }
        out++;
    } while (na && nb);

    if (na) {
        do {
            *out++ = *a++;
        } while (--na);
    } else if (nb) {
        do {
            *out++ = *b++;
        } while (--nb);
    }
}

static void ReadHuffTable(VARBITS* vb, const U8** table, U32* reads, U8* syms)
{
    U8 tmp2[16];
    U8 tmp[16];
    U32 i, n, q, t, v, used, depth;

    VARBITSGET(*vb, i, 4);
    *table = huff4decodes[i];
    *reads = huff4reads[i];

    if (i == 0) {
        for (q = 0; q < 16; q++)
            syms[q] = q;
    } else if (!VARBITSGET1(*vb, t)) {
        VARBITSGET(*vb, depth, 2);
        if (depth == 0) {
            U8* p = syms;
            for (q = 0; q < 16; q += 2) {
                if (VARBITSGET1(*vb, t)) {
                    p[0] = q + 1;
                    p[1] = q;
                } else {
                    p[1] = q + 1;
                    p[0] = q;
                }
                p += 2;
            }
        } else {
            for (q = 0; q < 16; q += 2) {
                if (VARBITSGET1(*vb, t)) {
                    tmp[q] = q + 1;
                    tmp[q + 1] = q;
                } else {
                    tmp[q] = q;
                    tmp[q + 1] = q + 1;
                }
            }

            if (depth == 1) {
                mergesort(vb, syms, tmp, tmp + 2, 2);
                mergesort(vb, syms + 4, tmp + 4, tmp + 6, 2);
                mergesort(vb, syms + 8, tmp + 8, tmp + 10, 2);
                mergesort(vb, syms + 12, tmp + 12, tmp + 14, 2);
            } else {
                mergesort(vb, tmp2, tmp, tmp + 2, 2);
                mergesort(vb, tmp2 + 4, tmp + 4, tmp + 6, 2);
                mergesort(vb, tmp2 + 8, tmp + 8, tmp + 10, 2);
                mergesort(vb, tmp2 + 12, tmp + 12, tmp + 14, 2);

                if (depth == 2) {
                    mergesort(vb, syms, tmp2, tmp2 + 4, 4);
                    mergesort(vb, syms + 8, tmp2 + 8, tmp2 + 12, 4);
                } else {
                    mergesort(vb, tmp, tmp2, tmp2 + 4, 4);
                    mergesort(vb, tmp + 8, tmp2 + 8, tmp2 + 12, 4);
                    mergesort(vb, syms, tmp, tmp + 8, 8);
                }
            }
        }
    } else {
        VARBITSGET(*vb, n, 3);
        used = 0xFFFF;
        for (q = 0; q <= n; q++) {
            VARBITSGET(*vb, v, 4);
            syms[q] = v;
            used &= ~(1 << v);
        }
        q = 0;
        do {
            if (used & 1)
                syms[++n] = q;
            q++;
        } while (used >>= 1);
    }
}

static void StartReadHuff4Bundle(BUNDLE* b, VARBITS* vb)
{
    ReadHuffTable(vb, (const U8**)&b->table, &b->reads, b->syms);
}

static void StartReadHuff8Bundle(BUNDLE* b, VARBITS* vb, HUFF8* h)
{
    U32 i;

    for (i = 0; i < 16; i++)
        ReadHuffTable(vb, (const U8**)&h->tables[i], &h->reads[i], h->syms[i]);
    ReadHuffTable(vb, (const U8**)&b->table, &b->reads, b->syms);
    h->prev = 0;
}

static void CheckReadRLEHuff4Bundle(BUNDLE* b, VARBITS* vb)
{
    U32 n, t, w;
    U8 v, sym, last;
    U8* p;
    U8* syms;
    const U8* tbl;

    if (b->cur == b->end) {
        VARBITSGET(*vb, n, b->lenbits);
        if (n) {
            b->end = b->base + n;
            b->cur = b->base;
            if (!VARBITSGET1(*vb, t)) {
                v = b->reads;
                p = b->base;
                tbl = b->table;
                syms = b->syms;
                last = 0;
                while (n) {
                    HUFF4GET(*vb, sym, tbl, v, syms);
                    if (sym > 11) {
                        w = last | (last << 8);
                        sym = rlelens[sym - 12];
                        w |= w << 16;
                        n -= sym;
                        do {
                            *(U32*)p = w;
                            p += 4;
                            sym -= 4;
                        } while (sym);
                    } else {
                        *p++ = sym;
                        last = sym;
                        n--;
                    }
                }
            } else {
                VARBITSGET(*vb, v, 4);
                memset(b->base, v, n);
            }
        } else {
            b->cur = b->base + 4;
            b->end = b->base;
        }
    }
}

static void CheckReadHuff8Bundle(BUNDLE* b, VARBITS* vb, HUFF8* h)
{
    S32 n;
    U32 t, reads, prev;
    U8 lo, v;
    U8* p;
    U8* syms;
    const U8* tbl;

    if (b->cur == b->end) {
        VARBITSGET(*vb, n, b->lenbits);
        if (n) {
            b->cur = b->base;
            b->end = b->base + n;
            p = b->base;
            syms = b->syms;
            reads = b->reads;
            tbl = b->table;
            prev = h->prev;
            if (VARBITSGET1(*vb, t))
                n = -20 - n;
            do {
                HUFF4GET(*vb, v, h->tables[prev], h->reads[prev], h->syms[prev]);
                prev = v;
                HUFF4GET(*vb, lo, tbl, reads, syms);
                v = (v << 4) | lo;
                *p++ = (v & 0x80) ? -128 - (v & 0x7F) : v | 0x80;
            } while (--n > 0);
            if (n < -22)
                memset(b->base, *b->base, -(n + 21));
            h->prev = prev;
        } else {
            b->cur = b->base + 4;
            b->end = b->base;
        }
    }
}

static void NewCheckReadHuff8Bundle(BUNDLE* b, VARBITS* vb, HUFF8* h)
{
    S32 n;
    U32 t, reads, prev;
    U8 hi, lo;
    U8* p;
    U8* syms;
    const U8* tbl;

    if (b->cur == b->end) {
        VARBITSGET(*vb, n, b->lenbits);
        if (n) {
            b->cur = b->base;
            b->end = b->base + n;
            p = b->base;
            syms = b->syms;
            reads = b->reads;
            tbl = b->table;
            prev = h->prev;
            if (VARBITSGET1(*vb, t))
                n = -20 - n;
            do {
                HUFF4GET(*vb, hi, h->tables[prev], h->reads[prev], h->syms[prev]);
                prev = hi;
                HUFF4GET(*vb, lo, tbl, reads, syms);
                *p++ = lo | (hi << 4);
            } while (--n > 0);
            if (n < -22)
                memset(b->base, *b->base, -(n + 21));
            h->prev = prev;
        } else {
            b->cur = b->base + 4;
            b->end = b->base;
        }
    }
}

static void CheckReadHuff4Bundle(BUNDLE* b, VARBITS* vb)
{
    U32 n, t, v;
    U8* p;
    U8* syms;
    const U8* tbl;

    if (b->cur == b->end) {
        VARBITSGET(*vb, n, b->lenbits);
        if (n) {
            b->end = b->base + n;
            b->cur = b->base;
            if (!VARBITSGET1(*vb, t)) {
                v = b->reads;
                p = b->base;
                tbl = b->table;
                syms = b->syms;
                while (n--) {
                    HUFF4GET(*vb, *p++, tbl, v, syms);
                }
            } else {
                VARBITSGET(*vb, v, 4);
                memset(b->base, v, n);
            }
        } else {
            b->cur = b->base + 4;
            b->end = b->base;
        }
    }
}

static void CheckReadHuff4PairBundle(BUNDLE* b, VARBITS* vb)
{
    U32 n, v;
    U8 lo, hi;
    U8* p;
    U8* syms;
    const U8* tbl;

    if (b->cur == b->end) {
        VARBITSGET(*vb, n, b->lenbits);
        if (n) {
            b->end = b->base + n;
            b->cur = b->base;
            v = b->reads;
            p = b->base;
            tbl = b->table;
            syms = b->syms;
            while (n--) {
                HUFF4GET(*vb, lo, tbl, v, syms);
                HUFF4GET(*vb, hi, tbl, v, syms);
                *p++ = lo | (hi << 4);
            }
        } else {
            b->cur = b->base + 4;
            b->end = b->base;
        }
    }
}

static void CheckReadHuff4SBundle(BUNDLE* b, VARBITS* vb)
{
    U32 n, t, r;
    S8 v;
    U8* p;
    U8* syms;
    const U8* tbl;

    if (b->cur == b->end) {
        VARBITSGET(*vb, n, b->lenbits);
        if (n) {
            b->cur = b->base;
            b->end = b->base + n;
            if (!VARBITSGET1(*vb, t)) {
                r = b->reads;
                p = b->base;
                tbl = b->table;
                syms = b->syms;
                while (n--) {
                    U32 s;
                    HUFF4GET(*vb, v, tbl, r, syms);
                    if (v && VARBITSGET1(*vb, s))
                        v = -v;
                    *p++ = v;
                }
            } else {
                VARBITSGET(*vb, v, 4);
                if (v && VARBITSGET1(*vb, t))
                    v = -v;
                memset(b->base, v, n);
            }
        } else {
            b->cur = b->base + 4;
            b->end = b->base;
        }
    }
}

static void CheckReadDelta16Bundle(BUNDLE* b, VARBITS* vb)
{
    U32 n, t, cnt, w;
    U16 v;
    S16 d;
    U16* p;

    if (b->cur == b->end) {
        VARBITSGET(*vb, n, b->lenbits);
        if (n) {
            p = (U16*)b->base;
            if (b->bias) {
                VARBITSGET(*vb, v, b->bits - 1);
                if (v && VARBITSGET1(*vb, t))
                    v = -v;
            } else {
                VARBITSGET(*vb, v, b->bits);
            }
            *p++ = v;
            b->cur = b->base;
            b->end = b->base + n * 2;
            n--;
            while (n) {
                cnt = n;
                if (cnt > 8)
                    cnt = 8;
                VARBITSGET(*vb, w, 4);
                if (w) {
                    U32 i = cnt;
                    while (i--) {
                        U32 s;
                        VARBITSGET(*vb, d, w);
                        if (d && VARBITSGET1(*vb, s))
                            d = -d;
                        v += d;
                        *p++ = v;
                    }
                } else {
                    radmemset16(p, v, cnt * 2);
                    p += cnt;
                }
                n -= cnt;
            }
        } else {
            b->cur = b->base + 4;
            b->end = b->base;
        }
    }
}

static U32 getbunsize(U32 count, U32 width, U32 bits, U32 mult)
{
    if (bits <= 8)
        bits = 8;
    else if (bits < 16)
        bits = 16;
    return (((count * bits) >> 3) + ((((width >> 3) * mult) * bits) >> 3) + 3) & ~3;
}

void ExpandBundleSizes(void** bundles, U32 width)
{
    bundles[0] = (void*)getbunsize(512, width, 4, 1);
    bundles[1] = (void*)getbunsize(512, width >> 1, 4, 1);
    bundles[2] = (void*)getbunsize(512, width, 8, 64);
    bundles[3] = (void*)getbunsize(512, width, 8, 8);
    bundles[4] = (void*)getbunsize(512, width, 5, 1);
    bundles[5] = (void*)getbunsize(512, width, 5, 1);
    bundles[6] = (void*)getbunsize(512, width, 11, 1);
    bundles[7] = (void*)getbunsize(512, width, 11, 1);
    bundles[8] = (void*)getbunsize(512, width, 4, 48);
}

void* ExpandPlane(U8* dest, U8* prev, U32 width, U32 height, U32 pitch, void* data, U32 key, U8* mask, U32 scale,
                  void** bundles, U32 flags)
{
    BUNDLE types;
    BUNDLE stypes;
    BUNDLE colors;
    BUNDLE pats;
    BUNDLE xoffs;
    BUNDLE yoffs;
    BUNDLE intradc;
    BUNDLE interdc;
    BUNDLE runs;
    HUFF8 h8;
    S16 coefs[64];
    F64 tmp[8];
    VARBITS vb;
    void (*readcolors)(BUNDLE*, VARBITS*, HUFF8*);
    U32 rowadd, maskpitch;
    U32 x, y, xc, t;

    maskpitch = pitch / (16 / scale);
    rowadd = pitch * 8 - width;
    x = 0;
    vb.cur = data;
    vb.start = data;
    vb.bitlen = 0;
    vb.bits = 0;
    readcolors = (flags & 0x8000) ? CheckReadHuff8Bundle : NewCheckReadHuff8Bundle;

    OpenReadBundle(bundles[0], &types, 512, width, 4, 1, 0);
    OpenReadBundle(bundles[1], &stypes, 512, width >> 1, 4, 1, 0);
    OpenReadBundle(bundles[2], &colors, 512, width, 8, 64, 0);
    OpenReadBundle(bundles[3], &pats, 512, width, 8, 8, 0);
    OpenReadBundle(bundles[4], &xoffs, 512, width, 5, 1, 1);
    OpenReadBundle(bundles[5], &yoffs, 512, width, 5, 1, 1);
    OpenReadBundle(bundles[6], &intradc, 512, width, 11, 1, 0);
    OpenReadBundle(bundles[7], &interdc, 512, width, 11, 1, 1);
    OpenReadBundle(bundles[8], &runs, 512, width, 4, 48, 0);

    StartReadHuff4Bundle(&types, &vb);
    StartReadHuff4Bundle(&stypes, &vb);
    StartReadHuff8Bundle(&colors, &vb, &h8);
    StartReadHuff4Bundle(&pats, &vb);
    StartReadHuff4Bundle(&xoffs, &vb);
    StartReadHuff4Bundle(&yoffs, &vb);
    StartReadHuff4Bundle(&runs, &vb);

    for (y = 0; y < height; y += 8) {
        CheckReadRLEHuff4Bundle(&types, &vb);
        CheckReadRLEHuff4Bundle(&stypes, &vb);
        readcolors(&colors, &vb, &h8);
        CheckReadHuff4PairBundle(&pats, &vb);
        CheckReadHuff4SBundle(&xoffs, &vb);
        CheckReadHuff4SBundle(&yoffs, &vb);
        CheckReadDelta16Bundle(&intradc, &vb);
        CheckReadDelta16Bundle(&interdc, &vb);
        CheckReadHuff4Bundle(&runs, &vb);

        for (xc = width; xc; xc -= 8) {
            switch (*types.cur++) {
            case 0: {
                U8* s0;
                U8* d0;
                U8* s1;
                U8* d1;
                s0 = prev;
                d0 = dest;
                d1 = dest + pitch;
                s1 = prev + pitch;
                if (!(((U32)dest | (U32)prev) & 7)) {
                    COPY8(ROWF, d0, pitch, s0, pitch, d1, s1);
                } else {
                    COPY8(ROWW, d0, pitch, s0, pitch, d1, s1);
                }
                break;
            }

            case 3: {
                const U8* sp;
                S32 i;

                VARBITSGET(vb, t, 4);
                sp = patterns[t];
                mask[x >> 1] = 1;
                i = 0;
                do {
                    U32 run, b;
                    if (VARBITSGET1(vb, b)) {
                        U8 c = *colors.cur++;
                        run = *runs.cur++ + 1;
                        while (run--)
                            ((U8*)tmp)[sp[i++]] = c;
                    } else {
                        run = *runs.cur++ + 1;
                        while (run--)
                            ((U8*)tmp)[sp[i++]] = *colors.cur++;
                    }
                } while (i < 63);
                if (i == 63)
                    ((U8*)tmp)[sp[63]] = *colors.cur++;
                {
                    U8* d0 = dest;
                    U8* d1 = dest + pitch;
                    if (!((U32)dest & 7)) {
                        ROWS8C(ROWFC, d0, d1, pitch, tmp);
                    } else {
                        ROWS8C(ROWWC, d0, d1, pitch, tmp);
                    }
                }
                break;
            }

            case 5:
                mask[x >> 1] = 1;
                coefs[0] = *(U16*)intradc.cur;
                intradc.cur += 2;
                ReadBPLossless(coefs, &vb);
                VARBITSGET(vb, t, 4);
                FastIDCT8x8(dest, pitch, coefs, t);
                break;

            case 4: {
                U8* s0;
                U8* d0;
                U8* s1;
                U8* d1;
                mask[x >> 1] = 1;
                s0 = prev + pitch * (S8)*yoffs.cur++ + (S8)*xoffs.cur++;
                d0 = (U8*)tmp;
                s1 = s0 + pitch;
                d1 = d0 + 8;
                if (!((U32)s0 & 7)) {
                    COPY8(ROWF, d0, 8, s0, pitch, d1, s1);
                } else {
                    COPY8(ROWW, d0, 8, s0, pitch, d1, s1);
                }
                VARBITSGET(vb, t, 7);
                ReadBPLossyWithMotion(dest, pitch, &vb, t, (U8*)tmp);
                break;
            }

            case 7: {
                U8* s0;
                U8* d0;
                U8* s1;
                U8* d1;
                mask[x >> 1] = 1;
                s0 = prev + pitch * (S8)*yoffs.cur++ + (S8)*xoffs.cur++;
                d0 = (U8*)tmp;
                s1 = s0 + pitch;
                d1 = d0 + 8;
                if (!((U32)s0 & 7)) {
                    COPY8(ROWF, d0, 8, s0, pitch, d1, s1);
                } else {
                    COPY8(ROWW, d0, 8, s0, pitch, d1, s1);
                }
                coefs[0] = *(U16*)interdc.cur;
                interdc.cur += 2;
                ReadBPLossless(coefs, &vb);
                VARBITSGET(vb, t, 4);
                FastmIDCT8x8WithMotion(dest, pitch, coefs, t, (U8*)tmp);
                break;
            }

            case 6: {
                U32 w;
                U8* d0;
                U8* d1;
                d0 = dest;
                d1 = dest + pitch;
                w = *colors.cur++;
                mask[x >> 1] = 1;
                w |= w << 8;
                w |= w << 16;
                ROWS8(FILLROW, d0, d1, pitch);
                break;
            }

            case 8: {
                U32 c1, c2, pat;
                U8* d0;
                U8* d1;
                d0 = dest;
                d1 = dest + pitch;
                c1 = *colors.cur++;
                c1 |= c1 << 8;
                c1 |= c1 << 16;
                c2 = *colors.cur++;
                c2 |= c2 << 8;
                c2 |= c2 << 16;
                mask[x >> 1] = 1;
                ROWS8(PATROW, d0, d1, pitch);
                break;
            }

            case 2: {
                U8* s0;
                U8* d0;
                U8* s1;
                U8* d1;
                mask[x >> 1] = 1;
                s0 = prev + pitch * (S8)*yoffs.cur++ + (S8)*xoffs.cur++;
                d0 = dest;
                s1 = s0 + pitch;
                d1 = dest + pitch;
                if (!(((U32)dest | (U32)s0) & 7)) {
                    COPY8(ROWF, d0, pitch, s0, pitch, d1, s1);
                } else {
                    COPY8(ROWW, d0, pitch, s0, pitch, d1, s1);
                }
                break;
            }

            case 9:
                mask[x >> 1] = 1;
                {
                    U8* d0 = dest;
                    U8* d1 = dest + pitch;
                    if (!(((U32)dest | (U32)colors.cur) & 7)) {
                        ROWS8C(ROWFC, d0, d1, pitch, colors.cur);
                    } else {
                        ROWS8C(ROWWC, d0, d1, pitch, colors.cur);
                    }
                }
                colors.cur += 64;
                break;

            case 1:
                if (!(y & 8)) {
                    switch (*stypes.cur++) {
                    case 6: {
                        U32 w, i;
                        U8* d0;
                        U8* d1;
                        w = *colors.cur++;
                        w |= w << 8;
                        w |= w << 16;
                        d0 = dest;
                        d1 = dest + pitch;
                        FILLROW16(d0);
                        for (i = 0; i < 7; i++) {
                            FILLROW16(d1);
                            d0 = d1 + pitch;
                            FILLROW16(d0);
                            d1 = d0 + pitch;
                        }
                        FILLROW16(d1);
                        break;
                    }

                    case 8: {
                        U32 c1, c2, i, pat;
                        U8* d0;
                        U8* d1;
                        c1 = *colors.cur++;
                        c1 |= c1 << 8;
                        c1 |= c1 << 16;
                        c2 = *colors.cur++;
                        c2 |= c2 << 8;
                        c2 |= c2 << 16;
                        d0 = dest;
                        d1 = dest + pitch;
                        for (i = 0; i < 8; i++) {
                            U32 w;
                            pat = *pats.cur++;
                            w = (c1 & mask3[pat & 3]) | (c2 & mask4[pat & 3]);
                            ((U32*)d0)[0] = w;
                            ((U32*)d1)[0] = w;
                            w = (c1 & mask3[(pat >> 2) & 3]) | (c2 & mask4[(pat >> 2) & 3]);
                            ((U32*)d0)[1] = w;
                            ((U32*)d1)[1] = w;
                            w = (c1 & mask3[(pat >> 4) & 3]) | (c2 & mask4[(pat >> 4) & 3]);
                            ((U32*)d0)[2] = w;
                            ((U32*)d1)[2] = w;
                            w = (c1 & mask3[(pat >> 6) & 3]) | (c2 & mask4[(pat >> 6) & 3]);
                            ((U32*)d0)[3] = w;
                            ((U32*)d1)[3] = w;
                            d0 = d1 + pitch;
                            d1 = d0 + pitch;
                        }
                        break;
                    }

                    case 3: {
                        const U8* sp;
                        S32 i;
                        U8* d0;
                        U8* d1;
                        U16* sr;

                        VARBITSGET(vb, t, 4);
                        sp = patterns[t];
                        i = 0;
                        do {
                            U32 run, b;
                            if (VARBITSGET1(vb, b)) {
                                U8 c = *colors.cur++;
                                run = *runs.cur++ + 1;
                                while (run--)
                                    ((U8*)tmp)[sp[i++]] = c;
                            } else {
                                run = *runs.cur++ + 1;
                                while (run--)
                                    ((U8*)tmp)[sp[i++]] = *colors.cur++;
                            }
                        } while (i < 63);
                        if (i == 63)
                            ((U8*)tmp)[sp[63]] = *colors.cur++;
                        d0 = dest;
                        d1 = dest + pitch;
                        sr = (U16*)tmp;
                        for (i = 0; i < 8; i++) {
                            U32 v;
                            v = sr[0];
                            v = ((v << 8) | (v & 0xFF)) | ((v << 16) & 0xFF000000);
                            ((U32*)d0)[0] = v;
                            ((U32*)d1)[0] = v;
                            v = sr[1];
                            v = ((v << 8) | (v & 0xFF)) | ((v << 16) & 0xFF000000);
                            ((U32*)d0)[1] = v;
                            ((U32*)d1)[1] = v;
                            v = sr[2];
                            v = ((v << 8) | (v & 0xFF)) | ((v << 16) & 0xFF000000);
                            ((U32*)d0)[2] = v;
                            ((U32*)d1)[2] = v;
                            v = sr[3];
                            v = ((v << 8) | (v & 0xFF)) | ((v << 16) & 0xFF000000);
                            ((U32*)d0)[3] = v;
                            ((U32*)d1)[3] = v;
                            sr += 4;
                            d0 = d1 + pitch;
                            d1 = d0 + pitch;
                        }
                        break;
                    }

                    case 5:
                        coefs[0] = *(U16*)intradc.cur;
                        intradc.cur += 2;
                        ReadBPLossless(coefs, &vb);
                        VARBITSGET(vb, t, 4);
                        FastIDCT8x8d(dest, pitch, coefs, t);
                        break;

                    case 9: {
                        U32 i;
                        U8* d;
                        d = dest;
                        for (i = 0; i < 8; i++) {
                            U16* sr = (U16*)colors.cur;
                            U32 v;
                            v = sr[0];
                            v = ((v << 8) | (v & 0xFF)) | ((v << 16) & 0xFF000000);
                            ((U32*)d)[0] = v;
                            ((U32*)(d + pitch))[0] = v;
                            v = sr[1];
                            v = ((v << 8) | (v & 0xFF)) | ((v << 16) & 0xFF000000);
                            ((U32*)d)[1] = v;
                            ((U32*)(d + pitch))[1] = v;
                            v = sr[2];
                            v = ((v << 8) | (v & 0xFF)) | ((v << 16) & 0xFF000000);
                            ((U32*)d)[2] = v;
                            ((U32*)(d + pitch))[2] = v;
                            v = sr[3];
                            v = ((v << 8) | (v & 0xFF)) | ((v << 16) & 0xFF000000);
                            ((U32*)d)[3] = v;
                            ((U32*)(d + pitch))[3] = v;
                            colors.cur = (U8*)(sr + 4);
                            d += pitch * 2;
                        }
                        break;
                    }
                    }
                }
                mask[x >> 1] = 1;
                x += scale;
                mask[x >> 1] = 1;
                dest += 8;
                prev += 8;
                xc -= 8;
                break;
            }
            x += scale;
            dest += 8;
            prev += 8;
        }

        if (scale == 1) {
            if (y & 8)
                mask += maskpitch;
        } else {
            mask += maskpitch;
        }
        x = 0;
        dest += rowadd;
        prev += rowadd;
    }

    vb.bitlen = 0;
    return vb.cur;
}

void ExpandBink(U8* dest, U8* prev, U8* adest, U8* aprev, U8* mask, U32 width, U32 height, U32 pitch, U32 pheight,
                void* data, U32 key, void** bundles, U32 flags, U32 hflags)
{
    U32 sep;
    void* end;

    if (hflags & 0x100000) {
        if (flags & 0x100000)
            ExpandPlane(adest, aprev, (width + 7) & ~7, (height + 7) & ~7, pitch, (U8*)data + 4, key, mask, 1, bundles,
                        flags);
        data = (U8*)data + *(U32*)data;
    }

    sep = flags & 0x8000;
    if (!sep)
        data = (U8*)data + 4;
    end = ExpandPlane(dest, prev, (width + 7) & ~7, (height + 7) & ~7, pitch, data, key, mask, 1, bundles, flags);
    if (!sep)
        end = (U8*)data + ((U32*)data)[-1] - 4;

    if (!(flags & 0x20000)) {
        dest += pitch * pheight;
        prev += pitch * pheight;
        width = (((width + 1) >> 1) + 7) & ~7;
        height = (((height + 1) >> 1) + 7) & ~7;
        pitch >>= 1;
        pheight >>= 1;
        end = ExpandPlane(dest, prev, width, height, pitch, end, key, mask, 2, bundles, flags);
        ExpandPlane(dest + pitch * pheight, prev + pitch * pheight, width, height, pitch, end, key, mask, 2, bundles,
                    flags);
    }
}
