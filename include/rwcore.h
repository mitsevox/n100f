#ifndef RWCORE_H
#define RWCORE_H

#include "types.h"

#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE 1
#endif

// RenderWare types get members once reconstructed code accesses them or a declared type
// embeds them; the others stay opaque.
typedef struct RwLLLink RwLLLink;
struct RwLLLink
{
    RwLLLink* next;
    RwLLLink* prev;
};

typedef struct RwLinkList RwLinkList;
struct RwLinkList
{
    RwLLLink link;
};

typedef struct RwObject RwObject;
struct RwObject
{
    U8 type;
    U8 subType;
    U8 flags;
    U8 privateFlags;
    void* parent;
};

typedef struct RwObjectHasFrame RwObjectHasFrame;
struct RwObjectHasFrame
{
    RwObject object;
    RwLLLink lFrame;
    RwObjectHasFrame* (*sync)(RwObjectHasFrame*);
};

typedef struct RwRGBAReal RwRGBAReal;
struct RwRGBAReal
{
    F32 red;
    F32 green;
    F32 blue;
    F32 alpha;
};

typedef struct RwV3d RwV3d;
struct RwV3d
{
    F32 x;
    F32 y;
    F32 z;
};

typedef struct RwBBox RwBBox;
struct RwBBox
{
    RwV3d sup;
    RwV3d inf;
};

typedef struct RwRGBA RwRGBA;
struct RwRGBA
{
    U8 red;
    U8 green;
    U8 blue;
    U8 alpha;
};

typedef enum RwFogType
{
    rwFOGTYPENAFOGTYPE = 0,
    rwFOGTYPELINEAR = 1,
    rwFOGTYPEEXPONENTIAL = 2,
    rwFOGTYPEEXPONENTIAL2 = 3,
    rwFOGTYPEFORCEENUMSIZEINT = 0x7FFFFFFF
} RwFogType;

typedef struct RwMatrixTag RwMatrixTag;
struct RwMatrixTag
{
    RwV3d right;
    U32 flags;
    RwV3d up;
    U32 pad1;
    RwV3d at;
    U32 pad2;
    RwV3d pos;
    U32 pad3;
};

typedef struct RwMatrixTag RwMatrix;
typedef struct RwCamera RwCamera;
typedef struct RwFrame RwFrame;
typedef struct RwRaster RwRaster;
typedef struct RwStream RwStream;

typedef struct RwV2d RwV2d;
struct RwV2d
{
    F32 x;
    F32 y;
};

typedef struct RwPlane RwPlane;
struct RwPlane
{
    RwV3d normal;
    F32 distance;
};

typedef struct RwFrustumPlane RwFrustumPlane;
struct RwFrustumPlane
{
    RwPlane plane;
    U8 closestX;
    U8 closestY;
    U8 closestZ;
    U8 pad;
};

typedef enum RwCameraProjection
{
    rwNACAMERAPROJECTION = 0,
    rwPERSPECTIVE = 1,
    rwPARALLEL = 2,
    rwCAMERAPROJECTIONFORCEENUMSIZEINT = 0x7FFFFFFF
} RwCameraProjection;

// Partial: the frustum bounding box and corners after frustumPlanes are not declared.
struct RwCamera
{
    RwObjectHasFrame object;
    RwCameraProjection projectionType;
    RwCamera* (*beginUpdate)(RwCamera*);
    RwCamera* (*endUpdate)(RwCamera*);
    RwMatrix viewMatrix;
    RwRaster* frameBuffer;
    RwRaster* zBuffer;
    RwV2d viewWindow;
    RwV2d recipViewWindow;
    RwV2d viewOffset;
    F32 nearPlane;
    F32 farPlane;
    F32 fogPlane;
    F32 zScale;
    F32 zShift;
    U16 renderFrame;
    U16 pad;
    RwFrustumPlane frustumPlanes[6];
};

// Partial: only the members up to the world matrix are declared.
struct RwFrame
{
    RwObject object;
    RwLLLink inDirtyListLink;
    RwMatrix modelling;
    RwMatrix ltm;
};

typedef RwFrame* (*RwFrameCallBack)(RwFrame* frame, void* data);

typedef struct RwSphere RwSphere;
struct RwSphere
{
    RwV3d center;
    F32 radius;
};

