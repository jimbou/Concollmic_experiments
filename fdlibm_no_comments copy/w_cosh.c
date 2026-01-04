





#include "fdlibm.h"
#include <stdio.h>

#ifdef __STDC__
	double cosh(double x)		
#else
	double cosh(x)			
	double x;
#endif
{
#ifdef _IEEE_LIBM
	return __ieee754_cosh(x);
#else
	double z;
	z = __ieee754_cosh(x);
	if(_LIB_VERSION == _IEEE_ || isnan(x)) {
		return z;
	}
	if(fabs(x)>7.10475860073943863426e+02) {	
	        return __kernel_standard(x,x,5); 
	} else {
	    return z;
	}
#endif
}

#include <stdio.h>
#include <stdlib.h>

