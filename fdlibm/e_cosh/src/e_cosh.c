





#include "fdlibm.h"

#ifdef __STDC__
static const double one = 1.0, half=0.5, huge = 1.0e300;
#else
static double one = 1.0, half=0.5, huge = 1.0e300;
#endif

#ifdef __STDC__
	double __ieee754_cosh(double x)
#else
	double __ieee754_cosh(x)
	double x;
#endif
{	
	double t,w;
	int ix;
	unsigned lx;

    
	ix = __HI(x);
	ix &= 0x7fffffff;

    
	if(ix>=0x7ff00000) return x*x;	

    
	if(ix<0x3fd62e43) {
	    t = expm1(fabs(x));
	    w = one+t;
	    if (ix<0x3c800000) return w;	
	    return one+(t*t)/(w+w);
	}

    
	if (ix < 0x40360000) {
		t = __ieee754_exp(fabs(x));
		return half*t+half/t;
	}

    
	if (ix < 0x40862E42)  return half*__ieee754_exp(fabs(x));

    
	lx = *( (((*(unsigned*)&one)>>29)) + (unsigned*)&x);
	if (ix<0x408633CE || 
	      (ix==0x408633ce)&&(lx<=(unsigned)0x8fb9f87d)) {
	    w = __ieee754_exp(half*fabs(x));
	    t = half*w;
	    return t*w;
	}

    
	return huge*huge;
}

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {

    if (argc != 2) {
        printf("Usage: %s <arg>\n", argv[0]);
        return 1;
    }

    double a0 = atof(argv[1]);

    double r = __ieee754_cosh(a0);
    printf("%f\n", r);

    return 0;
}
