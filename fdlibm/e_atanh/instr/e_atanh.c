





#include "fdlibm.h"
#include <stdio.h>

#ifdef __STDC__
static const double one = 1.0, huge = 1e300;
#else
static double one = 1.0, huge = 1e300;
#endif

static double zero = 0.0;

#ifdef __STDC__
	double __ieee754_atanh(double x)
#else
	double __ieee754_atanh(x)
	double x;
#endif
{
	fprintf(stderr, "\n");
	double t;
	int hx,ix;
	unsigned lx;
	hx = __HI(x);		
	lx = __LO(x);		
	ix = hx&0x7fffffff;
	if ((ix|((lx|(-lx))>>31))>0x3ff00000) 
	{
	    fprintf(stderr, "[e_atanh.c] enter __ieee754_atanh 2\n");
	    return (x-x)/(x-x);
	    // fprintf(stderr, "[e_atanh.c] exit __ieee754_atanh 2\n");
	}
	if(ix==0x3ff00000) 
	{
	    fprintf(stderr, "[e_atanh.c] enter __ieee754_atanh 3\n");
	    return x/zero;
	    // fprintf(stderr, "[e_atanh.c] exit __ieee754_atanh 3\n");
	}
	if(ix<0x3e300000&&(huge+x)>zero)
	{
	    fprintf(stderr, "[e_atanh.c] enter __ieee754_atanh 4\n");
	    return x;
	    // fprintf(stderr, "[e_atanh.c] exit __ieee754_atanh 4\n");
	}
	fprintf(stderr, "[e_atanh.c] enter __ieee754_atanh 5\n");
	__HI(x) = ix;		
	if(ix<0x3fe00000) {		
	    fprintf(stderr, "[e_atanh.c] enter __ieee754_atanh 6\n");
	    t = x+x;
	    t = 0.5*log1p(t+t*x/(one-x));
	    // fprintf(stderr, "[e_atanh.c] exit __ieee754_atanh 6\n");
	} else 
	{
	    fprintf(stderr, "[e_atanh.c] enter __ieee754_atanh 7\n");
	    t = 0.5*log1p((x+x)/(one-x));
	    // fprintf(stderr, "[e_atanh.c] exit __ieee754_atanh 7\n");
	}
	if(hx>=0)
	{
	    fprintf(stderr, "[e_atanh.c] enter __ieee754_atanh 8\n");
	    return t;
	    // fprintf(stderr, "[e_atanh.c] exit __ieee754_atanh 8\n");
	} else 
	{
	    fprintf(stderr, "[e_atanh.c] enter __ieee754_atanh 9\n");
	    return -t;
	    // fprintf(stderr, "[e_atanh.c] exit __ieee754_atanh 9\n");
	}
	// fprintf(stderr, "[e_atanh.c] exit __ieee754_atanh 5\n");
}

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
    fprintf(stderr, "\n");
    if (argc != 2) {
        fprintf(stderr, "[e_atanh.c] enter main 2\n");
        printf("Usage: %s <arg>\n", argv[0]);
        return 1;
        // fprintf(stderr, "[e_atanh.c] exit main 2\n");
    }
    fprintf(stderr, "[e_atanh.c] enter main 3\n");
    double a0 = atof(argv[1]);
    // fprintf(stderr, "[e_atanh.c] exit main 3\n");

    fprintf(stderr, "[e_atanh.c] enter main 4\n");
    double r = __ieee754_atanh(a0);
    printf("%f\n", r);
    // fprintf(stderr, "[e_atanh.c] exit main 4\n");

    fprintf(stderr, "[e_atanh.c] enter main 5\n");
    return 0;
    // fprintf(stderr, "[e_atanh.c] exit main 5\n");
}
// Total cost: 0.003768
// Total split cost: 0.000000, input tokens: 0, output tokens: 0, cache read tokens: 0, cache write tokens: 0, split chunks: [(0, 60)]
// Total instrumented cost: 0.003768, input tokens: 6084, output tokens: 1932, cache read tokens: 0, cache write tokens: 0
