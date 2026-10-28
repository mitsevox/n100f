#ifndef BINK_DCT_H
#define BINK_DCT_H

#include "types.h"

void FastIDCT8x8(unsigned char* out, int stride, short* in, int qidx);
void FastIDCT8x8d(unsigned char* out, int stride, short* in, int qidx);
void FastmIDCT8x8WithMotion(unsigned char* out, int stride, short* in, int qidx, unsigned char* m);

#endif
