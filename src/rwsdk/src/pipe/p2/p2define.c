#include <rwcore.h>
#include <stdarg.h>
#include <string.h>

// Each node has room for this many outputs after the pipeline's node array.
#define NODE_MAXOUTPUTS 32

// Scratch space used while a pipeline is unlocked: StalacTiteAlloc takes blocks from the top
// down, StalacMiteAlloc from the bottom up, until the two ends meet.
typedef struct MemoryLimits MemoryLimits;
struct MemoryLimits
{
    U8* tite;
    U8* mite;
};

static MemoryLimits gMemoryLimits = { NULL, NULL };

void* StalacTiteAlloc(U32 size)
{
    size = (size + 3) & ~3;
    gMemoryLimits.tite -= size;

    if (gMemoryLimits.tite < gMemoryLimits.mite)
    {
        RwError err;

        gMemoryLimits.tite += size;
        err.pluginID = 1;
        err.errorCode = _rwerror(0x80000013, size);
        RwErrorSet(&err);
        return NULL;
    }

    return gMemoryLimits.tite;
}

void* StalacMiteAlloc(U32 size)
{
    size = (size + 3) & ~3;
    gMemoryLimits.mite += size;

    if (gMemoryLimits.mite > gMemoryLimits.tite)
    {
        RwError err;

        gMemoryLimits.mite -= size;
        err.pluginID = 1;
        err.errorCode = _rwerror(0x80000013, size);
        RwErrorSet(&err);
        return NULL;
    }

    return gMemoryLimits.mite - size;
}

// Counts the different cluster definitions the pipeline's nodes are interested in, taking them
// in increasing address order.
U32 PipelineCalcNumUniqueClusters(RxPipeline* pipeline)
{
    U32 lastCluster;
    U32 nextCluster;
    U32 numUnique;

    numUnique = 0;
    nextCluster = 0;

    for (;;)
    {
        U32 i;

        lastCluster = nextCluster;
        nextCluster = (U32)-1;

        for (i = 0; i < pipeline->numNodes; i++)
        {
            RxNodeDefinition* nodeDef;
            U32 j;

            nodeDef = pipeline->nodes[i].nodeDef;

            for (j = 0; j < nodeDef->io.numClustersOfInterest; j++)
            {
                U32 cluster;

                cluster = (U32)nodeDef->io.clustersOfInterest[j].clusterDef;

                if (cluster > lastCluster && cluster < nextCluster)
                {
                    nextCluster = cluster;
                }
            }
        }

        if (nextCluster == (U32)-1)
        {
            break;
        }

        numUnique++;
    }

    return numUnique;
}

S32 LockPipelineExpandData(RxPipeline* pipeline, RxPipeline* srcPipeline)
{
    U32* outputs;
    S32 i;
    RxPipelineNodeTopSortData* topSortData;

    if (pipeline != srcPipeline)
    {
        for (i = srcPipeline->numNodes - 1; i >= 0; i--)
        {
            memcpy(&pipeline->nodes[i], &srcPipeline->nodes[i], sizeof(RxPipelineNode));
            pipeline->nodes[i].slotClusterRefs = NULL;
            pipeline->nodes[i].slotsContinue = NULL;
            pipeline->nodes[i].privateData = NULL;
            pipeline->nodes[i].inputToClusterSlot = NULL;

            if (pipeline->nodes[i].initializationDataSize)
            {
                pipeline->nodes[i].initializationData =
                    RwMalloc(pipeline->nodes[i].initializationDataSize);

                if (!pipeline->nodes[i].initializationData)
                {
                    RwError err;

                    err.pluginID = 1;
                    err.errorCode =
                        _rwerror(0x80000013, pipeline->nodes[i].initializationDataSize);
                    RwErrorSet(&err);
                    return FALSE;
                }

                memcpy(pipeline->nodes[i].initializationData,
                       srcPipeline->nodes[i].initializationData,
                       pipeline->nodes[i].initializationDataSize);
            }
        }

        pipeline->numNodes = srcPipeline->numNodes;
    }

    outputs = (U32*)&pipeline->nodes[RXPIPELINEGLOBAL(maxNodesPerPipeline)];

    for (i = srcPipeline->numNodes - 1; i >= 0; i--)
    {
        pipeline->nodes[i].outputs = outputs + i * NODE_MAXOUTPUTS;

        if (srcPipeline->nodes[i].outputs)
        {
            memcpy(pipeline->nodes[i].outputs, srcPipeline->nodes[i].outputs,
                   NODE_MAXOUTPUTS * sizeof(U32));
        }
    }

    topSortData = (RxPipelineNodeTopSortData*)(outputs + RXPIPELINEGLOBAL(maxNodesPerPipeline)
                                                            * NODE_MAXOUTPUTS);

    for (i = 0; i < srcPipeline->numNodes; i++)
    {
        topSortData[i].numIns = 0;
        topSortData[i].numInsVisited = 0;
        topSortData[i].req = NULL;
        pipeline->nodes[i].topSortData = &topSortData[i];
    }

    return TRUE;
}

