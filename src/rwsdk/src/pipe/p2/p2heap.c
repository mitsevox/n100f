#include <rwcore.h>
#include <string.h>

// Blocks are rounded up to this many bytes.
#define HEAP_ALIGNMENT 32

// A free remainder smaller than this stays with the allocated block.
#define HEAP_MINFREE 256

// Superblock data starts on this boundary.
#define HEAP_SUPERBLOCKALIGNMENT 128

// The free block table grows by this many entries.
#define HEAP_FREEBLOCKSGROWTH 32

static rxHeapFreeBlock* _rxHeapFreeBlocksNewEntry(RxHeap* heap)
{
    rxHeapFreeBlock* freeBlocks = heap->freeBlocks;
    U32 numUsed = heap->entriesUsed;

    if (heap->entriesAlloced <= numUsed)
    {
        freeBlocks = (rxHeapFreeBlock*)RwRealloc(
            freeBlocks,
            (heap->entriesAlloced += HEAP_FREEBLOCKSGROWTH) * sizeof(rxHeapFreeBlock));

        if (!freeBlocks)
        {
            RwError err;

            err.pluginID = 1;
            err.errorCode =
                _rwerror(0x80000013, heap->entriesAlloced * sizeof(rxHeapFreeBlock));
            RwErrorSet(&err);

            heap->entriesAlloced -= HEAP_FREEBLOCKSGROWTH;
        }
        else
        {
            if (freeBlocks != heap->freeBlocks && numUsed)
            {
                rxHeapFreeBlock* entry = freeBlocks;

                do
                {
                    entry->ptr->freeEntry = entry;
                    entry++;
                } while (--numUsed);
            }

            heap->freeBlocks = freeBlocks;
        }
    }

    if (freeBlocks)
    {
        freeBlocks = &freeBlocks[heap->entriesUsed++];
    }

    return freeBlocks;
}

static void _rxHeapFreeBlocksDeleteEntry(RxHeap* heap, rxHeapFreeBlock* entry)
{
    if (&heap->freeBlocks[heap->entriesUsed - 1] != entry)
    {
        *entry = heap->freeBlocks[heap->entriesUsed - 1];
        entry->ptr->freeEntry = entry;
    }

    heap->entriesUsed--;
}

static rxHeapSuperBlockDescriptor* _rxHeapSuperBlockCreate(U32 size)
{
    rxHeapSuperBlockDescriptor* superBlock = (rxHeapSuperBlockDescriptor*)RwMalloc(
        size + sizeof(rxHeapSuperBlockDescriptor) + (HEAP_SUPERBLOCKALIGNMENT - 1));

    if (superBlock)
    {
        superBlock->start =
            (void*)(((U32)superBlock + sizeof(rxHeapSuperBlockDescriptor) +
                     (HEAP_SUPERBLOCKALIGNMENT - 1)) &
                    ~(HEAP_SUPERBLOCKALIGNMENT - 1));
        superBlock->size = size;
        superBlock->next = NULL;
    }

    return superBlock;
}

static void _rxHeapSuperBlockDestroy(rxHeapSuperBlockDescriptor* superBlock)
{
    if (superBlock)
    {
        RwFree(superBlock);
    }
}

static S32 _rxHeapSuperBlockReset(RxHeap* heap, rxHeapSuperBlockDescriptor* superBlock,
                                  rxHeapSuperBlockDescriptor* prevSuperBlock)
{
    rxHeapFreeBlock* entry = _rxHeapFreeBlocksNewEntry(heap);

    if (entry)
    {
        rxHeapBlockHeader* first = (rxHeapBlockHeader*)superBlock->start;
        rxHeapBlockHeader* free = first + 1;
        rxHeapBlockHeader* last = (rxHeapBlockHeader*)((U8*)first + superBlock->size -
                                                       sizeof(rxHeapBlockHeader));

        first->prev = NULL;
        first->next = NULL;
        first->size = 0;
        first->freeEntry = NULL;
        *last = *first;

        first->next = free;
        free->prev = first;
        free->next = last;
        last->prev = free;
        free->size = (U8*)last - (U8*)(free + 1);
        free->freeEntry = entry;

        entry->ptr = free;
        entry->size = free->size;

        if (prevSuperBlock)
        {
            rxHeapBlockHeader* prevLast =
                (rxHeapBlockHeader*)((U8*)prevSuperBlock->start + prevSuperBlock->size -
                                     sizeof(rxHeapBlockHeader));

            prevLast->next = first;
            first->prev = prevLast;
        }

        return TRUE;
    }

    return FALSE;
}

