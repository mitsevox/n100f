#ifndef _DOLPHIN_AX_H_
#define _DOLPHIN_AX_H_

#include <dolphin/types.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _AXPBDPOP {
    s16 aL;
    s16 aAuxAL;
    s16 aAuxBL;
    s16 aR;
    s16 aAuxAR;
    s16 aAuxBR;
    s16 aS;
    s16 aAuxAS;
    s16 aAuxBS;
} AXPBDPOP;

// Only the depop block is declared; the parameter block's other fields are not used yet.
typedef struct _AXPB {
    u8 unk00[0x52];
    AXPBDPOP dpop;
} AXPB;

typedef struct _AXSPB {
    u16 dpopLHi;
    u16 dpopLLo;
    s16 dpopLDelta;
    u16 dpopRHi;
    u16 dpopRLo;
    s16 dpopRDelta;
    u16 dpopSHi;
    u16 dpopSLo;
    s16 dpopSDelta;
    u16 dpopALHi;
    u16 dpopALLo;
    s16 dpopALDelta;
    u16 dpopARHi;
    u16 dpopARLo;
    s16 dpopARDelta;
    u16 dpopASHi;
    u16 dpopASLo;
    s16 dpopASDelta;
    u16 dpopBLHi;
    u16 dpopBLLo;
    s16 dpopBLDelta;
    u16 dpopBRHi;
    u16 dpopBRLo;
    s16 dpopBRDelta;
    u16 dpopBSHi;
    u16 dpopBSLo;
    s16 dpopBSDelta;
} AXSPB;

typedef struct _AXPROFILE {
    u64 axFrameStart;
    u64 auxProcessingStart;
    u64 auxProcessingEnd;
    u64 userCallbackStart;
    u64 userCallbackEnd;
    u64 axFrameEnd;
    u32 axNumVoices;
} AXPROFILE;

u32 __AXGetStudio(void);
void __AXPrintStudio(void);
void __AXSPBInit(void);
void __AXSPBQuit(void);
void __AXDepopVoice(AXPB* p);

AXPROFILE* __AXGetCurrentProfile(void);

#ifdef __cplusplus
}
#endif

#endif
