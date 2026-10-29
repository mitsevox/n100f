#include <rwcore.h>
#include <string.h>

// Works out, when a pipeline is unlocked, which clusters each node needs and which of the
// packet's cluster slots each of them uses.

#define REQGETENTRY(req, index) (((index) < (req)->numEntries) ? &(req)->entries[index] : NULL)

static rxReq* _ReqCreate(RxPipelineNode* node, U32 maxEntries)
{
    rxReq* req;

    req = (rxReq*)StalacTiteAlloc(sizeof(rxReq));

    if (req)
    {
        req->entries = (rxReqEntry*)StalacTiteAlloc(maxEntries * sizeof(rxReqEntry));

        if (req->entries)
        {
            req->maxEntries = maxEntries;
            req->numEntries = 0;
            req->node = node;
            req->usedSlots = 0;
            return req;
        }
    }

    return NULL;
}

static rxReqEntry* _ReqSearch4Cluster(rxReq* req, RxClusterDefinition* clusterDef)
{
    if (req->numEntries)
    {
        rxReqEntry* entry;
        U32 i;

        entry = req->entries;

        for (i = 0; i < req->numEntries; i++, entry++)
        {
            if (entry->clusterDef == clusterDef)
            {
                return entry;
            }
        }
    }

    return NULL;
}

static rxReqEntry* _ReqAddEntry(rxReq* req, RxClusterDefinition* clusterDef,
                                RxClusterValidityReq rqdOrOpt, U32 numRefs, RxPipelineNode* node)
{
    rxReqEntry* entry;

    entry = _ReqSearch4Cluster(req, clusterDef);

    if (entry)
    {
        if (rqdOrOpt == rxCLREQ_REQUIRED)
        {
            entry->rqdOrOpt = rxCLREQ_REQUIRED;
            entry->node = node;
        }

        return entry;
    }

    entry = &req->entries[req->numEntries++];
    entry->clusterDef = clusterDef;
    entry->rqdOrOpt = rqdOrOpt;
    entry->numRefs = numRefs;
    entry->nextInScope = NULL;
    entry->req = req;
    entry->scope = NULL;
    entry->outputMask = 0;
    entry->slot = (U32)-1;
    entry->node = node;
    return entry;
}

static void _ReqDeleteEntry(rxReq* req, rxReqEntry* entry)
{
    if (entry != &req->entries[req->numEntries - 1])
    {
        *entry = req->entries[req->numEntries - 1];
    }

    req->numEntries--;
}

static U32 _IoSpecSearch4Cluster(RxIoSpec* io, RxClusterDefinition* clusterDef)
{
    U32 i;

    for (i = 0; i < io->numClustersOfInterest; i++)
    {
        if (io->clustersOfInterest[i].clusterDef == clusterDef)
        {
            return i;
        }
    }

    return (U32)-1;
}

// How an output leaves a cluster: by the node's entry for it, or by the output's default.
#define OUTPUTGETVALIDITY(nodeDef, outputSpec, clusterDef, valid)                                  \
    do                                                                                             \
    {                                                                                              \
        U32 _index = _IoSpecSearch4Cluster(&(nodeDef)->io, (clusterDef));                          \
                                                                                                   \
        (valid) = (_index == (U32)-1) ? (outputSpec)->allOtherClusters                             \
                                      : (outputSpec)->outputClusters[_index];                      \
    } while (0)

