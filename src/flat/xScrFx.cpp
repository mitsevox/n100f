#include "xScrFx.h"
#include "zMenu.h"

#include <rwcore.h>
#include <string.h>

struct _xFadeData
{
    S32 active;
    S32 hold;
    iColor_tag src;
    iColor_tag dest;
    F32 time_passed;
    F32 time_total;
    void (*cb)();
    S32 fancy;
};

static F32 mLetterboxO;
static F32 mLetterboxTO;
static _xFadeData mFade;

void xScrFxInit()
{
    memset(&mFade, 0, sizeof(_xFadeData));
    mLetterboxO = 0.0f;
    mLetterboxTO = 0.0f;
}

// Covers the whole screen in opaque black at the near plane.
void DrawScreenSizeRectangle()
{
    RwVideoMode video_mode;
    static U16 indices[4] = { 0, 1, 2, 3 };
    static RwIm2DVertex v[4];

    RwEngineGetVideoModeInfo(&video_mode, RwEngineGetCurrentVideoMode());

    RwIm2DVertexSetScreenX(&v[0], 0.0f);
    RwIm2DVertexSetScreenY(&v[0], 0.0f);
    RwIm2DVertexSetScreenZ(&v[0], 0.0f);
    RwIm2DVertexSetScreenX(&v[1], video_mode.width);
    RwIm2DVertexSetScreenY(&v[1], 0.0f);
    RwIm2DVertexSetScreenZ(&v[1], 0.0f);
    RwIm2DVertexSetScreenX(&v[2], 0.0f);
    RwIm2DVertexSetScreenY(&v[2], video_mode.height);
    RwIm2DVertexSetScreenZ(&v[2], 0.0f);
    RwIm2DVertexSetScreenX(&v[3], video_mode.width);
    RwIm2DVertexSetScreenY(&v[3], video_mode.height);
    RwIm2DVertexSetScreenZ(&v[3], 0.0f);

    RwIm2DVertexSetIntRGBA(&v[0], 0, 0, 0, 255);
    RwIm2DVertexSetIntRGBA(&v[1], 0, 0, 0, 255);
    RwIm2DVertexSetIntRGBA(&v[2], 0, 0, 0, 255);
    RwIm2DVertexSetIntRGBA(&v[3], 0, 0, 0, 255);

    RwIm2DRenderIndexedPrimitive(rwPRIMTYPETRISTRIP, v, 4, indices, 4);
}

// Starts a fade from base to dest over the given time. The last argument is not used.
void xScrFxFade(iColor_tag* base, iColor_tag* dest, F32 seconds, void (*callback)(), S32 hold, S32)
{
    mFade.active = 1;
    mFade.src = *base;
    mFade.dest = *dest;
    mFade.time_passed = 0.0f;
    mFade.time_total = seconds;
    mFade.cb = callback;
    if (!hold)
    {
        mFade.hold = 0;
    }
    else
    {
        mFade.hold = 1;
    }
    mFade.fancy = 0;
}

void xScrFxLetterbox(S32 enable)
{
    if (zMenuRunning())
    {
        enable = 0;
    }

    if (enable)
    {
        mLetterboxTO = 16.0f;
    }
    else
    {
        mLetterboxTO = 0.0f;
    }
}

S32 xScrFxIsLetterbox()
{
    return mLetterboxTO > 0.0f;
}

S32 xScrFxIsFading()
{
    return mFade.active;
}
