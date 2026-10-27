#include "MSL_C.PPCEABI.bare.H/ansi_files.h"

extern void (*__stdio_exit)(void);
void __close_all(void);

void clearerr(FILE* file)
{
    file->state.eof = 0;
    file->state.error = 0;
}

int feof(FILE* file)
{
    return file->state.eof;
}

void __stdio_atexit(void)
{
    __stdio_exit = __close_all;
}
