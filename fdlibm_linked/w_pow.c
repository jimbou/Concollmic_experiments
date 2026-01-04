






#include "fdlibm.h"
#include "stdio.h"


#ifdef __STDC__
	double pow(double x, double y)	
#else
	double pow(x,y)			
	double x,y;
#endif
{
#ifdef _IEEE_LIBM
	return  __ieee754_pow(x,y);
#else
	fprintf(stderr, "\n");
	double z;
	z=__ieee754_pow(x,y);
	if(_LIB_VERSION == _IEEE_|| isnan(y)) {
	    return z;
	}
	if(isnan(x)) {
	    if(y==0.0) {
	        return __kernel_standard(x,y,42); 
	    } else {
		return z;
	    }
	}
	if(x==0.0){ 
	    if(y==0.0) {
	        return __kernel_standard(x,y,20); 
	    }
	    if(finite(y)&&y<0.0) {
	        return __kernel_standard(x,y,23); 
	    }
	    return z;
	}
	if(!finite(z)) {
	    if(finite(x)&&finite(y)) {
	        if(isnan(z)) {
	            return __kernel_standard(x,y,24); 
	        } else {
	            return __kernel_standard(x,y,21); 
	        }
	    }
	} 
	if(z==0.0&&finite(x)&&finite(y)) {
	    return __kernel_standard(x,y,22); 
	}
	return z;
#endif
}

#include <stdio.h>
#include <stdlib.h>