S32 _NodeCreate(RxPipeline* pipeline, RxPipelineNode* node, RxNodeDefinition* nodeDef)
{
    S32 result;
    U32 numOutputs;

    numOutputs = nodeDef->io.numOutputs;
    result = TRUE;

    memset(node, 0, sizeof(RxPipelineNode));

    if (numOutputs > NODE_MAXOUTPUTS)
    {
        RwError err;

        err.pluginID = 1;
        err.errorCode = _rwerror(0x29);
        RwErrorSet(&err);
        result = FALSE;
    }

    if (nodeDef->io.numClustersOfInterest > 32)
    {
        RwError err;

        err.pluginID = 1;
        err.errorCode = _rwerror(0x28);
        RwErrorSet(&err);
        result = FALSE;
    }

    if (numOutputs >= RXPIPELINEGLOBAL(maxNodesPerPipeline))
    {
        RwError err;

        err.pluginID = 1;
        err.errorCode = _rwerror(0x2A);
        RwErrorSet(&err);
        result = FALSE;
    }

    if (result)
    {
        U32* outputs;
        U32 i;
        RxPipelineNodeTopSortData* topSortData;

        outputs = (U32*)&pipeline->nodes[RXPIPELINEGLOBAL(maxNodesPerPipeline)];
        outputs += pipeline->numNodes * NODE_MAXOUTPUTS;
        node->outputs = outputs;
        node->numOutputs = numOutputs;

        for (i = 0; i < node->numOutputs; i++)
        {
            *outputs++ = (U32)-1;
        }

        outputs = (U32*)&pipeline->nodes[RXPIPELINEGLOBAL(maxNodesPerPipeline)];
        topSortData = (RxPipelineNodeTopSortData*)(outputs + RXPIPELINEGLOBAL(maxNodesPerPipeline)
                                                                * NODE_MAXOUTPUTS);
        topSortData += pipeline->numNodes;
        topSortData->numIns = 0;
        topSortData->numInsVisited = 0;
        topSortData->req = NULL;
        node->topSortData = topSortData;
        node->initializationData = NULL;
        node->initializationDataSize = 0;
        node->nodeDef = nodeDef;
        pipeline->numNodes++;
    }

    return result;
}

// The pipeline being sorted and the number of nodes already placed at its front.
typedef struct PipelineTopSortState PipelineTopSortState;
struct PipelineTopSortState
{
    RxPipeline* pipeline;
    U32 numSorted;
};

