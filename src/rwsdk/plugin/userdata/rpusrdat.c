#include <rpusrdat.h>
#include <string.h>

#define USERDATA_PLUGIN_ID 0x11F

// The user data arrays an object carries, in the plugin space of each object type.
typedef struct UserDataList UserDataList;
struct UserDataList
{
    S32 numElements;
    RpUserDataArray* userData;
};

RwModuleInfo userDataModule;
static S32 userDataTextureStreamOffset;
static S32 userDataTextureOffset;
static S32 userDataMaterialStreamOffset;
static S32 userDataMaterialOffset;
static S32 userDataLightStreamOffset;
static S32 userDataLightOffset;
static S32 userDataCameraStreamOffset;
static S32 userDataCameraOffset;
static S32 userDataFrameStreamOffset;
static S32 userDataFrameOffset;
static S32 userDataWorldSectorStreamOffset;
static S32 userDataWorldSectorOffset;
static S32 userDataGeometryStreamOffset;
static S32 userDataGeometryOffset;

static void* UserDataOpen(void* instance, S32 offset, S32 size)
{
    userDataModule.numInstances++;

    return instance;
}

static void* UserDataClose(void* instance, S32 offset, S32 size)
{
    userDataModule.numInstances--;

    return instance;
}

static RwStream* UserDataStreamRead(RpUserDataArray* array, RwStream* stream)
{
    S32 length;

    if (!RwStreamReadInt32(stream, &length, sizeof(S32)))
    {
        return NULL;
    }

    if (length > 0)
    {
        array->name = (char*)RwMalloc(length);

        if (!array->name)
        {
            return NULL;
        }

        if (!RwStreamRead(stream, array->name, length))
        {
            return NULL;
        }
    }

    if (!RwStreamReadInt32(stream, (S32*)&array->format, sizeof(S32)))
    {
        return NULL;
    }

    if (!RwStreamReadInt32(stream, &array->numElements, sizeof(S32)))
    {
        return NULL;
    }

    switch (array->format)
    {
    case rpINTUSERDATA:
    {
        S32* ints;
        S32 i;

        array->data = RwMalloc(array->numElements * sizeof(S32));
        ints = (S32*)array->data;

        if (!ints)
        {
            return NULL;
        }

        for (i = 0; i < array->numElements; i++)
        {
            if (!RwStreamReadInt32(stream, ints, sizeof(S32)))
            {
                return NULL;
            }

            ints++;
        }

        break;
    }
    case rpREALUSERDATA:
    {
        S32 i;
        F32* reals;

        array->data = RwMalloc(array->numElements * sizeof(F32));
        reals = (F32*)array->data;

        if (!reals)
        {
            return NULL;
        }

        for (i = 0; i < array->numElements; i++)
        {
            if (!RwStreamReadReal(stream, reals, sizeof(F32)))
            {
                return NULL;
            }

            reals++;
        }

        break;
    }
    case rpSTRINGUSERDATA:
    {
        S32 i;
        char** strings;

        array->data = RwMalloc(array->numElements * sizeof(char*));
        strings = (char**)array->data;

        if (!strings)
        {
            return NULL;
        }

        for (i = 0; i < array->numElements; i++)
        {
            if (!RwStreamReadInt32(stream, &length, sizeof(S32)))
            {
                return NULL;
            }

            if (length > 0)
            {
                *strings = (char*)RwMalloc(length);

                if (!*strings)
                {
                    return NULL;
                }

                if (!RwStreamRead(stream, *strings, length))
                {
                    return NULL;
                }
            }
            else
            {
                *strings = NULL;
            }

            strings++;
        }

        break;
    }
    default:
        return NULL;
    }

    return stream;
}

