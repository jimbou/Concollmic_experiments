




#include "fdlibm.h"
#include <stdio.h>


#ifdef __STDC__
	double atanh(double x)		
#else
	double atanh(x)			
	double x;
#endif
{
#ifdef _IEEE_LIBM
	fprintf(stderr, "[w_atanh.c] enter atanh 1\n");
	return __ieee754_atanh(x);
	// fprintf(stderr, "[w_atanh.c] exit atanh 1\n");
#else
	fprintf(stderr, "[w_atanh.c] enter atanh 2\n");
	double z,y;
	z = __ieee754_atanh(x);
	// fprintf(stderr, "[w_atanh.c] exit atanh 2\n");
	if(_LIB_VERSION == _IEEE_ || isnan(x)) {
		fprintf(stderr, "[w_atanh.c] enter atanh 3\n");
		return z;
		// fprintf(stderr, "[w_atanh.c] exit atanh 3\n");
	}
	fprintf(stderr, "[w_atanh.c] enter atanh 4\n");
	y = fabs(x);
	// fprintf(stderr, "[w_atanh.c] exit atanh 4\n");
	if(y>=1.0) {
		fprintf(stderr, "[w_atanh.c] enter atanh 5\n");
		if(y>1.0) {
			fprintf(stderr, "[w_atanh.c] enter atanh 6\n");
			return __kernel_standard(x,x,30); 
			// fprintf(stderr, "[w_atanh.c] exit atanh 6\n");
		} else {
			fprintf(stderr, "[w_atanh.c] enter atanh 7\n");
			return __kernel_standard(x,x,31); 
			// fprintf(stderr, "[w_atanh.c] exit atanh 7\n");
		}
		// fprintf(stderr, "[w_atanh.c] exit atanh 5\n");
	} else {
		fprintf(stderr, "[w_atanh.c] enter atanh 8\n");
		return z;
		// fprintf(stderr, "[w_atanh.c] exit atanh 8\n");
	}
#endif
}

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
	fprintf(stderr, "\n");
	if (argc != 2) {
		fprintf(stderr, "[w_atanh.c] enter main 2\n");
		printf("Usage: %s <arg>\n", argv[0]);
		return 1;
		// fprintf(stderr, "[w_atanh.c] exit main 2\n");
	}
	fprintf(stderr, "[w_atanh.c] enter main 3\n");
	double a0 = atof(argv[1]);
	// fprintf(stderr, "[w_atanh.c] exit main 3\n");

	fprintf(stderr, "[w_atanh.c] enter main 4\n");
	double r = atanh(a0);
	printf("%f\n", r);

	return 0;
	// fprintf(stderr, "[w_atanh.c] exit main 4\n");
}
// Total cost: 0.001312
// Total split cost: 0.000000, input tokens: 0, output tokens: 0, cache read tokens: 0, cache write tokens: 0, split chunks: [(0, 49)]
// Total instrumented cost: 0.001312, input tokens: 2393, output tokens: 605, cache read tokens: 0, cache write tokens: 0
