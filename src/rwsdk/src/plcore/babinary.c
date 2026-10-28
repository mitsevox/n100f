#include <rwcore.h>

void* RwMemLittleEndian32(void* mem, U32 size)
{
    U32* word = (U32*)mem;
    U32 count = size >> 2;

    while (count--)
    {
        __stwbrx(*word, word, 0);
        word++;
    }

    return mem;
}

void* RwMemLittleEndian16(void* mem, U32 size)
{
    U16* half = (U16*)mem;
    U32 count = size >> 1;

    while (count--)
    {
        *half = (*half >> 8) | (*half << 8);
        half++;
    }

    return mem;
}

void* RwMemNative32(void* mem, U32 size)
{
    U32* word = (U32*)mem;
    U32 count = size >> 2;

    while (count--)
    {
        __stwbrx(*word, word, 0);
        word++;
    }

    return mem;
}

void* RwMemNative16(void* mem, U32 size)
{
    U16* half = (U16*)mem;
    U32 count = size >> 1;

    while (count--)
    {
        *half = (*half >> 8) | (*half << 8);
        half++;
    }

    return mem;
}
