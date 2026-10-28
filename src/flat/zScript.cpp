#include "zScript.h"

void zScriptInit(void* data, void* asset)
{
    _zScript* script = (_zScript*)data;
    zScriptAsset* tasset = (zScriptAsset*)asset;

    xBaseInit(script, tasset);
    script->eventFunc = &zScriptEventCB;
    script->tasset = tasset;

    // The asset's timed events come first; the links follow them.
    if (script->linkCount != 0)
    {
        script->link = (p2LinkAsset*)(script->tasset + 1);
        script->link += tasset->eventCount;
    }
    else
    {
        script->link = NULL;
    }

    script->time = script->tasset->scriptStartTime;
    script->more = 1;
}
