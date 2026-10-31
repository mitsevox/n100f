#include <rwcore.h>

// The clip flags of a camera-space vertex: outside which plane of the view volume it lies.
#define CLIPFLAG_XLO 0x01
#define CLIPFLAG_XHI 0x02
#define CLIPFLAG_YLO 0x04
#define CLIPFLAG_YHI 0x08
#define CLIPFLAG_ZLO 0x10
#define CLIPFLAG_ZHI 0x20
#define CLIPFLAG_X (CLIPFLAG_XLO | CLIPFLAG_XHI)
#define CLIPFLAG_XY (CLIPFLAG_X | CLIPFLAG_YLO | CLIPFLAG_YHI)

// The vertex indices of a polygon being clipped wrap round a ring of this many entries.
#define CLIPRING_MASK 0xF

// What the clipper records about each new vertex in the interpolants cluster: the new vertex,
// the edge's inside and outside vertices, and how far along the edge the new one lies.
typedef struct ClipInterpolant ClipInterpolant;
struct ClipInterpolant
{
    RwImVertexIndex newVert;
    RwImVertexIndex vert0;
    RwImVertexIndex vert1;
    F32 param;
};

typedef S32 (*ClipEdgesFunc)(RxCluster* camVerts, RxCluster* devVerts, RxCluster* interpolants,
                             RwImVertexIndex* vertIndices, S32* start, S32* end, U32 clipFlag,
                             RwClipVertexFunc clipFunc);

#define CLUSTER_VERTEX(_type, _cluster, _index)                                                    \
    ((_type*)((U8*)(_cluster)->data + (_cluster)->stride * (_index)))

#define CLIP_LERP(_a, _b, _t) ((_a) + (_t) * ((_b) - (_a)))

#define CLIP_LERPTEXCOORDS(_new, _v0, _v1, _t)                                                     \
    do                                                                                             \
    {                                                                                              \
        (_new)->u = CLIP_LERP((_v0)->u, (_v1)->u, (_t));                                           \
        (_new)->v = CLIP_LERP((_v0)->v, (_v1)->v, (_t));                                           \
    } while (0)

#define CLIP_LERPCOLOR(_new, _v0, _v1, _t)                                                         \
    do                                                                                             \
    {                                                                                              \
        (_new)->emissiveColor.red = (U8)((F32)(_v0)->emissiveColor.red +                           \
                                         (_t) * (F32)((_v1)->emissiveColor.red -                   \
                                                      (_v0)->emissiveColor.red));                  \
        (_new)->emissiveColor.green = (U8)((F32)(_v0)->emissiveColor.green +                       \
                                           (_t) * (F32)((_v1)->emissiveColor.green -               \
                                                        (_v0)->emissiveColor.green));              \
        (_new)->emissiveColor.blue = (U8)((F32)(_v0)->emissiveColor.blue +                         \
                                          (_t) * (F32)((_v1)->emissiveColor.blue -                 \
                                                       (_v0)->emissiveColor.blue));                \
        (_new)->emissiveColor.alpha = (U8)((F32)(_v0)->emissiveColor.alpha +                       \
                                           (_t) * (F32)((_v1)->emissiveColor.alpha -               \
                                                        (_v0)->emissiveColor.alpha));              \
    } while (0)

// The flags of a new vertex against the planes not yet clipped: a perspective view volume is
// bounded by 0 and z, a parallel one by 0 and 1.
#define CLIP_YFLAGS(_vert, _limit)                                                                 \
    (((_vert)->cameraVertex.y < 0.0f)       ? CLIPFLAG_YLO                                         \
     : ((_vert)->cameraVertex.y > (_limit)) ? CLIPFLAG_YHI                                         \
                                            : 0)

#define CLIP_XFLAGS(_vert, _limit)                                                                 \
    (((_vert)->cameraVertex.x < 0.0f)       ? CLIPFLAG_XLO                                         \
     : ((_vert)->cameraVertex.x > (_limit)) ? CLIPFLAG_XHI                                         \
                                            : 0)

