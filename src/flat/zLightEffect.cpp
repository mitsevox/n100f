#include "zLightEffect.h"

#include "iMath.h"

static inline F32 leGetRandom()
{
    return iurand();
}

void zLightEffectInitStrobe(_zLight*)
{
}

void zLightEffectInitDim(_zLight*)
{
}

void zLightEffectInitHalfDim(_zLight*)
{
}

void zLightEffectInitRandomCol(_zLight*)
{
}

void zLightEffectInitFlicker(_zLight* zlight)
{
    *zlight->reg = 0.0f;
    zLightOn(zlight, 1);
}

// Counts the light's timer down; when it runs out, picks the next interval and dims each colour
// channel of the asset's colour by a random 0.1 to 0.3.
static void EffectFlicker(_zLight* zlight, F32 seconds, F32 min, F32 rnd)
{
    iLight* l = &zlight->light;
    F32* reg = zlight->reg;

    *reg -= seconds;

    if (*reg <= 0.0f)
    {
        *reg = rnd * leGetRandom();
        *reg += min;

        l->color.r = zlight->tasset->lightColor[0] - (leGetRandom() * 0.2f + 0.1f);
        l->color.g = zlight->tasset->lightColor[1] - (leGetRandom() * 0.2f + 0.1f);
        l->color.b = zlight->tasset->lightColor[2] - (leGetRandom() * 0.2f + 0.1f);

        if (l->color.r > 1.0f)
        {
            l->color.r = 1.0f;
        }
        else if (l->color.r < 0.0f)
        {
            l->color.r = 0.0f;
        }

        if (l->color.g > 1.0f)
        {
            l->color.g = 1.0f;
        }
        else if (l->color.g < 0.0f)
        {
            l->color.g = 0.0f;
        }

        if (l->color.b > 1.0f)
        {
            l->color.b = 1.0f;
        }
        else if (l->color.b < 0.0f)
        {
            l->color.b = 0.0f;
        }

        iLightSetColor(l, &l->color);
    }
}

void zLightEffectFlicker(_zLight* zlight, F32 seconds)
{
    EffectFlicker(zlight, seconds, 0.05f, 0.2f);
}

void zLightEffectFlickerSlow(_zLight* zlight, F32 seconds)
{
    EffectFlicker(zlight, seconds, 0.2f, 0.3f);
}

void zLightEffectFlickerErratic(_zLight* zlight, F32 seconds)
{
    EffectFlicker(zlight, seconds, 0.0f, 0.1f);
}

void zLightEffectStrobeSlow(_zLight*, F32)
{
}

void zLightEffectStrobe(_zLight*, F32)
{
}

void zLightEffectStrobeFast(_zLight*, F32)
{
}

void zLightEffectDimSlow(_zLight*, F32)
{
}

void zLightEffectDim(_zLight*, F32)
{
}

void zLightEffectDimFast(_zLight*, F32)
{
}

void zLightEffectHalfDimSlow(_zLight*, F32)
{
}

void zLightEffectHalfDim(_zLight*, F32)
{
}

void zLightEffectHalfDimFast(_zLight*, F32)
{
}

void zLightEffectRandomColSlow(_zLight*, F32)
{
}

void zLightEffectRandomCol(_zLight*, F32)
{
}

void zLightEffectRandomColFast(_zLight*, F32)
{
}

void zLightEffectInitCauldron(_zLight* zlight)
{
    *zlight->reg = 0.0f;
    zLightOn(zlight, 1);
}

// Moves a colour channel (f1) toward its target (f2) by at most f3.
static inline F32 leBlendToCol(F32 f1, F32 f2, F32 f3)
{
    if (f1 > f2)
    {
        f1 -= f3;
        if (f1 < f2)
        {
            f1 = f2;
        }
    }
    else if (f1 < f2)
    {
        f1 += f3;
        if (f1 > f2)
        {
            f1 = f2;
        }
    }

    return f1;
}

// Every quarter second picks a new random target colour (reg[1..3]) and blends the light toward it.
void zLightEffectCauldron(_zLight* zlight, F32 seconds)
{
    iLight* l = &zlight->light;
    F32* reg = zlight->reg;

    *reg -= seconds;

    if (*reg <= 0.0f)
    {
        reg[0] = 0.25f;
        reg[1] = 0.5f * leGetRandom();
        reg[2] = 0.4f * leGetRandom() + 0.6f;
        reg[3] = 0.5f * leGetRandom();
    }

    F32 amount = 3.0f * seconds;
    l->color.r = leBlendToCol(l->color.r, reg[1], amount);
    l->color.g = leBlendToCol(l->color.g, reg[2], amount);
    l->color.b = leBlendToCol(l->color.b, reg[3], amount);

    iLightSetColor(l, &l->color);
}
