#include <limits.h>
#include <locale.h>
#include <string.h>

// The "C" locale: only the decimal point is set, every other field is empty or CHAR_MAX.
struct lconv __lconv = {
    ".",      "",       "",       "",       "",       "",       "",       "",       "",
    CHAR_MAX, CHAR_MAX, CHAR_MAX, CHAR_MAX, CHAR_MAX, CHAR_MAX, CHAR_MAX, "",       CHAR_MAX,
    CHAR_MAX, CHAR_MAX, CHAR_MAX, CHAR_MAX, CHAR_MAX, CHAR_MAX,
};

// Only the "C" locale exists.
char* setlocale(int category, const char* locale)
{
    if (locale == NULL || strcmp(locale, "C") == 0 || strcmp(locale, "") == 0)
    {
        return "C";
    }
    return NULL;
}
