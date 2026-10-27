#include "bink/radmem.h"

static void** ptrs[32];
static U32 amt[32];

static U32 pushtot = 0;
static U32 pushcur = 0;
static void*** pushptr = ptrs;
static U32* pushamt = amt;
static U32 cursize = 32;

void pushmalloc(void** ptr, U32 amount)
{
    if (cursize == pushcur)
    {
        void*** np = radmalloc(cursize * 16);
        U32* na = (U32*)((char*)np + cursize * 8);
        memcpy(np, pushptr, cursize * 4);
        memcpy(na, pushamt, cursize * 4);
        cursize *= 2;
        if (cursize != 64)
            radfree(pushptr);
        pushptr = np;
        pushamt = na;
    }

    amount = (amount + 31) & ~31;
    amount += ((((pushtot >> 5) & 31) - ((amount >> 5) & 31) + 33) << 5) & 0x3E0;
    pushtot += amount;
    pushamt[pushcur] = amount;
    pushptr[pushcur++] = ptr;
}

U32 popmalloctotal(void)
{
    return pushtot;
}

void* popmalloc(U32 extra)
{
    char* base;
    char* p;
    U32 i;

    extra = (extra + 31) & ~31;
    base = radmalloc(pushtot + extra);
    pushtot = 0;
    if (base)
    {
        p = base + extra;
        for (i = 0; i < pushcur; i++)
        {
            *pushptr[i] = p;
            p += pushamt[i];
        }
    }
    pushcur = 0;
    return base;
}
