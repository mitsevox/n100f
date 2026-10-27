#ifndef _DOLPHIN_PPCARCH_H_
#define _DOLPHIN_PPCARCH_H_

#include <dolphin/types.h>

#ifdef __cplusplus
extern "C" {
#endif

#define HID0_ICE 0x00008000u
#define HID0_DCE 0x00004000u

#define HID2_DCHERR 0x00800000
#define HID2_DNCERR 0x00400000
#define HID2_DCMERR 0x00200000
#define HID2_DQOERR 0x00100000

#define SRR1_DMA_BIT 0x00200000

#define L2CR_L2E 0x80000000
#define L2CR_L2I 0x00200000

#define MSR_IR 0x00000020
#define MSR_DR 0x00000010
#define MSR_RI 0x00000002

#define MSR_RI_BIT 30

u32 PPCMfmsr(void);
void PPCMtmsr(u32 newMSR);
u32 PPCMfhid0(void);
u32 PPCMfl2cr(void);
void PPCMtl2cr(u32 newL2cr);
u32 PPCMfhid2(void);
void PPCMthid2(u32 newhid2);
u32 PPCMfwpar(void);
void PPCMtwpar(u32 newwpar);
void PPCMtdec(u32 newDec);
void PPCSync(void);
void PPCDisableSpeculation(void);
void PPCSetFpNonIEEEMode(void);
void PPCHalt(void);

#ifdef __cplusplus
}
#endif

#endif
