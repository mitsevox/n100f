#include <rwcore.h>

static S32 PerspectiveClipProcessPacket(RxPipelineNode* self, RxPacket* packet)
{
    RxCluster* camVerts;
    RxCluster* devVerts;
    RxCluster* indices;
    RxCluster* meshState;
    RxCluster* renderState;
    RxMeshStateVector* meshData;
    U32 stride;
    U32 i;
    RxRenderStateVector* rsvp;
    S32 result;
    U32 numLines;

    static const RwClipVertexFunc PerspClipFuncs[] = {
        _rwGeneratePerspClippedVertexZLO, _rwGeneratePerspClippedVertexZHI,
        _rwGeneratePerspClippedVertexYLO, _rwGeneratePerspClippedVertexYHI,
        _rwGeneratePerspClippedVertexXLO, _rwGeneratePerspClippedVertexXHI,
    };

    result = TRUE;

    camVerts = RxClusterLockWrite(packet, 0, self);
    devVerts = RxClusterLockWrite(packet, 1, self);
    indices = RxClusterLockWrite(packet, 2, self);
    meshState = RxClusterLockWrite(packet, 3, self);
    renderState = RxClusterLockRead(packet, 4);

    if (renderState && renderState->data)
    {
        rsvp = (RxRenderStateVector*)renderState->currentData;
    }
    else
    {
        rsvp = &RXPIPELINEGLOBAL(defaultRenderState);
    }

    meshData = (RxMeshStateVector*)meshState->currentData;
    stride = indices->stride;

    if (meshData->ClipFlagsOr)
    {
        numLines = meshData->NumElements;

        for (i = 0; i < numLines; i++)
        {
            RwImVertexIndex* line;
            RwImVertexIndex vertIndices[16];
            S32 start;
            S32 end;
            RwImVertexIndex idx0;
            RwImVertexIndex idx1;
            U32 numVerts;
            U8 clipFlagsAnd;
            U32 keep;
            U8 clipFlagsOr;

            keep = FALSE;

            idx0 = ((RwImVertexIndex*)indices->currentData)[0];
            idx1 = ((RwImVertexIndex*)indices->currentData)[1];
            numVerts = camVerts->numUsed;

            {
                RxCamSpace3DVertex* v0;
                RxCamSpace3DVertex* v1;

                v0 = (RxCamSpace3DVertex*)((U8*)camVerts->data + camVerts->stride * idx0);
                v1 = (RxCamSpace3DVertex*)((U8*)camVerts->data + camVerts->stride * idx1);
                clipFlagsOr = v0->clipFlags | v1->clipFlags;
                clipFlagsAnd = v0->clipFlags & v1->clipFlags;
            }

            if (clipFlagsOr)
            {
                if (!clipFlagsAnd)
                {
                    S32 j;

                    if (camVerts->numUsed + 12 > camVerts->numAlloced)
                    {
                        result = (NULL != RxClusterResizeData(camVerts, camVerts->numAlloced
                                                                            + (numLines - i + 12)));
                        if (!result)
                        {
                            goto failed;
                        }
                    }

                    if (devVerts->numUsed + 12 > devVerts->numAlloced)
                    {
                        result = (NULL != RxClusterResizeData(devVerts, devVerts->numAlloced
                                                                            + (numLines - i + 12)));
                        if (!result)
                        {
                            goto failed;
                        }
                    }

                    line = (RwImVertexIndex*)indices->currentData;

                    start = 0;
                    end = 2;
                    vertIndices[0] = idx0;
                    vertIndices[1] = idx1;

                    if (clipFlagsOr & 0x30)
                    {
                        if ((clipFlagsOr & 0x10)
                            && _rwForOneEdge(camVerts, devVerts, vertIndices, &start, &end, 0x10,
                                             PerspClipFuncs[0])
                                   < 2)
                        {
                            goto clipped;
                        }

                        if ((clipFlagsOr & 0x20)
                            && _rwForOneEdge(camVerts, devVerts, vertIndices, &start, &end, 0x20,
                                             PerspClipFuncs[1])
                                   < 2)
                        {
                            goto clipped;
                        }

                        clipFlagsOr =
                            ((RxCamSpace3DVertex*)((U8*)camVerts->data
                                                   + camVerts->stride * vertIndices[start]))
                                ->clipFlags
                            | ((RxCamSpace3DVertex*)((U8*)camVerts->data
                                                     + camVerts->stride
                                                           * vertIndices[(start + 1) & 15]))
                                  ->clipFlags;
                    }

                    if (clipFlagsOr & 0xC)
                    {
                        if ((clipFlagsOr & 0x4)
                            && _rwForOneEdge(camVerts, devVerts, vertIndices, &start, &end, 0x4,
                                             PerspClipFuncs[2])
                                   < 2)
                        {
                            goto clipped;
                        }

                        if ((clipFlagsOr & 0x8)
                            && _rwForOneEdge(camVerts, devVerts, vertIndices, &start, &end, 0x8,
                                             PerspClipFuncs[3])
                                   < 2)
                        {
                            goto clipped;
                        }

                        clipFlagsOr =
                            ((RxCamSpace3DVertex*)((U8*)camVerts->data
                                                   + camVerts->stride * vertIndices[start]))
                                ->clipFlags
                            | ((RxCamSpace3DVertex*)((U8*)camVerts->data
                                                     + camVerts->stride
                                                           * vertIndices[(start + 1) & 15]))
                                  ->clipFlags;
                    }

                    if (clipFlagsOr & 0x3)
                    {
                        if ((clipFlagsOr & 0x1)
                            && _rwForOneEdge(camVerts, devVerts, vertIndices, &start, &end, 0x1,
                                             PerspClipFuncs[4])
                                   < 2)
                        {
                            goto clipped;
                        }

                        if ((clipFlagsOr & 0x2)
                            && _rwForOneEdge(camVerts, devVerts, vertIndices, &start, &end, 0x2,
                                             PerspClipFuncs[5])
                                   < 2)
                        {
                            goto clipped;
                        }
                    }

                    j = start;

                    do
                    {
                        RxCamSpace3DVertex* camVert;
                        U8 vertClipFlags;

                        camVert = (RxCamSpace3DVertex*)((U8*)camVerts->data
                                                        + camVerts->stride * vertIndices[j]);
                        vertClipFlags = camVert->clipFlags;

                        if (vertClipFlags & 0x3F)
                        {
                            RwIm2DVertex* devVert;
                            F32 recipZ;
                            F32 x;
                            F32 y;

                            recipZ = 1.0f / camVert->cameraVertex.z;
                            x = camVert->cameraVertex.x;
                            y = camVert->cameraVertex.y;
                            devVert = (RwIm2DVertex*)((U8*)devVerts->data
                                                      + devVerts->stride * vertIndices[j]);

                            switch (vertClipFlags & 0x3)
                            {
                            case 0x2:
                                devVert->x = gClipInfo.camWidth + gClipInfo.camOffsetX;
                                break;
                            case 0x1:
                                devVert->x = gClipInfo.camOffsetX;
                                break;
                            default:
                                devVert->x =
                                    gClipInfo.camOffsetX
                                    + recipZ * (gClipInfo.camWidth * x);
                                break;
                            }

                            switch (vertClipFlags & 0xF)
                            {
                            case 0x8:
                                devVert->y = gClipInfo.camHeight + gClipInfo.camOffsetY;
                                break;
                            case 0x4:
                                devVert->y = gClipInfo.camOffsetY;
                                break;
                            default:
                                devVert->y =
                                    gClipInfo.camOffsetY
                                    + recipZ * (gClipInfo.camHeight * y);
                                break;
                            }

                            switch (vertClipFlags & 0x3F)
                            {
                            case 0x20:
                                devVert->z = gClipInfo.farScreenZ;
                                break;
                            case 0x10:
                                devVert->z = gClipInfo.nearScreenZ;
                                break;
                            default:
                                devVert->z = gClipInfo.zShift + gClipInfo.zScale * recipZ;
                                break;
                            }

                            devVert->u = camVert->u;
                            devVert->v = camVert->v;
                        }

                        j = (j + 1) & 15;
                    } while (j != end);

                    if (!(rsvp->ShadeMode & rwSHADEMODEGOURAUD))
                    {
                        while (vertIndices[start] != idx0)
                        {
                            vertIndices[end] = vertIndices[start];
                            end = (end + 1) & 15;
                            start = (start + 1) & 15;
                        }

                        if (vertIndices[start] != idx0)
                        {
                            ((RwIm2DVertex*)((U8*)devVerts->data
                                             + devVerts->stride * vertIndices[start]))
                                ->emissiveColor.red =
                                ((RwIm2DVertex*)((U8*)devVerts->data + devVerts->stride * idx0))
                                    ->emissiveColor.red;
                            ((RwIm2DVertex*)((U8*)devVerts->data
                                             + devVerts->stride * vertIndices[start]))
                                ->emissiveColor.green =
                                ((RwIm2DVertex*)((U8*)devVerts->data + devVerts->stride * idx0))
                                    ->emissiveColor.green;
                            ((RwIm2DVertex*)((U8*)devVerts->data
                                             + devVerts->stride * vertIndices[start]))
                                ->emissiveColor.blue =
                                ((RwIm2DVertex*)((U8*)devVerts->data + devVerts->stride * idx0))
                                    ->emissiveColor.blue;
                            ((RwIm2DVertex*)((U8*)devVerts->data
                                             + devVerts->stride * vertIndices[start]))
                                ->emissiveColor.alpha =
                                ((RwIm2DVertex*)((U8*)devVerts->data + devVerts->stride * idx0))
                                    ->emissiveColor.alpha;
                        }
                    }

                    keep = TRUE;
                    line[0] = vertIndices[start];
                    line[1] = vertIndices[(start + 1) & 15];
                }
            }
            else
            {
                keep = TRUE;
            }

        clipped:
            if (!keep)
            {
                RwImVertexIndex* src;

                src = (RwImVertexIndex*)((U8*)indices->data
                                         + indices->stride * ((numLines - 1) * 2));
                numLines--;
                i--;

                ((RwImVertexIndex*)indices->currentData)[0] = src[0];
                ((RwImVertexIndex*)indices->currentData)[1] = src[1];
                indices->numUsed -= 2;
                camVerts->numUsed = numVerts;
                devVerts->numUsed = numVerts;
            }
            else
            {
                indices->currentData = (U8*)indices->currentData + stride * 2;
            }

        failed:
            if (!result)
            {
                _rxPacketDestroy(packet);
                return result;
            }
        }

        meshData->NumElements = indices->numUsed / 2;
        meshData->NumVertices = camVerts->numUsed;
    }

    if (!meshData->NumElements)
    {
        RxPacketDispatch(packet, 1, self);
    }
    else
    {
        RxPacketDispatch(packet, 0, self);
    }

    return result;
}