static rxHeapBlockHeader* _rxHeapGrow(RxHeap* heap, U32 size)
{
    rxHeapSuperBlockDescriptor* superBlock;
    U32 superBlockSize = size + 3 * sizeof(rxHeapBlockHeader);

    if (superBlockSize < heap->superBlockSize)
    {
        superBlockSize = heap->superBlockSize;
    }

    superBlock = _rxHeapSuperBlockCreate(superBlockSize);

    if (superBlock)
    {
        if (_rxHeapSuperBlockReset(heap, superBlock, heap->head))
        {
            rxHeapSuperBlockDescriptor* cur;
            U32 total = 0;
            char message[256];

            superBlock->next = heap->head;
            heap->head = superBlock;

            for (cur = heap->head; cur; cur = cur->next)
            {
                total += cur->size;
            }

            rwsprintf(message, "Heap resized from %d to %d bytes", total - superBlockSize,
                      total);

            return (rxHeapBlockHeader*)superBlock->start + 1;
        }

        _rxHeapSuperBlockDestroy(superBlock);
    }

    return NULL;
}

static void* _rxHeapAllocFrom(RxHeap* heap, rxHeapFreeBlock* freeBlock, U32 size)
{
    rxHeapBlockHeader* header;
    U32 remainder;

    remainder = freeBlock->size - size;

    if (remainder < HEAP_MINFREE)
    {
        header = freeBlock->ptr;
        _rxHeapFreeBlocksDeleteEntry(heap, freeBlock);

        header->freeEntry = NULL;

        return header + 1;
    }
    else
    {
        rxHeapBlockHeader* newHeader;

        header = freeBlock->ptr;
        newHeader = (rxHeapBlockHeader*)((U8*)(header + 1) + size);

        newHeader->size = remainder - sizeof(rxHeapBlockHeader);
        newHeader->prev = header;
        newHeader->next = header->next;
        newHeader->freeEntry = freeBlock;

        header->size = size;
        header->next = newHeader;

        if (newHeader->next)
        {
            newHeader->next->prev = newHeader;
        }

        freeBlock->ptr = newHeader;
        freeBlock->size = newHeader->size;
    }

    header->freeEntry = NULL;

    return header + 1;
}

static void* _rxHeapReallocLarger(RxHeap* heap, void* block, U32 newSize,
                                  rxHeapBlockHeader* header, U32 sizeDelta)
{
    void* newBlock;

    if (header->next && header->next->freeEntry)
    {
        U32 available = header->next->size + sizeof(rxHeapBlockHeader);

        if (available >= sizeDelta)
        {
            rxHeapFreeBlock* entry = header->next->freeEntry;
            U32 total = header->size + available;

            if (total - newSize < HEAP_MINFREE)
            {
                _rxHeapFreeBlocksDeleteEntry(heap, entry);

                header->size = total;
                header->next = header->next->next;

                if (header->next)
                {
                    header->next->prev = header;
                }
            }
            else
            {
                rxHeapBlockHeader* newHeader =
                    (rxHeapBlockHeader*)((U8*)(header + 1) + newSize);

                newHeader->freeEntry = entry;
                newHeader->size = header->next->size - sizeDelta;
                newHeader->prev = header;
                newHeader->next = header->next->next;

                if (newHeader->next)
                {
                    newHeader->next->prev = newHeader;
                }

                header->size = newSize;
                header->next = newHeader;

                newHeader->freeEntry->ptr = newHeader;
                newHeader->freeEntry->size = newHeader->size;
            }

            return block;
        }
    }

    newBlock = RxHeapAlloc(heap, newSize);

    if (newBlock)
    {
        memcpy(newBlock, block, header->size);
        RxHeapFree(heap, block);

        return newBlock;
    }

    {
        RwError err;

        err.pluginID = 1;
        err.errorCode = _rwerror(0x80000013, newSize);
        RwErrorSet(&err);
    }

    return NULL;
}

static void* _rxHeapReallocSmaller(RxHeap* heap, void* block, U32 newSize,
                                   rxHeapBlockHeader* header, S32 sizeDelta)
{
    rxHeapFreeBlock* entry;
    rxHeapBlockHeader* newHeader;

    if (sizeDelta < HEAP_MINFREE)
    {
        return block;
    }

    entry = _rxHeapFreeBlocksNewEntry(heap);

    if (!entry)
    {
        return block;
    }

    newHeader = (rxHeapBlockHeader*)((U8*)(header + 1) + newSize);

    newHeader->size = sizeDelta - sizeof(rxHeapBlockHeader);
    newHeader->prev = header;
    newHeader->next = header->next;
    newHeader->freeEntry = entry;

    header->size = newSize;
    header->next = newHeader;

    if (newHeader->next)
    {
        newHeader->next->prev = newHeader;
    }

    entry->ptr = newHeader;
    entry->size = newHeader->size;

    return block;
}

