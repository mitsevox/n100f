#ifndef BINK_BINKACD_H
#define BINK_BINKACD_H

#include "types.h"

void* BinkAudioDecompressOpen(U32 rate, U32 chans, U32 flags);
void BinkAudioDecompress(void* ac, S16** out, U32* len, void* src, void** srcend);
void BinkAudioDecompressClose(void* ac);

#endif
