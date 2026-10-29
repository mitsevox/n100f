#include <rwcore.h>

static S32 ParallelCullProcessPacket(RxPipelineNode* self, RxPacket* packet)
{
    RxCluster* camVerts;
    RxCluster* indices;
    RxCluster* meshState;
    RxMeshStateVector* meshData;
    RxCamSpace3DVertex* v0;
    RxCamSpace3DVertex* v1;
    RxCamSpace3DVertex* v2;
    RwImVertexIndex* src;
    U16 stride;
    U32 i;
    RwSplitBits area;

    camVerts = RxClusterLockRead(packet, 0);
    RxClusterLockRead(packet, 1);
    indices = RxClusterLockWrite(packet, 2, self);
    meshState = RxClusterLockWrite(packet, 3, self);

    meshData = (RxMeshStateVector*)meshState->currentData;
    stride = indices->stride;

    for (i = 0; i < meshData->NumElements; i++)
    {
        v0 = (RxCamSpace3DVertex*)((U8*)camVerts->data
                                   + camVerts->stride * ((RwImVertexIndex*)indices->currentData)[0]);
        v1 = (RxCamSpace3DVertex*)((U8*)camVerts->data
                                   + camVerts->stride * ((RwImVertexIndex*)indices->currentData)[1]);
        v2 = (RxCamSpace3DVertex*)((U8*)camVerts->data
                                   + camVerts->stride * ((RwImVertexIndex*)indices->currentData)[2]);

        area.nReal = (v0->cameraVertex.x - v1->cameraVertex.x) * (v2->cameraVertex.y - v1->cameraVertex.y)
             - (v0->cameraVertex.y - v1->cameraVertex.y) * (v2->cameraVertex.x - v1->cameraVertex.x);

        if (area.nInt < 0)
        {
            src = (RwImVertexIndex*)indices->data + (meshData->NumElements * 3 - 3);

            ((RwImVertexIndex*)indices->currentData)[0] = src[0];
            ((RwImVertexIndex*)indices->currentData)[1] = src[1];
            ((RwImVertexIndex*)indices->currentData)[2] = src[2];
            meshData->NumElements--;
            i--;
        }
        else
        {
            indices->currentData = (U8*)indices->currentData + stride * 3;
        }
    }

    if (meshData->NumElements < indices->numUsed * 3)
    {
        indices->numUsed = meshData->NumElements * 3;
    }

    if (!meshData->NumElements)
    {
        RxPacketDispatch(packet, 1, self);
    }
    else
    {
        RxPacketDispatch(packet, 0, self);
    }

    return TRUE;
}

