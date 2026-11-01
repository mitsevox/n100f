#include <rpworld.h>

// The generic atomic instance node: copies an atomic's vertices (blending morph targets when the
// atomic is between two) and its meshes into the packet's clusters. The GameCube build uses its
// own all-in-one node instead; this one stays linked from the shared pipeline code.

// The node body is still in assembly (its June 2001 name is AtomicInstanceNode, with the helpers
// instanceVertPos and instanceMeshes).
static S32 AtomicInstanceNode(RxPipelineNode* self, RxPipelineNodeParam* params);

RxNodeDefinition* RxNodeDefinitionGetAtomicInstance(void)
{
    static RxClusterRef N1clofinterest[] = {
        { &RxClObjSpace3DVertices, rxCLALLOWABSENT, 0 },
        { &RxClIndices, rxCLALLOWABSENT, 0 },
        { &RxClMeshState, rxCLALLOWABSENT, 0 },
        { &RxClRenderState, rxCLALLOWABSENT, 0 },
    };

    static RxClusterValidityReq N1inputreqs[] = {
        rxCLREQ_DONTWANT,
        rxCLREQ_DONTWANT,
        rxCLREQ_DONTWANT,
        rxCLREQ_DONTWANT,
    };

    static char _AtomicInstanceOut[] = "AtomicInstanceOut";

    static RxClusterValid N1outcl1[] = {
        rxCLVALID_VALID,
        rxCLVALID_VALID,
        rxCLVALID_VALID,
        rxCLVALID_VALID,
    };

    static RxOutputSpec N1outputs[] = {
        { _AtomicInstanceOut, N1outcl1, rxCLVALID_NOCHANGE },
    };

    static char _AtomicInstance_csl[] = "AtomicInstance.csl";

    static RxNodeDefinition nodeAtomicInstanceCSL = {
        _AtomicInstance_csl,
        { AtomicInstanceNode, NULL, NULL, NULL, NULL, NULL, NULL },
        { 4, N1clofinterest, N1inputreqs, 1, N1outputs },
        0,
        rxNODEDEFCONST,
        0,
    };

    return &nodeAtomicInstanceCSL;
}
