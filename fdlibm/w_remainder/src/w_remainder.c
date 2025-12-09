





#include "fdlibm.h"

#ifdef __STDC__
	double remainder(double x, double y)	
#else
	double remainder(x,y)			
	double x,y;
#endif
{
#ifdef _IEEE_LIBM
	return __ieee754_remainder(x,y);
#else
	double z;
	z = __ieee754_remainder(x,y);
	if(_LIB_VERSION == _IEEE_ || isnan(y)) return z;
	if(y==0.0) 
	    return __kernel_standard(x,y,28); 
	else
	    return z;
#endif
}

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {

    if (argc != 3) {
        printf("Usage: %s <arg> <arg>\n", argv[0]);
        return 1;
    }

    double a0 = atof(argv[1]);
    double a1 = atof(argv[2]);

    double r = remainder(a0, a1);
    printf("%f\n", r);

    return 0;
}
