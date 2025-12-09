





#include "fdlibm.h"

#ifdef __STDC__
	double j0(double x)		
#else
	double j0(x)			
	double x;
#endif
{
#ifdef _IEEE_LIBM
	return __ieee754_j0(x);
#else
	double z = __ieee754_j0(x);
	if(_LIB_VERSION == _IEEE_ || isnan(x)) return z;
	if(fabs(x)>X_TLOSS) {
	        return __kernel_standard(x,x,34); 
	} else
	    return z;
#endif
}

#ifdef __STDC__
	double y0(double x)		
#else
	double y0(x)			
	double x;
#endif
{
#ifdef _IEEE_LIBM
	return __ieee754_y0(x);
#else
	double z;
	z = __ieee754_y0(x);
	if(_LIB_VERSION == _IEEE_ || isnan(x) ) return z;
        if(x <= 0.0){
                if(x==0.0)
                    
                    return __kernel_standard(x,x,8);
                else
                    
                    return __kernel_standard(x,x,9);
        }
	if(x>X_TLOSS) {
	        return __kernel_standard(x,x,35); 
	} else
	    return z;
#endif
}
