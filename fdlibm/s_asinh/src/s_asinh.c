





#include "fdlibm.h"

#ifdef __STDC__
static const double 
#else
static double 
#endif
one =  1.00000000000000000000e+00, 
ln2 =  6.93147180559945286227e-01, 
huge=  1.00000000000000000000e+300; 

#ifdef __STDC__
	double asinh(double x)
#else
	double asinh(x)
	double x;
#endif
{	
	double t,w;
	int hx,ix;
	hx = __HI(x);
	ix = hx&0x7fffffff;
	if(ix>=0x7ff00000) return x+x;	
	if(ix< 0x3e300000) {	
	    if(huge+x>one) return x;	
	} 
	if(ix>0x41b00000) {	
	    w = __ieee754_log(fabs(x))+ln2;
	} else if (ix>0x40000000) {	
	    t = fabs(x);
	    w = __ieee754_log(2.0*t+one/(sqrt(x*x+one)+t));
	} else {		
	    t = x*x;
	    w =log1p(fabs(x)+t/(one+sqrt(one+t)));
	}
	if(hx>0) return w; else return -w;
}

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {

    if (argc != 2) {
        printf("Usage: %s <arg>\n", argv[0]);
        return 1;
    }

    double a0 = atof(argv[1]);

    double r = asinh(a0);
    printf("%f\n", r);

    return 0;
}
