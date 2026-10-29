#include <rwcore.h>

static S32 ImmInstanceNode(RxPipelineNode* self, RxPipelineNodeParam* params)
{
    RwRGBA white = { 255, 255, 255, 255 };
    rwIm3DPool* pool = (rwIm3DPool*)params->dataParam;
    U32 numVerts = pool->numElements;
    RxPacket* packet;
    RxCluster* objVerts;
    RxCluster* meshState;
    RxCluster* renderState;
    RxMeshStateVector* meshData;
    RxRenderStateVector* rsvp;

    packet = RxPacketCreate(self);
    objVerts = RxClusterLockWrite(packet, 0, self);
    meshState = RxClusterLockWrite(packet, 1, self);
    renderState = RxClusterLockWrite(packet, 2, self);

    meshState = RxClusterInitializeData(meshState, 1, sizeof(RxMeshStateVector));
    renderState = RxClusterInitializeData(renderState, 1, sizeof(RxRenderStateVector));

    RxClusterSetExternalData(objVerts, pool->elements, pool->stride, numVerts);

    meshData = (RxMeshStateVector*)meshState->currentData;
    meshData->Flags = 0x100;
    meshData->Flags |= (pool->stash.flags & rwIM3D_VERTEXUV) ? 0x4 : 0;
    meshData->SourceObject = pool;

    if (pool->stash.ltm)
    {
        meshData->Obj2Cam = *pool->stash.ltm;
        RwMatrixTransform(&meshData->Obj2Cam, &RwCameraGetCurrentCamera()->viewMatrix,
                          rwCOMBINEPOSTCONCAT);
        meshData->Obj2World = *pool->stash.ltm;
    }
    else
    {
        meshData->Obj2Cam = RwCameraGetCurrentCamera()->viewMatrix;
        RwMatrixSetIdentityMacro(&meshData->Obj2World);
    }

    meshData->SurfaceProperties.ambient = 1.0f;
    meshData->SurfaceProperties.diffuse = 1.0f;
    meshData->SurfaceProperties.specular = 1.0f;
    meshData->Texture = NULL;
    meshData->MatCol = white;
    meshData->Pipeline = NULL;
    meshData->PrimType = rwPRIMTYPENAPRIMTYPE;
    meshData->NumElements = 0;
    meshData->NumVertices = numVerts;
    meshData->ClipFlagsAnd = 0;
    meshData->ClipFlagsOr = 0;
    meshState->numUsed++;

    rsvp = (RxRenderStateVector*)renderState->currentData;
    *rsvp = RXPIPELINEGLOBAL(defaultRenderState);
    rsvp->Flags |= (pool->stash.flags & rwIM3D_ALLOPAQUE) ? 0 : RSV_FLAG_VERTEXALPHA;
    renderState->numUsed++;

    RxPacketDispatch(packet, 0, self);

    return TRUE;
}

RxNodeDefinition* RxNodeDefinitionGetImmInstance(void)
{
    static RxClusterRef N1clofinterest[] = {
        { &RxClObjSpace3DVertices, rxCLALLOWABSENT, 0 },
        { &RxClMeshState, rxCLALLOWABSENT, 0 },
        { &RxClRenderState, rxCLALLOWABSENT, 0 },
    };

    static RxClusterValidityReq N1inputreqs[] = {
        rxCLREQ_DONTWANT,
        rxCLREQ_DONTWANT,
        rxCLREQ_DONTWANT,
    };

    static RxClusterValid N1outcl1[] = {
        rxCLVALID_VALID,
        rxCLVALID_VALID,
        rxCLVALID_VALID,
    };

    static char _ImmInstanceOut[] = "ImmInstanceOut";

    static RxOutputSpec N1outputs[] = {
        { _ImmInstanceOut, N1outcl1, rxCLVALID_NOCHANGE },
    };

    static char _ImmInstance_csl[] = "ImmInstance.csl";

    static RxNodeDefinition nodeImmInstanceCSL = {
        _ImmInstance_csl,
        { ImmInstanceNode, NULL, NULL, NULL, NULL, NULL, NULL },
        { 3, N1clofinterest, N1inputreqs, 1, N1outputs },
        0,
        rxNODEDEFCONST,
        0,
    };

    return &nodeImmInstanceCSL;
}
