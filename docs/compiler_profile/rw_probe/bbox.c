typedef float RwReal;
typedef int RwInt32;
typedef struct RwV3d { RwReal x, y, z; } RwV3d;
typedef struct RwBBox { RwV3d sup; RwV3d inf; } RwBBox;

RwBBox *BBoxCalculate(RwBBox *boundBox, const RwV3d *verts, RwInt32 numVerts)
{
    boundBox->inf = *verts;
    boundBox->sup = *verts;
    verts++;
    {
        RwInt32 i = numVerts - 1;
        while (i--)
    {
        if (boundBox->inf.x > verts->x) boundBox->inf.x = verts->x;
        if (boundBox->inf.y > verts->y) boundBox->inf.y = verts->y;
        if (boundBox->inf.z > verts->z) boundBox->inf.z = verts->z;
        if (boundBox->sup.x < verts->x) boundBox->sup.x = verts->x;
        if (boundBox->sup.y < verts->y) boundBox->sup.y = verts->y;
        if (boundBox->sup.z < verts->z) boundBox->sup.z = verts->z;
        verts++;
    }
    }
    return boundBox;
}

RwBBox *BBoxInitialize(RwBBox *boundBox, const RwV3d *vertex)
{
    boundBox->inf = *vertex;
    boundBox->sup = *vertex;
    return boundBox;
}

RwBBox *BBoxAddPoint(RwBBox *boundBox, const RwV3d *vertex)
{
    if (boundBox->inf.x > vertex->x) boundBox->inf.x = vertex->x;
    if (boundBox->inf.y > vertex->y) boundBox->inf.y = vertex->y;
    if (boundBox->inf.z > vertex->z) boundBox->inf.z = vertex->z;
    if (boundBox->sup.x < vertex->x) boundBox->sup.x = vertex->x;
    if (boundBox->sup.y < vertex->y) boundBox->sup.y = vertex->y;
    if (boundBox->sup.z < vertex->z) boundBox->sup.z = vertex->z;
    return boundBox;
}
