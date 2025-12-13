#include <stdio.h>
#include <stdlib.h>

#include "fdlibm.h"

#ifdef __STDC__
	double sqrt(double x)		
#else
	double sqrt(x)			
	double x;
#endif
{
#ifdef _IEEE_LIBM
	fprintf(stderr, "[w_sqrt.c] enter sqrt 1\n");
	return __ieee754_sqrt(x);
	// fprintf(stderr, "[w_sqrt.c] exit sqrt 1\n");
#else
	fprintf(stderr, "[w_sqrt.c] enter sqrt 2\n");
	double z;
	z = __ieee754_sqrt(x);
	// fprintf(stderr, "[w_sqrt.c] exit sqrt 2\n");
	if(_LIB_VERSION == _IEEE_ || isnan(x)) {
		fprintf(stderr, "[w_sqrt.c] enter sqrt 3\n");
		return z;
		// fprintf(stderr, "[w_sqrt.c] exit sqrt 3\n");
	}
	if(x<0.0) {
		fprintf(stderr, "[w_sqrt.c] enter sqrt 4\n");
		return __kernel_standard(x,x,26); 
		// fprintf(stderr, "[w_sqrt.c] exit sqrt 4\n");
	} else {
		fprintf(stderr, "[w_sqrt.c] enter sqrt 5\n");
		return z;
		// fprintf(stderr, "[w_sqrt.c] exit sqrt 5\n");
	}
#endif
}

int main(int argc, char** argv) {
	fprintf(stderr, "\n");
	if (argc != 2) {
		fprintf(stderr, "[w_sqrt.c] enter main 2\n");
		printf("Usage: %s <arg>\n", argv[0]);
		return 1;
		// fprintf(stderr, "[w_sqrt.c] exit main 2\n");
	}

	fprintf(stderr, "[w_sqrt.c] enter main 3\n");
	double a0 = atof(argv[1]);

	double r = sqrt(a0);
	printf("%f\n", r);

	return 0;
	// fprintf(stderr, "[w_sqrt.c] exit main 3\n");
}
// Total cost: 0.001146
// Total split cost: 0.000000, input tokens: 0, output tokens: 0, cache read tokens: 0, cache write tokens: 0, split chunks: [(0, 45)]
// Total instrumented cost: 0.001146, input tokens: 2357, output tokens: 463, cache read tokens: 0, cache write tokens: 0
