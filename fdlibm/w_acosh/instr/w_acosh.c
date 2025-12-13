#include <stdio.h>
#include "fdlibm.h"

#ifdef __STDC__
	double acosh(double x)		
#else
	double acosh(x)			
	double x;
#endif
{
#ifdef _IEEE_LIBM
	fprintf(stderr, "[w_acosh.c] enter acosh 1\n");
	return __ieee754_acosh(x);
	// fprintf(stderr, "[w_acosh.c] exit acosh 1\n");
#else
	fprintf(stderr, "[w_acosh.c] enter acosh 2\n");
	double z;
	z = __ieee754_acosh(x);
	// fprintf(stderr, "[w_acosh.c] exit acosh 2\n");
	if(_LIB_VERSION == _IEEE_ || isnan(x)) {
		fprintf(stderr, "[w_acosh.c] enter acosh 3\n");
		return z;
		// fprintf(stderr, "[w_acosh.c] exit acosh 3\n");
	}
	if(x<1.0) {
		fprintf(stderr, "[w_acosh.c] enter acosh 4\n");
	        return __kernel_standard(x,x,29); 
		// fprintf(stderr, "[w_acosh.c] exit acosh 4\n");
	} else {
		fprintf(stderr, "[w_acosh.c] enter acosh 5\n");
	    return z;
		// fprintf(stderr, "[w_acosh.c] exit acosh 5\n");
	}
#endif
}

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
	fprintf(stderr, "\n");
    if (argc != 2) {
		fprintf(stderr, "[w_acosh.c] enter main 2\n");
        printf("Usage: %s <arg>\n", argv[0]);
        return 1;
		// fprintf(stderr, "[w_acosh.c] exit main 2\n");
    }

	fprintf(stderr, "[w_acosh.c] enter main 3\n");
    double a0 = atof(argv[1]);

    double r = acosh(a0);
    printf("%f\n", r);

    return 0;
	// fprintf(stderr, "[w_acosh.c] exit main 3\n");
}
// Total cost: 0.001110
// Total split cost: 0.000000, input tokens: 0, output tokens: 0, cache read tokens: 0, cache write tokens: 0, split chunks: [(0, 45)]
// Total instrumented cost: 0.001110, input tokens: 2360, output tokens: 430, cache read tokens: 0, cache write tokens: 0
