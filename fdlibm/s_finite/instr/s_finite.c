





#include "fdlibm.h"
#include <stdio.h>

#ifdef __STDC__
	int finite(double x)
#else
	int finite(x)
	double x;
#endif
{
	fprintf(stderr, "[s_finite.c] enter finite 1\n");
	int hx; 
	hx = __HI(x);
	return  (unsigned)((hx&0x7fffffff)-0x7ff00000)>>31;
	// fprintf(stderr, "[s_finite.c] exit finite 1\n");
}

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
	fprintf(stderr, "\n");

    if (argc != 2) {
        fprintf(stderr, "[s_finite.c] enter main 2\n");
        printf("Usage: %s <arg>\n", argv[0]);
        return 1;
        // fprintf(stderr, "[s_finite.c] exit main 2\n");
    }

    fprintf(stderr, "[s_finite.c] enter main 3\n");
    double a0 = atof(argv[1]);

    int r = finite(a0);
    printf("%f\n", r);

    return 0;
    // fprintf(stderr, "[s_finite.c] exit main 3\n");
}
// Total cost: 0.000913
// Total split cost: 0.000000, input tokens: 0, output tokens: 0, cache read tokens: 0, cache write tokens: 0, split chunks: [(0, 37)]
// Total instrumented cost: 0.000913, input tokens: 2314, output tokens: 262, cache read tokens: 0, cache write tokens: 0
