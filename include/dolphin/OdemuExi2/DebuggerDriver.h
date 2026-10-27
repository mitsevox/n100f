#ifndef _DOLPHIN_ODEMUEXI2_DEBUGGERDRIVER_H_
#define _DOLPHIN_ODEMUEXI2_DEBUGGERDRIVER_H_

#include <dolphin/types.h>
#include <dolphin/os/OSInterrupt.h>

#ifdef __cplusplus
extern "C" {
#endif

void DBInitComm(volatile u8** inputPendingPtrRef, __OSInterruptHandler mtrCallback);
void DBInitInterrupts(void);
u32 DBQueryData(void);
BOOL DBRead(void* data, u32 size);
BOOL DBWrite(const void* data, u32 size);
void DBOpen(void);
void DBClose(void);
BOOL Hu_IsStub(void);

#ifdef __cplusplus
}
#endif

#endif