S32 _rwForAllEdgesWithRxInterpolant(RxCluster* camVerts, RxCluster* devVerts,
                                    RxCluster* interpolants, RwImVertexIndex* vertIndices,
                                    S32* start, S32* end, U32 clipFlag, RwClipVertexFunc clipFunc)
{
    S32 newEnd = *end;
    S32 current = *start;
    RwImVertexIndex prevIndex = vertIndices[(newEnd - 1) & CLIPRING_MASK];
    RwImVertexIndex curIndex = vertIndices[current];
    S32 stop = newEnd;
    RxCamSpace3DVertex* prevCam = CLUSTER_VERTEX(RxCamSpace3DVertex, camVerts, prevIndex);
    RwIm2DVertex* prevDev = CLUSTER_VERTEX(RwIm2DVertex, devVerts, prevIndex);
    RxCamSpace3DVertex* curCam = CLUSTER_VERTEX(RxCamSpace3DVertex, camVerts, curIndex);
    RwIm2DVertex* curDev = CLUSTER_VERTEX(RwIm2DVertex, devVerts, curIndex);

    do
    {
        if (!(prevCam->clipFlags & clipFlag))
        {
            vertIndices[newEnd] = prevIndex;
            newEnd = (newEnd + 1) & CLIPRING_MASK;
        }

        if (clipFlag & (prevCam->clipFlags ^ curCam->clipFlags))
        {
            RwImVertexIndex newIndex = camVerts->numUsed++;
            ClipInterpolant* interp = (ClipInterpolant*)interpolants->currentData;

            vertIndices[newEnd] = newIndex;
            newEnd = (newEnd + 1) & CLIPRING_MASK;
            devVerts->numUsed++;

            if (prevCam->clipFlags & clipFlag)
            {
                interp->param = clipFunc(CLUSTER_VERTEX(RxCamSpace3DVertex, camVerts, newIndex),
                                         prevCam, curCam,
                                         CLUSTER_VERTEX(RwIm2DVertex, devVerts, newIndex),
                                         prevDev, curDev);
                interp->newVert = newIndex;
                interp->vert0 = prevIndex;
                interp->vert1 = curIndex;
            }
            else
            {
                interp->param = clipFunc(CLUSTER_VERTEX(RxCamSpace3DVertex, camVerts, newIndex),
                                         curCam, prevCam,
                                         CLUSTER_VERTEX(RwIm2DVertex, devVerts, newIndex),
                                         curDev, prevDev);
                interp->newVert = newIndex;
                interp->vert0 = curIndex;
                interp->vert1 = prevIndex;
            }

            interpolants->numUsed++;
            interpolants->currentData = (U8*)interpolants->currentData + interpolants->stride;
        }

        prevIndex = curIndex;
        curIndex = vertIndices[(current + 1) & CLIPRING_MASK];
        current = (current + 1) & CLIPRING_MASK;
        prevCam = curCam;
        prevDev = curDev;
        curCam = CLUSTER_VERTEX(RxCamSpace3DVertex, camVerts, curIndex);
        curDev = CLUSTER_VERTEX(RwIm2DVertex, devVerts, curIndex);
    } while (current != stop);

    *start = current;
    *end = newEnd;

    return (newEnd - current) & CLIPRING_MASK;
}