// Drops one reference to the cluster from the node and, once no input passes it in any more,
// from every node after it that receives it unchanged.
static void _PropDownElimPath(RxPipeline* pipeline, RxPipelineNode* node,
                              RxClusterDefinition* clusterDef)
{
    rxReqEntry* entry;
    RxNodeDefinition* nodeDef;

    nodeDef = node->nodeDef;
    entry = _ReqSearch4Cluster(node->topSortData->req, clusterDef);

    if (entry && --entry->numRefs == 0)
    {
        U32 i;

        _ReqDeleteEntry(node->topSortData->req, entry);

        for (i = 0; i < node->numOutputs; i++)
        {
            if (node->outputs[i] != (U32)-1)
            {
                RxOutputSpec* outputSpec;
                RxClusterValid valid;

                outputSpec = &node->nodeDef->io.outputs[i];
                OUTPUTGETVALIDITY(nodeDef, outputSpec, clusterDef, valid);

                if (valid == rxCLVALID_NOCHANGE)
                {
                    _PropDownElimPath(pipeline, &pipeline->nodes[node->outputs[i]], clusterDef);
                }
            }
        }
    }
}

static rxScope* _ScopeTraceCreate(rxScope** scopeList)
{
    rxScope* scope;

    scope = (rxScope*)StalacTiteAlloc(sizeof(rxScope));

    if (scope)
    {
        scope->next = NULL;
        scope->entries = NULL;
        scope->parent = NULL;
        scope->nextInList = *scopeList;
        *scopeList = scope;
        return scope;
    }

    return NULL;
}

static void _ScopeTraceMerge(rxScope** scopeList, rxScope* scope0, rxScope* scope1)
{
    while (scope0->parent)
    {
        scope0 = scope0->parent;
    }

    while (scope1->parent)
    {
        scope1 = scope1->parent;
    }

    if (scope0 != scope1)
    {
        rxScope* last;

        last = scope0;

        while (last->next)
        {
            last = last->next;
        }

        last->next = scope1;
        scope1->parent = scope0;

        while (*scopeList != scope1)
        {
            scopeList = &(*scopeList)->nextInList;
        }

        *scopeList = scope1->nextInList;
    }
}

// Works back from the last node, giving each node the clusters it needs and the ones the nodes
// after it need passed through, and removes the clusters an output invalidates from the nodes it
// feeds.
static U32 _PropagateDependenciesAndKillDeadPaths(RxPipeline* pipeline)
{
    RxPipelineNode* node;
    U32 n;

    n = pipeline->numNodes;
    node = &pipeline->nodes[n - 1];

    do
    {
        RxNodeDefinition* nodeDef;
        U32 i;

        nodeDef = node->nodeDef;

        for (i = 0; i < nodeDef->io.numClustersOfInterest; i++)
        {
            U32 j;
            RxClusterDefinition* clusterDef;

            clusterDef = nodeDef->io.clustersOfInterest[i].clusterDef;

            if (!clusterDef)
            {
                RwError err;

                err.pluginID = 1;
                err.errorCode = _rwerror(0x1F, node->nodeDef->name);
                RwErrorSet(&err);
                return 0x1F;
            }

            for (j = i + 1; j < nodeDef->io.numClustersOfInterest; j++)
            {
                if (clusterDef == nodeDef->io.clustersOfInterest[j].clusterDef)
                {
                    RwError err;

                    err.pluginID = 1;
                    err.errorCode = _rwerror(0x1E, node->nodeDef->name, clusterDef->name);
                    RwErrorSet(&err);
                    return 0x1E;
                }
            }
        }

        if (!(node->topSortData->req = _ReqCreate(node, PipelineCalcNumUniqueClusters(pipeline))))
        {
            RwError err;

            err.pluginID = 1;
            err.errorCode = _rwerror(0x20);
            RwErrorSet(&err);
            return 0x20;
        }

        for (i = 0; i < node->numOutputs; i++)
        {
            if (node->outputs[i] != (U32)-1)
            {
                U32 j;
                RxPipelineNode* next;
                RxOutputSpec* outputSpec;

                next = &pipeline->nodes[node->outputs[i]];
                outputSpec = &node->nodeDef->io.outputs[i];

                for (j = 0; j < next->topSortData->req->numEntries; j++)
                {
                    rxReqEntry* nextEntry;
                    RxClusterValid valid;

                    nextEntry = REQGETENTRY(next->topSortData->req, j);
                    OUTPUTGETVALIDITY(nodeDef, outputSpec, nextEntry->clusterDef, valid);

                    if (valid == rxCLVALID_NOCHANGE)
                    {
                        if (!_ReqAddEntry(node->topSortData->req, nextEntry->clusterDef,
                                          nextEntry->rqdOrOpt, node->topSortData->numIns,
                                          nextEntry->node))
                        {
                            RwError err;

                            err.pluginID = 1;
                            err.errorCode = _rwerror(0x20);
                            RwErrorSet(&err);
                            return 0x20;
                        }
                    }
                    else if (nextEntry->rqdOrOpt == rxCLREQ_REQUIRED)
                    {
                        if (valid != rxCLVALID_VALID)
                        {
                            RwError err;

                            err.pluginID = 1;
                            err.errorCode =
                                _rwerror(0x1D, nextEntry->clusterDef->name,
                                         nextEntry->node->nodeDef->name, node->nodeDef->name, i,
                                         outputSpec->name);
                            RwErrorSet(&err);
                            return 0x1D;
                        }
                    }
                    else if (valid == rxCLVALID_INVALID)
                    {
                        _PropDownElimPath(pipeline, next, nextEntry->clusterDef);
                    }
                }
            }
        }

        for (i = 0; i < nodeDef->io.numClustersOfInterest; i++)
        {
            RxClusterValidityReq rqdOrOpt;

            rqdOrOpt = nodeDef->io.inputRequirements[i];

            if (rqdOrOpt != rxCLREQ_DONTWANT)
            {
                if (!_ReqAddEntry(node->topSortData->req, nodeDef->io.clustersOfInterest[i].clusterDef,
                                  rqdOrOpt, node->topSortData->numIns, node))
                {
                    RwError err;

                    err.pluginID = 1;
                    err.errorCode = _rwerror(0x20);
                    RwErrorSet(&err);
                    return 0x20;
                }
            }
        }

        node--;
    } while (--n);

    return 0;
}

