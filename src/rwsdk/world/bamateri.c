#include <rpworld.h>

typedef struct MaterialModuleData MaterialModuleData;
struct MaterialModuleData
{
    RwFreeList* freeList;
};

#define MATERIAL_DATA(var) \
    (((MaterialModuleData*)((U8*)RwEngineInstance + materialModule.globalsOffset))->var)

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
    MATERIAL_DATA(freeList) = RwFreeListCreate(materialTKList.sizeOfStruct, 20, 0);

    if (!MATERIAL_DATA(freeList))
    {
        return NULL;
    }

    materialModule.numInstances++;

    return instance;
}

void* _rpMaterialClose(void* instance, S32 offset, S32 size)
{
    if (MATERIAL_DATA(freeList))
    {
        RwFreeListDestroy(MATERIAL_DATA(freeList));
        MATERIAL_DATA(freeList) = NULL;
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
        RWSRCGLOBAL(memoryFree)(MATERIAL_DATA(freeList), material);
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
