#include "zEnt.h"

#include "xString.h"

#include <stdlib.h>

// Each parameter is an 8-byte header followed by WordLength words of string.
char* zParamGetString(zAssetModelParam* param, U32 size, char* tok, char* def)
{
    U32 hash = xStrHash(tok);

    while (param != NULL && size != 0)
    {
        if (param->HashID == hash)
        {
            return (char*)param->String;
        }

        size -= param->WordLength * 4 + 8;
        param = (zAssetModelParam*)((char*)param + (param->WordLength * 4 + 8));
    }
    return def;
}

S32 zParamGetInt(zAssetModelParam* param, U32 size, const char* tok, S32 def)
{
    char* str = zParamGetString(param, size, (char*)tok, NULL);
    if (str != NULL)
    {
        return atoi(str);
    }
    return def;
}

void zEntEvent(xBase* to, U32 toEvent, F32* toParam)
{
    zEntEvent(NULL, 0, to, toEvent, toParam, NULL, 0);
}

void zEntEvent(xBase* to, U32 toEvent, F32* toParam, xBase* toParamWidget)
{
    zEntEvent(NULL, 0, to, toEvent, toParam, toParamWidget, 0);
}
