#ifndef MSL_SCANF_H
#define MSL_SCANF_H

enum __ReadProcActions { __GetAChar, __UngetAChar, __TestForError };

typedef struct {
    char* NextChar;
    int NullCharDetected;
} __InStrCtrl;

int __StringRead(void* str, int ch, int behavior);

#endif
