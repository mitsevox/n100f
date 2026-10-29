#include <rwcore.h>

// RxRenderStateVector flags.
#define RSV_FLAG_TEXTUREPERSPECTIVE 0x1
#define RSV_FLAG_ZTEST 0x2
#define RSV_FLAG_ZWRITE 0x4
#define RSV_FLAG_VERTEXALPHA 0x8
#define RSV_FLAG_STATE19 0x20

// Render states this unit reads that the enum does not name.
#define RSV_STATE_FOGTABLE ((RwRenderState)18)
#define RSV_STATE_19 ((RwRenderState)19)

RxRenderStateVector* RxRenderStateVectorSetDefaultRenderStateVector(RxRenderStateVector* rsvp)
{
    if (rsvp)
    {
        if (RWSRCGLOBAL(engineStatus) == rwENGINESTATUSSTARTED)
        {
            *rsvp = RXPIPELINEGLOBAL(defaultRenderState);
        }
        else if (rsvp != &RXPIPELINEGLOBAL(defaultRenderState))
        {
            RwError err;

            err.pluginID = 1;
            err.errorCode = _rwerror(0x80000018);
            RwErrorSet(&err);

            return NULL;
        }
        else
        {
            RwRGBA white = { 255, 255, 255, 255 };

            rsvp->Flags = RSV_FLAG_TEXTUREPERSPECTIVE | RSV_FLAG_ZTEST | RSV_FLAG_ZWRITE;
            rsvp->ShadeMode = rwSHADEMODEGOURAUD;
            rsvp->SrcBlend = rwBLENDSRCALPHA;
            rsvp->DestBlend = rwBLENDINVSRCALPHA;
            rsvp->TextureRaster = NULL;
            rsvp->AddressModeU = rwTEXTUREADDRESSWRAP;
            rsvp->AddressModeV = rwTEXTUREADDRESSWRAP;
            rsvp->FilterMode = rwFILTERLINEAR;
            rsvp->BorderColor = white;
            rsvp->FogType = rwFOGTYPENAFOGTYPE;
            rsvp->FogColor = white;
            rsvp->FogTable = NULL;
        }

        return rsvp;
    }

    {
        RwError err;

        err.pluginID = 1;
        err.errorCode = _rwerror(0x80000016);
        RwErrorSet(&err);
    }

    return NULL;
}

RxRenderStateVector* RxRenderStateVectorCreate(S32 current)
{
    if (RWSRCGLOBAL(engineStatus) == rwENGINESTATUSSTARTED)
    {
        RxRenderStateVector* rsvp = (RxRenderStateVector*)RwMalloc(sizeof(RxRenderStateVector));

        if (rsvp)
        {
            if (current)
            {
                if (!RxRenderStateVectorLoadDriverState(rsvp))
                {
                    RxRenderStateVectorDestroy(rsvp);
                }
            }
            else
            {
                *rsvp = RXPIPELINEGLOBAL(defaultRenderState);
            }

            return rsvp;
        }

        {
            RwError err;

            err.pluginID = 1;
            err.errorCode = _rwerror(0x80000013, sizeof(RxRenderStateVector));
            RwErrorSet(&err);
        }

        return NULL;
    }

    {
        RwError err;

        err.pluginID = 1;
        err.errorCode = _rwerror(0x80000018);
        RwErrorSet(&err);
    }

    return NULL;
}

void RxRenderStateVectorDestroy(RxRenderStateVector* rsvp)
{
    if (rsvp)
    {
        RwFree(rsvp);
    }
    else
    {
        RwError err;

        err.pluginID = 1;
        err.errorCode = _rwerror(0x80000016);
        RwErrorSet(&err);
    }
}

RxRenderStateVector* RxRenderStateVectorLoadDriverState(RxRenderStateVector* rsvp)
{
    if (rsvp)
    {
        S32 state;

        rsvp->Flags = 0;

        RwRenderStateGet(rwRENDERSTATETEXTUREPERSPECTIVE, &state);
        if (state)
        {
            rsvp->Flags |= RSV_FLAG_TEXTUREPERSPECTIVE;
        }

        RwRenderStateGet(rwRENDERSTATEZTESTENABLE, &state);
        if (state)
        {
            rsvp->Flags |= RSV_FLAG_ZTEST;
        }

        RwRenderStateGet(rwRENDERSTATEZWRITEENABLE, &state);
        if (state)
        {
            rsvp->Flags |= RSV_FLAG_ZWRITE;
        }

        RwRenderStateGet(RSV_STATE_19, &state);
        if (state)
        {
            rsvp->Flags |= RSV_FLAG_STATE19;
        }

        RwRenderStateGet(rwRENDERSTATEVERTEXALPHAENABLE, &state);
        if (state)
        {
            rsvp->Flags |= RSV_FLAG_VERTEXALPHA;
        }

        RwRenderStateGet(rwRENDERSTATESHADEMODE, &rsvp->ShadeMode);
        RwRenderStateGet(rwRENDERSTATESRCBLEND, &rsvp->SrcBlend);
        RwRenderStateGet(rwRENDERSTATEDESTBLEND, &rsvp->DestBlend);
        RwRenderStateGet(rwRENDERSTATETEXTURERASTER, &rsvp->TextureRaster);

        if (RwRenderStateGet(rwRENDERSTATETEXTUREADDRESS, &rsvp->AddressModeU))
        {
            rsvp->AddressModeV = rsvp->AddressModeU;
        }
        else
        {
            RwRenderStateGet(rwRENDERSTATETEXTUREADDRESSU, &rsvp->AddressModeU);
            RwRenderStateGet(rwRENDERSTATETEXTUREADDRESSV, &rsvp->AddressModeV);
        }

        RwRenderStateGet(rwRENDERSTATETEXTUREFILTER, &rsvp->FilterMode);
        RwRenderStateGet(rwRENDERSTATEBORDERCOLOR, &rsvp->BorderColor);
        RwRenderStateGet(rwRENDERSTATEFOGTYPE, &rsvp->FogType);
        RwRenderStateGet(rwRENDERSTATEFOGCOLOR, &rsvp->FogColor);
        RwRenderStateGet(RSV_STATE_FOGTABLE, &rsvp->FogTable);

        return rsvp;
    }

    {
        RwError err;

        err.pluginID = 1;
        err.errorCode = _rwerror(0x80000016);
        RwErrorSet(&err);
    }

    return NULL;
}
