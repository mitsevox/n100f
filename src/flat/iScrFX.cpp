#include "iScrFX.h"
#include "iColor.h"

#include <rwcore.h>

// Frames still to be covered in black at the end of the camera's scene.
static U32 sBlackoutFrames;

// The game blacks out the next frames around the title screen and movies so a half-built scene
// never shows.
void iScrFxSetBlackoutFrames(U32 count)
{
    sBlackoutFrames = count;
}

void iScrFxCameraEndScene(RwCamera* pCamera)
{
    if (sBlackoutFrames != 0)
    {
        sBlackoutFrames--;
        iColor_tag black = { 0, 0, 0, 255 };
        iScrFxDrawFade(NULL, &black);
    }
}

void iScrFxBegin()
{
    RwRenderStateSet(rwRENDERSTATETEXTUREFILTER, (void*)rwFILTERNEAREST);
    RwRenderStateSet(rwRENDERSTATEFOGENABLE, (void*)FALSE);
    RwRenderStateSet(rwRENDERSTATEZTESTENABLE, (void*)FALSE);
    RwRenderStateSet(rwRENDERSTATEZWRITEENABLE, (void*)FALSE);
    RwRenderStateSet(rwRENDERSTATETEXTURERASTER, NULL);
    RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, (void*)TRUE);
    RwRenderStateSet(rwRENDERSTATESRCBLEND, (void*)rwBLENDSRCALPHA);
    RwRenderStateSet(rwRENDERSTATEDESTBLEND, (void*)rwBLENDINVSRCALPHA);
}

void iScrFxEnd()
{
    RwRenderStateSet(rwRENDERSTATEFOGENABLE, (void*)FALSE);
    RwRenderStateSet(rwRENDERSTATEZTESTENABLE, (void*)TRUE);
    RwRenderStateSet(rwRENDERSTATEZWRITEENABLE, (void*)TRUE);
    RwRenderStateSet(rwRENDERSTATETEXTURERASTER, NULL);
    RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, (void*)FALSE);
    RwRenderStateSet(rwRENDERSTATESRCBLEND, (void*)rwBLENDONE);
    RwRenderStateSet(rwRENDERSTATEDESTBLEND, (void*)rwBLENDZERO);
}

// Covers the whole 640x480 screen in one colour.
void iScrFxDrawFade(RwCamera* pCamera, iColor_tag* col)
{
    U16 indices[4] = { 0, 1, 2, 3 };
    RwIm2DVertex v[4];
    F32 nearZ = RwIm2DGetNearScreenZ();

    RwIm2DVertexSetScreenX(&v[0], 0.0f);
    RwIm2DVertexSetScreenY(&v[0], 0.0f);
    RwIm2DVertexSetScreenZ(&v[0], nearZ);
    RwIm2DVertexSetScreenX(&v[1], 640.0f);
    RwIm2DVertexSetScreenY(&v[1], 0.0f);
    RwIm2DVertexSetScreenZ(&v[1], nearZ);
    RwIm2DVertexSetScreenX(&v[2], 0.0f);
    RwIm2DVertexSetScreenY(&v[2], 480.0f);
    RwIm2DVertexSetScreenZ(&v[2], nearZ);
    RwIm2DVertexSetScreenX(&v[3], 640.0f);
    RwIm2DVertexSetScreenY(&v[3], 480.0f);
    RwIm2DVertexSetScreenZ(&v[3], nearZ);

    RwIm2DVertexSetIntRGBA(&v[0], col->r, col->g, col->b, col->a);
    RwIm2DVertexSetIntRGBA(&v[1], col->r, col->g, col->b, col->a);
    RwIm2DVertexSetIntRGBA(&v[2], col->r, col->g, col->b, col->a);
    RwIm2DVertexSetIntRGBA(&v[3], col->r, col->g, col->b, col->a);

    RwIm2DRenderIndexedPrimitive(rwPRIMTYPETRISTRIP, v, 4, indices, 4);
}

// Draws black bars y_offset pixels tall across the top and bottom of the screen. Only the last
// vertex receives the near-plane depth; the target writes it four times.
void iScrFxDrawLetterbox(RwCamera* pCamera, S32 y_offset)
{
    U16 indices[4] = { 0, 1, 2, 3 };
    RwIm2DVertex v[4];
    F32 nearZ = RwIm2DGetNearScreenZ();

    RwIm2DVertexSetScreenX(&v[0], 0.0f);
    RwIm2DVertexSetScreenY(&v[0], 0.0f);
    RwIm2DVertexSetScreenZ(&v[3], nearZ);
    RwIm2DVertexSetScreenX(&v[1], 640.0f);
    RwIm2DVertexSetScreenY(&v[1], 0.0f);
    RwIm2DVertexSetScreenZ(&v[3], nearZ);
    RwIm2DVertexSetScreenX(&v[2], 0.0f);
    RwIm2DVertexSetScreenY(&v[2], y_offset);
    RwIm2DVertexSetScreenZ(&v[3], nearZ);
    RwIm2DVertexSetScreenX(&v[3], 640.0f);
    RwIm2DVertexSetScreenY(&v[3], y_offset);
    RwIm2DVertexSetScreenZ(&v[3], nearZ);

    RwIm2DVertexSetIntRGBA(&v[0], 0, 0, 0, 255);
    RwIm2DVertexSetIntRGBA(&v[1], 0, 0, 0, 255);
    RwIm2DVertexSetIntRGBA(&v[2], 0, 0, 0, 255);
    RwIm2DVertexSetIntRGBA(&v[3], 0, 0, 0, 255);

    RwIm2DRenderIndexedPrimitive(rwPRIMTYPETRISTRIP, v, 4, indices, 4);

    RwIm2DVertexSetScreenX(&v[0], 0.0f);
    RwIm2DVertexSetScreenY(&v[0], 480.0f - y_offset);
    RwIm2DVertexSetScreenX(&v[1], 640.0f);
    RwIm2DVertexSetScreenY(&v[1], 480.0f - y_offset);
    RwIm2DVertexSetScreenX(&v[2], 0.0f);
    RwIm2DVertexSetScreenY(&v[2], 480.0f);
    RwIm2DVertexSetScreenX(&v[3], 640.0f);
    RwIm2DVertexSetScreenY(&v[3], 480.0f);

    RwIm2DRenderIndexedPrimitive(rwPRIMTYPETRISTRIP, v, 4, indices, 4);
}
