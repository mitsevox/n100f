#include "zRenderState.h"
#include "xstransvc.h"

#include "rwcore.h"
#include "dolphin/gx/GXTev.h"

// The game's blend modes map one to one onto RenderWare's rwBLEND values.
static S32 sBlendTable[11] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 };
static _SDRenderState sRS;

// The compiler's deferred mode emits the functions in reverse order of definition.
void zRenderStateInit()
{
    sRS = SDRS_Unknown;
}

// Every state starts from the default one.
void zRenderState(_SDRenderState next)
{
    if (next == sRS)
    {
        return;
    }

    if (next != SDRS_Default)
    {
        zRenderState(SDRS_Default);
    }

    sRS = next;

    switch (next)
    {
    case SDRS_Default:
        RwRenderStateSet(rwRENDERSTATETEXTUREFILTER, (void*)2);
        RwRenderStateSet(rwRENDERSTATEFOGENABLE, (void*)1);
        RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, (void*)1);
        RwRenderStateSet(rwRENDERSTATECULLMODE, (void*)1);
        RwRenderStateSet(rwRENDERSTATETEXTUREADDRESS, (void*)1);
        RwRenderStateSet(rwRENDERSTATESRCBLEND, (void*)1);
        RwRenderStateSet(rwRENDERSTATEDESTBLEND, (void*)2);
        RwRenderStateSet(rwRENDERSTATESHADEMODE, (void*)2);
        RwRenderStateSet(rwRENDERSTATEZWRITEENABLE, (void*)1);
        RwRenderStateSet(rwRENDERSTATEZTESTENABLE, (void*)1);
        break;
    case SDRS_Particles:
        RwRenderStateSet(rwRENDERSTATEZWRITEENABLE, (void*)0);
        RwRenderStateSet(rwRENDERSTATEZTESTENABLE, (void*)1);
        RwRenderStateSet(rwRENDERSTATETEXTUREFILTER, (void*)2);
        RwRenderStateSet(rwRENDERSTATEFOGENABLE, (void*)0);
        RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, (void*)1);
        RwRenderStateSet(rwRENDERSTATECULLMODE, (void*)1);
        RwRenderStateSet(rwRENDERSTATETEXTUREADDRESS, (void*)1);
        RwRenderStateSet(rwRENDERSTATESHADEMODE, (void*)1);
        break;
    case SDRS_Tide:
        RwRenderStateSet(rwRENDERSTATEZWRITEENABLE, (void*)1);
        RwRenderStateSet(rwRENDERSTATEZTESTENABLE, (void*)1);
        RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, (void*)1);
        RwRenderStateSet(rwRENDERSTATESRCBLEND, (void*)5);
        RwRenderStateSet(rwRENDERSTATEDESTBLEND, (void*)6);
        RwRenderStateSet(rwRENDERSTATECULLMODE, (void*)2);
        break;
    case SDRS_OpaqueModels:
        RwRenderStateSet(rwRENDERSTATEZWRITEENABLE, (void*)1);
        RwRenderStateSet(rwRENDERSTATEZTESTENABLE, (void*)1);
        RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, (void*)0);
        RwRenderStateSet(rwRENDERSTATECULLMODE, (void*)1);
        RwRenderStateSet(rwRENDERSTATESHADEMODE, (void*)2);
        RwRenderStateSet(rwRENDERSTATESRCBLEND, (void*)2);
        RwRenderStateSet(rwRENDERSTATEDESTBLEND, (void*)1);
        break;
    case SDRS_AlphaModels:
        RwRenderStateSet(rwRENDERSTATEZWRITEENABLE, (void*)1);
        RwRenderStateSet(rwRENDERSTATEZTESTENABLE, (void*)1);
        RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, (void*)1);
        RwRenderStateSet(rwRENDERSTATECULLMODE, (void*)1);
        RwRenderStateSet(rwRENDERSTATESHADEMODE, (void*)2);
        RwRenderStateSet(rwRENDERSTATESRCBLEND, (void*)5);
        RwRenderStateSet(rwRENDERSTATEDESTBLEND, (void*)6);
        GXSetAlphaCompare(GX_GEQUAL, 0x80, GX_AOP_AND, GX_LEQUAL, 0xFF);
        break;
    case SDRS_Lightning:
        RwRenderStateSet(rwRENDERSTATEZWRITEENABLE, (void*)0);
        RwRenderStateSet(rwRENDERSTATEZTESTENABLE, (void*)1);
        RwRenderStateSet(rwRENDERSTATESHADEMODE, (void*)1);
        RwRenderStateSet(rwRENDERSTATECULLMODE, (void*)1);
        RwRenderStateSet(rwRENDERSTATESRCBLEND, (void*)5);
        RwRenderStateSet(rwRENDERSTATEDESTBLEND, (void*)6);
        break;
    case SDRS_SkidMark:
        RwRenderStateSet(rwRENDERSTATEZWRITEENABLE, (void*)0);
        RwRenderStateSet(rwRENDERSTATEZTESTENABLE, (void*)1);
        RwRenderStateSet(rwRENDERSTATESHADEMODE, (void*)2);
        RwRenderStateSet(rwRENDERSTATECULLMODE, (void*)1);
        RwRenderStateSet(rwRENDERSTATESRCBLEND, (void*)5);
        RwRenderStateSet(rwRENDERSTATEDESTBLEND, (void*)6);
        break;
    case SDRS_Font:
        RwRenderStateSet(rwRENDERSTATEZWRITEENABLE, (void*)0);
        RwRenderStateSet(rwRENDERSTATEZTESTENABLE, (void*)0);
        RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, (void*)0);
        RwRenderStateSet(rwRENDERSTATECULLMODE, (void*)1);
        RwRenderStateSet(rwRENDERSTATESRCBLEND, (void*)2);
        RwRenderStateSet(rwRENDERSTATEDESTBLEND, (void*)1);
        break;
    case SDRS_HUD:
        RwRenderStateSet(rwRENDERSTATEZWRITEENABLE, (void*)1);
        RwRenderStateSet(rwRENDERSTATEZTESTENABLE, (void*)1);
        RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, (void*)1);
        RwRenderStateSet(rwRENDERSTATECULLMODE, (void*)2);
        RwRenderStateSet(rwRENDERSTATESRCBLEND, (void*)5);
        RwRenderStateSet(rwRENDERSTATEDESTBLEND, (void*)6);
        break;
    case SDRS_Bubble:
        RwRenderStateSet(rwRENDERSTATEZWRITEENABLE, (void*)1);
        RwRenderStateSet(rwRENDERSTATEZTESTENABLE, (void*)1);
        RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, (void*)0);
        RwRenderStateSet(rwRENDERSTATESRCBLEND, (void*)5);
        RwRenderStateSet(rwRENDERSTATEDESTBLEND, (void*)6);
        RwRenderStateSet(rwRENDERSTATECULLMODE, (void*)2);
        break;
    case SDRS_SkyBack:
        RwRenderStateSet(rwRENDERSTATEFOGENABLE, (void*)0);
        RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, (void*)0);
        RwRenderStateSet(rwRENDERSTATECULLMODE, (void*)1);
        RwRenderStateSet(rwRENDERSTATETEXTUREADDRESS, (void*)1);
        RwRenderStateSet(rwRENDERSTATESHADEMODE, (void*)1);
        RwRenderStateSet(rwRENDERSTATEZWRITEENABLE, (void*)0);
        RwRenderStateSet(rwRENDERSTATEZTESTENABLE, (void*)0);
        break;
    }
}

void zRenderStateSetTexture(U32 assetID)
{
    if (assetID == 0)
    {
        RwRenderStateSet(rwRENDERSTATETEXTURERASTER, NULL);
    }
    else
    {
        RwTexture* texture = (RwTexture*)xSTFindAsset(assetID, NULL);
        if (texture)
        {
            RwRaster* raster = texture->raster;
            if (raster)
            {
                RwRenderStateSet(rwRENDERSTATETEXTURERASTER, raster);
            }
        }
    }
}

void zRenderStateSetSrcBlendMode(S32 xmode)
{
    RwRenderStateSet(rwRENDERSTATESRCBLEND, (void*)sBlendTable[xmode]);
}

void zRenderStateSetDstBlendMode(S32 xmode)
{
    RwRenderStateSet(rwRENDERSTATEDESTBLEND, (void*)sBlendTable[xmode]);
}
