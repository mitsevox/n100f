#include <stdlib.h>
#include <string.h>
#include "MSL_C.PPCEABI.bare.H/ansi_files.h"
#include "MSL_C.PPCEABI.bare.H/buffer_io.h"

int __read_console(__file_handle file, unsigned char* buff, size_t* count, __idle_proc idle_proc);
int __write_console(__file_handle file, unsigned char* buff, size_t* count, __idle_proc idle_proc);
int __close_console(__file_handle file);

int __position_file(__file_handle file, unsigned long* position, int mode, __idle_proc idle_proc);
int __read_file(__file_handle file, unsigned char* buff, size_t* count, __idle_proc idle_proc);
int __write_file(__file_handle file, unsigned char* buff, size_t* count, __idle_proc idle_proc);
int __close_file(__file_handle file);

static unsigned char stdin_buff[0x100];
static unsigned char stdout_buff[0x100];
static unsigned char stderr_buff[0x100];

FILE __files[4] = {
    { 0, { 0, 1, 1, __console_file, 0 }, { __neutral, 0, 0, 0 }, 0, 0, 0, { 0, 0 }, { 0, 0 }, 0,
      stdin_buff, sizeof(stdin_buff), stdin_buff, 0, 0, 0, 0,
      NULL, __read_console, __write_console, __close_console, NULL, &__files[1] },
    { 1, { 0, 2, 1, __console_file, 0 }, { __neutral, 0, 0, 0 }, 0, 0, 0, { 0, 0 }, { 0, 0 }, 0,
      stdout_buff, sizeof(stdout_buff), stdout_buff, 0, 0, 0, 0,
      NULL, __read_console, __write_console, __close_console, NULL, &__files[2] },
    { 2, { 0, 2, 0, __console_file, 0 }, { __neutral, 0, 0, 0 }, 0, 0, 0, { 0, 0 }, { 0, 0 }, 0,
      stderr_buff, sizeof(stderr_buff), stderr_buff, 0, 0, 0, 0,
      NULL, __read_console, __write_console, __close_console, NULL, &__files[3] },
};

FILE* __find_unopened_file(void)
{
    FILE* p = __files[2].next_file_struct;
    FILE* plast;

    while (p != NULL) {
        if (p->mode.file_kind == __closed_file)
            return p;

        plast = p;
        p     = p->next_file_struct;
    }

    if ((p = (FILE*)malloc(sizeof(FILE))) != NULL) {
        memset(p, 0, sizeof(FILE));
        p->is_dynamically_allocated = 1;
        plast->next_file_struct     = p;
        return p;
    }

    return NULL;
}

void __init_file(FILE* file, __file_modes mode, char* buff, size_t size)
{
    file->handle            = 0;
    file->mode              = mode;
    file->state.io_state    = __neutral;
    file->state.free_buffer = 0;
    file->state.eof         = 0;
    file->state.error       = 0;
    file->position          = 0;

    if (size)
        setvbuf(file, buff, _IOFBF, size);
    else
        setvbuf(file, NULL, _IONBF, 0);

    file->buffer_ptr    = file->buffer;
    file->buffer_length = 0;

    if (file->mode.file_kind == __disk_file) {
        file->position_proc = __position_file;
        file->read_proc     = __read_file;
        file->write_proc    = __write_file;
        file->close_proc    = __close_file;
    }

    file->idle_proc = NULL;
}

void __close_all(void)
{
    FILE* file = &__files[0];
    FILE* last_file;

    while (file != NULL) {
        if (file->mode.file_kind != __closed_file) {
            fclose(file);
        }

        last_file = file;
        file      = file->next_file_struct;

        if (last_file->is_dynamically_allocated) {
            free(last_file);
        } else {
            last_file->mode.file_kind = __unavailable_file;
            if (file != NULL && file->is_dynamically_allocated) {
                last_file->next_file_struct = NULL;
            }
        }
    }
}

unsigned int __flush_all(void)
{
    unsigned int ret = 0;
    FILE* file       = &__files[0];

    while (file) {
        if (file->mode.file_kind != __closed_file && fflush(file)) {
            ret = -1;
        }
        file = file->next_file_struct;
    }

    return ret;
}

int __flush_line_buffered_output_files(void)
{
    int ret    = 0;
    FILE* file = &__files[0];

    while (file) {
        if (file->mode.file_kind != __closed_file && (file->mode.buffer_mode & _IOLBF)
            && file->state.io_state == __writing) {
            if (fflush(file))
                ret = -1;
        }
        file = file->next_file_struct;
    }

    return ret;
}
