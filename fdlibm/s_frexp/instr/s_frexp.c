





#include "fdlibm.h"
#include <stdio.h>

#ifdef __STDC__
static const double
#else
static double
#endif
two54 =  1.80143985094819840000e+16; 

#ifdef __STDC__
	double frexp(double x, int *eptr)
#else
	double frexp(x, eptr)
	double x; int *eptr;
#endif
{
	fprintf(stderr, "[s_frexp.c] enter frexp 1\n");
	int  hx, ix, lx;
	hx = __HI(x);
	ix = 0x7fffffff&hx;
	lx = __LO(x);
	*eptr = 0;
	// fprintf(stderr, "[s_frexp.c] exit frexp 1\n");
	if(ix>=0x7ff00000||((ix|lx)==0)) {
		fprintf(stderr, "[s_frexp.c] enter frexp 2\n");
		return x;
		// fprintf(stderr, "[s_frexp.c] exit frexp 2\n");
	}
	if (ix<0x00100000) {
		fprintf(stderr, "[s_frexp.c] enter frexp 3\n");
		x *= two54;
		hx = __HI(x);
		ix = hx&0x7fffffff;
		*eptr = -54;
		// fprintf(stderr, "[s_frexp.c] exit frexp 3\n");
	}
	fprintf(stderr, "[s_frexp.c] enter frexp 4\n");
	*eptr += (ix>>20)-1022;
	hx = (hx&0x800fffff)|0x3fe00000;
	__HI(x) = hx;
	return x;
	// fprintf(stderr, "[s_frexp.c] exit frexp 4\n");
}

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
	fprintf(stderr, "\n");
	if (argc != 2) {
		fprintf(stderr, "[s_frexp.c] enter main 2\n");
		printf("Usage: %s <x>\n", argv[0]);
		return 1;
		// fprintf(stderr, "[s_frexp.c] exit main 2\n");
	}
	fprintf(stderr, "[s_frexp.c] enter main 3\n");
	double x = atof(argv[1]);
	int e = 0;
	// fprintf(stderr, "[s_frexp.c] exit main 3\n");

	fprintf(stderr, "[s_frexp.c] enter main 4\n");
	double r = frexp(x, &e);
	// fprintf(stderr, "[s_frexp.c] exit main 4\n");

	fprintf(stderr, "[s_frexp.c] enter main 5\n");
	printf("result = %f\n", r);
	printf("exponent = %d\n", e);

	return 0;
	// fprintf(stderr, "[s_frexp.c] exit main 5\n");
}
// Total cost: 0.001433
// Total split cost: 0.000000, input tokens: 0, output tokens: 0, cache read tokens: 0, cache write tokens: 0, split chunks: [(0, 59)]
// Total instrumented cost: 0.001433, input tokens: 2513, output tokens: 686, cache read tokens: 0, cache write tokens: 0
