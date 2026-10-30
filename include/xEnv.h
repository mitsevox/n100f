#ifndef XENV_H
#define XENV_H

#include "types.h"
#include "iEnv.h"

// The GC object is 0x28 bytes (zSceneInit allocates that much); the PS2 copy is 0x40
// because its iEnv is 16-byte aligned. geom points at ienv for a world loaded with type 0.
struct _xEnv
{
    iEnv* geom;
    iEnv ienv;
};

extern _xEnv* gCurXEnv;

void xEnvLoadBsp(_xEnv* env, const void* data, U32 datasize, S32 dataType);
void xEnvSetup(_xEnv* env);
void xEnvRender(_xEnv* env);
void xEnvFree(_xEnv* env);

#endif
