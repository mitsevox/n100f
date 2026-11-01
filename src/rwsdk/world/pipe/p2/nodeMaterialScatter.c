#include <rpworld.h>

// The material scatter node: sends each mesh's packet on to its material's pipeline, or to the
// default material pipeline where the material has none.

extern RxClusterDefinition RxClLights;

// The node's cluster slots.
#define SCATTERCL_MESHSTATE 0

static S32 _MaterialScatterNode(RxPipelineNode* self, RxPipelineNodeParam* params)
{
    RxPacket* packet = RxPacketFetch(self);
    RxCluster* meshState = RxClusterLockRead(packet, SCATTERCL_MESHSTATE);
    RxPipeline* pipeline = ((RxMeshStateVector*)meshState->currentData)->Pipeline;

    if (!pipeline)
    {
        pipeline = RXPIPELINEGLOBAL(defaultMaterialPipeline);
    }

    RxPacketDispatchToPipeline(packet, pipeline, self);

    return TRUE;
}

RxNodeDefinition* RxNodeDefinitionGetMaterialScatter(void)
{
    static RxClusterRef N2clofinterest[] = {
        { &RxClMeshState, rxCLALLOWABSENT, 0 },
        { &RxClObjSpace3DVertices, rxCLALLOWABSENT, 0 },
        { &RxClIndices, rxCLALLOWABSENT, 0 },
        { &RxClRenderState, rxCLALLOWABSENT, 0 },
        { &RxClLights, rxCLALLOWABSENT, 0 },
    };

    static RxClusterValidityReq N2inputreqs[] = {
        rxCLREQ_REQUIRED,
        rxCLREQ_OPTIONAL,
        rxCLREQ_OPTIONAL,
        rxCLREQ_OPTIONAL,
        rxCLREQ_OPTIONAL,
    };

    static char _MaterialScatter_csl[] = "MaterialScatter.csl";

    static RxNodeDefinition nodeMaterialScatterCSL = {
        _MaterialScatter_csl,
        { _MaterialScatterNode, NULL, NULL, NULL, NULL, NULL, NULL },
        { 5, N2clofinterest, N2inputreqs, 0, NULL },
        0,
        rxNODEDEFCONST,
        0,
    };

    return &nodeMaterialScatterCSL;
}
