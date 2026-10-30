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
