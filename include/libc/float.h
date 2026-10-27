#ifndef FLOAT_H
#define FLOAT_H

// MSL C library declarations, limited to what reconstructed units use (docs/libraries.md).
#ifdef __cplusplus
extern "C" {
#endif

extern int __double_min[];
extern int __double_max[];
extern int __extended_min[];
extern int __extended_max[];

#define DBL_MIN (*(double*)__double_min)
#define DBL_MAX (*(double*)__double_max)
#define LDBL_MIN (*(long double*)__extended_min)
#define LDBL_MAX (*(long double*)__extended_max)

#ifdef __cplusplus
}
#endif

#endif
