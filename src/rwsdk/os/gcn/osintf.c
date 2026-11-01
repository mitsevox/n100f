#include <rwcore.h>

// Says whether a file path is absolute: it starts with a drive letter and a colon, or with a
// backslash.
S32 _rwpathisabsolute(const char* path)
{
    if (path[1] == ':' &&
        ((path[0] >= 'A' && path[0] <= 'Z') || (path[0] >= 'a' && path[0] <= 'z')))
    {
        return 1;
    }

    return path[0] == '\\';
}
