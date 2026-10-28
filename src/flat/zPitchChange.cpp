#include "zPitchChange.h"
#include "iMath.h"

#include <math.h>

S32 zPitchChange(S32 angle, F32* offset, S32, F32 max)
{
    S32 old_angle;

    old_angle = angle;

    do
    {
        angle = iurand() * 360.0f;
    } while (fabsf(old_angle) >= fabsf(angle) - 10.0f && fabsf(old_angle) <= fabsf(angle) + 10.0f);

    *offset = (S32)(angle / 360.0f * max);

    return angle;
}
