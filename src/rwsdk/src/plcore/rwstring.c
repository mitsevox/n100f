#include <rwcore.h>

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
    char ch;

    do
    {
        ch = *str;
        if (ch == c)
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
    char ch;

    do
    {
        ch = *str;
        if (ch == c)
        {
            found = (char*)str;
        }
        str++;
    } while (ch);

    return found;
}
