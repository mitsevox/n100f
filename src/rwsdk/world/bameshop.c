#include <rpworld.h>
#include <stdlib.h>
#include <string.h>

// The mesh optimiser: sorts a build mesh's triangles by material and turns them into a
// triangle list mesh header or, by default, a triangle strip one.

// The vertex index a triangle list mesh marks a placed triangle with.
#define TRILISTUSED 0xFFFF

// The passes over each starting triangle: the last builds the strip for keeps, the ones
// before it only try a starting edge.
#define TRISTRIPNUMPASSES 4

// The sort key flag of a material that needs blending.
static const U32 SortTransparent = 16;

// Builds a mesh header from a build mesh; the mesh optimiser's triangle strip method.
typedef RpMeshHeader* (*TriStripMethod)(RpBuildMesh* buildMesh, void* data);

typedef struct TriStripBinEntry TriStripBinEntry;
typedef struct TriStripEdge TriStripEdge;

// A triangle and its three edges, kept in the bin of its number of free neighbours.
struct TriStripBinEntry
{
    U32 triIndex;
    TriStripEdge* edge[3];
    TriStripBinEntry* next;
    TriStripBinEntry* prev;
    S32 used;
    S32 trialUsed;
    U8 numNeighbours;
};

// An edge between two vertices and the one or two triangles on it.
struct TriStripEdge
{
    U16 vert[2];
    TriStripBinEntry* tri[2];
    TriStripEdge* next;
};

typedef struct TriStripFreeLists TriStripFreeLists;
struct TriStripFreeLists
{
    RwFreeList* binEntryFreeList;
    RwFreeList* edgeFreeList;
};

// A strip of vertex indices with room for space indices.
typedef struct TriStripList TriStripList;
struct TriStripList
{
    U16* indices;
    U32 numIndices;
    U32 space;
    TriStripList* next;
};

typedef struct MeshOpStatic MeshOpStatic;
struct MeshOpStatic
{
    RpMeshHeader emptyMeshHeader;
    TriStripMethod triStripMethod;
    void* triStripData;
};

static MeshOpStatic MeshOp = { { 0, 0, 0, 0, 0 }, RpBuildMeshGenerateDefaultTriStrip, NULL };

#define RwFreeListAlloc(_fl) (RWSRCGLOBAL(memoryAlloc)(_fl))
#define RwFreeListFree(_fl, _p) (RWSRCGLOBAL(memoryFree)((_fl), (_p)))

// The number of triangles on an edge that a pass has not used yet.
#define TRISTRIPEDGEFREE(_edge, _flag)                                                             \
    (((_edge)->tri[1] && !(_edge)->tri[1]->_flag) + ((_edge)->tri[0] && !(_edge)->tri[0]->_flag))

// Takes a triangle out of its bin.
#define TRISTRIPBINREMOVE(_bins, _entry)                                                           \
    do                                                                                             \
    {                                                                                              \
        if ((_bins)[(_entry)->numNeighbours] == (_entry))                                          \
        {                                                                                          \
            (_bins)[(_entry)->numNeighbours] = (_bins)[(_entry)->numNeighbours]->next;             \
            if ((_bins)[(_entry)->numNeighbours])                                                  \
            {                                                                                      \
                (_bins)[(_entry)->numNeighbours]->prev = NULL;                                     \
            }                                                                                      \
        }                                                                                          \
        else                                                                                       \
        {                                                                                          \
            if ((_entry)->next)                                                                    \
            {                                                                                      \
                (_entry)->next->prev = (_entry)->prev;                                             \
            }                                                                                      \
            if ((_entry)->prev)                                                                    \
            {                                                                                      \
                (_entry)->prev->next = (_entry)->next;                                             \
            }                                                                                      \
        }                                                                                          \
    } while (0)

// Puts a triangle at the head of its bin.
#define TRISTRIPBININSERT(_bins, _entry)                                                           \
    do                                                                                             \
    {                                                                                              \
        (_entry)->next = (_bins)[(_entry)->numNeighbours];                                         \
        if ((_entry)->next)                                                                        \
        {                                                                                          \
            (_entry)->next->prev = (_entry);                                                       \
        }                                                                                          \
        (_bins)[(_entry)->numNeighbours] = (_entry);                                               \
        (_entry)->prev = NULL;                                                                     \
    } while (0)

