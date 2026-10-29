#include <rwcore.h>

static S32 _ImmMangleLineIndicesNode(RxPipelineNode* self, RxPipelineNodeParam* params)
{
    RxPacket* packet;
    RxCluster* meshState;
    RxCluster* indices;
    RxMeshStateVector* meshData;
    RwImVertexIndex* index;
    RwImVertexIndex* src;
    U32 numIndices;
    U32 numLines;
    void* data;
    U32 i;

    packet = RxPacketFetch(self);

    meshState = RxClusterLockWrite(packet, 0, self);
    meshData = (RxMeshStateVector*)meshState->currentData;

    indices = RxClusterLockRead(packet, 1);

    if (!indices->numUsed)
    {
        indices = RxClusterLockWrite(packet, 1, self);

        if (meshData->PrimType == rwPRIMTYPELINELIST)
        {
            numIndices = meshData->NumElements * 2;
            indices = RxClusterInitializeData(indices, numIndices, sizeof(RwImVertexIndex));
            index = (RwImVertexIndex*)indices->currentData;

            for (i = 0; i < numIndices; i++)
            {
                *index++ = i;
            }

            indices->numUsed = numIndices;
        }
        else if (meshData->PrimType == rwPRIMTYPEPOLYLINE)
        {
            numIndices = meshData->NumElements * 2;
            indices = RxClusterInitializeData(indices, numIndices, sizeof(RwImVertexIndex));
            index = (RwImVertexIndex*)indices->currentData;

            for (i = 0; i < meshData->NumElements; i++)
            {
                index[0] = i;
                index[1] = i + 1;
                index += 2;
            }

            indices->numUsed = numIndices;
            meshData->PrimType = rwPRIMTYPELINELIST;
        }
        else
        {
            RxPacketDispatch(packet, 0, self);

            return FALSE;
        }
    }
    else if (meshData->PrimType != rwPRIMTYPELINELIST)
    {
        if (meshData->PrimType == rwPRIMTYPEPOLYLINE)
        {
            numLines = indices->numUsed - 1;
            src = (RwImVertexIndex*)((U8*)indices->data + indices->stride * (numLines - 1));

            data = RxHeapAlloc(params->heap, numLines * 2 * sizeof(RwImVertexIndex));
            indices = RxClusterSetData(indices, data, sizeof(RwImVertexIndex), numLines * 2);
            index = (RwImVertexIndex*)((U8*)indices->data + indices->stride * ((numLines - 1) * 2));

            for (i = 0; i < numLines; i++)
            {
                index[1] = src[1];
                index[0] = src[0];
                src--;
                index -= 2;
            }

            indices->numUsed = numLines * 2;
            meshData->PrimType = rwPRIMTYPELINELIST;
        }
        else
        {
            RxPacketDispatch(packet, 0, self);

            return FALSE;
        }
    }

    RxPacketDispatch(packet, 0, self);

    return TRUE;
}

RxNodeDefinition* RxNodeDefinitionGetImmMangleLineIndices(void)
{
    static RxClusterRef N1clofinterest[] = {
        { &RxClMeshState, rxCLALLOWABSENT, 0 },
        { &RxClIndices, rxCLALLOWABSENT, 0 },
    };

    static RxClusterValidityReq N1inputreqs[] = {
        rxCLREQ_REQUIRED,
        rxCLREQ_OPTIONAL,
    };

    static RxClusterValid N1outValid[] = {
        rxCLVALID_VALID,
        rxCLVALID_VALID,
    };

    static char _ImmRenderSetupOut[] = "ImmRenderSetupOut";

    static RxOutputSpec N1outputs[] = {
        { _ImmRenderSetupOut, N1outValid, rxCLVALID_NOCHANGE },
    };

    static char _ImmMangleLineIndices_csl[] = "ImmMangleLineIndices.csl";

    static RxNodeDefinition nodeImmMangleLineIndicesCSL = {
        _ImmMangleLineIndices_csl,
        { _ImmMangleLineIndicesNode, NULL, NULL, NULL, NULL, NULL, NULL },
        { 2, N1clofinterest, N1inputreqs, 1, N1outputs },
        0,
        rxNODEDEFCONST,
        0,
    };

    return &nodeImmMangleLineIndicesCSL;
}