static RwStream* UserDataStreamWrite(const RpUserDataArray* array, RwStream* stream)
{
    S32 length;

    length = array->name ? rwstrlen(array->name) + 1 : 0;

    if (!RwStreamWriteInt32(stream, &length, sizeof(S32)))
    {
        return NULL;
    }

    if (length > 0)
    {
        if (!RwStreamWrite(stream, array->name, length))
        {
            return NULL;
        }
    }

    if (!RwStreamWriteInt32(stream, (const S32*)&array->format, sizeof(S32)))
    {
        return NULL;
    }

    if (!RwStreamWriteInt32(stream, &array->numElements, sizeof(S32)))
    {
        return NULL;
    }

    switch (array->format)
    {
    case rpINTUSERDATA:
    {
        const S32* ints = (const S32*)array->data;
        S32 i;

        for (i = 0; i < array->numElements; i++)
        {
            if (!RwStreamWriteInt32(stream, ints, sizeof(S32)))
            {
                return NULL;
            }

            ints++;
        }

        break;
    }
    case rpREALUSERDATA:
    {
        S32 i;
        const F32* reals = (const F32*)array->data;

        for (i = 0; i < array->numElements; i++)
        {
            if (!RwStreamWriteReal(stream, reals, sizeof(F32)))
            {
                return NULL;
            }

            reals++;
        }

        break;
    }
    case rpSTRINGUSERDATA:
    {
        S32 i;
        char** strings = (char**)array->data;

        for (i = 0; i < array->numElements; i++)
        {
            length = *strings ? rwstrlen(*strings) + 1 : 0;

            if (!RwStreamWriteInt32(stream, &length, sizeof(S32)))
            {
                return NULL;
            }

            if (length > 0)
            {
                if (!RwStreamWrite(stream, *strings, length))
                {
                    return NULL;
                }
            }

            strings++;
        }

        break;
    }
    default:
        return NULL;
    }

    return stream;
}

// Sets _dest to a copy of _string, or to NULL when there is none or memory runs out.
#define UserDataStringCopy(_dest, _string)                                                         \
    do                                                                                             \
    {                                                                                              \
        (_dest) = NULL;                                                                            \
                                                                                                   \
        if (_string)                                                                               \
        {                                                                                          \
            (_dest) = (char*)RwMalloc(rwstrlen(_string) + 1);                                      \
                                                                                                   \
            if (_dest)                                                                             \
            {                                                                                      \
                rwstrcpy((_dest), (_string));                                                      \
            }                                                                                      \
        }                                                                                          \
    } while (0)

static void UserDataListDestroy(UserDataList* list)
{
    char** strings;
    RpUserDataArray* array;
    S32 j;
    S32 i;

    if (list->userData)
    {
        for (i = 0; i < list->numElements; i++)
        {
            array = &list->userData[i];

            if (array->name)
            {
                RwFree(array->name);
            }

            if (array->format == rpSTRINGUSERDATA)
            {
                strings = (char**)array->data;

                for (j = 0; j < array->numElements; j++)
                {
                    if (*strings)
                    {
                        RwFree(*strings);
                    }

                    strings++;
                }
            }

            if (array->data)
            {
                RwFree(array->data);
            }
        }

        RwFree(list->userData);
    }

    list->userData = NULL;
    list->numElements = 0;
}

static inline void UserDataArrayCopy(RpUserDataArray* dstArray, const RpUserDataArray* srcArray)
{
    char** dstStrings;
    char** srcStrings;
    S32 j;
    S32 size;

    dstArray->format = srcArray->format;
    dstArray->numElements = srcArray->numElements;

    if (srcArray->name)
    {
        UserDataStringCopy(dstArray->name, srcArray->name);
    }

    if (srcArray->data)
    {
        size = dstArray->numElements * RpUserDataGetFormatSize(dstArray->format);

        dstArray->data = RwMalloc(size);

        if (dstArray->format == rpSTRINGUSERDATA)
        {
            srcStrings = (char**)srcArray->data;
            dstStrings = (char**)dstArray->data;

            for (j = 0; j < dstArray->numElements; j++)
            {
                if (!*srcStrings)
                {
                    *dstStrings = NULL;
                }
                else
                {
                    UserDataStringCopy(*dstStrings, *srcStrings);
                }

                srcStrings++;
                dstStrings++;
            }
        }
        else
        {
            memcpy(dstArray->data, srcArray->data, size);
        }
    }
}

