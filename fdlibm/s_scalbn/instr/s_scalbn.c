#include <stdio.h>

#include "fdlibm.h"

#ifdef __STDC__
static const double
#else
static double
#endif
two54   =  1.80143985094819840000e+16, 
twom54  =  5.55111512312578270212e-17, 
huge   = 1.0e+300,
tiny   = 1.0e-300;

#ifdef __STDC__
	double scalbn (double x, int n)
#else
	double scalbn (x,n)
	double x; int n;
#endif
{
	fprintf(stderr, "\n");
	int  k,hx,lx;
	hx = __HI(x);
	lx = __LO(x);
        k = (hx&0x7ff00000)>>20;		
        if (k==0) {				
            if ((lx|(hx&0x7fffffff))==0) {
		fprintf(stderr, "[s_scalbn.c] enter scalbn 2\n");
		return x;
		// fprintf(stderr, "[s_scalbn.c] exit scalbn 2\n");
	    }
	    fprintf(stderr, "[s_scalbn.c] enter scalbn 3\n");
	    x *= two54; 
	    hx = __HI(x);
	    k = ((hx&0x7ff00000)>>20) - 54; 
            if (n< -50000) {
		fprintf(stderr, "[s_scalbn.c] enter scalbn 4\n");
		return tiny*x;
		// fprintf(stderr, "[s_scalbn.c] exit scalbn 4\n");
	    }
	    // fprintf(stderr, "[s_scalbn.c] exit scalbn 3\n");
        }
        if (k==0x7ff) {
		fprintf(stderr, "[s_scalbn.c] enter scalbn 5\n");
		return x+x;
		// fprintf(stderr, "[s_scalbn.c] exit scalbn 5\n");
	}
        fprintf(stderr, "\n");
        k = k+n; 
        if (k >  0x7fe) {
		fprintf(stderr, "[s_scalbn.c] enter scalbn 7\n");
		return huge*copysign(huge,x);
		// fprintf(stderr, "[s_scalbn.c] exit scalbn 7\n");
	}
        if (k > 0) 				
	    {
		fprintf(stderr, "[s_scalbn.c] enter scalbn 8\n");
		__HI(x) = (hx&0x800fffff)|(k<<20); return x;
		// fprintf(stderr, "[s_scalbn.c] exit scalbn 8\n");
	    }
        if (k <= -54)
            if (n > 50000) 	
		{
			fprintf(stderr, "[s_scalbn.c] enter scalbn 9\n");
			return huge*copysign(huge,x);
			// fprintf(stderr, "[s_scalbn.c] exit scalbn 9\n");
		}
	    else {
		fprintf(stderr, "[s_scalbn.c] enter scalbn 10\n");
		return tiny*copysign(tiny,x);
		// fprintf(stderr, "[s_scalbn.c] exit scalbn 10\n");
	    }
        fprintf(stderr, "[s_scalbn.c] enter scalbn 11\n");
        k += 54;				
        __HI(x) = (hx&0x800fffff)|(k<<20);
        return x*twom54;
        // fprintf(stderr, "[s_scalbn.c] exit scalbn 11\n");
}

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
	fprintf(stderr, "\n");
    if (argc != 3) {
		fprintf(stderr, "[s_scalbn.c] enter main 2\n");
        printf("Usage: %s <arg> <arg>\n", argv[0]);
        return 1;
        // fprintf(stderr, "[s_scalbn.c] exit main 2\n");
    }

    fprintf(stderr, "[s_scalbn.c] enter main 3\n");
    double a0 = atof(argv[1]);
    int a1 = atoi(argv[2]);

    double r = scalbn(a0, a1);
    printf("%f\n", r);

    return 0;
    // fprintf(stderr, "[s_scalbn.c] exit main 3\n");
}
// Total cost: 0.001708
// Total split cost: 0.000000, input tokens: 0, output tokens: 0, cache read tokens: 0, cache write tokens: 0, split chunks: [(0, 68)]
// Total instrumented cost: 0.001708, input tokens: 2672, output tokens: 897, cache read tokens: 0, cache write tokens: 0
