#include <rwcore.h>

static S32 PerspectiveClipProcessPacket(RxPipelineNode* self, RxPacket* packet)
{
    S32 result;
    RxCluster* camVerts;
    RxCluster* devVerts;
    RxCluster* indices;
    RxCluster* meshState;
    RxCluster* renderState;
    RxCluster* interpolants;
    RxMeshStateVector* meshData;
    U32 stride;
    U32 i;
    RxRenderStateVector* rsvp;
    U32 numTris;

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

    interpolants = RxClusterLockWrite(packet, 5, self);

    meshData = (RxMeshStateVector*)meshState->currentData;
    stride = indices->stride;

    if (interpolants)
    {
        interpolants->stride = 0xC;
        interpolants->numUsed = 0;
    }

    if (meshData->ClipFlagsOr)
    {
        numTris = meshData->NumElements;

        for (i = 0; i < numTris; i++)
        {
            U32 numVerts;
            RwImVertexIndex* tri;
            RwImVertexIndex vertIndices[16];
            S32 start;
            S32 end;
            RwImVertexIndex idx0;
            RwImVertexIndex idx1;
            RwImVertexIndex idx2;
            RwImVertexIndex firstNewVert;

            U8 clipFlagsOr;
            U8 clipFlagsAnd;
            U32 numNewTris;

            numNewTris = 0;

            idx0 = ((RwImVertexIndex*)indices->currentData)[0];
            idx1 = ((RwImVertexIndex*)indices->currentData)[1];
            idx2 = ((RwImVertexIndex*)indices->currentData)[2];
            numVerts = camVerts->numUsed;
            firstNewVert = (RwImVertexIndex)numVerts;

            {
                RxCamSpace3DVertex* v0;
                RxCamSpace3DVertex* v1;
                RxCamSpace3DVertex* v2;

                v0 = (RxCamSpace3DVertex*)((U8*)camVerts->data + camVerts->stride * idx0);
                v1 = (RxCamSpace3DVertex*)((U8*)camVerts->data + camVerts->stride * idx1);
                v2 = (RxCamSpace3DVertex*)((U8*)camVerts->data + camVerts->stride * idx2);
                clipFlagsOr = v0->clipFlags | v1->clipFlags | v2->clipFlags;
                clipFlagsAnd = v0->clipFlags & v1->clipFlags & v2->clipFlags;
            }

            if (clipFlagsOr)
            {
                if (!clipFlagsAnd)
                {
                    RwImVertexIndex first;
                    S32 j;

                    if (indices->numUsed + 24 > indices->numAlloced)
                    {
                        result = (NULL != RxClusterResizeData(indices, indices->numAlloced
                                                                           + (numTris - i + 24)));
                        if (!result)
                        {
                            goto failed;
                        }

                        indices->currentData = (U8*)indices->data + indices->stride * (i * 3);
                    }

                    if (camVerts->numUsed + 12 > camVerts->numAlloced)
                    {
                        result = (NULL != RxClusterResizeData(camVerts, camVerts->numAlloced
                                                                            + (numTris - i + 12)));
                        if (!result)
                        {
                            goto failed;
                        }
                    }

                    if (devVerts->numUsed + 12 > devVerts->numAlloced)
                    {
                        result = (NULL != RxClusterResizeData(devVerts, devVerts->numAlloced
                                                                            + (numTris - i + 12)));
                        if (!result)
                        {
                            goto failed;
                        }
                    }

                    if (interpolants && interpolants->numUsed + 12 > interpolants->numAlloced)
                    {
                        RxCluster* cluster;

                        U32 numElements;

                        numElements = interpolants->numAlloced + (numTris - i + 12);

                        if (!interpolants->numAlloced)
                        {
                            cluster = RxClusterInitializeData(interpolants, numElements, 0xC);
                        }
                        else
                        {
                            cluster = RxClusterResizeData(interpolants, numElements);
                        }

                        if (!cluster)
                        {
                            result = FALSE;
                            goto failed;
                        }

                        interpolants->currentData =
                            (U8*)interpolants->data + interpolants->stride * interpolants->numUsed;
                    }

                    tri = (RwImVertexIndex*)indices->currentData;

                    start = 0;
                    end = 3;
                    vertIndices[0] = idx0;
                    vertIndices[1] = idx1;
                    vertIndices[2] = idx2;

                    if (clipFlagsOr & 0x30)
                    {
                        if ((clipFlagsOr & 0x10)
                            && _rwForAllEdges(camVerts, devVerts, interpolants, vertIndices, &start,
                                              &end, 0x10, PerspClipFuncs[0])
                                   < 3)
                        {
                            goto clipped;
                        }

                        if ((clipFlagsOr & 0x20)
                            && _rwForAllEdges(camVerts, devVerts, interpolants, vertIndices, &start,
                                              &end, 0x20, PerspClipFuncs[1])
                                   < 3)
                        {
                            goto clipped;
                        }

                        j = start;
                        clipFlagsOr = ((RxCamSpace3DVertex*)((U8*)camVerts->data
                                                             + camVerts->stride * vertIndices[j]))
                                          ->clipFlags;
                        j = (j + 1) & 15;

                        do
                        {
                            clipFlagsOr |= ((RxCamSpace3DVertex*)((U8*)camVerts->data
                                                                  + camVerts->stride
                                                                        * vertIndices[j]))
                                               ->clipFlags;
                            j = (j + 1) & 15;
                        } while (j != end);
                    }

                    if (clipFlagsOr & 0xC)
                    {
                        if ((clipFlagsOr & 0x4)
                            && _rwForAllEdges(camVerts, devVerts, interpolants, vertIndices, &start,
                                              &end, 0x4, PerspClipFuncs[2])
                                   < 3)
                        {
                            goto clipped;
                        }

                        if ((clipFlagsOr & 0x8)
                            && _rwForAllEdges(camVerts, devVerts, interpolants, vertIndices, &start,
                                              &end, 0x8, PerspClipFuncs[3])
                                   < 3)
                        {
                            goto clipped;
                        }

                        j = start;
                        clipFlagsOr = ((RxCamSpace3DVertex*)((U8*)camVerts->data
                                                             + camVerts->stride * vertIndices[j]))
                                          ->clipFlags;
                        j = (j + 1) & 15;

                        do
                        {
                            clipFlagsOr |= ((RxCamSpace3DVertex*)((U8*)camVerts->data
                                                                  + camVerts->stride
                                                                        * vertIndices[j]))
                                               ->clipFlags;
                            j = (j + 1) & 15;
                        } while (j != end);
                    }

                    if (clipFlagsOr & 0x3)
                    {
                        if ((clipFlagsOr & 0x1)
                            && _rwForAllEdges(camVerts, devVerts, interpolants, vertIndices, &start,
                                              &end, 0x1, PerspClipFuncs[4])
                                   < 3)
                        {
                            goto clipped;
                        }

                        if ((clipFlagsOr & 0x2)
                            && _rwForAllEdges(camVerts, devVerts, interpolants, vertIndices, &start,
                                              &end, 0x2, PerspClipFuncs[5])
                                   < 3)
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
                                devVert->x = gClipInfo.camOffsetX + recipZ * (gClipInfo.camWidth * x);
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
                                    gClipInfo.camOffsetY + recipZ * (gClipInfo.camHeight * y);
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

                    first = vertIndices[start];

                    if (!(rsvp->ShadeMode & rwSHADEMODEGOURAUD))
                    {
                        while (first != idx2 && first < firstNewVert
)
                        {
                            vertIndices[end] = vertIndices[start];
                            end = (end + 1) & 15;
                            start = (start + 1) & 15;
                            first = vertIndices[start];
                        }

                        if (first != idx2)
                        {
                            ((RwIm2DVertex*)((U8*)devVerts->data + devVerts->stride * first))
                                ->emissiveColor.red =
                                ((RwIm2DVertex*)((U8*)devVerts->data + devVerts->stride * idx2))
                                    ->emissiveColor.red;
                            ((RwIm2DVertex*)((U8*)devVerts->data + devVerts->stride * first))
                                ->emissiveColor.green =
                                ((RwIm2DVertex*)((U8*)devVerts->data + devVerts->stride * idx2))
                                    ->emissiveColor.green;
                            ((RwIm2DVertex*)((U8*)devVerts->data + devVerts->stride * first))
                                ->emissiveColor.blue =
                                ((RwIm2DVertex*)((U8*)devVerts->data + devVerts->stride * idx2))
                                    ->emissiveColor.blue;
                            ((RwIm2DVertex*)((U8*)devVerts->data + devVerts->stride * first))
                                ->emissiveColor.alpha =
                                ((RwIm2DVertex*)((U8*)devVerts->data + devVerts->stride * idx2))
                                    ->emissiveColor.alpha;
                        }
                    }

                    {
                        S32 prev;
                        S32 next;

                        prev = (start + 2) & 15;
                        next = (prev + 1) & 15;

                        numNewTris = 1;
                        tri[0] = vertIndices[(start + 1) & 15];
                        tri[1] = vertIndices[prev];
                        tri[2] = first;

                        indices->currentData =
                            (U8*)indices->data + indices->stride * (indices->numUsed - 3);

                        while (next != end)
                        {
                            indices->currentData = (U8*)indices->currentData + stride * 3;
                            numNewTris++;
                            ((RwImVertexIndex*)indices->currentData)[0] = vertIndices[prev];
                            ((RwImVertexIndex*)indices->currentData)[1] = vertIndices[next];
                            ((RwImVertexIndex*)indices->currentData)[2] = first;
                            prev = next;
                            next = (next + 1) & 15;
                        }
                    }

                    indices->currentData = tri;
                }
            }
            else
            {
                numNewTris = 1;
            }

        clipped:
            if (!numNewTris)
            {
                RwImVertexIndex* src;
                RwImVertexIndex* last;
                U32 numNewVerts;

                src = (RwImVertexIndex*)((U8*)indices->data
                                         + indices->stride * ((numTris - 1) * 3));
                last = (RwImVertexIndex*)((U8*)indices->data
                                          + indices->stride * (indices->numUsed - 3));
                numNewVerts = camVerts->numUsed - numVerts;
                numTris--;
                i--;

                ((RwImVertexIndex*)indices->currentData)[0] = src[0];
                ((RwImVertexIndex*)indices->currentData)[1] = src[1];
                ((RwImVertexIndex*)indices->currentData)[2] = src[2];
                src[0] = last[0];
                src[1] = last[1];
                src[2] = last[2];
                indices->numUsed -= 3;
                camVerts->numUsed = numVerts;
                devVerts->numUsed = numVerts;

                if (interpolants)
                {
                    U16 interpStride;

                    interpStride = interpolants->stride;
                    interpolants->numUsed -= numNewVerts;

                    while (numNewVerts--)
                    {
                        interpolants->currentData = (U8*)interpolants->currentData - interpStride;
                    }

                }
            }
            else
            {
                indices->numUsed += (numNewTris - 1) * 3;
                indices->currentData = (U8*)indices->currentData + stride * 3;
            }

        failed:
            if (!result)
            {
                _rxPacketDestroy(packet);
                return result;
            }
        }

        meshData->NumElements = indices->numUsed / 3;
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
    S32 result;
    RxCluster* camVerts;
    RxCluster* devVerts;
    RxCluster* indices;
    RxCluster* meshState;
    RxCluster* renderState;
    RxCluster* interpolants;
    RxMeshStateVector* meshData;
    U32 stride;
    U32 i;
    RxRenderStateVector* rsvp;
    U32 numTris;

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

    interpolants = RxClusterLockWrite(packet, 5, self);

    meshData = (RxMeshStateVector*)meshState->currentData;
    stride = indices->stride;

    if (interpolants)
    {
        interpolants->stride = 0xC;
        interpolants->numUsed = 0;
    }

    if (meshData->ClipFlagsOr)
    {
        numTris = meshData->NumElements;

        for (i = 0; i < numTris; i++)
        {
            U32 numVerts;
            RwImVertexIndex* tri;
            RwImVertexIndex vertIndices[16];
            S32 start;
            S32 end;
            RwImVertexIndex idx0;
            RwImVertexIndex idx1;
            RwImVertexIndex idx2;
            RwImVertexIndex firstNewVert;

            U8 clipFlagsOr;
            U8 clipFlagsAnd;
            U32 numNewTris;

            numNewTris = 0;

            idx0 = ((RwImVertexIndex*)indices->currentData)[0];
            idx1 = ((RwImVertexIndex*)indices->currentData)[1];
            idx2 = ((RwImVertexIndex*)indices->currentData)[2];
            numVerts = camVerts->numUsed;
            firstNewVert = (RwImVertexIndex)numVerts;

            {
                RxCamSpace3DVertex* v0;
                RxCamSpace3DVertex* v1;
                RxCamSpace3DVertex* v2;

                v0 = (RxCamSpace3DVertex*)((U8*)camVerts->data + camVerts->stride * idx0);
                v1 = (RxCamSpace3DVertex*)((U8*)camVerts->data + camVerts->stride * idx1);
                v2 = (RxCamSpace3DVertex*)((U8*)camVerts->data + camVerts->stride * idx2);
                clipFlagsOr = v0->clipFlags | v1->clipFlags | v2->clipFlags;
                clipFlagsAnd = v0->clipFlags & v1->clipFlags & v2->clipFlags;
            }

            if (clipFlagsOr)
            {
                if (!clipFlagsAnd)
                {
                    RwImVertexIndex first;
                    S32 j;

                    if (indices->numUsed + 24 > indices->numAlloced)
                    {
                        result = (NULL != RxClusterResizeData(indices, indices->numAlloced
                                                                           + (numTris - i + 24)));
                        if (!result)
                        {
                            goto failed;
                        }

                        indices->currentData = (U8*)indices->data + indices->stride * (i * 3);
                    }

                    if (camVerts->numUsed + 12 > camVerts->numAlloced)
                    {
                        result = (NULL != RxClusterResizeData(camVerts, camVerts->numAlloced
                                                                            + (numTris - i + 12)));
                        if (!result)
                        {
                            goto failed;
                        }
                    }

                    if (devVerts->numUsed + 12 > devVerts->numAlloced)
                    {
                        result = (NULL != RxClusterResizeData(devVerts, devVerts->numAlloced
                                                                            + (numTris - i + 12)));
                        if (!result)
                        {
                            goto failed;
                        }
                    }

                    if (interpolants && interpolants->numUsed + 12 > interpolants->numAlloced)
                    {
                        RxCluster* cluster;

                        U32 numElements;

                        numElements = interpolants->numAlloced + (numTris - i + 12);

                        if (!interpolants->numAlloced)
                        {
                            cluster = RxClusterInitializeData(interpolants, numElements, 0xC);
                        }
                        else
                        {
                            cluster = RxClusterResizeData(interpolants, numElements);
                        }

                        if (!cluster)
                        {
                            result = FALSE;
                            goto failed;
                        }

                        interpolants->currentData =
                            (U8*)interpolants->data + interpolants->stride * interpolants->numUsed;
                    }

                    tri = (RwImVertexIndex*)indices->currentData;

                    start = 0;
                    end = 3;
                    vertIndices[0] = idx0;
                    vertIndices[1] = idx1;
                    vertIndices[2] = idx2;

                    if (clipFlagsOr & 0x30)
                    {
                        if ((clipFlagsOr & 0x10)
                            && _rwForAllEdges(camVerts, devVerts, interpolants, vertIndices, &start,
                                              &end, 0x10, ParaClipFuncs[0])
                                   < 3)
                        {
                            goto clipped;
                        }

                        if ((clipFlagsOr & 0x20)
                            && _rwForAllEdges(camVerts, devVerts, interpolants, vertIndices, &start,
                                              &end, 0x20, ParaClipFuncs[1])
                                   < 3)
                        {
                            goto clipped;
                        }

                        j = start;
                        clipFlagsOr = ((RxCamSpace3DVertex*)((U8*)camVerts->data
                                                             + camVerts->stride * vertIndices[j]))
                                          ->clipFlags;
                        j = (j + 1) & 15;

                        do
                        {
                            clipFlagsOr |= ((RxCamSpace3DVertex*)((U8*)camVerts->data
                                                                  + camVerts->stride
                                                                        * vertIndices[j]))
                                               ->clipFlags;
                            j = (j + 1) & 15;
                        } while (j != end);
                    }

                    if (clipFlagsOr & 0xC)
                    {
                        if ((clipFlagsOr & 0x4)
                            && _rwForAllEdges(camVerts, devVerts, interpolants, vertIndices, &start,
                                              &end, 0x4, ParaClipFuncs[2])
                                   < 3)
                        {
                            goto clipped;
                        }

                        if ((clipFlagsOr & 0x8)
                            && _rwForAllEdges(camVerts, devVerts, interpolants, vertIndices, &start,
                                              &end, 0x8, ParaClipFuncs[3])
                                   < 3)
                        {
                            goto clipped;
                        }

                        j = start;
                        clipFlagsOr = ((RxCamSpace3DVertex*)((U8*)camVerts->data
                                                             + camVerts->stride * vertIndices[j]))
                                          ->clipFlags;
                        j = (j + 1) & 15;

                        do
                        {
                            clipFlagsOr |= ((RxCamSpace3DVertex*)((U8*)camVerts->data
                                                                  + camVerts->stride
                                                                        * vertIndices[j]))
                                               ->clipFlags;
                            j = (j + 1) & 15;
                        } while (j != end);
                    }

                    if (clipFlagsOr & 0x3)
                    {
                        if ((clipFlagsOr & 0x1)
                            && _rwForAllEdges(camVerts, devVerts, interpolants, vertIndices, &start,
                                              &end, 0x1, ParaClipFuncs[4])
                                   < 3)
                        {
                            goto clipped;
                        }

                        if ((clipFlagsOr & 0x2)
                            && _rwForAllEdges(camVerts, devVerts, interpolants, vertIndices, &start,
                                              &end, 0x2, ParaClipFuncs[5])
                                   < 3)
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
                                devVert->x = gClipInfo.camOffsetX + gClipInfo.camWidth * x;
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
                                    gClipInfo.camOffsetY + gClipInfo.camHeight * y;
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

                    first = vertIndices[start];

                    if (!(rsvp->ShadeMode & rwSHADEMODEGOURAUD))
                    {
                        while (first != idx2 && first < firstNewVert
)
                        {
                            vertIndices[end] = vertIndices[start];
                            end = (end + 1) & 15;
                            start = (start + 1) & 15;
                            first = vertIndices[start];
                        }

                        if (first != idx2)
                        {
                            ((RwIm2DVertex*)((U8*)devVerts->data + devVerts->stride * first))
                                ->emissiveColor.red =
                                ((RwIm2DVertex*)((U8*)devVerts->data + devVerts->stride * idx2))
                                    ->emissiveColor.red;
                            ((RwIm2DVertex*)((U8*)devVerts->data + devVerts->stride * first))
                                ->emissiveColor.green =
                                ((RwIm2DVertex*)((U8*)devVerts->data + devVerts->stride * idx2))
                                    ->emissiveColor.green;
                            ((RwIm2DVertex*)((U8*)devVerts->data + devVerts->stride * first))
                                ->emissiveColor.blue =
                                ((RwIm2DVertex*)((U8*)devVerts->data + devVerts->stride * idx2))
                                    ->emissiveColor.blue;
                            ((RwIm2DVertex*)((U8*)devVerts->data + devVerts->stride * first))
                                ->emissiveColor.alpha =
                                ((RwIm2DVertex*)((U8*)devVerts->data + devVerts->stride * idx2))
                                    ->emissiveColor.alpha;
                        }
                    }

                    {
                        S32 prev;
                        S32 next;

                        prev = (start + 2) & 15;
                        next = (prev + 1) & 15;

                        numNewTris = 1;
                        tri[0] = vertIndices[(start + 1) & 15];
                        tri[1] = vertIndices[prev];
                        tri[2] = first;

                        indices->currentData =
                            (U8*)indices->data + indices->stride * (indices->numUsed - 3);

                        while (next != end)
                        {
                            indices->currentData = (U8*)indices->currentData + stride * 3;
                            numNewTris++;
                            ((RwImVertexIndex*)indices->currentData)[0] = vertIndices[prev];
                            ((RwImVertexIndex*)indices->currentData)[1] = vertIndices[next];
                            ((RwImVertexIndex*)indices->currentData)[2] = first;
                            prev = next;
                            next = (next + 1) & 15;
                        }
                    }

                    indices->currentData = tri;
                }
            }
            else
            {
                numNewTris = 1;
            }

        clipped:
            if (!numNewTris)
            {
                RwImVertexIndex* src;
                RwImVertexIndex* last;
                U32 numNewVerts;

                src = (RwImVertexIndex*)((U8*)indices->data
                                         + indices->stride * ((numTris - 1) * 3));
                last = (RwImVertexIndex*)((U8*)indices->data
                                          + indices->stride * (indices->numUsed - 3));
                numNewVerts = camVerts->numUsed - numVerts;
                numTris--;
                i--;

                ((RwImVertexIndex*)indices->currentData)[0] = src[0];
                ((RwImVertexIndex*)indices->currentData)[1] = src[1];
                ((RwImVertexIndex*)indices->currentData)[2] = src[2];
                src[0] = last[0];
                src[1] = last[1];
                src[2] = last[2];
                indices->numUsed -= 3;
                camVerts->numUsed = numVerts;
                devVerts->numUsed = numVerts;

                if (interpolants)
                {
                    U16 interpStride;

                    interpStride = interpolants->stride;
                    interpolants->numUsed -= numNewVerts;

                    while (numNewVerts--)
                    {
                        interpolants->currentData = (U8*)interpolants->currentData - interpStride;
                    }

                }
            }
            else
            {
                indices->numUsed += (numNewTris - 1) * 3;
                indices->currentData = (U8*)indices->currentData + stride * 3;
            }

        failed:
            if (!result)
            {
                _rxPacketDestroy(packet);
                return result;
            }
        }

        meshData->NumElements = indices->numUsed / 3;
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

static S32 TriangleClipNode(RxPipelineNode* self, RxPipelineNodeParam* params)
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

RxNodeDefinition* RxNodeDefinitionGetClipTriangle(void)
{
    static RxClusterRef N5clofinterest[] = {
        { &RxClCamSpace3DVertices, rxCLALLOWABSENT, 0 },
        { &RxClScrSpace2DVertices, rxCLALLOWABSENT, 0 },
        { &RxClIndices, rxCLALLOWABSENT, 0 },
        { &RxClMeshState, rxCLALLOWABSENT, 0 },
        { &RxClRenderState, rxCLALLOWABSENT, 0 },
        { &RxClInterpolants, rxCLALLOWABSENT, 0 },
    };

    static RxClusterValidityReq N5inputreqs[] = {
        rxCLREQ_REQUIRED, rxCLREQ_REQUIRED, rxCLREQ_REQUIRED, rxCLREQ_REQUIRED, rxCLREQ_OPTIONAL,
        rxCLREQ_OPTIONAL,
    };

    static RxClusterValid N5outcl1[] = {
        rxCLVALID_VALID, rxCLVALID_VALID, rxCLVALID_VALID, rxCLVALID_VALID, rxCLVALID_NOCHANGE,
        rxCLVALID_VALID,
    };

    static RxClusterValid N5outcl2[] = {
        rxCLVALID_VALID, rxCLVALID_VALID, rxCLVALID_INVALID, rxCLVALID_VALID, rxCLVALID_NOCHANGE,
        rxCLVALID_INVALID,
    };

    static char _ClipOut[] = "ClipOut";

    static char _ClipAllClipped[] = "ClipAllClipped";

    static RxOutputSpec N5outputs[] = {
        { _ClipOut, N5outcl1, rxCLVALID_NOCHANGE },
        { _ClipAllClipped, N5outcl2, rxCLVALID_NOCHANGE },
    };

    static char _ClipTriangle_csl[] = "ClipTriangle.csl";

    static RxNodeDefinition nodeClipTriangleCSL = {
        _ClipTriangle_csl,
        { TriangleClipNode, NULL, NULL, NULL, NULL, NULL, NULL },
        { 6, N5clofinterest, N5inputreqs, 2, N5outputs },
        0,
        rxNODEDEFCONST,
        0,
    };

    return &nodeClipTriangleCSL;
}