S32 _rwForAllEdgesWithoutRxInterpolant(RxCluster* camVerts, RxCluster* devVerts,
                                       RxCluster* interpolants, RwImVertexIndex* vertIndices,
                                       S32* start, S32* end, U32 clipFlag,
                                       RwClipVertexFunc clipFunc)
{
    S32 newEnd = *end;
    S32 current = *start;
    RwImVertexIndex prevIndex = vertIndices[(newEnd - 1) & CLIPRING_MASK];
    RwImVertexIndex curIndex = vertIndices[current];
    S32 stop = newEnd;
    RxCamSpace3DVertex* prevCam = CLUSTER_VERTEX(RxCamSpace3DVertex, camVerts, prevIndex);
    RwIm2DVertex* prevDev = CLUSTER_VERTEX(RwIm2DVertex, devVerts, prevIndex);
    RxCamSpace3DVertex* curCam = CLUSTER_VERTEX(RxCamSpace3DVertex, camVerts, curIndex);
    RwIm2DVertex* curDev = CLUSTER_VERTEX(RwIm2DVertex, devVerts, curIndex);

    do
    {
        if (!(prevCam->clipFlags & clipFlag))
        {
            vertIndices[newEnd] = prevIndex;
            newEnd = (newEnd + 1) & CLIPRING_MASK;
        }

        if (clipFlag & (prevCam->clipFlags ^ curCam->clipFlags))
        {
            RwImVertexIndex newIndex = camVerts->numUsed++;

            vertIndices[newEnd] = newIndex;
            newEnd = (newEnd + 1) & CLIPRING_MASK;
            devVerts->numUsed++;

            if (prevCam->clipFlags & clipFlag)
            {
                clipFunc(CLUSTER_VERTEX(RxCamSpace3DVertex, camVerts, newIndex), prevCam, curCam,
                         CLUSTER_VERTEX(RwIm2DVertex, devVerts, newIndex), prevDev, curDev);
            }
            else
            {
                clipFunc(CLUSTER_VERTEX(RxCamSpace3DVertex, camVerts, newIndex), curCam, prevCam,
                         CLUSTER_VERTEX(RwIm2DVertex, devVerts, newIndex), curDev, prevDev);
            }
        }

        prevIndex = curIndex;
        curIndex = vertIndices[(current + 1) & CLIPRING_MASK];
        current = (current + 1) & CLIPRING_MASK;
        prevCam = curCam;
        prevDev = curDev;
        curCam = CLUSTER_VERTEX(RxCamSpace3DVertex, camVerts, curIndex);
        curDev = CLUSTER_VERTEX(RwIm2DVertex, devVerts, curIndex);
    } while (current != stop);

    *start = current;
    *end = newEnd;

    return (newEnd - current) & CLIPRING_MASK;
}

S32 _rwForAllEdges(RxCluster* camVerts, RxCluster* devVerts, RxCluster* interpolants,
                   RwImVertexIndex* vertIndices, S32* start, S32* end, U32 clipFlag,
                   RwClipVertexFunc clipFunc)
{
    ClipEdgesFunc func = interpolants ? _rwForAllEdgesWithRxInterpolant
                                      : _rwForAllEdgesWithoutRxInterpolant;

    return func(camVerts, devVerts, interpolants, vertIndices, start, end, clipFlag, clipFunc);
}

S32 _rwForOneEdge(RxCluster* camVerts, RxCluster* devVerts, RwImVertexIndex* vertIndices, S32* start,
                  S32* end, U32 clipFlag, RwClipVertexFunc clipFunc)
{
    S32 current = *start;
    S32 newEnd = *end;
    RwImVertexIndex nextIndex = vertIndices[(current + 1) & CLIPRING_MASK];
    S32 oldEnd = newEnd;
    RwImVertexIndex curIndex = vertIndices[current];
    RxCamSpace3DVertex* nextCam = CLUSTER_VERTEX(RxCamSpace3DVertex, camVerts, nextIndex);
    RwIm2DVertex* nextDev = CLUSTER_VERTEX(RwIm2DVertex, devVerts, nextIndex);
    RxCamSpace3DVertex* curCam = CLUSTER_VERTEX(RxCamSpace3DVertex, camVerts, curIndex);
    RwIm2DVertex* curDev = CLUSTER_VERTEX(RwIm2DVertex, devVerts, curIndex);

    if (!(nextCam->clipFlags & clipFlag))
    {
        vertIndices[newEnd] = nextIndex;
        newEnd = (newEnd + 1) & CLIPRING_MASK;
    }

    if (clipFlag & (nextCam->clipFlags ^ curCam->clipFlags))
    {
        RwImVertexIndex newIndex = camVerts->numUsed++;

        vertIndices[newEnd] = newIndex;
        newEnd = (newEnd + 1) & CLIPRING_MASK;
        devVerts->numUsed++;

        if (nextCam->clipFlags & clipFlag)
        {
            clipFunc(CLUSTER_VERTEX(RxCamSpace3DVertex, camVerts, newIndex), nextCam, curCam,
                     CLUSTER_VERTEX(RwIm2DVertex, devVerts, newIndex), nextDev, curDev);
        }
        else
        {
            clipFunc(CLUSTER_VERTEX(RxCamSpace3DVertex, camVerts, newIndex), curCam, nextCam,
                     CLUSTER_VERTEX(RwIm2DVertex, devVerts, newIndex), curDev, nextDev);
        }
    }

    if (!(curCam->clipFlags & clipFlag))
    {
        vertIndices[newEnd] = curIndex;
        newEnd = (newEnd + 1) & CLIPRING_MASK;
    }

    *start = *end;
    *end = newEnd;

    return (newEnd - oldEnd) & CLIPRING_MASK;
}

