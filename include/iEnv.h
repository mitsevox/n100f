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
    RpLight* light[2];
    RwFrame* light_frame[2];
    S32 memlvl;
};

void iEnvFree(iEnv* env);
void iEnvEndRenderFX(iEnv* env);

#endif
