





#include "fdlibm.h"
#include <stdio.h>

#ifdef __STDC__
	double significand(double x)
#else
	double significand(x)
	double x;
#endif
{
	fprintf(stderr, "[s_significand.c] enter significand 1\n");
	return __ieee754_scalb(x,(double) -ilogb(x));
	// fprintf(stderr, "[s_significand.c] exit significand 1\n");
}

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
	fprintf(stderr, "\n");
    if (argc != 2) {
	fprintf(stderr, "[s_significand.c] enter main 2\n");
        printf("Usage: %s <arg>\n", argv[0]);
        return 1;
	// fprintf(stderr, "[s_significand.c] exit main 2\n");
    }

    fprintf(stderr, "[s_significand.c] enter main 3\n");
    double a0 = atof(argv[1]);

    double r = significand(a0);
    printf("%f\n", r);

    return 0;
	// fprintf(stderr, "[s_significand.c] exit main 3\n");
}
// Total cost: 0.001030
// Total split cost: 0.000000, input tokens: 0, output tokens: 0, cache read tokens: 0, cache write tokens: 0, split chunks: [(0, 35)]
// Total instrumented cost: 0.001030, input tokens: 2298, output tokens: 372, cache read tokens: 0, cache write tokens: 0
