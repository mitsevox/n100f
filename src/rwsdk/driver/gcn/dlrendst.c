#include <rwdlgcn.h>
#include <string.h>

// The GameCube render states: the driver keeps the last value of each state it maps to GX (depth
// test and write, cull mode, fog, blending, alpha test) so it only calls GX on a change. The
// texture states live in the filter and addressing word of a placeholder texture whose raster is
// the current texture raster.

// The GX fog type for each RenderWare fog type, and the GX blend factor for each blend mode.
GXFogType _RwDlFogConvTable[4] = {
    GX_FOG_NONE, GX_FOG_LIN, GX_FOG_EXP, GX_FOG_EXP2
};
GXBlendFactor _RwDlBlendConvTable[12] = {
    GX_BL_ZERO, GX_BL_ZERO, GX_BL_ONE, GX_BL_SRCCLR,
    GX_BL_INVSRCCLR, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_BL_DSTALPHA,
    GX_BL_INVDSTALPHA, GX_BL_SRCCLR, GX_BL_INVSRCCLR, GX_BL_ZERO
};

RwGcnRenderStateCache _RwDlStateCache;

// The 4x4 white raster, and the placeholder texture that holds the texture states.
RwTexture* _RwDlTexture;
RwRaster* _RwDlRasterWhite;

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

    _RwDlStateCache.fogEnable = FALSE;
    _RwDlStateCache.fogType = rwFOGTYPELINEAR;
    _RwDlStateCache.fogColor = 0xFFFFFFFF;
    _RwDlStateCache.gxFogColor.r = 0xFF;
    _RwDlStateCache.gxFogColor.g = 0xFF;
    _RwDlStateCache.gxFogColor.b = 0xFF;
    _RwDlStateCache.gxFogColor.a = 0xFF;
    _RwDlStateCache.fogEnd = -1.0f;
    _RwDlStateCache.zWriteEnable = TRUE;
    _RwDlStateCache.zTestEnable = TRUE;
    _RwDlStateCache.zFunc = GX_LEQUAL;
    GXSetZMode(GX_TRUE, GX_LEQUAL, GX_TRUE);
    GXSetZCompLoc(GX_TRUE);

    _RwDlStateCache.srcBlend = rwBLENDSRCALPHA;
    _RwDlStateCache.destBlend = rwBLENDINVSRCALPHA;
    _RwDlStateCache.gxSrcBlend = GX_BL_SRCALPHA;
    _RwDlStateCache.gxDestBlend = GX_BL_INVSRCALPHA;
    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_CLEAR);

    _RwDlStateCache.cullMode = rwCULLMODECULLBACK;
    GXSetCullMode(GX_CULL_FRONT);

    _RwDlStateCache.alphaRef0 = 0;
    _RwDlStateCache.alphaRef1 = 0;
    _RwDlStateCache.alphaTest = DLRENDST_ALPHAALWAYS;
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

    _RwDlRasterWhite = RwRasterCreate(4, 4, 16, rwRASTERTYPETEXTURE | rwRASTERFORMAT565);
    pixels = RwRasterLock(_RwDlRasterWhite, 0, rwRASTERLOCKWRITE | rwRASTERLOCKRAW);
    memset(pixels, 0xFF, 4 * 4 * 2);
    RwRasterUnlock(_RwDlRasterWhite);

    _RwDlTexture = RwTextureCreate(NULL);
    _RwDlTexture->filterAddressing =
        (_RwDlTexture->filterAddressing & ~rwTEXTUREFILTERMODEMASK) | rwFILTERLINEAR;
    _RwDlTexture->filterAddressing = (_RwDlTexture->filterAddressing & ~rwTEXTUREADDRESSINGMASK) |
                                     (rwTEXTUREADDRESSWRAP << 8) | (rwTEXTUREADDRESSWRAP << 12);
}

void _rwDlRenderStateClose(void)
{
    RwTextureDestroy(_RwDlTexture);
    _RwDlTexture = NULL;
    RwRasterDestroy(_RwDlRasterWhite);
    _RwDlRasterWhite = NULL;
}

