





#include "fdlibm.h"
#include <stdio.h>

#ifdef __STDC__
static const double one = 1.0, shuge = 1.0e307;
#else
static double one = 1.0, shuge = 1.0e307;
#endif

#ifdef __STDC__
	double __ieee754_sinh(double x)
#else
	double __ieee754_sinh(x)
	double x;
#endif
{	
	fprintf(stderr, "\n");
	double t,w,h;
	int ix,jx;
	unsigned lx;

    
	jx = __HI(x);
	ix = jx&0x7fffffff;

    
	if(ix>=0x7ff00000) {
		return x+x;
	}

	h = 0.5;
	if (jx<0) {
		h = -h;
	}
    
	if (ix < 0x40360000) {		
		fprintf(stderr, "\n");
	    if (ix<0x3e300000) {		
			if(shuge+x>one) {
				return x;
			}
		}
	    t = expm1(fabs(x));
	    if(ix<0x3ff00000) {
			return h*(2.0*t-t*t/(t+one));
		}
	    return h*(t+t/(t+one));
	}

    
	if (ix < 0x40862E42) {
		return h*__ieee754_exp(fabs(x));
	}

    
	lx = *( (((*(unsigned*)&one)>>29)) + (unsigned*)&x);
	if (ix<0x408633CE || (ix==0x408633ce)&&(lx<=(unsigned)0x8fb9f87d)) {
	    w = __ieee754_exp(0.5*fabs(x));
	    t = h*w;
	    return t*w;
	}

    
	return x*shuge;
}

#include <stdio.h>
#include <stdlib.h>

