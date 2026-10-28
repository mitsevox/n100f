#include <rwcore.h>
#include <string.h>

// Cluster flags.
#define CLUSTER_VALID 0x1U
#define CLUSTER_EXTERNAL 0x2U
#define CLUSTER_EXTERNALMODIFIABLE 0x4U
#define CLUSTER_MODIFIED 0x8U

// Each node has room for this many outputs after the pipeline's node array.
#define NODE_MAXOUTPUTS 32

// The pipeline being executed and the parameters its nodes receive.
typedef struct RxExecContext RxExecContext;
struct RxExecContext
{
    RxPipeline* pipeline;
    RxPipelineNode* unk4;
    S32 exitCode;
    U32 unkC;
    RxPipelineNodeParam params;
};

RxExecContext _rxExecCtxGlobal;
U32 _rxHeapInitialSize;
U32 _rxPipelineMaxNodes;
S32 RxPipelineInstanced;
RxHeap* _rxHeapGlobal;

S32 _rxPipelineClose(void)
{
    if (RxPipelineInstanced)
    {
        RwFreeListDestroy(RXPIPELINEGLOBAL(pipesFreeList));
        RXPIPELINEGLOBAL(pipesFreeList) = NULL;

        RxHeapDestroy(_rxHeapGlobal);
        _rxHeapGlobal = NULL;

        RxPipelineInstanced = FALSE;
    }

    return TRUE;
}

S32 _rxPipelineOpen(void)
{
    if (!RxPipelineInstanced)
    {
        _rxHeapGlobal = RxHeapCreate(_rxHeapInitialSize);

        if (!_rxHeapGlobal)
        {
            return FALSE;
        }

        RXPIPELINEGLOBAL(pipesFreeList) = RwFreeListCreate(sizeof(RxPipeline), 64, 4);

        if (!RXPIPELINEGLOBAL(pipesFreeList))
        {
            RxHeapDestroy(_rxHeapGlobal);
            _rxHeapGlobal = NULL;
            return FALSE;
        }

        RXPIPELINEGLOBAL(maxNodesPerPipeline) = _rxPipelineMaxNodes;

        RxRenderStateVectorSetDefaultRenderStateVector(&RXPIPELINEGLOBAL(defaultRenderState));

        RXPIPELINEGLOBAL(unk38) = NULL;
        RXPIPELINEGLOBAL(unk34) = NULL;

        RxPipelineInstanced = TRUE;

        return TRUE;
    }

    return FALSE;
}

RxPipelineNode* PipelineNodeDestroy(RxPipelineNode* node, RxPipeline* pipeline)
{
    if (!pipeline->locked)
    {
        if (node->nodeDef->nodeMethods.pipelineNodeTerm)
        {
            node->nodeDef->nodeMethods.pipelineNodeTerm(node);
        }

        node->nodeDef->InputPipesCnt--;

        if (node->nodeDef->InputPipesCnt == 0)
        {
            if (node->nodeDef->nodeMethods.nodeTerm)
            {
                node->nodeDef->nodeMethods.nodeTerm(node->nodeDef);
            }

            if (node->nodeDef->editable)
            {
                RwFree(node->nodeDef);
                node->nodeDef = NULL;
            }
        }

        if (node->initializationData)
        {
            RwFree(node->initializationData);
            node->initializationData = NULL;
            node->initializationDataSize = 0;
        }

        memset(node, 0, sizeof(RxPipelineNode));
    }
    else
    {
        S32 nodeIndex;

        if (node->initializationData)
        {
            RwFree(node->initializationData);
            node->initializationData = NULL;
            node->initializationDataSize = 0;
        }

        if (node->nodeDef->InputPipesCnt == 0 && node->nodeDef->editable)
        {
            RwFree(node->nodeDef);
            node->nodeDef = NULL;
        }

        nodeIndex = node - pipeline->nodes;

        if (nodeIndex < pipeline->numNodes - 1)
        {
            U32 output;
            U32* srcOutputs;
            U32* dstOutputs;
            RxPipelineNodeTopSortData* srcTopSortData;
            RxPipelineNodeTopSortData* dstTopSortData;
            U32 i;

            dstOutputs = (U32*)&pipeline->nodes[RXPIPELINEGLOBAL(maxNodesPerPipeline)];
            dstOutputs += nodeIndex * NODE_MAXOUTPUTS;
            srcOutputs = dstOutputs + NODE_MAXOUTPUTS;

            for (i = nodeIndex; i < pipeline->numNodes - 1; i++)
            {
                memcpy(dstOutputs, srcOutputs, NODE_MAXOUTPUTS * sizeof(U32));
                dstOutputs = srcOutputs;
                srcOutputs += NODE_MAXOUTPUTS;
            }

            dstOutputs = (U32*)&pipeline->nodes[RXPIPELINEGLOBAL(maxNodesPerPipeline)];
            dstOutputs += RXPIPELINEGLOBAL(maxNodesPerPipeline) * NODE_MAXOUTPUTS;
            dstTopSortData = (RxPipelineNodeTopSortData*)dstOutputs;
            srcTopSortData = dstTopSortData + 1;

            for (i = nodeIndex; i < pipeline->numNodes - 1; i++)
            {
                memcpy(dstTopSortData, srcTopSortData, sizeof(RxPipelineNodeTopSortData));
                dstTopSortData = srcTopSortData;
                srcTopSortData++;
            }

            for (i = nodeIndex; i < pipeline->numNodes - 1; i++)
            {
                memcpy(&pipeline->nodes[i], &pipeline->nodes[i + 1], sizeof(RxPipelineNode));
                pipeline->nodes[i].outputs -= NODE_MAXOUTPUTS;
                pipeline->nodes[i].topSortData--;
            }

            for (i = 0; i < pipeline->numNodes - 1; i++)
            {
                U32 j;

                for (j = 0; j < pipeline->nodes[i].numOutputs; j++)
                {
                    output = pipeline->nodes[i].outputs[j];

                    if (output >= nodeIndex)
                    {
                        if (nodeIndex == output)
                        {
                            pipeline->nodes[i].outputs[j] = (U32)-1;
                        }
                        else
                        {
                            pipeline->nodes[i].outputs[j] = output - 1;
                        }
                    }
                }
            }
        }
    }

    pipeline->numNodes--;

    return node;
}

