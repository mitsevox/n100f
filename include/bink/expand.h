#ifndef BINK_EXPAND_H
#define BINK_EXPAND_H

#include "types.h"

void ExpandBundleSizes(void** bundles, U32 width);
void ExpandBink(U8* dest, U8* prev, U8* adest, U8* aprev, U8* mask, U32 width, U32 height,
                U32 pitch, U32 pheight, void* data, U32 key, void** bundles, U32 flags, U32 hflags);

#endif
