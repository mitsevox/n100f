#ifndef _DOLPHIN_AMCEXI2STUBS_H_
#define _DOLPHIN_AMCEXI2STUBS_H_

#include <dolphin/types.h>
#include <dolphin/os/OSInterrupt.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef __OSInterruptHandler AmcEXICallback;

void EXI2_Init(volatile u8** inputPendingPtrRef, AmcEXICallback monitorCallback);
void EXI2_EnableInterrupts(void);
int EXI2_Poll(void);
int EXI2_ReadN(void* bytes, u32 length);
int EXI2_WriteN(const void* bytes, u32 length);
void EXI2_Reserve(void);
void EXI2_Unreserve(void);
BOOL AMC_IsStub(void);

#ifdef __cplusplus
}
#endif

#endif
