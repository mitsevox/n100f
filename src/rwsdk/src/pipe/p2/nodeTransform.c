#include <rwcore.h>

static S32 TransformNodeParallel(RxPipelineNode* self, RwCamera* camera)
{
    RwRaster* raster;
    RxPacket* packet;
    RxCluster* objVerts;
    RxCluster* camVerts;
    RxCluster* devVerts;
    RxCluster* meshState;
    S32 colored;
    S32 prelit;
    S32 textured;
    RxMeshStateVector* meshData;
    RwRGBA color;
    U16 objStride;
    U16 camStride;
    U16 devStride;
    S32 clipFlagsOr;
    S32 clipFlagsAnd;
    U32 i;

    raster = camera->frameBuffer;
    gClipInfo.camOffsetX = raster->nOffsetX;
    gClipInfo.camOffsetY = raster->nOffsetY;
    gClipInfo.camWidth = raster->width;
    gClipInfo.camHeight = raster->height;
    gClipInfo.farClip = camera->farPlane;
    gClipInfo.nearClip = camera->nearPlane;
    gClipInfo.zScale = camera->zScale;
    gClipInfo.zShift = camera->zShift;

    packet = RxPacketFetch(self);

    colored = FALSE;
    prelit = FALSE;
    textured = FALSE;

    objVerts = RxClusterLockRead(packet, 0);
    camVerts = RxClusterLockWrite(packet, 1, self);
    devVerts = RxClusterLockWrite(packet, 2, self);
    meshState = RxClusterLockWrite(packet, 3, self);

    meshData = (RxMeshStateVector*)meshState->currentData;

    if (!meshData->NumVertices)
    {
        return TRUE;
    }

    camVerts = RxClusterInitializeData(camVerts, meshData->NumVertices, sizeof(RxCamSpace3DVertex));
    devVerts = RxClusterInitializeData(devVerts, meshData->NumVertices, sizeof(RwIm2DVertex));

    if (meshData->Flags & 0x100)
    {
        colored = TRUE;
    }
    else if (meshData->Flags & 0x8)
    {
        prelit = TRUE;
    }
    else
    {
        color = meshData->MatCol;
    }

    if (meshData->Flags & 0x4)
    {
        textured = TRUE;
    }

    objStride = objVerts->stride;
    camStride = camVerts->stride;
    devStride = devVerts->stride;
    clipFlagsOr = 0;
    clipFlagsAnd = -1;

    for (i = meshData->NumVertices; i; i--)
    {
        RwMatrix* mat = &meshData->Obj2Cam;
        F32 x;
        F32 y;
        F32 z;
        U8 clipFlags;
        U8 zClip;
        U8 yClip;
        U8 xClip;

        x = ((RxObjSpace3DVertex*)objVerts->currentData)->x * mat->right.x
          + ((RxObjSpace3DVertex*)objVerts->currentData)->y * mat->up.x
          + ((RxObjSpace3DVertex*)objVerts->currentData)->z * mat->at.x + mat->pos.x;
        y = ((RxObjSpace3DVertex*)objVerts->currentData)->x * mat->right.y
          + ((RxObjSpace3DVertex*)objVerts->currentData)->y * mat->up.y
          + ((RxObjSpace3DVertex*)objVerts->currentData)->z * mat->at.y + mat->pos.y;
        z = ((RxObjSpace3DVertex*)objVerts->currentData)->x * mat->right.z
          + ((RxObjSpace3DVertex*)objVerts->currentData)->y * mat->up.z
          + ((RxObjSpace3DVertex*)objVerts->currentData)->z * mat->at.z + mat->pos.z;

        ((RxCamSpace3DVertex*)camVerts->currentData)->cameraVertex.x = x;
        ((RxCamSpace3DVertex*)camVerts->currentData)->cameraVertex.y = y;
        ((RxCamSpace3DVertex*)camVerts->currentData)->cameraVertex.z = z;

        if (prelit)
        {
            color.red = ((RxObjSpace3DVertex*)objVerts->currentData)->r;
            color.green = ((RxObjSpace3DVertex*)objVerts->currentData)->g;
            color.blue = ((RxObjSpace3DVertex*)objVerts->currentData)->b;
            color.alpha = ((RxObjSpace3DVertex*)objVerts->currentData)->a;
            ((RxCamSpace3DVertex*)camVerts->currentData)->col.red = color.red;
            ((RxCamSpace3DVertex*)camVerts->currentData)->col.green = color.green;
            ((RxCamSpace3DVertex*)camVerts->currentData)->col.blue = color.blue;
            ((RxCamSpace3DVertex*)camVerts->currentData)->col.alpha = color.alpha;
        }
        else if (colored)
        {
            color.red = ((RxObjSpace3DVertex*)objVerts->currentData)->r;
            color.green = ((RxObjSpace3DVertex*)objVerts->currentData)->g;
            color.blue = ((RxObjSpace3DVertex*)objVerts->currentData)->b;
            color.alpha = ((RxObjSpace3DVertex*)objVerts->currentData)->a;
            ((RxCamSpace3DVertex*)camVerts->currentData)->col.red = color.red;
            ((RxCamSpace3DVertex*)camVerts->currentData)->col.green = color.green;
            ((RxCamSpace3DVertex*)camVerts->currentData)->col.blue = color.blue;
            ((RxCamSpace3DVertex*)camVerts->currentData)->col.alpha = color.alpha;
        }
        else
        {
            ((RxCamSpace3DVertex*)camVerts->currentData)->col.red = 0.0f;
            ((RxCamSpace3DVertex*)camVerts->currentData)->col.green = 0.0f;
            ((RxCamSpace3DVertex*)camVerts->currentData)->col.blue = 0.0f;
            ((RxCamSpace3DVertex*)camVerts->currentData)->col.alpha = 255.0f;
        }

        ((RwIm2DVertex*)devVerts->currentData)->emissiveColor.red = color.red;
        ((RwIm2DVertex*)devVerts->currentData)->emissiveColor.green = color.green;
        ((RwIm2DVertex*)devVerts->currentData)->emissiveColor.blue = color.blue;
        ((RwIm2DVertex*)devVerts->currentData)->emissiveColor.alpha = color.alpha;

        if (z < gClipInfo.nearClip)
        {
            zClip = 0x10;
        }
        else if (z > gClipInfo.farClip)
        {
            zClip = 0x20;
        }
        else
        {
            zClip = 0;
        }

        if (y < 0.0f)
        {
            yClip = 0x4;
        }
        else if (y > 1.0f)
        {
            yClip = 0x8;
        }
        else
        {
            yClip = 0;
        }

        if (x < 0.0f)
        {
            xClip = 0x1;
        }
        else if (x > 1.0f)
        {
            xClip = 0x2;
        }
        else
        {
            xClip = 0;
        }

        ((RxCamSpace3DVertex*)camVerts->currentData)->clipFlags = zClip | yClip | xClip;
        clipFlags = ((RxCamSpace3DVertex*)camVerts->currentData)->clipFlags;
        clipFlagsOr |= clipFlags;
        clipFlagsAnd &= clipFlags;

        if (!clipFlags)
        {
            ((RwIm2DVertex*)devVerts->currentData)->x = gClipInfo.camOffsetX + x * gClipInfo.camWidth;
            ((RwIm2DVertex*)devVerts->currentData)->y = gClipInfo.camOffsetY + y * gClipInfo.camHeight;
            ((RwIm2DVertex*)devVerts->currentData)->z = gClipInfo.zShift + z * gClipInfo.zScale;

            if (textured)
            {
                F32 u = ((RxObjSpace3DVertex*)objVerts->currentData)->u;
                F32 v = ((RxObjSpace3DVertex*)objVerts->currentData)->v;

                ((RxCamSpace3DVertex*)camVerts->currentData)->u = u;
                ((RxCamSpace3DVertex*)camVerts->currentData)->v = v;
                ((RwIm2DVertex*)devVerts->currentData)->u = u;
                ((RwIm2DVertex*)devVerts->currentData)->v = v;
            }
        }
        else
        {
            if (textured)
            {
                F32 u = ((RxObjSpace3DVertex*)objVerts->currentData)->u;
                F32 v = ((RxObjSpace3DVertex*)objVerts->currentData)->v;

                ((RxCamSpace3DVertex*)camVerts->currentData)->u = u;
                ((RxCamSpace3DVertex*)camVerts->currentData)->v = v;
            }

            ((RwIm2DVertex*)devVerts->currentData)->z = 0.0f;
        }

        objVerts->currentData = (U8*)objVerts->currentData + objStride;
        camVerts->currentData = (U8*)camVerts->currentData + camStride;
        devVerts->currentData = (U8*)devVerts->currentData + devStride;
    }

    devVerts->numUsed = meshData->NumVertices;
    camVerts->numUsed = meshData->NumVertices;
    meshData->ClipFlagsOr = clipFlagsOr;
    meshData->ClipFlagsAnd = clipFlagsAnd;

    if (clipFlagsAnd)
    {
        RxPacketDispatch(packet, 1, self);
    }
    else
    {
        RxPacketDispatch(packet, 0, self);
    }

    return TRUE;
}