F32 _rwGeneratePerspClippedVertexZLO(RxCamSpace3DVertex* newCamVert,
                                     RxCamSpace3DVertex* camVert0,
                                     RxCamSpace3DVertex* camVert1, RwIm2DVertex* newDevVert,
                                     RwIm2DVertex* devVert0, RwIm2DVertex* devVert1)
{
    F32 t = (gClipInfo.nearClip - camVert0->cameraVertex.z) /
            (camVert1->cameraVertex.z - camVert0->cameraVertex.z);

    newCamVert->cameraVertex.x = CLIP_LERP(camVert0->cameraVertex.x, camVert1->cameraVertex.x, t);
    newCamVert->cameraVertex.y = CLIP_LERP(camVert0->cameraVertex.y, camVert1->cameraVertex.y, t);
    newCamVert->cameraVertex.z = gClipInfo.nearClip;
    CLIP_LERPTEXCOORDS(newCamVert, camVert0, camVert1, t);
    CLIP_LERPCOLOR(newDevVert, devVert0, devVert1, t);
    newCamVert->clipFlags = CLIPFLAG_ZLO;
    newCamVert->clipFlags |= CLIP_YFLAGS(newCamVert, newCamVert->cameraVertex.z);
    newCamVert->clipFlags |= CLIP_XFLAGS(newCamVert, newCamVert->cameraVertex.z);

    return t;
}

F32 _rwGeneratePerspClippedVertexZHI(RxCamSpace3DVertex* newCamVert,
                                     RxCamSpace3DVertex* camVert0,
                                     RxCamSpace3DVertex* camVert1, RwIm2DVertex* newDevVert,
                                     RwIm2DVertex* devVert0, RwIm2DVertex* devVert1)
{
    F32 t = (gClipInfo.farClip - camVert0->cameraVertex.z) /
            (camVert1->cameraVertex.z - camVert0->cameraVertex.z);

    newCamVert->cameraVertex.x = CLIP_LERP(camVert0->cameraVertex.x, camVert1->cameraVertex.x, t);
    newCamVert->cameraVertex.y = CLIP_LERP(camVert0->cameraVertex.y, camVert1->cameraVertex.y, t);
    newCamVert->cameraVertex.z = gClipInfo.farClip;
    CLIP_LERPTEXCOORDS(newCamVert, camVert0, camVert1, t);
    CLIP_LERPCOLOR(newDevVert, devVert0, devVert1, t);
    newCamVert->clipFlags = CLIPFLAG_ZHI;
    newCamVert->clipFlags |= CLIP_YFLAGS(newCamVert, newCamVert->cameraVertex.z);
    newCamVert->clipFlags |= CLIP_XFLAGS(newCamVert, newCamVert->cameraVertex.z);

    return t;
}