static S32 ParallelClipProcessPacket(RxPipelineNode* self, RxPacket* packet)
{
    RxCluster* camVerts;
    RxCluster* devVerts;
    RxCluster* indices;
    RxCluster* meshState;
    RxCluster* renderState;
    RxMeshStateVector* meshData;
    U32 stride;
    U32 i;
    RxRenderStateVector* rsvp;
    S32 result;
    U32 numLines;

    static const RwClipVertexFunc ParaClipFuncs[] = {
        _rwGenerateParallelZLOClippedVertex, _rwGenerateParallelZHIClippedVertex,
        _rwGenerateParallelYLOClippedVertex, _rwGenerateParallelYHIClippedVertex,
        _rwGenerateParallelXLOClippedVertex, _rwGenerateParallelXHIClippedVertex,
    };

    result = TRUE;

    camVerts = RxClusterLockWrite(packet, 0, self);
    devVerts = RxClusterLockWrite(packet, 1, self);
    indices = RxClusterLockWrite(packet, 2, self);
    meshState = RxClusterLockWrite(packet, 3, self);
    renderState = RxClusterLockRead(packet, 4);

    if (renderState && renderState->data)
    {
        rsvp = (RxRenderStateVector*)renderState->currentData;
    }
    else
    {
        rsvp = &RXPIPELINEGLOBAL(defaultRenderState);
    }

    meshData = (RxMeshStateVector*)meshState->currentData;
    stride = indices->stride;

    if (meshData->ClipFlagsOr)
    {
        numLines = meshData->NumElements;

        for (i = 0; i < numLines; i++)
        {
            RwImVertexIndex* line;
            RwImVertexIndex vertIndices[16];
            S32 start;
            S32 end;
            RwImVertexIndex idx0;
            RwImVertexIndex idx1;
            U32 numVerts;
            U8 clipFlagsAnd;
            U32 keep;
            U8 clipFlagsOr;

            keep = FALSE;

            idx0 = ((RwImVertexIndex*)indices->currentData)[0];
            idx1 = ((RwImVertexIndex*)indices->currentData)[1];
            numVerts = camVerts->numUsed;

            {
                RxCamSpace3DVertex* v0;
                RxCamSpace3DVertex* v1;

                v0 = (RxCamSpace3DVertex*)((U8*)camVerts->data + camVerts->stride * idx0);
                v1 = (RxCamSpace3DVertex*)((U8*)camVerts->data + camVerts->stride * idx1);
                clipFlagsOr = v0->clipFlags | v1->clipFlags;
                clipFlagsAnd = v0->clipFlags & v1->clipFlags;
            }

            if (clipFlagsOr)
            {
                if (!clipFlagsAnd)
                {
                    S32 j;

                    if (camVerts->numUsed + 12 > camVerts->numAlloced)
                    {
                        result = (NULL != RxClusterResizeData(camVerts, camVerts->numAlloced
                                                                            + (numLines - i + 12)));
                        if (!result)
                        {
                            goto failed;
                        }
                    }

                    if (devVerts->numUsed + 12 > devVerts->numAlloced)
                    {
                        result = (NULL != RxClusterResizeData(devVerts, devVerts->numAlloced
                                                                            + (numLines - i + 12)));
                        if (!result)
                        {
                            goto failed;
                        }
                    }

                    line = (RwImVertexIndex*)indices->currentData;

                    start = 0;
                    end = 2;
                    vertIndices[0] = idx0;
                    vertIndices[1] = idx1;

                    if (clipFlagsOr & 0x30)
                    {
                        if ((clipFlagsOr & 0x10)
                            && _rwForOneEdge(camVerts, devVerts, vertIndices, &start, &end, 0x10,
                                             ParaClipFuncs[0])
                                   < 2)
                        {
                            goto clipped;
                        }

                        if ((clipFlagsOr & 0x20)
                            && _rwForOneEdge(camVerts, devVerts, vertIndices, &start, &end, 0x20,
                                             ParaClipFuncs[1])
                                   < 2)
                        {
                            goto clipped;
                        }

                        clipFlagsOr =
                            ((RxCamSpace3DVertex*)((U8*)camVerts->data
                                                   + camVerts->stride * vertIndices[start]))
                                ->clipFlags
                            | ((RxCamSpace3DVertex*)((U8*)camVerts->data
                                                     + camVerts->stride
                                                           * vertIndices[(start + 1) & 15]))
                                  ->clipFlags;
                    }

                    if (clipFlagsOr & 0xC)
                    {
                        if ((clipFlagsOr & 0x4)
                            && _rwForOneEdge(camVerts, devVerts, vertIndices, &start, &end, 0x4,
                                             ParaClipFuncs[2])
                                   < 2)
                        {
                            goto clipped;
                        }

                        if ((clipFlagsOr & 0x8)
                            && _rwForOneEdge(camVerts, devVerts, vertIndices, &start, &end, 0x8,
                                             ParaClipFuncs[3])
                                   < 2)
                        {
                            goto clipped;
                        }

                        clipFlagsOr =
                            ((RxCamSpace3DVertex*)((U8*)camVerts->data
                                                   + camVerts->stride * vertIndices[start]))
                                ->clipFlags
                            | ((RxCamSpace3DVertex*)((U8*)camVerts->data
                                                     + camVerts->stride
                                                           * vertIndices[(start + 1) & 15]))
                                  ->clipFlags;
                    }

                    if (clipFlagsOr & 0x3)
                    {
                        if ((clipFlagsOr & 0x1)
                            && _rwForOneEdge(camVerts, devVerts, vertIndices, &start, &end, 0x1,
                                             ParaClipFuncs[4])
                                   < 2)
                        {
                            goto clipped;
                        }

                        if ((clipFlagsOr & 0x2)
                            && _rwForOneEdge(camVerts, devVerts, vertIndices, &start, &end, 0x2,
                                             ParaClipFuncs[5])
                                   < 2)
                        {
                            goto clipped;
                        }
                    }

                    j = start;

                    do
                    {
                        RxCamSpace3DVertex* camVert;
                        U8 vertClipFlags;

                        camVert = (RxCamSpace3DVertex*)((U8*)camVerts->data
                                                        + camVerts->stride * vertIndices[j]);
                        vertClipFlags = camVert->clipFlags;

                        if (vertClipFlags & 0x3F)
                        {
                            RwIm2DVertex* devVert;
                            F32 z;
                            F32 x;
                            F32 y;

                            z = camVert->cameraVertex.z;
                            x = camVert->cameraVertex.x;
                            y = camVert->cameraVertex.y;
                            devVert = (RwIm2DVertex*)((U8*)devVerts->data
                                                      + devVerts->stride * vertIndices[j]);

                            switch (vertClipFlags & 0x3)
                            {
                            case 0x2:
                                devVert->x = gClipInfo.camWidth + gClipInfo.camOffsetX;
                                break;
                            case 0x1:
                                devVert->x = gClipInfo.camOffsetX;
                                break;
                            default:
                                devVert->x =
                                    gClipInfo.camOffsetX
                                    + gClipInfo.camWidth * x;
                                break;
                            }

                            switch (vertClipFlags & 0xF)
                            {
                            case 0x8:
                                devVert->y = gClipInfo.camHeight + gClipInfo.camOffsetY;
                                break;
                            case 0x4:
                                devVert->y = gClipInfo.camOffsetY;
                                break;
                            default:
                                devVert->y =
                                    gClipInfo.camOffsetY
                                    + gClipInfo.camHeight * y;
                                break;
                            }

                            switch (vertClipFlags & 0x3F)
                            {
                            case 0x20:
                                devVert->z = gClipInfo.farScreenZ;
                                break;
                            case 0x10:
                                devVert->z = gClipInfo.nearScreenZ;
                                break;
                            default:
                                devVert->z = gClipInfo.zShift + gClipInfo.zScale * z;
                                break;
                            }

                            devVert->u = camVert->u;
                            devVert->v = camVert->v;
                        }

                        j = (j + 1) & 15;
                    } while (j != end);

                    if (!(rsvp->ShadeMode & rwSHADEMODEGOURAUD))
                    {
                        while (vertIndices[start] != idx0)
                        {
                            vertIndices[end] = vertIndices[start];
                            end = (end + 1) & 15;
                            start = (start + 1) & 15;
                        }

                        if (vertIndices[start] != idx0)
                        {
                            ((RwIm2DVertex*)((U8*)devVerts->data
                                             + devVerts->stride * vertIndices[start]))
                                ->emissiveColor.red =
                                ((RwIm2DVertex*)((U8*)devVerts->data + devVerts->stride * idx0))
                                    ->emissiveColor.red;
                            ((RwIm2DVertex*)((U8*)devVerts->data
                                             + devVerts->stride * vertIndices[start]))
                                ->emissiveColor.green =
                                ((RwIm2DVertex*)((U8*)devVerts->data + devVerts->stride * idx0))
                                    ->emissiveColor.green;
                            ((RwIm2DVertex*)((U8*)devVerts->data
                                             + devVerts->stride * vertIndices[start]))
                                ->emissiveColor.blue =
                                ((RwIm2DVertex*)((U8*)devVerts->data + devVerts->stride * idx0))
                                    ->emissiveColor.blue;
                            ((RwIm2DVertex*)((U8*)devVerts->data
                                             + devVerts->stride * vertIndices[start]))
                                ->emissiveColor.alpha =
                                ((RwIm2DVertex*)((U8*)devVerts->data + devVerts->stride * idx0))
                                    ->emissiveColor.alpha;
                        }
                    }

                    keep = TRUE;
                    line[0] = vertIndices[start];
                    line[1] = vertIndices[(start + 1) & 15];
                }
            }
            else
            {
                keep = TRUE;
            }

        clipped:
            if (!keep)
            {
                RwImVertexIndex* src;

                src = (RwImVertexIndex*)((U8*)indices->data
                                         + indices->stride * ((numLines - 1) * 2));
                numLines--;
                i--;

                ((RwImVertexIndex*)indices->currentData)[0] = src[0];
                ((RwImVertexIndex*)indices->currentData)[1] = src[1];
                indices->numUsed -= 2;
                camVerts->numUsed = numVerts;
                devVerts->numUsed = numVerts;
            }
            else
            {
                indices->currentData = (U8*)indices->currentData + stride * 2;
            }

        failed:
            if (!result)
            {
                _rxPacketDestroy(packet);
                return result;
            }
        }

        meshData->NumElements = indices->numUsed / 2;
        meshData->NumVertices = camVerts->numUsed;
    }

    if (!meshData->NumElements)
    {
        RxPacketDispatch(packet, 1, self);
    }
    else
    {
        RxPacketDispatch(packet, 0, self);
    }

    return result;
}

