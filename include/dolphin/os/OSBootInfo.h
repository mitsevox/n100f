#ifndef _DOLPHIN_OSBOOTINFO_H_
#define _DOLPHIN_OSBOOTINFO_H_

#include <dolphin/dvd.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OSBootInfo {
    DVDDiskID DVDDiskID;
    u32 magic;
    u32 version;
    u32 memorySize;
    u32 consoleType;
    void* arenaLo;
    void* arenaHi;
    void* FSTLocation;
    u32 FSTMaxLength;
} OSBootInfo;

#ifdef __cplusplus
}
#endif

#endif
