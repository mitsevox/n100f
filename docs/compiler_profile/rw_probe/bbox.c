typedef float RwReal;
typedef int RwInt32;
typedef struct RwV3d { RwReal x, y, z; } RwV3d;
typedef struct RwBBox { RwV3d sup; RwV3d inf; } RwBBox;

RwBBox *BBoxCalculate(RwBBox *box, const RwV3d *points, RwInt32 count)
{
    box->inf = *points;
    box->sup = *points;
    points++;
    {
        RwInt32 i = count - 1;
        while (i--)
    {
        if (box->inf.x > points->x) box->inf.x = points->x;
        if (box->inf.y > points->y) box->inf.y = points->y;
        if (box->inf.z > points->z) box->inf.z = points->z;
        if (box->sup.x < points->x) box->sup.x = points->x;
        if (box->sup.y < points->y) box->sup.y = points->y;
        if (box->sup.z < points->z) box->sup.z = points->z;
        points++;
    }
    }
    return box;
}

RwBBox *BBoxInitialize(RwBBox *box, const RwV3d *point)
{
    box->inf = *point;
    box->sup = *point;
    return box;
}

RwBBox *BBoxAddPoint(RwBBox *box, const RwV3d *point)
{
    if (box->inf.x > point->x) box->inf.x = point->x;
    if (box->inf.y > point->y) box->inf.y = point->y;
    if (box->inf.z > point->z) box->inf.z = point->z;
    if (box->sup.x < point->x) box->sup.x = point->x;
    if (box->sup.y < point->y) box->sup.y = point->y;
    if (box->sup.z < point->z) box->sup.z = point->z;
    return box;
}