S32 _rwDlGetRenderState(RwRenderState state, void* param)
{
    switch (state)
    {
    case rwRENDERSTATEFOGENABLE:
        *(S32*)param = _RwDlStateCache.fogEnable;
        return TRUE;
    case rwRENDERSTATEFOGTYPE:
        *(S32*)param = _RwDlStateCache.fogType;
        return TRUE;
    case rwRENDERSTATEFOGCOLOR:
        *(U32*)param = _RwDlStateCache.fogColor;
        return TRUE;
    case rwRENDERSTATEFOGDENSITY:
        return FALSE;
    case 18:
        return FALSE;
    case rwRENDERSTATETEXTUREADDRESS:
    {
        S32 addressU = (_RwDlTexture->filterAddressing & rwTEXTUREADDRESSINGUMASK) >> 8;
        S32 addressV = (_RwDlTexture->filterAddressing & rwTEXTUREADDRESSINGVMASK) >> 12;

        if (addressU == addressV)
        {
            *(S32*)param = addressU;
            return TRUE;
        }

        return FALSE;
    }
    case rwRENDERSTATETEXTUREADDRESSU:
        *(S32*)param = (_RwDlTexture->filterAddressing & rwTEXTUREADDRESSINGUMASK) >> 8;
        return TRUE;
    case rwRENDERSTATETEXTUREADDRESSV:
        *(S32*)param = (_RwDlTexture->filterAddressing & rwTEXTUREADDRESSINGVMASK) >> 12;
        return TRUE;
    case rwRENDERSTATETEXTUREFILTER:
        *(S32*)param = _RwDlTexture->filterAddressing & rwTEXTUREFILTERMODEMASK;
        return TRUE;
    case rwRENDERSTATETEXTURERASTER:
        *(RwRaster**)param = _RwDlTexture->raster;
        return TRUE;
    case rwRENDERSTATEZWRITEENABLE:
        *(S32*)param = _RwDlStateCache.zWriteEnable;
        return TRUE;
    case rwRENDERSTATEZTESTENABLE:
        *(S32*)param = _RwDlStateCache.zTestEnable;
        return TRUE;
    case rwRENDERSTATESRCBLEND:
        *(S32*)param = _RwDlStateCache.srcBlend;
        return TRUE;
    case rwRENDERSTATEDESTBLEND:
        *(S32*)param = _RwDlStateCache.destBlend;
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
        *(S32*)param = _RwDlStateCache.cullMode;
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
        if (!_RwDlStateCache.fogEnable)
        {
            F32 fogEnd;

            if (0.0f > _RwDlStateCache.fogEnd)
            {
                fogEnd = camera->farPlane;
            }
            else
            {
                fogEnd = _RwDlStateCache.fogEnd;
            }

            GXSetFog(_RwDlFogConvTable[_RwDlStateCache.fogType], camera->fogPlane, fogEnd,
                     camera->nearPlane, camera->farPlane, _RwDlStateCache.gxFogColor);
            _RwDlStateCache.fogEnable = TRUE;
        }
    }
    else if (_RwDlStateCache.fogEnable)
    {
        GXSetFog(GX_FOG_NONE, camera->fogPlane, camera->farPlane, camera->nearPlane,
                 camera->farPlane, _RwDlStateCache.gxFogColor);
        _RwDlStateCache.fogEnable = FALSE;
    }

    return TRUE;
}

// Loads the placeholder texture and sets the alpha test for its raster: a raster with alpha
// tests alpha after texturing.
void _rwDlTextureRasterFlush(void)
{
    S32 mode = DLRENDST_ALPHAALWAYS;

    if (_RwDlTexture->raster)
    {
        _rwDlTextureSet(_RwDlTexture, GX_TEXMAP0);
        mode = (RWGCRASTEREXT(_RwDlTexture->raster)->flags & RWGCRASTER_ALPHA) ?
                   DLRENDST_ALPHATEXTURE :
                   DLRENDST_ALPHAALWAYS;
    }

    RwGameCubeSetAlphaCompare(mode);
}

// Update the cached alpha mode and configure the supported GX comparisons.
void RwGameCubeSetAlphaCompare(S32 mode)
{
    if (_RwDlStateCache.alphaTest != mode)
    {
        switch (mode)
        {
        case 4:
            break;
        case DLRENDST_ALPHAALWAYS:
            GXSetAlphaCompare(GX_ALWAYS, _RwDlStateCache.alphaRef0, GX_AOP_AND, GX_ALWAYS,
                              _RwDlStateCache.alphaRef1);
            GXSetZCompLoc(GX_TRUE);
            break;
        case DLRENDST_ALPHAGREATER:
            GXSetAlphaCompare(GX_GREATER, _RwDlStateCache.alphaRef0, GX_AOP_AND, GX_GREATER,
                              _RwDlStateCache.alphaRef1);
            GXSetZCompLoc(GX_TRUE);
            break;
        case DLRENDST_ALPHATEXTURE:
            GXSetAlphaCompare(GX_GREATER, _RwDlStateCache.alphaRef0, GX_AOP_AND, GX_GREATER,
                              _RwDlStateCache.alphaRef1);
            GXSetZCompLoc(GX_FALSE);
            break;
        }

        _RwDlStateCache.alphaTest = mode;
    }
}