void RxHeapFree(RxHeap* heap, void* block)
{
    rxHeapBlockHeader* header = (rxHeapBlockHeader*)block - 1;
    S32 prevFree = header->prev && header->prev->freeEntry;
    S32 nextFree = header->next && header->next->freeEntry;

    if (prevFree)
    {
        if (nextFree)
        {
            _rxHeapFreeBlocksDeleteEntry(heap, header->next->freeEntry);

            header->prev->size += header->size + header->next->size + 2 * sizeof(rxHeapBlockHeader);
            header->prev->freeEntry->size = header->prev->size;
            header->prev->next = header->next->next;

            if (header->next->next)
            {
                header->next->next->prev = header->prev;
            }
        }
        else
        {
            header->prev->size += header->size + sizeof(rxHeapBlockHeader);
            header->prev->freeEntry->size = header->prev->size;
            header->prev->next = header->next;

            if (header->next)
            {
                header->next->prev = header->prev;
            }
        }
    }
    else if (nextFree)
    {
        header->size += header->next->size + sizeof(rxHeapBlockHeader);
        header->freeEntry = header->next->freeEntry;
        header->next->freeEntry->ptr = header;
        header->next->freeEntry->size = header->size;
        header->next = header->next->next;

        if (header->next)
        {
            header->next->prev = header;
        }
    }
    else
    {
        rxHeapFreeBlock* entry = _rxHeapFreeBlocksNewEntry(heap);

        if (entry)
        {
            entry->ptr = header;
            entry->size = header->size;
            header->freeEntry = entry;
        }
    }
}

void* RxHeapAlloc(RxHeap* heap, U32 size)
{
    U32 i;
    rxHeapFreeBlock* freeBlock;
    rxHeapBlockHeader* header;

    size = (size + (HEAP_ALIGNMENT - 1)) & ~(HEAP_ALIGNMENT - 1);

    freeBlock = &heap->freeBlocks[heap->entriesUsed - 1];

    i = heap->entriesUsed;

    do
    {
        if (freeBlock->size >= size)
        {
            heap->dirty = TRUE;

            return _rxHeapAllocFrom(heap, freeBlock, size);
        }

        freeBlock--;
    } while (--i);

    header = _rxHeapGrow(heap, size);

    if (header)
    {
        heap->dirty = TRUE;

        return _rxHeapAllocFrom(heap, header->freeEntry, size);
    }

    {
        RwError err;

        err.pluginID = 1;
        err.errorCode = _rwerror(0x80000013, size);
        RwErrorSet(&err);
    }

    return NULL;
}

static void* _rxHeapRealloc(RxHeap* heap, void* block, U32 newSize)
{
    rxHeapBlockHeader* header = (rxHeapBlockHeader*)block - 1;
    S32 sizeDelta = newSize - header->size;

    if (sizeDelta > 0)
    {
        return _rxHeapReallocLarger(heap, block, newSize, header, sizeDelta);
    }

    return _rxHeapReallocSmaller(heap, block, newSize, header, -sizeDelta);
}

void* RxHeapRealloc(RxHeap* heap, void* block, U32 newSize, S32 allowCopy)
{
    newSize = (newSize + (HEAP_ALIGNMENT - 1)) & ~(HEAP_ALIGNMENT - 1);

    return _rxHeapRealloc(heap, block, newSize);
}

S32 _rxHeapReset(RxHeap* heap)
{
    rxHeapSuperBlockDescriptor* prevSuperBlock = NULL;
    rxHeapSuperBlockDescriptor* superBlock;

    heap->entriesUsed = 0;

    for (superBlock = heap->head->next; superBlock; superBlock = superBlock->next)
    {
        if (!_rxHeapSuperBlockReset(heap, superBlock, prevSuperBlock))
        {
            return FALSE;
        }

        if (!prevSuperBlock)
        {
            heap->headBlock = (rxHeapBlockHeader*)superBlock->start;
        }

        prevSuperBlock = superBlock;
    }

    superBlock = heap->head;

    if (!_rxHeapSuperBlockReset(heap, superBlock, prevSuperBlock))
    {
        return FALSE;
    }

    if (!prevSuperBlock)
    {
        heap->headBlock = (rxHeapBlockHeader*)superBlock->start;
    }

    heap->dirty = FALSE;

    return TRUE;
}

void RxHeapDestroy(RxHeap* heap)
{
    if (heap)
    {
        rxHeapSuperBlockDescriptor* superBlock;

        if (heap->freeBlocks)
        {
            RwFree(heap->freeBlocks);
            heap->freeBlocks = NULL;
        }

        superBlock = heap->head;

        while (superBlock)
        {
            rxHeapSuperBlockDescriptor* next = superBlock->next;

            _rxHeapSuperBlockDestroy(superBlock);
            superBlock = next;
        }

        RwFree(heap);
    }
}

RxHeap* RxHeapCreate(U32 size)
{
    RxHeap* heap;

    if (size < 0x400)
    {
        size = 0x400;
    }

    heap = (RxHeap*)RwMalloc(sizeof(RxHeap));

    if (heap)
    {
        rxHeapSuperBlockDescriptor* superBlock;

        size = (size + (HEAP_ALIGNMENT - 1)) & ~(HEAP_ALIGNMENT - 1);

        if (size < 0x80)
        {
            size = 0x80;
        }

        superBlock = _rxHeapSuperBlockCreate(size);

        if (superBlock)
        {
            heap->superBlockSize = size;
            heap->head = superBlock;
            heap->freeBlocks = NULL;
            heap->entriesAlloced = 0;
            heap->entriesUsed = 0;
            heap->dirty = TRUE;

            if (!heap->dirty ? TRUE : _rxHeapReset(heap))
            {
                return heap;
            }

            _rxHeapSuperBlockDestroy(superBlock);
        }

        RwFree(heap);
    }

    return NULL;
}
