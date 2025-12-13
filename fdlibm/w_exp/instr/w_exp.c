





#include "fdlibm.h"
#include <stdio.h>

#ifdef __STDC__
static const double
#else
static double
#endif
o_threshold=  7.09782712893383973096e+02,  
u_threshold= -7.45133219101941108420e+02;  

#ifdef __STDC__
	double exp(double x)		
#else
	double exp(x)			
	double x;
#endif
{
#ifdef _IEEE_LIBM
	fprintf(stderr, "[w_exp.c] enter exp 1\n");
	return __ieee754_exp(x);
	// fprintf(stderr, "[w_exp.c] exit exp 1\n");
#else
	fprintf(stderr, "[w_exp.c] enter exp 2\n");
	double z;
	z = __ieee754_exp(x);
	// fprintf(stderr, "[w_exp.c] exit exp 2\n");
	if(_LIB_VERSION == _IEEE_) {
		fprintf(stderr, "[w_exp.c] enter exp 3\n");
		return z;
		// fprintf(stderr, "[w_exp.c] exit exp 3\n");
	}
	fprintf(stderr, "\n");
	if(finite(x)) {
		fprintf(stderr, "[w_exp.c] enter exp 5\n");
		if(x>o_threshold) {
			fprintf(stderr, "[w_exp.c] enter exp 6\n");
			return __kernel_standard(x,x,6); 
			// fprintf(stderr, "[w_exp.c] exit exp 6\n");
		} else if(x<u_threshold) {
			fprintf(stderr, "[w_exp.c] enter exp 7\n");
			return __kernel_standard(x,x,7); 
			// fprintf(stderr, "[w_exp.c] exit exp 7\n");
		}
		// fprintf(stderr, "[w_exp.c] exit exp 5\n");
	}
	fprintf(stderr, "[w_exp.c] enter exp 8\n");
	return z;
	// fprintf(stderr, "[w_exp.c] exit exp 8\n");
#endif
}

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
	fprintf(stderr, "\n");
	if (argc != 2) {
		fprintf(stderr, "[w_exp.c] enter main 2\n");
		printf("Usage: %s <arg>\n", argv[0]);
		return 1;
		// fprintf(stderr, "[w_exp.c] exit main 2\n");
	}
	fprintf(stderr, "[w_exp.c] enter main 3\n");
	double a0 = atof(argv[1]);
	// fprintf(stderr, "[w_exp.c] exit main 3\n");
	fprintf(stderr, "[w_exp.c] enter main 4\n");
	double r = exp(a0);
	printf("%f\n", r);
	return 0;
	// fprintf(stderr, "[w_exp.c] exit main 4\n");
}
// Total cost: 0.001368
// Total split cost: 0.000000, input tokens: 0, output tokens: 0, cache read tokens: 0, cache write tokens: 0, split chunks: [(0, 56)]
// Total instrumented cost: 0.001368, input tokens: 2428, output tokens: 648, cache read tokens: 0, cache write tokens: 0
