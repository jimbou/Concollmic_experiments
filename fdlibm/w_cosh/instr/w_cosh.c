





#include "fdlibm.h"
#include <stdio.h>

#ifdef __STDC__
	double cosh(double x)		
#else
	double cosh(x)			
	double x;
#endif
{
#ifdef _IEEE_LIBM
	fprintf(stderr, "[w_cosh.c] enter cosh 1\n");
	return __ieee754_cosh(x);
	// fprintf(stderr, "[w_cosh.c] exit cosh 1\n");
#else
	fprintf(stderr, "[w_cosh.c] enter cosh 2\n");
	double z;
	z = __ieee754_cosh(x);
	if(_LIB_VERSION == _IEEE_ || isnan(x)) {
		fprintf(stderr, "[w_cosh.c] enter cosh 3\n");
		return z;
		// fprintf(stderr, "[w_cosh.c] exit cosh 3\n");
	}
	if(fabs(x)>7.10475860073943863426e+02) {	
		fprintf(stderr, "[w_cosh.c] enter cosh 4\n");
	        return __kernel_standard(x,x,5); 
		// fprintf(stderr, "[w_cosh.c] exit cosh 4\n");
	} else {
		fprintf(stderr, "[w_cosh.c] enter cosh 5\n");
	    return z;
		// fprintf(stderr, "[w_cosh.c] exit cosh 5\n");
	}
	// fprintf(stderr, "[w_cosh.c] exit cosh 2\n");
#endif
}

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
	fprintf(stderr, "\n");

    if (argc != 2) {
		fprintf(stderr, "[w_cosh.c] enter main 2\n");
        printf("Usage: %s <arg>\n", argv[0]);
        return 1;
		// fprintf(stderr, "[w_cosh.c] exit main 2\n");
    }

	fprintf(stderr, "[w_cosh.c] enter main 3\n");
    double a0 = atof(argv[1]);

    double r = cosh(a0);
    printf("%f\n", r);

    return 0;
	// fprintf(stderr, "[w_cosh.c] exit main 3\n");
}
// Total cost: 0.001129
// Total split cost: 0.000000, input tokens: 0, output tokens: 0, cache read tokens: 0, cache write tokens: 0, split chunks: [(0, 45)]
// Total instrumented cost: 0.001129, input tokens: 2372, output tokens: 444, cache read tokens: 0, cache write tokens: 0
