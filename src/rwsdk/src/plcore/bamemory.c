#include <rwcore.h>
#include <stdlib.h>
#include <string.h>

// Free lists: pools of equal-sized entries carved out of blocks. Each list keeps a stack of its
// free entries, which grows by one block's worth of entries whenever a new block is allocated.

void* realloc(void* ptr, size_t size);

S32 _rwMemoryOpen(const RwMemoryFunctions* memFuncs);
void _rwMemoryClose(void);

// Every free list, so that they can be found from anywhere.
static RwLinkList llGAllFreeLists;

// The engine's default calloc, built on malloc.
static void* FakeCalloc(U32 numObj, U32 sizeObj)
{
    U32 size = numObj * sizeObj;
    void* mem = malloc(size);

    if (mem)
    {
        memset(mem, 0, size);
    }

    return mem;
}

S32 RwFreeListDestroy(RwFreeList* freeList)
{
    RwFreeBlock* block;

    if (freeList->freeListStack)
    {
        RwFree(freeList->freeListStack);
    }

    block = freeList->firstBlock;
    while (block)
    {
        RwFreeBlock* next = block->nextBlock;

        RwFree(block);
        block = next;
    }

    rwLinkListRemoveLLLink(&freeList->lFreeList);
    RwFree(freeList);

    return TRUE;
}

RwFreeList* RwFreeListCreate(S32 entrySize, S32 entriesPerBlock, S32 alignment)
{
    RwFreeList* freeList = (RwFreeList*)RwMalloc(sizeof(RwFreeList));

    if (freeList)
    {
        S32 size;

        freeList->alignmentMinusOne = ((alignment > 1) ? alignment : 1) - 1;
        size = (entrySize + freeList->alignmentMinusOne) & ~freeList->alignmentMinusOne;

        freeList->entrySize = size;
        freeList->entriesPerBlock = entriesPerBlock;
        freeList->blockSize = size * entriesPerBlock + sizeof(RwFreeBlock);
        freeList->blockSize += freeList->alignmentMinusOne;

        freeList->freeListStack = (void**)RwMalloc(sizeof(void*));
        if (freeList->freeListStack)
        {
            freeList->freeListStackTop = freeList->freeListStack - 1;
            freeList->entriesAllocated = 0;
            freeList->firstBlock = NULL;
            rwLinkListAddLLLink(&llGAllFreeLists, &freeList->lFreeList);
        }
        else
        {
            RwError err;
            err.pluginID = 1;
            err.errorCode = _rwerror(0x80000013, sizeof(void*));
            RwErrorSet(&err);
            RwFree(freeList);
            freeList = NULL;
        }
    }
    else
    {
        RwError err;
        err.pluginID = 1;
        err.errorCode = _rwerror(0x80000013, sizeof(RwFreeList));
        RwErrorSet(&err);
        freeList = NULL;
    }

    return freeList;
}

RwFreeList* _rwFreeListFreeReal(RwFreeList* freeList, void* pData)
{
    *++freeList->freeListStackTop = pData;

    return freeList;
}

void* _rwFreeListAllocReal(RwFreeList* freeList)
{
    if (freeList->freeListStackTop < freeList->freeListStack)
    {
        RwFreeBlock* block;
        void** stack;
        U32 stackSize;
        U8* entry;
        S32 count;

        block = (RwFreeBlock*)RwMalloc(freeList->blockSize);
        if (!block)
        {
            RwError err;
            err.pluginID = 1;
            err.errorCode = _rwerror(0x80000013, freeList->blockSize);
            RwErrorSet(&err);
            return NULL;
        }

        stackSize = (freeList->entriesAllocated + freeList->entriesPerBlock) * sizeof(void*);
        stack = (void**)RwMalloc(stackSize);
        if (!stack)
        {
            RwError err;
            err.pluginID = 1;
            err.errorCode = _rwerror(0x80000013, stackSize);
            RwErrorSet(&err);
            RwFree(block);
            return NULL;
        }

        RwFree(freeList->freeListStack);
        freeList->freeListStack = stack;
        freeList->entriesAllocated += freeList->entriesPerBlock;

        block->nextBlock = freeList->firstBlock;
        freeList->firstBlock = block;

        count = freeList->entriesPerBlock;
        entry = (U8*)(((U32)block + (freeList->blockSize - freeList->entrySize)) &
                      ~freeList->alignmentMinusOne);
        while (count--)
        {
            *stack++ = entry;
            entry -= freeList->entrySize;
        }

        freeList->freeListStackTop = freeList->freeListStack + (freeList->entriesPerBlock - 1);
    }

    return *freeList->freeListStackTop--;
}

RwFreeList* RwFreeListForAllUsed(RwFreeList* freeList, void (*callBack)(void* mem, void* data),
                                 void* data)
{
    RwFreeBlock* block;

    for (block = freeList->firstBlock; block; block = block->nextBlock)
    {
        S32 count = freeList->entriesPerBlock;
        U8* entry = (U8*)(((U32)block + freeList->alignmentMinusOne + sizeof(RwFreeBlock)) &
                          ~freeList->alignmentMinusOne);

        while (count--)
        {
            void** cur;

            for (cur = freeList->freeListStack; cur <= freeList->freeListStackTop; cur++)
            {
                if (*cur == entry)
                {
                    break;
                }
            }

            if (cur > freeList->freeListStackTop)
            {
                callBack(entry, data);
            }

            entry += freeList->entrySize;
        }
    }

    return freeList;
}

void _rwMemoryClose(void)
{
}

S32 _rwMemoryOpen(const RwMemoryFunctions* memFuncs)
{
    rwLinkListInitialize(&llGAllFreeLists);

    if (memFuncs)
    {
        RWSRCGLOBAL(memoryFuncs) = *memFuncs;
    }
    else
    {
        // The C library's functions take size_t (unsigned long here), the engine's U32.
        RWSRCGLOBAL(memoryFuncs).rwmalloc = (void* (*)(U32))malloc;
        RWSRCGLOBAL(memoryFuncs).rwfree = free;
        RWSRCGLOBAL(memoryFuncs).rwrealloc = (void* (*)(void*, U32))realloc;
        RWSRCGLOBAL(memoryFuncs).rwcalloc = FakeCalloc;
    }

    return TRUE;
}
