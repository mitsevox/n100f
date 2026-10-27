#ifndef _DOLPHIN_OSFONT_H_
#define _DOLPHIN_OSFONT_H_

#include <dolphin/types.h>

#ifdef __cplusplus
extern "C" {
#endif

#define OS_FONT_ENCODE_ANSI 0u
#define OS_FONT_ENCODE_SJIS 1u

u16 OSGetFontEncode(void);

#ifdef __cplusplus
}
#endif

#endif