// Orders triangles by blending, then texture raster, pipeline, texture and material.
static S32 SortPolygons(const void* pA, const void* pB)
{
    RpMaterial* matA = (*(RpBuildMeshTriangle**)pA)->material;
    RpMaterial* matB = (*(RpBuildMeshTriangle**)pB)->material;
    RwRaster* rasA = NULL;
    RwRaster* rasB = NULL;
    RwTexture* texA = NULL;
    RwTexture* texB = NULL;
    RxPipeline* pipeA = NULL;
    RxPipeline* pipeB = NULL;
    U32 keyA = 0;
    U32 keyB = 0;

    if (matA == matB)
    {
        return 0;
    }

    if (matA)
    {
        if (matA->texture)
        {
            S32 format;

            texA = matA->texture;
            rasA = texA->raster;
            format = RwRasterGetFormat(rasA) & rwRASTERFORMATPIXELFORMATMASK;

            if (format == rwRASTERFORMAT1555 || format == rwRASTERFORMAT4444 ||
                format == rwRASTERFORMAT8888)
            {
                keyA |= SortTransparent;
            }
        }

        if (matA->color.alpha != 0xFF)
        {
            keyA |= SortTransparent;
        }

        pipeA = matA->pipeline;
    }

    if (matB)
    {
        if (matB->texture)
        {
            S32 format;

            texB = matB->texture;
            rasB = texB->raster;
            format = RwRasterGetFormat(rasB) & rwRASTERFORMATPIXELFORMATMASK;

            if (format == rwRASTERFORMAT1555 || format == rwRASTERFORMAT4444 ||
                format == rwRASTERFORMAT8888)
            {
                keyB |= SortTransparent;
            }
        }

        if (matB->color.alpha != 0xFF)
        {
            keyB |= SortTransparent;
        }

        pipeB = matB->pipeline;
    }

    keyA |= (rasA > rasB) ? 8 : 0;
    keyB |= (rasB > rasA) ? 8 : 0;
    keyA |= (pipeA > pipeB) ? 4 : 0;
    keyB |= (pipeB > pipeA) ? 4 : 0;
    keyA |= (texA > texB) ? 2 : 0;
    keyB |= (texB > texA) ? 2 : 0;
    keyA |= (matA > matB) ? 1 : 0;
    keyB |= (matB > matA) ? 1 : 0;

    return keyA - keyB;
}

// Reorders a triangle list mesh so that each run of triangles shares no vertex, taking the
// first free triangle each time.
static RpMesh* SortPolygonsInTriListMesh(RpMesh* mesh, RpMeshHeader* meshHeader, void* pData)
{
    U16* indices = mesh->indices;
    U32 numTris = mesh->numIndices / 3;
    U32 maxIndex = 0;
    U32 i;
    U32 size;
    U32* usedVerts;

    for (i = 0; i < mesh->numIndices; i++)
    {
        maxIndex |= indices[i];
    }

    size = (maxIndex + 31) >> 3;
    usedVerts = (U32*)RwMalloc(size);

    if (usedVerts)
    {
        U32 copySize = mesh->numIndices * sizeof(U16);
        U16* copy = (U16*)RwMalloc(copySize);

        if (copy)
        {
            U32 numPlaced;

            memcpy(copy, indices, copySize);

            numPlaced = 0;

            while (numPlaced < numTris)
            {
                U8* bytes = (U8*)usedVerts;
                U16* tri = copy;
                U32 j = size;

                while (j--)
                {
                    *bytes++ = 0;
                }

                for (j = 0; j < numTris; j++, tri += 3)
                {
                    if ((tri[0] & tri[1] & tri[2]) != TRILISTUSED &&
                        !(usedVerts[tri[0] >> 5] & (1 << (tri[0] & 31))) &&
                        !(usedVerts[tri[1] >> 5] & (1 << (tri[1] & 31))) &&
                        !(usedVerts[tri[2] >> 5] & (1 << (tri[2] & 31))))
                    {
                        usedVerts[tri[0] >> 5] |= 1 << (tri[0] & 31);
                        numPlaced++;
                        usedVerts[tri[1] >> 5] |= 1 << (tri[1] & 31);
                        usedVerts[tri[2] >> 5] |= 1 << (tri[2] & 31);

                        indices[0] = tri[0];
                        indices[1] = tri[1];
                        indices[2] = tri[2];
                        indices += 3;

                        tri[2] = TRILISTUSED;
                        tri[1] = TRILISTUSED;
                        tri[0] = TRILISTUSED;
                    }
                }
            }

            RwFree(copy);
        }

        RwFree(usedVerts);

        return mesh;
    }

    return mesh;
}

// Finds the edge from vert0 to vert1 among the edges so far, or adds it.
static TriStripEdge* TriStripAddEdge(TriStripFreeLists* freeLists, TriStripEdge** edgeList,
                                     TriStripBinEntry* entry, U16 vert0, U16 vert1)
{
    TriStripEdge* edge;

    for (edge = *edgeList; edge; edge = edge->next)
    {
        if (vert0 == edge->vert[1] && vert1 == edge->vert[0] && !edge->tri[1])
        {
            edge->tri[0]->numNeighbours++;
            entry->numNeighbours++;
            edge->tri[1] = entry;

            return edge;
        }
    }

    edge = (TriStripEdge*)RwFreeListAlloc(freeLists->edgeFreeList);
    edge->vert[0] = vert0;
    edge->vert[1] = vert1;
    edge->tri[0] = entry;
    edge->tri[1] = NULL;
    edge->next = *edgeList;
    *edgeList = edge;

    return edge;
}

