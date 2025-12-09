




#include "fdlibm.h"


#ifdef __STDC__
	double atanh(double x)		
#else
	double atanh(x)			
	double x;
#endif
{
#ifdef _IEEE_LIBM
	return __ieee754_atanh(x);
#else
	double z,y;
	z = __ieee754_atanh(x);
	if(_LIB_VERSION == _IEEE_ || isnan(x)) return z;
	y = fabs(x);
	if(y>=1.0) {
	    if(y>1.0)
	        return __kernel_standard(x,x,30); 
	    else 
	        return __kernel_standard(x,x,31); 
	} else
	    return z;
#endif
}
