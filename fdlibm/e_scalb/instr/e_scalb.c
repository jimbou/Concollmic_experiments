#include <stdio.h>
#include "fdlibm.h"

#ifdef _SCALB_INT
#ifdef __STDC__
	double __ieee754_scalb(double x, int fn)
#else
	double __ieee754_scalb(x,fn)
	double x; int fn;
#endif
#else
#ifdef __STDC__
	double __ieee754_scalb(double x, double fn)
#else
	double __ieee754_scalb(x,fn)
	double x, fn;
#endif
#endif
{
#ifdef _SCALB_INT
	fprintf(stderr, "[e_scalb.c] enter __ieee754_scalb 1\n");
	return scalbn(x,fn);
	// fprintf(stderr, "[e_scalb.c] exit __ieee754_scalb 1\n");
#else
	fprintf(stderr, "\n");
	if (isnan(x)||isnan(fn)) {
		fprintf(stderr, "[e_scalb.c] enter __ieee754_scalb 3\n");
		return x*fn;
		// fprintf(stderr, "[e_scalb.c] exit __ieee754_scalb 3\n");
	}
	if (!finite(fn)) {
		fprintf(stderr, "[e_scalb.c] enter __ieee754_scalb 4\n");
		if(fn>0.0) {
			fprintf(stderr, "[e_scalb.c] enter __ieee754_scalb 5\n");
			return x*fn;
			// fprintf(stderr, "[e_scalb.c] exit __ieee754_scalb 5\n");
		} else {
			fprintf(stderr, "[e_scalb.c] enter __ieee754_scalb 6\n");
			return x/(-fn);
			// fprintf(stderr, "[e_scalb.c] exit __ieee754_scalb 6\n");
		}
		// fprintf(stderr, "[e_scalb.c] exit __ieee754_scalb 4\n");
	}
	if (rint(fn)!=fn) {
		fprintf(stderr, "[e_scalb.c] enter __ieee754_scalb 7\n");
		return (fn-fn)/(fn-fn);
		// fprintf(stderr, "[e_scalb.c] exit __ieee754_scalb 7\n");
	}
	if ( fn > 65000.0) {
		fprintf(stderr, "[e_scalb.c] enter __ieee754_scalb 8\n");
		return scalbn(x, 65000);
		// fprintf(stderr, "[e_scalb.c] exit __ieee754_scalb 8\n");
	}
	if (-fn > 65000.0) {
		fprintf(stderr, "[e_scalb.c] enter __ieee754_scalb 9\n");
		return scalbn(x,-65000);
		// fprintf(stderr, "[e_scalb.c] exit __ieee754_scalb 9\n");
	}
	fprintf(stderr, "[e_scalb.c] enter __ieee754_scalb 10\n");
	return scalbn(x,(int)fn);
	// fprintf(stderr, "[e_scalb.c] exit __ieee754_scalb 10\n");
#endif
}

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
	fprintf(stderr, "\n");
    if (argc != 3) {
		fprintf(stderr, "[e_scalb.c] enter main 2\n");
        printf("Usage: %s <arg> <arg>\n", argv[0]);
        return 1;
		// fprintf(stderr, "[e_scalb.c] exit main 2\n");
    }

	fprintf(stderr, "[e_scalb.c] enter main 3\n");
    double a0 = atof(argv[1]);
    double a1 = atof(argv[2]);

    double r = __ieee754_scalb(a0, a1);
    printf("%f\n", r);

    return 0;
	// fprintf(stderr, "[e_scalb.c] exit main 3\n");
}
// Total cost: 0.001548
// Total split cost: 0.000000, input tokens: 0, output tokens: 0, cache read tokens: 0, cache write tokens: 0, split chunks: [(0, 57)]
// Total instrumented cost: 0.001548, input tokens: 2500, output tokens: 794, cache read tokens: 0, cache write tokens: 0
