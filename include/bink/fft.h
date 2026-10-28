#ifndef BINK_FFT_H
#define BINK_FFT_H

void rdft(int n, int isgn, float* a, int* ip, float* w);
void ddct(int n, int isgn, float* a, int* ip, float* w);

#endif
