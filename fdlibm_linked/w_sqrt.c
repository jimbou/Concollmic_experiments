#include <stdio.h>
#include <stdlib.h>

#include "fdlibm.h"

#ifdef __STDC__
	double sqrt(double x)		
#else
	double sqrt(x)			
	double x;
#endif
{
#ifdef _IEEE_LIBM
	return __ieee754_sqrt(x);
#else
	double z;
	z = __ieee754_sqrt(x);
	if(_LIB_VERSION == _IEEE_ || isnan(x)) {
		return z;
	}
	if(x<0.0) {
		return __kernel_standard(x,x,26); 
	} else {
		return z;
	}
#endif
}

