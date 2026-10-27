#include <rwcore.h>

typedef struct ResHeap ResHeap;
typedef struct ResHeapBlock ResHeapBlock;

struct ResHeap
{
    ResHeapBlock* first;
    ResHeapBlock* firstFree;
};

struct ResHeapBlock
{
    ResHeap* heap;
    ResHeapBlock* next;
    ResHeapBlock* prev;
    U32 size;
    S32 used;
    U8 pad[0xC];
};

S32 _rwResHeapInit(void* mem, U32 size)
{
    ResHeap* heap = (ResHeap*)mem;
    ResHeapBlock* entry = (ResHeapBlock*)(((U32)mem + sizeof(ResHeap) + 31) & ~31);
    S32 freeSize = (S32)((((U32)mem + size) & ~31) - (U32)entry) - sizeof(ResHeapBlock);

    if (freeSize < 32)
    {
        return 0;
    }

    entry->heap = heap;
    entry->next = NULL;
    entry->prev = NULL;
    entry->used = 0;
    entry->size = freeSize;
    heap->first = entry;
    heap->firstFree = entry;

    return 1;
}

S32 _rwResHeapClose(void* mem)
{
    return 1;
}

void _rwResHeapFree(void* mem)
{
    ResHeapBlock* entry = (ResHeapBlock*)mem - 1;
    ResHeap* heap;
    ResHeapBlock* prev;
    ResHeapBlock* next;

    entry->used = 0;
    heap = entry->heap;
    prev = entry->prev;
    next = entry->next;

    if (heap->firstFree == NULL || entry < heap->firstFree)
    {
        heap->firstFree = entry;
    }

    if (prev && (~prev->used & 1))
    {
        prev->next = next;
        if (next)
        {
            next->prev = prev;
        }
        prev->size += entry->size + sizeof(ResHeapBlock);
        entry = prev;
    }

    if (next && (~next->used & 1))
    {
        entry->next = next->next;
        if (next->next)
        {
            next->next->prev = entry;
        }
        entry->size += next->size + sizeof(ResHeapBlock);
    }
}

void* _rwResHeapAlloc(void* mem, U32 size)
{
    ResHeap* heap = (ResHeap*)mem;
    ResHeapBlock* entry;
    ResHeapBlock* best = NULL;

    size = (size + 31) & ~31;

    for (entry = heap->firstFree; entry; entry = entry->next)
    {
        if ((~entry->used & 1) && entry->size >= size && (!best || entry->size < best->size))
        {
            best = entry;
        }
    }

    if (!best)
    {
        return NULL;
    }

    if (best->size > size + 2 * sizeof(ResHeapBlock))
    {
        ResHeapBlock* split = (ResHeapBlock*)((U8*)best + size + sizeof(ResHeapBlock));

        if (best->next && (~best->next->used & 1))
        {
            split->next = best->next->next;
            split->size = best->next->size + (best->size - size);
        }
        else
        {
            split->next = best->next;
            split->size = best->size - size - sizeof(ResHeapBlock);
        }

        best->next = split;
        split->used = 0;
        split->prev = best;
        if (split->next)
        {
            split->next->prev = split;
        }
        best->size = size;
        split->heap = best->heap;
    }

    if (best == heap->firstFree)
    {
        do
        {
            heap->firstFree = heap->firstFree->next;
        } while (heap->firstFree && (heap->firstFree->used & 1));
    }

    best->used = 1;

    return best + 1;
}
