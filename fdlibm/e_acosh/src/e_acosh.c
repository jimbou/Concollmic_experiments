





#include "fdlibm.h"

#ifdef __STDC__
static const double 
#else
static double 
#endif
one	= 1.0,
ln2	= 6.93147180559945286227e-01;  

#ifdef __STDC__
	double __ieee754_acosh(double x)
#else
	double __ieee754_acosh(x)
	double x;
#endif
{	
	double t;
	int hx;
	hx = __HI(x);
	if(hx<0x3ff00000) {		
	    return (x-x)/(x-x);
	} else if(hx >=0x41b00000) {	
	    if(hx >=0x7ff00000) {	
	        return x+x;
	    } else 
		return __ieee754_log(x)+ln2;	
	} else if(((hx-0x3ff00000)|__LO(x))==0) {
	    return 0.0;			
	} else if (hx > 0x40000000) {	
	    t=x*x;
	    return __ieee754_log(2.0*x-one/(x+sqrt(t-one)));
	} else {			
	    t = x-one;
	    return log1p(t+sqrt(2.0*t+t*t));
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

    double r = __ieee754_acosh(a0);
    printf("%f\n", r);

    return 0;
}
