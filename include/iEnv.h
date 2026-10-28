#ifndef IENV_H
#define IENV_H

#include "types.h"

#include <rpworld.h>
#include <rwcore.h>

struct iEnv
{
    RpWorld* world;
    RpWorld* collision;
    RpWorld* fx;
    RpWorld* camera;
};

void iEnvFree(iEnv* env);
void iEnvRender(iEnv* env);
void iEnvEndRenderFX(iEnv* env);

#endif