// Moves node nodeIndex to the next free place at the front of the pipeline, renumbering every
// output that points at either of the two swapped nodes, then visits each node it feeds once all
// of that node's inputs have been placed.
void PipelineTopSort(PipelineTopSortState* state, U32 nodeIndex)
{
    U32 sorted;
    RxPipelineNode* node;
    U32 i;

    sorted = state->numSorted;

    if (sorted != nodeIndex)
    {
        U32* outputs0;
        U32* outputs1;
        RxPipelineNodeTopSortData* topSortData0;
        RxPipelineNodeTopSortData* topSortData1;
        RxPipelineNodeTopSortData tmpTopSortData;
        RxPipelineNode tmpNode;

        outputs0 = state->pipeline->nodes[sorted].outputs;
        outputs1 = state->pipeline->nodes[nodeIndex].outputs;

        for (i = 0; i < NODE_MAXOUTPUTS; i++)
        {
            U32 tmp;

            tmp = outputs0[i];
            outputs0[i] = outputs1[i];
            outputs1[i] = tmp;
        }

        state->pipeline->nodes[sorted].outputs = outputs1;
        state->pipeline->nodes[nodeIndex].outputs = outputs0;

        topSortData0 = state->pipeline->nodes[sorted].topSortData;
        topSortData1 = state->pipeline->nodes[nodeIndex].topSortData;
        tmpTopSortData = *topSortData0;
        *topSortData0 = *topSortData1;
        *topSortData1 = tmpTopSortData;
        state->pipeline->nodes[sorted].topSortData = topSortData1;
        state->pipeline->nodes[nodeIndex].topSortData = topSortData0;

        tmpNode = state->pipeline->nodes[sorted];
        state->pipeline->nodes[sorted] = state->pipeline->nodes[nodeIndex];
        state->pipeline->nodes[nodeIndex] = tmpNode;

        for (i = 0; i < state->pipeline->numNodes; i++)
        {
            U32 j;
            RxPipelineNode* other;

            other = &state->pipeline->nodes[i];

            for (j = 0; j < other->numOutputs; j++)
            {
                if (other->outputs[j] == sorted)
                {
                    other->outputs[j] = nodeIndex;
                }
                else if (other->outputs[j] == nodeIndex)
                {
                    other->outputs[j] = sorted;
                }
            }
        }
    }

    node = &state->pipeline->nodes[state->numSorted];
    state->numSorted++;

    if (node->numOutputs)
    {
        for (i = 0; i < node->numOutputs; i++)
        {
            U32 output;

            output = node->outputs[i];

            if (output != (U32)-1)
            {
                RxPipelineNode* next;

                next = &state->pipeline->nodes[output];
                next->topSortData->numInsVisited++;

                if (next->topSortData->numIns == next->topSortData->numInsVisited)
                {
                    PipelineTopSort(state, output);
                }
            }
        }
    }
}

RxNodeOutput RxPipelineNodeFindOutputByName(RxPipelineNode* node, const char* outputname)
{
    if (node && node->nodeDef && outputname)
    {
        S32 i;
        S32 numOutputs = node->nodeDef->io.numOutputs;
        RxOutputSpec* output = node->nodeDef->io.outputs;

        for (i = 0; i < numOutputs; i++, output++)
        {
            if (!rwstrcmp(output->name, outputname))
            {
                return &node->outputs[i];
            }
        }
    }

    return NULL;
}

RxNodeOutput RxPipelineNodeFindOutputByIndex(RxPipelineNode* node, U32 outputindex)
{
    if (node && node->nodeDef && outputindex < node->numOutputs)
    {
        return &node->outputs[outputindex];
    }

    return NULL;
}

RxNodeInput RxPipelineNodeFindInput(RxPipelineNode* node)
{
    if (node && node->nodeDef)
    {
        return node;
    }

    return NULL;
}

// Grows the pipeline's block (its nodes, their outputs and their sort data) to size, moving every
// pointer into the block along with it.
static inline S32 PipelineGrowSuperBlock(RxPipeline* pipeline, U32 size)
{
    void* oldBlock;
    void* newBlock;

    oldBlock = pipeline->superBlock;
    newBlock = RwRealloc(oldBlock, size);

    if (newBlock)
    {
        U32 numNodes;
        S32 offset;
        U32 i;

        numNodes = pipeline->numNodes;
        offset = (U8*)newBlock - (U8*)oldBlock;
        pipeline->superBlock = newBlock;
        pipeline->superBlockSize = size;
        pipeline->nodes = (RxPipelineNode*)pipeline->superBlock;

        if (pipeline->embeddedPacket)
        {
            pipeline->embeddedPacket = (RxPacket*)((U8*)pipeline->embeddedPacket + offset);
        }

        if (pipeline->inputRequirements)
        {
            pipeline->inputRequirements =
                (RxPipelineRequiresCluster*)((U8*)pipeline->inputRequirements + offset);
        }

        for (i = 0; i < numNodes; i++)
        {
            if (pipeline->nodes[i].outputs)
            {
                pipeline->nodes[i].outputs = (U32*)((U8*)pipeline->nodes[i].outputs + offset);
            }

            if (pipeline->nodes[i].slotClusterRefs)
            {
                pipeline->nodes[i].slotClusterRefs =
                    (RxPipelineCluster**)((U8*)pipeline->nodes[i].slotClusterRefs + offset);
            }

            if (pipeline->nodes[i].slotsContinue)
            {
                pipeline->nodes[i].slotsContinue =
                    (U32*)((U8*)pipeline->nodes[i].slotsContinue + offset);
            }

            if (pipeline->nodes[i].privateData)
            {
                pipeline->nodes[i].privateData = (U8*)pipeline->nodes[i].privateData + offset;
            }

            if (pipeline->nodes[i].inputToClusterSlot)
            {
                pipeline->nodes[i].inputToClusterSlot =
                    (U32*)((U8*)pipeline->nodes[i].inputToClusterSlot + offset);
            }

            if (pipeline->nodes[i].topSortData)
            {
                pipeline->nodes[i].topSortData =
                    (RxPipelineNodeTopSortData*)((U8*)pipeline->nodes[i].topSortData + offset);
            }
        }
    }
    else
    {
        RwError err;

        err.pluginID = 1;
        err.errorCode = _rwerror(0x80000013, size);
        RwErrorSet(&err);
        return FALSE;
    }

    return TRUE;
}

