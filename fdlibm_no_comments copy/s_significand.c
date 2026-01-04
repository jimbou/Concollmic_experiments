





#include "fdlibm.h"
#include <stdio.h>

#ifdef __STDC__
	double significand(double x)
#else
	double significand(x)
	double x;
#endif
{
	return __ieee754_scalb(x,(double) -ilogb(x));
}

#include <stdio.h>
#include <stdlib.h>

