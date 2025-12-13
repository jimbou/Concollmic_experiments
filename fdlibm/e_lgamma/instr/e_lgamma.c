





#include "fdlibm.h"
#include <stdio.h>

extern int signgam;

#ifdef __STDC__
	double __ieee754_lgamma(double x)
#else
	double __ieee754_lgamma(x)
	double x;
#endif
{
	fprintf(stderr, "[e_lgamma.c] enter __ieee754_lgamma 1\n");
	return __ieee754_lgamma_r(x,&signgam);
	// fprintf(stderr, "[e_lgamma.c] exit __ieee754_lgamma 1\n");
}

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
	fprintf(stderr, "[e_lgamma.c] enter main 1\n");
    if (argc != 2) {
		fprintf(stderr, "[e_lgamma.c] enter main 2\n");
        printf("Usage: %s <arg>\n", argv[0]);
        return 1;
		// fprintf(stderr, "[e_lgamma.c] exit main 2\n");
    }
	// fprintf(stderr, "[e_lgamma.c] exit main 1\n");

	fprintf(stderr, "[e_lgamma.c] enter main 3\n");
    double a0 = atof(argv[1]);

    double r = __ieee754_lgamma(a0);
    printf("%f\n", r);

    return 0;
	// fprintf(stderr, "[e_lgamma.c] exit main 3\n");
}
// Total cost: 0.000998
// Total split cost: 0.000000, input tokens: 0, output tokens: 0, cache read tokens: 0, cache write tokens: 0, split chunks: [(0, 37)]
// Total instrumented cost: 0.000998, input tokens: 2310, output tokens: 340, cache read tokens: 0, cache write tokens: 0