typedef struct RwMemory RwMemory;
struct RwMemory
{
    U8* start;
    U32 length;
};

typedef enum RwStreamType
{
    rwNASTREAM = 0,
    rwSTREAMFILE,
    rwSTREAMFILENAME,
    rwSTREAMMEMORY,
    rwSTREAMCUSTOM,
    rwSTREAMTYPEFORCEENUMSIZEINT = 0x7FFFFFFF
} RwStreamType;

typedef enum RwStreamAccessType
{
    rwNASTREAMACCESS = 0,
    rwSTREAMREAD,
    rwSTREAMWRITE,
    rwSTREAMAPPEND,
    rwSTREAMACCESSTYPEFORCEENUMSIZEINT = 0x7FFFFFFF
} RwStreamAccessType;

typedef enum RwFrustumTestResult
{
    rwSPHEREOUTSIDE = 0,
    rwSPHEREBOUNDARY = 1,
    rwSPHEREINSIDE = 2,
    rwFRUSTUMTESTRESULTFORCEENUMSIZEINT = 0x7FFFFFFF
} RwFrustumTestResult;

#define rwID_CLUMP 0x10

#define rwMATRIXTYPEORTHONORMAL 0x00000003
#define rwMATRIXINTERNALIDENTITY 0x00020000

#define RwMatrixSetIdentityMacro(m)                                                                \
    do                                                                                             \
    {                                                                                              \
        (m)->right.x = (m)->up.y = (m)->at.z = (F32)((1.0));                                       \
        (m)->right.y = (m)->right.z = (m)->up.x = (F32)((0.0));                                    \
        (m)->up.z = (m)->at.x = (m)->at.y = (F32)((0.0));                                          \
        (m)->pos.x = (m)->pos.y = (m)->pos.z = (F32)((0.0));                                       \
        (m)->flags |= (rwMATRIXINTERNALIDENTITY | rwMATRIXTYPEORTHONORMAL);                        \
    } while (0)

#define RwCameraGetCurrentCamera() ((RwCamera*)RWSRCGLOBAL(curCamera))

typedef enum RwCameraClearMode
{
    rwCAMERACLEARIMAGE = 0x1,
    rwCAMERACLEARZ = 0x2,
    rwCAMERACLEARSTENCIL = 0x4,
    rwCAMERACLEARMODEFORCEENUMSIZEINT = 0x7FFFFFFF
} RwCameraClearMode;

typedef enum RwRasterType
{
    rwRASTERTYPENORMAL = 0x00,
    rwRASTERTYPEZBUFFER = 0x01,
    rwRASTERTYPECAMERA = 0x02,
    rwRASTERTYPETEXTURE = 0x04,
    rwRASTERTYPECAMERATEXTURE = 0x05,
    rwRASTERTYPEMASK = 0x07,
    rwRASTERDONTALLOCATE = 0x80,
    rwRASTERTYPEFORCEENUMSIZEINT = 0x7FFFFFFF
} RwRasterType;

typedef enum RwRenderState
{
    rwRENDERSTATENARENDERSTATE = 0,
    rwRENDERSTATETEXTURERASTER,
    rwRENDERSTATETEXTUREADDRESS,
    rwRENDERSTATETEXTUREADDRESSU,
    rwRENDERSTATETEXTUREADDRESSV,
    rwRENDERSTATETEXTUREPERSPECTIVE,
    rwRENDERSTATEZTESTENABLE,
    rwRENDERSTATESHADEMODE,
    rwRENDERSTATEZWRITEENABLE,
    rwRENDERSTATETEXTUREFILTER,
    rwRENDERSTATESRCBLEND,
    rwRENDERSTATEDESTBLEND,
    rwRENDERSTATEVERTEXALPHAENABLE,
    rwRENDERSTATEBORDERCOLOR,
    rwRENDERSTATEFOGENABLE,
    rwRENDERSTATEFOGCOLOR,
    rwRENDERSTATEFOGTYPE,
    rwRENDERSTATEFOGDENSITY,
    rwRENDERSTATEFOGTABLE,
    rwRENDERSTATEALPHAPRIMITIVEBUFFER,
    rwRENDERSTATECULLMODE,
    rwRENDERSTATEFORCEENUMSIZEINT = 0x7FFFFFFF
} RwRenderState;

