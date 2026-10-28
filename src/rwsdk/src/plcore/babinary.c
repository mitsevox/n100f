#include <rwcore.h>
#include <string.h>

// Size of the stack buffer the real and integer writers convert through.
#define CONVERT_BUFFER_SIZE 256

// A chunk header as it is stored in a stream.
typedef struct BinaryChunkHeader BinaryChunkHeader;
struct BinaryChunkHeader
{
    U32 type;
    U32 length;
    U32 libraryID;
};

static S32 ChunkIsComplex(RwChunkHeaderInfo* chunkHeaderInfo)
{
    S32 isComplex;

    switch (chunkHeaderInfo->type)
    {
    case rwID_STRUCT:
        isComplex = FALSE;
        break;
    case rwID_STRING:
        isComplex = FALSE;
        break;
    case rwID_EXTENSION:
        isComplex = FALSE;
        break;
    case rwID_CAMERA:
        isComplex = TRUE;
        break;
    case rwID_TEXTURE:
        isComplex = TRUE;
        break;
    case rwID_MATERIAL:
        isComplex = TRUE;
        break;
    case rwID_MATLIST:
        isComplex = TRUE;
        break;
    case rwID_ATOMICSECT:
        isComplex = TRUE;
        break;
    case rwID_PLANESECT:
        isComplex = TRUE;
        break;
    case rwID_WORLD:
        isComplex = TRUE;
        break;
    case rwID_MATRIX:
        isComplex = FALSE;
        break;
    case rwID_FRAMELIST:
        isComplex = TRUE;
        break;
    case rwID_GEOMETRY:
        isComplex = TRUE;
        break;
    case rwID_CLUMP:
        isComplex = TRUE;
        break;
    case rwID_LIGHT:
        isComplex = TRUE;
        break;
    case rwID_UNICODESTRING:
        isComplex = FALSE;
        break;
    case rwID_ATOMIC:
        isComplex = TRUE;
        break;
    case rwID_GEOMETRYLIST:
        isComplex = TRUE;
        break;
    default:
        isComplex = FALSE;
        break;
    }

    return isComplex;
}

S32 _rwStreamReadChunkHeader(RwStream* stream, U32* type, U32* length, U32* version, U32* buildNum)
{
    BinaryChunkHeader header;
    S32 readOK = RwStreamRead(stream, &header, sizeof(header)) == sizeof(header);

    if (!readOK)
    {
        RwError err;
        err.pluginID = 1;
        err.errorCode = _rwerror(0x8000001A);
        RwErrorSet(&err);
        return FALSE;
    }

    RwMemNative32(&header, sizeof(header));

    if (type)
    {
        *type = header.type;
    }

    if (length)
    {
        *length = header.length;
    }

    if (!(header.libraryID & 0xFFFF0000))
    {
        if (version)
        {
            *version = header.libraryID << 8;
        }

        if (buildNum)
        {
            *buildNum = 0;
        }
    }
    else
    {
        if (version)
        {
            *version = (((header.libraryID >> 14) & 0x3FF00) + 0x30000) |
                       ((header.libraryID >> 16) & 0x3F);
        }

        if (buildNum)
        {
            *buildNum = header.libraryID & 0xFFFF;
        }
    }

    return TRUE;
}

RwStream* _rwStreamWriteVersionedChunkHeader(RwStream* stream, S32 type, S32 size, U32 version,
                                             U32 buildNum)
{
    BinaryChunkHeader header;

    header.type = type;
    header.length = size;
    header.libraryID = (((version - 0x30000) & 0x3FF00) << 14) | ((version & 0x3F) << 16) |
                       (buildNum & 0xFFFF);

    RwMemLittleEndian32(&header, sizeof(header));

    return RwStreamWrite(stream, &header, sizeof(header));
}

S32 RwStreamFindChunk(RwStream* stream, U32 type, U32* lengthOut, U32* versionOut)
{
    U32 readType;
    U32 readLength;
    U32 readVersion;

    while (_rwStreamReadChunkHeader(stream, &readType, &readLength, &readVersion, NULL))
    {
        if (readType == type)
        {
            if (readVersion < rwLIBRARYBASEVERSION)
            {
                RwError err;
                err.pluginID = 1;
                err.errorCode = _rwerror(0x80000004);
                RwErrorSet(&err);
                return FALSE;
            }

            if (readVersion > rwLIBRARYCURRENTVERSION)
            {
                RwError err;
                err.pluginID = 1;
                err.errorCode = _rwerror(0x80000004);
                RwErrorSet(&err);
                return FALSE;
            }

            if (lengthOut)
            {
                *lengthOut = readLength;
            }

            if (versionOut)
            {
                *versionOut = readVersion;
            }

            return TRUE;
        }

        if (!RwStreamSkip(stream, readLength))
        {
            return FALSE;
        }
    }

    return FALSE;
}

