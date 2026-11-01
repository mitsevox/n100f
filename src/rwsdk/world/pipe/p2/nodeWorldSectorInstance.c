#include <rpworld.h>

// The generic world sector instance node: copies a sector's vertices and meshes into the packet's
// clusters. The GameCube build uses its own all-in-one node instead; this one stays linked from
// the shared pipeline code.

// The node body is still in assembly (its June 2001 name is WorldSectorInstanceNodeFn, with the
// helpers instanceVerts, instanceMeshes and calcTriPlanes).
static S32 WorldSectorInstanceNodeFn(RxPipelineNode* self, RxPipelineNodeParam* params);

RxNodeDefinition* RxNodeDefinitionGetWorldSectorInstance(void)
{
    static RxClusterRef gNodeClusters[] = {
        { &RxClObjSpace3DVertices, rxCLALLOWABSENT, 0 },
        { &RxClIndices, rxCLALLOWABSENT, 0 },
        { &RxClMeshState, rxCLALLOWABSENT, 0 },
        { &RxClRenderState, rxCLALLOWABSENT, 0 },
    };

    static RxClusterValidityReq gNodeReqs[] = {
        rxCLREQ_DONTWANT,
        rxCLREQ_DONTWANT,
        rxCLREQ_DONTWANT,
        rxCLREQ_DONTWANT,
    };

    static char _Output[] = "Output";

    static RxClusterValid gNodeOut1[] = {
        rxCLVALID_VALID,
        rxCLVALID_VALID,
        rxCLVALID_VALID,
        rxCLVALID_VALID,
    };

    static RxOutputSpec gNodeOuts[] = {
        { _Output, gNodeOut1, rxCLVALID_INVALID },
    };

    static char _WorldSectorInstance_csl[] = "WorldSectorInstance.csl";

    static RxNodeDefinition nodeWorldSectorInstanceCSL = {
        _WorldSectorInstance_csl,
        { WorldSectorInstanceNodeFn, NULL, NULL, NULL, NULL, NULL, NULL },
        { 4, gNodeClusters, gNodeReqs, 1, gNodeOuts },
        0,
        rxNODEDEFCONST,
        0,
    };

    return &nodeWorldSectorInstanceCSL;
}