#define rwObjectGetParent(object) (((const RwObject*)(object))->parent)
#define RwFrameGetMatrix(frame) (&(frame)->modelling)
#define RwCameraGetFrame(camera) ((RwFrame*)rwObjectGetParent(camera))
#define RwCameraSetFrame(camera, frame) \
    (_rwObjectHasFrameSetFrame((camera), (frame)), (camera))
#define RwCameraGetRaster(camera) ((camera)->frameBuffer)
#define RwCameraSetRaster(camera, raster) (((camera)->frameBuffer = (raster)), (camera))
#define RwCameraGetZRaster(camera) ((camera)->zBuffer)
#define RwCameraSetZRaster(camera, zRaster) (((camera)->zBuffer = (zRaster)), (camera))
#define RwCameraSetFogDistance(camera, fogDistance) \
    (((camera)->fogPlane = (fogDistance)), (camera))

typedef struct RwError RwError;
struct RwError
{
    S32 pluginID;
    S32 errorCode;
};

typedef struct RwModuleInfo RwModuleInfo;
struct RwModuleInfo
{
    S32 globalsOffset;
    S32 numInstances;
};

typedef struct RwMemoryFunctions RwMemoryFunctions;
struct RwMemoryFunctions
{
    void* (*rwmalloc)(U32 size);
    void (*rwfree)(void* mem);
    void* (*rwrealloc)(void* mem, U32 newSize);
    void* (*rwcalloc)(U32 numObj, U32 sizeObj);
};

typedef struct RwFileFunctions RwFileFunctions;
struct RwFileFunctions
{
    S32 (*rwfexist)(const char* name);
    void* (*rwfopen)(const char* name, const char* access);
    S32 (*rwfclose)(void* fptr);
    U32 (*rwfread)(void* addr, U32 size, U32 count, void* fptr);
    U32 (*rwfwrite)(const void* addr, U32 size, U32 count, void* fptr);
    char* (*rwfgets)(char* buffer, S32 maxLen, void* fptr);
    S32 (*rwfputs)(const char* buffer, void* fptr);
    S32 (*rwfeof)(void* fptr);
    S32 (*rwfseek)(void* fptr, S32 offset, S32 origin);
    S32 (*rwfflush)(void* fptr);
    S32 (*rwftell)(void* fptr);
};

typedef struct RwSurfaceProperties RwSurfaceProperties;
struct RwSurfaceProperties
{
    F32 ambient;
    F32 specular;
    F32 diffuse;
};

// Partial: only refCount is declared; the GameCube layout has it at +0x54.
typedef struct RwTexture RwTexture;
struct RwTexture
{
    U8 unk0[0x54];
    S32 refCount;
};

typedef struct RwFreeList RwFreeList;

typedef void* (*RwMemoryAllocFn)(RwFreeList* fl);
typedef RwFreeList* (*RwMemoryFreeFn)(RwFreeList* fl, void* pData);

// Partial: the members after the two object sizes are not declared.
typedef struct RwPluginRegistry RwPluginRegistry;
struct RwPluginRegistry
{
    S32 sizeOfStruct;
    S32 origSizeOfStruct;
    U8 unk8[0x10];
};

typedef void* (*RwPluginObjectConstructor)(void* object, S32 offsetInObject, S32 sizeInObject);
typedef void* (*RwPluginObjectDestructor)(void* object, S32 offsetInObject, S32 sizeInObject);
typedef void* (*RwPluginObjectCopy)(void* dstObject, const void* srcObject, S32 offsetInObject,
                                    S32 sizeInObject);
typedef RwStream* (*RwPluginDataChunkReadCallBack)(RwStream* stream, S32 binaryLength,
                                                   void* object, S32 offsetInObject,
                                                   S32 sizeInObject);
typedef RwStream* (*RwPluginDataChunkWriteCallBack)(RwStream* stream, S32 binaryLength,
                                                    const void* object, S32 offsetInObject,
                                                    S32 sizeInObject);
typedef S32 (*RwPluginDataChunkGetSizeCallBack)(const void* object, S32 offsetInObject,
                                                 S32 sizeInObject);

// Partial: only the members reconstructed code reaches are declared.
typedef struct RwGlobals RwGlobals;
struct RwGlobals
{
    void* curCamera;
    void* curWorld;
    U16 renderFrame;
    U16 lightFrame;
    U8 unkC[0xB8];
    RwFileFunctions fileFuncs;
    U8 unkF0[0x40];
    RwMemoryFunctions memoryFuncs;
    RwMemoryAllocFn memoryAlloc;
    RwMemoryFreeFn memoryFree;
};