// Checks that every node can be reached from the entry point and that the paths contain no
// loops, then sorts the nodes so each comes after every node that feeds it, entry point first.
static inline RxPipeline* PipelineUnlockTopSort(RxPipeline* pipeline)
{
    PipelineTopSortState state;
    RxPipelineNode* node;
    U32 i;

    state.pipeline = pipeline;
    state.numSorted = 0;

    node = pipeline->nodes;

    for (i = 0; i < pipeline->numNodes; node++, i++)
    {
        if (node->nodeDef)
        {
            node->topSortData->numInsVisited = 0;
            node->topSortData->numIns = 0;
        }
    }

    node = pipeline->nodes;

    for (i = 0; i < pipeline->numNodes; node++, i++)
    {
        if (node->nodeDef)
        {
            U32* outputs;
            U32 j;

            j = node->numOutputs;

            if (j)
            {
                outputs = node->outputs;

                do
                {
                    if ((S32)*outputs != -1)
                    {
                        pipeline->nodes[*outputs].topSortData->numIns++;
                    }

                    outputs++;
                } while (--j);
            }
        }
    }

    if (pipeline->nodes[pipeline->entryPoint].topSortData->numIns != 0)
    {
        RwError err;

        err.pluginID = 1;
        err.errorCode = _rwerror(0x24);
        RwErrorSet(&err);
        return NULL;
    }

    for (i = 0; i < pipeline->numNodes; i++)
    {
        if (i != pipeline->entryPoint && pipeline->nodes[i].topSortData->numIns == 0)
        {
            RwError err;

            err.pluginID = 1;
            err.errorCode = _rwerror(0x22);
            RwErrorSet(&err);
            return NULL;
        }
    }

    PipelineTopSort(&state, pipeline->entryPoint);

    for (i = 0; i < pipeline->numNodes; i++)
    {
        if (pipeline->nodes[i].topSortData->numIns
            != pipeline->nodes[i].topSortData->numInsVisited)
        {
            RwError err;

            err.pluginID = 1;
            err.errorCode = _rwerror(0x1C);
            RwErrorSet(&err);
            return NULL;
        }
    }

    pipeline->entryPoint = 0;
    return pipeline;
}

