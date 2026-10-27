/*
 * wrapper fmod(x,y)
 */

#include "MSL_C.PPCEABI.bare.H/fdlibm.h"

double fmod(double x, double y) /* wrapper fmod */
{
  return __ieee754_fmod(x, y);
}