static S32 TransformNodePerspective(RxPipelineNode* self, RwCamera* camera)
{
    RwRaster* raster;
    RxPacket* packet;
    RxCluster* objVerts;
    RxCluster* camVerts;
    RxCluster* devVerts;
    RxCluster* meshState;
    S32 colored;
    S32 prelit;
    S32 textured;
    RxMeshStateVector* meshData;
    RwRGBA color;
    U16 objStride;
    U16 camStride;
    U16 devStride;
    S32 clipFlagsOr;
    S32 clipFlagsAnd;
    U32 i;

    raster = camera->frameBuffer;
    gClipInfo.camOffsetX = raster->nOffsetX;
    gClipInfo.camOffsetY = raster->nOffsetY;
    gClipInfo.camWidth = raster->width;
    gClipInfo.camHeight = raster->height;
    gClipInfo.farClip = camera->farPlane;
    gClipInfo.nearClip = camera->nearPlane;
    gClipInfo.zScale = camera->zScale;
    gClipInfo.zShift = camera->zShift;

    packet = RxPacketFetch(self);

    colored = FALSE;
    prelit = FALSE;
    textured = FALSE;

    objVerts = RxClusterLockRead(packet, 0);
    camVerts = RxClusterLockWrite(packet, 1, self);
    devVerts = RxClusterLockWrite(packet, 2, self);
    meshState = RxClusterLockWrite(packet, 3, self);

    meshData = (RxMeshStateVector*)meshState->currentData;

    if (!meshData->NumVertices)
    {
        return TRUE;
    }

    camVerts = RxClusterInitializeData(camVerts, meshData->NumVertices, sizeof(RxCamSpace3DVertex));
    devVerts = RxClusterInitializeData(devVerts, meshData->NumVertices, sizeof(RwIm2DVertex));

    if (meshData->Flags & 0x100)
    {
        colored = TRUE;
    }
    else if (meshData->Flags & 0x8)
    {
        prelit = TRUE;
    }
    else
    {
        color = meshData->MatCol;
    }

    if (meshData->Flags & 0x4)
    {
        textured = TRUE;
    }

    objStride = objVerts->stride;
    camStride = camVerts->stride;
    devStride = devVerts->stride;
    clipFlagsOr = 0;
    clipFlagsAnd = -1;

    for (i = meshData->NumVertices; i; i--)
    {
        RwMatrix* mat = &meshData->Obj2Cam;
        F32 x;
        F32 y;
        F32 z;
        U8 clipFlags;
        U8 zClip;
        U8 yClip;
        U8 xClip;

        x = ((RxObjSpace3DVertex*)objVerts->currentData)->x * mat->right.x
          + ((RxObjSpace3DVertex*)objVerts->currentData)->y * mat->up.x
          + ((RxObjSpace3DVertex*)objVerts->currentData)->z * mat->at.x + mat->pos.x;
        y = ((RxObjSpace3DVertex*)objVerts->currentData)->x * mat->right.y
          + ((RxObjSpace3DVertex*)objVerts->currentData)->y * mat->up.y
          + ((RxObjSpace3DVertex*)objVerts->currentData)->z * mat->at.y + mat->pos.y;
        z = ((RxObjSpace3DVertex*)objVerts->currentData)->x * mat->right.z
          + ((RxObjSpace3DVertex*)objVerts->currentData)->y * mat->up.z
          + ((RxObjSpace3DVertex*)objVerts->currentData)->z * mat->at.z + mat->pos.z;

        ((RxCamSpace3DVertex*)camVerts->currentData)->cameraVertex.x = x;
        ((RxCamSpace3DVertex*)camVerts->currentData)->cameraVertex.y = y;
        ((RxCamSpace3DVertex*)camVerts->currentData)->cameraVertex.z = z;

        if (prelit)
        {
            color.red = ((RxObjSpace3DVertex*)objVerts->currentData)->r;
            color.green = ((RxObjSpace3DVertex*)objVerts->currentData)->g;
            color.blue = ((RxObjSpace3DVertex*)objVerts->currentData)->b;
            color.alpha = ((RxObjSpace3DVertex*)objVerts->currentData)->a;
            ((RxCamSpace3DVertex*)camVerts->currentData)->col.red = color.red;
            ((RxCamSpace3DVertex*)camVerts->currentData)->col.green = color.green;
            ((RxCamSpace3DVertex*)camVerts->currentData)->col.blue = color.blue;
            ((RxCamSpace3DVertex*)camVerts->currentData)->col.alpha = color.alpha;
        }
        else if (colored)
        {
            color.red = ((RxObjSpace3DVertex*)objVerts->currentData)->r;
            color.green = ((RxObjSpace3DVertex*)objVerts->currentData)->g;
            color.blue = ((RxObjSpace3DVertex*)objVerts->currentData)->b;
            color.alpha = ((RxObjSpace3DVertex*)objVerts->currentData)->a;
            ((RxCamSpace3DVertex*)camVerts->currentData)->col.red = color.red;
            ((RxCamSpace3DVertex*)camVerts->currentData)->col.green = color.green;
            ((RxCamSpace3DVertex*)camVerts->currentData)->col.blue = color.blue;
            ((RxCamSpace3DVertex*)camVerts->currentData)->col.alpha = color.alpha;
        }
        else
        {
            ((RxCamSpace3DVertex*)camVerts->currentData)->col.red = 0.0f;
            ((RxCamSpace3DVertex*)camVerts->currentData)->col.green = 0.0f;
            ((RxCamSpace3DVertex*)camVerts->currentData)->col.blue = 0.0f;
            ((RxCamSpace3DVertex*)camVerts->currentData)->col.alpha = 255.0f;
        }

        ((RwIm2DVertex*)devVerts->currentData)->emissiveColor.red = color.red;
        ((RwIm2DVertex*)devVerts->currentData)->emissiveColor.green = color.green;
        ((RwIm2DVertex*)devVerts->currentData)->emissiveColor.blue = color.blue;
        ((RwIm2DVertex*)devVerts->currentData)->emissiveColor.alpha = color.alpha;

        if (z < gClipInfo.nearClip)
        {
            zClip = 0x10;
        }
        else if (z > gClipInfo.farClip)
        {
            zClip = 0x20;
        }
        else
        {
            zClip = 0;
        }

        if (y < 0.0f)
        {
            yClip = 0x4;
        }
        else if (y > z)
        {
            yClip = 0x8;
        }
        else
        {
            yClip = 0;
        }

        if (x < 0.0f)
        {
            xClip = 0x1;
        }
        else if (x > z)
        {
            xClip = 0x2;
        }
        else
        {
            xClip = 0;
        }

        ((RxCamSpace3DVertex*)camVerts->currentData)->clipFlags = zClip | yClip | xClip;
        clipFlags = ((RxCamSpace3DVertex*)camVerts->currentData)->clipFlags;
        clipFlagsOr |= clipFlags;
        clipFlagsAnd &= clipFlags;

        if (!clipFlags)
        {
            F32 recipZ = 1.0f / z;

            ((RwIm2DVertex*)devVerts->currentData)->x =
                gClipInfo.camOffsetX + gClipInfo.camWidth * (x * recipZ);
            ((RwIm2DVertex*)devVerts->currentData)->y =
                gClipInfo.camOffsetY + gClipInfo.camHeight * (y * recipZ);
            ((RwIm2DVertex*)devVerts->currentData)->z = gClipInfo.zShift + recipZ * gClipInfo.zScale;

            if (textured)
            {
                F32 u = ((RxObjSpace3DVertex*)objVerts->currentData)->u;
                F32 v = ((RxObjSpace3DVertex*)objVerts->currentData)->v;

                ((RxCamSpace3DVertex*)camVerts->currentData)->u = u;
                ((RxCamSpace3DVertex*)camVerts->currentData)->v = v;
                ((RwIm2DVertex*)devVerts->currentData)->u = u;
                ((RwIm2DVertex*)devVerts->currentData)->v = v;
            }
        }
        else
        {
            if (textured)
            {
                F32 u = ((RxObjSpace3DVertex*)objVerts->currentData)->u;
                F32 v = ((RxObjSpace3DVertex*)objVerts->currentData)->v;

                ((RxCamSpace3DVertex*)camVerts->currentData)->u = u;
                ((RxCamSpace3DVertex*)camVerts->currentData)->v = v;
            }

            ((RwIm2DVertex*)devVerts->currentData)->z = 0.0f;
        }

        objVerts->currentData = (U8*)objVerts->currentData + objStride;
        camVerts->currentData = (U8*)camVerts->currentData + camStride;
        devVerts->currentData = (U8*)devVerts->currentData + devStride;
    }

    devVerts->numUsed = meshData->NumVertices;
    camVerts->numUsed = meshData->NumVertices;
    meshData->ClipFlagsOr = clipFlagsOr;
    meshData->ClipFlagsAnd = clipFlagsAnd;

    if (clipFlagsAnd)
    {
        RxPacketDispatch(packet, 1, self);
    }
    else
    {
        RxPacketDispatch(packet, 0, self);
    }

    return TRUE;
}

