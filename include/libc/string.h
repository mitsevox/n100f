#ifndef STRING_H
#define STRING_H

// MSL C library declarations, limited to what reconstructed units use (docs/libraries.md).

typedef unsigned long size_t;

#ifdef __cplusplus
extern "C" {
#endif

void* memcpy(void* dst, const void* src, size_t n);
int memcmp(const void* a, const void* b, size_t n);
void* memset(void* dst, int val, size_t n);
size_t strlen(const char* s);
char* strncpy(char* dst, const char* src, size_t n);

#ifdef __cplusplus
}
#endif

#endif
