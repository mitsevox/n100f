#include <rwdlgcn.h>
#include <string.h>

// The GameCube render states: the driver keeps the last value of each state it maps to GX (depth
// test and write, cull mode, fog, blending, alpha test) so it only calls GX on a change. The
// texture states live in the filter and addressing word of a placeholder texture whose raster is
// the current texture raster.

// The GX fog type for each RenderWare fog type, and the GX blend factor for each blend mode.
GXFogType lbl_801DE390[4] = {
    GX_FOG_NONE, GX_FOG_LIN, GX_FOG_EXP, GX_FOG_EXP2
};
GXBlendFactor lbl_801DE3A0[12] = {
    GX_BL_ZERO, GX_BL_ZERO, GX_BL_ONE, GX_BL_SRCCLR,
    GX_BL_INVSRCCLR, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_BL_DSTALPHA,
    GX_BL_INVDSTALPHA, GX_BL_SRCCLR, GX_BL_INVSRCCLR, GX_BL_ZERO
};

RwGcnRenderStateCache lbl_8025E348;

// The 4x4 white raster, and the placeholder texture that holds the texture states.
RwTexture* lbl_8025F87C;
RwRaster* lbl_8025F878;

// The alpha test modes: none, alpha above the reference, and alpha above the reference with the
// depth test after texturing. Written for this project.
#define DLRENDST_ALPHAALWAYS 1
#define DLRENDST_ALPHAGREATER 2
#define DLRENDST_ALPHATEXTURE 3

void _rwDlTextureSet(RwTexture* texture, U32 texMap);

void _rwDlRenderStateOpen(void)
{
    GXColor white = {0xFF, 0xFF, 0xFF, 0xFF};
    U8* pixels;

    lbl_8025E348.fogEnable = FALSE;
    lbl_8025E348.fogType = rwFOGTYPELINEAR;
    lbl_8025E348.fogColor = 0xFFFFFFFF;
    lbl_8025E348.gxFogColor.r = 0xFF;
    lbl_8025E348.gxFogColor.g = 0xFF;
    lbl_8025E348.gxFogColor.b = 0xFF;
    lbl_8025E348.gxFogColor.a = 0xFF;
    lbl_8025E348.fogEnd = -1.0f;
    lbl_8025E348.zWriteEnable = TRUE;
    lbl_8025E348.zTestEnable = TRUE;
    lbl_8025E348.zFunc = GX_LEQUAL;
    GXSetZMode(GX_TRUE, GX_LEQUAL, GX_TRUE);
    GXSetZCompLoc(GX_TRUE);

    lbl_8025E348.srcBlend = rwBLENDSRCALPHA;
    lbl_8025E348.destBlend = rwBLENDINVSRCALPHA;
    lbl_8025E348.gxSrcBlend = GX_BL_SRCALPHA;
    lbl_8025E348.gxDestBlend = GX_BL_INVSRCALPHA;
    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_CLEAR);

    lbl_8025E348.cullMode = rwCULLMODECULLBACK;
    GXSetCullMode(GX_CULL_FRONT);

    lbl_8025E348.alphaRef0 = 0;
    lbl_8025E348.alphaRef1 = 0;
    lbl_8025E348.alphaTest = DLRENDST_ALPHAALWAYS;
    GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_AND, GX_ALWAYS, 0);
    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_CLEAR);

    GXSetChanCtrl(GX_ALPHA0, GX_FALSE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT_NULL, GX_DF_NONE,
                  GX_AF_NONE);
    GXSetChanCtrl(GX_ALPHA1, GX_FALSE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT_NULL, GX_DF_NONE,
                  GX_AF_NONE);
    GXSetChanCtrl(GX_COLOR0, GX_FALSE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT_NULL, GX_DF_NONE,
                  GX_AF_NONE);
    GXSetChanCtrl(GX_COLOR1, GX_FALSE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT_NULL, GX_DF_NONE,
                  GX_AF_NONE);
    GXSetChanMatColor(GX_ALPHA0, white);
    GXSetChanMatColor(GX_ALPHA1, white);
    GXSetChanMatColor(GX_COLOR0, white);
    GXSetChanMatColor(GX_COLOR1, white);
    GXSetColorUpdate(GX_TRUE);
    GXSetAlphaUpdate(GX_TRUE);
    GXSetCopyClear(white, 0xFFFFFF);
    GXSetCurrentMtx(GX_PNMTX0);

    lbl_8025F878 = RwRasterCreate(4, 4, 16, rwRASTERTYPETEXTURE | rwRASTERFORMAT565);
    pixels = RwRasterLock(lbl_8025F878, 0, rwRASTERLOCKWRITE | rwRASTERLOCKRAW);
    memset(pixels, 0xFF, 4 * 4 * 2);
    RwRasterUnlock(lbl_8025F878);

    lbl_8025F87C = RwTextureCreate(NULL);
    lbl_8025F87C->filterAddressing =
        (lbl_8025F87C->filterAddressing & ~rwTEXTUREFILTERMODEMASK) | rwFILTERLINEAR;
    lbl_8025F87C->filterAddressing = (lbl_8025F87C->filterAddressing & ~rwTEXTUREADDRESSINGMASK) |
                                     (rwTEXTUREADDRESSWRAP << 8) | (rwTEXTUREADDRESSWRAP << 12);
}

