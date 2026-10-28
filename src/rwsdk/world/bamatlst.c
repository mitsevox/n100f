#include <rpworld.h>

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