RxPipeline* RxLockedPipeUnlock(RxLockedPipe* pipeline)
{
    if (pipeline && pipeline->locked)
    {
        if (pipeline->numNodes)
        {
            S32 i;
            U32 size;
            RxPipelineNode* node;
            U32 maxSize;
            U32 dataSize;
            U32 numUnique;
            U32 numFailed = 0;
            S32 failed = FALSE;
            U32 numOutputs = 0;

            if (pipeline->entryPoint >= pipeline->numNodes
                || !pipeline->nodes[pipeline->entryPoint].nodeDef)
            {
                RwError err;

                err.pluginID = 1;
                err.errorCode = _rwerror(0x24);
                RwErrorSet(&err);
                return NULL;
            }

            numUnique = PipelineCalcNumUniqueClusters(pipeline);

            maxSize = RXPIPELINEGLOBAL(maxNodesPerPipeline) * sizeof(RxPipelineNode)
                      + RXPIPELINEGLOBAL(maxNodesPerPipeline) * NODE_MAXOUTPUTS * sizeof(U32)
                      + RXPIPELINEGLOBAL(maxNodesPerPipeline) * sizeof(RxPipelineNodeTopSortData);

            // The nodes and their outputs while they are sorted, and the space the dependency
            // chasing works in.
            size = pipeline->numNodes * sizeof(RxPipelineNode);
            node = pipeline->nodes;

            for (i = 0; i < pipeline->numNodes; i++)
            {
                size += node->numOutputs * sizeof(U32);
            }

            size = size + RXPIPELINEGLOBAL(maxNodesPerPipeline) * sizeof(RxPipelineNodeTopSortData)
                   + (pipeline->numNodes * numUnique * sizeof(rxReqEntry)
                      + pipeline->numNodes * numUnique * sizeof(rxScope))
                   + pipeline->numNodes * sizeof(rxReq);

            // The data the unlocked pipeline keeps: its clusters, its input requirements, each
            // node's cluster references, continuity flags, private data and input slots, and the
            // packet it executes with.
            dataSize = numUnique * sizeof(RxPipelineCluster)
                       + numUnique * sizeof(RxPipelineRequiresCluster);
            dataSize += pipeline->numNodes * numUnique * sizeof(RxPipelineCluster*);
            dataSize += pipeline->numNodes * (numUnique + 1) * sizeof(U32);

            for (i = 0; i < pipeline->numNodes; i++, node++)
            {
                if (node->nodeDef->pipelineNodePrivateDataSize)
                {
                    dataSize += node->nodeDef->pipelineNodePrivateDataSize;
                }

                dataSize += node->nodeDef->io.numClustersOfInterest * sizeof(U32);
            }

            dataSize += sizeof(RxPacket) + (numUnique - 1) * sizeof(RxCluster);
            size += dataSize;
            size = (size > maxSize) ? size : maxSize;

            if (size > pipeline->superBlockSize)
            {
                if (!PipelineGrowSuperBlock(pipeline, size))
                {
                    return NULL;
                }
            }

            gMemoryLimits.tite = (U8*)pipeline->superBlock + size;
            gMemoryLimits.mite = NULL;

            pipeline = PipelineUnlockTopSort(pipeline);

            if (!pipeline)
            {
                return NULL;
            }

            {
                RxPipelineNodeTopSortData* topSortData;
                RxPipelineNodeTopSortData* newTopSortData;
                U32* newOutputs;
                U32* outputs;

                // Move the sort data to the top of the block, then pack the outputs in after
                // the nodes.
                outputs = (U32*)&pipeline->nodes[RXPIPELINEGLOBAL(maxNodesPerPipeline)];
                topSortData = (RxPipelineNodeTopSortData*)(outputs
                                                           + RXPIPELINEGLOBAL(maxNodesPerPipeline)
                                                                 * NODE_MAXOUTPUTS);
                topSortData += pipeline->numNodes - 1;
                newTopSortData = (RxPipelineNodeTopSortData*)((U8*)pipeline->superBlock + size
                                                              - sizeof(RxPipelineNodeTopSortData));

                for (i = pipeline->numNodes - 1; i >= 0; i--)
                {
                    memcpy(newTopSortData, topSortData, sizeof(RxPipelineNodeTopSortData));
                    pipeline->nodes[i].topSortData = newTopSortData;
                    topSortData--;
                    newTopSortData--;
                }

                newOutputs = (U32*)&pipeline->nodes[pipeline->numNodes];

                for (i = 0; i < pipeline->numNodes; i++)
                {
                    if (pipeline->nodes[i].numOutputs == 0)
                    {
                        pipeline->nodes[i].outputs = NULL;
                    }
                    else
                    {
                        memcpy(newOutputs, outputs, pipeline->nodes[i].numOutputs * sizeof(U32));
                        pipeline->nodes[i].outputs = newOutputs;
                    }

                    numOutputs += pipeline->nodes[i].numOutputs;
                    newOutputs += pipeline->nodes[i].numOutputs;
                    outputs += NODE_MAXOUTPUTS;
                }

                gMemoryLimits.tite = (U8*)topSortData;
                gMemoryLimits.mite = (U8*)(outputs + numOutputs);
            }

            if (_rxChaseDependencies(pipeline))
            {
                return NULL;
            }

            if (!PipelineGrowSuperBlock(pipeline, gMemoryLimits.mite - (U8*)pipeline->superBlock))
            {
                goto fail;
            }

            for (i = 0; i < pipeline->numNodes; i++)
            {
                pipeline->nodes[i].topSortData = NULL;
            }

            for (i = pipeline->numNodes - 1; i >= 0; i--)
            {
                RxNodeDefinition* nodeDef;

                node = &pipeline->nodes[i];
                nodeDef = node->nodeDef;

                if (nodeDef->InputPipesCnt++ == 0 && nodeDef->nodeMethods.nodeInit
                    && !nodeDef->nodeMethods.nodeInit(nodeDef))
                {
                    failed = TRUE;
                    numFailed = pipeline->numNodes - 1 - i;
                    break;
                }

                if (nodeDef->nodeMethods.pipelineNodeInit
                    && !nodeDef->nodeMethods.pipelineNodeInit(node))
                {
                    nodeDef->InputPipesCnt--;

                    if (nodeDef->InputPipesCnt == 0 && nodeDef->nodeMethods.nodeTerm)
                    {
                        nodeDef->nodeMethods.nodeTerm(nodeDef);
                    }

                    failed = TRUE;
                    numFailed = pipeline->numNodes - 1 - i;
                    break;
                }
            }

            if (!failed)
            {
                for (i = pipeline->numNodes - 1; i >= 0; i--)
                {
                    RxNodeDefinition* nodeDef;

                    nodeDef = pipeline->nodes[i].nodeDef;

                    if (nodeDef->nodeMethods.pipelineNodeConfig
                        && !nodeDef->nodeMethods.pipelineNodeConfig(&pipeline->nodes[i], pipeline))
                    {
                        numFailed = pipeline->numNodes;
                        failed = TRUE;
                        break;
                    }
                }
            }

            if (failed)
            {
                for (i = pipeline->numNodes - numFailed; i < pipeline->numNodes; i++)
                {
                    RxNodeDefinition* nodeDef;

                    nodeDef = pipeline->nodes[i].nodeDef;

                    if (nodeDef->nodeMethods.pipelineNodeTerm)
                    {
                        nodeDef->nodeMethods.pipelineNodeTerm(&pipeline->nodes[i]);
                    }

                    nodeDef->InputPipesCnt--;

                    if (nodeDef->InputPipesCnt == 0 && nodeDef->nodeMethods.nodeTerm)
                    {
                        nodeDef->nodeMethods.nodeTerm(nodeDef);
                    }
                }

                goto fail;
            }
        }

        pipeline->locked = FALSE;
        return pipeline;
    }

    if (!pipeline)
    {
        RwError err;

        err.pluginID = 1;
        err.errorCode = _rwerror(0x80000016);
        RwErrorSet(&err);
    }
    else
    {
        RwError err;

        err.pluginID = 1;
        err.errorCode = _rwerror(0x34);
        RwErrorSet(&err);
    }

    return NULL;

fail:
    LockPipelineExpandData(pipeline, pipeline);
    return NULL;
}