static S32 PerspectiveCullProcessPacket(RxPipelineNode* self, RxPacket* packet)
{
    RxCluster* camVerts;
    RxCluster* devVerts;
    RxCluster* indices;
    RxCluster* meshState;
    RxMeshStateVector* meshData;
    RwImVertexIndex* src;
    U16 stride;
    U32 i;

    camVerts = RxClusterLockRead(packet, 0);
    devVerts = RxClusterLockRead(packet, 1);
    indices = RxClusterLockWrite(packet, 2, self);
    meshState = RxClusterLockWrite(packet, 3, self);

    meshData = (RxMeshStateVector*)meshState->currentData;
    stride = indices->stride;

    if (!meshData->ClipFlagsOr)
    {
        for (i = 0; i < meshData->NumElements; i++)
        {
            RwIm2DVertex* v0;
            RwIm2DVertex* v1;
            RwIm2DVertex* v2;
            RwSplitBits area;

            v0 = (RwIm2DVertex*)((U8*)devVerts->data
                                 + devVerts->stride * ((RwImVertexIndex*)indices->currentData)[0]);
            v1 = (RwIm2DVertex*)((U8*)devVerts->data
                                 + devVerts->stride * ((RwImVertexIndex*)indices->currentData)[1]);
            v2 = (RwIm2DVertex*)((U8*)devVerts->data
                                 + devVerts->stride * ((RwImVertexIndex*)indices->currentData)[2]);

            area.nReal = (v0->x - v1->x) * (v2->y - v1->y) - (v0->y - v1->y) * (v2->x - v1->x);

            if (area.nInt <= 0)
            {
                src = (RwImVertexIndex*)indices->data + (meshData->NumElements * 3 - 3);
                ((RwImVertexIndex*)indices->currentData)[0] = src[0];
                ((RwImVertexIndex*)indices->currentData)[1] = src[1];
                ((RwImVertexIndex*)indices->currentData)[2] = src[2];
                meshData->NumElements--;
                i--;
            }
            else
            {
                indices->currentData = (U8*)indices->currentData + stride * 3;
            }
        }
    }
    else
    {
        for (i = 0; i < meshData->NumElements; i++)
        {
            RwImVertexIndex* index;
            RxCamSpace3DVertex* v0;
            RxCamSpace3DVertex* v1;
            RxCamSpace3DVertex* v2;

            index = (RwImVertexIndex*)indices->currentData;

            v0 = (RxCamSpace3DVertex*)((U8*)camVerts->data + camVerts->stride * index[0]);
            v1 = (RxCamSpace3DVertex*)((U8*)camVerts->data + camVerts->stride * index[1]);
            v2 = (RxCamSpace3DVertex*)((U8*)camVerts->data + camVerts->stride * index[2]);

            if (!(v0->clipFlags & v1->clipFlags & v2->clipFlags))
            {
                if (v0->clipFlags | v1->clipFlags | v2->clipFlags)
                {
                    RwV3d a;
                    RwV3d b;
                    RwV3d n;
                    RwSplitBits dot;

                    RwV3dSubMacro(&a, &v0->cameraVertex, &v1->cameraVertex);
                    RwV3dSubMacro(&b, &v2->cameraVertex, &v1->cameraVertex);
                    RwV3dCrossProductMacro(&n, &a, &b);
                    dot.nReal = RwV3dDotProductMacro(&n, &v0->cameraVertex);

                    if (dot.nInt <= 0)
                    {
                        src = (RwImVertexIndex*)indices->data + (meshData->NumElements * 3 - 3);
                        ((RwImVertexIndex*)indices->currentData)[0] = src[0];
                        ((RwImVertexIndex*)indices->currentData)[1] = src[1];
                        ((RwImVertexIndex*)indices->currentData)[2] = src[2];
                        meshData->NumElements--;
                        i--;
                    }
                    else
                    {
                        indices->currentData = (U8*)indices->currentData + stride * 3;
                    }
                }
                else
                {
                    RwIm2DVertex* d0;
                    RwIm2DVertex* d1;
                    RwIm2DVertex* d2;
                    RwSplitBits area;

                    d0 = (RwIm2DVertex*)((U8*)devVerts->data + devVerts->stride * index[0]);
                    d1 = (RwIm2DVertex*)((U8*)devVerts->data + devVerts->stride * index[1]);
                    d2 = (RwIm2DVertex*)((U8*)devVerts->data + devVerts->stride * index[2]);

                    area.nReal = (d0->x - d1->x) * (d2->y - d1->y) - (d0->y - d1->y) * (d2->x - d1->x);

                    if (area.nInt <= 0)
                    {
                        src = (RwImVertexIndex*)indices->data + (meshData->NumElements * 3 - 3);
                        ((RwImVertexIndex*)indices->currentData)[0] = src[0];
                        ((RwImVertexIndex*)indices->currentData)[1] = src[1];
                        ((RwImVertexIndex*)indices->currentData)[2] = src[2];
                        meshData->NumElements--;
                        i--;
                    }
                    else
                    {
                        indices->currentData = (U8*)indices->currentData + stride * 3;
                    }
                }
            }
            else
            {
                src = (RwImVertexIndex*)indices->data + (meshData->NumElements * 3 - 3);
                ((RwImVertexIndex*)indices->currentData)[0] = src[0];
                ((RwImVertexIndex*)indices->currentData)[1] = src[1];
                ((RwImVertexIndex*)indices->currentData)[2] = src[2];
                meshData->NumElements--;
                i--;
            }
        }
    }

    if (meshData->NumElements < indices->numUsed * 3)
    {
        indices->numUsed = meshData->NumElements * 3;
    }

    if (!meshData->NumElements)
    {
        RxPacketDispatch(packet, 1, self);
    }
    else
    {
        RxPacketDispatch(packet, 0, self);
    }

    return TRUE;
}

static S32 _CullTriangleNode(RxPipelineNode* self, RxPipelineNodeParam* params)
{
    static S32 (*CullFunc[])(RxPipelineNode*, RxPacket*) = {
        ParallelCullProcessPacket,
        PerspectiveCullProcessPacket,
    };

    S32 (*cullFunc)(RxPipelineNode*, RxPacket*);

    cullFunc = CullFunc[RwCameraGetProjection(RwCameraGetCurrentCamera()) == rwPERSPECTIVE];

    return cullFunc(self, RxPacketFetch(self));
}

RxNodeDefinition* RxNodeDefinitionGetCullTriangle(void)
{
    static RxClusterRef N1clofinterest[] = {
        { &RxClCamSpace3DVertices, rxCLALLOWABSENT, 0 },
        { &RxClScrSpace2DVertices, rxCLALLOWABSENT, 0 },
        { &RxClIndices, rxCLALLOWABSENT, 0 },
        { &RxClMeshState, rxCLALLOWABSENT, 0 },
    };

    static RxClusterValidityReq N1inputreqs[] = {
        rxCLREQ_REQUIRED,
        rxCLREQ_REQUIRED,
        rxCLREQ_REQUIRED,
        rxCLREQ_REQUIRED,
    };

    static RxClusterValid N1outcl1[] = {
        rxCLVALID_NOCHANGE,
        rxCLVALID_NOCHANGE,
        rxCLVALID_VALID,
        rxCLVALID_VALID,
    };

    static RxClusterValid N1outcl2[] = {
        rxCLVALID_NOCHANGE,
        rxCLVALID_NOCHANGE,
        rxCLVALID_INVALID,
        rxCLVALID_VALID,
    };

    static char _CullTriangleOut[] = "CullTriangleOut";

    static char _CullTriangleAllCulled[] = "CullTriangleAllCulled";

    static RxOutputSpec N1outputs[] = {
        { _CullTriangleOut, N1outcl1, rxCLVALID_NOCHANGE },
        { _CullTriangleAllCulled, N1outcl2, rxCLVALID_NOCHANGE },
    };

    static char _CullTriangle_csl[] = "CullTriangle.csl";

    static RxNodeDefinition nodeCullTriangleCSL = {
        _CullTriangle_csl,
        { _CullTriangleNode, NULL, NULL, NULL, NULL, NULL, NULL },
        { 4, N1clofinterest, N1inputreqs, 2, N1outputs },
        0,
        rxNODEDEFCONST,
        0,
    };

    return &nodeCullTriangleCSL;
}