// Adds the clusters each node creates or passes on as valid, and marks every entry with the
// outputs that carry its cluster on.
static U32 _ForAllNodeReqsAddOutputClustersAndBuildContinuityBitfields(RxPipeline* pipeline)
{
    RxPipelineNode* node;
    U32 n;

    n = pipeline->numNodes;
    node = pipeline->nodes;

    do
    {
        U32 i;
        RxNodeDefinition* nodeDef;

        nodeDef = node->nodeDef;

        for (i = 0; i < nodeDef->io.numClustersOfInterest; i++)
        {
            if (nodeDef->io.clustersOfInterest[i].forcePresent)
            {
                if (!_ReqAddEntry(node->topSortData->req, nodeDef->io.clustersOfInterest[i].clusterDef,
                                  rxCLREQ_DONTWANT, 1, node))
                {
                    RwError err;

                    err.pluginID = 1;
                    err.errorCode = _rwerror(0x20);
                    RwErrorSet(&err);
                    return 0x20;
                }
            }
        }

        for (i = 0; i < node->numOutputs; i++)
        {
            if (node->outputs[i] != (U32)-1)
            {
                U32 j;
                RxPipelineNode* next;
                RxOutputSpec* outputSpec;

                next = &pipeline->nodes[node->outputs[i]];
                outputSpec = &node->nodeDef->io.outputs[i];

                for (j = 0; j < next->topSortData->req->numEntries; j++)
                {
                    rxReqEntry* nextEntry;
                    RxClusterValid valid;

                    nextEntry = REQGETENTRY(next->topSortData->req, j);
                    OUTPUTGETVALIDITY(nodeDef, outputSpec, nextEntry->clusterDef, valid);

                    if (valid != rxCLVALID_INVALID)
                    {
                        rxReqEntry* entry;

                        if (valid != rxCLVALID_NOCHANGE)
                        {
                            entry = _ReqAddEntry(node->topSortData->req, nextEntry->clusterDef,
                                                 rxCLREQ_DONTWANT, 1, node);

                            if (!entry)
                            {
                                RwError err;

                                err.pluginID = 1;
                                err.errorCode = _rwerror(0x20);
                                RwErrorSet(&err);
                                return 0x20;
                            }
                        }
                        else
                        {
                            entry = _ReqSearch4Cluster(node->topSortData->req, nextEntry->clusterDef);
                        }

                        if (entry)
                        {
                            entry->outputMask |= 1 << i;
                        }
                    }
                }
            }
        }

        node++;
    } while (--n);

    return 0;
}

