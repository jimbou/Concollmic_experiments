





#include "fdlibm.h"

#ifdef __STDC__
static const double
#else
static double
#endif
o_threshold=  7.09782712893383973096e+02,  
u_threshold= -7.45133219101941108420e+02;  

#ifdef __STDC__
	double exp(double x)		
#else
	double exp(x)			
	double x;
#endif
{
#ifdef _IEEE_LIBM
	return __ieee754_exp(x);
#else
	double z;
	z = __ieee754_exp(x);
	if(_LIB_VERSION == _IEEE_) return z;
	if(finite(x)) {
	    if(x>o_threshold)
	        return __kernel_standard(x,x,6); 
	    else if(x<u_threshold)
	        return __kernel_standard(x,x,7); 
	} 
	return z;
#endif
}

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {

    if (argc != 2) {
        printf("Usage: %s <arg>\n", argv[0]);
        return 1;
    }

    double a0 = atof(argv[1]);

    double r = exp(a0);
    printf("%f\n", r);

    return 0;
}
