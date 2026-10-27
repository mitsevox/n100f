#ifndef _DOLPHIN_DB_H_
#define _DOLPHIN_DB_H_

#include <dolphin/os/OSException.h>

#ifdef __cplusplus
extern "C" {
#endif

#define OS_DBINTERFACE_ADDR 0x00000040

typedef struct DBInterface {
    u32 bPresent;
    u32 exceptionMask;
    void (*ExceptionDestination)(void);
    void* exceptionReturn;
} DBInterface;

extern BOOL DBVerbose;

void DBInit(void);
BOOL __DBIsExceptionMarked(__OSException exception);
void DBPrintf(char* str, ...);

#ifdef __cplusplus
}
#endif

#endif
