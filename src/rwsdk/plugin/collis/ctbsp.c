#include <rpcollis.h>

typedef struct nodeInfo nodeInfo;
struct nodeInfo
{
    U32 type;
    U32 index;
};

// Node types: a leaf is 1, a branch is 2. A branch node's own type field is the
// byte offset of its split axis inside an RwV3d.
#define LEAF_NODE 1
#define AXIS_COMPONENT(vec, axis) (*(F32*)((U8*)&(vec) + (axis)))

RpCollBSPTree* _rpCollBSPTreeInit(RpCollBSPTree* tree, S32 numLeafNodes)
{
    S32 numBranchNodes = numLeafNodes - 1;

    tree->numLeafNodes = numLeafNodes;

    if (numBranchNodes > 0)
    {
        tree->branchNodes = (RpCollBSPBranchNode*)(tree + 1);
        tree->leafNodes = (RpCollBSPLeafNode*)(tree->branchNodes + numBranchNodes);
    }
    else
    {
        tree->branchNodes = NULL;
        tree->leafNodes = (RpCollBSPLeafNode*)(tree + 1);
    }

    return tree;
}

S32 _rpCollBSPTreeMemGetSize(S32 numLeafNodes)
{
    return sizeof(RpCollBSPTree) + (numLeafNodes - 1) * sizeof(RpCollBSPBranchNode) +
           numLeafNodes * sizeof(RpCollBSPLeafNode);
}

void _rpCollBSPTreeDestroy(RpCollBSPTree* tree)
{
    if (tree)
    {
        RwFree(tree);
    }
}

RpCollBSPTree* _rpCollBSPTreeStreamWrite(RpCollBSPTree* tree, RwStream* stream)
{
    U32 count = tree->numLeafNodes - 1;
    RpCollBSPBranchNode* branch = tree->branchNodes;
    RpCollBSPLeafNode* leaf;

    while (count--)
    {
        U32 types = ((U32)branch->type << 16) | (branch->leftType << 8) | branch->rightType;
        U32 nodes = ((U32)branch->leftNode << 16) | branch->rightNode;

        if (!RwStreamWriteInt32(stream, (S32*)&types, sizeof(types)) ||
            !RwStreamWriteInt32(stream, (S32*)&nodes, sizeof(nodes)) ||
            !RwStreamWriteReal(stream, &branch->leftValue, sizeof(branch->leftValue)) ||
            !RwStreamWriteReal(stream, &branch->rightValue, sizeof(branch->rightValue)))
        {
            return NULL;
        }

        branch++;
    }

    count = tree->numLeafNodes;
    leaf = tree->leafNodes;

    while (count--)
    {
        U32 polygons = ((U32)leaf->numPolygons << 16) | leaf->firstPolygon;

        if (!RwStreamWriteInt32(stream, (S32*)&polygons, sizeof(polygons)))
        {
            return NULL;
        }

        leaf++;
    }

    return tree;
}

RpCollBSPTree* _rpCollBSPTreeStreamRead(RpCollBSPTree* tree, RwStream* stream)
{
    U32 count = tree->numLeafNodes - 1;
    RpCollBSPBranchNode* branch = tree->branchNodes;
    RpCollBSPLeafNode* leaf;

    while (count--)
    {
        U32 types;
        U32 nodes;

        if (!RwStreamReadInt32(stream, (S32*)&types, sizeof(types)) ||
            !RwStreamReadInt32(stream, (S32*)&nodes, sizeof(nodes)) ||
            !RwStreamReadReal(stream, &branch->leftValue, sizeof(branch->leftValue)) ||
            !RwStreamReadReal(stream, &branch->rightValue, sizeof(branch->rightValue)))
        {
            return NULL;
        }

        branch->type = types >> 16;
        branch->leftType = types >> 8;
        branch->rightType = types;
        branch->leftNode = nodes >> 16;
        branch->rightNode = nodes;

        branch++;
    }

    count = tree->numLeafNodes;
    leaf = tree->leafNodes;

    while (count--)
    {
        U32 polygons;

        if (!RwStreamReadInt32(stream, (S32*)&polygons, sizeof(polygons)))
        {
            _rpCollBSPTreeDestroy(tree);
            return NULL;
        }

        leaf->numPolygons = polygons >> 16;
        leaf->firstPolygon = polygons;

        leaf++;
    }

    return tree;
}