void _rwDlRenderStateClose(void)
{
    RwTextureDestroy(lbl_8025F87C);
    lbl_8025F87C = NULL;
    RwRasterDestroy(lbl_8025F878);
    lbl_8025F878 = NULL;
}

S32 _rwDlGetRenderState(RwRenderState state, void* param)
{
    switch (state)
    {
    case rwRENDERSTATEFOGENABLE:
        *(S32*)param = lbl_8025E348.fogEnable;
        return TRUE;
    case rwRENDERSTATEFOGTYPE:
        *(S32*)param = lbl_8025E348.fogType;
        return TRUE;
    case rwRENDERSTATEFOGCOLOR:
        *(U32*)param = lbl_8025E348.fogColor;
        return TRUE;
    case rwRENDERSTATEFOGDENSITY:
        return FALSE;
    case 18:
        return FALSE;
    case rwRENDERSTATETEXTUREADDRESS:
    {
        S32 addressU = (lbl_8025F87C->filterAddressing & rwTEXTUREADDRESSINGUMASK) >> 8;
        S32 addressV = (lbl_8025F87C->filterAddressing & rwTEXTUREADDRESSINGVMASK) >> 12;

        if (addressU == addressV)
        {
            *(S32*)param = addressU;
            return TRUE;
        }

        return FALSE;
    }
    case rwRENDERSTATETEXTUREADDRESSU:
        *(S32*)param = (lbl_8025F87C->filterAddressing & rwTEXTUREADDRESSINGUMASK) >> 8;
        return TRUE;
    case rwRENDERSTATETEXTUREADDRESSV:
        *(S32*)param = (lbl_8025F87C->filterAddressing & rwTEXTUREADDRESSINGVMASK) >> 12;
        return TRUE;
    case rwRENDERSTATETEXTUREFILTER:
        *(S32*)param = lbl_8025F87C->filterAddressing & rwTEXTUREFILTERMODEMASK;
        return TRUE;
    case rwRENDERSTATETEXTURERASTER:
        *(RwRaster**)param = lbl_8025F87C->raster;
        return TRUE;
    case rwRENDERSTATEZWRITEENABLE:
        *(S32*)param = lbl_8025E348.zWriteEnable;
        return TRUE;
    case rwRENDERSTATEZTESTENABLE:
        *(S32*)param = lbl_8025E348.zTestEnable;
        return TRUE;
    case rwRENDERSTATESRCBLEND:
        *(S32*)param = lbl_8025E348.srcBlend;
        return TRUE;
    case rwRENDERSTATEDESTBLEND:
        *(S32*)param = lbl_8025E348.destBlend;
        return TRUE;
    case rwRENDERSTATESHADEMODE:
        *(S32*)param = rwSHADEMODEGOURAUD;
        return TRUE;
    case rwRENDERSTATEBORDERCOLOR:
        return FALSE;
    case rwRENDERSTATETEXTUREPERSPECTIVE:
        *(S32*)param = TRUE;
        return TRUE;
    case 19:
        return FALSE;
    case rwRENDERSTATECULLMODE:
        *(S32*)param = lbl_8025E348.cullMode;
        return TRUE;
    default:
        return FALSE;
    }
}

