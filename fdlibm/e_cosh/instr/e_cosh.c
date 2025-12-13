





#include "fdlibm.h"
#include <stdio.h>

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
	fprintf(stderr, "[e_cosh.c] enter __ieee754_cosh 1\n");
	double t,w;
	int ix;
	unsigned lx;

    
	ix = __HI(x);
	ix &= 0x7fffffff;
	// fprintf(stderr, "[e_cosh.c] exit __ieee754_cosh 1\n");

    
	if(ix>=0x7ff00000) {
		fprintf(stderr, "[e_cosh.c] enter __ieee754_cosh 2\n");
		return x*x;
		// fprintf(stderr, "[e_cosh.c] exit __ieee754_cosh 2\n");
	}	

    
	if(ix<0x3fd62e43) {
		fprintf(stderr, "[e_cosh.c] enter __ieee754_cosh 3\n");
	    t = expm1(fabs(x));
	    w = one+t;
	    if (ix<0x3c800000) {
			fprintf(stderr, "[e_cosh.c] enter __ieee754_cosh 4\n");
			return w;
			// fprintf(stderr, "[e_cosh.c] exit __ieee754_cosh 4\n");
		}	
		fprintf(stderr, "[e_cosh.c] enter __ieee754_cosh 5\n");
	    return one+(t*t)/(w+w);
	    // fprintf(stderr, "[e_cosh.c] exit __ieee754_cosh 5\n");
	}
	// fprintf(stderr, "[e_cosh.c] exit __ieee754_cosh 3\n");

    
	if (ix < 0x40360000) {
		fprintf(stderr, "[e_cosh.c] enter __ieee754_cosh 6\n");
		t = __ieee754_exp(fabs(x));
		return half*t+half/t;
		// fprintf(stderr, "[e_cosh.c] exit __ieee754_cosh 6\n");
	}

    
	if (ix < 0x40862E42) {
		fprintf(stderr, "[e_cosh.c] enter __ieee754_cosh 7\n");
		return half*__ieee754_exp(fabs(x));
		// fprintf(stderr, "[e_cosh.c] exit __ieee754_cosh 7\n");
	}

    
	if (ix < 0x40862E42)  return half*__ieee754_exp(fabs(x));

    
	lx = *( (((*(unsigned*)&one)>>29)) + (unsigned*)&x);
	if (ix<0x408633CE || 
	      (ix==0x408633ce)&&(lx<=(unsigned)0x8fb9f87d)) {
	    fprintf(stderr, "[e_cosh.c] enter __ieee754_cosh 8\n");
	    w = __ieee754_exp(half*fabs(x));
	    t = half*w;
	    return t*w;
	    // fprintf(stderr, "[e_cosh.c] exit __ieee754_cosh 8\n");
	}

    
	fprintf(stderr, "[e_cosh.c] enter __ieee754_cosh 9\n");
	return huge*huge;
	// fprintf(stderr, "[e_cosh.c] exit __ieee754_cosh 9\n");
}

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
	fprintf(stderr, "\n");

    if (argc != 2) {
		fprintf(stderr, "[e_cosh.c] enter main 2\n");
        printf("Usage: %s <arg>\n", argv[0]);
        return 1;
        // fprintf(stderr, "[e_cosh.c] exit main 2\n");
    }

	fprintf(stderr, "[e_cosh.c] enter main 3\n");
    double a0 = atof(argv[1]);

    double r = __ieee754_cosh(a0);
    printf("%f\n", r);

    return 0;
	// fprintf(stderr, "[e_cosh.c] exit main 3\n");
}
// Total cost: 0.001752
// Total split cost: 0.000000, input tokens: 0, output tokens: 0, cache read tokens: 0, cache write tokens: 0, split chunks: [(0, 79)]
// Total instrumented cost: 0.001752, input tokens: 2611, output tokens: 952, cache read tokens: 0, cache write tokens: 0