S32 _rpCollBSPTreeStreamGetSize(RpCollBSPTree* tree)
{
    return (tree->numLeafNodes - 1) * sizeof(RpCollBSPBranchNode) +
           tree->numLeafNodes * sizeof(RpCollBSPLeafNode);
}

// Point where the line crosses the plane at value along the split axis, stepping
// the other two coordinates by the line's gradients.
#define LINE_PLANE_POINT(point, line, grad, axis, value)          \
    {                                                             \
        F32 delta;                                                \
                                                                  \
        switch (axis)                                             \
        {                                                         \
        case 0:                                                   \
            delta = (value) - (line).start.x;                     \
            (point).x = (value);                                  \
            (point).y = (line).start.y + (grad)->dydx * delta;    \
            (point).z = (line).start.z + (grad)->dzdx * delta;    \
            break;                                                \
        case 4:                                                   \
            delta = (value) - (line).start.y;                     \
            (point).y = (value);                                  \
            (point).x = (line).start.x + (grad)->dxdy * delta;    \
            (point).z = (line).start.z + (grad)->dzdy * delta;    \
            break;                                                \
        case 8:                                                   \
            delta = (value) - (line).start.z;                     \
            (point).z = (value);                                  \
            (point).x = (line).start.x + (grad)->dxdz * delta;    \
            (point).y = (line).start.y + (grad)->dydz * delta;    \
            break;                                                \
        }                                                         \
    }

// Visit every leaf the line passes through, splitting the line at each plane it crosses.
RpCollBSPTree* _rpCollBSPTreeForAllLineLeafNodeIntersections(RpCollBSPTree* tree, RwLine* line,
                                                             RpV3dGradient* grad,
                                                             S32 (*callBack)(S32, S32, void*),
                                                             void* data)
{
    RwLine lineStack[33];
    nodeInfo nodeStack[33];
    RwLine currLine;
    nodeInfo node;
    S32 nStack = 0;
    RpCollBSPBranchNode* branch;
    RpCollBSPLeafNode* leaf;

    node.type = (tree->branchNodes != NULL) + LEAF_NODE;
    node.index = 0;
    currLine = *line;

    while (nStack >= 0)
    {
        if (node.type == LEAF_NODE)
        {
            leaf = &tree->leafNodes[node.index];

            if (!callBack(leaf->numPolygons, leaf->firstPolygon, data))
            {
                return NULL;
            }

            node = nodeStack[nStack];
            currLine = lineStack[nStack];
            nStack--;
        }
        else
        {
            RwSplitBits lStart;
            RwSplitBits lEnd;
            RwSplitBits rStart;
            RwSplitBits rEnd;
            U32 leftType;
            U32 rightType;
            U32 leftNode;
            U32 rightNode;

            branch = &tree->branchNodes[node.index];
            leftType = branch->leftType;
            rightType = branch->rightType;
            leftNode = branch->leftNode;
            rightNode = branch->rightNode;

            lStart.nReal = AXIS_COMPONENT(currLine.start, branch->type) - branch->leftValue;
            lEnd.nReal = AXIS_COMPONENT(currLine.end, branch->type) - branch->leftValue;
            rStart.nReal = AXIS_COMPONENT(currLine.start, branch->type) - branch->rightValue;
            rEnd.nReal = AXIS_COMPONENT(currLine.end, branch->type) - branch->rightValue;

            if (rStart.nInt < 0 && rEnd.nInt < 0)
            {
                node.type = leftType;
                node.index = leftNode;
            }
            else if (lStart.nInt >= 0 && lEnd.nInt >= 0)
            {
                node.type = rightType;
                node.index = rightNode;
            }
            else if (!((lStart.nInt ^ lEnd.nInt) & 0x80000000) &&
                     !((rStart.nInt ^ rEnd.nInt) & 0x80000000))
            {
                // The line lies in the overlap of both sides.
                if (rStart.nInt < rEnd.nInt)
                {
                    nStack++;
                    lineStack[nStack] = currLine;
                    nodeStack[nStack].type = rightType;
                    nodeStack[nStack].index = rightNode;
                    node.type = leftType;
                    node.index = leftNode;
                }
                else
                {
                    nStack++;
                    lineStack[nStack] = currLine;
                    nodeStack[nStack].type = leftType;
                    nodeStack[nStack].index = leftNode;
                    node.type = rightType;
                    node.index = rightNode;
                }
            }
            else if (((lStart.nInt ^ lEnd.nInt) & 0x80000000) && rStart.nInt >= 0 &&
                     rEnd.nInt >= 0)
            {
                // Only the left side's plane is crossed.
                RwV3d vLeft;

                LINE_PLANE_POINT(vLeft, currLine, grad, branch->type, branch->leftValue);

                if (lStart.nInt < 0)
                {
                    nStack++;
                    lineStack[nStack] = currLine;
                    nodeStack[nStack].type = rightType;
                    nodeStack[nStack].index = rightNode;
                    node.type = leftType;
                    node.index = leftNode;
                    currLine.end = vLeft;
                }
                else
                {
                    nStack++;
                    lineStack[nStack].start = vLeft;
                    lineStack[nStack].end = currLine.end;
                    nodeStack[nStack].type = leftType;
                    nodeStack[nStack].index = leftNode;
                    node.type = rightType;
                    node.index = rightNode;
                }
            }
            else if (((rStart.nInt ^ rEnd.nInt) & 0x80000000) && lStart.nInt < 0 &&
                     lEnd.nInt < 0)
            {
                // Only the right side's plane is crossed.
                RwV3d vRight;

                LINE_PLANE_POINT(vRight, currLine, grad, branch->type, branch->rightValue);

                if (rStart.nInt < 0)
                {
                    nStack++;
                    lineStack[nStack].start = vRight;
                    lineStack[nStack].end = currLine.end;
                    nodeStack[nStack].type = rightType;
                    nodeStack[nStack].index = rightNode;
                    node.type = leftType;
                    node.index = leftNode;
                }
                else
                {
                    nStack++;
                    lineStack[nStack] = currLine;
                    nodeStack[nStack].type = leftType;
                    nodeStack[nStack].index = leftNode;
                    node.type = rightType;
                    node.index = rightNode;
                    currLine.end = vRight;
                }
            }
            else
            {
                // Both planes are crossed.
                RwV3d vLeft;
                RwV3d vRight;

                LINE_PLANE_POINT(vLeft, currLine, grad, branch->type, branch->leftValue);
                LINE_PLANE_POINT(vRight, currLine, grad, branch->type, branch->rightValue);

                if (lStart.nInt < 0)
                {
                    nStack++;
                    lineStack[nStack].start = vRight;
                    lineStack[nStack].end = currLine.end;
                    nodeStack[nStack].type = rightType;
                    nodeStack[nStack].index = rightNode;
                    node.type = leftType;
                    node.index = leftNode;
                    currLine.end = vLeft;
                }
                else
                {
                    nStack++;
                    lineStack[nStack].start = vLeft;
                    lineStack[nStack].end = currLine.end;
                    nodeStack[nStack].type = leftType;
                    nodeStack[nStack].index = leftNode;
                    node.type = rightType;
                    node.index = rightNode;
                    currLine.end = vRight;
                }
            }
        }
    }

    return tree;
}

