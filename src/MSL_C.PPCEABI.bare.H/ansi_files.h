#ifndef MSL_ANSI_FILES_H
#define MSL_ANSI_FILES_H

#include <stdio.h>


void __init_file(FILE* file, __file_modes mode, char* buff, size_t size);
FILE* __find_unopened_file(void);
unsigned int __flush_all(void);
void __close_all(void);
int __flush_line_buffered_output_files(void);

#endif
