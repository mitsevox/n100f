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

static S32 _rpMaterialListAppendMaterial(RpMaterialList* matList, RpMaterial* material)
{
    if (matList->space > matList->numMaterials)
    {
        matList->materials[matList->numMaterials] = material;
        material->refCount++;

        return matList->numMaterials++;
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
            RwError err;
            err.pluginID = 2;
            err.errorCode = _rwerror(0x80000013, size);
            RwErrorSet(&err);
            return -1;
        }

        matList->materials = materials;
        matList->space += MATERIALLIST_GROWBY;

        materials[matList->numMaterials] = material;
        material->refCount++;

        return matList->numMaterials++;
    }
}

RpMaterialList* _rpMaterialListStreamRead(RwStream* stream, RpMaterialList* matList)
{
    S32 len;
    U32 size;
    U32 version;

    if (!RwStreamFindChunk(stream, rwID_STRUCT, &size, &version))
    {
        return NULL;
    }

    if (version >= rwLIBRARYBASEVERSION && version <= rwLIBRARYCURRENTVERSION)
    {
        S32* matIndex;
        S32 i;

        S32 status = (RwStreamReadInt32(stream, &len, sizeof(len)) != NULL);

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

        matIndex = (S32*)RwMalloc(len * sizeof(S32));

        status = (RwStreamReadInt32(stream, matIndex, len * sizeof(S32)) != NULL);

        if (!status)
        {
            RwFree(matIndex);
            _rpMaterialListDeinitialize(matList);
            return NULL;
        }

        for (i = 0; i < len; i++)
        {
            RpMaterial* material;

            if (matIndex[i] < 0)
            {
                if (!RwStreamFindChunk(stream, rwID_MATERIAL, NULL, &version))
                {
                    RwFree(matIndex);
                    _rpMaterialListDeinitialize(matList);
                    return NULL;
                }

                if (version >= rwLIBRARYBASEVERSION && version <= rwLIBRARYCURRENTVERSION)
                {
                    material = RpMaterialStreamRead(stream);

                    if (!material)
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
                material = matList->materials[matIndex[i]];
                material->refCount++;
            }

            _rpMaterialListAppendMaterial(matList, material);
            RpMaterialDestroy(material);
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
