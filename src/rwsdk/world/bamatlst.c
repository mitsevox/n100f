#include <rpworld.h>

RpMaterialList* _rpMaterialListDeinitialize(RpMaterialList* matList)
{
    RpMaterial** materials = matList->materials;

    if (materials)
    {
        S32 numMaterials = matList->numMaterials;
        S32 i;

        for (i = 0; i < numMaterials; i++)
        {
            RpMaterialDestroy(materials[i]);
            materials[i] = NULL;
        }

        RwFree(materials);
        matList->materials = NULL;
    }

    matList->numMaterials = 0;
    matList->space = 0;

    return matList;
}

RpMaterialList* _rpMaterialListInitialize(RpMaterialList* matList)
{
    matList->space = 0;
    matList->materials = NULL;
    matList->numMaterials = 0;

    return matList;
}

RpMaterial* _rpMaterialListGetMaterial(const RpMaterialList* matList, S32 matIndex)
{
    return matList->materials[matIndex];
}

S32 _rpMaterialListFindMaterialIndex(const RpMaterialList* matList, const RpMaterial* material)
{
    S32 matIndex = matList->numMaterials;

    while (matIndex-- > 0)
    {
        if (matList->materials[matIndex] == material)
        {
            break;
        }
    }

    return matIndex;
}

// The number of slots a full list grows by.
#define MATERIALLIST_GROWBY 20

RpMaterial* RpMaterialStreamRead(RwStream* stream);

// Reports a failed allocation of size bytes (project-own name; inlined).
static void MaterialListNoMemory(U32 size)
{
    RwError err;
    err.pluginID = 2;
    err.errorCode = _rwerror(0x80000013, size);
    RwErrorSet(&err);
}

// Makes room for space materials, keeping the ones already listed.
static RpMaterialList* MaterialListAlloc(RpMaterialList* matList, S32 space)
{
    if (matList->space < space)
    {
        U32 size = space * sizeof(RpMaterial*);
        RpMaterial** materials;

        if (matList->materials)
        {
            materials = (RpMaterial**)RwRealloc(matList->materials, size);
        }
        else
        {
            materials = (RpMaterial**)RwMalloc(size);
        }

        if (!materials)
        {
            RwError err;
            err.pluginID = 2;
            err.errorCode = _rwerror(0x80000013, size);
            RwErrorSet(&err);
            return NULL;
        }

        matList->materials = materials;
        matList->space = space;
    }

    return matList;
}

// Reads a material list chunk: the count, the index array, then each new material. An index
// below zero means a material follows in the stream; any other index shares an earlier one.
// The append is written out in the loop, and status also holds the index array's byte count.
RpMaterialList* _rpMaterialListStreamRead(RwStream* stream, RpMaterialList* matList)
{
    S32 len;
    U32 size;
    S32* index;
    S32 i;
    S32* matIndex;
    RpMaterial* material;
    S32 status;
    U32 version;

    if (!RwStreamFindChunk(stream, rwID_STRUCT, &size, &version))
    {
        return NULL;
    }

    if (version >= rwLIBRARYBASEVERSION && version <= rwLIBRARYCURRENTVERSION)
    {
        status = (RwStreamReadInt32(stream, &len, sizeof(len)) != NULL);

        if (!status)
        {
            return NULL;
        }

        _rpMaterialListInitialize(matList);

        if (len == 0)
        {
            return matList;
        }

        if (!MaterialListAlloc(matList, len))
        {
            _rpMaterialListDeinitialize(matList);
            return NULL;
        }

        status = len * sizeof(S32);
        matIndex = (S32*)RwMalloc(status);

        status = (RwStreamReadInt32(stream, matIndex, len * sizeof(S32)) != NULL);

        if (!status)
        {
            RwFree(matIndex);
            _rpMaterialListDeinitialize(matList);
            return NULL;
        }

        index = matIndex;

        for (i = 0; i < len; i++)
        {
            if (*index < 0)
            {
                if (!RwStreamFindChunk(stream, rwID_MATERIAL, NULL, &version))
                {
                    RwFree(matIndex);
                    _rpMaterialListDeinitialize(matList);
                    return NULL;
                }

                if (version >= rwLIBRARYBASEVERSION && version <= rwLIBRARYCURRENTVERSION)
                {
                    RpMaterial* newMaterial = RpMaterialStreamRead(stream);

                    if ((material = newMaterial) == NULL)
                    {
                        RwFree(matIndex);
                        _rpMaterialListDeinitialize(matList);
                        return NULL;
                    }
                }
                else
                {
                    RwError err;
                    err.pluginID = 2;
                    err.errorCode = _rwerror(0x80000004);
                    RwErrorSet(&err);
                    RwFree(matIndex);
                    _rpMaterialListDeinitialize(matList);
                    return NULL;
                }
            }
            else
            {
                material = _rpMaterialListGetMaterial(matList, *index);
                material->refCount++;
            }

            if (matList->space > matList->numMaterials)
            {
                matList->materials[matList->numMaterials] = material;
                material->refCount++;
                matList->numMaterials++;
            }
            else
            {
                U32 size = (matList->space + MATERIALLIST_GROWBY) * sizeof(RpMaterial*);
                RpMaterial** materials;

                if (matList->materials)
                {
                    materials = (RpMaterial**)RwRealloc(matList->materials, size);
                }
                else
                {
                    materials = (RpMaterial**)RwMalloc(size);
                }

                if (!materials)
                {
                    MaterialListNoMemory(size);
                }
                else
                {
                    matList->materials = materials;
                    matList->space += MATERIALLIST_GROWBY;

                    materials[matList->numMaterials] = material;
                    material->refCount++;
                    matList->numMaterials++;
                }
            }
            RpMaterialDestroy(material);
            index++;
        }

        RwFree(matIndex);

        return matList;
    }

    {
        RwError err;
        err.pluginID = 2;
        err.errorCode = _rwerror(0x80000004);
        RwErrorSet(&err);
    }

    return NULL;
}
