





#include "fdlibm.h"

#ifdef __STDC__
	double acosh(double x)		
#else
	double acosh(x)			
	double x;
#endif
{
#ifdef _IEEE_LIBM
	return __ieee754_acosh(x);
#else
	double z;
	z = __ieee754_acosh(x);
	if(_LIB_VERSION == _IEEE_ || isnan(x)) return z;
	if(x<1.0) {
	        return __kernel_standard(x,x,29); 
	} else
	    return z;
#endif
}
