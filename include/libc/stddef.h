#ifndef STDDEF_H
#define STDDEF_H

// MSL C library declarations, limited to what reconstructed units use (docs/libraries.md).

typedef unsigned long size_t;

#ifndef __cplusplus
typedef unsigned short wchar_t;
#endif

#ifndef NULL
#define NULL 0
#endif

#endif
