





#include "fdlibm.h"
#include <stdio.h>

#ifdef __STDC__
static const double zero = 0.0;
#else
static double zero = 0.0;
#endif


#ifdef __STDC__
	double __ieee754_remainder(double x, double p)
#else
	double __ieee754_remainder(x,p)
	double x,p;
#endif
{
	fprintf(stderr, "[e_remainder.c] enter __ieee754_remainder 1\n");
	int hx,hp;
	unsigned sx,lx,lp;
	double p_half;

	hx = __HI(x);		
	lx = __LO(x);		
	hp = __HI(p);		
	lp = __LO(p);		
	sx = hx&0x80000000;
	hp &= 0x7fffffff;
	hx &= 0x7fffffff;
	// fprintf(stderr, "[e_remainder.c] exit __ieee754_remainder 1\n");

    
	if((hp|lp)==0) {
		fprintf(stderr, "[e_remainder.c] enter __ieee754_remainder 2\n");
		return (x*p)/(x*p);
		// fprintf(stderr, "[e_remainder.c] exit __ieee754_remainder 2\n");
	}
	if((hx>=0x7ff00000)||			
	  ((hp>=0x7ff00000)&&			
	  (((hp-0x7ff00000)|lp)!=0))) {
		fprintf(stderr, "[e_remainder.c] enter __ieee754_remainder 3\n");
	    return (x*p)/(x*p);
		// fprintf(stderr, "[e_remainder.c] exit __ieee754_remainder 3\n");
	}


	if (hp<=0x7fdfffff) {
		fprintf(stderr, "[e_remainder.c] enter __ieee754_remainder 4\n");
		x = __ieee754_fmod(x,p+p);
		// fprintf(stderr, "[e_remainder.c] exit __ieee754_remainder 4\n");
	}
	if (((hx-hp)|(lx-lp))==0) {
		fprintf(stderr, "[e_remainder.c] enter __ieee754_remainder 5\n");
		return zero*x;
		// fprintf(stderr, "[e_remainder.c] exit __ieee754_remainder 5\n");
	}
	fprintf(stderr, "[e_remainder.c] enter __ieee754_remainder 6\n");
	x  = fabs(x);
	p  = fabs(p);
	// fprintf(stderr, "[e_remainder.c] exit __ieee754_remainder 6\n");
	if (hp<0x00200000) {
		fprintf(stderr, "[e_remainder.c] enter __ieee754_remainder 7\n");
	    if(x+x>p) {
			fprintf(stderr, "[e_remainder.c] enter __ieee754_remainder 8\n");
		x-=p;
		if(x+x>=p) {
			fprintf(stderr, "[e_remainder.c] enter __ieee754_remainder 9\n");
			x -= p;
			// fprintf(stderr, "[e_remainder.c] exit __ieee754_remainder 9\n");
		}
		// fprintf(stderr, "[e_remainder.c] exit __ieee754_remainder 8\n");
	    }
		// fprintf(stderr, "[e_remainder.c] exit __ieee754_remainder 7\n");
	} else {
		fprintf(stderr, "[e_remainder.c] enter __ieee754_remainder 10\n");
	    p_half = 0.5*p;
	    if(x>p_half) {
			fprintf(stderr, "[e_remainder.c] enter __ieee754_remainder 11\n");
		x-=p;
		if(x>=p_half) {
			fprintf(stderr, "[e_remainder.c] enter __ieee754_remainder 12\n");
			x -= p;
			// fprintf(stderr, "[e_remainder.c] exit __ieee754_remainder 12\n");
		}
		// fprintf(stderr, "[e_remainder.c] exit __ieee754_remainder 11\n");
	    }
		// fprintf(stderr, "[e_remainder.c] exit __ieee754_remainder 10\n");
	}
	fprintf(stderr, "[e_remainder.c] enter __ieee754_remainder 13\n");
	__HI(x) ^= sx;
	return x;
	// fprintf(stderr, "[e_remainder.c] exit __ieee754_remainder 13\n");
}

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
	fprintf(stderr, "\n");

    if (argc != 3) {
		fprintf(stderr, "[e_remainder.c] enter main 2\n");
        printf("Usage: %s <arg> <arg>\n", argv[0]);
        return 1;
		// fprintf(stderr, "[e_remainder.c] exit main 2\n");
    }

	fprintf(stderr, "[e_remainder.c] enter main 3\n");
    double a0 = atof(argv[1]);
    double a1 = atof(argv[2]);

    double r = __ieee754_remainder(a0, a1);
    printf("%f\n", r);

    return 0;
	// fprintf(stderr, "[e_remainder.c] exit main 3\n");
}
// Total cost: 0.001925
// Total split cost: 0.000000, input tokens: 0, output tokens: 0, cache read tokens: 0, cache write tokens: 0, split chunks: [(0, 80)]
// Total instrumented cost: 0.001925, input tokens: 2653, output tokens: 1099, cache read tokens: 0, cache write tokens: 0