F32 _rwGeneratePerspClippedVertexYLO(RxCamSpace3DVertex* newCamVert,
                                     RxCamSpace3DVertex* camVert0,
                                     RxCamSpace3DVertex* camVert1, RwIm2DVertex* newDevVert,
                                     RwIm2DVertex* devVert0, RwIm2DVertex* devVert1)
{
    F32 t = camVert0->cameraVertex.y / (camVert0->cameraVertex.y - camVert1->cameraVertex.y);

    newCamVert->cameraVertex.x = CLIP_LERP(camVert0->cameraVertex.x, camVert1->cameraVertex.x, t);
    newCamVert->cameraVertex.z = CLIP_LERP(camVert0->cameraVertex.z, camVert1->cameraVertex.z, t);
    newCamVert->cameraVertex.y = 0.0f;
    CLIP_LERPTEXCOORDS(newCamVert, camVert0, camVert1, t);
    CLIP_LERPCOLOR(newDevVert, devVert0, devVert1, t);
    newCamVert->clipFlags = (camVert0->clipFlags & ~CLIPFLAG_XY) | CLIPFLAG_YLO;
    newCamVert->clipFlags |= CLIP_XFLAGS(newCamVert, newCamVert->cameraVertex.z);

    return t;
}

F32 _rwGeneratePerspClippedVertexYHI(RxCamSpace3DVertex* newCamVert,
                                     RxCamSpace3DVertex* camVert0,
                                     RxCamSpace3DVertex* camVert1, RwIm2DVertex* newDevVert,
                                     RwIm2DVertex* devVert0, RwIm2DVertex* devVert1)
{
    F32 dist0 = camVert0->cameraVertex.y - camVert0->cameraVertex.z;
    F32 t = dist0 / (dist0 - (camVert1->cameraVertex.y - camVert1->cameraVertex.z));

    newCamVert->cameraVertex.x = CLIP_LERP(camVert0->cameraVertex.x, camVert1->cameraVertex.x, t);
    newCamVert->cameraVertex.z = CLIP_LERP(camVert0->cameraVertex.z, camVert1->cameraVertex.z, t);
    newCamVert->cameraVertex.y = newCamVert->cameraVertex.z;
    CLIP_LERPTEXCOORDS(newCamVert, camVert0, camVert1, t);
    CLIP_LERPCOLOR(newDevVert, devVert0, devVert1, t);
    newCamVert->clipFlags = (camVert0->clipFlags & ~CLIPFLAG_XY) | CLIPFLAG_YHI;
    newCamVert->clipFlags |= CLIP_XFLAGS(newCamVert, newCamVert->cameraVertex.z);

    return t;
}

F32 _rwGeneratePerspClippedVertexXLO(RxCamSpace3DVertex* newCamVert,
                                     RxCamSpace3DVertex* camVert0,
                                     RxCamSpace3DVertex* camVert1, RwIm2DVertex* newDevVert,
                                     RwIm2DVertex* devVert0, RwIm2DVertex* devVert1)
{
    F32 t = camVert0->cameraVertex.x / (camVert0->cameraVertex.x - camVert1->cameraVertex.x);

    newCamVert->cameraVertex.y = CLIP_LERP(camVert0->cameraVertex.y, camVert1->cameraVertex.y, t);
    newCamVert->cameraVertex.z = CLIP_LERP(camVert0->cameraVertex.z, camVert1->cameraVertex.z, t);
    newCamVert->cameraVertex.x = 0.0f;
    CLIP_LERPTEXCOORDS(newCamVert, camVert0, camVert1, t);
    CLIP_LERPCOLOR(newDevVert, devVert0, devVert1, t);
    newCamVert->clipFlags = (camVert0->clipFlags & ~CLIPFLAG_X) | CLIPFLAG_XLO;

    return t;
}

F32 _rwGeneratePerspClippedVertexXHI(RxCamSpace3DVertex* newCamVert,
                                     RxCamSpace3DVertex* camVert0,
                                     RxCamSpace3DVertex* camVert1, RwIm2DVertex* newDevVert,
                                     RwIm2DVertex* devVert0, RwIm2DVertex* devVert1)
{
    F32 dist0 = camVert0->cameraVertex.x - camVert0->cameraVertex.z;
    F32 t = dist0 / (dist0 - (camVert1->cameraVertex.x - camVert1->cameraVertex.z));

    newCamVert->cameraVertex.y = CLIP_LERP(camVert0->cameraVertex.y, camVert1->cameraVertex.y, t);
    newCamVert->cameraVertex.z = CLIP_LERP(camVert0->cameraVertex.z, camVert1->cameraVertex.z, t);
    newCamVert->cameraVertex.x = newCamVert->cameraVertex.z;
    CLIP_LERPTEXCOORDS(newCamVert, camVert0, camVert1, t);
    CLIP_LERPCOLOR(newDevVert, devVert0, devVert1, t);
    newCamVert->clipFlags = (camVert0->clipFlags & ~CLIPFLAG_X) | CLIPFLAG_XHI;

    return t;
}