static S32 LineClipNode(RxPipelineNode* self, RxPipelineNodeParam* params)
{
    static S32 (*ClipFunc[])(RxPipelineNode*, RxPacket*) = {
        ParallelClipProcessPacket,
        PerspectiveClipProcessPacket,
    };

    RwCamera* camera;
    RwRaster* raster;
    S32 (*clipFunc)(RxPipelineNode*, RxPacket*);

    camera = RwCameraGetCurrentCamera();
    raster = camera->frameBuffer;
    clipFunc = ClipFunc[RwCameraGetProjection(camera) == rwPERSPECTIVE];

    gClipInfo.zScale = camera->zScale;
    gClipInfo.zShift = camera->zShift;
    gClipInfo.camWidth = raster->width;
    gClipInfo.camHeight = raster->height;
    gClipInfo.camOffsetX = raster->nOffsetX;
    gClipInfo.camOffsetY = raster->nOffsetY;
    gClipInfo.nearClip = camera->nearPlane;
    gClipInfo.farClip = camera->farPlane;
    gClipInfo.nearScreenZ = RwIm2DGetNearScreenZ();
    gClipInfo.farScreenZ = RwIm2DGetFarScreenZ();

    return clipFunc(self, RxPacketFetch(self));
}

RxNodeDefinition* RxNodeDefinitionGetClipLine(void)
{
    static RxClusterRef N5clofinterest[] = {
        { &RxClCamSpace3DVertices, rxCLALLOWABSENT, 0 },
        { &RxClScrSpace2DVertices, rxCLALLOWABSENT, 0 },
        { &RxClIndices, rxCLALLOWABSENT, 0 },
        { &RxClMeshState, rxCLALLOWABSENT, 0 },
        { &RxClRenderState, rxCLALLOWABSENT, 0 },
    };

    static RxClusterValidityReq N5inputreqs[] = {
        rxCLREQ_REQUIRED, rxCLREQ_REQUIRED, rxCLREQ_REQUIRED, rxCLREQ_REQUIRED, rxCLREQ_OPTIONAL,
    };

    static RxClusterValid N5outcl1[] = {
        rxCLVALID_VALID, rxCLVALID_VALID, rxCLVALID_VALID, rxCLVALID_VALID, rxCLVALID_NOCHANGE,
    };

    static RxClusterValid N5outcl2[] = {
        rxCLVALID_VALID, rxCLVALID_VALID, rxCLVALID_INVALID, rxCLVALID_VALID, rxCLVALID_NOCHANGE,
    };

    static char _ClipOut[] = "ClipOut";

    static char _ClipAllClipped[] = "ClipAllClipped";

    static RxOutputSpec N5outputs[] = {
        { _ClipOut, N5outcl1, rxCLVALID_NOCHANGE },
        { _ClipAllClipped, N5outcl2, rxCLVALID_NOCHANGE },
    };

    static char _ClipLine_csl[] = "ClipLine.csl";

    static RxNodeDefinition nodeClipLineCSL = {
        _ClipLine_csl,
        { LineClipNode, NULL, NULL, NULL, NULL, NULL, NULL },
        { 5, N5clofinterest, N5inputreqs, 2, N5outputs },
        0,
        rxNODEDEFCONST,
        0,
    };

    return &nodeClipLineCSL;
}
