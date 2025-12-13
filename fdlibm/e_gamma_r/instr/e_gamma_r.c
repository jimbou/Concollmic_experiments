#include <stdio.h>

#include "fdlibm.h"

#ifdef __STDC__
	double __ieee754_gamma_r(double x, int *signgamp)
#else
	double __ieee754_gamma_r(x,signgamp)
	double x; int *signgamp;
#endif
{
	fprintf(stderr, "[e_gamma_r.c] enter __ieee754_gamma_r 1\n");
	return __ieee754_lgamma_r(x,signgamp);
	// fprintf(stderr, "[e_gamma_r.c] exit __ieee754_gamma_r 1\n");
}


#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
	fprintf(stderr, "\n");
	if (argc != 2) {
		fprintf(stderr, "[e_gamma_r.c] enter main 2\n");
		printf("Usage: %s <x>\n", argv[0]);
		return 1;
		// fprintf(stderr, "[e_gamma_r.c] exit main 2\n");
	}

	fprintf(stderr, "[e_gamma_r.c] enter main 3\n");
	double x = atof(argv[1]);
	int sign = 0;

	double r = __ieee754_gamma_r(x, &sign);

	printf("result = %f\n", r);
	printf("signgamp = %d\n", sign);

	return 0;
	// fprintf(stderr, "[e_gamma_r.c] exit main 3\n");
}
// Total cost: 0.001047
// Total split cost: 0.000000, input tokens: 0, output tokens: 0, cache read tokens: 0, cache write tokens: 0, split chunks: [(0, 38)]
// Total instrumented cost: 0.001047, input tokens: 2351, output tokens: 375, cache read tokens: 0, cache write tokens: 0