S32 _rwDlRenderStateFogEnable(S32 enable)
{
    RwCamera* camera = RwCameraGetCurrentCamera();

    if (enable)
    {
        if (!lbl_8025E348.fogEnable)
        {
            F32 fogEnd;

            if (0.0f > lbl_8025E348.fogEnd)
            {
                fogEnd = camera->farPlane;
            }
            else
            {
                fogEnd = lbl_8025E348.fogEnd;
            }

            GXSetFog(lbl_801DE390[lbl_8025E348.fogType], camera->fogPlane, fogEnd,
                     camera->nearPlane, camera->farPlane, lbl_8025E348.gxFogColor);
            lbl_8025E348.fogEnable = TRUE;
        }
    }
    else if (lbl_8025E348.fogEnable)
    {
        GXSetFog(GX_FOG_NONE, camera->fogPlane, camera->farPlane, camera->nearPlane,
                 camera->farPlane, lbl_8025E348.gxFogColor);
        lbl_8025E348.fogEnable = FALSE;
    }

    return TRUE;
}

// Loads the placeholder texture and sets the alpha test for its raster: a raster with alpha
// tests alpha after texturing.
void _rwDlTextureRasterFlush(void)
{
    S32 mode = DLRENDST_ALPHAALWAYS;

    if (lbl_8025F87C->raster)
    {
        _rwDlTextureSet(lbl_8025F87C, GX_TEXMAP0);
        mode = (RWGCRASTEREXT(lbl_8025F87C->raster)->flags & RWGCRASTER_ALPHA) ?
                   DLRENDST_ALPHATEXTURE :
                   DLRENDST_ALPHAALWAYS;
    }

    RwGameCubeSetAlphaCompare(mode);
}

// Update the cached alpha mode and configure the supported GX comparisons.
void RwGameCubeSetAlphaCompare(S32 mode)
{
    if (lbl_8025E348.alphaTest != mode)
    {
        switch (mode)
        {
        case 4:
            break;
        case DLRENDST_ALPHAALWAYS:
            GXSetAlphaCompare(GX_ALWAYS, lbl_8025E348.alphaRef0, GX_AOP_AND, GX_ALWAYS,
                              lbl_8025E348.alphaRef1);
            GXSetZCompLoc(GX_TRUE);
            break;
        case DLRENDST_ALPHAGREATER:
            GXSetAlphaCompare(GX_GREATER, lbl_8025E348.alphaRef0, GX_AOP_AND, GX_GREATER,
                              lbl_8025E348.alphaRef1);
            GXSetZCompLoc(GX_TRUE);
            break;
        case DLRENDST_ALPHATEXTURE:
            GXSetAlphaCompare(GX_GREATER, lbl_8025E348.alphaRef0, GX_AOP_AND, GX_GREATER,
                              lbl_8025E348.alphaRef1);
            GXSetZCompLoc(GX_FALSE);
            break;
        }

        lbl_8025E348.alphaTest = mode;
    }
}

// Sets the fog with the current camera's planes and the cached color. Name written for this
// project.
static inline void DlRenderStateSetFog(GXFogType type, RwCamera* camera)
{
    GXSetFog(type, camera->fogPlane, camera->farPlane, camera->nearPlane, camera->farPlane,
             lbl_8025E348.gxFogColor);
}

