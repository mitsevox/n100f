#ifndef MSL_BUFFER_IO_H
#define MSL_BUFFER_IO_H

#include "MSL_C.PPCEABI.bare.H/ansi_files.h"

int setvbuf(FILE* file, char* buffer, int mode, size_t size);
int __flush_buffer(FILE* file, size_t* bytes_flushed);
int __load_buffer(FILE* file, size_t* bytes_loaded, int alignment);
void __prep_buffer(FILE* file);
int fflush(FILE* file);
int fclose(FILE* file);

#endif
