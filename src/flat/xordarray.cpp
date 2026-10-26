#include "xordarray.h"

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
