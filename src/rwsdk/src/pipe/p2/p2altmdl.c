#include <rwcore.h>

// The context of the pipeline being executed, which RxPipelineExecute sets up and the nodes read.
RxExecContext _rxExecCtxGlobal;

// Embedded packets: each pipeline owns one packet, which its nodes pass along to each other and
// which one pipeline can hand its clusters on to the next.

void _rxPacketDestroy(RxPacket* packet);

// Hands the clusters the second pipeline requires from the first pipeline's embedded packet over
// to the second's, destroys the first packet and makes the second one pending.
void _rxEmbeddedPacketBetweenPipelines(RxPipeline* fromPipeline, RxPipeline* toPipeline)
{
    if (fromPipeline->embeddedPacketState > rxPKST_UNUSED)
    {
        RxPacket* fromPacket = fromPipeline->embeddedPacket;
        RxPacket* toPacket = toPipeline->embeddedPacket;
        U32 i;

        for (i = 0; i < toPipeline->numInputRequirements; i++)
        {
            RxPipelineRequiresCluster* requirement = &toPipeline->inputRequirements[i];
            U32 j;
            U32 numClusters = fromPacket->numClusters;

            for (j = 0; j < numClusters; j++)
            {
                RxPipelineCluster* clusterRef = fromPacket->clusters[j].clusterRef;

                if (clusterRef && clusterRef->clusterRef == requirement->clusterDef)
                {
                    break;
                }
            }

            if (j != numClusters)
            {
                if (fromPacket->clusters[j].data)
                {
                    toPacket->clusters[requirement->slotIndex] = fromPacket->clusters[j];
                    toPacket->clusters[requirement->slotIndex].clusterRef =
                        toPipeline->nodes[0].slotClusterRefs[requirement->slotIndex];
                    fromPacket->clusters[j].data = NULL;
                }
            }
        }

        _rxPacketDestroy(fromPacket);

        toPacket->slotClusterRefs = toPipeline->nodes[0].slotClusterRefs;
        toPacket->inputToClusterSlot = toPipeline->nodes[0].inputToClusterSlot;
        toPacket->slotsContinue = toPipeline->nodes[0].slotsContinue;
        toPipeline->embeddedPacketState = rxPKST_PENDING;
    }
}

// Passes the embedded packet from a node to the node one of its outputs leads to: unless every
// cluster continues down that output, releases the clusters that stop there, then points the packet
// at the next node's slot tables. Returns the next node, or NULL where the output leads nowhere.
RxPipelineNode* _rxEmbeddedPacketBetweenNodes(RxPipeline* pipeline, RxPipelineNode* nodeFrom,
                                              U32 whichOutput)
{
    RxPacket* packet;
    U32 nodeToIndex = nodeFrom->outputs[whichOutput];

    if (nodeToIndex != (U32)-1)
    {
        RxPipelineNode* nodeTo = &pipeline->nodes[nodeToIndex];

        if (pipeline->embeddedPacketState > rxPKST_UNUSED)
        {
            U32 outputMask = 1 << whichOutput;

            packet = pipeline->embeddedPacket;

            if (!(packet->slotsContinue[0] & outputMask))
            {
                U32 i = packet->numClusters;

                do
                {
                    if (!(outputMask & packet->slotsContinue[i]) && packet->clusters[i - 1].clusterRef)
                    {
                        if (packet->clusters[i - 1].data &&
                            !(packet->clusters[i - 1].flags & CLUSTER_EXTERNAL))
                        {
                            RxHeapFree(_rxHeapGlobal, packet->clusters[i - 1].data);
                        }

                        *(U32*)&packet->clusters[i - 1] = 0;
                        packet->clusters[i - 1].data = NULL;
                        packet->clusters[i - 1].numAlloced = 0;
                        packet->clusters[i - 1].numUsed = 0;
                        packet->clusters[i - 1].clusterRef = NULL;
                    }
                } while (--i);
            }

            packet->inputToClusterSlot = nodeTo->inputToClusterSlot;
            packet->slotsContinue = nodeTo->slotsContinue;
            packet->slotClusterRefs = nodeTo->slotClusterRefs;
            pipeline->embeddedPacketState = rxPKST_PENDING;
        }

        return nodeTo;
    }

    return NULL;
}

// Releases owned cluster buffers, clears each released cluster's flags and stride, and marks
// the pipeline's embedded packet unused.
void _rxPacketDestroy(RxPacket* packet)
{
    U32 i;
    RxCluster* cluster = packet->clusters;

    packet->pipeline->embeddedPacketState = rxPKST_UNUSED;

    i = packet->numClusters;
    do
    {
        if (cluster->clusterRef)
        {
            if (cluster->data && !(cluster->flags & CLUSTER_EXTERNAL))
            {
                RxHeapFree(_rxHeapGlobal, cluster->data);
            }

            *(U32*)cluster = 0;
            cluster->data = NULL;
            cluster->numAlloced = 0;
            cluster->numUsed = 0;
            cluster->clusterRef = NULL;
        }

        cluster++;
    } while (--i);

    packet->flags = 0;
}
