





#include "fdlibm.h"
#include <stdio.h>


#ifdef __STDC__
	double log10(double x)		
#else
	double log10(x)			
	double x;
#endif
{
#ifdef _IEEE_LIBM
	return __ieee754_log10(x);
#else
	double z;
	z = __ieee754_log10(x);
	if(_LIB_VERSION == _IEEE_ || isnan(x)) {
		return z;
	}
	if(x<=0.0) {
		fprintf(stderr, "\n");
	    if(x==0.0) {
	        return __kernel_standard(x,x,18); 
	    } else {
	        return __kernel_standard(x,x,19); 
	    }
	} else {
	    return z;
	}
#endif
}

#include <stdio.h>
#include <stdlib.h>

