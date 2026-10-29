#include <rwcore.h>

static S32 _ImmRenderSetupNode(RxPipelineNode* self, RxPipelineNodeParam* params)
{
    _rwIm3DPoolStash* stash = (_rwIm3DPoolStash*)params->dataParam;
    RxRenderStateVector* rsvp;
    RxPacket* packet;
    RxCluster* cluster;
    U32 numElements;

    if (!stash->objVerts)
    {
        return TRUE;
    }

    rsvp = RxRenderStateVectorCreate(TRUE);
    packet = RxPacketCreate(self);

    cluster = RxClusterLockWrite(packet, 0, self);

    if (cluster)
    {
        RxClusterSetExternalData(cluster, stash->objVerts, sizeof(RxObjSpace3DVertex),
                                 stash->numVerts);
    }

    if (stash->camVerts)
    {
        cluster = RxClusterLockWrite(packet, 1, self);
        RxClusterSetExternalData(cluster, stash->camVerts, sizeof(RxCamSpace3DVertex),
                                 stash->numVerts);
    }

    if (stash->devVerts)
    {
        cluster = RxClusterLockWrite(packet, 2, self);
        RxClusterSetExternalData(cluster, stash->devVerts, sizeof(RwIm2DVertex), stash->numVerts);
    }

    cluster = RxClusterLockWrite(packet, 3, self);
    RxClusterSetExternalData(cluster, stash->meshState, sizeof(RxMeshStateVector), 1);

    switch (stash->primType)
    {
    case rwPRIMTYPETRISTRIP:
        numElements = stash->numIndices - 2;
        break;
    case rwPRIMTYPETRILIST:
        numElements = stash->numIndices / 3;
        break;
    case rwPRIMTYPEPOINTLIST:
        numElements = stash->numIndices;
        break;
    case rwPRIMTYPELINELIST:
        numElements = stash->numIndices / 2;
        break;
    case rwPRIMTYPEPOLYLINE:
        numElements = stash->numIndices - 1;
        break;
    case rwPRIMTYPETRIFAN:
        numElements = stash->numIndices - 2;
        break;
    default:
        return FALSE;
    }

    stash->meshState->NumElements = numElements;
    stash->meshState->PrimType = stash->primType;

    cluster = RxClusterLockWrite(packet, 4, self);

    if (stash->renderState)
    {
        S32 vertexAlpha = FALSE;

        RxClusterSetExternalData(cluster, stash->renderState, sizeof(RxRenderStateVector), 1);

        if (stash->renderState->Flags & RSV_FLAG_VERTEXALPHA)
        {
            vertexAlpha = TRUE;
        }

        *stash->renderState = *rsvp;

        if (!vertexAlpha)
        {
            stash->renderState->Flags &= ~RSV_FLAG_VERTEXALPHA;
        }
    }
    else
    {
        cluster = RxClusterInitializeData(cluster, 1, sizeof(RxRenderStateVector));
        *(RxRenderStateVector*)cluster->currentData = *rsvp;
        cluster->numUsed++;
    }

    RxRenderStateVectorDestroy(rsvp);

    if (stash->indices)
    {
        cluster = RxClusterLockWrite(packet, 5, self);
        RxClusterSetExternalData(cluster, stash->indices, sizeof(RwImVertexIndex),
                                 stash->numIndices);
        RxPacketDispatch(packet, 0, self);
    }
    else
    {
        RxPacketDispatch(packet, 1, self);
    }

    return TRUE;
}

RxNodeDefinition* RxNodeDefinitionGetImmRenderSetup(void)
{
    static RxClusterRef N1clofinterest[] = {
        { &RxClObjSpace3DVertices, rxCLALLOWABSENT, 0 },
        { &RxClCamSpace3DVertices, rxCLALLOWABSENT, 0 },
        { &RxClScrSpace2DVertices, rxCLALLOWABSENT, 0 },
        { &RxClMeshState, rxCLALLOWABSENT, 0 },
        { &RxClRenderState, rxCLALLOWABSENT, 0 },
        { &RxClIndices, rxCLALLOWABSENT, 0 },
    };

    static RxClusterValidityReq N1inputreqs[] = {
        rxCLREQ_DONTWANT, rxCLREQ_DONTWANT, rxCLREQ_DONTWANT,
        rxCLREQ_DONTWANT, rxCLREQ_DONTWANT, rxCLREQ_DONTWANT,
    };

    static RxClusterValid N1outcl1[] = {
        rxCLVALID_VALID, rxCLVALID_VALID, rxCLVALID_VALID,
        rxCLVALID_VALID, rxCLVALID_VALID, rxCLVALID_VALID,
    };

    static RxClusterValid N1outcl2[] = {
        rxCLVALID_VALID, rxCLVALID_VALID, rxCLVALID_VALID,
        rxCLVALID_VALID, rxCLVALID_VALID, rxCLVALID_INVALID,
    };

    static char _ImmRenderSetupOut[] = "ImmRenderSetupOut";
    static char _ImmRenderSetupOutUnindexed[] = "ImmRenderSetupOutUnindexed";

    static RxOutputSpec N1outputs[] = {
        { _ImmRenderSetupOut, N1outcl1, rxCLVALID_INVALID },
        { _ImmRenderSetupOutUnindexed, N1outcl2, rxCLVALID_INVALID },
    };

    static char _ImmRenderSetup_csl[] = "ImmRenderSetup.csl";

    static RxNodeDefinition nodeImmRenderSetupCSL = {
        _ImmRenderSetup_csl,
        { _ImmRenderSetupNode, NULL, NULL, NULL, NULL, NULL, NULL },
        { 6, N1clofinterest, N1inputreqs, 2, N1outputs },
        0,
        rxNODEDEFCONST,
        0,
    };

    return &nodeImmRenderSetupCSL;
}