S32 _rwDlSetRenderState(RwRenderState state, void* param)
{
    S32 result = FALSE;

    switch (state)
    {
    case rwRENDERSTATEFOGENABLE:
        result = _rwDlRenderStateFogEnable((S32)param);
        break;
    case rwRENDERSTATEFOGCOLOR:
        if ((U32)param != lbl_8025E348.fogColor)
        {
            RwCamera* camera = RwCameraGetCurrentCamera();

            lbl_8025E348.gxFogColor.a = (U32)param >> 24;
            lbl_8025E348.gxFogColor.r = ((U32)param >> 16) & 0xFF;
            lbl_8025E348.gxFogColor.g = ((U32)param >> 8) & 0xFF;
            lbl_8025E348.gxFogColor.b = (U32)param & 0xFF;
            DlRenderStateSetFog(lbl_801DE390[lbl_8025E348.fogType], camera);
            lbl_8025E348.fogColor = (S32)param;
        }

        result = TRUE;
        break;
    case rwRENDERSTATEFOGTYPE:
    {
        S32 accepted;

        if ((S32)param != lbl_8025E348.fogType)
        {
            RwCamera* camera;

            if ((S32)param != rwFOGTYPELINEAR)
            {
                accepted = FALSE;
                goto fogtypeDone;
            }

            camera = RwCameraGetCurrentCamera();
            DlRenderStateSetFog(lbl_801DE390[(S32)param], camera);
            lbl_8025E348.fogType = (S32)param;
        }

        accepted = TRUE;
fogtypeDone:
        result = accepted;
        break;
    }
    case rwRENDERSTATEFOGDENSITY:
        result = FALSE;
        break;
    case 18:
        result = FALSE;
        break;
    case rwRENDERSTATETEXTUREADDRESS:
    {
        S32 accepted;

        if ((S32)param == rwTEXTUREADDRESSBORDER)
        {
            accepted = FALSE;
        }
        else
        {
            RwTextureSetAddressing(lbl_8025F87C, param);
            accepted = TRUE;
        }
        result = accepted;
        break;
    }
    case rwRENDERSTATETEXTUREADDRESSU:
    {
        S32 accepted;

        if ((S32)param == rwTEXTUREADDRESSBORDER)
        {
            accepted = FALSE;
        }
        else
        {
            lbl_8025F87C->filterAddressing =
                (lbl_8025F87C->filterAddressing & ~rwTEXTUREADDRESSINGUMASK) |
                (((S32)param << 8) & rwTEXTUREADDRESSINGUMASK);
            accepted = TRUE;
        }
        result = accepted;
        break;
    }
    case rwRENDERSTATETEXTUREADDRESSV:
    {
        S32 accepted;

        if ((S32)param == rwTEXTUREADDRESSBORDER)
        {
            accepted = FALSE;
        }
        else
        {
            lbl_8025F87C->filterAddressing =
                (lbl_8025F87C->filterAddressing & ~rwTEXTUREADDRESSINGVMASK) |
                (((S32)param << 12) & rwTEXTUREADDRESSINGVMASK);
            accepted = TRUE;
        }
        result = accepted;
        break;
    }
    case rwRENDERSTATETEXTUREFILTER:
        RwTextureSetFilterMode(lbl_8025F87C, param);
        result = TRUE;
        break;
    case rwRENDERSTATETEXTURERASTER:
        if (param != lbl_8025F87C->raster)
        {
            _rwDlTextureSetRaster(lbl_8025F87C, param, 0);
        }

        result = TRUE;
        break;
    case rwRENDERSTATEZWRITEENABLE:
        if ((S32)param)
        {
            if (!lbl_8025E348.zWriteEnable)
            {
                GXSetZMode(GX_TRUE, lbl_8025E348.zFunc, GX_TRUE);
                lbl_8025E348.zWriteEnable = TRUE;
            }
        }
        else if (lbl_8025E348.zWriteEnable)
        {
            GXSetZMode(GX_TRUE, lbl_8025E348.zFunc, GX_FALSE);
            lbl_8025E348.zWriteEnable = FALSE;
        }

        result = TRUE;
        break;
    case rwRENDERSTATEZTESTENABLE:
        if ((S32)param)
        {
            if (!lbl_8025E348.zTestEnable)
            {
                GXSetZMode(GX_TRUE, GX_LEQUAL, lbl_8025E348.zWriteEnable);
                lbl_8025E348.zTestEnable = TRUE;
                lbl_8025E348.zFunc = GX_LEQUAL;
            }
        }
        else if (lbl_8025E348.zTestEnable)
        {
            GXSetZMode(GX_TRUE, GX_ALWAYS, lbl_8025E348.zWriteEnable);
            lbl_8025E348.zTestEnable = FALSE;
            lbl_8025E348.zFunc = GX_ALWAYS;
        }

        result = TRUE;
        break;
    case rwRENDERSTATESRCBLEND:
    {
        S32 accepted;

        if ((S32)param != lbl_8025E348.srcBlend)
        {
            switch ((S32)param)
            {
            case rwBLENDZERO:
            case rwBLENDONE:
            case rwBLENDSRCALPHA:
            case rwBLENDINVSRCALPHA:
            case rwBLENDDESTALPHA:
            case rwBLENDINVDESTALPHA:
            case rwBLENDDESTCOLOR:
            case rwBLENDINVDESTCOLOR:
            {
                GXBlendFactor factor = lbl_801DE3A0[(S32)param];

                GXSetBlendMode(GX_BM_BLEND, factor, lbl_8025E348.gxDestBlend, GX_LO_CLEAR);
                lbl_8025E348.srcBlend = (S32)param;
                lbl_8025E348.gxSrcBlend = factor;
                break;
            }
            case rwBLENDSRCALPHASAT:
            default:
                accepted = FALSE;
                goto srcblendDone;
            }
        }

        accepted = TRUE;
srcblendDone:
        result = accepted;
        break;
    }
    case rwRENDERSTATEDESTBLEND:
    {
        S32 accepted;

        if ((S32)param != lbl_8025E348.destBlend)
        {
            switch ((S32)param)
            {
            case rwBLENDZERO:
            case rwBLENDONE:
            case rwBLENDSRCCOLOR:
            case rwBLENDINVSRCCOLOR:
            case rwBLENDSRCALPHA:
            case rwBLENDINVSRCALPHA:
            case rwBLENDDESTALPHA:
            case rwBLENDINVDESTALPHA:
            {
                GXBlendFactor factor = lbl_801DE3A0[(S32)param];

                GXSetBlendMode(GX_BM_BLEND, lbl_8025E348.gxSrcBlend, factor, GX_LO_CLEAR);
                lbl_8025E348.destBlend = (S32)param;
                lbl_8025E348.gxDestBlend = factor;
                break;
            }
            case rwBLENDDESTCOLOR:
            case rwBLENDINVDESTCOLOR:
            case rwBLENDSRCALPHASAT:
            default:
                accepted = FALSE;
                goto destblendDone;
            }
        }

        accepted = TRUE;
destblendDone:
        result = accepted;
        break;
    }
    case rwRENDERSTATESHADEMODE:
        result = ((S32)param == rwSHADEMODEGOURAUD);
        break;
    case rwRENDERSTATEBORDERCOLOR:
        result = FALSE;
        break;
    case rwRENDERSTATETEXTUREPERSPECTIVE:
        result = (S32)param;
        break;
    case 19:
        result = !param;
        break;
    case rwRENDERSTATECULLMODE:
        if ((S32)param != lbl_8025E348.cullMode)
        {
            GXSetCullMode((S32)param - 1);
            lbl_8025E348.cullMode = (S32)param;
        }

        result = TRUE;
        break;
    }

    return result;
}
