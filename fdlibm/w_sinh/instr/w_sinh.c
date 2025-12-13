





#include "fdlibm.h"
#include <stdio.h>

#ifdef __STDC__
	double sinh(double x)		
#else
	double sinh(x)			
	double x;
#endif
{
#ifdef _IEEE_LIBM
	fprintf(stderr, "[w_sinh.c] enter sinh 1\n");
	return __ieee754_sinh(x);
	// fprintf(stderr, "[w_sinh.c] exit sinh 1\n");
#else
	fprintf(stderr, "[w_sinh.c] enter sinh 2\n");
	double z; 
	z = __ieee754_sinh(x);
	// fprintf(stderr, "[w_sinh.c] exit sinh 2\n");
	if(_LIB_VERSION == _IEEE_) {
		fprintf(stderr, "[w_sinh.c] enter sinh 3\n");
		return z;
		// fprintf(stderr, "[w_sinh.c] exit sinh 3\n");
	}
	fprintf(stderr, "[w_sinh.c] enter sinh 4\n");
	if(!finite(z)&&finite(x)) {
		fprintf(stderr, "[w_sinh.c] enter sinh 5\n");
	    return __kernel_standard(x,x,25); 
	    // fprintf(stderr, "[w_sinh.c] exit sinh 5\n");
	} else {
		fprintf(stderr, "[w_sinh.c] enter sinh 6\n");
	    return z;
	    // fprintf(stderr, "[w_sinh.c] exit sinh 6\n");
	}
	// fprintf(stderr, "[w_sinh.c] exit sinh 4\n");
#endif
}

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
	fprintf(stderr, "\n");

    if (argc != 2) {
		fprintf(stderr, "[w_sinh.c] enter main 2\n");
        printf("Usage: %s <arg>\n", argv[0]);
        return 1;
        // fprintf(stderr, "[w_sinh.c] exit main 2\n");
    }

	fprintf(stderr, "[w_sinh.c] enter main 3\n");
    double a0 = atof(argv[1]);

    double r = sinh(a0);
    printf("%f\n", r);

    return 0;
    // fprintf(stderr, "[w_sinh.c] exit main 3\n");
}
// Total cost: 0.001157
// Total split cost: 0.000000, input tokens: 0, output tokens: 0, cache read tokens: 0, cache write tokens: 0, split chunks: [(0, 45)]
// Total instrumented cost: 0.001157, input tokens: 2355, output tokens: 474, cache read tokens: 0, cache write tokens: 0
