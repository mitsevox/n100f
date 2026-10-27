#ifndef _DOLPHIN_DVD_PRIVATE_H_
#define _DOLPHIN_DVD_PRIVATE_H_

#include <dolphin/dvd.h>

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

void __DVDClearWaitingQueue(void);
BOOL __DVDPushWaitingQueue(s32 prio, DVDCommandBlock* block);
DVDCommandBlock* __DVDPopWaitingQueue(void);
BOOL __DVDCheckWaitingQueue(void);
BOOL __DVDDequeueWaitingQueue(DVDCommandBlock* block);

void __DVDStoreErrorCode(u32 error);

void __fstLoad(void);

#endif
