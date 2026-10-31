#include <rpworld.h>

// The post-light node: clamps the lit camera-space colors to 0-255, modulates them by the
// material color where the mesh asks for it, and stores them as the screen vertices' colors.

// The node's cluster slots.
#define POSTLIGHTCL_MESHSTATE 0
#define POSTLIGHTCL_CAMVERTS 1
#define POSTLIGHTCL_DEVVERTS 2
#define POSTLIGHTCL_VSTEPS 3

extern RxClusterDefinition RxClVSteps;

// Moves a cluster's cursor on by one element.
#define CLUSTER_NEXT(_cluster)                                                                     \
    ((_cluster)->currentData = (U8*)(_cluster)->currentData + (_cluster)->stride)

// Clamps a color channel to 0-255 by comparing its bits with those of 255.0: a negative value has
// its sign bit set and so compares higher, and becomes zero.
#define POSTLIGHT_CLAMP(_channel)                                                                  \
    if (*(U32*)&(_channel) > *(const U32*)&maxColor)                                               \
    {                                                                                              \
        *(U32*)&(_channel) = *(const U32*)&maxColor & ~(*(S32*)&(_channel) >> 31);                 \
    }

// Moves both cursors over the next run of unlit vertices.
#define POSTLIGHT_SKIP(_count)                                                                     \
    {                                                                                              \
        U32 _n;                                                                                    \
                                                                                                   \
        for (_n = (_count); _n; _n--)                                                              \
        {                                                                                          \
            CLUSTER_NEXT(camVerts);                                                                \
            CLUSTER_NEXT(devVerts);                                                                \
        }                                                                                          \
    }

static S32 _PostLightFn(RxPipelineNode* self, RxPipelineNodeParam* params)
{
    static const F32 maxColor = 255.0f;

    RxPacket* packet = RxPacketFetch(self);
    RxCluster* meshState = RxClusterLockRead(packet, POSTLIGHTCL_MESHSTATE);
    RxMeshStateVector* meshData = (RxMeshStateVector*)meshState->currentData;

    if (!meshData->ClipFlagsAnd)
    {
        U8 noSteps = 1;
        RwRGBA matCol = meshData->MatCol;
        RxCluster* camVerts = RxClusterLockWrite(packet, POSTLIGHTCL_CAMVERTS, self);
        RxCluster* devVerts = RxClusterLockWrite(packet, POSTLIGHTCL_DEVVERTS, self);
        RxCluster* vSteps = RxClusterLockRead(packet, POSTLIGHTCL_VSTEPS);
        U8* step;
        S32 stepInc;
        S32 numVerts;

        if (vSteps && vSteps->numUsed)
        {
            step = (U8*)vSteps->data;
            numVerts = vSteps->numUsed;
            stepInc = 1;
            POSTLIGHT_SKIP(*step);
            step++;
        }
        else
        {
            numVerts = meshData->NumVertices;
            step = &noSteps;
            stepInc = 0;
        }

        if (!(meshData->Flags & rpGEOMETRYMODULATEMATERIALCOLOR) ||
            *(U32*)&matCol == 0xFFFFFFFF)
        {
            while (numVerts--)
            {
                RxCamSpace3DVertex* camVert = (RxCamSpace3DVertex*)camVerts->currentData;
                RwIm2DVertex* devVert = (RwIm2DVertex*)devVerts->currentData;

                POSTLIGHT_CLAMP(camVert->col.red);
                POSTLIGHT_CLAMP(camVert->col.green);
                POSTLIGHT_CLAMP(camVert->col.blue);

                devVert->emissiveColor.red = (U8)camVert->col.red;
                devVert->emissiveColor.green = (U8)camVert->col.green;
                devVert->emissiveColor.blue = (U8)camVert->col.blue;
                devVert->emissiveColor.alpha = (U8)camVert->col.alpha;

                POSTLIGHT_SKIP(*step);
                step += stepInc;
            }
        }
        else
        {
            F32 redScale = (1.0f / 255.0f) * matCol.red;
            F32 greenScale = (1.0f / 255.0f) * matCol.green;
            F32 blueScale = (1.0f / 255.0f) * matCol.blue;
            F32 alphaScale = (1.0f / 255.0f) * matCol.alpha;

            while (numVerts--)
            {
                RxCamSpace3DVertex* camVert = (RxCamSpace3DVertex*)camVerts->currentData;
                RwIm2DVertex* devVert = (RwIm2DVertex*)devVerts->currentData;

                camVert->col.red *= redScale;
                camVert->col.green *= greenScale;
                camVert->col.blue *= blueScale;
                camVert->col.alpha *= alphaScale;

                POSTLIGHT_CLAMP(camVert->col.red);
                POSTLIGHT_CLAMP(camVert->col.green);
                POSTLIGHT_CLAMP(camVert->col.blue);
                POSTLIGHT_CLAMP(camVert->col.alpha);

                devVert->emissiveColor.red = (U8)camVert->col.red;
                devVert->emissiveColor.green = (U8)camVert->col.green;
                devVert->emissiveColor.blue = (U8)camVert->col.blue;
                devVert->emissiveColor.alpha = (U8)camVert->col.alpha;

                POSTLIGHT_SKIP(*step);
                step += stepInc;
            }
        }
    }

    RxPacketDispatch(packet, 0, self);

    return TRUE;
}

RxNodeDefinition* RxNodeDefinitionGetPostLight(void)
{
    static RxClusterRef PostLightCLI[] = {
        { &RxClMeshState, rxCLALLOWABSENT, 0 },
        { &RxClCamSpace3DVertices, rxCLALLOWABSENT, 0 },
        { &RxClScrSpace2DVertices, rxCLALLOWABSENT, 0 },
        { &RxClVSteps, rxCLALLOWABSENT, 0 },
    };

    static RxClusterValidityReq PostLightInputVR[] = {
        rxCLREQ_REQUIRED,
        rxCLREQ_REQUIRED,
        rxCLREQ_REQUIRED,
        rxCLREQ_OPTIONAL,
    };

    static RxClusterValid PostLightOutputV[] = {
        rxCLVALID_NOCHANGE,
        rxCLVALID_VALID,
        rxCLVALID_VALID,
        rxCLVALID_NOCHANGE,
    };

    static char outputName[] = "Output";

    static RxOutputSpec PostLightOutput[] = {
        { outputName, PostLightOutputV, rxCLVALID_NOCHANGE },
    };

    static char _PostLight_csl[] = "PostLight.csl";

    static RxNodeDefinition nodePostLightCSL = {
        _PostLight_csl,
        { _PostLightFn, NULL, NULL, NULL, NULL, NULL, NULL },
        { 4, PostLightCLI, PostLightInputVR, 1, PostLightOutput },
        0,
        rxNODEDEFCONST,
        0,
    };

    return &nodePostLightCSL;
}
