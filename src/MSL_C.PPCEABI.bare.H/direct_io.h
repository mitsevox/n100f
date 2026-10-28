#ifndef MSL_DIRECT_IO_H
#define MSL_DIRECT_IO_H

#include "MSL_C.PPCEABI.bare.H/ansi_files.h"

size_t __fwrite(const void* buffer, size_t memb_size, size_t num_memb, FILE* file);
size_t fwrite(const void* buffer, size_t memb_size, size_t num_memb, FILE* file);
size_t __fread(void* buffer, size_t memb_size, size_t num_memb, FILE* file);
size_t fread(void* buffer, size_t memb_size, size_t num_memb, FILE* file);

#endif
