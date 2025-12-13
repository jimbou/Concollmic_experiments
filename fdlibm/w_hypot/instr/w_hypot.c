





#include "fdlibm.h"
#include "stdio.h"

#ifdef __STDC__
	double hypot(double x, double y)
#else
	double hypot(x,y)		
	double x,y;
#endif
{
#ifdef _IEEE_LIBM
	fprintf(stderr, "[w_hypot.c] enter hypot 1\n");
	return __ieee754_hypot(x,y);
	// fprintf(stderr, "[w_hypot.c] exit hypot 1\n");
#else
	fprintf(stderr, "[w_hypot.c] enter hypot 2\n");
	double z;
	z = __ieee754_hypot(x,y);
	// fprintf(stderr, "[w_hypot.c] exit hypot 2\n");
	if(_LIB_VERSION == _IEEE_)
	{
		fprintf(stderr, "[w_hypot.c] enter hypot 3\n");
		return z;
		// fprintf(stderr, "[w_hypot.c] exit hypot 3\n");
	}
	fprintf(stderr, "[w_hypot.c] enter hypot 4\n");
	if((!finite(z))&&finite(x)&&finite(y))
	{
		fprintf(stderr, "[w_hypot.c] enter hypot 5\n");
		return __kernel_standard(x,y,4); 
		// fprintf(stderr, "[w_hypot.c] exit hypot 5\n");
	}
	else
	{
		fprintf(stderr, "[w_hypot.c] enter hypot 6\n");
		return z;
		// fprintf(stderr, "[w_hypot.c] exit hypot 6\n");
	}
	// fprintf(stderr, "[w_hypot.c] exit hypot 4\n");
#endif
}

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
	fprintf(stderr, "\n");
	if (argc != 3) {
		fprintf(stderr, "[w_hypot.c] enter main 2\n");
		printf("Usage: %s <arg> <arg>\n", argv[0]);
		return 1;
		// fprintf(stderr, "[w_hypot.c] exit main 2\n");
	}
	fprintf(stderr, "[w_hypot.c] enter main 3\n");
	double a0 = atof(argv[1]);
	double a1 = atof(argv[2]);
	// fprintf(stderr, "[w_hypot.c] exit main 3\n");

	fprintf(stderr, "[w_hypot.c] enter main 4\n");
	double r = hypot(a0, a1);
	printf("%f\n", r);
	// fprintf(stderr, "[w_hypot.c] exit main 4\n");

	fprintf(stderr, "[w_hypot.c] enter main 5\n");
	return 0;
	// fprintf(stderr, "[w_hypot.c] exit main 5\n");
}
// Total cost: 0.001210
// Total split cost: 0.000000, input tokens: 0, output tokens: 0, cache read tokens: 0, cache write tokens: 0, split chunks: [(0, 47)]
// Total instrumented cost: 0.001210, input tokens: 2383, output tokens: 515, cache read tokens: 0, cache write tokens: 0