static void UserDataListCopy(UserDataList* dst, const UserDataList* src)
{
    S32 i;

    UserDataListDestroy(dst);

    dst->numElements = src->numElements;

    if (dst->numElements > 0)
    {
        dst->userData = (RpUserDataArray*)RwMalloc(dst->numElements * sizeof(RpUserDataArray));

        for (i = 0; i < dst->numElements; i++)
        {
            UserDataArrayCopy(&dst->userData[i], &src->userData[i]);
        }
    }
}

static S32 UserDataListAddElement(UserDataList* list, char* name, RpUserDataFormat format,
                                  S32 numElements)
{
    S32 index = -1;
    S32 i;
    RpUserDataArray* array;

    for (i = 0; i < list->numElements; i++)
    {
        if (!list->userData[i].data)
        {
            index = i;
        }
    }

    if (index == -1)
    {
        if (list->userData)
        {
            RpUserDataArray* userData =
                (RpUserDataArray*)RwMalloc((list->numElements + 1) * sizeof(RpUserDataArray));

            if (!userData)
            {
                return -1;
            }

            memcpy(userData, list->userData, list->numElements * sizeof(RpUserDataArray));
            RwFree(list->userData);
            list->userData = userData;
        }
        else
        {
            list->userData =
                (RpUserDataArray*)RwMalloc((list->numElements + 1) * sizeof(RpUserDataArray));

            if (!list->userData)
            {
                return -1;
            }
        }

        index = list->numElements;
        list->numElements++;
    }

    array = &list->userData[index];

    array->data = RwMalloc(numElements * RpUserDataGetFormatSize(format));

    if (!array->data)
    {
        return -1;
    }

    UserDataStringCopy(array->name, name);
    array->format = format;
    array->numElements = numElements;

    return index;
}

static S32 UserDataListGetCount(const UserDataList* list)
{
    S32 count = 0;
    S32 i;

    for (i = 0; i < list->numElements; i++)
    {
        if (list->userData[i].data)
        {
            count++;
        }
    }

    return count;
}

static void* UserDataObjectConstruct(void* object, S32 offset, S32 size)
{
    UserDataList* list = (UserDataList*)((U8*)object + offset);

    list->numElements = 0;
    list->userData = NULL;

    return object;
}

static void* UserDataObjectDestruct(void* object, S32 offset, S32 size)
{
    UserDataListDestroy((UserDataList*)((U8*)object + offset));

    return object;
}

static void* UserDataObjectCopy(void* dstObject, const void* srcObject, S32 offset, S32 size)
{
    UserDataListCopy((UserDataList*)((U8*)dstObject + offset),
                     (const UserDataList*)((const U8*)srcObject + offset));

    return dstObject;
}

static RwStream* UserDataListStreamRead(UserDataList* list, RwStream* stream)
{
    S32 numElements;
    RwStream* curStream = stream;
    S32 i;

    if (list)
    {
        if (RwStreamReadInt32(stream, &numElements, sizeof(S32)))
        {
            list->numElements = numElements;
            list->userData =
                (RpUserDataArray*)RwMalloc(list->numElements * sizeof(RpUserDataArray));

            for (i = 0; i < list->numElements; i++)
            {
                curStream = UserDataStreamRead(&list->userData[i], curStream);
            }
        }
    }

    return stream;
}

// Reads an object's user data list from the plugin chunk.
static RwStream* UserDataObjectStreamRead(RwStream* stream, S32 binaryLength, void* object,
                                          S32 offset, S32 size)
{
    UserDataList* list = (UserDataList*)((U8*)object + offset);

    return UserDataListStreamRead(list, stream);
}

static RwStream* UserDataListStreamWrite(const UserDataList* list, RwStream* stream)
{
    RwStream* curStream = stream;
    S32 i;

    if (list && list->numElements > 0)
    {
        if (RwStreamWriteInt32(stream, &list->numElements, sizeof(S32)))
        {
            for (i = 0; i < list->numElements; i++)
            {
                curStream = UserDataStreamWrite(&list->userData[i], curStream);
            }
        }
    }

    return stream;
}

