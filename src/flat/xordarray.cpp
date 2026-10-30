#include "xordarray.h"
#include "xMath.h"
#include "xMemMgr.h"

// The warning level sits at 95% of capacity, one below it when that rounds to full.
void XOrdInit(st_XORDEREDARRAY* array, S32 max)
{
    S32 cnt;

    array->list = (void**)xMemAlloc(gActiveHeap, max << 2);
    array->cnt = 0;
    array->max = max;
    array->warnlvl = 0.95f * max;
    if (array->warnlvl == array->max)
    {
        cnt = array->max - 1;
        array->warnlvl = MAX(0, cnt);
    }
}

void XOrdDone(st_XORDEREDARRAY* array)
{
    array->list = NULL;
    array->cnt = 0;
    array->max = 0;
    array->warnlvl = 0;
}

void XOrdAppend(st_XORDEREDARRAY* array, void* elt)
{
    if (array->cnt >= array->max)
    {
        return;
    }
    array->list[array->cnt++] = elt;
}

void XOrdInsert(st_XORDEREDARRAY* array, void* elt, S32 (*compare)(void*, void*))
{
    S32 i;

    if (array->cnt >= array->max)
    {
        return;
    }
    array->cnt++;

    for (i = array->cnt - 1; i > 0; i--)
    {
        if (compare(array->list[i - 1], elt) <= 0)
        {
            array->list[i] = elt;
            return;
        }
        array->list[i] = array->list[i - 1];
    }

    array->list[0] = elt;
}

S32 XOrdLookup(st_XORDEREDARRAY* array, const void* key, S32 (*test)(const void*, void*))
{
    S32 da_idx = -1;
    S32 k0 = 0;
    S32 k1 = array->cnt;
    S32 k;
    S32 v;

    while (k1 > k0)
    {
        k = (k0 + k1) / 2;
        v = test(key, array->list[k]);
        if (v == 0)
        {
            da_idx = k;
            break;
        }
        if (v > 0)
        {
            k0 = k + 1;
        }
        else
        {
            k1 = k;
        }
    }

    return da_idx;
}

// Shell sort with the 3h+1 gap sequence. Each pass starts one past the gap and stops shifting
// at index h, so the first slots of a pass are never compared; the target does the same.
void XOrdSort(st_XORDEREDARRAY* array, S32 (*test)(const void*, void*))
{
    void** list = array->list;
    S32 num = array->cnt;
    S32 i;
    S32 j;
    S32 h = 1;
    void* v;

    while (h <= num)
    {
        h = h * 3 + 1;
    }

    while (h != 1)
    {
        h /= 3;
        for (i = h + 1; i < num; i++)
        {
            v = list[i];
            j = i;
            while (test(list[j - h], v) > 0)
            {
                list[j] = list[j - h];
                j -= h;
                if (j <= h)
                {
                    break;
                }
            }
            list[j] = v;
        }
    }
}
