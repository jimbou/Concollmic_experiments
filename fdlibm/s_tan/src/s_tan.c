





#include "fdlibm.h"

#ifdef __STDC__
	double tan(double x)
#else
	double tan(x)
	double x;
#endif
{
	double y[2],z=0.0;
	int n, ix;

    
	ix = __HI(x);

    
	ix &= 0x7fffffff;
	if(ix <= 0x3fe921fb) return __kernel_tan(x,z,1);

    
	else if (ix>=0x7ff00000) return x-x;		

    
	else {
	    n = __ieee754_rem_pio2(x,y);
	    return __kernel_tan(y[0],y[1],1-((n&1)<<1)); 
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

    double r = tan(a0);
    printf("%f\n", r);

    return 0;
}