#define RWSRCGLOBAL(var) (((RwGlobals*)RwEngineInstance)->var)

#ifdef __cplusplus
extern "C" {
#endif

extern void* RwEngineInstance;

RwCamera* RwCameraCreate();
S32 RwCameraDestroy(RwCamera* camera);
RwCamera* RwCameraBeginUpdate(RwCamera* camera);
RwCamera* RwCameraEndUpdate(RwCamera* camera);
RwCamera* RwCameraClear(RwCamera* camera, RwRGBA* colour, S32 clearMode);
RwCamera* RwCameraShowRaster(RwCamera* camera, void* pDev, U32 flags);
RwCamera* RwCameraSetNearClipPlane(RwCamera* camera, F32 nearClip);
RwCamera* RwCameraSetFarClipPlane(RwCamera* camera, F32 farClip);
RwCamera* RwCameraSetViewWindow(RwCamera* camera, const RwV2d* viewWindow);
void _rwObjectHasFrameSetFrame(void* object, RwFrame* frame);
RwRaster* RwRasterCreate(S32 width, S32 height, S32 depth, S32 flags);
S32 RwRasterDestroy(RwRaster* raster);
S32 RwRenderStateSet(RwRenderState state, void* value);
RwMatrix* RwMatrixUpdate(RwMatrix* matrix);
RwFrame* RwFrameCreate();
S32 RwFrameDestroy(RwFrame* frame);
RwFrame* RwFrameOrthoNormalize(RwFrame* frame);
RwFrame* RwFrameForAllChildren(RwFrame* frame, RwFrameCallBack callBack, void* data);
RwFrame* RwFrameGetRoot(const RwFrame* frame);
S32 RwFrameDestroyHierarchy(RwFrame* frame);
RwMatrix* RwMatrixMultiply(RwMatrix* matrixOut, const RwMatrix* MatrixIn1, const RwMatrix* matrixIn2);
RwV3d* RwV3dTransformPoints(RwV3d* pointsOut, const RwV3d* pointsIn, S32 numPoints,
                            const RwMatrix* matrix);
RwFrustumTestResult RwCameraFrustumTestSphere(const RwCamera* camera, const RwSphere* sphere);
RwStream* RwStreamOpen(RwStreamType type, RwStreamAccessType accessType, const void* pData);
S32 RwStreamClose(RwStream* stream, void* pData);
S32 RwStreamFindChunk(RwStream* stream, U32 type, U32* lengthOut, U32* versionOut);
void _rwFrameSyncDirty();

RwError* RwErrorSet(RwError* code);
S32 _rwerror(S32 code, ...);

RwFreeList* RwFreeListCreate(S32 entrySize, S32 entriesPerBlock, S32 alignment);
S32 RwFreeListDestroy(RwFreeList* freeList);
RwFreeList* RwFreeListForAllUsed(RwFreeList* freeList, void (*callBack)(void* mem, void* data),
                                 void* data);

S32 _rwPluginRegistryAddPlugin(RwPluginRegistry* reg, S32 size, U32 pluginID,
                               RwPluginObjectConstructor constructCB,
                               RwPluginObjectDestructor destructCB,
                               RwPluginObjectCopy copyCB);
S32 _rwPluginRegistryAddPluginStream(RwPluginRegistry* reg, U32 pluginID,
                                     RwPluginDataChunkReadCallBack readCB,
                                     RwPluginDataChunkWriteCallBack writeCB,
                                     RwPluginDataChunkGetSizeCallBack getSizeCB);
const RwPluginRegistry* _rwPluginRegistryInitObject(const RwPluginRegistry* reg, void* object);
const RwPluginRegistry* _rwPluginRegistryDeInitObject(const RwPluginRegistry* reg, void* object);

void _rwObjectHasFrameReleaseFrame(void* object);

S32 RwTextureDestroy(RwTexture* texture);

RwFrame* RwFrameUpdateObjects(RwFrame* frame);

F32 _rwSqrt(F32 num);
F32 _rwInvSqrt(F32 num);

#ifdef __cplusplus
}
#endif

#endif
