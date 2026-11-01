#include <rwcore.h>
#include <string.h>

// The stream module's data in the engine instance.
typedef struct StreamModuleData StreamModuleData;
struct StreamModuleData
{
    RwFreeList* freeList;
};

#define STREAM_DATA(var) \
    (((StreamModuleData*)((U8*)RwEngineInstance + streamModule.globalsOffset))->var)

// The size by which a memory stream's block grows when a write overruns it.
#define STREAM_MEMORYBLOCKSIZE 512

// The seek origin a file stream's skip passes: the current position.
#define STREAM_SEEKCURRENT 1

static RwModuleInfo streamModule;

// Creates the free list the streams come from (ten per block).
void* _rwStreamModuleOpen(void* instance, S32 offset, S32 size)
{
    streamModule.globalsOffset = offset;

    STREAM_DATA(freeList) = RwFreeListCreate(sizeof(RwStream), 10, 0);
    if (!STREAM_DATA(freeList))
    {
        return NULL;
    }

    streamModule.numInstances++;

    return instance;
}

void* _rwStreamModuleClose(void* instance, S32 offset, S32 size)
{
    if (STREAM_DATA(freeList))
    {
        RwFreeListDestroy(STREAM_DATA(freeList));
    }

    streamModule.numInstances--;

    return instance;
}

U32 RwStreamRead(RwStream* stream, void* buffer, U32 length)
{
    switch (stream->type)
    {
    case rwSTREAMFILE:
    case rwSTREAMFILENAME:
    {
        void* fp = stream->Type.file.fpFile;
        U32 bytesRead = RWSRCGLOBAL(fileFuncs).rwfread(buffer, 1, length, fp);

        if (bytesRead != length)
        {
            if (RWSRCGLOBAL(fileFuncs).rwfeof(fp))
            {
                RwError err;

                err.pluginID = 1;
                err.errorCode = _rwerror(5);
                RwErrorSet(&err);
            }
            else
            {
                RwError err;

                err.pluginID = 1;
                err.errorCode = _rwerror(0x8000001A);
                RwErrorSet(&err);
            }
        }

        return bytesRead;
    }
    case rwSTREAMMEMORY:
    {
        U32 available = stream->Type.memory.nSize - stream->Type.memory.position;

        if (length > available)
        {
            RwError err;

            length = available;

            err.pluginID = 1;
            err.errorCode = _rwerror(5);
            RwErrorSet(&err);
        }

        memcpy(buffer, stream->Type.memory.memBlock + stream->Type.memory.position, length);
        stream->Type.memory.position += length;

        return length;
    }
    case rwSTREAMCUSTOM:
        return stream->Type.custom.sfnread(stream->Type.custom.data, buffer, length);
    default:
    {
        RwError err;

        err.pluginID = 1;
        err.errorCode = _rwerror(0xE);
        RwErrorSet(&err);

        return 0;
    }
    }
}

RwStream* RwStreamWrite(RwStream* stream, const void* buffer, U32 length)
{
    switch (stream->type)
    {
    case rwSTREAMFILE:
    case rwSTREAMFILENAME:
        if (RWSRCGLOBAL(fileFuncs).rwfwrite(buffer, 1, length, stream->Type.file.fpFile) != length)
        {
            RwError err;

            err.pluginID = 1;
            err.errorCode = _rwerror(0x8000001C);
            RwErrorSet(&err);

            return NULL;
        }

        return stream;
    case rwSTREAMMEMORY:
        if (!stream->Type.memory.memBlock)
        {
            stream->Type.memory.memBlock = (U8*)RwMalloc(STREAM_MEMORYBLOCKSIZE);

            if (!stream->Type.memory.memBlock)
            {
                RwError err;

                err.pluginID = 1;
                err.errorCode = _rwerror(0x80000013, STREAM_MEMORYBLOCKSIZE);
                RwErrorSet(&err);

                return NULL;
            }

            stream->Type.memory.nSize = STREAM_MEMORYBLOCKSIZE;
        }

        if (stream->Type.memory.nSize - stream->Type.memory.position < length)
        {
            U32 newSize;
            U8* newBlock;

            if (length < STREAM_MEMORYBLOCKSIZE)
            {
                newSize = stream->Type.memory.nSize + STREAM_MEMORYBLOCKSIZE;
            }
            else
            {
                newSize = length + stream->Type.memory.nSize;
            }

            newBlock = (U8*)RwRealloc(stream->Type.memory.memBlock, newSize);

            if (!newBlock)
            {
                RwError err;

                err.pluginID = 1;
                err.errorCode = _rwerror(0x80000013, newSize - stream->Type.memory.nSize);
                RwErrorSet(&err);

                return NULL;
            }

            stream->Type.memory.memBlock = newBlock;
            stream->Type.memory.nSize = newSize;
        }

        memcpy(stream->Type.memory.memBlock + stream->Type.memory.position, buffer, length);
        stream->Type.memory.position += length;

        return stream;
    case rwSTREAMCUSTOM:
        if (stream->Type.custom.sfnwrite(stream->Type.custom.data, buffer, length))
        {
            return stream;
        }

        return NULL;
    default:
    {
        RwError err;

        err.pluginID = 1;
        err.errorCode = _rwerror(0xE);
        RwErrorSet(&err);

        return NULL;
    }
    }
}

