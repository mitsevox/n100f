#ifndef _DOLPHIN_AI_H_
#define _DOLPHIN_AI_H_

#include <dolphin/types.h>

#ifdef __cplusplus
extern "C" {
#endif

extern volatile u32 __AIRegs[] AT_ADDRESS(0xCC006C00);

typedef void (*AISCallback)(u32 count);
typedef void (*AIDCallback)(void);

AIDCallback AIRegisterDMACallback(AIDCallback callback);
void AIInitDMA(u32 start_addr, u32 length);
void AIStartDMA(void);
void AIStopDMA(void);
void AISetStreamPlayState(u32 state);
u32 AIGetStreamPlayState(void);
void AISetDSPSampleRate(u32 rate);
u32 AIGetDSPSampleRate(void);
u32 AIGetStreamSampleRate(void);
void AISetStreamVolLeft(u8 vol);
u8 AIGetStreamVolLeft(void);
void AISetStreamVolRight(u8 vol);
u8 AIGetStreamVolRight(void);
void AIInit(u8* stack);

#ifdef __cplusplus
}
#endif

#endif
