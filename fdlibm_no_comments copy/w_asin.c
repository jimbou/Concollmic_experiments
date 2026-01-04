






#include "fdlibm.h"
#include <stdio.h>


#ifdef __STDC__
	double asin(double x)		
#else
	double asin(x)			
	double x;
#endif
{
#ifdef _IEEE_LIBM
	return __ieee754_asin(x);
#else
	double z;
	z = __ieee754_asin(x);
	if(_LIB_VERSION == _IEEE_ || isnan(x)) {
		return z;
	}
	if(fabs(x)>1.0) {
	        return __kernel_standard(x,x,2); 
	} else {
	    return z;
	}
#endif
}

#include <stdio.h>
#include <stdlib.h>

