





#include "fdlibm.h"

#ifdef __STDC__
	double sin(double x)
#else
	double sin(x)
	double x;
#endif
{
	double y[2],z=0.0;
	int n, ix;

    
	ix = __HI(x);

    
	ix &= 0x7fffffff;
	if(ix <= 0x3fe921fb) return __kernel_sin(x,z,0);

    
	else if (ix>=0x7ff00000) return x-x;

    
	else {
	    n = __ieee754_rem_pio2(x,y);
	    switch(n&3) {
		case 0: return  __kernel_sin(y[0],y[1],1);
		case 1: return  __kernel_cos(y[0],y[1]);
		case 2: return -__kernel_sin(y[0],y[1],1);
		default:
			return -__kernel_cos(y[0],y[1]);
	    }
	}
}

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {

    if (argc != 2) {
        printf("Usage: %s <arg>\n", argv[0]);
        return 1;
    }

    double a0 = atof(argv[1]);

    double r = sin(a0);
    printf("%f\n", r);

    return 0;
}