// Sets the fog with the current camera's planes and the cached color. Name written for this
// project.
static inline void DlRenderStateSetFog(GXFogType type, RwCamera* camera)
{
    GXSetFog(type, camera->fogPlane, camera->farPlane, camera->nearPlane, camera->farPlane,
             _RwDlStateCache.gxFogColor);
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
        if ((U32)param != _RwDlStateCache.fogColor)
        {
            RwCamera* camera = RwCameraGetCurrentCamera();

            _RwDlStateCache.gxFogColor.a = (U32)param >> 24;
            _RwDlStateCache.gxFogColor.r = ((U32)param >> 16) & 0xFF;
            _RwDlStateCache.gxFogColor.g = ((U32)param >> 8) & 0xFF;
            _RwDlStateCache.gxFogColor.b = (U32)param & 0xFF;
            DlRenderStateSetFog(_RwDlFogConvTable[_RwDlStateCache.fogType], camera);
            _RwDlStateCache.fogColor = (S32)param;
        }

        result = TRUE;
        break;
    case rwRENDERSTATEFOGTYPE:
    {
        S32 accepted;

        if ((S32)param != _RwDlStateCache.fogType)
        {
            RwCamera* camera;

            if ((S32)param != rwFOGTYPELINEAR)
            {
                accepted = FALSE;
                goto fogtypeDone;
            }

            camera = RwCameraGetCurrentCamera();
            DlRenderStateSetFog(_RwDlFogConvTable[(S32)param], camera);
            _RwDlStateCache.fogType = (S32)param;
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
            RwTextureSetAddressing(_RwDlTexture, param);
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
            _RwDlTexture->filterAddressing =
                (_RwDlTexture->filterAddressing & ~rwTEXTUREADDRESSINGUMASK) |
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
            _RwDlTexture->filterAddressing =
                (_RwDlTexture->filterAddressing & ~rwTEXTUREADDRESSINGVMASK) |
                (((S32)param << 12) & rwTEXTUREADDRESSINGVMASK);
            accepted = TRUE;
        }
        result = accepted;
        break;
    }
    case rwRENDERSTATETEXTUREFILTER:
        RwTextureSetFilterMode(_RwDlTexture, param);
        result = TRUE;
        break;
    case rwRENDERSTATETEXTURERASTER:
        if (param != _RwDlTexture->raster)
        {
            _rwDlTextureSetRaster(_RwDlTexture, param, 0);
        }

        result = TRUE;
        break;
    case rwRENDERSTATEZWRITEENABLE:
        if ((S32)param)
        {
            if (!_RwDlStateCache.zWriteEnable)
            {
                GXSetZMode(GX_TRUE, _RwDlStateCache.zFunc, GX_TRUE);
                _RwDlStateCache.zWriteEnable = TRUE;
            }
        }
        else if (_RwDlStateCache.zWriteEnable)
        {
            GXSetZMode(GX_TRUE, _RwDlStateCache.zFunc, GX_FALSE);
            _RwDlStateCache.zWriteEnable = FALSE;
        }

        result = TRUE;
        break;
    case rwRENDERSTATEZTESTENABLE:
        if ((S32)param)
        {
            if (!_RwDlStateCache.zTestEnable)
            {
                GXSetZMode(GX_TRUE, GX_LEQUAL, _RwDlStateCache.zWriteEnable);
                _RwDlStateCache.zTestEnable = TRUE;
                _RwDlStateCache.zFunc = GX_LEQUAL;
            }
        }
        else if (_RwDlStateCache.zTestEnable)
        {
            GXSetZMode(GX_TRUE, GX_ALWAYS, _RwDlStateCache.zWriteEnable);
            _RwDlStateCache.zTestEnable = FALSE;
            _RwDlStateCache.zFunc = GX_ALWAYS;
        }

        result = TRUE;
        break;
    case rwRENDERSTATESRCBLEND:
    {
        S32 accepted;

        if ((S32)param != _RwDlStateCache.srcBlend)
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
                GXBlendFactor factor = _RwDlBlendConvTable[(S32)param];

                GXSetBlendMode(GX_BM_BLEND, factor, _RwDlStateCache.gxDestBlend, GX_LO_CLEAR);
                _RwDlStateCache.srcBlend = (S32)param;
                _RwDlStateCache.gxSrcBlend = factor;
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

        if ((S32)param != _RwDlStateCache.destBlend)
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
                GXBlendFactor factor = _RwDlBlendConvTable[(S32)param];

                GXSetBlendMode(GX_BM_BLEND, _RwDlStateCache.gxSrcBlend, factor, GX_LO_CLEAR);
                _RwDlStateCache.destBlend = (S32)param;
                _RwDlStateCache.gxDestBlend = factor;
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
        if ((S32)param != _RwDlStateCache.cullMode)
        {
            GXSetCullMode((S32)param - 1);
            _RwDlStateCache.cullMode = (S32)param;
        }

        result = TRUE;
        break;
    }

    return result;
}
