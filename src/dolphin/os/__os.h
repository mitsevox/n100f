#ifndef _DOLPHIN_OS_PRIVATE_H_
#define _DOLPHIN_OS_PRIVATE_H_

#include <dolphin/os.h>

extern u32 __OSPhysicalMemSize AT_ADDRESS(OS_BASE_CACHED | 0x0028);
extern u32 __OSSimulatedMemSize AT_ADDRESS(OS_BASE_CACHED | 0x00F0);
extern volatile int __OSTVMode AT_ADDRESS(OS_BASE_CACHED | 0x00CC);
extern volatile u16 __OSDeviceCode AT_ADDRESS(OS_BASE_CACHED | 0x30E6);

extern volatile u16 __MEMRegs[] AT_ADDRESS(0xCC004000);

#define __OS_EXCEPTION_DSI 2
#define __OS_EXCEPTION_ISI 3
#define __OS_EXCEPTION_ALIGNMENT 5
#define __OS_EXCEPTION_PROGRAM 6
#define __OS_EXCEPTION_FLOATING_POINT 7
#define __OS_EXCEPTION_DECREMENTER 8
#define __OS_EXCEPTION_MAX 15

extern volatile u32 __DIRegs[] AT_ADDRESS(0xCC006000);

extern volatile __OSInterrupt __OSLastInterrupt;
extern volatile u32 __OSLastInterruptSrr0;
extern volatile OSTime __OSLastInterruptTime;

extern void* BOOT_REGION_START AT_ADDRESS(0x812FDFF0);
extern void* BOOT_REGION_END AT_ADDRESS(0x812FDFEC);

typedef struct BI2Debug {
    s32 debugMonSize;
    s32 simMemSize;
    u32 argOffset;
    u32 debugFlag;
    int trackLocation;
    int trackSize;
    u32 countryCode;
    u8 unk[8];
    u32 padSpec;
} BI2Debug;

void __OSCacheInit(void);
u32 __OSGetDIConfig(void);
void __OSContextInit(void);
void __OSInitAudioSystem(void);
void __OSInitMemoryProtection(void);
void __OSInitSram(void);
void __OSInitSystemCall(void);
void __OSInterruptInit(void);
void __OSModuleInit(void);
void __OSThreadInit(void);
void __OSResetSWInterruptHandler(__OSInterrupt interrupt, OSContext* context);
void __OSReschedule(void);
void __OSUnlockAllMutex(OSThread* thread);

extern volatile OSContext* __OSFPUContext AT_ADDRESS(OS_BASE_CACHED | 0x00D8);
void __OSDoHotReset(u32 resetCode);
void __OSReboot(u32 resetCode, u32 bootDol);
void __OSStopAudioSystem(void);

extern OSThreadQueue __OSActiveThreadQueue AT_ADDRESS(OS_BASE_CACHED | 0x00DC);

OSTime __OSGetSystemTime(void);
OSTime __OSTimeToSystemTime(OSTime time);

void __OSUnhandledException(u8 exception, OSContext* context, u32 dsisr, u32 dar);

#endif
