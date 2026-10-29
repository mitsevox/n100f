#ifndef XENV_H
#define XENV_H

#include "types.h"
#include "iEnv.h"

// Partial: geom only. The embedded iEnv at +0x10 is left opaque.
struct _xEnv
{
    iEnv* geom;
    U8 opaque_4[0x40 - 0x4];
};

void xEnvRender(_xEnv* env);
void xEnvFree(_xEnv* env);

#endif
