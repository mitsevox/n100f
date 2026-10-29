#ifndef XENV_H
#define XENV_H

#include "types.h"
#include "iEnv.h"

// Partial: geom only. The embedded iEnv after it is left opaque. The GC object is 0x28
// bytes (zSceneInit allocates that much); the PS2 copy is 0x40 because its iEnv is
// 16-byte aligned.
struct _xEnv
{
    iEnv* geom;
    U8 opaque_4[0x28 - 0x4];
};

void xEnvRender(_xEnv* env);

#endif