// Makes a bin entry for each triangle and links the triangles that share an edge.
static TriStripBinEntry** TriStripBinEntryArrayCreate(U32 numTris, TriStripFreeLists* freeLists,
                                                      TriStripEdge** edgeList,
                                                      RpBuildMeshTriangle* triangles)
{
    TriStripBinEntry** entries = (TriStripBinEntry**)RwMalloc(numTris * sizeof(TriStripBinEntry*));
    U32 i;

    freeLists->binEntryFreeList = RwFreeListCreate(sizeof(TriStripBinEntry), numTris, 0);
    freeLists->edgeFreeList = RwFreeListCreate(sizeof(TriStripEdge), numTris / 2 + 1, 0);

    for (i = 0; i < numTris; i++, triangles++)
    {
        entries[i] = (TriStripBinEntry*)RwFreeListAlloc(freeLists->binEntryFreeList);
        entries[i]->numNeighbours = 0;
        entries[i]->triIndex = i;
        entries[i]->prev = NULL;
        entries[i]->next = NULL;
        entries[i]->used = FALSE;

        entries[i]->edge[0] = TriStripAddEdge(freeLists, edgeList, entries[i],
                                              triangles->vertIndex[0], triangles->vertIndex[1]);
        entries[i]->edge[1] = TriStripAddEdge(freeLists, edgeList, entries[i],
                                              triangles->vertIndex[1], triangles->vertIndex[2]);
        entries[i]->edge[2] = TriStripAddEdge(freeLists, edgeList, entries[i],
                                              triangles->vertIndex[2], triangles->vertIndex[0]);
    }

    return entries;
}

// Marks a triangle used. On the final pass it also leaves its bin, and its free neighbours
// move down a bin.
static void TriStripMarkTriUsed(TriStripBinEntry* entry, TriStripBinEntry** bins, S32 pass)
{
    U8 i;

    if (pass < TRISTRIPNUMPASSES)
    {
        entry->trialUsed = TRUE;
        return;
    }

    entry->used = TRUE;
    TRISTRIPBINREMOVE(bins, entry);

    for (i = 0; i < 3; i++)
    {
        TriStripEdge* edge = entry->edge[i];
        TriStripBinEntry* neighbour = NULL;

        if (edge->tri[0] && edge->tri[0] != entry && !edge->tri[0]->used)
        {
            neighbour = edge->tri[0];
        }
        else if (edge->tri[1] && !edge->tri[1]->used)
        {
            neighbour = edge->tri[1];
        }

        if (neighbour)
        {
            TRISTRIPBINREMOVE(bins, neighbour);
            neighbour->numNeighbours--;
            TRISTRIPBININSERT(bins, neighbour);
        }
    }
}

// Whether no triangle on an edge is free for the pass, so a strip crossing it ends.
static S32 TriStripEdgeIsDeadEnd(TriStripEdge* edge, S32 pass)
{
    if (pass < TRISTRIPNUMPASSES)
    {
        S32 deadEnd = TRUE;

        if (edge && TRISTRIPEDGEFREE(edge, trialUsed))
        {
            deadEnd = FALSE;
        }

        return deadEnd;
    }
    else
    {
        S32 deadEnd = TRUE;

        if (edge && TRISTRIPEDGEFREE(edge, used))
        {
            deadEnd = FALSE;
        }

        return deadEnd;
    }
}

// Whether a triangle on an edge is free for the pass.
static S32 TriStripEdgeHasFreeTri(TriStripEdge* edge, S32 pass)
{
    if (pass < TRISTRIPNUMPASSES)
    {
        S32 hasFree = FALSE;

        if (edge && TRISTRIPEDGEFREE(edge, trialUsed))
        {
            hasFree = TRUE;
        }

        return hasFree;
    }
    else
    {
        S32 hasFree = FALSE;

        if (edge && TRISTRIPEDGEFREE(edge, used))
        {
            hasFree = TRUE;
        }

        return hasFree;
    }
}

