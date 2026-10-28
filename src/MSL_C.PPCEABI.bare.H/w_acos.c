/*
 * wrap_acos(x)
 */

#include "MSL_C.PPCEABI.bare.H/fdlibm.h"

double acos(double x) /* wrapper acos */
{
  return __ieee754_acos(x);
}
