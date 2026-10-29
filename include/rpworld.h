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
typedef struct RpCollSector RpCollSector;
typedef struct RpMeshHeader RpMeshHeader;

struct RpMeshHeader
{
    U32 flags;
    U16 numMeshes;
    U16 serialNum;
    U32 totalIndicesInMesh;
    U32 firstMeshOffset;
};

typedef struct RpMaterial RpMaterial;

typedef struct RpMesh RpMesh;
struct RpMesh
{
    U16* indices;
    U32 numIndices;
    RpMaterial* material;
};

typedef RpMesh* (*RpMeshCallBack)(RpMesh* mesh, RpMeshHeader* meshHeader, void* pData);
typedef RpMaterial* (*RpMaterialCallBack)(RpMaterial* material, void* data);
struct RpMaterial
{
    RwTexture* texture;
    RwRGBA color;
    RxPipeline* pipeline;
    RwSurfaceProperties surfaceProps;
    S16 refCount;
    S16 pad;
};

struct RpSector
{
    S32 type;
};

typedef struct RpPlaneSector RpPlaneSector;
struct RpPlaneSector
{
    S32 type;
    F32 value;
    RpSector* leftSubTree;
    RpSector* rightSubTree;
    F32 leftValue;
    F32 rightValue;
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
typedef RpWorldSector* (*RpWorldSectorCallBack)(RpWorldSector* worldSector, void* data);

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

typedef enum RpInterpolatorFlag
{
    rpINTERPOLATORDIRTYINSTANCE = 0x01,
    rpINTERPOLATORDIRTYSPHERE = 0x02,
    rpINTERPOLATORNOFRAMEDIRTY = 0x04,
    rpINTERPOLATORFLAGFORCEENUMSIZEINT = 0x7FFFFFFF
} RpInterpolatorFlag;

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

#define rpATOMICCOLLISIONTEST 0x01

#define rpGEOMETRYNATIVE 0x01000000

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

// Partial: the GameCube layout has the light and camera lists the PS2 DWARF lacks; the members
// after them are not declared.
struct RpClump
{
    RwObject object;
    RwLinkList atomicList;
    RwLinkList lightList;
    RwLinkList cameraList;
    RwLLLink inWorldLink;
    U8 unk28[0x8];
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

#define rpLIGHTPOSITIONINGSTART 0x80

typedef enum RpLightType
{
    rpNALIGHTTYPE = 0,
    rpLIGHTDIRECTIONAL,
    rpLIGHTAMBIENT,
    rpLIGHTPOINT = rpLIGHTPOSITIONINGSTART,
    rpLIGHTSPOT,
    rpLIGHTSPOTSOFT,
    rpLIGHTTYPEFORCEENUMSIZEINT = 0x7FFFFFFF
} RpLightType;

#define RpLightGetType(_light) ((RpLightType)rwObjectGetSubType((_light)))

typedef RwCamera* (*RwCameraCallBack)(RwCamera* camera, void* data);
typedef RpLight* (*RpLightCallBack)(RpLight* light, void* data);

#ifdef __cplusplus
extern "C" {
#endif

S32 RpLightDestroy(RpLight* light);
S32 RpLightRegisterPlugin(S32 size, U32 pluginID, RwPluginObjectConstructor constructCB,
                          RwPluginObjectDestructor destructCB, RwPluginObjectCopy copyCB);
S32 RpLightRegisterPluginStream(U32 pluginID, RwPluginDataChunkReadCallBack readCB,
                                RwPluginDataChunkWriteCallBack writeCB,
                                RwPluginDataChunkGetSizeCallBack getSizeCB);
S32 RpMaterialRegisterPlugin(S32 size, U32 pluginID, RwPluginObjectConstructor constructCB,
                             RwPluginObjectDestructor destructCB, RwPluginObjectCopy copyCB);
S32 RpMaterialRegisterPluginStream(U32 pluginID, RwPluginDataChunkReadCallBack readCB,
                                   RwPluginDataChunkWriteCallBack writeCB,
                                   RwPluginDataChunkGetSizeCallBack getSizeCB);

RpGeometry* RpGeometryLock(RpGeometry* geometry, S32 lockMode);

S32 RpAtomicDestroy(RpAtomic* atomic);
const RwSphere* RpAtomicGetWorldBoundingSphere(RpAtomic* atomic);
S32 RpAtomicRegisterPlugin(S32 size, U32 pluginID, RwPluginObjectConstructor constructCB,
                           RwPluginObjectDestructor destructCB, RwPluginObjectCopy copyCB);
S32 RpAtomicRegisterPluginStream(U32 pluginID, RwPluginDataChunkReadCallBack readCB,
                                 RwPluginDataChunkWriteCallBack writeCB,
                                 RwPluginDataChunkGetSizeCallBack getSizeCB);
S32 RpAtomicSetStreamAlwaysCallBack(U32 pluginID, RwPluginDataChunkAlwaysCallBack alwaysCB);
S32 RpAtomicSetStreamRightsCallBack(U32 pluginID, RwPluginDataChunkRightsCallBack rightsCB);
S32 RpAtomicGetPluginOffset(U32 pluginID);
S32 RpClumpRegisterPlugin(S32 size, U32 pluginID, RwPluginObjectConstructor constructCB,
                          RwPluginObjectDestructor destructCB, RwPluginObjectCopy copyCB);
S32 RpGeometryRegisterPlugin(S32 size, U32 pluginID, RwPluginObjectConstructor constructCB,
                             RwPluginObjectDestructor destructCB, RwPluginObjectCopy copyCB);
S32 RpGeometryRegisterPluginStream(U32 pluginID, RwPluginDataChunkReadCallBack readCB,
                                   RwPluginDataChunkWriteCallBack writeCB,
                                   RwPluginDataChunkGetSizeCallBack getSizeCB);
S32 RpWorldSectorRegisterPlugin(S32 size, U32 pluginID, RwPluginObjectConstructor constructCB,
                                RwPluginObjectDestructor destructCB, RwPluginObjectCopy copyCB);
S32 RpWorldSectorRegisterPluginStream(U32 pluginID, RwPluginDataChunkReadCallBack readCB,
                                      RwPluginDataChunkWriteCallBack writeCB,
                                      RwPluginDataChunkGetSizeCallBack getSizeCB);
S32 RpWorldRegisterPlugin(S32 size, U32 pluginID, RwPluginObjectConstructor constructCB,
                          RwPluginObjectDestructor destructCB, RwPluginObjectCopy copyCB);
S32 RpWorldRegisterPluginStream(U32 pluginID, RwPluginDataChunkReadCallBack readCB,
                                RwPluginDataChunkWriteCallBack writeCB,
                                RwPluginDataChunkGetSizeCallBack getSizeCB);

S32 RpClumpDestroy(RpClump* clump);
RpClump* RpClumpAddAtomic(RpClump* clump, RpAtomic* atomic);
RpClump* RpClumpRemoveAtomic(RpClump* clump, RpAtomic* atomic);
RpClump* RpClumpForAllAtomics(RpClump* clump, RpAtomicCallBack callback, void* pData);
RpClump* RpClumpForAllCameras(RpClump* clump, RwCameraCallBack callback, void* pData);
RpClump* RpClumpForAllLights(RpClump* clump, RpLightCallBack callback, void* pData);
RpClump* RpClumpStreamRead(RwStream* stream);

RpWorld* RpWorldAddCamera(RpWorld* world, RwCamera* camera);
RpWorld* RpWorldRemoveCamera(RpWorld* world, RwCamera* camera);
S32 RpWorldDestroy(RpWorld* world);

RxPipeline* RpWorldSetDefaultSectorPipeline(RxPipeline* pipeline);
RxPipeline* RpAtomicSetDefaultPipeline(RxPipeline* pipeline);
void _rpWorldPipelineClose(void);
S32 _rpWorldPipelineOpen(void);
S32 _rpWorldPipeAttach(void);
S32 _rpCreatePlatformMaterialPipelines(void);
void _rpDestroyPlatformMaterialPipelines(void);
S32 _rpCreatePlatformWorldSectorPipelines(void);
void _rpDestroyPlatformWorldSectorPipelines(void);
S32 _rpCreatePlatformAtomicPipelines(void);
void _rpDestroyPlatformAtomicPipelines(void);

RxNodeDefinition* RxNodeDefinitionGetLight(void);
RxNodeDefinition* RxNodeDefinitionGetPostLight(void);
RxNodeDefinition* RxNodeDefinitionGetAtomicInstance(void);
RxNodeDefinition* RxNodeDefinitionGetAtomicEnumerateLights(void);
RxNodeDefinition* RxNodeDefinitionGetMaterialScatter(void);
RxNodeDefinition* RxNodeDefinitionGetWorldSectorInstance(void);
RxNodeDefinition* RxNodeDefinitionGetWorldSectorEnumerateLights(void);
RxNodeDefinition* RxNodeDefinitionGetGameCubeAtomicAllInOne(void);
RxNodeDefinition* RxNodeDefinitionGetGameCubeWorldSectorAllInOne(void);

#ifdef __cplusplus
}
#endif

#endif
