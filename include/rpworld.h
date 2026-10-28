#ifndef RPWORLD_H
#define RPWORLD_H

#include <rwcore.h>

// RenderWare types get members once reconstructed code accesses them or a declared type
// embeds them; the others stay opaque.
typedef struct RpAtomic RpAtomic;
typedef struct RpSector RpSector;
typedef struct RpPolygon RpPolygon;
typedef struct RpVertexNormal RpVertexNormal;
typedef struct RwTexCoords RwTexCoords;
typedef struct RwResEntry RwResEntry;
typedef struct RpCollSector RpCollSector;
typedef struct RpMeshHeader RpMeshHeader;
typedef struct RxPipeline RxPipeline;

typedef struct RpMaterial RpMaterial;
struct RpMaterial
{
    RwTexture* texture;
    RwRGBA color;
    RxPipeline* pipeline;
    RwSurfaceProperties surfaceProps;
    S16 refCount;
    S16 pad;
};

typedef struct RpWorldSector RpWorldSector;
struct RpWorldSector
{
    S32 type;
    RpPolygon* polygons;
    RwV3d* vertices;
    RpVertexNormal* normals;
    RwTexCoords* texCoords[8];
    RwRGBA* preLitLum;
    RwResEntry* repEntry;
    RwLinkList collAtomicsInWorldSector;
    RwLinkList noCollAtomicsInWorldSector;
    RwLinkList lightsInWorldSector;
    RwBBox boundingBox;
    RwBBox tightBoundingBox;
    RpCollSector* colSectorRoot;
    RpMeshHeader* mesh;
    RxPipeline* pipeline;
    U16 matListWindowBase;
    U16 numVertices;
    U16 numPolygons;
    U16 pad;
};

typedef RpWorldSector* (*RpWorldSectorCallBackRender)(RpWorldSector* worldSector);

typedef enum RpWorldRenderOrder
{
    rpWORLDRENDERNARENDERORDER = 0,
    rpWORLDRENDERFRONT2BACK = 1,
    rpWORLDRENDERBACK2FRONT = 2,
    rpWORLDRENDERORDERFORCEENUMSIZEINT = 0x7FFFFFFF
} RpWorldRenderOrder;

typedef struct RpMaterialList RpMaterialList;
struct RpMaterialList
{
    RpMaterial** materials;
    S32 numMaterials;
    S32 space;
};

typedef struct RpWorld RpWorld;
struct RpWorld
{
    RwObject object;
    U32 flags;
    RpWorldRenderOrder renderOrder;
    RpMaterialList matList;
    RpSector* rootSector;
    S32 numTexCoordSets;
    S32 numClumpsInWorld;
    RwLLLink* currentClumpLink;
    RwLinkList clumpList;
    RwLinkList lightList;
    RwLinkList directionalLightList;
    RwV3d worldOrigin;
    RwBBox boundingBox;
    RwSurfaceProperties surfaceProps;
    RpWorldSectorCallBackRender renderCallBack;
    RxPipeline* pipeline;
};

typedef struct RpGeometry RpGeometry;
typedef struct RpClump RpClump;
typedef struct RpTriangle RpTriangle;

typedef struct RpInterpolator RpInterpolator;
struct RpInterpolator
{
    S32 flags;
    S16 startMorphTarget;
    S16 endMorphTarget;
    F32 time;
    F32 recipTime;
    F32 position;
};

typedef struct RpMorphTarget RpMorphTarget;
struct RpMorphTarget
{
    RpGeometry* parentGeom;
    RwSphere boundingSphere;
    RwV3d* verts;
    RwV3d* normals;
};

struct RpGeometry
{
    RwObject object;
    U32 flags;
    U16 lockedSinceLastInst;
    S16 refCount;
    S32 numTriangles;
    S32 numVertices;
    S32 numMorphTargets;
    S32 numTexCoordSets;
    RpMaterialList matList;
    RpTriangle* triangles;
    RwRGBA* preLitLum;
    RwTexCoords* texCoords[8];
    RwSurfaceProperties ignoredSurfaceProps;
    RpMeshHeader* mesh;
    RwResEntry* repEntry;
    RpMorphTarget* morphTarget;
};

typedef RpAtomic* (*RpAtomicCallBackRender)(RpAtomic* atomic);
typedef RpAtomic* (*RpAtomicCallBack)(RpAtomic* atomic, void* data);

struct RpAtomic
{
    RwObjectHasFrame object;
    RwResEntry* repEntry;
    RpGeometry* geometry;
    RwSphere boundingSphere;
    RwSphere worldBoundingSphere;
    RpClump* clump;
    RwLLLink inClumpLink;
    RpAtomicCallBackRender renderCallBack;
    RpInterpolator interpolator;
    U16 renderFrame;
    U16 pad;
    RwLinkList llWorldSectorsInAtomic;
    RxPipeline* pipeline;
};

struct RpClump
{
    RwObject object;
    RwLinkList atomicList;
    RwLLLink inWorldLink;
    RpClump* (*callback)(RpClump* clump, void* data);
    U16 renderFrame;
    U16 pad;
};

#define RpAtomicGetFrame(_atomic) ((RwFrame*)rwObjectGetParent(_atomic))
#define RpClumpGetFrame(_clump) ((RwFrame*)rwObjectGetParent(_clump))
#define RpClumpSetFrame(_clump, _frame) (rwObjectSetParent(_clump, _frame), (_clump))

typedef struct RpLight RpLight;
struct RpLight
{
    RwObjectHasFrame object;
    F32 radius;
    RwRGBAReal color;
    F32 minusCosAngle;
    RwLinkList WorldSectorsInLight;
    RwLLLink inWorld;
    U16 lightFrame;
    U16 pad;
};

#ifdef __cplusplus
extern "C" {
#endif

S32 RpLightDestroy(RpLight* light);

S32 RpClumpDestroy(RpClump* clump);
RpClump* RpClumpForAllAtomics(RpClump* clump, RpAtomicCallBack callback, void* pData);
RpClump* RpClumpStreamRead(RwStream* stream);

RpWorld* RpWorldAddCamera(RpWorld* world, RwCamera* camera);
RpWorld* RpWorldRemoveCamera(RpWorld* world, RwCamera* camera);
S32 RpWorldDestroy(RpWorld* world);

#ifdef __cplusplus
}
#endif

#endif
