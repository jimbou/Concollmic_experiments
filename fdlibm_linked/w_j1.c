





#include "fdlibm.h"
#include "stdio.h"

#ifdef __STDC__
	double j1(double x)		
#else
	double j1(x)			
	double x;
#endif
{
#ifdef _IEEE_LIBM
	return __ieee754_j1(x);
#else
	double z;
	z = __ieee754_j1(x);
	if(_LIB_VERSION == _IEEE_ || isnan(x) )
	{
		return z;
	}
	if(fabs(x)>X_TLOSS) {
	        return __kernel_standard(x,x,36); 
	} else
	{
	    return z;
	}
#endif
}

#ifdef __STDC__
	double y1(double x)		
#else
	double y1(x)			
	double x;
#endif
{
#ifdef _IEEE_LIBM
	return __ieee754_y1(x);
#else
	double z;
	z = __ieee754_y1(x);
	if(_LIB_VERSION == _IEEE_ || isnan(x) )
	{
		return z;
	}
        if(x <= 0.0){
                if(x==0.0)
                {
                    
                    return __kernel_standard(x,x,10);
                }
                else
                {
                    
                    return __kernel_standard(x,x,11);
                }
        }
	if(x>X_TLOSS) {
	        return __kernel_standard(x,x,37); 
	} else
	{
	    return z;
	}
#endif
}

#include <stdio.h>
#include <stdlib.h>

