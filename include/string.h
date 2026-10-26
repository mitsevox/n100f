#ifndef STRING_H
#define STRING_H

// Project stub declaring only what reconstructed units call. Replace it when MSL's own
// headers are reconstructed.

typedef unsigned long size_t;

#ifdef __cplusplus
extern "C" {
#endif

void* memcpy(void* dst, const void* src, size_t n);
size_t strlen(const char* s);

#ifdef __cplusplus
}
#endif

#endif
