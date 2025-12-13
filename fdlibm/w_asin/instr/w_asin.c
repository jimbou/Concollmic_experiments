






#include "fdlibm.h"
#include <stdio.h>


#ifdef __STDC__
	double asin(double x)		
#else
	double asin(x)			
	double x;
#endif
{
#ifdef _IEEE_LIBM
	fprintf(stderr, "[w_asin.c] enter asin 1\n");
	return __ieee754_asin(x);
	// fprintf(stderr, "[w_asin.c] exit asin 1\n");
#else
	fprintf(stderr, "[w_asin.c] enter asin 2\n");
	double z;
	z = __ieee754_asin(x);
	// fprintf(stderr, "[w_asin.c] exit asin 2\n");
	if(_LIB_VERSION == _IEEE_ || isnan(x)) {
		fprintf(stderr, "[w_asin.c] enter asin 3\n");
		return z;
		// fprintf(stderr, "[w_asin.c] exit asin 3\n");
	}
	if(fabs(x)>1.0) {
	        fprintf(stderr, "[w_asin.c] enter asin 4\n");
	        return __kernel_standard(x,x,2); 
	        // fprintf(stderr, "[w_asin.c] exit asin 4\n");
	} else {
	    fprintf(stderr, "[w_asin.c] enter asin 5\n");
	    return z;
	    // fprintf(stderr, "[w_asin.c] exit asin 5\n");
	}
#endif
}

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
    fprintf(stderr, "[w_asin.c] enter main 1\n");
    if (argc != 2) {
        fprintf(stderr, "[w_asin.c] enter main 2\n");
        printf("Usage: %s <arg>\n", argv[0]);
        return 1;
        // fprintf(stderr, "[w_asin.c] exit main 2\n");
    }
    // fprintf(stderr, "[w_asin.c] exit main 1\n");

    fprintf(stderr, "[w_asin.c] enter main 3\n");
    double a0 = atof(argv[1]);
    // fprintf(stderr, "[w_asin.c] exit main 3\n");

    fprintf(stderr, "[w_asin.c] enter main 4\n");
    double r = asin(a0);
    printf("%f\n", r);
    // fprintf(stderr, "[w_asin.c] exit main 4\n");

    fprintf(stderr, "[w_asin.c] enter main 5\n");
    return 0;
    // fprintf(stderr, "[w_asin.c] exit main 5\n");
}
// Total cost: 0.001219
// Total split cost: 0.000000, input tokens: 0, output tokens: 0, cache read tokens: 0, cache write tokens: 0, split chunks: [(0, 47)]
// Total instrumented cost: 0.001219, input tokens: 2361, output tokens: 529, cache read tokens: 0, cache write tokens: 0
