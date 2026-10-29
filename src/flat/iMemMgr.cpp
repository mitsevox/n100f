#include "iMemMgr.h"

#include "xMemMgr.h"

#include <dolphin/os.h>
#include <stdlib.h>

extern U8 _stack_addr[];
extern U8 _stack_end[];

U32 mem_top_alloc;
U32 mem_base_alloc;
OSHeapHandle the_heap;
U32 hs;
U32 he;

void iMemInit()
{
    void* p;
    U32 hi = (U32)OSGetArenaHi() & ~0x1F;

    he = hi;
    hs = ((U32)OSGetArenaLo() + 0x1F) & ~0x1F;

    the_heap = OSCreateHeap(OSInitAlloc((void*)hs, (void*)hi, 1), (void*)he);

    if (the_heap >= 0)
    {
        OSSetCurrentHeap(the_heap);
    }
    else
    {
        exit(-5);
    }

    p = OSAllocFromHeap(__OSCurrHeap, 0x641000);

    mem_base_alloc = (U32)p;
    mem_top_alloc = (U32)p + 0x641000;

    gMemInfo.system.addr = 0;
    gMemInfo.system.size = 0x100000;
    gMemInfo.system.flags = 0x20;
    gMemInfo.stack.addr = (U32)_stack_addr;
    gMemInfo.stack.size = _stack_end - _stack_addr;
    gMemInfo.stack.flags = 0x820;
    gMemInfo.DRAM.addr = (U32)p;
    gMemInfo.DRAM.size = 0x641000;
    gMemInfo.DRAM.flags = 0x820;
    gMemInfo.SRAM.addr = 0;
    gMemInfo.SRAM.size = 0x200000;
    gMemInfo.SRAM.flags = 0x660;
}

void iMemExit()
{
    free((void*)gMemInfo.DRAM.addr);
    gMemInfo.DRAM.addr = 0;
}