// Extends a strip across edge for as long as a free triangle lies beyond it, swapping to the
// third edge of a triangle when the strip would end there. Returns the triangles it adds.
static U32 TriStripFollow(TriStripList* strip, TriStripEdge* edge, TriStripBinEntry** bins,
                          RpBuildMeshTriangle* triangles, S32 pass)
{
    U32 numAdded = 0;
    TriStripEdge* swapEdge = NULL;

    while (edge)
    {
        TriStripBinEntry* tri = NULL;
        TriStripEdge* inEdge;
        U16 newVert = 0;
        U16 vertA;
        U16 vertB;
        U16 lastVert;
        U16* vertIndex;

        if (strip->numIndices & 1)
        {
            vertA = strip->indices[strip->numIndices - 1];
            vertB = strip->indices[strip->numIndices - 2];
        }
        else
        {
            vertA = strip->indices[strip->numIndices - 2];
            vertB = strip->indices[strip->numIndices - 1];
        }

        if (pass < TRISTRIPNUMPASSES)
        {
            if (edge->tri[0] && !edge->tri[0]->trialUsed)
            {
                tri = edge->tri[0];
            }
            else if (edge->tri[1] && !edge->tri[1]->trialUsed)
            {
                tri = edge->tri[1];
            }
        }
        else
        {
            if (edge->tri[0] && !edge->tri[0]->used)
            {
                tri = edge->tri[0];
            }
            else if (edge->tri[1] && !edge->tri[1]->used)
            {
                tri = edge->tri[1];
            }
        }

        if (!tri)
        {
            break;
        }

        numAdded++;
        TriStripMarkTriUsed(tri, bins, pass);

        vertIndex = triangles[tri->triIndex].vertIndex;

        if (vertA != vertIndex[0] && vertB != vertIndex[0])
        {
            newVert = vertIndex[0];
        }

        if (vertA != vertIndex[1] && vertB != vertIndex[1])
        {
            newVert = vertIndex[1];
        }

        if (vertA != vertIndex[2] && vertB != vertIndex[2])
        {
            newVert = vertIndex[2];
        }

        inEdge = edge;
        lastVert = strip->indices[strip->numIndices - 1];

        if ((tri->edge[0]->vert[0] == lastVert && tri->edge[0]->vert[1] == newVert) ||
            (tri->edge[0]->vert[0] == newVert && tri->edge[0]->vert[1] == lastVert))
        {
            edge = tri->edge[0];
        }
        else if ((tri->edge[1]->vert[0] == lastVert && tri->edge[1]->vert[1] == newVert) ||
                 (tri->edge[1]->vert[0] == newVert && tri->edge[1]->vert[1] == lastVert))
        {
            edge = tri->edge[1];
        }
        else if ((tri->edge[2]->vert[0] == lastVert && tri->edge[2]->vert[1] == newVert) ||
                 (tri->edge[2]->vert[0] == newVert && tri->edge[2]->vert[1] == lastVert))
        {
            edge = tri->edge[2];
        }
        else
        {
            edge = NULL;
        }

        if (TriStripEdgeIsDeadEnd(edge, pass))
        {

            if (tri->edge[0] != inEdge && tri->edge[0] != edge)
            {
                swapEdge = tri->edge[0];
            }
            else if (tri->edge[1] != inEdge && tri->edge[1] != edge)
            {
                swapEdge = tri->edge[1];
            }
            else if (tri->edge[2] != inEdge && tri->edge[2] != edge)
            {
                swapEdge = tri->edge[2];
            }

            if (TriStripEdgeHasFreeTri(swapEdge, pass))
            {
                if (strip->numIndices & 1)
                {
                    strip->indices[strip->numIndices] = newVert;
                    edge = NULL;
                    strip->numIndices++;
                }
                else
                {
                    // Repeat the vertex before the last so the strip turns onto the third edge.
                    edge = swapEdge;
                    strip->indices[strip->numIndices] = strip->indices[strip->numIndices - 2];
                    strip->numIndices++;
                    strip->indices[strip->numIndices] = newVert;
                    strip->numIndices++;
                }
            }
            else
            {
                strip->indices[strip->numIndices] = newVert;
                edge = NULL;
                strip->numIndices++;
            }
        }
        else
        {
            strip->indices[strip->numIndices] = newVert;
            strip->numIndices++;
        }
    }

    return numAdded;
}

// Adds an empty strip with room for space indices to the head of a strip list.
static TriStripList* TriStripListAdd(TriStripList** stripList, U32 space)
{
    TriStripList* strip = (TriStripList*)RwFreeListAlloc(MESHGLOBAL(triStripListEntryFreeList));

    strip->next = *stripList;
    *stripList = strip;
    strip->space = space;
    strip->numIndices = 0;
    strip->indices = (U16*)RwMalloc(strip->space * sizeof(U32));

    return strip;
}

