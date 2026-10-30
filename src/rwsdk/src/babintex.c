#include <rwcore.h>
#include <string.h>

// The size of a chunk header in the stream, the build number chunk headers are written with, and
// the size of a texture's filter and addressing word in its texture chunk.
#define CHUNKHEADER_SIZE 12
#define LIBRARY_BUILDNUM 0xFFFF
#define TEXTURE_STREAMDATASIZE 4

// How many characters a string chunk is read in at a time.
#define STRING_READCHUNK 64

// The size of the name and mask name buffers a texture chunk is read into.
#define TEXTURE_STREAMNAMESIZE 128

// The standard device function that reads a native texture from a stream.
#define STDFUNC_NATIVETEXTUREREAD 26

// The data of a texture dictionary's struct chunk.
typedef struct TexDictionaryStreamData TexDictionaryStreamData;
struct TexDictionaryStreamData
{
    S32 numTextures;
};

extern RwPluginRegistry textureTKList;
extern RwPluginRegistry texDictTKList;

S32 _rwStringStreamGetSize(const char* string)
{
    if (!string)
    {
        string = "";
    }

    return (rwstrlen(string) + 1 + 3) & ~3;
}

const char* _rwStringStreamWrite(const char* string, RwStream* stream)
{
    S32 size;

    if (!string)
    {
        string = "";
    }

    size = _rwStringStreamGetSize(string);

    if (!_rwStreamWriteVersionedChunkHeader(stream, rwID_STRING, size, rwLIBRARYCURRENTVERSION,
                                            LIBRARY_BUILDNUM))
    {
        return NULL;
    }

    if (!RwStreamWrite(stream, string, size))
    {
        return NULL;
    }

    return string;
}

static char* StringStreamRead(char* nativeString, RwStream* stream, U32 length)
{
    char buffer[STRING_READCHUNK];
    char* dst;

    if (!nativeString)
    {
        nativeString = (char*)RwMalloc(length);

        if (!nativeString)
        {
            RwError err;

            err.pluginID = 1;
            err.errorCode = _rwerror(0x80000013, length);
            RwErrorSet(&err);
            return NULL;
        }
    }

    dst = nativeString;

    while (length)
    {
        U32 bytesToRead = STRING_READCHUNK;
        U32 i;

        if (length <= STRING_READCHUNK)
        {
            bytesToRead = length;
        }

        if (RwStreamRead(stream, buffer, bytesToRead) != bytesToRead)
        {
            return NULL;
        }

        length -= bytesToRead;

        for (i = 0; i < bytesToRead; i++)
        {
            dst[i] = buffer[i];
        }

        dst += bytesToRead;
    }

    return nativeString;
}

static char* UnicodeStringStreamRead(char* nativeString, RwStream* stream, U32 length)
{
    U16 buffer[STRING_READCHUNK];
    char* dst;
    S32 allocated = 0;

    if (!nativeString)
    {
        nativeString = (char*)RwMalloc(length);

        if (!nativeString)
        {
            RwError err;

            err.pluginID = 1;
            err.errorCode = _rwerror(0x80000013, length);
            RwErrorSet(&err);
            return NULL;
        }

        allocated = 1;
    }

    dst = nativeString;

    while (length)
    {
        U32 bytesToRead = sizeof(buffer);
        U32 i;
        U32 charsToCopy;

        if (length <= sizeof(buffer))
        {
            bytesToRead = length;
        }

        if (RwStreamRead(stream, buffer, bytesToRead) != bytesToRead)
        {
            if (allocated)
            {
                RwFree(nativeString);
            }

            return NULL;
        }

        charsToCopy = bytesToRead >> 1;
        length -= bytesToRead;

        for (i = 0; i < charsToCopy; i++)
        {
            dst[i] = (char)buffer[i];
        }

        dst += charsToCopy;
    }

    return nativeString;
}

char* _rwStringStreamFindAndRead(char* string, RwStream* stream)
{
    U32 type;
    U32 length;
    U32 version;

    while (_rwStreamReadChunkHeader(stream, &type, &length, &version, NULL))
    {
        S32 versionOk = 0;

        if (version >= rwLIBRARYBASEVERSION && version <= rwLIBRARYCURRENTVERSION)
        {
            versionOk = 1;
        }

        if (!versionOk)
        {
            RwError err;

            err.pluginID = 1;
            err.errorCode = _rwerror(0x80000004);
            RwErrorSet(&err);
            return NULL;
        }

        if (type == rwID_STRING)
        {
            return StringStreamRead(string, stream, length);
        }

        if (type == rwID_UNICODESTRING)
        {
            return UnicodeStringStreamRead(string, stream, length);
        }

        if (!RwStreamSkip(stream, length))
        {
            return NULL;
        }
    }

    return NULL;
}

