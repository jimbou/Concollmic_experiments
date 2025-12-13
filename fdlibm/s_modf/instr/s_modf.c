





#include "fdlibm.h"
#include <stdio.h>
#ifdef __STDC__
static const double one = 1.0;
#else
static double one = 1.0;
#endif

#ifdef __STDC__
	double modf(double x, double *iptr)
#else
	double modf(x, iptr)
	double x,*iptr;
#endif
{
	fprintf(stderr, "[s_modf.c] enter modf 1\n");
	int i0,i1,j0;
	unsigned i;
	i0 =  __HI(x);		
	i1 =  __LO(x);		
	j0 = ((i0>>20)&0x7ff)-0x3ff;	
	// fprintf(stderr, "[s_modf.c] exit modf 1\n");
	if(j0<20) {			
	    fprintf(stderr, "\n");
	    if(j0<0) {			
		fprintf(stderr, "[s_modf.c] enter modf 3\n");
		__HIp(iptr) = i0&0x80000000;
		__LOp(iptr) = 0;		
		return x;
		// fprintf(stderr, "[s_modf.c] exit modf 3\n");
	    } else {
		fprintf(stderr, "[s_modf.c] enter modf 4\n");
		i = (0x000fffff)>>j0;
		// fprintf(stderr, "[s_modf.c] exit modf 4\n");
		if(((i0&i)|i1)==0) {		
		    fprintf(stderr, "[s_modf.c] enter modf 5\n");
		    *iptr = x;
		    __HI(x) &= 0x80000000;
		    __LO(x)  = 0;	
		    return x;
		    // fprintf(stderr, "[s_modf.c] exit modf 5\n");
		} else {
		    fprintf(stderr, "[s_modf.c] enter modf 6\n");
		    __HIp(iptr) = i0&(~i);
		    __LOp(iptr) = 0;
		    return x - *iptr;
		    // fprintf(stderr, "[s_modf.c] exit modf 6\n");
		}
	    }
	} else if (j0>51) {		
	    fprintf(stderr, "[s_modf.c] enter modf 7\n");
	    *iptr = x*one;
	    __HI(x) &= 0x80000000;
	    __LO(x)  = 0;	
	    return x;
	    // fprintf(stderr, "[s_modf.c] exit modf 7\n");
	} else {			
	    fprintf(stderr, "[s_modf.c] enter modf 8\n");
	    i = ((unsigned)(0xffffffff))>>(j0-20);
	    // fprintf(stderr, "[s_modf.c] exit modf 8\n");
	    if((i1&i)==0) { 		
		fprintf(stderr, "[s_modf.c] enter modf 9\n");
		*iptr = x;
		__HI(x) &= 0x80000000;
		__LO(x)  = 0;	
		return x;
		// fprintf(stderr, "[s_modf.c] exit modf 9\n");
	    } else {
		fprintf(stderr, "[s_modf.c] enter modf 10\n");
		__HIp(iptr) = i0;
		__LOp(iptr) = i1&(~i);
		return x - *iptr;
		// fprintf(stderr, "[s_modf.c] exit modf 10\n");
	    }
	}
}

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
    fprintf(stderr, "\n");
    if (argc != 2) {
        fprintf(stderr, "[s_modf.c] enter main 2\n");
        printf("Usage: %s <x>\n", argv[0]);
        return 1;
        // fprintf(stderr, "[s_modf.c] exit main 2\n");
    }

    fprintf(stderr, "[s_modf.c] enter main 3\n");
    double x = atof(argv[1]);
    double iptr;
    // fprintf(stderr, "[s_modf.c] exit main 3\n");

    fprintf(stderr, "[s_modf.c] enter main 4\n");
    double frac = modf(x, &iptr);
    // fprintf(stderr, "[s_modf.c] exit main 4\n");

    fprintf(stderr, "[s_modf.c] enter main 5\n");
    printf("fraction = %f\n", frac);
    printf("integer  = %f\n", iptr);
    return 0;
    // fprintf(stderr, "[s_modf.c] exit main 5\n");
}
// Total cost: 0.001823
// Total split cost: 0.000000, input tokens: 0, output tokens: 0, cache read tokens: 0, cache write tokens: 0, split chunks: [(0, 82)]
// Total instrumented cost: 0.001823, input tokens: 2722, output tokens: 989, cache read tokens: 0, cache write tokens: 0