RxLockedPipe* RxPipelineLock(RxPipeline* pipeline)
{
    if (!pipeline->locked)
    {
        U32 size;
        RxNodeDefinition* nodeDef;
        U32 i;

        size = RXPIPELINEGLOBAL(maxNodesPerPipeline) * sizeof(RxPipelineNode)
               + RXPIPELINEGLOBAL(maxNodesPerPipeline) * NODE_MAXOUTPUTS * sizeof(U32)
               + RXPIPELINEGLOBAL(maxNodesPerPipeline) * sizeof(RxPipelineNodeTopSortData);

        if (pipeline->nodes)
        {
            if (size > pipeline->superBlockSize)
            {
                if (!PipelineGrowSuperBlock(pipeline, size))
                {
                    return NULL;
                }
            }

            if (!LockPipelineExpandData(pipeline, pipeline))
            {
                return NULL;
            }
        }
        else
        {
            pipeline->superBlock = RwMalloc(size);

            if (!pipeline->superBlock)
            {
                RwError err;

                err.pluginID = 1;
                err.errorCode = _rwerror(0x80000013, size);
                RwErrorSet(&err);
                return NULL;
            }

            pipeline->superBlockSize = size;
            pipeline->nodes = (RxPipelineNode*)pipeline->superBlock;
        }

        pipeline->locked = TRUE;

        if (pipeline->nodes)
        {
            for (i = 0; i < pipeline->numNodes; i++)
            {
                nodeDef = pipeline->nodes[i].nodeDef;

                if (nodeDef->nodeMethods.pipelineNodeTerm)
                {
                    nodeDef->nodeMethods.pipelineNodeTerm(&pipeline->nodes[i]);
                }

                if (--pipeline->nodes[i].nodeDef->InputPipesCnt == 0
                    && nodeDef->nodeMethods.nodeTerm)
                {
                    nodeDef->nodeMethods.nodeTerm(pipeline->nodes[i].nodeDef);
                }

                pipeline->nodes[i].slotClusterRefs = NULL;
            }
        }
    }

    return pipeline;
}