RwStream* RwStreamSkip(RwStream* stream, U32 offset)
{
    if (!offset)
    {
        return stream;
    }

    switch (stream->type)
    {
    case rwSTREAMFILE:
    case rwSTREAMFILENAME:
    {
        void* fp = stream->Type.file.fpFile;

        if (RWSRCGLOBAL(fileFuncs).rwfseek(fp, offset, STREAM_SEEKCURRENT))
        {
            if (RWSRCGLOBAL(fileFuncs).rwfeof(fp))
            {
                RwError err;

                err.pluginID = 1;
                err.errorCode = _rwerror(5);
                RwErrorSet(&err);
            }

            return NULL;
        }

        return stream;
    }
    case rwSTREAMMEMORY:
        if (stream->Type.memory.position + offset > stream->Type.memory.nSize)
        {
            RwError err;

            stream->Type.memory.position = stream->Type.memory.nSize;

            err.pluginID = 1;
            err.errorCode = _rwerror(5);
            RwErrorSet(&err);

            return NULL;
        }

        stream->Type.memory.position += offset;

        return stream;
    case rwSTREAMCUSTOM:
        if (stream->Type.custom.sfnskip(stream->Type.custom.data, offset))
        {
            return stream;
        }

        return NULL;
    default:
    {
        RwError err;

        err.pluginID = 1;
        err.errorCode = _rwerror(0xE);
        RwErrorSet(&err);

        return NULL;
    }
    }
}

S32 RwStreamClose(RwStream* stream, void* pData)
{
    S32 result;

    switch (stream->type)
    {
    case rwSTREAMFILE:
        result = TRUE;
        break;
    case rwSTREAMFILENAME:
        result = !RWSRCGLOBAL(fileFuncs).rwfclose(stream->Type.file.fpFile);
        break;
    case rwSTREAMMEMORY:
        if (stream->accessType != rwSTREAMREAD && pData)
        {
            ((RwMemory*)pData)->start = stream->Type.memory.memBlock;
            ((RwMemory*)pData)->length = stream->Type.memory.position;
        }

        result = TRUE;
        break;
    case rwSTREAMCUSTOM:
        if (stream->Type.custom.sfnclose)
        {
            stream->Type.custom.sfnclose(stream->Type.custom.data);
        }

        result = TRUE;
        break;
    default:
    {
        RwError err;

        err.pluginID = 1;
        err.errorCode = _rwerror(0xE);
        RwErrorSet(&err);

        return FALSE;
    }
    }

    if (stream->rwOwned)
    {
        RWSRCGLOBAL(memoryFree)(STREAM_DATA(freeList), stream);
    }

    return result;
}

// Opens a stream of the given type and access: an already open file (checked with ftell), a file
// by name (opened "rb", "wb" or "ab"), a memory block (read from its start, written from empty,
// or appended after its length), or a custom stream (its callbacks copied). NULL on failure.
RwStream* RwStreamOpen(RwStreamType type, RwStreamAccessType accessType, const void* pData)
{
    RwStream* stream;
    RwStream* result;

    stream = (RwStream*)RWSRCGLOBAL(memoryAlloc)(STREAM_DATA(freeList));
    result = NULL;

    if (stream)
    {
        stream->type = type;
        stream->accessType = accessType;
        stream->rwOwned = TRUE;

        switch (type)
        {
        case rwSTREAMFILE:
            if (RWSRCGLOBAL(fileFuncs).rwftell((void*)pData) == -1)
            {
                result = NULL;
            }
            else
            {
                stream->Type.file.constfpFile = pData;
                result = stream;
            }
            break;
        case rwSTREAMFILENAME:
        {
            void* fp = NULL;

            switch (accessType)
            {
            case rwSTREAMREAD:
                fp = RWSRCGLOBAL(fileFuncs).rwfopen((const char*)pData, "rb");
                break;
            case rwSTREAMWRITE:
                fp = RWSRCGLOBAL(fileFuncs).rwfopen((const char*)pData, "wb");
                break;
            case rwSTREAMAPPEND:
                fp = RWSRCGLOBAL(fileFuncs).rwfopen((const char*)pData, "ab");
                break;
            default:
            {
                RwError err;

                err.pluginID = 1;
                err.errorCode = _rwerror(0xD);
                RwErrorSet(&err);
                break;
            }
            }

            if (fp)
            {
                stream->Type.file.fpFile = fp;
                result = stream;
            }
            else
            {
                RwError err;

                err.pluginID = 1;
                err.errorCode = _rwerror(0x80000002, pData);
                RwErrorSet(&err);
            }
            break;
        }
        case rwSTREAMMEMORY:
        {
            RwStream* opened = NULL;

            switch (accessType)
            {
            case rwSTREAMREAD:
                stream->Type.memory.position = 0;
                opened = stream;
                stream->Type.memory.nSize = ((const RwMemory*)pData)->length;
                stream->Type.memory.memBlock = ((const RwMemory*)pData)->start;
                break;
            case rwSTREAMWRITE:
                stream->Type.memory.position = 0;
                opened = stream;
                stream->Type.memory.nSize = 0;
                stream->Type.memory.memBlock = NULL;
                break;
            case rwSTREAMAPPEND:
                opened = stream;
                stream->Type.memory.position = ((const RwMemory*)pData)->length;
                stream->Type.memory.nSize = ((const RwMemory*)pData)->length;
                stream->Type.memory.memBlock = ((const RwMemory*)pData)->start;
                break;
            default:
            {
                RwError err;

                err.pluginID = 1;
                err.errorCode = _rwerror(0xD);
                RwErrorSet(&err);
                break;
            }
            }

            result = opened;
            break;
        }
        case rwSTREAMCUSTOM:
            memcpy(&stream->Type.custom, pData, sizeof(RwStreamCustom));
            result = stream;
            break;
        default:
        {
            RwError err;

            err.pluginID = 1;
            err.errorCode = _rwerror(0xE);
            RwErrorSet(&err);
            break;
        }
        }
    }

    if (!result)
    {
        RWSRCGLOBAL(memoryFree)(STREAM_DATA(freeList), stream);
        stream = NULL;
    }

    return stream;
}
