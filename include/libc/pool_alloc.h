#ifndef POOL_ALLOC_H
#define POOL_ALLOC_H

// MSL C library declarations, limited to what reconstructed units use (docs/libraries.md).
#ifdef __cplusplus
extern "C" {
#endif

void* __sys_alloc(unsigned long size);
void __sys_free(void* ptr);

#ifdef __cplusplus
}
#endif

#endif
