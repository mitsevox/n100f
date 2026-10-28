/*
 * wrapper atan2(y,x)
 */

#include "MSL_C.PPCEABI.bare.H/fdlibm.h"

double atan2(double y, double x) /* wrapper atan2 */
{
  return __ieee754_atan2(y, x);
}
