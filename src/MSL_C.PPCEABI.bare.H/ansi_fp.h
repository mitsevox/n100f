#ifndef MSL_ANSI_FP_H
#define MSL_ANSI_FP_H

#define SIGDIGLEN 36

typedef struct decimal {
    char sign;
    char unused;
    short exp;
    struct {
        unsigned char length;
        unsigned char text[SIGDIGLEN];
        unsigned char unused;
    } sig;
} decimal;

double __dec2num(const decimal* d);

#endif
