#ifndef XMEMMGR_H
#define XMEMMGR_H

#include "types.h"

struct xMemPool
{
    void* FreeList;
    U16 NextOffset;
    U16 Flags;
    void* UsedList;
    void (*InitCB)(xMemPool*, void*);
    void* Buffer;
    U16 Size;
    U16 NumRealloc;
    U32 Total;
};

extern U32 gActiveHeap;

void* xMemAlloc(U32 heapID, U32 size);
void* xMemPoolAlloc(xMemPool* pool);
void* xMemPushTemp(U32 size);
void xMemPopTemp(void* addr);

#endif
