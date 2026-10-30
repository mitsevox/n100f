#include "xMemMgr.h"
#include "iMemMgr.h"

#include <stdlib.h>

struct xMemBlock_tag;

struct xHeapState_tag
{
    U32 curr;
    U16 blk_ct;
    U16 pad;
    U32 used;
    U32 wasted;
};

struct xMemHeap_tag
{
    U32 flags;
    U32 hard_base;
    U32 size;
    S16 opp_heap[2];
    xHeapState_tag state[8];
    U16 state_idx;
    U16 max_blks;
    xMemBlock_tag* blk;
    xMemBlock_tag* lastblk;
};

xMemInfo_tag gMemInfo;
xMemHeap_tag gxHeap[3];
U32 gActiveHeap;
static void (*sMemBaseNotifyFunc)();

void xMemExit()
{
    iMemExit();
}

void* xMemPushTemp(U32 size)
{
    return malloc(size);
}

void xMemPopTemp(void* addr)
{
    free(addr);
}

// Saves the active heap's state on its stack and returns the depth before the push.
S32 xMemPushBase()
{
    xMemHeap_tag* heap = &gxHeap[gActiveHeap];

    heap->state_idx++;
    xHeapState_tag* sp = &heap->state[heap->state_idx];
    *sp = *(sp - 1);

    if (sMemBaseNotifyFunc != NULL)
    {
        sMemBaseNotifyFunc();
    }
    return heap->state_idx - 1;
}

// A negative depth pops that many states.
S32 xMemPopBase(S32 depth)
{
    xMemHeap_tag* heap = &gxHeap[gActiveHeap];
    if (depth < 0)
    {
        depth = heap->state_idx + depth;
    }

    heap->state_idx = depth;
    if (sMemBaseNotifyFunc != NULL)
    {
        sMemBaseNotifyFunc();
    }

    return heap->state_idx;
}

void xMemRegisterBaseNotifyFunc(void (*func)())
{
    sMemBaseNotifyFunc = func;
}

S32 xMemGetBase()
{
    return gxHeap[gActiveHeap].state_idx;
}

// Links the new elements into a chain through the word at NextOffset and puts the chain in front
// of the free list.
static void xMemPoolAddElements(xMemPool* pool, void* buffer, U32 count)
{
    S32 i;
    void* curr;
    void (*initCB)(xMemPool*, void*);
    U32 next;
    U32 size;

    initCB = pool->InitCB;
    next = pool->NextOffset;
    size = pool->Size;

    curr = buffer;
    for (i = 0; i < (S32)count - 1; i++)
    {
        *(void**)((U32)curr + next) = (void*)((U32)curr + size);
        if (initCB != NULL)
        {
            initCB(pool, curr);
        }
        curr = (void*)((U32)curr + size);
    }

    *(void**)((U32)curr + next) = pool->FreeList;
    if (initCB != NULL)
    {
        initCB(pool, curr);
    }
    pool->FreeList = buffer;
    pool->Total += count;
}

void xMemPoolSetup(xMemPool* pool, void* buffer, U32 nextOffset, U32 flags,
                   void (*initCB)(xMemPool*, void*), U32 size, U32 count, U32 numRealloc)
{
    pool->FreeList = NULL;
    pool->NextOffset = nextOffset;
    pool->Flags = flags;
    pool->UsedList = NULL;
    pool->InitCB = initCB;
    pool->Buffer = buffer;
    pool->Size = size;
    pool->NumRealloc = numRealloc;
    pool->Total = 0;
    xMemPoolAddElements(pool, buffer, count);
}

// An empty pool grows by NumRealloc elements from the active heap. Flag 0x1 keeps a list of the
// elements in use.
void* xMemPoolAlloc(xMemPool* pool)
{
    void* retval = pool->FreeList;
    U32 next = pool->NextOffset;
    U32 flags = pool->Flags;

    if (retval == NULL)
    {
        xMemPoolAddElements(pool, xMemAlloc(gActiveHeap, pool->NumRealloc * pool->Size),
                            pool->NumRealloc);
        retval = pool->FreeList;
    }

    pool->FreeList = *(void**)((U32)retval + next);
    if (flags & 1)
    {
        *(void**)((U32)retval + next) = pool->UsedList;
        pool->UsedList = retval;
    }

    return retval;
}

void xMemPoolFree(xMemPool* pool, void* data)
{
    void** prev;
    void* freeList;
    U32 next;
    void* curr;

    if (data == NULL)
    {
        return;
    }

    freeList = pool->FreeList;
    next = pool->NextOffset;
    if (pool->Flags & 1)
    {
        prev = &pool->UsedList;
        curr = pool->UsedList;

        while (curr != NULL && curr != data)
        {
            prev = (void**)((U32)curr + next);
            curr = *prev;
        }

        if (curr != NULL)
        {
            *prev = *(void**)((U32)curr + next);
        }
    }

    *(void**)((U32)data + next) = freeList;
    pool->FreeList = data;
}
