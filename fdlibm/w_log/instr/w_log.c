





#include "fdlibm.h"
#include <stdio.h>


#ifdef __STDC__
	double log(double x)		
#else
	double log(x)			
	double x;
#endif
{
#ifdef _IEEE_LIBM
	fprintf(stderr, "[w_log.c] enter log 1\n");
	return __ieee754_log(x);
	// fprintf(stderr, "[w_log.c] exit log 1\n");
#else
	fprintf(stderr, "[w_log.c] enter log 2\n");
	double z;
	z = __ieee754_log(x);
	if(_LIB_VERSION == _IEEE_ || isnan(x) || x > 0.0) {
		fprintf(stderr, "[w_log.c] enter log 3\n");
		return z;
		// fprintf(stderr, "[w_log.c] exit log 3\n");
	}
	if(x==0.0)
	{
		fprintf(stderr, "[w_log.c] enter log 4\n");
		return __kernel_standard(x,x,16); 
		// fprintf(stderr, "[w_log.c] exit log 4\n");
	}
	else 
	{
		fprintf(stderr, "[w_log.c] enter log 5\n");
		return __kernel_standard(x,x,17); 
		// fprintf(stderr, "[w_log.c] exit log 5\n");
	}
	// fprintf(stderr, "[w_log.c] exit log 2\n");
#endif
}

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
	fprintf(stderr, "\n");
	if (argc != 2) {
		fprintf(stderr, "[w_log.c] enter main 2\n");
		printf("Usage: %s <arg>\n", argv[0]);
		return 1;
		// fprintf(stderr, "[w_log.c] exit main 2\n");
	}
	fprintf(stderr, "[w_log.c] enter main 3\n");
	double a0 = atof(argv[1]);
	double r = log(a0);
	printf("%f\n", r);
	return 0;
	// fprintf(stderr, "[w_log.c] exit main 3\n");
}
// Total cost: 0.001113
// Total split cost: 0.000000, input tokens: 0, output tokens: 0, cache read tokens: 0, cache write tokens: 0, split chunks: [(0, 46)]
// Total instrumented cost: 0.001113, input tokens: 2366, output tokens: 431, cache read tokens: 0, cache write tokens: 0
