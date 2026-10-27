#ifndef STDIO_H
#define STDIO_H

#include <stdarg.h>

// MSL C library declarations, limited to what reconstructed units use (docs/libraries.md).
#include <stddef.h>
#include <eof.h>

#ifdef __cplusplus
extern "C" {
#endif

int sprintf(char* s, const char* format, ...);
int vprintf(const char* format, va_list arg);

#ifdef __cplusplus
}
#endif

#endif
