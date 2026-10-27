#ifndef _DOLPHIN_DVD_PRIVATE_H_
#define _DOLPHIN_DVD_PRIVATE_H_

#include <dolphin/dvd.h>
#include <dolphin/os.h>

typedef struct DVDBB2 {
    u32 bootFilePosition;
    u32 FSTPosition;
    u32 FSTLength;
    u32 FSTMaxLength;
    void* FSTAddress;
    u32 userPosition;
    u32 userLength;
    u32 padding0;
} DVDBB2;

void __DVDPrepareResetAsync(DVDCBCallback callback);
BOOL DVDReadAbsAsyncPrio(DVDCommandBlock* block, void* addr, s32 length, s32 offset, DVDCBCallback callback, s32 prio);
BOOL DVDSeekAbsAsyncPrio(DVDCommandBlock* block, s32 offset, DVDCBCallback callback, s32 prio);

extern OSThreadQueue __DVDThreadQueue;
extern u32 __DVDLongFileNameFlag;

void __DVDFSInit(void);

void __DVDInitWA(void);
void __DVDInterruptHandler(__OSInterrupt interrupt, OSContext* context);
void __DVDLowSetWAType(u32 type, u32 location);
BOOL DVDLowRead(void* addr, u32 length, u32 offset, DVDLowCallback callback);
BOOL DVDLowSeek(u32 offset, DVDLowCallback callback);
BOOL DVDLowWaitCoverClose(DVDLowCallback callback);
BOOL DVDLowReadDiskID(DVDDiskID* diskID, DVDLowCallback callback);
BOOL DVDLowStopMotor(DVDLowCallback callback);
BOOL DVDLowRequestError(DVDLowCallback callback);
BOOL DVDLowInquiry(DVDDriveInfo* info, DVDLowCallback callback);
BOOL DVDLowAudioStream(u32 subcmd, u32 length, u32 offset, DVDLowCallback callback);
BOOL DVDLowRequestAudioStatus(u32 subcmd, DVDLowCallback callback);
BOOL DVDLowAudioBufferConfig(BOOL enable, u32 size, DVDLowCallback callback);
void DVDLowReset(void);
BOOL DVDLowBreak(void);

void __DVDClearWaitingQueue(void);
BOOL __DVDPushWaitingQueue(s32 prio, DVDCommandBlock* block);
DVDCommandBlock* __DVDPopWaitingQueue(void);
BOOL __DVDCheckWaitingQueue(void);
BOOL __DVDDequeueWaitingQueue(DVDCommandBlock* block);

void __DVDStoreErrorCode(u32 error);

void __fstLoad(void);

#endif
