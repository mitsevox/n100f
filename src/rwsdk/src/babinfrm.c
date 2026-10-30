#include <rwcore.h>

extern RwPluginRegistry frameTKList;

// How far a streamed matrix may stray from orthonormal and still be flagged as orthonormal.
#define FRAME_ORTHONORMALTOLERANCE 0.01f
#define FRAME_MINDETERMINANT 0.99f

// A frame's stream chunk: its modelling matrix and the index of its parent in the list.
typedef struct FrameChunkInfo FrameChunkInfo;
struct FrameChunkInfo
{
    RwV3d right;
    RwV3d up;
    RwV3d at;
    RwV3d pos;
    S32 parentIndex;
    U32 data;
};

rwFrameList* _rwFrameListDeinitialize(rwFrameList* frameList)
{
    if (frameList->numFrames)
    {
        RWSRCGLOBAL(memoryFuncs).rwfree(frameList->frames);
    }

    return frameList;
}

S32 RwFrameRegisterPluginStream(U32 pluginID, RwPluginDataChunkReadCallBack readCB,
                                RwPluginDataChunkWriteCallBack writeCB,
                                RwPluginDataChunkGetSizeCallBack getSizeCB)
{
    return _rwPluginRegistryAddPluginStream(&frameTKList, pluginID, readCB, writeCB, getSizeCB);
}

rwFrameList* _rwFrameListStreamRead(RwStream* stream, rwFrameList* frameList)
{
    S32 i;
    S32 numFrames;
    U32 size;
    U32 version;

    if (!RwStreamFindChunk(stream, rwID_STRUCT, &size, &version))
    {
        return NULL;
    }

    if (version >= rwLIBRARYBASEVERSION && version <= rwLIBRARYCURRENTVERSION)
    {
        if (RwStreamRead(stream, &numFrames, sizeof(numFrames)) != sizeof(numFrames))
        {
            return NULL;
        }

        RwMemNative32(&numFrames, sizeof(numFrames));

        frameList->numFrames = numFrames;
        frameList->frames = (RwFrame**)RwMalloc(numFrames * sizeof(RwFrame*));

        if (!frameList->frames)
        {
            RwError err;
            err.pluginID = 1;
            err.errorCode = _rwerror(0x80000013, numFrames * sizeof(RwFrame*));
            RwErrorSet(&err);
            return NULL;
        }

        for (i = 0; i < numFrames; i++)
        {
            FrameChunkInfo info;
            RwFrame* frame;
            RwMatrix* matrix;

            if (RwStreamRead(stream, &info, sizeof(info)) != sizeof(info))
            {
                RwFree(frameList->frames);
                return NULL;
            }

            RwMemNative32(&info, sizeof(info));
            RwMemFloat32ToReal(&info.right, sizeof(info.right));
            RwMemFloat32ToReal(&info.up, sizeof(info.up));
            RwMemFloat32ToReal(&info.at, sizeof(info.at));
            RwMemFloat32ToReal(&info.pos, sizeof(info.pos));

            frame = RwFrameCreate();
            if (!frame)
            {
                RwFree(frameList->frames);
                return NULL;
            }

            matrix = RwFrameGetMatrix(frame);
            matrix->right = info.right;
            matrix->up = info.up;
            matrix->at = info.at;
            matrix->pos = info.pos;

            if (FRAME_ORTHONORMALTOLERANCE >= _rwMatrixNormalError(matrix) &&
                FRAME_ORTHONORMALTOLERANCE >= _rwMatrixOrthogonalError(matrix) &&
                FRAME_MINDETERMINANT <= _rwMatrixDeterminant(matrix))
            {
                matrix->flags &= ~rwMATRIXINTERNALIDENTITY;
            }
            else
            {
                matrix->flags &= ~(rwMATRIXINTERNALIDENTITY | rwMATRIXTYPEORTHONORMAL);
            }

            frameList->frames[i] = frame;

            if (info.parentIndex >= 0)
            {
                RwFrameAddChild(frameList->frames[info.parentIndex], frame);
            }
        }
    }
    else
    {
        RwError err;
        err.pluginID = 1;
        err.errorCode = _rwerror(0x80000004);
        RwErrorSet(&err);
        return NULL;
    }

    for (i = 0; i < numFrames; i++)
    {
        if (!_rwPluginRegistryReadDataChunks(&frameTKList, stream, frameList->frames[i]))
        {
            return NULL;
        }
    }

    return frameList;
}