RxHeap* RxHeapGetGlobalHeap(void)
{
    return _rxHeapGlobal;
}

RxPacket* RxPacketCreate(RxPipelineNode* node)
{
    RxPacket* packet;

    _rxExecCtxGlobal.pipeline->embeddedPacketState = rxPKST_INUSE;
    packet = _rxExecCtxGlobal.pipeline->embeddedPacket;

    packet->inputToClusterSlot = node->inputToClusterSlot;
    packet->slotsContinue = node->slotsContinue;
    packet->slotClusterRefs = node->slotClusterRefs;

    return packet;
}

RxCluster* RxClusterSetExternalData(RxCluster* cluster, void* data, U16 stride, U32 numElements)
{
    if (cluster->data && !(cluster->flags & CLUSTER_EXTERNAL) && cluster->data != data)
    {
        RxHeapFree(_rxHeapGlobal, cluster->data);
        cluster->data = NULL;
    }

    cluster->data = data;
    cluster->currentData = data;
    cluster->stride = stride;
    cluster->flags |= CLUSTER_VALID | CLUSTER_EXTERNAL;
    cluster->numAlloced = numElements;
    cluster->numUsed = numElements;

    return cluster;
}

RxCluster* RxClusterSetData(RxCluster* cluster, void* data, U16 stride, U32 numElements)
{
    if (cluster->data && !(cluster->flags & CLUSTER_EXTERNAL) && cluster->data != data)
    {
        RxHeapFree(_rxHeapGlobal, cluster->data);
        cluster->data = NULL;
    }

    cluster->data = data;
    cluster->currentData = data;
    cluster->stride = stride;
    cluster->flags |= CLUSTER_VALID;
    cluster->numAlloced = numElements;
    cluster->numUsed = numElements;

    return cluster;
}

RxCluster* RxClusterInitializeData(RxCluster* cluster, U32 numElements, U16 stride)
{
    U32 size = numElements * stride;

    if (cluster->data && !(cluster->flags & CLUSTER_EXTERNAL))
    {
        RxHeapFree(_rxHeapGlobal, cluster->data);
    }

    cluster->currentData = cluster->data = RxHeapAlloc(_rxHeapGlobal, size);
    cluster->numAlloced = numElements;
    cluster->numUsed = 0;
    cluster->flags |= CLUSTER_VALID;
    cluster->stride = stride;

    return cluster;
}

