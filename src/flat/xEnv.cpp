#include "xEnv.h"

void xEnvRender(_xEnv* env)
{
    if (env->geom != NULL)
    {
        iEnvRender(env->geom);
    }
}
