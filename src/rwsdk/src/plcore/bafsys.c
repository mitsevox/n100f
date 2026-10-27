#include <rwcore.h>

RwFileFunctions* RwOsGetFileInterface(void)
{
    return &RWSRCGLOBAL(fileFuncs);
}

S32 rwfexist(const char* name)
{
    void* fp = RWSRCGLOBAL(fileFuncs).rwfopen(name, "rb");
    S32 exists = fp != NULL;

    if (fp)
    {
        RWSRCGLOBAL(fileFuncs).rwfclose(fp);
    }

    return exists;
}

void _rwFileSystemClose(void)
{
}
