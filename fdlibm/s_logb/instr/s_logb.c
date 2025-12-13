





#include "fdlibm.h"
#include <stdio.h>

#ifdef __STDC__
	double logb(double x)
#else
	double logb(x)
	double x;
#endif
{
	fprintf(stderr, "[s_logb.c] enter logb 1\n");
	int lx,ix;
	ix = (__HI(x))&0x7fffffff;	
	lx = __LO(x);			
	if((ix|lx)==0) {
		fprintf(stderr, "[s_logb.c] enter logb 2\n");
		return -1.0/fabs(x);
		// fprintf(stderr, "[s_logb.c] exit logb 2\n");
	}
	if(ix>=0x7ff00000) {
		fprintf(stderr, "[s_logb.c] enter logb 3\n");
		return x*x;
		// fprintf(stderr, "[s_logb.c] exit logb 3\n");
	}
	if((ix>>=20)==0) {
		fprintf(stderr, "[s_logb.c] enter logb 4\n");
		return -1022.0;
		// fprintf(stderr, "[s_logb.c] exit logb 4\n");
	} else {
		fprintf(stderr, "[s_logb.c] enter logb 5\n");
		return (double) (ix-1023);
		// fprintf(stderr, "[s_logb.c] exit logb 5\n");
	}
	// fprintf(stderr, "[s_logb.c] exit logb 1\n");
}

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
	fprintf(stderr, "\n");
    if (argc != 2) {
		fprintf(stderr, "[s_logb.c] enter main 2\n");
        printf("Usage: %s <arg>\n", argv[0]);
        return 1;
		// fprintf(stderr, "[s_logb.c] exit main 2\n");
    }

	fprintf(stderr, "[s_logb.c] enter main 3\n");
    double a0 = atof(argv[1]);

    double r = logb(a0);
    printf("%f\n", r);

    return 0;
	// fprintf(stderr, "[s_logb.c] exit main 3\n");
}
// Total cost: 0.001140
// Total split cost: 0.000000, input tokens: 0, output tokens: 0, cache read tokens: 0, cache write tokens: 0, split chunks: [(0, 43)]
// Total instrumented cost: 0.001140, input tokens: 2376, output tokens: 453, cache read tokens: 0, cache write tokens: 0
