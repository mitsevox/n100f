#include <rwcore.h>
#include <stdio.h>

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

S32 _rwFileSystemOpen(void)
{
    RWSRCGLOBAL(fileFuncs).rwfexist = rwfexist;
    RWSRCGLOBAL(fileFuncs).rwfopen = (void* (*)(const char*, const char*))fopen;
    RWSRCGLOBAL(fileFuncs).rwfclose = (S32 (*)(void*))fclose;
    RWSRCGLOBAL(fileFuncs).rwfread = (U32 (*)(void*, U32, U32, void*))fread;
    RWSRCGLOBAL(fileFuncs).rwfwrite = (U32 (*)(const void*, U32, U32, void*))fwrite;
    RWSRCGLOBAL(fileFuncs).rwfgets = (char* (*)(char*, S32, void*))fgets;
    RWSRCGLOBAL(fileFuncs).rwfputs = (S32 (*)(const char*, void*))fputs;
    RWSRCGLOBAL(fileFuncs).rwfeof = (S32 (*)(void*))feof;
    RWSRCGLOBAL(fileFuncs).rwfseek = (S32 (*)(void*, S32, S32))fseek;
    RWSRCGLOBAL(fileFuncs).rwfflush = (S32 (*)(void*))fflush;
    RWSRCGLOBAL(fileFuncs).rwftell = (S32 (*)(void*))ftell;

    return 1;
}

void _rwFileSystemClose(void)
{
}
