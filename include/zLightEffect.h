#ifndef ZLIGHTEFFECT_H
#define ZLIGHTEFFECT_H

#include "zLight.h"

void zLightEffectInitStrobe(_zLight*);
void zLightEffectInitDim(_zLight*);
void zLightEffectInitHalfDim(_zLight*);
void zLightEffectInitRandomCol(_zLight*);
void zLightEffectInitFlicker(_zLight* zlight);
void zLightEffectFlicker(_zLight* zlight, F32 seconds);
void zLightEffectFlickerSlow(_zLight* zlight, F32 seconds);
void zLightEffectFlickerErratic(_zLight* zlight, F32 seconds);
void zLightEffectStrobeSlow(_zLight*, F32);
void zLightEffectStrobe(_zLight*, F32);
void zLightEffectStrobeFast(_zLight*, F32);
void zLightEffectDimSlow(_zLight*, F32);
void zLightEffectDim(_zLight*, F32);
void zLightEffectDimFast(_zLight*, F32);
void zLightEffectHalfDimSlow(_zLight*, F32);
void zLightEffectHalfDim(_zLight*, F32);
void zLightEffectHalfDimFast(_zLight*, F32);
void zLightEffectRandomColSlow(_zLight*, F32);
void zLightEffectRandomCol(_zLight*, F32);
void zLightEffectRandomColFast(_zLight*, F32);
void zLightEffectInitCauldron(_zLight* zlight);
void zLightEffectCauldron(_zLight* zlight, F32 seconds);

#endif