// Covers the triangles with strips, starting each one at a triangle with the fewest free
// neighbours. With tryAllEdges each start is tried along all three of its edges first.
static S32 TriStripStripTris(RpBuildMeshTriangle* triangles, U32 numTris, TriStripList** stripList,
                             S32 tryAllEdges)
{
    TriStripEdge* edgeList = NULL;
    TriStripBinEntry* bins[4];
    TriStripFreeLists freeLists;
    TriStripBinEntry** entries;
    TriStripList* forward;
    TriStripList* backward;
    TriStripEdge* edge;
    TriStripEdge* forwardEdge = NULL;
    TriStripEdge* backwardEdge = NULL;
    U32 numUsed = 0;
    U32 i;

    bins[0] = NULL;
    bins[1] = NULL;
    bins[2] = NULL;
    bins[3] = NULL;
    freeLists.binEntryFreeList = NULL;
    freeLists.edgeFreeList = NULL;

    entries = TriStripBinEntryArrayCreate(numTris, &freeLists, &edgeList, triangles);

    for (i = 0; i < numTris; i++)
    {
        TRISTRIPBININSERT(bins, entries[i]);
    }

    forward = (TriStripList*)RwFreeListAlloc(MESHGLOBAL(triStripListEntryFreeList));
    forward->space = numTris * 2 + 2;
    forward->numIndices = 0;
    forward->indices = (U16*)RwMalloc(forward->space * sizeof(U32));

    backward = (TriStripList*)RwFreeListAlloc(MESHGLOBAL(triStripListEntryFreeList));
    backward->space = numTris * 2 + 2;
    backward->numIndices = 0;
    backward->indices = (U16*)RwMalloc(backward->space * sizeof(U32));

    while (numUsed < numTris)
    {
        U32 bestUsed = 0;
        S32 pass = tryAllEdges ? 0 : TRISTRIPNUMPASSES - 1;

        if (bins[0])
        {
            // A triangle with no free neighbours is a strip of its own.
            TriStripBinEntry* entry = bins[0];
            TriStripList* strip =
                (TriStripList*)RwFreeListAlloc(MESHGLOBAL(triStripListEntryFreeList));
            U16* vertIndex;

            strip->next = *stripList;
            *stripList = strip;
            strip->space = 3;
            strip->numIndices = 3;
            strip->indices = (U16*)RwMalloc(3 * sizeof(U32));

            vertIndex = triangles[entry->triIndex].vertIndex;
            strip->indices[0] = vertIndex[0];
            strip->indices[1] = vertIndex[1];
            strip->indices[2] = vertIndex[2];

            bins[0]->used = TRUE;
            bins[0]->trialUsed = TRUE;
            bins[0] = bins[0]->next;

            if (bins[0])
            {
                bins[0]->prev = NULL;
            }

            numUsed++;
        }
        else
        {
            TriStripBinEntry* entry;
            U32 startUsed;
            S32 bin = 1;
            U32 start;
            U32 bestEdge;

            while (!bins[bin])
            {
                bin++;
            }

            entry = bins[bin];

            // Start along an edge whose neighbours both lead on, or else towards the most
            // free neighbours.
            if (TRISTRIPEDGEFREE(entry->edge[2], used) >= 2 &&
                TRISTRIPEDGEFREE(entry->edge[1], used) >= 2)
            {
                start = 1;
            }
            else if (TRISTRIPEDGEFREE(entry->edge[0], used) >= 2 &&
                     TRISTRIPEDGEFREE(entry->edge[2], used) >= 2)
            {
                start = 2;
            }
            else if (TRISTRIPEDGEFREE(entry->edge[1], used) >= 2 &&
                     TRISTRIPEDGEFREE(entry->edge[0], used) >= 2)
            {
                start = 0;
            }
            else if (TRISTRIPEDGEFREE(entry->edge[0], used) >
                     TRISTRIPEDGEFREE(entry->edge[1], used))
            {
                if (TRISTRIPEDGEFREE(entry->edge[0], used) >
                    TRISTRIPEDGEFREE(entry->edge[2], used))
                {
                    start = 2;
                }
                else
                {
                    start = 1;
                }
            }
            else if (TRISTRIPEDGEFREE(entry->edge[1], used) >
                     TRISTRIPEDGEFREE(entry->edge[2], used))
            {
                start = 0;
            }
            else
            {
                start = 1;
            }

            startUsed = numUsed;
            bestEdge = start;

            do
            {
                U32 edgeIndex;
                U16* vertIndex;
                S32 backwardFree;

                for (i = 0; i < numTris; i++)
                {
                    entries[i]->trialUsed = entries[i]->used;
                }

                switch (pass++)
                {
                    case 0:
                        edgeIndex = start % 3;
                        break;
                    case 1:
                        edgeIndex = (start + 1) % 3;
                        break;
                    case 2:
                        edgeIndex = (start + 2) % 3;
                        break;
                    default:
                        edgeIndex = bestEdge;
                        break;
                }

                switch (edgeIndex)
                {
                    case 0:
                        forwardEdge = bins[bin]->edge[1];
                        backwardEdge = bins[bin]->edge[0];
                        break;
                    case 1:
                        forwardEdge = bins[bin]->edge[2];
                        backwardEdge = bins[bin]->edge[1];
                        break;
                    case 2:
                        forwardEdge = bins[bin]->edge[0];
                        backwardEdge = bins[bin]->edge[2];
                        break;
                }

                vertIndex = triangles[bins[bin]->triIndex].vertIndex;
                forward->indices[0] = vertIndex[edgeIndex % 3];
                forward->indices[1] = vertIndex[(edgeIndex + 1) % 3];
                forward->indices[2] = vertIndex[(edgeIndex + 2) % 3];
                forward->numIndices = 3;

                TriStripMarkTriUsed(bins[bin], bins, pass);
                numUsed =
                    startUsed + 1 + TriStripFollow(forward, forwardEdge, bins, triangles, pass);

                if (pass < TRISTRIPNUMPASSES)
                {
                    backwardFree = TRISTRIPEDGEFREE(backwardEdge, trialUsed) > 0;
                }
                else
                {
                    backwardFree = TRISTRIPEDGEFREE(backwardEdge, used) > 0;
                }

                if (backwardFree)
                {
                    U32 numStart;

                    backward->indices[0] = forward->indices[1];
                    backward->indices[1] = forward->indices[0];
                    backward->numIndices = 2;
                    numStart = backward->numIndices;

                    numUsed += TriStripFollow(backward, backwardEdge, bins, triangles, pass);

                    // Keep the winding when the backward strip is reversed onto the front.
                    if (numStart & 1)
                    {
                        backward->indices[numStart] = backward->indices[numStart - 2];
                        backward->numIndices++;
                    }

                    if (numUsed > bestUsed)
                    {
                        bestUsed = numUsed;
                        bestEdge = edgeIndex;
                    }

                    if (pass >= TRISTRIPNUMPASSES)
                    {
                        TriStripList* strip = TriStripListAdd(
                            stripList, forward->numIndices + (backward->numIndices - 2));

                        while (backward->numIndices > 2)
                        {
                            strip->indices[strip->numIndices] =
                                backward->indices[backward->numIndices - 1];
                            strip->numIndices++;
                            backward->numIndices--;
                        }

                        memcpy(&strip->indices[strip->numIndices], forward->indices,
                               forward->numIndices * sizeof(U32));
                        strip->numIndices = strip->space;
                    }
                }
                else
                {
                    if (numUsed > bestUsed)
                    {
                        bestUsed = numUsed;
                        bestEdge = edgeIndex;
                    }

                    if (pass >= TRISTRIPNUMPASSES)
                    {
                        TriStripList* strip =
                            (TriStripList*)RwFreeListAlloc(MESHGLOBAL(triStripListEntryFreeList));

                        strip->next = *stripList;
                        *stripList = strip;
                        strip->space = forward->numIndices;
                        strip->numIndices = forward->numIndices;
                        strip->indices = (U16*)RwMalloc(forward->numIndices * sizeof(U32));
                        memcpy(strip->indices, forward->indices, forward->numIndices * sizeof(U32));
                    }
                }
            } while (pass < TRISTRIPNUMPASSES);
        }
    }

    RwFree(backward->indices);
    backward->indices = NULL;
    RwFreeListFree(MESHGLOBAL(triStripListEntryFreeList), backward);

    RwFree(forward->indices);
    forward->indices = NULL;
    RwFreeListFree(MESHGLOBAL(triStripListEntryFreeList), forward);

    for (edge = edgeList; edge;)
    {
        TriStripEdge* next = edge->next;

        RwFreeListFree(freeLists.edgeFreeList, edge);
        edge = next;
    }

    RwFreeListDestroy(freeLists.edgeFreeList);
    freeLists.edgeFreeList = NULL;

    for (i = 0; i < numTris; i++)
    {
        RwFreeListFree(freeLists.binEntryFreeList, entries[i]);
        entries[i] = NULL;
    }

    RwFreeListDestroy(freeLists.binEntryFreeList);
    freeLists.binEntryFreeList = NULL;

    RwFree(entries);

    return TRUE;
}

