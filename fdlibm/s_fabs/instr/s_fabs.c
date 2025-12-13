#include <stdio.h>
#include <stdlib.h>
#include "fdlibm.h"

#ifdef __STDC__
	double fabs(double x)
#else
	double fabs(x)
	double x;
#endif
{
	fprintf(stderr, "[s_fabs.c] enter fabs 1\n");
	__HI(x) &= 0x7fffffff;
        return x;
        // fprintf(stderr, "[s_fabs.c] exit fabs 1\n");
}

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
    fprintf(stderr, "\n");
    if (argc != 2) {
        fprintf(stderr, "[s_fabs.c] enter main 2\n");
        printf("Usage: %s <arg>\n", argv[0]);
        return 1;
        // fprintf(stderr, "[s_fabs.c] exit main 2\n");
    }

    fprintf(stderr, "[s_fabs.c] enter main 3\n");
    double a0 = atof(argv[1]);

    double r = fabs(a0);
    printf("%f\n", r);

    return 0;
    // fprintf(stderr, "[s_fabs.c] exit main 3\n");
}
// Total cost: 0.000885
// Total split cost: 0.000000, input tokens: 0, output tokens: 0, cache read tokens: 0, cache write tokens: 0, split chunks: [(0, 36)]
// Total instrumented cost: 0.000885, input tokens: 2297, output tokens: 241, cache read tokens: 0, cache write tokens: 0
