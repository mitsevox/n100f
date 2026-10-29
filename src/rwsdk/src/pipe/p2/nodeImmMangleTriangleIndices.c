#include <rwcore.h>

static S32 _ImmMangleTriangleIndicesNode(RxPipelineNode* self, RxPipelineNodeParam* params)
{
    RxPacket* packet;
    RxCluster* meshState;
    RxCluster* indices;
    RxMeshStateVector* meshData;
    RwImVertexIndex* index;
    RwImVertexIndex* src;
    RwImVertexIndex first;
    U32 numIndices;
    U32 numTris;
    U32 odd;
    U32 i;
    void* data;

    packet = RxPacketFetch(self);

    meshState = RxClusterLockWrite(packet, 0, self);
    meshData = (RxMeshStateVector*)meshState->currentData;

    indices = RxClusterLockRead(packet, 1);

    if (!indices->numUsed)
    {
        indices = RxClusterLockWrite(packet, 1, self);

        switch (meshData->PrimType)
        {
        case rwPRIMTYPETRILIST:
            numIndices = meshData->NumElements * 3;
            indices = RxClusterInitializeData(indices, numIndices, sizeof(RwImVertexIndex));
            index = (RwImVertexIndex*)indices->currentData;

            for (i = 0; i < numIndices; i++)
            {
                *index++ = i;
            }

            indices->numUsed = numIndices;
            break;
        case rwPRIMTYPETRIFAN:
            numIndices = meshData->NumElements * 3;
            indices = RxClusterInitializeData(indices, numIndices, sizeof(RwImVertexIndex));
            index = (RwImVertexIndex*)indices->currentData;

            for (i = 0; i < meshData->NumElements; i++)
            {
                index[0] = 0;
                index[1] = i + 1;
                index[2] = i + 2;
                index += 3;
            }

            indices->numUsed = numIndices;
            meshData->PrimType = rwPRIMTYPETRILIST;
            break;
        case rwPRIMTYPETRISTRIP:
            numIndices = meshData->NumElements * 3;
            indices = RxClusterInitializeData(indices, numIndices, sizeof(RwImVertexIndex));
            index = (RwImVertexIndex*)indices->currentData;

            for (i = 0; i < meshData->NumElements; i++)
            {
                odd = i & 1;
                index[0] = i + odd;
                index[1] = i + (odd ^ 1);
                index[2] = i + 2;
                index += 3;
            }

            indices->numUsed = numIndices;
            meshData->PrimType = rwPRIMTYPETRILIST;
            break;
        default:
            RxPacketDispatch(packet, 0, self);

            return FALSE;
        }
    }
    else
    {
        switch (meshData->PrimType)
        {
        case rwPRIMTYPETRILIST:
            break;
        case rwPRIMTYPETRIFAN:
            numTris = indices->numUsed - 2;
            first = *(RwImVertexIndex*)indices->currentData;
            src = (RwImVertexIndex*)((U8*)indices->data + indices->stride * numTris);

            data = RxHeapAlloc(params->heap, sizeof(RwImVertexIndex) * 3 * numTris);
            numIndices = numTris * 3;
            indices = RxClusterSetData(indices, data, sizeof(RwImVertexIndex), numIndices);
            index = (RwImVertexIndex*)((U8*)indices->data + indices->stride * ((numTris - 1) * 3));

            for (i = 0; i < numTris; i++)
            {
                index[2] = src[1];
                index[1] = src[0];
                index[0] = first;
                src--;
                index -= 3;
            }

            indices->numUsed = numIndices;
            meshData->PrimType = rwPRIMTYPETRILIST;
            break;
        case rwPRIMTYPETRISTRIP:
            src = (RwImVertexIndex*)((U8*)indices->data + indices->stride * (indices->numUsed - 3));
            numTris = indices->numUsed - 2;

            data = RxHeapAlloc(params->heap, sizeof(RwImVertexIndex) * 3 * numTris);
            numIndices = numTris * 3;
            indices = RxClusterSetData(indices, data, sizeof(RwImVertexIndex), numIndices);
            index = (RwImVertexIndex*)((U8*)indices->data + indices->stride * ((numTris - 1) * 3));

            odd = numTris & 1;

            for (i = 0; i < numTris; i++)
            {
                index[0] = src[odd ^ 1];
                index[1] = src[odd];
                index[2] = src[2];
                odd ^= 1;
                src--;
                index -= 3;
            }

            indices->numUsed = numIndices;
            meshData->PrimType = rwPRIMTYPETRILIST;
            break;
        default:
            RxPacketDispatch(packet, 0, self);

            return FALSE;
        }
    }

    RxPacketDispatch(packet, 0, self);

    return TRUE;
}

RxNodeDefinition* RxNodeDefinitionGetImmMangleTriangleIndices(void)
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

    static char _ImmMangleTriangleIndices_csl[] = "ImmMangleTriangleIndices.csl";

    static RxNodeDefinition nodeImmMangleTriangleIndicesCSL = {
        _ImmMangleTriangleIndices_csl,
        { _ImmMangleTriangleIndicesNode, NULL, NULL, NULL, NULL, NULL, NULL },
        { 2, N1clofinterest, N1inputreqs, 1, N1outputs },
        0,
        rxNODEDEFCONST,
        0,
    };

    return &nodeImmMangleTriangleIndicesCSL;
}
