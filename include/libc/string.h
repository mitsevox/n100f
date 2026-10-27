#ifndef STRING_H
#define STRING_H

// MSL C library declarations, limited to what reconstructed units use (docs/libraries.md).

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

void* memcpy(void* dst, const void* src, size_t n);
void* memmove(void* dst, const void* src, size_t n);
int memcmp(const void* lhs, const void* rhs, size_t n);
void* memchr(const void* ptr, int ch, size_t n);
void* __memrchr(const void* ptr, int ch, size_t n);
void* memset(void* dst, int val, size_t n);
size_t strlen(const char* s);
char* strcpy(char* dst, const char* src);
char* strncpy(char* dst, const char* src, size_t n);
char* strcat(char* dst, const char* src);
char* strncat(char* dst, const char* src, size_t n);
int strcmp(const char* str1, const char* str2);
int strncmp(const char* str1, const char* str2, size_t n);
char* strchr(const char* str, int c);
char* strtok(char* str, const char* delim);
char* strstr(const char* str, const char* pat);

#ifdef __cplusplus
}
#endif

#endif
