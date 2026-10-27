#ifndef _DOLPHIN_VI_H_
#define _DOLPHIN_VI_H_

#include <dolphin/types.h>

#ifdef __cplusplus
extern "C" {
#endif

#define VI_NTSC 0
#define VI_PAL 1
#define VI_MPAL 2
#define VI_EURGB60 5

extern volatile u16 __VIRegs[] AT_ADDRESS(0xCC002000);

u32 VIGetTvFormat(void);

#ifdef __cplusplus
}
#endif

#endif