static S32 TransformNode(RxPipelineNode* self, RxPipelineNodeParam* params)
{
    if (RwCameraGetProjection(RwCameraGetCurrentCamera()) != rwPERSPECTIVE)
    {
        return TransformNodeParallel(self, RwCameraGetCurrentCamera());
    }

    return TransformNodePerspective(self, RwCameraGetCurrentCamera());
}

RxNodeDefinition* RxNodeDefinitionGetTransform(void)
{
    static RxClusterRef N3clofinterest[] = {
        { &RxClObjSpace3DVertices, rxCLALLOWABSENT, 0 },
        { &RxClCamSpace3DVertices, rxCLALLOWABSENT, 0 },
        { &RxClScrSpace2DVertices, rxCLALLOWABSENT, 0 },
        { &RxClMeshState, rxCLALLOWABSENT, 0 },
    };

    static RxClusterValidityReq N3inputreqs[] = {
        rxCLREQ_REQUIRED,
        rxCLREQ_DONTWANT,
        rxCLREQ_DONTWANT,
        rxCLREQ_REQUIRED,
    };

    static RxClusterValid N3outcl1[] = {
        rxCLVALID_NOCHANGE,
        rxCLVALID_VALID,
        rxCLVALID_VALID,
        rxCLVALID_VALID,
    };

    static RxClusterValid N3outcl2[] = {
        rxCLVALID_NOCHANGE,
        rxCLVALID_VALID,
        rxCLVALID_VALID,
        rxCLVALID_VALID,
    };

    static char _TransformOut[] = "TransformOut";

    static char _TransformOut2[] = "TransformOut2";

    static RxOutputSpec N3outputs[] = {
        { _TransformOut, N3outcl1, rxCLVALID_NOCHANGE },
        { _TransformOut2, N3outcl2, rxCLVALID_NOCHANGE },
    };

    static char _Transform_csl[] = "Transform.csl";

    static RxNodeDefinition nodeTransformCSL = {
        _Transform_csl,
        { TransformNode, NULL, NULL, NULL, NULL, NULL, NULL },
        { 4, N3clofinterest, N3inputreqs, 2, N3outputs },
        0,
        rxNODEDEFCONST,
        0,
    };

    return &nodeTransformCSL;
}
