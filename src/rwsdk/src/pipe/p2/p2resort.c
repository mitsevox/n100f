#include <rwcore.h>

// Partitions with fewer items than this are left to the insertion sort.
#define SORT_MINPARTITION 5

// The insertion sort looks this far ahead for its sentinel.
#define SORT_SENTINELSEARCH 4

#define SORTKEY(_item, _keyOffset) (*(U32*)((_item) + (_keyOffset)))

#define SWAPITEMS(_a, _b, _stride)                                                           \
    {                                                                                        \
        U32* _pa = (U32*)(_a);                                                               \
        U32* _pb = (U32*)(_b);                                                               \
        U32 _n;                                                                              \
                                                                                             \
        for (_n = (_stride); _n >= 4; _n -= 4)                                               \
        {                                                                                    \
            U32 _tmp = *_pb;                                                                 \
                                                                                             \
            *_pb++ = *_pa;                                                                   \
            *_pa++ = _tmp;                                                                   \
        }                                                                                    \
    }

typedef struct SortStackEntry SortStackEntry;
struct SortStackEntry
{
    U8* lo;
    U8* hi;
    U32 bit;
};

static S32 _msbitpos(U32 value)
{
    S32 pos;

    if (value)
    {
        pos = 0;

        while (value >>= 1)
        {
            pos++;
        }
    }
    else
    {
        pos = -1;
    }

    return pos;
}

static void _repartition(U8* lo, U8* hi, U32 stride, U32 keyOffset, U32 bit)
{
    SortStackEntry stack[32];
    U32 minSize = stride * SORT_MINPARTITION;
    SortStackEntry* sp = stack;

    sp->lo = lo;
    sp->hi = hi;
    sp->bit = bit;
    sp++;

    do
    {
        sp--;
        lo = sp->lo;
        hi = sp->hi;
        bit = sp->bit;

        for (;;)
        {
            U8* start = lo;
            U8* end = hi;

            for (;;)
            {
                while (!(SORTKEY(lo, keyOffset) & bit))
                {
                    lo += stride;

                    if (lo > hi)
                    {
                        goto partitioned;
                    }
                }

                while (SORTKEY(hi, keyOffset) & bit)
                {
                    hi -= stride;

                    if (lo > hi)
                    {
                        goto partitioned;
                    }
                }

                SWAPITEMS(lo, hi, stride);

                lo += stride;
                hi -= stride;

                if (lo > hi)
                {
                    break;
                }
            }

        partitioned:
            bit >>= 1;

            if (!bit)
            {
                break;
            }

            if (end >= hi + stride + minSize)
            {
                sp->lo = hi + stride;
                sp->hi = end;
                sp->bit = bit;
                sp++;
            }

            hi = lo - stride;
            lo = start;

            if (hi < start + minSize)
            {
                break;
            }
        }
    } while (sp != stack);
}

static inline void _insertionsort(U8* base, U32 numItems, U32 stride, U32 keyOffset)
{
    U32 minIndex;
    U32 i;
    U32 minKey;
    U8* item;

    i = SORT_SENTINELSEARCH;

    if (numItems - 1 < SORT_SENTINELSEARCH)
    {
        i = numItems - 1;
    }

    minIndex = i;
    minKey = SORTKEY(base + i * stride, keyOffset);
    i--;

    do
    {
        U32 key = SORTKEY(base + i * stride, keyOffset);

        if (key < minKey)
        {
            minKey = key;
            minIndex = i;
        }
    } while (i--);

    if (minIndex)
    {
        SWAPITEMS(base, base + minIndex * stride, stride);
    }

    for (i = numItems, item = base; item += stride, --i;)
    {
        U32 key;
        U8* cur;

        key = SORTKEY(item, keyOffset);

        for (cur = item; cur -= stride, SORTKEY(cur, keyOffset) > key;)
        {
            SWAPITEMS(cur, cur + stride, stride);
        }

    }
}

void _rx_rxRadixExchangeSort(void* base, U32 numItems, U32 stride, U32 keyOffset, U32 minKey,
                             U32 maxKey)
{
    if (base && keyOffset + sizeof(U32) <= stride && minKey < maxKey)
    {
        if (numItems > SORT_MINPARTITION)
        {
            _repartition((U8*)base, (U8*)base + (numItems - 1) * stride, stride, keyOffset,
                         1 << _msbitpos(maxKey));
        }

        if (numItems > 1)
        {
            _insertionsort((U8*)base, numItems, stride, keyOffset);
        }
    }
}
