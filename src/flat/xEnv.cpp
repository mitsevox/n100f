#include "xEnv.h"

_xEnv* gCurXEnv;

// A data type of 0 loads into the iEnv embedded in this object.
void xEnvLoadBsp(_xEnv* env, const void* data, U32 datasize, S32 dataType)
{
    if (dataType == 0)
    {
        env->geom = &env->ienv;
    }

    iEnvLoad(env->geom, data, datasize, dataType);
    gCurXEnv = env;
}

void xEnvSetup(_xEnv* env)
{
    iEnvDefaultLighting(env->geom);
    gCurXEnv = env;
}

void xEnvFree(_xEnv* env)
{
    if (env->geom != NULL)
    {
        iEnvFree(env->geom);
        env->geom = NULL;
    }
}

void xEnvRender(_xEnv* env)
{
    if (env->geom != NULL)
    {
        iEnvRender(env->geom);
    }
}