F32 _rwGenerateParallelZLOClippedVertex(RxCamSpace3DVertex* newCamVert,
                                        RxCamSpace3DVertex* camVert0,
                                        RxCamSpace3DVertex* camVert1, RwIm2DVertex* newDevVert,
                                        RwIm2DVertex* devVert0, RwIm2DVertex* devVert1)
{
    F32 t = (gClipInfo.nearClip - camVert0->cameraVertex.z) /
            (camVert1->cameraVertex.z - camVert0->cameraVertex.z);

    newCamVert->cameraVertex.x = CLIP_LERP(camVert0->cameraVertex.x, camVert1->cameraVertex.x, t);
    newCamVert->cameraVertex.y = CLIP_LERP(camVert0->cameraVertex.y, camVert1->cameraVertex.y, t);
    newCamVert->cameraVertex.z = gClipInfo.nearClip;
    CLIP_LERPTEXCOORDS(newCamVert, camVert0, camVert1, t);
    CLIP_LERPCOLOR(newDevVert, devVert0, devVert1, t);
    newCamVert->clipFlags = CLIPFLAG_ZLO;
    newCamVert->clipFlags |= CLIP_YFLAGS(newCamVert, 1.0f);
    newCamVert->clipFlags |= CLIP_XFLAGS(newCamVert, 1.0f);

    return t;
}

F32 _rwGenerateParallelZHIClippedVertex(RxCamSpace3DVertex* newCamVert,
                                        RxCamSpace3DVertex* camVert0,
                                        RxCamSpace3DVertex* camVert1, RwIm2DVertex* newDevVert,
                                        RwIm2DVertex* devVert0, RwIm2DVertex* devVert1)
{
    F32 t = (gClipInfo.farClip - camVert0->cameraVertex.z) /
            (camVert1->cameraVertex.z - camVert0->cameraVertex.z);

    newCamVert->cameraVertex.x = CLIP_LERP(camVert0->cameraVertex.x, camVert1->cameraVertex.x, t);
    newCamVert->cameraVertex.y = CLIP_LERP(camVert0->cameraVertex.y, camVert1->cameraVertex.y, t);
    newCamVert->cameraVertex.z = gClipInfo.farClip;
    CLIP_LERPTEXCOORDS(newCamVert, camVert0, camVert1, t);
    CLIP_LERPCOLOR(newDevVert, devVert0, devVert1, t);
    newCamVert->clipFlags = CLIPFLAG_ZHI;
    newCamVert->clipFlags |= CLIP_YFLAGS(newCamVert, 1.0f);
    newCamVert->clipFlags |= CLIP_XFLAGS(newCamVert, 1.0f);

    return t;
}

F32 _rwGenerateParallelYLOClippedVertex(RxCamSpace3DVertex* newCamVert,
                                        RxCamSpace3DVertex* camVert0,
                                        RxCamSpace3DVertex* camVert1, RwIm2DVertex* newDevVert,
                                        RwIm2DVertex* devVert0, RwIm2DVertex* devVert1)
{
    F32 t = camVert0->cameraVertex.y / (camVert0->cameraVertex.y - camVert1->cameraVertex.y);

    newCamVert->cameraVertex.x = CLIP_LERP(camVert0->cameraVertex.x, camVert1->cameraVertex.x, t);
    newCamVert->cameraVertex.z = CLIP_LERP(camVert0->cameraVertex.z, camVert1->cameraVertex.z, t);
    newCamVert->cameraVertex.y = 0.0f;
    CLIP_LERPTEXCOORDS(newCamVert, camVert0, camVert1, t);
    CLIP_LERPCOLOR(newDevVert, devVert0, devVert1, t);
    newCamVert->clipFlags = (camVert0->clipFlags & ~CLIPFLAG_XY) | CLIPFLAG_YLO;
    newCamVert->clipFlags |= CLIP_XFLAGS(newCamVert, 1.0f);

    return t;
}

