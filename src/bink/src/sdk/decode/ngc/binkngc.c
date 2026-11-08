#include <dolphin/os.h>
#include "bink/radmem.h"

#define CNTLZW(x) ({ U32 _r; __asm__("cntlzw %0,%1" : "=r"(_r) : "r"(x)); _r; })

#define SUBMUL64(hi, lo, q, d) __asm__("mulhwu %1,%4,%5\n\tmullw %0,%4,%5\n\tsubfc %3,%0,%3\n\tsubfe %2,%1,%2" : "=&r"(_pl), "=&r"(_ph), "+r"(hi), "+r"(lo) : "r"(q), "r"(d))

inline U32 div64(U32 hi, U32 lo, U32 d)
{
    U32 inv, q, dh, s, t;
    U32 _pl, _ph;
    int sh, m;

    if ((d & (d - 1)) == 0)
    {
        U32 s = CNTLZW(d);
        return (lo >> (31 - s)) | (hi << (s + 1));
    }

    inv = 0xFFFFFFFF / d;
    dh = (d + 0xFFFF) >> 16;
    q = 0;
    if (dh)
    {
        s = CNTLZW(hi);
        q = (hi << s) / dh;
        sh = 16 - s;
        m = sh >> 31;
        q >>= -sh & m;
        q <<= sh & ~m;
        SUBMUL64(hi, lo, q, d);
    }

    while (hi)
    {
        t = inv * hi;
        SUBMUL64(hi, lo, t, d);
        q += t;
    }

    return q + lo / d;
}

inline void ReadTimeBase(U64* t)
{
    U32 hi, lo, hi2;

    do
    {
        __asm__ volatile("mftbu %0" : "=r"(hi));
        __asm__ volatile("mftb %0" : "=r"(lo));
        __asm__ volatile("mftbu %0" : "=r"(hi2));
        // FAKEMATCH: lo++; lo--; is a no-op that adds references to lo so ProDG gives lo priority over hi in register allocation; real form believed to be the loop without it.
        lo++; lo--;
    } while (hi != hi2);

    ((U32*)t)[1] = lo;
    ((U32*)t)[0] = hi;
}

U32 mult64anddiv(U32 a, U32 b, U32 d)
{
    U32 hi, lo;
    __asm__("mulhwu %1,%2,%3\n\tmullw %0,%2,%3" : "=&r"(lo), "=&r"(hi) : "r"(a), "r"(b));
    return div64(hi, lo, d);
}

U32 mult64andshift(U32 a, U32 b, U32 s)
{
    U32 hi, lo;
    __asm__("mulhwu %1,%2,%3\n\tmullw %0,%2,%3" : "=&r"(lo), "=&r"(hi) : "r"(a), "r"(b));
    return (lo >> s) | (hi << (32 - s));
}

void radmemset16(void* dest, U16 value, U32 count)
{
    U32 n = count >> 1;
    U32 v = (value << 16) | value;
    U32* d = (U32*)dest;

    while (n--)
        *d++ = v;

    if (count & 1)
        *(U16*)d = value;
}

U32 RADTimerRead(void)
{
    static U64 starttime = 0;
    U64 t = OSGetTime();
    U32 q, h, l;

    if (starttime == 0)
        starttime = t;

    t -= starttime;
    q = (U64)(U32)(t >> 32) * 106048;
    t -= (U64)q * 40500;
    h = (U32)(t >> 32) * 0xCF2049A1;
    l = (U32)(((U64)(U32)t * 0xCF2049A1) >> 32);
    return q + ((h + l) >> 15);
}

void RADCycleTimerStartAddr(U32* addr)
{
    U64 t;
    ReadTimeBase(&t);
    *addr = (U32)t;
}

void RADCycleTimerDeltaAddr(U32* addr)
{
    U64 t;
    ReadTimeBase(&t);
    *addr = (U32)t - *addr;
}

void RADCycleTimerStartAddr64(U64* addr)
{
    ReadTimeBase(addr);
}

void RADCycleTimerDeltaAddr64(U64* addr)
{
    U64 t;
    ReadTimeBase(&t);
    *addr = t - *addr;
}

static RADMEMALLOC usermalloc = 0;
static RADMEMFREE userfree = 0;
static RADMEMALLOC userarammalloc = 0;
static RADMEMFREE useraramfree = 0;

void RADSetMemory(RADMEMALLOC a, RADMEMFREE f)
{
    usermalloc = a;
    userfree = f;
}

void* radmalloc(U32 size)
{
    U32 request;
    U8* p;
    U8 kind;
    U8 off;

    if (size == 0 || size == 0xFFFFFFFF)
        return 0;

    request = size + 64;

    if (usermalloc && (p = usermalloc(request)) != 0)
    {
        if (p == (U8*)-1)
            return 0;
        kind = 3;
    }
    else
    {
        p = OSAllocFromHeap(__OSCurrHeap, request);
        if (p == 0)
            return 0;
        kind = 0;
    }

    off = 64 - ((U32)p & 31);
    p += off;
    p[-1] = off;
    p[-2] = kind;
    if (kind == 3)
        ((RADMEMFREE*)p)[-2] = userfree;
    return p;
}

void radfree(void* ptr)
{
    U8* p = ptr;

    if (p)
    {
        if (p[-2] == 3)
            ((RADMEMFREE*)p)[-2](p - p[-1]);
        else
            OSFreeToHeap(__OSCurrHeap, p - p[-1]);
    }
}

void RADSetAudioMemory(RADMEMALLOC a, RADMEMFREE f)
{
    userarammalloc = a;
    useraramfree = f;
}

void* radaudiomalloc(U32 size)
{
    if (userarammalloc)
        return userarammalloc(size);
    return 0;
}

void radaudiofree(void* ptr)
{
    if (useraramfree)
        useraramfree(ptr);
}

