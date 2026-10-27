#include <rpworld.h>

typedef struct rpMaterialGlobals rpMaterialGlobals;
struct rpMaterialGlobals
{
    RwFreeList* matFreeList;
};

#define MATERIALGLOBAL(var) \
    (((rpMaterialGlobals*)((U8*)RwEngineInstance + materialModule.globalsOffset))->var)

static RwPluginRegistry materialTKList = { sizeof(RpMaterial), sizeof(RpMaterial) };
static RwSurfaceProperties defaultSurfaceProperties = { 1.0f, 1.0f, 1.0f };
static RwModuleInfo materialModule;

void _rpMaterialSetDefaultSurfaceProperties(const RwSurfaceProperties* surfaceProps)
{
    if (!surfaceProps)
    {
        defaultSurfaceProperties.ambient = 1.0f;
        defaultSurfaceProperties.diffuse = 1.0f;
        defaultSurfaceProperties.specular = 1.0f;
        return;
    }

    defaultSurfaceProperties = *surfaceProps;
}

void* _rpMaterialOpen(void* instance, S32 offset, S32 size)
{
    materialModule.globalsOffset = offset;
    MATERIALGLOBAL(matFreeList) = RwFreeListCreate(materialTKList.sizeOfStruct, 20, 0);

    if (!MATERIALGLOBAL(matFreeList))
    {
        return NULL;
    }

    materialModule.numInstances++;

    return instance;
}

void* _rpMaterialClose(void* instance, S32 offset, S32 size)
{
    if (MATERIALGLOBAL(matFreeList))
    {
        RwFreeListDestroy(MATERIALGLOBAL(matFreeList));
        MATERIALGLOBAL(matFreeList) = NULL;
    }

    materialModule.numInstances--;

    return instance;
}

RpMaterial* RpMaterialSetTexture(RpMaterial* material, RwTexture* texture);

S32 RpMaterialDestroy(RpMaterial* material)
{
    if (material->refCount == 1)
    {
        _rwPluginRegistryDeInitObject(&materialTKList, material);
        RpMaterialSetTexture(material, NULL);
        RWSRCGLOBAL(freeListFree)(MATERIALGLOBAL(matFreeList), material);
    }
    else
    {
        material->refCount--;
    }

    return 1;
}

RpMaterial* RpMaterialSetTexture(RpMaterial* material, RwTexture* texture)
{
    if (texture)
    {
        texture->refCount++;
    }

    if (material->texture)
    {
        RwTextureDestroy(material->texture);
    }

    material->texture = texture;

    return material;
}

S32 RpMaterialRegisterPlugin(S32 size, U32 pluginID, RwPluginObjectConstructor constructCB,
                             RwPluginObjectDestructor destructCB, RwPluginObjectCopy copyCB)
{
    return _rwPluginRegistryAddPlugin(&materialTKList, size, pluginID, constructCB, destructCB,
                                      copyCB);
}

S32 RpMaterialRegisterPluginStream(U32 pluginID, RwPluginDataChunkReadCallBack readCB,
                                   RwPluginDataChunkWriteCallBack writeCB,
                                   RwPluginDataChunkGetSizeCallBack getSizeCB)
{
    return _rwPluginRegistryAddPluginStream(&materialTKList, pluginID, readCB, writeCB,
                                            getSizeCB);
}
