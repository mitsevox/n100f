/*
 * wrapper pow(x,y) return x**y
 */

#include "MSL_C.PPCEABI.bare.H/fdlibm.h"

double pow(double x, double y) /* wrapper pow */
{
  return __ieee754_pow(x, y);
}
