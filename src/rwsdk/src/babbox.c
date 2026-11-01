#include <rwcore.h>

// Axis-aligned bounding boxes: sup holds the largest coordinates seen, inf the smallest.

// Widens a box so that it holds a point.
#define BBOX_ADDPOINT(_box, _point)                                                                \
    {                                                                                              \
        if ((_box)->inf.x > (_point)->x)                                                           \
        {                                                                                          \
            (_box)->inf.x = (_point)->x;                                                           \
        }                                                                                          \
        if ((_box)->inf.y > (_point)->y)                                                           \
        {                                                                                          \
            (_box)->inf.y = (_point)->y;                                                           \
        }                                                                                          \
        if ((_box)->inf.z > (_point)->z)                                                           \
        {                                                                                          \
            (_box)->inf.z = (_point)->z;                                                           \
        }                                                                                          \
        if ((_box)->sup.x < (_point)->x)                                                           \
        {                                                                                          \
            (_box)->sup.x = (_point)->x;                                                           \
        }                                                                                          \
        if ((_box)->sup.y < (_point)->y)                                                           \
        {                                                                                          \
            (_box)->sup.y = (_point)->y;                                                           \
        }                                                                                          \
        if ((_box)->sup.z < (_point)->z)                                                           \
        {                                                                                          \
            (_box)->sup.z = (_point)->z;                                                           \
        }                                                                                          \
    }

RwBBox* RwBBoxCalculate(RwBBox* boundBox, const RwV3d* verts, S32 numVerts)
{
    boundBox->inf = *verts;
    boundBox->sup = *verts;

    for (numVerts--, verts++; numVerts; numVerts--, verts++)
    {
        BBOX_ADDPOINT(boundBox, verts);
    }

    return boundBox;
}

RwBBox* RwBBoxInitialize(RwBBox* boundBox, const RwV3d* vertex)
{
    boundBox->inf = *vertex;
    boundBox->sup = *vertex;

    return boundBox;
}

RwBBox* RwBBoxAddPoint(RwBBox* boundBox, const RwV3d* vertex)
{
    BBOX_ADDPOINT(boundBox, vertex);

    return boundBox;
}
