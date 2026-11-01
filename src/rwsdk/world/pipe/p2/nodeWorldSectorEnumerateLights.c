#include <rpworld.h>

// The world sector light enumeration node: fills the packet's lights cluster with the world's
// global lights and the sector's local lights.

extern RxClusterDefinition RxClLights;

// The node's cluster slots.
#define ENUMLIGHTSCL_LIGHTS 0

static RpLight* WorldLightsCB(RpLight* light, void* pData)
{
    RxCluster* lights = (RxCluster*)pData;

    if (rwObjectTestFlags(light, rpLIGHTLIGHTWORLD))
    {
        if (lights->numUsed == lights->numAlloced)
        {
            lights = RxClusterResizeData(lights, lights->numUsed + 8);
        }
        *(RpLight**)((U8*)lights->data + lights->stride * lights->numUsed) = light;
        lights->numUsed++;
    }

    return light;
}

static S32 WorldSectorEnumerateLightsNodeBody(RxPipelineNode* self, RxPipelineNodeParam* params)
{
    RxPacket* packet = RxPacketFetch(self);
    RxCluster* lights;
    RpWorldSector* sector;

    lights = RxClusterLockWrite(packet, ENUMLIGHTSCL_LIGHTS, self);
    sector = (RpWorldSector*)params->dataParam;
    lights = RxClusterInitializeData(lights, 8, sizeof(RpLight*));

    if (((RpWorld*)RWSRCGLOBAL(curWorld))->flags & rpWORLDLIGHT)
    {
        _rpWorldForAllGlobalLights(WorldLightsCB, lights);
        _rpWorldSectorForAllLocalLights(sector, WorldLightsCB, lights);
    }

    RxPacketDispatch(packet, 0, self);

    return TRUE;
}

RxNodeDefinition* RxNodeDefinitionGetWorldSectorEnumerateLights(void)
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

    static char _WorldSectorEnumerateLights_csl[] = "WorldSectorEnumerateLights.csl";

    static RxNodeDefinition nodeWorldSectorEnumerateLightsCSL = {
        _WorldSectorEnumerateLights_csl,
        { WorldSectorEnumerateLightsNodeBody, NULL, NULL, NULL, NULL, NULL, NULL },
        { 1, gNodeClusters, gNodeReqs, 1, gNodeOuts },
        0,
        rxNODEDEFCONST,
        0,
    };

    return &nodeWorldSectorEnumerateLightsCSL;
}
