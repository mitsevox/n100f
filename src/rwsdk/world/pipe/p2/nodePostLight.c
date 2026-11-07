#include <rpworld.h>

// The post-light node: scales the lit camera-space colors by the material color where the mesh
// asks for it, clamps them to 0-255 (alpha only when scaled) and stores them as the screen
// vertices' colors.

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
    if (((RwSplitBits*)&(_channel))->nUInt > *(const U32*)&maxColor)                                 \
    {                                                                                              \
        U32 _bits = *(const U32*)&maxColor & ~(((RwSplitBits*)&(_channel))->nInt >> 31);           \
                                                                                                   \
        ((RwSplitBits*)&(_channel))->nUInt = _bits;                                                \
    }

// Moves both cursors on by count vertices.
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

// The node body: writes each lit vertex's clamped color to its screen vertex, skipping the
// vertices that the optional step cluster passes over. No color is written when all the
// vertices share a clip flag (the mesh lies wholly off screen); the packet is still passed on.
static S32 _PostLightFn(RxPipelineNode* self, RxPipelineNodeParam* params)
{
    static const F32 maxColor = 255.0f;

    RxPacket* packet;
    RxCluster* meshState = RxClusterLockRead(packet = RxPacketFetch(self), POSTLIGHTCL_MESHSTATE);
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
                RwRGBA* outputColor = &devVert->emissiveColor;
                RwRGBAReal* color = &camVert->col;

                POSTLIGHT_CLAMP(color->red);
                POSTLIGHT_CLAMP(color->green);
                POSTLIGHT_CLAMP(color->blue);

                outputColor->red = (U8)color->red;
                outputColor->green = (U8)color->green;
                outputColor->blue = (U8)color->blue;
                outputColor->alpha = (U8)color->alpha;

                {
                    U32 n = *step;
                    step += stepInc;
                    POSTLIGHT_SKIP(n);
                }
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
                RwRGBA* outputColor = &devVert->emissiveColor;
                RwRGBAReal* color = &camVert->col;

                color->red *= redScale;
                color->green *= greenScale;
                color->blue *= blueScale;
                color->alpha *= alphaScale;

                POSTLIGHT_CLAMP(color->red);
                POSTLIGHT_CLAMP(color->green);
                POSTLIGHT_CLAMP(color->blue);
                POSTLIGHT_CLAMP(color->alpha);

                outputColor->red = (U8)color->red;
                outputColor->green = (U8)color->green;
                outputColor->blue = (U8)color->blue;
                outputColor->alpha = (U8)color->alpha;

                {
                    U32 n = *step;
                    step += stepInc;
                    POSTLIGHT_SKIP(n);
                }
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
