#include <stdio.h>
#include "fdlibm.h"


#ifdef __STDC__
	double fmod(double x, double y)	
#else
	double fmod(x,y)		
	double x,y;
#endif
{
#ifdef _IEEE_LIBM
	return __ieee754_fmod(x,y);
#else
	double z;
	z = __ieee754_fmod(x,y);
	if(_LIB_VERSION == _IEEE_ ||isnan(y)||isnan(x)) {
		return z;
	}
	if(y==0.0) {
	        return __kernel_standard(x,y,27); 
	} else {
	    return z;
	}
#endif
}

#include <stdio.h>
#include <stdlib.h>

