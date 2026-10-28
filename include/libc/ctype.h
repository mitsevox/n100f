#ifndef CTYPE_H
#define CTYPE_H

// MSL C library declarations, limited to what reconstructed units use (docs/libraries.md).
#include <eof.h>

#ifdef __cplusplus
extern "C" {
#endif

#define __control_char 0x01
#define __motion_char 0x02
#define __space_char 0x04
#define __punctuation 0x08
#define __digit 0x10
#define __hex_digit 0x20
#define __lower_case 0x40
#define __upper_case 0x80
#define __letter (__lower_case | __upper_case)
#define __whitespace (__motion_char | __space_char)

extern unsigned char __ctype_map[];
extern unsigned char __lower_map[];
extern unsigned char __upper_map[];

int tolower(int c);

inline int isalpha(int c) { return (int)(__ctype_map[(unsigned char)c] & __letter); }
inline int isdigit(int c) { return (int)(__ctype_map[(unsigned char)c] & __digit); }
inline int isspace(int c) { return (int)(__ctype_map[(unsigned char)c] & __whitespace); }
inline int isupper(int c) { return (int)(__ctype_map[(unsigned char)c] & __upper_case); }
inline int isxdigit(int c) { return (int)(__ctype_map[(unsigned char)c] & __hex_digit); }
inline int toupper(int c) { return (c == -1 ? -1 : (int)__upper_map[(unsigned char)c]); }

#ifdef __cplusplus
}
#endif

#endif