RxCluster* RxClusterResizeData(RxCluster* cluster, U32 numElements)
{
    cluster->currentData = cluster->data =
        RxHeapRealloc(_rxHeapGlobal, cluster->data, numElements * cluster->stride, TRUE);
    cluster->flags |= CLUSTER_VALID;

    if (cluster->numUsed > numElements)
    {
        cluster->numUsed = numElements;
    }

    cluster->numAlloced = numElements;

    return cluster;
}

RxCluster* RxClusterLockWrite(RxPacket* packet, U32 clusterIndex, RxPipelineNode* node)
{
    U32 slot = packet->inputToClusterSlot[clusterIndex];
    RxCluster* cluster;

    if (slot != (U32)-1)
    {
        cluster = &packet->clusters[slot];

        if (!cluster->clusterRef)
        {
            cluster->attributes =
                (cluster->clusterRef = packet->slotClusterRefs[slot])->creationAttributes;
        }

        cluster->flags |= CLUSTER_MODIFIED;

        if ((cluster->flags & (CLUSTER_EXTERNAL | CLUSTER_EXTERNALMODIFIABLE)) == CLUSTER_EXTERNAL)
        {
            void* data = cluster->data;
            U32 numAlloced = cluster->numAlloced;
            U32 numUsed = cluster->numUsed;

            cluster->flags &= ~CLUSTER_EXTERNAL;
            cluster->data = NULL;

            if (!RxClusterInitializeData(cluster, numAlloced, cluster->stride))
            {
                cluster->flags |= CLUSTER_EXTERNAL;
                cluster->data = data;
                return NULL;
            }

            cluster->numUsed = numUsed;
            memcpy(cluster->data, data, numUsed * cluster->stride);
        }

        cluster->currentData = cluster->data;

        return cluster;
    }

    return NULL;
}

static void ExecuteNode(RxPipelineNode* node, RxPipelineNodeParam* params)
{
    U32 result = node->nodeDef->nodeMethods.nodeBody(node, params);

    if (!result)
    {
        _rxExecCtxGlobal.exitCode = result;
    }
}

RxPipeline* RxPipelineExecute(RxPipeline* pipeline, void* data, S32 heapReset)
{
    if (heapReset && _rxHeapGlobal->dirty)
    {
        _rxHeapReset(_rxHeapGlobal);
    }

    _rxExecCtxGlobal.exitCode = TRUE;
    _rxExecCtxGlobal.pipeline = pipeline;
    _rxExecCtxGlobal.params.dataParam = data;
    _rxExecCtxGlobal.params.heap = _rxHeapGlobal;

    pipeline->embeddedPacketState = rxPKST_PACKETLESS;

    ExecuteNode(pipeline->nodes, &_rxExecCtxGlobal.params);

    if (pipeline->embeddedPacketState > rxPKST_UNUSED)
    {
        pipeline->embeddedPacketState = rxPKST_INUSE;
        _rxPacketDestroy(pipeline->embeddedPacket);
    }

    _rxExecCtxGlobal.params.dataParam = NULL;
    _rxExecCtxGlobal.pipeline = NULL;
    _rxExecCtxGlobal.params.heap = NULL;

    if (_rxExecCtxGlobal.exitCode)
    {
        return pipeline;
    }

    return NULL;
}

RxPipeline* RxPipelineCreate(void)
{
    RxPipeline* pipeline = (RxPipeline*)RWSRCGLOBAL(memoryAlloc)(RXPIPELINEGLOBAL(pipesFreeList));

    if (pipeline)
    {
        memset(pipeline, 0, sizeof(RxPipeline));
        pipeline->locked = FALSE;
        return pipeline;
    }
    else
    {
        RwError err;

        err.pluginID = 1;
        err.errorCode = _rwerror(0x80000013, sizeof(RxPipeline));
        RwErrorSet(&err);
        return NULL;
    }
}

void _rxPipelineDestroy(RxPipeline* pipeline)
{
    if (pipeline)
    {
        U32 numNodes;
        U32 i;
        RxPipelineNode* node;

        node = pipeline->nodes;
        numNodes = pipeline->numNodes;

        for (i = 0; i < numNodes; i++)
        {
            PipelineNodeDestroy(node, pipeline);
            node++;
        }

        pipeline->nodes = NULL;

        if (pipeline->superBlock)
        {
            RwFree(pipeline->superBlock);
            pipeline->superBlock = NULL;
            pipeline->superBlockSize = 0;
        }

        RWSRCGLOBAL(memoryFree)(RXPIPELINEGLOBAL(pipesFreeList), pipeline);
    }
}