// Visit every leaf whose region overlaps the box, depth first with an explicit stack.
RpCollBSPTree* _rpCollBSPTreeForAllBoxLeafNodeIntersections(RpCollBSPTree* tree, RwBBox* box,
                                                            S32 (*callBack)(S32, S32, void*),
                                                            void* data)
{
    nodeInfo nodeStack[33];
    nodeInfo node;
    S32 nStack = 0;

    node.index = 0;
    node.type = (tree->branchNodes != NULL) + LEAF_NODE;

    while (nStack >= 0)
    {
        if (node.type == LEAF_NODE)
        {
            RpCollBSPLeafNode* leaf = &tree->leafNodes[node.index];

            if (!callBack(leaf->numPolygons, leaf->firstPolygon, data))
            {
                return NULL;
            }

            node = nodeStack[nStack];
            nStack--;
        }
        else
        {
            RpCollBSPBranchNode* branch = &tree->branchNodes[node.index];

            if (AXIS_COMPONENT(box->inf, branch->type) < branch->leftValue)
            {
                node.type = branch->leftType;
                node.index = branch->leftNode;

                if (AXIS_COMPONENT(box->sup, branch->type) >= branch->rightValue)
                {
                    nStack++;
                    nodeStack[nStack].index = branch->rightNode;
                    nodeStack[nStack].type = branch->rightType;
                }
            }
            else
            {
                node.type = branch->rightType;
                node.index = branch->rightNode;
            }
        }
    }

    return tree;
}