// Gives every entry a scope, joining the scopes of entries that pass a cluster from one node to
// the next.
static U32 _TraceClusterScopes(RxPipeline* pipeline, rxScope** scopeList)
{
    U32 n;
    RxPipelineNode* node;

    n = pipeline->numNodes;
    node = pipeline->nodes;

    do
    {
        U32 i;

        for (i = 0; i < node->topSortData->req->numEntries; i++)
        {
            rxReqEntry* entry;

            entry = REQGETENTRY(node->topSortData->req, i);

            if (!entry->scope)
            {
                if (!(entry->scope = _ScopeTraceCreate(scopeList)))
                {
                    RwError err;

                    err.pluginID = 1;
                    err.errorCode = _rwerror(0x20);
                    RwErrorSet(&err);
                    return 0x20;
                }

                entry->nextInScope = entry->scope->entries;
                entry->scope->entries = entry;
            }
        }

        for (i = 0; i < node->numOutputs; i++)
        {
            if (node->outputs[i] != (U32)-1)
            {
                U32 j;
                RxPipelineNode* next;

                next = &pipeline->nodes[node->outputs[i]];

                for (j = 0; j < next->topSortData->req->numEntries; j++)
                {
                    rxReqEntry* nextEntry;

                    nextEntry = REQGETENTRY(next->topSortData->req, j);

                    if (nextEntry->rqdOrOpt != rxCLREQ_DONTWANT)
                    {
                        rxReqEntry* entry;

                        entry = _ReqSearch4Cluster(node->topSortData->req, nextEntry->clusterDef);

                        if (entry && (entry->outputMask & (1 << i)))
                        {
                            if (!nextEntry->scope)
                            {
                                nextEntry->nextInScope = entry->scope->entries;
                                entry->scope->entries = nextEntry;
                                nextEntry->scope = entry->scope;
                            }
                            else
                            {
                                _ScopeTraceMerge(scopeList, nextEntry->scope, entry->scope);
                            }
                        }
                    }
                }
            }
        }

        node++;
    } while (--n);

    return 0;
}

// Gives each scope the lowest slot that none of its nodes uses yet.
static U32 _AssignClusterSlots(RxPipeline* pipeline, rxScope** scopeList)
{
    rxScope* scope;
    U32 numSlots;

    numSlots = 0;

    for (scope = *scopeList; scope; scope = scope->nextInList)
    {
        rxScope* merged;
        U32 slot;
        U32 usedSlots;
        rxReqEntry* entry;

        usedSlots = 0;
        merged = scope;

        do
        {
            for (entry = merged->entries; entry; entry = entry->nextInScope)
            {
                usedSlots |= entry->req->usedSlots;
            }

            merged = merged->next;
        } while (merged);

        slot = 0;

        while (usedSlots & 1)
        {
            usedSlots >>= 1;
            slot++;
        }

        if (slot >= numSlots)
        {
            numSlots = slot + 1;
        }

        merged = scope;

        do
        {
            for (entry = merged->entries; entry; entry = entry->nextInScope)
            {
                entry->slot = slot;
                entry->req->usedSlots |= 1 << slot;
            }

            merged = merged->next;
        } while (merged);
    }

    pipeline->packetNumClusterSlots = numSlots;
    return 0;
}

