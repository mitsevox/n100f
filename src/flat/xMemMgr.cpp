#include "xMemMgr.h"
#include "iMemMgr.h"

#include "xDebug.h"

#include <stdlib.h>
#include <string.h>

struct xMemBlock_tag
{
    U32 addr;
    U32 size;
};

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

// The empty debug-mode callback that xMemInit registers; it follows xMemInit and has no known name.
extern "C" void fn_800300A4();

// Bit 0x100 makes the heap grow down from the top of its area. The alignment is 1 << bits 9-13;
// bit 0x10000 keeps the block headers in a separate table.
inline void xMemInitHeap(xMemHeap_tag* heap, U32 base, U32 size, U32 flags)
{
    U32 old_base = base;
    U32 align = 1 << ((flags >> 9) & 0x1F);
    if (flags & 0x100)
    {
        base = base & -align;
        size -= old_base - base;
    }
    else
    {
        base = (base + align - 1) & -align;
        size -= base - old_base;
    }

    heap->state_idx = 0;
    heap->hard_base = base;
    heap->size = size;
    heap->flags = flags;
    heap->lastblk = NULL;
    xHeapState_tag* sp = &heap->state[heap->state_idx];
    sp->curr = base;
    sp->blk_ct = 0;
    heap->max_blks = -1;
    heap->blk = NULL;
    heap->opp_heap[0] = -1;
    heap->opp_heap[1] = -1;
}

// Heap 0 grows up from the start of game memory; heaps 1 and 2 grow down from its end.
void xMemInit()
{
    iMemInit();
    xMemInitHeap(&gxHeap[0], gMemInfo.DRAM.addr, gMemInfo.DRAM.size, 0x8826);
    xMemInitHeap(&gxHeap[1], gMemInfo.DRAM.addr + gMemInfo.DRAM.size, gMemInfo.DRAM.size, 0x8925);
    xMemInitHeap(&gxHeap[2], gMemInfo.DRAM.addr + gMemInfo.DRAM.size, gMemInfo.DRAM.size, 0x892A);
    gxHeap[0].opp_heap[0] = 1;
    gxHeap[0].opp_heap[1] = 2;
    gxHeap[1].opp_heap[0] = 0;
    gxHeap[1].opp_heap[1] = 2;
    gxHeap[2].opp_heap[0] = 0;
    gxHeap[2].opp_heap[1] = 1;
    gActiveHeap = 0;
    xDebugModeRegisterCallback(11, fn_800300A4);
}

void xMemExit()
{
    iMemExit();
}

#define XMEM_ALIGN(x, align) (((x) + (align) - 1) & -(align))

// Grows the heap's last block in place by size bytes, cleared, and returns the new part.
void* xMemGrowAlloc(U32 heapID, U32 size)
{
    void* memptr;
    xHeapState_tag* sp;
    xMemBlock_tag* hdr;
    xMemHeap_tag* heap = &gxHeap[heapID];
    U32 oldalignsize;
    U32 align;

    hdr = heap->lastblk;
    align = 1 << ((heap->flags >> 9) & 0x1F);
    sp = &heap->state[heap->state_idx];
    oldalignsize = XMEM_ALIGN(hdr->size, align);
    align = XMEM_ALIGN(hdr->size + size, align) - oldalignsize;
    if (sp->used + align > heap->size)
    {
        return NULL;
    }

    if (heap->flags & 0x100)
    {
        memptr = (void*)(hdr->addr - size);
        hdr->addr = (U32)memptr;
        sp->curr -= align;
    }
    else
    {
        sp->curr += align;
        memptr = (void*)(hdr->addr + hdr->size);
    }

    sp->used += align;
    hdr->size += size;
    memset(memptr, 0, size);
    return memptr;
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
