#ifndef RTSLERP_H
#define RTSLERP_H

#include "rwcore.h"
#include "rtquat.h"

typedef struct RtQuatSlerpCache RtQuatSlerpCache;
struct RtQuatSlerpCache
{
    RtQuat raFrom;
    RtQuat raTo;
    F32 omega;
    S32 nearlyZeroOm;
};

#define RtQuatSlerpMacro(qpResult, qpFrom, qpTo, rT, sCache)                                       \
    do                                                                                             \
    {                                                                                              \
        if ((rT) <= ((F32)0))                                                                      \
        {                                                                                          \
            *(qpResult) = *(qpFrom);                                                               \
        }                                                                                          \
        else if (((F32)1) <= (rT))                                                                 \
        {                                                                                          \
            *(qpResult) = *(qpTo);                                                                 \
        }                                                                                          \
        else                                                                                       \
        {                                                                                          \
            F32 rSclFrom = ((F32)1) - (rT);                                                        \
            F32 rSclTo = (rT);                                                                     \
                                                                                                   \
            if (!((sCache)->nearlyZeroOm))                                                         \
            {                                                                                      \
                rSclFrom *= (sCache)->omega;                                                       \
                RwSinMinusPiToPiMacro(rSclFrom, rSclFrom);                                         \
                rSclTo *= (sCache)->omega;                                                         \
                RwSinMinusPiToPiMacro(rSclTo, rSclTo);                                             \
            }                                                                                      \
                                                                                                   \
            RwV3dScaleMacro(&(qpResult)->imag, &(sCache)->raFrom.imag, rSclFrom);                  \
            RwV3dIncrementScaledMacro(&(qpResult)->imag, &(sCache)->raTo.imag, rSclTo);            \
            (qpResult)->real =                                                                     \
                ((sCache)->raFrom.real * rSclFrom) + ((sCache)->raTo.real * rSclTo);               \
        }                                                                                          \
    } while (0)

#define RtQuatSlerp(qpResult, qpFrom, qpTo, rT, sCache)                                            \
    RtQuatSlerpMacro(qpResult, qpFrom, qpTo, rT, sCache)

#ifdef __cplusplus
extern "C" {
#endif

void RtQuatSetupSlerpCache(RtQuat* qpFrom, RtQuat* qpTo, RtQuatSlerpCache* sCache);

#ifdef __cplusplus
}
#endif

#endif
