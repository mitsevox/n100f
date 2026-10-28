#ifndef XORDARRAY_H
#define XORDARRAY_H

#include "types.h"

struct st_XORDEREDARRAY
{
    void** list;
    S32 cnt;
    S32 max;
    S32 warnlvl;
};

void XOrdInit(st_XORDEREDARRAY* array, S32 max);
S32 XOrdLookup(st_XORDEREDARRAY* array, const void* key, S32 (*test)(const void*, void*));
void XOrdInsert(st_XORDEREDARRAY* array, void* elt, S32 (*compare)(void*, void*));
void XOrdAppend(st_XORDEREDARRAY* array, void* elt);

#endif
