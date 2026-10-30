#include "xFont.h"
#include "xString.h"
#include "xstransvc.h"
#include "rwcore.h"

extern xFontInfo gFontList[4];

U32 iFontInitFont(RwTexture* texture, const char* characters, U32 layout, U16 u, U16 v,
                  U16 char_width, U16 char_height, U16 spacing);

U32 gDBfont;
static RwRaster* sButtonRaster;

// All three fonts are cells of the one "font" texture; the third, the small debug font, is
// kept in gDBfont.
void iFontInit()
{
    RwTexture* tex = (RwTexture*)xSTFindAsset(xStrHash("font"), NULL);

    iFontInitFont(tex, "~{}#ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789?!.,-:_\"'&()<>/%", 14, 0, 0x84,
                  18, 22, 2);
    iFontInitFont(tex,
                  "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789?!.,;:+-=/&()%"
                  "\"'_<>*[]~{}#",
                  14, 0, 0, 18, 22, 0);
    gDBfont = iFontInitFont(tex, "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ,./*+-=:;%<>[]|()_", 18,
                            0x94, 0xE8, 6, 8, 6);

    sButtonRaster = *(RwRaster**)xSTFindAsset(xStrHash("buttons_all"), NULL);
}

static RwIm2DVertex ourGlobals[120];
static RxRenderStateVector rsv;
static RwIm2DVertex* im2d_vert;
static S32 im2d_vert_ct;
static F32 rcz;
static F32 nsz;
static RwRaster* sRaster;

// Character quads are batched in ourGlobals between iFontSetCharRenderState and
// iFontRestoreRenderState, which draws whatever is left before restoring the saved states.
void iFontRestoreRenderState()
{
    if (im2d_vert_ct > 0)
    {
        RwIm2DRenderPrimitive(rwPRIMTYPETRILIST, im2d_vert, im2d_vert_ct);
        im2d_vert_ct = 0;
    }

    RwRenderStateSet(rwRENDERSTATEFOGENABLE, (void*)((rsv.Flags >> 4) & 1));
    RwRenderStateSet(rwRENDERSTATESRCBLEND, (void*)rsv.SrcBlend);
    RwRenderStateSet(rwRENDERSTATEDESTBLEND, (void*)rsv.DestBlend);
    RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, (void*)((rsv.Flags >> 3) & 1));
    RwRenderStateSet(rwRENDERSTATETEXTURERASTER, rsv.TextureRaster);
    RwRenderStateSet(rwRENDERSTATESHADEMODE, (void*)rsv.ShadeMode);
    RwRenderStateSet(rwRENDERSTATEZWRITEENABLE, (void*)((rsv.Flags >> 2) & 1));
    im2d_vert = NULL;
}

void iFontSetCharRenderState(xFontBox* box)
{
    xFontInfo* font = &gFontList[box->font];

    RwRenderStateSet(rwRENDERSTATEFOGENABLE, (void*)FALSE);
    RwRenderStateSet(rwRENDERSTATESRCBLEND, (void*)rwBLENDSRCALPHA);
    RwRenderStateSet(rwRENDERSTATEDESTBLEND, (void*)rwBLENDINVSRCALPHA);
    RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, (void*)TRUE);
    RwRenderStateSet(rwRENDERSTATETEXTURERASTER, font->ps.raster);
    RwRenderStateSet(rwRENDERSTATESHADEMODE, (void*)rwSHADEMODEFLAT);
    RwRenderStateSet(rwRENDERSTATEZWRITEENABLE, (void*)FALSE);
    sRaster = font->ps.raster;
    im2d_vert = ourGlobals;
}

void iFontSetBackdropRenderState(xFontBox*)
{
    RwRenderStateSet(rwRENDERSTATEFOGENABLE, (void*)FALSE);
    RwRenderStateSet(rwRENDERSTATESRCBLEND, (void*)rwBLENDSRCALPHA);
    RwRenderStateSet(rwRENDERSTATEDESTBLEND, (void*)rwBLENDINVSRCALPHA);
    RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, (void*)TRUE);
    RwRenderStateSet(rwRENDERSTATETEXTURERASTER, NULL);
}

// Saves the render states the font code changes and the camera depth values its quads use.
void iFontSaveRenderState()
{
    RxRenderStateVectorLoadDriverState(&rsv);
    rcz = 1.0f / RwCameraGetCurrentCamera()->nearPlane;
    nsz = RwIm2DGetNearScreenZ();
}
