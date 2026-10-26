#include "xIni.h"
#include "xString.h"

#include <stdlib.h>

static S32 xIniGetIndex(xIniFile* ini, char* tok)
{
    for (S32 i = 0; i < ini->NumValues; i++)
    {
        if (xStricmp(ini->Values[i].tok, tok) == 0)
        {
            return i;
        }
    }

    return -1;
}

S32 xIniGetInt(xIniFile* ini, char* tok, S32 def)
{
    S32 index = xIniGetIndex(ini, tok);
    if (index == -1)
    {
        return def;
    }
    return atoi(ini->Values[index].val);
}

F32 xIniGetFloat(xIniFile* ini, char* tok, F32 def)
{
    S32 index = xIniGetIndex(ini, tok);
    if (index == -1)
    {
        return def;
    }
    return atof(ini->Values[index].val);
}

char* xIniGetString(xIniFile* ini, char* tok, char* def)
{
    S32 index = xIniGetIndex(ini, tok);
    if (index == -1)
    {
        return def;
    }
    return ini->Values[index].val;
}
