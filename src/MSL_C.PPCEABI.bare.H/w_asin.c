/*
 * wrapper asin(x)
 */

#include "MSL_C.PPCEABI.bare.H/fdlibm.h"

double asin(double x) /* wrapper asin */
{
  return __ieee754_asin(x);
}
