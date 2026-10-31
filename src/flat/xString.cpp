#include "xString.h"

#include <stdlib.h>

// Case-insensitive: lower-case letters hash as upper case.
U32 xStrHash(char* str)
{
    U32 i = 0;
    char c;

    while ((c = *str) != '\0')
    {
        str++;
        i = (char)(c - (c & (c >> 1) & 0x20)) + i * 0x83;
    }

    return i;
}

// strtok with the position kept in nextoken instead of a static.
char* xStrTok(char* string, const char* control, char** nextoken)
{
    U8* str;
    const U8* ctrl = (const U8*)control;
    U8 map[32];
    S32 count;

    for (count = 0; count < 32; count++)
    {
        map[count] = 0;
    }

    do
    {
        map[*ctrl >> 3] |= (U8)(1 << (*ctrl & 0x7));
    } while (*ctrl++ != '\0');

    if (string != NULL)
    {
        str = (U8*)string;
    }
    else
    {
        str = (U8*)*nextoken;
    }

    while ((map[(*str >> 3) & 0x1F] & (1 << (*str & 0x7))) && *str != '\0')
    {
        str++;
    }

    string = (char*)str;

    for (; *str != '\0'; str++)
    {
        if (map[(*str >> 3) & 0x1F] & (1 << (*str & 0x7)))
        {
            *str++ = '\0';
            break;
        }
    }

    *nextoken = (char*)str;

    if (string == (char*)str)
    {
        return NULL;
    }

    return string;
}

// Like xStrTok, but copies the token into the buffer and leaves the source alone. The buffer holds
// the position for the next call first, then the token.
char* xStrTokBuffer(const char* string, const char* control, void* buffer)
{
    U8* str;
    const U8* ctrl = (const U8*)control;
    U8 map[32];
    S32 count;
    char** nextoken = (char**)buffer;
    char* dest = (char*)buffer + sizeof(char*);
    char* token;

    for (count = 0; count < 32; count++)
    {
        map[count] = 0;
    }

    do
    {
        map[*ctrl >> 3] |= (U8)(1 << (*ctrl & 0x7));
    } while (*ctrl++ != '\0');

    if (string != NULL)
    {
        str = (U8*)string;
    }
    else
    {
        str = (U8*)*nextoken;
    }

    while ((map[(*str >> 3) & 0x1F] & (1 << (*str & 0x7))) && *str != '\0')
    {
        str++;
    }

    token = (char*)str;

    for (; *str != '\0'; str++)
    {
        if (map[(*str >> 3) & 0x1F] & (1 << (*str & 0x7)))
        {
            str++;
            break;
        }
        *dest++ = *str;
    }

    *dest = '\0';
    *nextoken = (char*)str;

    if (token == (char*)str)
    {
        return NULL;
    }

    return (char*)buffer + sizeof(char*);
}

static inline char xStrToUpper(char c)
{
    bool lower = false;
    if (c >= 'a' && c <= 'z')
    {
        lower = true;
    }
    if (lower)
    {
        c -= 'a' - 'A';
    }
    return c;
}

S32 xStricmp(const char* string1, const char* string2)
{
    S32 result = 0;
    S32 done = 0;

    while (xStrToUpper(*string1) == xStrToUpper(*string2) && !done)
    {
        if (*string1 == '\0' || *string2 == '\0')
        {
            done = 1;
        }
        else
        {
            string1++;
            string2++;
        }
    }

    if (*string1 != *string2)
    {
        result = 1;
        if (xStrToUpper(*string1) < xStrToUpper(*string2))
        {
            result = -1;
        }
    }

    return result;
}

char* xStrupr(char* string)
{
    char* p = string;

    while (*p != '\0')
    {
        *p = (*p >= 'a' && *p <= 'z' ? *p - 32 : *p);
        p++;
    }

    return string;
}

// Reads up to max numbers from text such as "[1, -2.5, 3]" into dest and returns how many it read.
// Brackets, braces, parentheses, '+', ',', ':' and ';' separate numbers like white space does.
S32 xStrParseFloatList(F32* dest, const char* strbuf, S32 max)
{
    char* str = (char*)strbuf;
    S32 index;
    S32 digits;
    bool negate;
    char* numstart;
    char savech;

    if (!str)
    {
        return 0;
    }

    for (index = 0; *str != '\0' && index < max; index++)
    {
        while (*str == '\t' || *str == ' ' || *str == '[' || *str == ']' || *str == '{' ||
               *str == '}' || *str == '(' || *str == ')' || *str == '+' || *str == ',' ||
               *str == ':' || *str == ';')
        {
            str++;
        }

        if (*str == '\0')
        {
            return index;
        }

        if (*str == '-')
        {
            negate = true;
            str++;

            while (*str == '\t' || *str == ' ')
            {
                str++;
            }
        }
        else
        {
            negate = false;
        }

        numstart = str;
        digits = 0;

        while ((*str >= '0' && *str <= '9') || *str == '.' || *str == 'E' || *str == 'e' ||
               *str == 'f')
        {
            if (*str >= '0' && *str <= '9')
            {
                digits++;
            }

            str++;
        }

        if (digits == 0)
        {
            return index;
        }

        savech = *str;

        *str = '\0';
        *dest = atof(numstart);

        if (negate)
        {
            *dest = -*dest;
        }

        *str = savech;
        dest++;
    }

    return index;
}