U32 RwTextureStreamGetSize(const RwTexture* texture)
{
    U32 size = CHUNKHEADER_SIZE + TEXTURE_STREAMDATASIZE;

    size += CHUNKHEADER_SIZE + _rwStringStreamGetSize(texture->name);
    size += CHUNKHEADER_SIZE + _rwStringStreamGetSize(texture->mask);
    size += CHUNKHEADER_SIZE + _rwPluginRegistryGetSize(&textureTKList, texture);

    return size;
}

const RwTexture* RwTextureStreamWrite(const RwTexture* texture, RwStream* stream)
{
    U32 data;
    U8 buildMipmaps;

    if (!_rwStreamWriteVersionedChunkHeader(stream, rwID_TEXTURE, RwTextureStreamGetSize(texture),
                                            rwLIBRARYCURRENTVERSION, LIBRARY_BUILDNUM))
    {
        return NULL;
    }

    if (!_rwStreamWriteVersionedChunkHeader(stream, rwID_STRUCT, TEXTURE_STREAMDATASIZE,
                                            rwLIBRARYCURRENTVERSION, LIBRARY_BUILDNUM))
    {
        return NULL;
    }

    if (texture->raster && !(texture->raster->cFormat & (rwRASTERFORMATAUTOMIPMAP >> 8)))
    {
        buildMipmaps = 1;
    }
    else
    {
        buildMipmaps = 0;
    }

    data = (texture->filterAddressing & 0xFFFF) | ((U32)buildMipmaps << 16);

    RwMemLittleEndian32(&data, sizeof(data));

    if (!RwStreamWrite(stream, &data, sizeof(data)))
    {
        return NULL;
    }

    if (!_rwStringStreamWrite(texture->name, stream))
    {
        return NULL;
    }

    if (!_rwStringStreamWrite(texture->mask, stream))
    {
        return NULL;
    }

    if (!_rwPluginRegistryWriteDataChunks(&textureTKList, stream, texture))
    {
        return NULL;
    }

    return texture;
}

S32 RwTextureRegisterPluginStream(U32 pluginID, RwPluginDataChunkReadCallBack readCB,
                                  RwPluginDataChunkWriteCallBack writeCB,
                                  RwPluginDataChunkGetSizeCallBack getSizeCB)
{
    return _rwPluginRegistryAddPluginStream(&textureTKList, pluginID, readCB, writeCB,
                                            getSizeCB);
}

RwTexture* RwTextureStreamRead(RwStream* stream)
{
    U32 size;
    U32 version;
    U32 data;
    U8 filterMode;
    U8 flags;
    S32 mipmapping;
    S32 autoMipmapping;
    RwTexture* texture;
    char textureName[TEXTURE_STREAMNAMESIZE];
    char maskName[TEXTURE_STREAMNAMESIZE];

    if (!RwStreamFindChunk(stream, rwID_STRUCT, &size, &version))
    {
        return NULL;
    }

    if (version >= rwLIBRARYBASEVERSION && version <= rwLIBRARYCURRENTVERSION)
    {
        memset(&data, 0, sizeof(data));

        if (RwStreamRead(stream, &data, size) != size)
        {
            return NULL;
        }

        RwMemNative32(&data, sizeof(data));

        if (!((data >> 12) & 0xF))
        {
            data |= (((data >> 8) & 0xF) << 12) & rwTEXTUREADDRESSINGVMASK;
        }

        filterMode = data & 0xFF;
        flags = (data >> 16) & 0xFF;

        mipmapping = RwTextureGetMipmapping();
        autoMipmapping = RwTextureGetAutoMipmapping();

        if (filterMode == rwFILTERMIPNEAREST || filterMode == rwFILTERMIPLINEAR ||
            filterMode == rwFILTERLINEARMIPNEAREST || filterMode == rwFILTERLINEARMIPLINEAR)
        {
            RwTextureSetMipmapping(TRUE);

            if (flags & 1)
            {
                RwTextureSetAutoMipmapping(FALSE);
            }
            else
            {
                RwTextureSetAutoMipmapping(TRUE);
            }
        }
        else
        {
            RwTextureSetMipmapping(FALSE);
            RwTextureSetAutoMipmapping(FALSE);
        }

        if (!_rwStringStreamFindAndRead(textureName, stream))
        {
            RwTextureSetMipmapping(mipmapping);
            RwTextureSetAutoMipmapping(autoMipmapping);
            return NULL;
        }

        if (!_rwStringStreamFindAndRead(maskName, stream))
        {
            RwTextureSetMipmapping(mipmapping);
            RwTextureSetAutoMipmapping(autoMipmapping);
            return NULL;
        }

        texture = RwTextureRead(textureName, maskName);

        if (!texture)
        {
            _rwPluginRegistrySkipDataChunks(&textureTKList, stream);
            RwTextureSetMipmapping(mipmapping);
            RwTextureSetAutoMipmapping(autoMipmapping);
            return NULL;
        }

        if (texture->refCount == 1)
        {
            texture->filterAddressing = data & 0xFFFF;
        }

        RwTextureSetMipmapping(mipmapping);
        RwTextureSetAutoMipmapping(autoMipmapping);

        if (!_rwPluginRegistryReadDataChunks(&textureTKList, stream, texture))
        {
            return NULL;
        }

        return texture;
    }
    else
    {
        RwError err;

        err.pluginID = 1;
        err.errorCode = _rwerror(0x80000004);
        RwErrorSet(&err);
        return NULL;
    }
}