// Adds a pipeline cluster for the cluster to the array that starts at *data.
static void _MyEnumPipelineClustersCallBack(RxClusterDefinition* clusterDef, rxScope* scope,
                                            void* data)
{
    RxPipelineCluster* cluster;

    cluster = (RxPipelineCluster*)StalacMiteAlloc(sizeof(RxPipelineCluster));

    if (!*(RxPipelineCluster**)data)
    {
        *(RxPipelineCluster**)data = cluster;
    }

    cluster->clusterRef = clusterDef;
    cluster->creationAttributes = clusterDef->defaultAttributes;
}

// Calls the callback once for each different cluster in the scope list and returns how many
// there are.
static U32 _EnumPipelineClusters(rxScope* scopeList,
                                 void (*callback)(RxClusterDefinition*, rxScope*, void*),
                                 void* data)
{
    rxScope* scope;
    U32 numClusters;

    numClusters = 0;

    for (scope = scopeList; scope; scope = scope->nextInList)
    {
        rxScope* first;

        for (first = scopeList; first != scope; first = first->nextInList)
        {
            if (first->entries->clusterDef == scope->entries->clusterDef)
            {
                break;
            }
        }

        if (first == scope)
        {
            numClusters++;

            if (callback)
            {
                callback(scope->entries->clusterDef, scope, data);
            }
        }
    }

    return numClusters;
}

static U32 _CountHeadNodeRqdsAndOpts(RxPipeline* pipeline)
{
    rxReq* req;
    U32 numRqdsAndOpts;
    U32 i;

    numRqdsAndOpts = 0;
    req = pipeline->nodes[0].topSortData->req;

    for (i = 0; i < req->numEntries; i++)
    {
        rxReqEntry* entry;

        entry = REQGETENTRY(req, i);

        if (entry->rqdOrOpt == rxCLREQ_REQUIRED || entry->rqdOrOpt == rxCLREQ_OPTIONAL)
        {
            numRqdsAndOpts++;
        }
    }

    return numRqdsAndOpts;
}

static void _WriteHeadNodeRqdsAndOpts2PipelineRequirements(RxPipeline* pipeline, U32 numRqdsAndOpts)
{
    U32 numRequirements;
    U32 i;

    if (numRqdsAndOpts)
    {
        pipeline->inputRequirements = (RxPipelineRequiresCluster*)StalacMiteAlloc(
            numRqdsAndOpts * sizeof(RxPipelineRequiresCluster));
    }

    numRequirements = 0;

    for (i = 0; i < pipeline->nodes[0].topSortData->req->numEntries; i++)
    {
        rxReqEntry* entry;

        entry = REQGETENTRY(pipeline->nodes[0].topSortData->req, i);

        if (entry->rqdOrOpt == rxCLREQ_REQUIRED || entry->rqdOrOpt == rxCLREQ_OPTIONAL)
        {
            RxPipelineRequiresCluster* requirement;

            requirement = &pipeline->inputRequirements[numRequirements++];
            requirement->clusterDef = entry->clusterDef;
            requirement->rqdOrOpt = entry->rqdOrOpt;
            requirement->slotIndex = entry->slot;
        }
    }

    pipeline->numInputRequirements = numRequirements;
    _rx_rxRadixExchangeSort(pipeline->inputRequirements, pipeline->numInputRequirements,
                            sizeof(RxPipelineRequiresCluster), 0, 0, (U32)-1);
}

