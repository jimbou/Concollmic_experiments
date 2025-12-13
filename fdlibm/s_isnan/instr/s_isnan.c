





#include "fdlibm.h"
#include <stdio.h>

#ifdef __STDC__
	int isnan(double x)
#else
	int isnan(x)
	double x;
#endif
{
	fprintf(stderr, "[s_isnan.c] enter isnan 1\n");
	int hx,lx;
	hx = (__HI(x)&0x7fffffff);
	lx = __LO(x);
	hx |= (unsigned)(lx|(-lx))>>31;	
	hx = 0x7ff00000 - hx;
	return ((unsigned)(hx))>>31;
	// fprintf(stderr, "[s_isnan.c] exit isnan 1\n");
}

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
	fprintf(stderr, "\n");
	if (argc != 2) {
		fprintf(stderr, "[s_isnan.c] enter main 2\n");
		printf("Usage: %s <arg>\n", argv[0]);
		return 1;
		// fprintf(stderr, "[s_isnan.c] exit main 2\n");
	}

	fprintf(stderr, "[s_isnan.c] enter main 3\n");
	double a0 = atof(argv[1]);

	int r = isnan(a0);
	printf("%f\n", r);

	return 0;
	// fprintf(stderr, "[s_isnan.c] exit main 3\n");
}
// Total cost: 0.000956
// Total split cost: 0.000000, input tokens: 0, output tokens: 0, cache read tokens: 0, cache write tokens: 0, split chunks: [(0, 40)]
// Total instrumented cost: 0.000956, input tokens: 2346, output tokens: 293, cache read tokens: 0, cache write tokens: 0
