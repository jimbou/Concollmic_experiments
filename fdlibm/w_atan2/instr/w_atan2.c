#include <stdio.h>
#include <stdlib.h>

#include "fdlibm.h"


#ifdef __STDC__
	double atan2(double y, double x)	
#else
	double atan2(y,x)			
	double y,x;
#endif
{
#ifdef _IEEE_LIBM
	fprintf(stderr, "[w_atan2.c] enter atan2 1\n");
	return __ieee754_atan2(y,x);
	// fprintf(stderr, "[w_atan2.c] exit atan2 1\n");
#else
	fprintf(stderr, "[w_atan2.c] enter atan2 2\n");
	double z;
	z = __ieee754_atan2(y,x);
	// fprintf(stderr, "[w_atan2.c] exit atan2 2\n");
	if(_LIB_VERSION == _IEEE_||isnan(x)||isnan(y)) {
		fprintf(stderr, "[w_atan2.c] enter atan2 3\n");
		return z;
		// fprintf(stderr, "[w_atan2.c] exit atan2 3\n");
	}
	if(x==0.0&&y==0.0) {
		fprintf(stderr, "[w_atan2.c] enter atan2 4\n");
	        return __kernel_standard(y,x,3); 
		// fprintf(stderr, "[w_atan2.c] exit atan2 4\n");
	} else {
		fprintf(stderr, "[w_atan2.c] enter atan2 5\n");
	    return z;
		// fprintf(stderr, "[w_atan2.c] exit atan2 5\n");
	}
#endif
}

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
	fprintf(stderr, "\n");
    if (argc != 3) {
		fprintf(stderr, "[w_atan2.c] enter main 2\n");
        printf("Usage: %s <arg> <arg>\n", argv[0]);
        return 1;
		// fprintf(stderr, "[w_atan2.c] exit main 2\n");
    }

	fprintf(stderr, "[w_atan2.c] enter main 3\n");
    double a0 = atof(argv[1]);
    double a1 = atof(argv[2]);

    double r = atan2(a0, a1);
    printf("%f\n", r);

    return 0;
	// fprintf(stderr, "[w_atan2.c] exit main 3\n");
}
// Total cost: 0.001211
// Total split cost: 0.000000, input tokens: 0, output tokens: 0, cache read tokens: 0, cache write tokens: 0, split chunks: [(0, 47)]
// Total instrumented cost: 0.001211, input tokens: 2402, output tokens: 511, cache read tokens: 0, cache write tokens: 0