// Joins the strips of a list into one, through degenerate triangles. With keepWinding every
// strip keeps its own winding order.
static S32 TriStripJoin(TriStripList** stripList, S32 keepWinding)
{
    TriStripList* joined;
    TriStripList* strip;
    TriStripList* rest;
    U32 i;

    if (!*stripList)
    {
        return FALSE;
    }

    joined = (TriStripList*)RwFreeListAlloc(MESHGLOBAL(triStripListEntryFreeList));
    joined->numIndices = 0;
    joined->space = 0;

    for (strip = *stripList; strip; strip = strip->next)
    {
        joined->space += strip->numIndices + 6;
    }

    joined->indices = (U16*)RwMalloc(joined->space * sizeof(U32));

    strip = *stripList;

    for (i = 0; i < strip->numIndices; i++)
    {
        joined->indices[joined->numIndices++] = strip->indices[i];
    }

    RwFree(strip->indices);
    strip->indices = NULL;
    rest = strip->next;
    RwFreeListFree(MESHGLOBAL(triStripListEntryFreeList), strip);

    while (rest)
    {
        // Prefer a strip that starts at the last vertex, then one that starts at the vertex
        // before it.
        for (strip = rest; strip; strip = strip->next)
        {
            U16* end = &joined->indices[joined->numIndices];

            if (strip->indices[0] == end[-1])
            {
                if ((joined->numIndices & 1) && keepWinding)
                {
                    end[0] = strip->indices[0];
                    joined->numIndices++;
                }

                break;
            }
        }

        if (!strip)
        {
            for (strip = rest; strip; strip = strip->next)
            {
                U16* end = &joined->indices[joined->numIndices];

                if (strip->indices[0] == end[-2])
                {
                    if (!(joined->numIndices & 1) && keepWinding)
                    {
                        end[0] = strip->indices[0];
                        joined->indices[joined->numIndices + 1] = strip->indices[0];
                        joined->numIndices += 2;
                    }
                    else
                    {
                        end[0] = strip->indices[0];
                        joined->numIndices++;
                    }

                    break;
                }
            }
        }

        if (!strip)
        {
            U16* end = &joined->indices[joined->numIndices];

            strip = rest;

            if (!(joined->numIndices & 1) || !keepWinding)
            {
                end[0] = end[-1];
                joined->indices[joined->numIndices + 1] = strip->indices[0];
                joined->numIndices += 2;
            }
            else
            {
                end[0] = end[-1];
                joined->indices[joined->numIndices + 1] = strip->indices[0];
                joined->indices[joined->numIndices + 2] = strip->indices[0];
                joined->numIndices += 3;
            }
        }

        for (i = 0; i < strip->numIndices; i++)
        {
            joined->indices[joined->numIndices++] = strip->indices[i];
        }

        RwFree(strip->indices);
        strip->indices = NULL;

        if (rest == strip)
        {
            rest = rest->next;
            RwFreeListFree(MESHGLOBAL(triStripListEntryFreeList), strip);
        }
        else
        {
            TriStripList* prev = rest;

            while (prev->next != strip)
            {
                prev = prev->next;
            }

            prev->next = strip->next;
            RwFreeListFree(MESHGLOBAL(triStripListEntryFreeList), strip);
        }
    }

    *stripList = joined;
    joined->next = NULL;

    return TRUE;
}

