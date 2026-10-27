#ifndef _DOLPHIN_EXI_H_
#define _DOLPHIN_EXI_H_

#include <dolphin/types.h>

#ifdef __cplusplus
extern "C" {
#endif

extern volatile u32 __EXIRegs[] AT_ADDRESS(0xCC006800);

#ifdef __cplusplus
}
#endif

#endif
