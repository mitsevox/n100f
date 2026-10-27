#ifndef _DOLPHIN_MTX_H_
#define _DOLPHIN_MTX_H_

#include <dolphin/types.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef f32 Mtx[3][4];
typedef f32 Mtx44[4][4];

typedef struct Vec {
    f32 x;
    f32 y;
    f32 z;
} Vec;

void PSMTXConcat(const Mtx a, const Mtx b, Mtx ab);
void C_MTXOrtho(Mtx44 m, f32 t, f32 b, f32 l, f32 r, f32 n, f32 f);

#ifdef __cplusplus
}
#endif

#endif
