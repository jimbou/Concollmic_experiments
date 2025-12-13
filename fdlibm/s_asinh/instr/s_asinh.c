#include <stdio.h>

#include "fdlibm.h"

#ifdef __STDC__
static const double 
#else
static double 
#endif
one =  1.00000000000000000000e+00, 
ln2 =  6.93147180559945286227e-01, 
huge=  1.00000000000000000000e+300; 

#ifdef __STDC__
	double asinh(double x)
#else
	double asinh(x)
	double x;
#endif
{	
	fprintf(stderr, "[s_asinh.c] enter asinh 1\n");
	double t,w;
	int hx,ix;
	hx = __HI(x);
	ix = hx&0x7fffffff;
	if(ix>=0x7ff00000) {
		fprintf(stderr, "[s_asinh.c] enter asinh 2\n");
		return x+x;
		// fprintf(stderr, "[s_asinh.c] exit asinh 2\n");
	}
	if(ix< 0x3e300000) {
		fprintf(stderr, "[s_asinh.c] enter asinh 3\n");
		if(huge+x>one) {
			fprintf(stderr, "[s_asinh.c] enter asinh 4\n");
			return x;
			// fprintf(stderr, "[s_asinh.c] exit asinh 4\n");
		}
		// fprintf(stderr, "[s_asinh.c] exit asinh 3\n");
	}
	if(ix>0x41b00000) {
		fprintf(stderr, "[s_asinh.c] enter asinh 5\n");
		w = __ieee754_log(fabs(x))+ln2;
		// fprintf(stderr, "[s_asinh.c] exit asinh 5\n");
	} else if (ix>0x40000000) {
		fprintf(stderr, "[s_asinh.c] enter asinh 6\n");
		t = fabs(x);
		w = __ieee754_log(2.0*t+one/(sqrt(x*x+one)+t));
		// fprintf(stderr, "[s_asinh.c] exit asinh 6\n");
	} else {
		fprintf(stderr, "[s_asinh.c] enter asinh 7\n");
		t = x*x;
		w =log1p(fabs(x)+t/(one+sqrt(one+t)));
		// fprintf(stderr, "[s_asinh.c] exit asinh 7\n");
	}
	if(hx>0) {
		fprintf(stderr, "[s_asinh.c] enter asinh 8\n");
		return w;
		// fprintf(stderr, "[s_asinh.c] exit asinh 8\n");
	} else {
		fprintf(stderr, "[s_asinh.c] enter asinh 9\n");
		return -w;
		// fprintf(stderr, "[s_asinh.c] exit asinh 9\n");
	}
	// fprintf(stderr, "[s_asinh.c] exit asinh 1\n");
}

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
	fprintf(stderr, "[s_asinh.c] enter main 1\n");
	if (argc != 2) {
		fprintf(stderr, "[s_asinh.c] enter main 2\n");
		printf("Usage: %s <arg>\n", argv[0]);
		return 1;
		// fprintf(stderr, "[s_asinh.c] exit main 2\n");
	}
	fprintf(stderr, "[s_asinh.c] enter main 3\n");
	double a0 = atof(argv[1]);
	fprintf(stderr, "[s_asinh.c] enter main 4\n");
	double r = asinh(a0);
	printf("%f\n", r);
	return 0;
	// fprintf(stderr, "[s_asinh.c] exit main 4\n");
	// fprintf(stderr, "[s_asinh.c] exit main 3\n");
	// fprintf(stderr, "[s_asinh.c] exit main 1\n");
}
// Total cost: 0.001617
// Total split cost: 0.000000, input tokens: 0, output tokens: 0, cache read tokens: 0, cache write tokens: 0, split chunks: [(0, 61)]
// Total instrumented cost: 0.001617, input tokens: 2545, output tokens: 845, cache read tokens: 0, cache write tokens: 0
