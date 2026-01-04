





#include "fdlibm.h"
#include <stdio.h>


#ifdef __STDC__
	double log(double x)		
#else
	double log(x)			
	double x;
#endif
{
#ifdef _IEEE_LIBM
	return __ieee754_log(x);
#else
	double z;
	z = __ieee754_log(x);
	if(_LIB_VERSION == _IEEE_ || isnan(x) || x > 0.0) {
		return z;
	}
	if(x==0.0)
	{
		return __kernel_standard(x,x,16); 
	}
	else 
	{
		return __kernel_standard(x,x,17); 
	}
#endif
}

#include <stdio.h>
#include <stdlib.h>

