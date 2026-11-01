#include <rpworld.h>

// The atomic light enumeration node: fills the packet's lights cluster with the world's global
// lights and the local lights of every sector the atomic is in, each light once.

extern RxClusterDefinition RxClLights;

// The node's cluster slots.
#define ENUMLIGHTSCL_LIGHTS 0

// Appends a light to a lights cluster, growing it by eight entries when it is full.
#define LIGHTS_ADD(_cluster, _light)                                                               \
    {                                                                                              \
        if ((_cluster)->numUsed == (_cluster)->numAlloced)                                         \
        {                                                                                          \
            (_cluster) = RxClusterResizeData((_cluster), (_cluster)->numUsed + 8);                 \
        }                                                                                          \
        *(RpLight**)((U8*)(_cluster)->data + (_cluster)->stride * (_cluster)->numUsed) = (_light); \
        (_cluster)->numUsed++;                                                                     \
    }

static RpLight* AtomicGlobalLightsCB(RpLight* light, void* pData)
{
    RxCluster* lights = (RxCluster*)pData;

    if (rwObjectTestFlags(light, rpLIGHTLIGHTATOMICS))
    {
        LIGHTS_ADD(lights, light);
    }

    return light;
}

static RpLight* AtomicLocalLightsCB(RpLight* light, void* pData)
{
    RxCluster* lights = (RxCluster*)pData;

    if (light->lightFrame != RWSRCGLOBAL(lightFrame))
    {
        if (rwObjectTestFlags(light, rpLIGHTLIGHTATOMICS))
        {
            LIGHTS_ADD(lights, light);
        }

        light->lightFrame = RWSRCGLOBAL(lightFrame);
    }

    return light;
}

static S32 AtomicEnumerateLightsNodeBody(RxPipelineNode* self, RxPipelineNodeParam* params)
{
    RpAtomic* atomic = (RpAtomic*)params->dataParam;
    RxPacket* packet = RxPacketFetch(self);
    RxCluster* lights;

    lights = RxClusterLockWrite(packet, ENUMLIGHTSCL_LIGHTS, self);
    lights = RxClusterInitializeData(lights, 8, sizeof(RpLight*));

    if ((atomic->geometry->flags & rpGEOMETRYLIGHT) && RWSRCGLOBAL(curWorld))
    {
        RwLLLink* cur;
        RwLLLink* end;

        RWSRCGLOBAL(lightFrame)++;
        _rpWorldForAllGlobalLights(AtomicGlobalLightsCB, lights);

        cur = rwLinkListGetFirstLLLink(&atomic->llWorldSectorsInAtomic);
        end = rwLinkListGetTerminator(&atomic->llWorldSectorsInAtomic);
        while (cur != end)
        {
            WorldTie* tie = rwLLLinkGetData(cur, WorldTie, atomicLink);

            _rpWorldSectorForAllLocalLights(tie->sector, AtomicLocalLightsCB, lights);
            cur = rwLLLinkGetNext(cur);
        }
    }

    RxPacketDispatch(packet, 0, self);

    return TRUE;
}

RxNodeDefinition* RxNodeDefinitionGetAtomicEnumerateLights(void)
{
    static RxClusterRef gNodeClusters[] = {
        { &RxClLights, rxCLALLOWABSENT, 0 },
    };

    static RxClusterValidityReq gNodeReqs[] = {
        rxCLREQ_DONTWANT,
    };

    static char _DefaultOutput[] = "DefaultOutput";

    static RxClusterValid gNodeOut1[] = {
        rxCLVALID_VALID,
    };

    static RxOutputSpec gNodeOuts[] = {
        { _DefaultOutput, gNodeOut1, rxCLVALID_NOCHANGE },
    };

    static char _AtomicEnumerateLights_csl[] = "AtomicEnumerateLights.csl";

    static RxNodeDefinition nodeAtomicEnumerateLightsCSL = {
        _AtomicEnumerateLights_csl,
        { AtomicEnumerateLightsNodeBody, NULL, NULL, NULL, NULL, NULL, NULL },
        { 1, gNodeClusters, gNodeReqs, 1, gNodeOuts },
        0,
        rxNODEDEFCONST,
        0,
    };

    return &nodeAtomicEnumerateLightsCSL;
}
