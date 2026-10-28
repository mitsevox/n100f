#ifndef MSL_CHAR_IO_H
#define MSL_CHAR_IO_H

#include "MSL_C.PPCEABI.bare.H/ansi_files.h"

#define SEEK_SET 0
#define SEEK_CUR 1
#define SEEK_END 2

int fputs(const char* string, FILE* file);
int fputc(int character, FILE* file);
char* fgets(char* string, int count, FILE* file);
int fgetc(FILE* file);
int fwide(FILE* file, int mode);
int fseek(FILE* file, long offset, int mode);
void __stdio_atexit(void);

#endif
