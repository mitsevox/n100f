#include <rwcore.h>

static S32 SubmitLineNode(RxPipelineNode* self, RxPipelineNodeParam* params)
{
    RxPacket* packet;
    RxCluster* devVerts;
    RxCluster* indices;
    RxCluster* meshState;
    RxCluster* renderState;
    RxMeshStateVector* meshData;
    RxRenderStateVector* rsvp;

    packet = RxPacketFetch(self);

    devVerts = RxClusterLockRead(packet, 0);
    indices = RxClusterLockRead(packet, 1);
    meshState = RxClusterLockRead(packet, 2);
    renderState = RxClusterLockRead(packet, 3);

    meshData = (RxMeshStateVector*)meshState->currentData;

    if (renderState && renderState->data)
    {
        rsvp = (RxRenderStateVector*)renderState->currentData;
    }
    else
    {
        rsvp = &RXPIPELINEGLOBAL(defaultRenderState);
    }

    if (rsvp->TextureRaster)
    {
        RwRenderStateSet(rwRENDERSTATETEXTURERASTER, rsvp->TextureRaster);
        RwRenderStateSet(rwRENDERSTATETEXTUREFILTER, (void*)rsvp->FilterMode);

        if (rsvp->AddressModeU == rsvp->AddressModeV)
        {
            RwRenderStateSet(rwRENDERSTATETEXTUREADDRESS, (void*)rsvp->AddressModeU);
        }
        else
        {
            RwRenderStateSet(rwRENDERSTATETEXTUREADDRESSU, (void*)rsvp->AddressModeU);
            RwRenderStateSet(rwRENDERSTATETEXTUREADDRESSV, (void*)rsvp->AddressModeV);
        }
    }
    else
    {
        RwRenderStateSet(rwRENDERSTATETEXTURERASTER, NULL);
    }

    RwRenderStateSet(rwRENDERSTATESHADEMODE, (void*)rsvp->ShadeMode);
    RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE,
                     (void*)((rsvp->Flags & RSV_FLAG_VERTEXALPHA) != 0));
    RwRenderStateSet(rwRENDERSTATESRCBLEND, (void*)rsvp->SrcBlend);
    RwRenderStateSet(rwRENDERSTATEDESTBLEND, (void*)rsvp->DestBlend);
    RwRenderStateSet(rwRENDERSTATEZTESTENABLE, (void*)((rsvp->Flags & RSV_FLAG_ZTEST) != 0));
    RwRenderStateSet(rwRENDERSTATEZWRITEENABLE, (void*)((rsvp->Flags & RSV_FLAG_ZWRITE) != 0));

    if (indices && indices->numUsed)
    {
        RwIm2DRenderIndexedPrimitive(rwPRIMTYPELINELIST, (RwIm2DVertex*)devVerts->currentData,
                                     meshData->NumVertices,
                                     (RwImVertexIndex*)indices->currentData,
                                     meshData->NumElements * 2);
    }
    else
    {
        RwIm2DRenderPrimitive(rwPRIMTYPELINELIST, (RwIm2DVertex*)devVerts->currentData,
                              meshData->NumVertices);
    }

    RxPacketDispatch(packet, 0, self);

    return TRUE;
}

RxNodeDefinition* RxNodeDefinitionGetSubmitLine(void)
{
    static RxClusterRef N1clofinterest[] = {
        { &RxClScrSpace2DVertices, rxCLALLOWABSENT, 0 },
        { &RxClIndices, rxCLALLOWABSENT, 0 },
        { &RxClMeshState, rxCLALLOWABSENT, 0 },
        { &RxClRenderState, rxCLALLOWABSENT, 0 },
    };

    static RxClusterValidityReq N1inputreqs[] = {
        rxCLREQ_REQUIRED,
        rxCLREQ_OPTIONAL,
        rxCLREQ_REQUIRED,
        rxCLREQ_OPTIONAL,
    };

    static RxClusterValid N1outcl1[] = {
        rxCLVALID_NOCHANGE,
        rxCLVALID_NOCHANGE,
        rxCLVALID_NOCHANGE,
        rxCLVALID_NOCHANGE,
    };

    static char _SubmitOut[] = "SubmitOut";

    static RxOutputSpec N1outputs[] = {
        { _SubmitOut, N1outcl1, rxCLVALID_NOCHANGE },
    };

    static char _SubmitLine_csl[] = "SubmitLine.csl";

    static RxNodeDefinition nodeSubmitLineCSL = {
        _SubmitLine_csl,
        { SubmitLineNode, NULL, NULL, NULL, NULL, NULL, NULL },
        { 4, N1clofinterest, N1inputreqs, 1, N1outputs },
        0,
        rxNODEDEFCONST,
        0,
    };

    return &nodeSubmitLineCSL;
}
