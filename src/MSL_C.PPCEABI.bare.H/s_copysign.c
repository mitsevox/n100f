/*
 * copysign(double x, double y)
 * copysign(x,y) returns a value with the magnitude of x and
 * with the sign bit of y.
 */

#include "MSL_C.PPCEABI.bare.H/fdlibm.h"

double copysign(double x, double y)
{
  __HI(x) = (__HI(x) & 0x7fffffff) | (__HI(y) & 0x80000000);
  return x;
}
