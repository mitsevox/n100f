#ifndef XSTRING_H
#define XSTRING_H

#include "types.h"

char* xStrupr(char* string);
S32 xStricmp(const char* string1, const char* string2);
U32 xStrHash(char* str);
S32 xStrParseFloatList(F32* dest, const char* strbuf, S32 max);
char* xStrTokBuffer(const char* string, const char* control, void* buffer);
char* xStrTok(char* string, const char* control, char** nextoken);

#endif
