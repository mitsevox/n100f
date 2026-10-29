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

struct xMemArea_tag
{
    U32 addr;
    U32 size;
    U32 flags;
};

struct xMemInfo_tag
{
    xMemArea_tag system;
    xMemArea_tag stack;
    xMemArea_tag DRAM;
    xMemArea_tag SRAM;
};

extern U32 gActiveHeap;
extern xMemInfo_tag gMemInfo;

void* xMemAlloc(U32 heapID, U32 size);
void* xMemGrowAlloc(U32 heapID, U32 size);
void xMemPoolSetup(xMemPool* pool, void* buffer, U32 nextOffset, U32 flags,
                   void (*initCB)(xMemPool*, void*), U32 size, U32 count, U32 numRealloc);
void* xMemPoolAlloc(xMemPool* pool);
void xMemPoolFree(xMemPool* pool, void* data);
void* xMemPushTemp(U32 size);
void xMemPopTemp(void* addr);

#endif
