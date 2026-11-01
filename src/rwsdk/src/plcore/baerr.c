#include <rwcore.h>
#include <stdarg.h>

typedef struct ErrorModuleData ErrorModuleData;
struct ErrorModuleData
{
    RwError lastError;
};

#define ERROR_DATA(var) \
    (((ErrorModuleData*)((U8*)RwEngineInstance + errorModule.globalsOffset))->var)

static RwModuleInfo errorModule;

// Records where the error module's globals sit in the engine instance and clears the last error
// (plugin 0, the "no error" code).
void* _rwErrorOpen(void* instance, S32 offset, S32 size)
{
    errorModule.globalsOffset = offset;
    errorModule.numInstances++;

    ERROR_DATA(lastError).pluginID = 0;
    ERROR_DATA(lastError).errorCode = (S32)0x80000000;

    return instance;
}

void* _rwErrorClose(void* instance)
{
    errorModule.numInstances--;

    return instance;
}

RwError* RwErrorSet(RwError* code)
{
    if (ERROR_DATA(lastError).pluginID == 0 &&
        ERROR_DATA(lastError).errorCode == (S32)0x80000000)
    {
        if (code->errorCode & 0x80000000)
        {
            ERROR_DATA(lastError).pluginID = 0;
        }
        else
        {
            ERROR_DATA(lastError).pluginID = code->pluginID;
        }
        ERROR_DATA(lastError).errorCode = code->errorCode;
    }

    return code;
}

S32 _rwerror(S32 code, ...)
{
    va_list args;

    va_start(args, code);
    va_end(args);

    return code;
}
