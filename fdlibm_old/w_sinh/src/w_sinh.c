





#include "fdlibm.h"

#ifdef __STDC__
	double sinh(double x)		
#else
	double sinh(x)			
	double x;
#endif
{
#ifdef _IEEE_LIBM
	return __ieee754_sinh(x);
#else
	double z; 
	z = __ieee754_sinh(x);
	if(_LIB_VERSION == _IEEE_) return z;
	if(!finite(z)&&finite(x)) {
	    return __kernel_standard(x,x,25); 
	} else
	    return z;
#endif
}
