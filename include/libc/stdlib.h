#ifndef STDLIB_H
#define STDLIB_H

// MSL C library declarations, limited to what reconstructed units use (docs/libraries.md).
#ifdef __cplusplus
extern "C" {
#endif

int atoi(const char* str);
double atof(const char* str);

#ifdef __cplusplus
}
#endif

#endif
