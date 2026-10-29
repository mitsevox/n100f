#include <rwcore.h>
#include <string.h>

static S32 PL2ImmStashNodeBody(RxPipelineNode* self, RxPipelineNodeParam* params)
{
    _rwIm3DPoolStash* stash = &((rwIm3DPool*)params->dataParam)->stash;
    RxPacket* packet;
    RxCluster* cluster;

    if (_rxExecCtxGlobal.pipeline->embeddedPacketState == rxPKST_PENDING)
    {
        _rxExecCtxGlobal.pipeline->embeddedPacketState = rxPKST_INUSE;
        packet = _rxExecCtxGlobal.pipeline->embeddedPacket;
    }
    else
    {
        packet = NULL;
    }

    memset(stash, 0, sizeof(_rwIm3DPoolStash));

    cluster = RxClusterLockRead(packet, 0);
    stash->objVerts = (RxObjSpace3DVertex*)cluster->currentData;
    cluster->flags |= CLUSTER_EXTERNAL;

    cluster = RxClusterLockRead(packet, 1);
    stash->camVerts = cluster->currentData;
    cluster->flags |= CLUSTER_EXTERNAL;

    cluster = RxClusterLockRead(packet, 2);
    stash->devVerts = cluster->currentData;
    cluster->flags |= CLUSTER_EXTERNAL;

    cluster = RxClusterLockRead(packet, 3);
    stash->meshState = (RxMeshStateVector*)cluster->currentData;
    stash->numVerts = stash->meshState->NumVertices;
    cluster->flags |= CLUSTER_EXTERNAL;

    cluster = RxClusterLockRead(packet, 4);
    stash->renderState = (RxRenderStateVector*)cluster->currentData;
    cluster->flags |= CLUSTER_EXTERNAL;

    _rxPacketDestroy(packet);

    return TRUE;
}

RxNodeDefinition* RxNodeDefinitionGetImmStash(void)
{
    static RxClusterRef nodeClusters[] = {
        { &RxClObjSpace3DVertices, rxCLALLOWABSENT, 0 },
        { &RxClCamSpace3DVertices, rxCLALLOWABSENT, 0 },
        { &RxClScrSpace2DVertices, rxCLALLOWABSENT, 0 },
        { &RxClMeshState, rxCLALLOWABSENT, 0 },
        { &RxClRenderState, rxCLALLOWABSENT, 0 },
    };

    static RxClusterValidityReq nodeReqs[] = {
        rxCLREQ_OPTIONAL, rxCLREQ_OPTIONAL, rxCLREQ_OPTIONAL, rxCLREQ_OPTIONAL, rxCLREQ_OPTIONAL,
    };

    static char _ImmStash_csl[] = "ImmStash.csl";

    static RxNodeDefinition nodeImmStashCSL = {
        _ImmStash_csl,
        { PL2ImmStashNodeBody, NULL, NULL, NULL, NULL, NULL, NULL },
        { 5, nodeClusters, nodeReqs, 0, NULL },
        0,
        rxNODEDEFCONST,
        0,
    };

    return &nodeImmStashCSL;
}