static RwTexture* destroyTexture(RwTexture* texture, void* data)
{
    RwTextureDestroy(texture);

    return texture;
}

RwTexDictionary* RwTexDictionaryStreamRead(RwStream* stream)
{
    U32 size;
    U32 version;
    TexDictionaryStreamData data;
    RwTexDictionary* texDict;
    RwTexture* texture;

    if (!RwStreamFindChunk(stream, rwID_STRUCT, &size, &version))
    {
        return NULL;
    }

    if (version >= rwLIBRARYBASEVERSION && version <= rwLIBRARYCURRENTVERSION)
    {
        memset(&data, 0, sizeof(data));

        if (RwStreamRead(stream, &data, size) != size)
        {
            return NULL;
        }

        RwMemNative32(&data, sizeof(data));

        texDict = RwTexDictionaryCreate();

        if (!texDict)
        {
            return NULL;
        }

        while (data.numTextures--)
        {
            if (!RwStreamFindChunk(stream, rwID_TEXTURENATIVE, &size, &version))
            {
                RwTexDictionaryForAllTextures(texDict, destroyTexture, NULL);
                RwTexDictionaryDestroy(texDict);
                return NULL;
            }

            if (version < rwLIBRARYBASEVERSION || version > rwLIBRARYCURRENTVERSION)
            {
                RwError err;

                RwTexDictionaryForAllTextures(texDict, destroyTexture, NULL);
                RwTexDictionaryDestroy(texDict);

                err.pluginID = 1;
                err.errorCode = _rwerror(0x80000004);
                RwErrorSet(&err);
                return NULL;
            }

            if (!RWSRCGLOBAL(stdFunc)[STDFUNC_NATIVETEXTUREREAD](stream, &texture, size))
            {
                RwTexDictionaryForAllTextures(texDict, destroyTexture, NULL);
                RwTexDictionaryDestroy(texDict);
                return NULL;
            }

            if (!texture)
            {
                RwTexDictionaryForAllTextures(texDict, destroyTexture, NULL);
                RwTexDictionaryDestroy(texDict);
                return NULL;
            }

            if (!_rwPluginRegistryReadDataChunks(&textureTKList, stream, texture))
            {
                RwTexDictionaryForAllTextures(texDict, destroyTexture, NULL);
                RwTexDictionaryDestroy(texDict);
                return NULL;
            }

            RwTexDictionaryAddTexture(texDict, texture);
        }

        if (!_rwPluginRegistryReadDataChunks(&texDictTKList, stream, texDict))
        {
            RwTexDictionaryForAllTextures(texDict, destroyTexture, NULL);
            RwTexDictionaryDestroy(texDict);
            return NULL;
        }

        return texDict;
    }
    else
    {
        RwError err;

        err.pluginID = 1;
        err.errorCode = _rwerror(0x80000004);
        RwErrorSet(&err);
        return NULL;
    }
}
