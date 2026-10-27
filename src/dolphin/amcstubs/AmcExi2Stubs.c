#include <dolphin/amcstubs/AmcExi2Stubs.h>

void EXI2_Init(volatile u8** inputPendingPtrRef, AmcEXICallback monitorCallback)
{
}

void EXI2_EnableInterrupts(void)
{
}

int EXI2_Poll(void)
{
    return 0;
}

int EXI2_ReadN(void* bytes, u32 length)
{
    return 0;
}

int EXI2_WriteN(const void* bytes, u32 length)
{
    return 0;
}

void EXI2_Reserve(void)
{
}

void EXI2_Unreserve(void)
{
}

BOOL AMC_IsStub(void)
{
    return TRUE;
}