// Writes an object's user data list to the plugin chunk.
static RwStream* UserDataObjectStreamWrite(RwStream* stream, S32 binaryLength,
                                           const void* object, S32 offset, S32 size)
{
    const UserDataList* list = (const UserDataList*)((const U8*)object + offset);

    return UserDataListStreamWrite(list, stream);
}

static S32 UserDataArrayGetSize(const UserDataList* list, S32 index)
{
    char** strings;
    const RpUserDataArray* array = &list->userData[index];
    S32 i;
    S32 size = 0;

    if (array)
    {
        size = sizeof(S32);

        if (array->name)
        {
            size += rwstrlen(array->name) + 1;
        }

        size += sizeof(S32) + sizeof(S32);

        switch (array->format)
        {
        case rpINTUSERDATA:
            size += array->numElements * sizeof(S32);
            break;
        case rpREALUSERDATA:
            size += array->numElements * sizeof(F32);
            break;
        case rpSTRINGUSERDATA:
        {
            strings = (char**)array->data;

            for (i = 0; i < array->numElements; i++)
            {
                size += sizeof(S32);

                if (*strings)
                {
                    S32 len = rwstrlen(*strings) + 1;

                    size += len;
                }

                strings++;
            }

            break;
        }
        }
    }

    return size;
}

static S32 UserDataListGetSize(const UserDataList* list)
{
    S32 i;
    S32 totalSize = 0;

    if (list && list->numElements > 0)
    {
        totalSize += sizeof(S32);

        for (i = 0; i < list->numElements; i++)
        {
            totalSize += UserDataArrayGetSize(list, i);
        }
    }

    return totalSize;
}

// The size of an object's user data list in the stream.
static S32 UserDataObjectGetSize(const void* object, S32 offset, S32 size)
{
    const UserDataList* list = (const UserDataList*)((const U8*)object + offset);

    return UserDataListGetSize(list);
}

S32 RpGeometryAddUserDataArray(RpGeometry* geometry, char* name, RpUserDataFormat format,
                               S32 numElements)
{
    return UserDataListAddElement((UserDataList*)((U8*)geometry + userDataGeometryOffset), name,
                                  format, numElements);
}

// The number of user data arrays on the geometry.
S32 RpGeometryGetUserDataArrayCount(const RpGeometry* geometry)
{
    const UserDataList* list = (const UserDataList*)((const U8*)geometry + userDataGeometryOffset);

    return UserDataListGetCount(list);
}

RpUserDataArray* RpGeometryGetUserDataArray(const RpGeometry* geometry, S32 data)
{
    const UserDataList* list = (const UserDataList*)((const U8*)geometry + userDataGeometryOffset);

    if (data < list->numElements)
    {
        return &list->userData[data];
    }

    return NULL;
}

S32 RpUserDataGetFormatSize(RpUserDataFormat format)
{
    switch (format)
    {
    case rpINTUSERDATA:
        return sizeof(S32);
    case rpREALUSERDATA:
        return sizeof(F32);
    case rpSTRINGUSERDATA:
        return sizeof(char*);
    default:
        return 0;
    }
}