// Builds a triangle strip mesh header with one strip per material.
static RpMeshHeader* TriStripMeshGenerate(RpBuildMesh* buildMesh, S32 tryAllEdges,
                                          S32 keepWinding)
{
    U16 numMeshes = 0;
    RpBuildMeshTriangle** sorted;
    RpBuildMeshTriangle** next;
    RpMesh** meshes;
    RpMesh* runs;
    RpMesh* run;
    RpMeshHeader* meshHeader;
    RpMesh* mesh;
    U16* indices;
    TriStripList* stripList;
    U32 numRuns;
    U32 size;
    U32 totalIndices;
    U32 i;

    sorted =
        (RpBuildMeshTriangle**)RwMalloc(buildMesh->numTriangles * sizeof(RpBuildMeshTriangle*));

    if (!sorted)
    {
        return NULL;
    }

    for (i = 0; i < buildMesh->numTriangles; i++)
    {
        sorted[i] = &buildMesh->meshTriangles[i];
    }

    qsort(sorted, buildMesh->numTriangles, sizeof(RpBuildMeshTriangle*), SortPolygons);

    numRuns = 1;

    if (buildMesh->numTriangles >= 2)
    {
        RpMaterial* material = sorted[0]->material;

        for (i = 1; i < buildMesh->numTriangles; i++)
        {
            if (sorted[i]->material != material)
            {
                material = sorted[i]->material;
                numRuns++;
            }
        }
    }

    meshes = (RpMesh**)RwMalloc(numRuns * sizeof(RpMesh*));
    runs = (RpMesh*)RwMalloc(numRuns * sizeof(RpMesh));

    // Each run counts its triangles in numIndices, from the first triangle of the next.
    numRuns = 1;
    runs[0].material = sorted[0]->material;
    runs[0].numIndices = 0;
    runs[0].indices = NULL;

    if (buildMesh->numTriangles >= 2)
    {
        run = &runs[1];

        for (i = 0; i < buildMesh->numTriangles - 1; i++)
        {
            if (sorted[i]->material != sorted[i + 1]->material)
            {
                run->material = sorted[i + 1]->material;
                numRuns++;
                run->numIndices = i + 1;
                run->indices = NULL;
                run[-1].numIndices = (i + 1) - run[-1].numIndices;
                run++;
            }
        }
    }

    runs[numRuns - 1].numIndices = buildMesh->numTriangles - runs[numRuns - 1].numIndices;

    MESHGLOBAL(triStripListEntryFreeList) =
        RwFreeListCreate(sizeof(TriStripList), buildMesh->numTriangles / 10 + 5, 0);

    stripList = NULL;
    next = sorted;
    run = runs;

    for (i = 0; i < numRuns; i++, run++)
    {
        RpBuildMeshTriangle* triangles =
            (RpBuildMeshTriangle*)RwMalloc(run->numIndices * sizeof(RpBuildMeshTriangle));
        TriStripList* strip;
        U32 j;

        for (j = 0; j < run->numIndices; j++)
        {
            triangles[j] = **next++;
        }

        TriStripStripTris(triangles, run->numIndices, &stripList, tryAllEdges);
        TriStripJoin(&stripList, keepWinding);

        for (strip = stripList; strip; strip = strip->next)
        {
            RpMesh* stripMesh = (RpMesh*)RwMalloc(sizeof(RpMesh) + strip->numIndices * sizeof(U16));

            stripMesh->material = run->material;
            stripMesh->numIndices = strip->numIndices;
            stripMesh->indices = (U16*)(stripMesh + 1);

            for (j = 0; j < stripMesh->numIndices; j++)
            {
                stripMesh->indices[j] = strip->indices[j];
            }

            meshes[numMeshes++] = stripMesh;
        }

        while (stripList)
        {
            strip = stripList;
            stripList = strip->next;
            RwFree(strip->indices);
            strip->indices = NULL;
            RwFreeListFree(MESHGLOBAL(triStripListEntryFreeList), strip);
        }

        RwFree(triangles);
    }

    RwFreeListDestroy(MESHGLOBAL(triStripListEntryFreeList));
    MESHGLOBAL(triStripListEntryFreeList) = NULL;

    size = sizeof(RpMeshHeader);
    totalIndices = 0;

    for (i = 0; i < numMeshes; i++)
    {
        size += sizeof(RpMesh) + meshes[i]->numIndices * sizeof(U16);
        totalIndices += meshes[i]->numIndices;
    }

    meshHeader = _rpMeshHeaderCreate(size);
    meshHeader->flags = rpMESHHEADERTRISTRIP;
    meshHeader->numMeshes = numMeshes;
    meshHeader->serialNum = MESHGLOBAL(nextSerialNum);
    meshHeader->firstMeshOffset = 0;
    meshHeader->totalIndicesInMesh = totalIndices;
    MESHGLOBAL(nextSerialNum)++;

    mesh = (RpMesh*)(meshHeader + 1);
    indices = (U16*)(mesh + numMeshes);

    for (i = 0; i < numMeshes; i++, mesh++)
    {
        mesh->indices = indices;
        mesh->numIndices = meshes[i]->numIndices;
        mesh->material = meshes[i]->material;
        memcpy(indices, meshes[i]->indices, meshes[i]->numIndices * sizeof(U16));
        indices += mesh->numIndices;
        RwFree(meshes[i]);
        meshes[i] = NULL;
    }

    RwFree(sorted);
    RwFree(meshes);
    RwFree(runs);

    return meshHeader;
}

