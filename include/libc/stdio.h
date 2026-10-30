#ifndef STDIO_H
#define STDIO_H

#include <stdarg.h>

// MSL C library declarations, limited to what reconstructed units use (docs/libraries.md).
#include <stddef.h>
#include <eof.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef unsigned long __file_handle;

enum __file_kinds { __closed_file, __disk_file, __console_file, __unavailable_file };

enum __file_orientation { __unoriented, __char_oriented, __wide_oriented };

enum __io_modes { __read = 1, __write = 2, __read_write = 3, __append = 4 };

enum __io_states { __neutral, __writing, __reading, __rereading };

enum __io_results { __no_io_error, __io_error, __io_EOF };

#define _IONBF 0
#define _IOLBF 1
#define _IOFBF 2

typedef struct {
    unsigned int open_mode : 2;
    unsigned int io_mode : 3;
    unsigned int buffer_mode : 2;
    unsigned int file_kind : 3;
    unsigned int file_orientation : 2;
    unsigned int binary_io : 1;
} __file_modes;

typedef struct {
    unsigned int io_state : 3;
    unsigned int free_buffer : 1;
    unsigned char eof;
    unsigned char error;
} __file_state;

typedef void (*__idle_proc)(void);
typedef int (*__pos_proc)(__file_handle file, unsigned long* position, int mode,
                          __idle_proc idle_proc);
typedef int (*__io_proc)(__file_handle file, unsigned char* buff, size_t* count,
                         __idle_proc idle_proc);
typedef int (*__close_proc)(__file_handle file);

typedef struct _FILE {
    __file_handle handle;
    __file_modes mode;
    __file_state state;
    unsigned char is_dynamically_allocated;
    char char_buffer;
    char char_buffer_overflow;
    char ungetc_buffer[2];
    wchar_t ungetc_wide_buffer[2];
    unsigned long position;
    unsigned char* buffer;
    unsigned long buffer_size;
    unsigned char* buffer_ptr;
    unsigned long buffer_length;
    unsigned long buffer_alignment;
    unsigned long saved_buffer_length;
    unsigned long buffer_position;
    __pos_proc position_proc;
    __io_proc read_proc;
    __io_proc write_proc;
    __close_proc close_proc;
    __idle_proc idle_proc;
    struct _FILE* next_file_struct;
} FILE;

extern FILE __files[4];

#define stderr (&__files[2])

int fprintf(FILE* file, const char* format, ...);
int printf(const char* format, ...);
int sprintf(char* s, const char* format, ...);
int vprintf(const char* format, va_list arg);

FILE* fopen(const char* name, const char* mode);
int fclose(FILE* file);
size_t fread(void* buffer, size_t memb_size, size_t num_memb, FILE* file);
size_t fwrite(const void* buffer, size_t memb_size, size_t num_memb, FILE* file);
char* fgets(char* string, int count, FILE* file);
int fputs(const char* string, FILE* file);
int feof(FILE* file);
int fseek(FILE* file, long offset, int mode);
int fflush(FILE* file);
long ftell(FILE* file);

#ifdef __cplusplus
}
#endif

#endif