void* RwMemLittleEndian32(void* mem, U32 size)
{
    U32* word = (U32*)mem;

    size >>= 2;
    while (size--)
    {
        *word = (*word >> 24) | ((*word >> 8) & 0xFF00) | ((*word << 8) & 0xFF0000) | (*word << 24);
        word++;
    }

    return mem;
}

void* RwMemLittleEndian16(void* mem, U32 size)
{
    U16* half = (U16*)mem;

    size >>= 1;
    while (size--)
    {
        *half = (*half >> 8) | (*half << 8);
        half++;
    }

    return mem;
}

void* RwMemNative32(void* mem, U32 size)
{
    U32* word = (U32*)mem;

    size >>= 2;
    while (size--)
    {
        *word = (*word >> 24) | ((*word >> 8) & 0xFF00) | ((*word << 8) & 0xFF0000) | (*word << 24);
        word++;
    }

    return mem;
}

void* RwMemNative16(void* mem, U32 size)
{
    U16* half = (U16*)mem;

    size >>= 1;
    while (size--)
    {
        *half = (*half >> 8) | (*half << 8);
        half++;
    }

    return mem;
}

void* RwMemRealToFloat32(void* mem, U32 size)
{
    F32* real = (F32*)mem;

    size >>= 2;
    while (size--)
    {
        *real = (F32)*real;
        real++;
    }

    return mem;
}

void* RwMemFloat32ToReal(void* mem, U32 size)
{
    F32* real = (F32*)mem;

    size >>= 2;
    while (size--)
    {
        *real = (F32)*real;
        real++;
    }

    return mem;
}

RwStream* RwStreamWriteReal(RwStream* stream, const F32* reals, U32 numBytes)
{
    U8 buffer[CONVERT_BUFFER_SIZE];

    while (numBytes)
    {
        U32 bytesToWrite = (numBytes < CONVERT_BUFFER_SIZE) ? numBytes : CONVERT_BUFFER_SIZE;

        memcpy(buffer, reals, bytesToWrite);
        RwMemRealToFloat32(buffer, bytesToWrite);
        RwMemLittleEndian32(buffer, bytesToWrite);

        if (!RwStreamWrite(stream, buffer, bytesToWrite))
        {
            return NULL;
        }

        numBytes -= bytesToWrite;
        reals = (const F32*)((const U8*)reals + bytesToWrite);
    }

    return stream;
}

RwStream* RwStreamWriteInt32(RwStream* stream, const S32* ints, U32 numBytes)
{
    U8 buffer[CONVERT_BUFFER_SIZE];

    while (numBytes)
    {
        U32 bytesToWrite = (numBytes < CONVERT_BUFFER_SIZE) ? numBytes : CONVERT_BUFFER_SIZE;

        memcpy(buffer, ints, bytesToWrite);
        RwMemLittleEndian32(buffer, bytesToWrite);

        if (!RwStreamWrite(stream, buffer, bytesToWrite))
        {
            return NULL;
        }

        numBytes -= bytesToWrite;
        ints = (const S32*)((const U8*)ints + bytesToWrite);
    }

    return stream;
}

RwStream* RwStreamReadReal(RwStream* stream, F32* reals, U32 numBytes)
{
    if (!RwStreamRead(stream, reals, numBytes))
    {
        RwError err;
        err.pluginID = 1;
        err.errorCode = _rwerror(0x8000001A);
        RwErrorSet(&err);
        return NULL;
    }

    RwMemNative32(reals, numBytes);
    RwMemFloat32ToReal(reals, numBytes);

    return stream;
}

RwStream* RwStreamReadInt32(RwStream* stream, S32* ints, U32 numBytes)
{
    if (!RwStreamRead(stream, ints, numBytes))
    {
        RwError err;
        err.pluginID = 1;
        err.errorCode = _rwerror(0x8000001A);
        RwErrorSet(&err);
        return NULL;
    }

    RwMemNative32(ints, numBytes);

    return stream;
}

RwStream* RwStreamReadChunkHeaderInfo(RwStream* stream, RwChunkHeaderInfo* chunkHeaderInfo)
{
    U32 type;
    U32 length;
    U32 version;
    U32 buildNum;

    if (!_rwStreamReadChunkHeader(stream, &type, &length, &version, &buildNum))
    {
        return NULL;
    }

    chunkHeaderInfo->type = type;
    chunkHeaderInfo->length = length;
    chunkHeaderInfo->version = version;
    chunkHeaderInfo->buildNum = buildNum;
    chunkHeaderInfo->isComplex = ChunkIsComplex(chunkHeaderInfo);

    return stream;
}
