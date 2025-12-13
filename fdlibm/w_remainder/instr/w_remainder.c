#include <stdio.h>






#include "fdlibm.h"

#ifdef __STDC__
	double remainder(double x, double y)	
#else
	double remainder(x,y)			
	double x,y;
#endif
{
#ifdef _IEEE_LIBM
	fprintf(stderr, "[w_remainder.c] enter remainder 1\n");
	return __ieee754_remainder(x,y);
	// fprintf(stderr, "[w_remainder.c] exit remainder 1\n");
#else
	fprintf(stderr, "[w_remainder.c] enter remainder 2\n");
	double z;
	z = __ieee754_remainder(x,y);
	// fprintf(stderr, "[w_remainder.c] exit remainder 2\n");
	if(_LIB_VERSION == _IEEE_ || isnan(y)) {
		fprintf(stderr, "[w_remainder.c] enter remainder 3\n");
		return z;
		// fprintf(stderr, "[w_remainder.c] exit remainder 3\n");
	}
	if(y==0.0) {
		fprintf(stderr, "[w_remainder.c] enter remainder 4\n");
		return __kernel_standard(x,y,28); 
		// fprintf(stderr, "[w_remainder.c] exit remainder 4\n");
	}
	else {
		fprintf(stderr, "[w_remainder.c] enter remainder 5\n");
		return z;
		// fprintf(stderr, "[w_remainder.c] exit remainder 5\n");
	}
#endif
}

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
	fprintf(stderr, "\n");

    if (argc != 3) {
		fprintf(stderr, "[w_remainder.c] enter main 2\n");
        printf("Usage: %s <arg> <arg>\n", argv[0]);
        return 1;
		// fprintf(stderr, "[w_remainder.c] exit main 2\n");
    }

	fprintf(stderr, "[w_remainder.c] enter main 3\n");
    double a0 = atof(argv[1]);
    double a1 = atof(argv[2]);

    double r = remainder(a0, a1);
    printf("%f\n", r);

    return 0;
	// fprintf(stderr, "[w_remainder.c] exit main 3\n");
}
// Total cost: 0.001133
// Total split cost: 0.000000, input tokens: 0, output tokens: 0, cache read tokens: 0, cache write tokens: 0, split chunks: [(0, 46)]
// Total instrumented cost: 0.001133, input tokens: 2383, output tokens: 445, cache read tokens: 0, cache write tokens: 0
