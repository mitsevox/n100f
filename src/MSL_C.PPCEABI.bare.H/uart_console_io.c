#include "MSL_C.PPCEABI.bare.H/ansi_files.h"

int InitializeUART(unsigned long baud_rate);
int WriteUARTN(const void* bytes, unsigned long length);
int __TRK_write_console(__file_handle handle, unsigned char* buffer, size_t* count,
                        __idle_proc idle_proc);

static inline int __init_uart_console(void)
{
    int err = 0;
    static int initialized = 0;

    if (initialized == 0) {
        err = InitializeUART(57600);
        if (err == 0)
            initialized = 1;
    }

    return err;
}

int __write_console(__file_handle handle, unsigned char* buffer, size_t* count,
                    __idle_proc idle_proc)
{
    if (__init_uart_console() != 0) {
        return 1;
    }

    if (WriteUARTN(buffer, *count) != 0) {
        *count = 0;
        return 1;
    }

    __TRK_write_console(handle, buffer, count, idle_proc);
    return 0;
}
