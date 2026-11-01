#include <rwcore.h>
#include <stdio.h>
#include <string.h>

int vsprintf(char* s, const char* format, va_list arg);
int sscanf(const char* s, const char* format, ...);

S32 StrICmp(const char* s1, const char* s2)
{
    if (s1 && s2)
    {
        char c1, c2;

        do
        {
            c1 = *s1;
            c2 = *s2;
            if (c1 >= 'A' && c1 <= 'Z')
            {
                c1 += 'a' - 'A';
            }
            if (c2 >= 'A' && c2 <= 'Z')
            {
                c2 += 'a' - 'A';
            }
            if (c1 != c2)
            {
                return c1 - c2;
            }
            s1++;
            s2++;
        } while (c1 && c2);

        if (c1 != c2)
        {
            return c1 - c2;
        }
    }

    return 0;
}

char* StrUpr(char* str)
{
    if (str)
    {
        char c;
        char* p = str;

        while ((c = *p))
        {
            if (c >= 'a' && c <= 'z')
            {
                *p = c - ('a' - 'A');
            }
            p++;
        }
    }

    return str;
}

char* StrLwr(char* str)
{
    if (str)
    {
        char c;
        char* p = str;

        while ((c = *p))
        {
            if (c >= 'A' && c <= 'Z')
            {
                *p = c + ('a' - 'A');
            }
            p++;
        }
    }

    return str;
}

char* StrChr(const char* str, char c)
{
    char* found = NULL;
    S32 match = c;
    char ch;

    do
    {
        ch = *str;
        if (ch == match)
        {
            found = (char*)str;
            break;
        }
        str++;
    } while (ch);

    return found;
}

char* StrRChr(const char* str, char c)
{
    char* found = NULL;
    S32 match = c;
    char ch;

    do
    {
        ch = *str;
        if (ch == match)
        {
            found = (char*)str;
        }
        str++;
    } while (ch);

    return found;
}

// Fills the engine's string function table: the C library's functions, and this file's own for
// the case-insensitive compare, the case conversions and the character searches.
S32 _rwStringOpen(void)
{
    RWSRCGLOBAL(stringFuncs).vecSprintf = sprintf;
    RWSRCGLOBAL(stringFuncs).vecVsprintf = vsprintf;
    RWSRCGLOBAL(stringFuncs).vecStrcpy = strcpy;
    RWSRCGLOBAL(stringFuncs).vecStrncpy = strncpy;
    RWSRCGLOBAL(stringFuncs).vecStrcat = strcat;
    RWSRCGLOBAL(stringFuncs).vecStrncat = strncat;
    RWSRCGLOBAL(stringFuncs).vecStrrchr = StrRChr;
    RWSRCGLOBAL(stringFuncs).vecStrchr = StrChr;
    RWSRCGLOBAL(stringFuncs).vecStrstr = strstr;
    RWSRCGLOBAL(stringFuncs).vecStrcmp = strcmp;
    RWSRCGLOBAL(stringFuncs).vecStricmp = StrICmp;
    RWSRCGLOBAL(stringFuncs).vecStrlen = strlen;
    RWSRCGLOBAL(stringFuncs).vecStrupr = StrUpr;
    RWSRCGLOBAL(stringFuncs).vecStrlwr = StrLwr;
    RWSRCGLOBAL(stringFuncs).vecStrtok = strtok;
    RWSRCGLOBAL(stringFuncs).vecSscanf = sscanf;

    return TRUE;
}

void _rwStringClose(void)
{
}
