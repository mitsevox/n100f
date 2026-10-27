#ifndef RWCORE_H
#define RWCORE_H

#include "types.h"

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
typedef void* (*RwPluginDataChunkReadCallBack)(void* stream, S32 binaryLength, void* object,
                                               S32 offsetInObject, S32 sizeInObject);
typedef void* (*RwPluginDataChunkWriteCallBack)(void* stream, S32 binaryLength,
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
    void* (*freeListAlloc)(RwFreeList* freeList);
    RwFreeList* (*freeListFree)(RwFreeList* freeList, void* element);
};

#define RWSRCGLOBAL(var) (((RwGlobals*)RwEngineInstance)->var)

#ifdef __cplusplus
extern "C" {
#endif

extern void* RwEngineInstance;

RwCamera* RwCameraShowRaster(RwCamera* camera, void* pDev, U32 flags);
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
