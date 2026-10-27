#include <rwcore.h>
#include <stdarg.h>

typedef struct rwErrorGlobals rwErrorGlobals;
struct rwErrorGlobals
{
    RwError errorCode;
};

#define ERRORGLOBAL(var) \
    (((rwErrorGlobals*)((U8*)RwEngineInstance + errorModule.globalsOffset))->var)

static RwModuleInfo errorModule;

void* _rwErrorClose(void* instance)
{
    errorModule.numInstances--;

    return instance;
}

RwError* RwErrorSet(RwError* code)
{
    if (ERRORGLOBAL(errorCode).pluginID == 0 &&
        ERRORGLOBAL(errorCode).errorCode == (S32)0x80000000)
    {
        if (code->errorCode & 0x80000000)
        {
            ERRORGLOBAL(errorCode).pluginID = 0;
        }
        else
        {
            ERRORGLOBAL(errorCode).pluginID = code->pluginID;
        }
        ERRORGLOBAL(errorCode).errorCode = code->errorCode;
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