RxPipelineNode* RxPipelineFindNodeByName(RxPipeline* pipeline, const char* name,
                                         RxPipelineNode* start, S32* nodeIndex)
{
    S32 valid = FALSE;

    if (pipeline && name && pipeline->numNodes)
    {
        valid = TRUE;
    }

    if (valid)
    {
        RxPipelineNode* node = pipeline->nodes;
        S32 n = pipeline->numNodes;

        if (start)
        {
            while (node != start && n > 0)
            {
                node++;
                n--;
            }

            node++;
            n--;
        }

        while (n > 0)
        {
            if (node->nodeDef && !rwstrcmp(node->nodeDef->name, name))
            {
                if (nodeIndex)
                {
                    *nodeIndex = n;
                }

                return node;
            }

            node++;
            n--;
        }
    }

    if (nodeIndex)
    {
        *nodeIndex = -1;
    }

    return NULL;
}

RxLockedPipe* RxLockedPipeAddFragment(RxLockedPipe* pipeline, U32* firstIndex,
                                      RxNodeDefinition* nodeDef0, ...)
{
    if (pipeline && pipeline->locked)
    {
        va_list va;
        RxNodeDefinition* nodeDef;
        U32 firstNode;
        U32 numNodes;

        va_start(va, nodeDef0);
        nodeDef = nodeDef0;
        numNodes = 0;

        while (nodeDef)
        {
            numNodes++;
            nodeDef = va_arg(va, RxNodeDefinition*);
        }

        va_end(va);

        if (numNodes)
        {
            U32 i;
            RxPipelineNode* lastNode;

            lastNode = NULL;
            firstNode = pipeline->numNodes;

            if (firstNode + numNodes > RXPIPELINEGLOBAL(maxNodesPerPipeline))
            {
                RwError err;

                err.pluginID = 1;
                err.errorCode = _rwerror(0x2A);
                RwErrorSet(&err);
                return NULL;
            }

            va_start(va, nodeDef0);
            nodeDef = nodeDef0;
            i = 0;

            while (nodeDef)
            {
                RxPipelineNode* node;

                node = &pipeline->nodes[firstNode + i];

                if (!_NodeCreate(pipeline, node, nodeDef))
                {
                    break;
                }

                if (lastNode
                    && !RxLockedPipeAddPath(pipeline, RxPipelineNodeFindOutputByIndex(lastNode, 0),
                                            RxPipelineNodeFindInput(node)))
                {
                    PipelineNodeDestroy(node, pipeline);
                    break;
                }

                lastNode = node;
                i++;
                nodeDef = va_arg(va, RxNodeDefinition*);
            }

            va_end(va);

            if (i == numNodes)
            {
                if (firstIndex)
                {
                    *firstIndex = firstNode;
                }

                return pipeline;
            }

            while (i--)
            {
                PipelineNodeDestroy(&pipeline->nodes[i + firstNode], pipeline);
            }
        }
    }
    else if (!pipeline)
    {
        RwError err;

        err.pluginID = 1;
        err.errorCode = _rwerror(0x80000016);
        RwErrorSet(&err);
    }
    else
    {
        RwError err;

        err.pluginID = 1;
        err.errorCode = _rwerror(0x34);
        RwErrorSet(&err);
    }

    return NULL;
}

static inline U32 PipelineNodeIndex(RxPipeline* pipeline, RxPipelineNode* node)
{
    U32 nodeIndex = node - pipeline->nodes;

    if (&pipeline->nodes[nodeIndex] == node && nodeIndex < pipeline->numNodes)
    {
        return nodeIndex;
    }

    return (U32)-1;
}

RxPipeline* RxLockedPipeAddPath(RxLockedPipe* pipeline, RxNodeOutput out, RxNodeInput in)
{
    if (pipeline && pipeline->locked && out && *out == (U32)-1 && in && in->nodeDef)
    {
        U32 nodeIndex = PipelineNodeIndex(pipeline, in);

        if (nodeIndex != (U32)-1)
        {
            *out = nodeIndex;

            return pipeline;
        }
    }

    return NULL;
}
