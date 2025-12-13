



#include "fdlibm.h"
#include <errno.h>
#include <stdio.h>

#ifdef __STDC__
	double ldexp(double value, int exp)
#else
	double ldexp(value, exp)
	double value; int exp;
#endif
{
	fprintf(stderr, "[s_ldexp.c] enter ldexp 1\n");
	if(!finite(value)||value==0.0) {
		fprintf(stderr, "[s_ldexp.c] enter ldexp 2\n");
		return value;
		// fprintf(stderr, "[s_ldexp.c] exit ldexp 2\n");
	}
	// fprintf(stderr, "[s_ldexp.c] exit ldexp 1\n");
	fprintf(stderr, "[s_ldexp.c] enter ldexp 3\n");
	value = scalbn(value,exp);
	// fprintf(stderr, "[s_ldexp.c] exit ldexp 3\n");
	fprintf(stderr, "[s_ldexp.c] enter ldexp 4\n");
	if(!finite(value)||value==0.0) {
		fprintf(stderr, "[s_ldexp.c] enter ldexp 5\n");
		errno = ERANGE;
		// fprintf(stderr, "[s_ldexp.c] exit ldexp 5\n");
	}
	// fprintf(stderr, "[s_ldexp.c] exit ldexp 4\n");
	fprintf(stderr, "[s_ldexp.c] enter ldexp 6\n");
	return value;
	// fprintf(stderr, "[s_ldexp.c] exit ldexp 6\n");
}

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
	fprintf(stderr, "[s_ldexp.c] enter main 1\n");
	if (argc != 3) {
		fprintf(stderr, "[s_ldexp.c] enter main 2\n");
		printf("Usage: %s <arg> <arg>\n", argv[0]);
		return 1;
		// fprintf(stderr, "[s_ldexp.c] exit main 2\n");
	}
	// fprintf(stderr, "[s_ldexp.c] exit main 1\n");
	fprintf(stderr, "[s_ldexp.c] enter main 3\n");
	double a0 = atof(argv[1]);
	int a1 = atoi(argv[2]);
	// fprintf(stderr, "[s_ldexp.c] exit main 3\n");
	fprintf(stderr, "[s_ldexp.c] enter main 4\n");
	double r = ldexp(a0, a1);
	printf("%f\n", r);
	// fprintf(stderr, "[s_ldexp.c] exit main 4\n");
	fprintf(stderr, "[s_ldexp.c] enter main 5\n");
	return 0;
	// fprintf(stderr, "[s_ldexp.c] exit main 5\n");
}
// Total cost: 0.002719
// Total split cost: 0.000000, input tokens: 0, output tokens: 0, cache read tokens: 0, cache write tokens: 0, split chunks: [(0, 38)]
// Total instrumented cost: 0.002719, input tokens: 5272, output tokens: 1178, cache read tokens: 0, cache write tokens: 0