RpMeshHeader* RpBuildMeshGenerateDefaultTriStrip(RpBuildMesh* buildMesh, void* data)
{
    return TriStripMeshGenerate(buildMesh, FALSE, TRUE);
}

// Builds a triangle list mesh header with one mesh per material.
RpMeshHeader* _rpTriListMeshGenerate(RpBuildMesh* buildMesh, void* data)
{
    RpBuildMeshTriangle** sorted;

    sorted =
        (RpBuildMeshTriangle**)RwMalloc(buildMesh->numTriangles * sizeof(RpBuildMeshTriangle*));

    if (sorted)
    {
        RpMeshHeader* meshHeader;
        RpMesh* mesh;
        U16* indices;
        U32 numMeshes;
        U32 size;
        U32 i;

        for (i = 0; i < buildMesh->numTriangles; i++)
        {
            sorted[i] = &buildMesh->meshTriangles[i];
        }

        qsort(sorted, buildMesh->numTriangles, sizeof(RpBuildMeshTriangle*), SortPolygons);

        numMeshes = 1;

        if (buildMesh->numTriangles >= 2)
        {
            RpMaterial* material = sorted[0]->material;

            for (i = 1; i < buildMesh->numTriangles; i++)
            {
                if (sorted[i]->material != material)
                {
                    material = sorted[i]->material;
                    numMeshes++;
                }
            }
        }

        size = sizeof(RpMeshHeader) + numMeshes * sizeof(RpMesh) +
               buildMesh->numTriangles * sizeof(U16) * 3;
        meshHeader = _rpMeshHeaderCreate(size);

        if (!meshHeader)
        {
            RwError err;
            RwFree(sorted);
            err.pluginID = 2;
            err.errorCode = _rwerror(0x80000013, size);
            RwErrorSet(&err);
            return NULL;
        }

        meshHeader->flags = 0;
        mesh = (RpMesh*)(meshHeader + 1);
        meshHeader->numMeshes = 1;
        indices = (U16*)(mesh + numMeshes);
        meshHeader->serialNum = MESHGLOBAL(nextSerialNum);
        meshHeader->firstMeshOffset = 0;
        meshHeader->totalIndicesInMesh = buildMesh->numTriangles * 3;
        MESHGLOBAL(nextSerialNum)++;

        mesh->numIndices = 0;
        mesh->material = sorted[0]->material;
        mesh->indices = indices;

        // The callers never pass an empty build mesh.
        i = 0;

        do
        {
            if (sorted[i]->material != mesh->material)
            {
                mesh[1].numIndices = 0;
                mesh[1].material = sorted[i]->material;
                mesh++;
                mesh->indices = indices;
                meshHeader->numMeshes++;
            }

            indices[0] = sorted[i]->vertIndex[0];
            indices[1] = sorted[i]->vertIndex[1];
            indices[2] = sorted[i]->vertIndex[2];
            indices += 3;
            mesh->numIndices += 3;
            i++;
        } while (i < buildMesh->numTriangles);

        _rpMeshHeaderForAllMeshes(meshHeader, SortPolygonsInTriListMesh, NULL);

        RwFree(sorted);

        return meshHeader;
    }
    else
    {
        RwError err;
        err.pluginID = 2;
        err.errorCode =
            _rwerror(0x80000013, buildMesh->numTriangles * sizeof(RpBuildMeshTriangle*));
        RwErrorSet(&err);
    }

    return NULL;
}

// Turns a build mesh into a mesh header: a triangle strip one through the current strip
// method, or a triangle list one. The build mesh is destroyed once that succeeds.
RpMeshHeader* _rpMeshOptimise(RpBuildMesh* buildMesh, U32 flags)
{
    if (buildMesh)
    {
        TriStripMethod generate;
        void* data;
        RpMeshHeader* meshHeader;

        if (buildMesh->numTriangles == 0)
        {
            _rpBuildMeshDestroy(buildMesh);
            return &MeshOp.emptyMeshHeader;
        }

        if (flags & rpMESHHEADERTRISTRIP)
        {
            generate = MeshOp.triStripMethod;
            data = MeshOp.triStripData;
        }
        else
        {
            generate = _rpTriListMeshGenerate;
            data = NULL;
        }

        meshHeader = generate(buildMesh, data);

        if (meshHeader)
        {
            _rpBuildMeshDestroy(buildMesh);
            return meshHeader;
        }
    }

    return NULL;
}
