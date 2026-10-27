#include <rwcore.h>

typedef struct rwResHeap rwResHeap;
typedef struct rwResHeapEntry rwResHeapEntry;

struct rwResHeap
{
    rwResHeapEntry* entries;
    rwResHeapEntry* freeEntries;
};

struct rwResHeapEntry
{
    rwResHeap* heap;
    rwResHeapEntry* next;
    rwResHeapEntry* prev;
    U32 size;
    S32 used;
    U8 pad[0xC];
};

S32 _rwResHeapInit(void* mem, U32 size)
{
    rwResHeap* heap = (rwResHeap*)mem;
    rwResHeapEntry* entry = (rwResHeapEntry*)(((U32)mem + sizeof(rwResHeap) + 31) & ~31);
    S32 freeSize = (S32)((((U32)mem + size) & ~31) - (U32)entry) - sizeof(rwResHeapEntry);

    if (freeSize < 32)
    {
        return 0;
    }

    entry->heap = heap;
    entry->next = NULL;
    entry->prev = NULL;
    entry->used = 0;
    entry->size = freeSize;
    heap->entries = entry;
    heap->freeEntries = entry;

    return 1;
}

S32 _rwResHeapClose(void* mem)
{
    return 1;
}

void _rwResHeapFree(void* mem)
{
    rwResHeapEntry* entry = (rwResHeapEntry*)mem - 1;
    rwResHeap* heap;
    rwResHeapEntry* prev;
    rwResHeapEntry* next;

    entry->used = 0;
    heap = entry->heap;
    prev = entry->prev;
    next = entry->next;

    if (heap->freeEntries == NULL || entry < heap->freeEntries)
    {
        heap->freeEntries = entry;
    }

    if (prev && (~prev->used & 1))
    {
        prev->next = next;
        if (next)
        {
            next->prev = prev;
        }
        prev->size += entry->size + sizeof(rwResHeapEntry);
        entry = prev;
    }

    if (next && (~next->used & 1))
    {
        entry->next = next->next;
        if (next->next)
        {
            next->next->prev = entry;
        }
        entry->size += next->size + sizeof(rwResHeapEntry);
    }
}

void* _rwResHeapAlloc(void* mem, U32 size)
{
    rwResHeap* heap = (rwResHeap*)mem;
    rwResHeapEntry* entry;
    rwResHeapEntry* best = NULL;

    size = (size + 31) & ~31;

    for (entry = heap->freeEntries; entry; entry = entry->next)
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

    if (best->size > size + 2 * sizeof(rwResHeapEntry))
    {
        rwResHeapEntry* split = (rwResHeapEntry*)((U8*)best + size + sizeof(rwResHeapEntry));

        if (best->next && (~best->next->used & 1))
        {
            split->next = best->next->next;
            split->size = best->next->size + (best->size - size);
        }
        else
        {
            split->next = best->next;
            split->size = best->size - size - sizeof(rwResHeapEntry);
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

    if (best == heap->freeEntries)
    {
        do
        {
            heap->freeEntries = heap->freeEntries->next;
        } while (heap->freeEntries && (heap->freeEntries->used & 1));
    }

    best->used = 1;

    return best + 1;
}
