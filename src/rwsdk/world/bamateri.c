#include <rpworld.h>
#include <string.h>

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

// A new material: white, untextured, the default surface properties, one reference. The linker
// drops it in this game (no caller outside the stream reader, which inlines it).
RpMaterial* RpMaterialCreate(void)
{
    RpMaterial* material;
    RwRGBA white;

    material = (RpMaterial*)RWSRCGLOBAL(memoryAlloc)(MATERIAL_DATA(freeList));
    if (!material)
    {
        return NULL;
    }

    material->refCount = 1;

    white.red = 255;
    white.green = 255;
    white.blue = 255;
    white.alpha = 255;

    material->color = white;
    material->texture = NULL;
    material->pipeline = NULL;
    material->surfaceProps = defaultSurfaceProperties;

    _rwPluginRegistryInitObject(&materialTKList, material);

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

// The material's struct chunk: flags, color, a word the reader ignores, whether a texture chunk
// follows, and the surface properties (absent from older streams). The type name and flags,
// ignored and hasTexture are this project's own; color and surfaceProps follow the DWARF RpMaterial.
typedef struct MaterialStreamHeader MaterialStreamHeader;
struct MaterialStreamHeader
{
    S32 flags;
    RwRGBA color;
    S32 ignored;
    S32 hasTexture;
    RwSurfaceProperties surfaceProps;
};

// Reads a material: its struct chunk, the texture when it has one, and the plugin data. NULL
// (and the material released) when a chunk is missing or of an unsupported version.
RpMaterial* RpMaterialStreamRead(RwStream* stream)
{
    U32 size;
    U32 version;
    MaterialStreamHeader header;
    RpMaterial* material;

    if (!RwStreamFindChunk(stream, rwID_STRUCT, &size, &version))
    {
        return NULL;
    }

    if (version >= 0x31000 && version <= 0x33002)
    {
        RwRGBA color;

        memset(&header, 0, sizeof(header));
        if (RwStreamRead(stream, &header, size) != size)
        {
            return NULL;
        }

        color = header.color;
        RwMemNative32(&header, sizeof(header));
        header.color = color;

        material = RpMaterialCreate();
        if (!material)
        {
            return NULL;
        }

        material->color = header.color;

        if (size <= (U32)((U8*)&header.surfaceProps - (U8*)&header))
        {
            material->surfaceProps = defaultSurfaceProperties;
        }
        else
        {
            RwMemFloat32ToReal(&header.surfaceProps, sizeof(header.surfaceProps));
            material->surfaceProps = header.surfaceProps;
        }

        material->texture = NULL;

        if (header.hasTexture)
        {
            if (!RwStreamFindChunk(stream, rwID_TEXTURE, NULL, &version))
            {
                RpMaterialDestroy(material);
                return NULL;
            }

            if (version >= 0x31000 && version <= 0x33002)
            {
                material->texture = RwTextureStreamRead(stream);
            }
            else
            {
                RwError err;

                RpMaterialDestroy(material);
                err.pluginID = 2;
                err.errorCode = _rwerror(0x80000004);
                RwErrorSet(&err);
                return NULL;
            }
        }

        if (!_rwPluginRegistryReadDataChunks(&materialTKList, stream, material))
        {
            RpMaterialDestroy(material);
            return NULL;
        }

        return material;
    }
    else
    {
        RwError err;

        err.pluginID = 2;
        err.errorCode = _rwerror(0x80000004);
        RwErrorSet(&err);
        return NULL;
    }
}