F32 _rwGenerateParallelYHIClippedVertex(RxCamSpace3DVertex* newCamVert,
                                        RxCamSpace3DVertex* camVert0,
                                        RxCamSpace3DVertex* camVert1, RwIm2DVertex* newDevVert,
                                        RwIm2DVertex* devVert0, RwIm2DVertex* devVert1)
{
    F32 t = (1.0f - camVert0->cameraVertex.y) /
            (camVert1->cameraVertex.y - camVert0->cameraVertex.y);

    newCamVert->cameraVertex.x = CLIP_LERP(camVert0->cameraVertex.x, camVert1->cameraVertex.x, t);
    newCamVert->cameraVertex.z = CLIP_LERP(camVert0->cameraVertex.z, camVert1->cameraVertex.z, t);
    newCamVert->cameraVertex.y = 1.0f;
    CLIP_LERPTEXCOORDS(newCamVert, camVert0, camVert1, t);
    CLIP_LERPCOLOR(newDevVert, devVert0, devVert1, t);
    newCamVert->clipFlags = (camVert0->clipFlags & ~CLIPFLAG_XY) | CLIPFLAG_YHI;
    newCamVert->clipFlags |= CLIP_XFLAGS(newCamVert, 1.0f);

    return t;
}

F32 _rwGenerateParallelXLOClippedVertex(RxCamSpace3DVertex* newCamVert,
                                        RxCamSpace3DVertex* camVert0,
                                        RxCamSpace3DVertex* camVert1, RwIm2DVertex* newDevVert,
                                        RwIm2DVertex* devVert0, RwIm2DVertex* devVert1)
{
    F32 t = camVert0->cameraVertex.x / (camVert0->cameraVertex.x - camVert1->cameraVertex.x);

    newCamVert->cameraVertex.y = CLIP_LERP(camVert0->cameraVertex.y, camVert1->cameraVertex.y, t);
    newCamVert->cameraVertex.z = CLIP_LERP(camVert0->cameraVertex.z, camVert1->cameraVertex.z, t);
    newCamVert->cameraVertex.x = 0.0f;
    CLIP_LERPTEXCOORDS(newCamVert, camVert0, camVert1, t);
    CLIP_LERPCOLOR(newDevVert, devVert0, devVert1, t);
    newCamVert->clipFlags = (camVert0->clipFlags & ~CLIPFLAG_X) | CLIPFLAG_XLO;

    return t;
}

F32 _rwGenerateParallelXHIClippedVertex(RxCamSpace3DVertex* newCamVert,
                                        RxCamSpace3DVertex* camVert0,
                                        RxCamSpace3DVertex* camVert1, RwIm2DVertex* newDevVert,
                                        RwIm2DVertex* devVert0, RwIm2DVertex* devVert1)
{
    F32 t = (1.0f - camVert0->cameraVertex.x) /
            (camVert1->cameraVertex.x - camVert0->cameraVertex.x);

    newCamVert->cameraVertex.y = CLIP_LERP(camVert0->cameraVertex.y, camVert1->cameraVertex.y, t);
    newCamVert->cameraVertex.z = CLIP_LERP(camVert0->cameraVertex.z, camVert1->cameraVertex.z, t);
    newCamVert->cameraVertex.x = 1.0f;
    CLIP_LERPTEXCOORDS(newCamVert, camVert0, camVert1, t);
    CLIP_LERPCOLOR(newDevVert, devVert0, devVert1, t);
    newCamVert->clipFlags = (camVert0->clipFlags & ~CLIPFLAG_X) | CLIPFLAG_XHI;

    return t;
}
