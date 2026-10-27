#ifndef STDLIB_H
#define STDLIB_H

// MSL C library declarations, limited to what reconstructed units use (docs/libraries.md).
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef int (*_compare_function)(const void*, const void*);

int abs(int n);
int rand(void);
int atoi(const char* str);
long strtol(const char* str, char** end, int base);
unsigned long strtoul(const char* str, char** end, int base);
double atof(const char* str);
void qsort(void* table_base, size_t num_members, size_t member_size,
           _compare_function compare_members);

#ifdef __cplusplus
}
#endif

#endif
