





#include "fdlibm.h"
#include <stdio.h>
#include <stdlib.h>
#ifdef __STDC__
static const double
#else
static double
#endif
two54      =  1.80143985094819840000e+16, 
ivln10     =  4.34294481903251816668e-01, 
log10_2hi  =  3.01029995663611771306e-01, 
log10_2lo  =  3.69423907715893078616e-13; 

static double zero   =  0.0;

#ifdef __STDC__
	double __ieee754_log10(double x)
#else
	double __ieee754_log10(x)
	double x;
#endif
{
	fprintf(stderr, "[e_log10.c] enter __ieee754_log10 1\n");
	double y,z;
	int i,k,hx;
	unsigned lx;

	hx = __HI(x);	
	lx = __LO(x);	
	// fprintf(stderr, "[e_log10.c] exit __ieee754_log10 1\n");

        k=0;
        if (hx < 0x00100000) {                  
            fprintf(stderr, "[e_log10.c] enter __ieee754_log10 2\n");
            if (((hx&0x7fffffff)|lx)==0)
            {
                fprintf(stderr, "[e_log10.c] enter __ieee754_log10 3\n");
                return -two54/zero;             
                // fprintf(stderr, "[e_log10.c] exit __ieee754_log10 3\n");
            }
            if (hx<0) 
            {
                fprintf(stderr, "[e_log10.c] enter __ieee754_log10 4\n");
                return (x-x)/zero;        
                // fprintf(stderr, "[e_log10.c] exit __ieee754_log10 4\n");
            }
            fprintf(stderr, "[e_log10.c] enter __ieee754_log10 5\n");
            k -= 54; x *= two54; 
            hx = __HI(x);                
            // fprintf(stderr, "[e_log10.c] exit __ieee754_log10 5\n");
        }
        // fprintf(stderr, "[e_log10.c] exit __ieee754_log10 2\n");
	
	fprintf(stderr, "[e_log10.c] enter __ieee754_log10 6\n");
	if (hx >= 0x7ff00000) 
	{
		fprintf(stderr, "[e_log10.c] enter __ieee754_log10 7\n");
		return x+x;
		// fprintf(stderr, "[e_log10.c] exit __ieee754_log10 7\n");
	}
	// fprintf(stderr, "[e_log10.c] exit __ieee754_log10 6\n");
	
	fprintf(stderr, "[e_log10.c] enter __ieee754_log10 8\n");
	k += (hx>>20)-1023;
	i  = ((unsigned)k&0x80000000)>>31;
        hx = (hx&0x000fffff)|((0x3ff-i)<<20);
        y  = (double)(k+i);
        __HI(x) = hx;
	z  = y*log10_2lo + ivln10*__ieee754_log(x);
	return  z+y*log10_2hi;
	// fprintf(stderr, "[e_log10.c] exit __ieee754_log10 8\n");
}



int main(int argc, char** argv) {
	fprintf(stderr, "[e_log10.c] enter main 1\n");

    if (argc != 2) {
        fprintf(stderr, "[e_log10.c] enter main 2\n");
        printf("Usage: %s <arg>\n", argv[0]);
        return 1;
        // fprintf(stderr, "[e_log10.c] exit main 2\n");
    }
    // fprintf(stderr, "[e_log10.c] exit main 1\n");

    fprintf(stderr, "[e_log10.c] enter main 3\n");
    double a0 = atof(argv[1]);

    double r = __ieee754_log10(a0);
    printf("%f\n", r);

    return 0;
    // fprintf(stderr, "[e_log10.c] exit main 3\n");
}
// Total cost: 0.001761
// Total split cost: 0.000000, input tokens: 0, output tokens: 0, cache read tokens: 0, cache write tokens: 0, split chunks: [(0, 69)]
// Total instrumented cost: 0.001761, input tokens: 2640, output tokens: 953, cache read tokens: 0, cache write tokens: 0
