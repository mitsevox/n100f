#ifndef ISYSTEM_H
#define ISYSTEM_H

#include "types.h"

S32 iGetMonth();
S32 iGetDay();
U32 iGetCurrFormattedTime(char* str);
U32 iGetCurrFormattedDate(char* str);
void iVSync();
void iFuncProfileDump();
void iFuncProfileParse(char* elfPath, S32 profile);
void iSystemExit();

#endif
