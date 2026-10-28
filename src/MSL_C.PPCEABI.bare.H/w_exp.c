/*
 * wrapper exp(x)
 */

#include "MSL_C.PPCEABI.bare.H/fdlibm.h"

double exp(double x) /* wrapper exp */
{
  return __ieee754_exp(x);
}
