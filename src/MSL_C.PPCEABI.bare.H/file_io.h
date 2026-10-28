#ifndef MSL_FILE_IO_H
#define MSL_FILE_IO_H

#include "MSL_C.PPCEABI.bare.H/ansi_files.h"

int __get_file_modes(const char* mode, __file_modes* modes);
FILE* freopen(const char* name, const char* mode, FILE* file);
FILE* fopen(const char* name, const char* mode);

#endif
