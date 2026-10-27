#ifndef _DOLPHIN_EXI_H_
#define _DOLPHIN_EXI_H_

#include <dolphin/types.h>
#include <dolphin/os/OSContext.h>

#ifdef __cplusplus
extern "C" {
#endif

extern volatile u32 __EXIRegs[] AT_ADDRESS(0xCC006800);

#define EXI_READ 0
#define EXI_WRITE 1

typedef void (*EXICallback)(s32 chan, OSContext* context);

EXICallback EXISetExiCallback(s32 chan, EXICallback exiCallback);
BOOL EXILock(s32 chan, u32 dev, EXICallback unlockedCallback);
BOOL EXIUnlock(s32 chan);
BOOL EXISelect(s32 chan, u32 dev, u32 freq);
BOOL EXIDeselect(s32 chan);
BOOL EXIImm(s32 chan, void* buf, s32 len, u32 type, EXICallback callback);
BOOL EXIImmEx(s32 chan, void* buf, s32 len, u32 mode);
BOOL EXIDma(s32 chan, void* buf, s32 len, u32 type, EXICallback callback);
BOOL EXISync(s32 chan);
BOOL EXIProbe(s32 chan);
s32 EXIProbeEx(s32 chan);
BOOL EXIAttach(s32 chan, EXICallback extCallback);
BOOL EXIDetach(s32 chan);
u32 EXIGetState(s32 chan);
s32 EXIGetID(s32 chan, u32 dev, u32* id);

void EXIInit(void);

#ifdef __cplusplus
}
#endif

#endif