S32 RpUserDataPluginAttach(void)
{
    if (RwEngineRegisterPlugin(0, USERDATA_PLUGIN_ID, UserDataOpen, UserDataClose) < 0)
    {
        return FALSE;
    }

    userDataGeometryOffset =
        RpGeometryRegisterPlugin(sizeof(UserDataList), USERDATA_PLUGIN_ID, UserDataObjectConstruct,
                                 UserDataObjectDestruct, UserDataObjectCopy);

    if (userDataGeometryOffset < 0)
    {
        return FALSE;
    }

    userDataGeometryStreamOffset =
        RpGeometryRegisterPluginStream(USERDATA_PLUGIN_ID, UserDataObjectStreamRead,
                                       UserDataObjectStreamWrite, UserDataObjectGetSize);

    if (userDataGeometryStreamOffset < 0)
    {
        return FALSE;
    }

    userDataWorldSectorOffset = RpWorldSectorRegisterPlugin(
        sizeof(UserDataList), USERDATA_PLUGIN_ID, UserDataObjectConstruct, UserDataObjectDestruct,
        UserDataObjectCopy);

    if (userDataGeometryOffset < 0)
    {
        return FALSE;
    }

    userDataWorldSectorStreamOffset =
        RpWorldSectorRegisterPluginStream(USERDATA_PLUGIN_ID, UserDataObjectStreamRead,
                                          UserDataObjectStreamWrite, UserDataObjectGetSize);

    if (userDataGeometryStreamOffset < 0)
    {
        return FALSE;
    }

    userDataFrameOffset =
        RwFrameRegisterPlugin(sizeof(UserDataList), USERDATA_PLUGIN_ID, UserDataObjectConstruct,
                              UserDataObjectDestruct, UserDataObjectCopy);

    if (userDataFrameOffset < 0)
    {
        return FALSE;
    }

    userDataFrameStreamOffset =
        RwFrameRegisterPluginStream(USERDATA_PLUGIN_ID, UserDataObjectStreamRead,
                                    UserDataObjectStreamWrite, UserDataObjectGetSize);

    if (userDataFrameStreamOffset < 0)
    {
        return FALSE;
    }

    userDataCameraOffset =
        RwCameraRegisterPlugin(sizeof(UserDataList), USERDATA_PLUGIN_ID, UserDataObjectConstruct,
                               UserDataObjectDestruct, UserDataObjectCopy);

    if (userDataCameraOffset < 0)
    {
        return FALSE;
    }

    userDataCameraStreamOffset =
        RwCameraRegisterPluginStream(USERDATA_PLUGIN_ID, UserDataObjectStreamRead,
                                     UserDataObjectStreamWrite, UserDataObjectGetSize);

    if (userDataCameraStreamOffset < 0)
    {
        return FALSE;
    }

    userDataLightOffset =
        RpLightRegisterPlugin(sizeof(UserDataList), USERDATA_PLUGIN_ID, UserDataObjectConstruct,
                              UserDataObjectDestruct, UserDataObjectCopy);

    if (userDataLightOffset < 0)
    {
        return FALSE;
    }

    userDataLightStreamOffset =
        RpLightRegisterPluginStream(USERDATA_PLUGIN_ID, UserDataObjectStreamRead,
                                    UserDataObjectStreamWrite, UserDataObjectGetSize);

    if (userDataLightStreamOffset < 0)
    {
        return FALSE;
    }

    userDataMaterialOffset =
        RpMaterialRegisterPlugin(sizeof(UserDataList), USERDATA_PLUGIN_ID, UserDataObjectConstruct,
                                 UserDataObjectDestruct, UserDataObjectCopy);

    if (userDataMaterialOffset < 0)
    {
        return FALSE;
    }

    userDataMaterialStreamOffset =
        RpMaterialRegisterPluginStream(USERDATA_PLUGIN_ID, UserDataObjectStreamRead,
                                       UserDataObjectStreamWrite, UserDataObjectGetSize);

    if (userDataMaterialStreamOffset < 0)
    {
        return FALSE;
    }

    userDataTextureOffset =
        RwTextureRegisterPlugin(sizeof(UserDataList), USERDATA_PLUGIN_ID, UserDataObjectConstruct,
                                UserDataObjectDestruct, UserDataObjectCopy);

    if (userDataTextureOffset < 0)
    {
        return FALSE;
    }

    userDataTextureStreamOffset =
        RwTextureRegisterPluginStream(USERDATA_PLUGIN_ID, UserDataObjectStreamRead,
                                      UserDataObjectStreamWrite, UserDataObjectGetSize);

    if (userDataTextureStreamOffset < 0)
    {
        return FALSE;
    }

    return TRUE;
}
