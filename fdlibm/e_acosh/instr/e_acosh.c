





#include "fdlibm.h"
#include <stdio.h>

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
	fprintf(stderr, "[e_acosh.c] enter __ieee754_acosh 1\n");
	double t;
	int hx;
	hx = __HI(x);
	// fprintf(stderr, "[e_acosh.c] exit __ieee754_acosh 1\n");
	if(hx<0x3ff00000) {		
	    fprintf(stderr, "[e_acosh.c] enter __ieee754_acosh 2\n");
	    return (x-x)/(x-x);
	    // fprintf(stderr, "[e_acosh.c] exit __ieee754_acosh 2\n");
	} else if(hx >=0x41b00000) {	
	    fprintf(stderr, "\n");
	    if(hx >=0x7ff00000) {	
	        fprintf(stderr, "[e_acosh.c] enter __ieee754_acosh 4\n");
	        return x+x;
	        // fprintf(stderr, "[e_acosh.c] exit __ieee754_acosh 4\n");
	    } else {
	        fprintf(stderr, "[e_acosh.c] enter __ieee754_acosh 5\n");
		return __ieee754_log(x)+ln2;
		// fprintf(stderr, "[e_acosh.c] exit __ieee754_acosh 5\n");
	    }
	} else if(((hx-0x3ff00000)|__LO(x))==0) {
	    fprintf(stderr, "[e_acosh.c] enter __ieee754_acosh 6\n");
	    return 0.0;			
	    // fprintf(stderr, "[e_acosh.c] exit __ieee754_acosh 6\n");
	} else if (hx > 0x40000000) {	
	    fprintf(stderr, "[e_acosh.c] enter __ieee754_acosh 7\n");
	    t=x*x;
	    return __ieee754_log(2.0*x-one/(x+sqrt(t-one)));
	    // fprintf(stderr, "[e_acosh.c] exit __ieee754_acosh 7\n");
	} else {			
	    fprintf(stderr, "[e_acosh.c] enter __ieee754_acosh 8\n");
	    t = x-one;
	    return log1p(t+sqrt(2.0*t+t*t));
	    // fprintf(stderr, "[e_acosh.c] exit __ieee754_acosh 8\n");
	}
}

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
    fprintf(stderr, "\n");
    if (argc != 2) {
        fprintf(stderr, "[e_acosh.c] enter main 2\n");
        printf("Usage: %s <arg>\n", argv[0]);
        return 1;
        // fprintf(stderr, "[e_acosh.c] exit main 2\n");
    }

    fprintf(stderr, "[e_acosh.c] enter main 3\n");
    double a0 = atof(argv[1]);

    double r = __ieee754_acosh(a0);
    printf("%f\n", r);

    return 0;
    // fprintf(stderr, "[e_acosh.c] exit main 3\n");
}
// Total cost: 0.001506
// Total split cost: 0.000000, input tokens: 0, output tokens: 0, cache read tokens: 0, cache write tokens: 0, split chunks: [(0, 61)]
// Total instrumented cost: 0.001506, input tokens: 2527, output tokens: 749, cache read tokens: 0, cache write tokens: 0
