





#include "fdlibm.h"
#include <stdio.h>


#ifdef __STDC__
	double log10(double x)		
#else
	double log10(x)			
	double x;
#endif
{
#ifdef _IEEE_LIBM
	fprintf(stderr, "[w_log10.c] enter log10 1\n");
	return __ieee754_log10(x);
	// fprintf(stderr, "[w_log10.c] exit log10 1\n");
#else
	fprintf(stderr, "[w_log10.c] enter log10 2\n");
	double z;
	z = __ieee754_log10(x);
	// fprintf(stderr, "[w_log10.c] exit log10 2\n");
	if(_LIB_VERSION == _IEEE_ || isnan(x)) {
		fprintf(stderr, "[w_log10.c] enter log10 3\n");
		return z;
		// fprintf(stderr, "[w_log10.c] exit log10 3\n");
	}
	if(x<=0.0) {
		fprintf(stderr, "\n");
	    if(x==0.0) {
			fprintf(stderr, "[w_log10.c] enter log10 5\n");
	        return __kernel_standard(x,x,18); 
	        // fprintf(stderr, "[w_log10.c] exit log10 5\n");
	    } else {
			fprintf(stderr, "[w_log10.c] enter log10 6\n");
	        return __kernel_standard(x,x,19); 
	        // fprintf(stderr, "[w_log10.c] exit log10 6\n");
	    }
	} else {
		fprintf(stderr, "[w_log10.c] enter log10 7\n");
	    return z;
	    // fprintf(stderr, "[w_log10.c] exit log10 7\n");
	}
#endif
}

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
	fprintf(stderr, "\n");
    if (argc != 2) {
		fprintf(stderr, "[w_log10.c] enter main 2\n");
        printf("Usage: %s <arg>\n", argv[0]);
        return 1;
        // fprintf(stderr, "[w_log10.c] exit main 2\n");
    }

	fprintf(stderr, "[w_log10.c] enter main 3\n");
    double a0 = atof(argv[1]);

    double r = log10(a0);
    printf("%f\n", r);

    return 0;
    // fprintf(stderr, "[w_log10.c] exit main 3\n");
}
// Total cost: 0.001269
// Total split cost: 0.000000, input tokens: 0, output tokens: 0, cache read tokens: 0, cache write tokens: 0, split chunks: [(0, 49)]
// Total instrumented cost: 0.001269, input tokens: 2383, output tokens: 569, cache read tokens: 0, cache write tokens: 0
