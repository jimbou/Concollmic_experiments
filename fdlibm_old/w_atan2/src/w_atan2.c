





#include "fdlibm.h"


#ifdef __STDC__
	double atan2(double y, double x)	
#else
	double atan2(y,x)			
	double y,x;
#endif
{
#ifdef _IEEE_LIBM
	return __ieee754_atan2(y,x);
#else
	double z;
	z = __ieee754_atan2(y,x);
	if(_LIB_VERSION == _IEEE_||isnan(x)||isnan(y)) return z;
	if(x==0.0&&y==0.0) {
	        return __kernel_standard(y,x,3); 
	} else
	    return z;
#endif
}
