#ifndef XSTRING_H
#define XSTRING_H

#include "types.h"

char* xStrupr(char* string);
S32 xStricmp(const char* string1, const char* string2);
U32 xStrHash(char* str);
char* xStrTokBuffer(const char* string, const char* control, void* buffer);

#endif
