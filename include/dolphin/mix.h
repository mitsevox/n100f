#ifndef _DOLPHIN_MIX_H_
#define _DOLPHIN_MIX_H_

#include <dolphin/types.h>
#include <dolphin/ax.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct MIXChannel
{
    AXVPB* axvpb;
    u32 mode;
    int input;
    int auxA;
    int auxB;
    int pan;
    int span;
    int fader;
    int l;
    int r;
    int f;
    int b;
    u16 v;
    u16 v1;
    u16 vL;
    u16 vL1;
    u16 vR;
    u16 vR1;
    u16 vS;
    u16 vS1;
    u16 vAL;
    u16 vAL1;
    u16 vAR;
    u16 vAR1;
    u16 vAS;
    u16 vAS1;
    u16 vBL;
    u16 vBL1;
    u16 vBR;
    u16 vBR1;
    u16 vBS;
    u16 vBS1;
} MIXChannel;

void MIXInit(void);
void MIXInitChannel(AXVPB* p, u32 mode, int input, int auxA, int auxB, int pan, int span,
                    int fader);
void MIXReleaseChannel(AXVPB* p);
void MIXAdjustInput(AXVPB* p, int adjust);
int MIXGetInput(AXVPB* p);
void MIXAdjustPan(AXVPB* p, int adjust);
int MIXGetPan(AXVPB* p);
void MIXUnMute(AXVPB* p);
void MIXAdjustFader(AXVPB* p, int adjust);
int MIXGetFader(AXVPB* p);
void MIXUpdateSettings(void);

#ifdef __cplusplus
}
#endif

#endif
