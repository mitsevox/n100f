#include "xScrFx.h"
#include "zMenu.h"
#include "zGame.h"
#include "zGlobals.h"
#include "iScrFX.h"
#include "iDraw.h"
#include "xModel.h"
#include "xMath3.h"

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

// zFMV.cpp: set once the end credits have played in s005.
extern "C" U32 lbl_8025EEC8;

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

// Draws the fade and the letterbox bars over the frame. After the end credits in s005 the screen
// stays black. A fancy fade draws black around the player's model, scaled up from or down to
// nothing in front of the camera, before the whole screen goes black.
void xScrFxUpdate(RwCamera* cam, F32 seconds)
{
    if (lbl_8025EEC8)
    {
        static const iColor_tag black = { 0, 0, 0, 255 };
        iScrFxBegin();
        iScrFxDrawFade(cam, (iColor_tag*)&black);
        iScrFxEnd();
        return;
    }

    iScrFxBegin();
    if (!zGameIsPaused())
    {
        if (mFade.active)
        {
            F32 t;
            if (mFade.hold == 2)
            {
                t = 1.0f;
            }
            else
            {
                if (mFade.time_passed == 0.0f)
                {
                    t = 0.0f;
                }
                else
                {
                    t = mFade.time_passed / mFade.time_total;
                }
                mFade.time_passed += seconds;
                if (t >= 1.0f)
                {
                    t = 1.0f;
                    if (mFade.hold == 1)
                    {
                        mFade.hold = 2;
                    }
                    else
                    {
                        mFade.active = 0;
                    }
                    if (mFade.cb)
                    {
                        mFade.cb();
                    }
                }
            }

            if (mFade.fancy)
            {
                S32 entering_scene = 0;
                if (mFade.src.a > mFade.dest.a)
                {
                    entering_scene = 1;
                }

                RwCameraClear(cam, NULL, rwCAMERACLEARZ);
                RwRenderStateSet(rwRENDERSTATEZTESTENABLE, (void*)FALSE);
                RwRenderStateSet(rwRENDERSTATEZWRITEENABLE, (void*)TRUE);
                RwRenderStateSet(rwRENDERSTATESRCBLEND, (void*)rwBLENDZERO);
                RwRenderStateSet(rwRENDERSTATEDESTBLEND, (void*)rwBLENDZERO);

                if (!entering_scene && t >= 0.95f)
                {
                    DrawScreenSizeRectangle();
                }
                else
                {
                    _xMat4x3 old;
                    _xMat4x3 old_head;
                    F32 dog_y;
                    F32 dog_z;
                    if (entering_scene)
                    {
                        F32 sc = 300.0f * t;
                        dog_y = -5.5f;
                        globals.player.ent.model->Scale.x = sc;
                        globals.player.ent.model->Scale.y = sc;
                        globals.player.ent.model->Scale.z = 1.0f;
                        globals.player.ent.model->Next->Scale.x = sc;
                        globals.player.ent.model->Next->Scale.y = sc;
                        globals.player.ent.model->Next->Scale.z = 1.0f;
                    }
                    else
                    {
                        dog_y = -20.5f;
                        F32 sc = (1.0f - t) * 300.0f;
                        globals.player.ent.model->Scale.x = sc;
                        globals.player.ent.model->Scale.y = sc;
                        globals.player.ent.model->Scale.z = 1.0f;
                        globals.player.ent.model->Next->Scale.x = sc;
                        globals.player.ent.model->Next->Scale.y = sc;
                        globals.player.ent.model->Next->Scale.z = 1.0f;
                    }
                    dog_z = 25.0f;

                    xMat4x3Copy(&old, (xMat4x3*)globals.player.ent.model->Mat);
                    xMat4x3Identity((xMat4x3*)globals.player.ent.model->Mat);
                    globals.player.ent.model->Mat->pos.y = dog_y;
                    globals.player.ent.model->Mat->pos.z = dog_z;
                    xMat4x3Copy(&old_head, (xMat4x3*)globals.player.ent.model->Next->Mat);
                    xMat4x3Identity((xMat4x3*)globals.player.ent.model->Next->Mat);
                    globals.player.ent.model->Next->Mat->pos.y = dog_y;
                    globals.player.ent.model->Next->Mat->pos.z = dog_z;

                    iDrawSetFBMSK(0xFFFFFFFF);
                    xModelRenderSingle(globals.player.ent.model);
                    xModelRenderSingle(globals.player.ent.model->Next);
                    xMat4x3Copy((xMat4x3*)globals.player.ent.model->Mat, &old);
                    xMat4x3Copy((xMat4x3*)globals.player.ent.model->Next->Mat, &old_head);
                    iDrawSetFBMSK(0);

                    globals.player.ent.model->Scale.x = 1.0f;
                    globals.player.ent.model->Scale.y = 1.0f;
                    globals.player.ent.model->Scale.z = 1.0f;
                    globals.player.ent.model->Next->Scale.x = 1.0f;
                    globals.player.ent.model->Next->Scale.y = 1.0f;
                    globals.player.ent.model->Next->Scale.z = 1.0f;

                    RwRenderStateSet(rwRENDERSTATEZTESTENABLE, (void*)TRUE);
                    RwRenderStateSet(rwRENDERSTATEZWRITEENABLE, (void*)FALSE);
                    RwRenderStateSet(rwRENDERSTATETEXTURERASTER, NULL);
                    DrawScreenSizeRectangle();
                }
            }
            else
            {
                iColor_tag col;
                col.r = (mFade.dest.r - mFade.src.r) * t + mFade.src.r;
                col.g = (mFade.dest.g - mFade.src.g) * t + mFade.src.g;
                col.b = (mFade.dest.b - mFade.src.b) * t + mFade.src.b;
                col.a = (mFade.dest.a - mFade.src.a) * t + mFade.src.a;
                iScrFxDrawFade(cam, &col);
            }
        }

        if (mLetterboxO < mLetterboxTO)
        {
            mLetterboxO += 100.0f * seconds;
            if (mLetterboxO > mLetterboxTO)
            {
                mLetterboxO = mLetterboxTO;
            }
        }
        else if (mLetterboxO > mLetterboxTO)
        {
            mLetterboxO -= 100.0f * seconds;
            if (mLetterboxO < mLetterboxTO)
            {
                mLetterboxO = mLetterboxTO;
            }
        }
    }

    if (mLetterboxO > 0.0f)
    {
        iScrFxDrawLetterbox(cam, mLetterboxO);
    }
    iScrFxEnd();
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