// Lays out the unlocked pipeline's data after its nodes: the packet it executes with, its
// clusters, each node's slot tables and private data, and the clusters its input supplies.
static U32 _ForAllNodesWriteClusterAllocations(RxPipeline* pipeline, rxScope* scopeList)
{
    RxPipelineCluster* clusters;
    U32 numRqdsAndOpts;
    U32 numClusters;
    U8* top;
    U8* bottom;
    U32 i;

    numRqdsAndOpts = _CountHeadNodeRqdsAndOpts(pipeline);
    numClusters = _EnumPipelineClusters(scopeList, NULL, NULL);

    top = (U8*)StalacTiteAlloc(0);
    bottom = (U8*)StalacMiteAlloc(0);
    memset(bottom, 0, top - bottom);

    pipeline->embeddedPacket = (RxPacket*)StalacMiteAlloc(
        sizeof(RxPacket) + (pipeline->packetNumClusterSlots - 1) * sizeof(RxCluster));
    clusters = NULL;
    pipeline->embeddedPacket->numClusters = pipeline->packetNumClusterSlots;
    pipeline->embeddedPacket->pipeline = pipeline;
    pipeline->embeddedPacketState = rxPKST_PACKETLESS;

    _EnumPipelineClusters(scopeList, _MyEnumPipelineClustersCallBack, &clusters);

    for (i = 0; i < pipeline->numNodes; i++)
    {
        U32 j;
        U32 continueMask;
        RxPipelineNode* node;

        node = &pipeline->nodes[i];

        if (pipeline->packetNumClusterSlots)
        {
            node->slotClusterRefs = (RxPipelineCluster**)StalacMiteAlloc(
                pipeline->packetNumClusterSlots * sizeof(RxPipelineCluster*));
        }

        node->slotsContinue =
            (U32*)StalacMiteAlloc((pipeline->packetNumClusterSlots + 1) * sizeof(U32));

        if (node->nodeDef->io.numClustersOfInterest)
        {
            node->inputToClusterSlot =
                (U32*)StalacMiteAlloc(node->nodeDef->io.numClustersOfInterest * sizeof(U32));
        }

        if (node->nodeDef->pipelineNodePrivateDataSize)
        {
            node->privateData =
                StalacMiteAlloc((node->nodeDef->pipelineNodePrivateDataSize + 3) & ~3);
        }

        continueMask = (U32)-1;

        for (j = 0; j < node->topSortData->req->numEntries; j++)
        {
            rxReqEntry* entry;
            U32 k;
            RxPipelineCluster* cluster;

            entry = REQGETENTRY(node->topSortData->req, j);
            cluster = NULL;

            for (k = 0; k < numClusters; k++)
            {
                if (clusters[k].clusterRef == entry->clusterDef)
                {
                    cluster = &clusters[k];
                    break;
                }
            }

            node->slotClusterRefs[entry->slot] = cluster;
            node->slotsContinue[entry->slot + 1] = entry->outputMask;
            continueMask &= entry->outputMask;
        }

        node->slotsContinue[0] = continueMask;

        for (j = 0; j < node->nodeDef->io.numClustersOfInterest; j++)
        {
            U32 k;

            node->inputToClusterSlot[j] = (U32)-1;

            for (k = 0; k < pipeline->packetNumClusterSlots; k++)
            {
                if (node->slotClusterRefs[k]
                    && node->slotClusterRefs[k]->clusterRef
                           == node->nodeDef->io.clustersOfInterest[j].clusterDef)
                {
                    node->inputToClusterSlot[j] = k;
                    break;
                }
            }
        }
    }

    _WriteHeadNodeRqdsAndOpts2PipelineRequirements(pipeline, numRqdsAndOpts);
    return 0;
}

U32 _rxChaseDependencies(RxPipeline* pipeline)
{
    rxScope* scopeList;
    U32 result;

    scopeList = NULL;

    result = _PropagateDependenciesAndKillDeadPaths(pipeline);

    if (!result)
    {
        result = _ForAllNodeReqsAddOutputClustersAndBuildContinuityBitfields(pipeline);

        if (!result)
        {
            result = _TraceClusterScopes(pipeline, &scopeList);

            if (!result)
            {
                result = _AssignClusterSlots(pipeline, &scopeList);

                if (!result)
                {
                    result = _ForAllNodesWriteClusterAllocations(pipeline, scopeList);
                }
            }
        }
    }

    return result;
}
