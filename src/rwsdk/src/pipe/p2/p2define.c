#include <rwcore.h>

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

RxNodeInput RxPipelineNodeFindInput(RxPipelineNode* node)
{
    if (node && node->nodeDef)
    {
        return node;
    }

    return NULL;
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

static U32 PipelineNodeIndex(RxPipeline* pipeline, RxPipelineNode* node)
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
