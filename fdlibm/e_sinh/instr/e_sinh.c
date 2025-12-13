





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
		fprintf(stderr, "[e_sinh.c] enter __ieee754_sinh 2\n");
		return x+x;
		// fprintf(stderr, "[e_sinh.c] exit __ieee754_sinh 2\n");
	}

	h = 0.5;
	if (jx<0) {
		fprintf(stderr, "[e_sinh.c] enter __ieee754_sinh 3\n");
		h = -h;
		// fprintf(stderr, "[e_sinh.c] exit __ieee754_sinh 3\n");
	}
    
	if (ix < 0x40360000) {		
		fprintf(stderr, "\n");
	    if (ix<0x3e300000) {		
			fprintf(stderr, "[e_sinh.c] enter __ieee754_sinh 5\n");
			if(shuge+x>one) {
				fprintf(stderr, "[e_sinh.c] enter __ieee754_sinh 6\n");
				return x;
				// fprintf(stderr, "[e_sinh.c] exit __ieee754_sinh 6\n");
			}
			// fprintf(stderr, "[e_sinh.c] exit __ieee754_sinh 5\n");
		}
	    t = expm1(fabs(x));
	    if(ix<0x3ff00000) {
			fprintf(stderr, "[e_sinh.c] enter __ieee754_sinh 7\n");
			return h*(2.0*t-t*t/(t+one));
			// fprintf(stderr, "[e_sinh.c] exit __ieee754_sinh 7\n");
		}
		fprintf(stderr, "[e_sinh.c] enter __ieee754_sinh 8\n");
	    return h*(t+t/(t+one));
		// fprintf(stderr, "[e_sinh.c] exit __ieee754_sinh 8\n");
	}

    
	if (ix < 0x40862E42) {
		fprintf(stderr, "[e_sinh.c] enter __ieee754_sinh 9\n");
		return h*__ieee754_exp(fabs(x));
		// fprintf(stderr, "[e_sinh.c] exit __ieee754_sinh 9\n");
	}

    
	lx = *( (((*(unsigned*)&one)>>29)) + (unsigned*)&x);
	if (ix<0x408633CE || (ix==0x408633ce)&&(lx<=(unsigned)0x8fb9f87d)) {
		fprintf(stderr, "[e_sinh.c] enter __ieee754_sinh 10\n");
	    w = __ieee754_exp(0.5*fabs(x));
	    t = h*w;
	    return t*w;
		// fprintf(stderr, "[e_sinh.c] exit __ieee754_sinh 10\n");
	}

    
	fprintf(stderr, "[e_sinh.c] enter __ieee754_sinh 11\n");
	return x*shuge;
	// fprintf(stderr, "[e_sinh.c] exit __ieee754_sinh 11\n");
}

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
	fprintf(stderr, "\n");

    if (argc != 2) {
		fprintf(stderr, "[e_sinh.c] enter main 2\n");
        printf("Usage: %s <arg>\n", argv[0]);
        return 1;
		// fprintf(stderr, "[e_sinh.c] exit main 2\n");
    }

	fprintf(stderr, "[e_sinh.c] enter main 3\n");
    double a0 = atof(argv[1]);

    double r = __ieee754_sinh(a0);
    printf("%f\n", r);

    return 0;
	// fprintf(stderr, "[e_sinh.c] exit main 3\n");
}
// Total cost: 0.001816
// Total split cost: 0.000000, input tokens: 0, output tokens: 0, cache read tokens: 0, cache write tokens: 0, split chunks: [(0, 75)]
// Total instrumented cost: 0.001816, input tokens: 2620, output tokens: 1008, cache read tokens: 0, cache write tokens: 0
