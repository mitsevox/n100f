#ifndef MEM_FUNCS_H
#define MEM_FUNCS_H

// MSL C library declarations, limited to what reconstructed units use (docs/libraries.md).
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

void __copy_longs_aligned(void* dst, const void* src, size_t n);
void __copy_longs_rev_aligned(void* dst, const void* src, size_t n);
void __copy_longs_unaligned(void* dst, const void* src, size_t n);
void __copy_longs_rev_unaligned(void* dst, const void* src, size_t n);

#ifdef __cplusplus
}
#endif

#endif
