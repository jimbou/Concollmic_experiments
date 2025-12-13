





#include "fdlibm.h"
#include <stdio.h>

#ifdef __STDC__
	double j0(double x)		
#else
	double j0(x)			
	double x;
#endif
{
#ifdef _IEEE_LIBM
	fprintf(stderr, "[w_j0.c] enter j0 1\n");
	return __ieee754_j0(x);
	// fprintf(stderr, "[w_j0.c] exit j0 1\n");
#else
	fprintf(stderr, "[w_j0.c] enter j0 2\n");
	double z = __ieee754_j0(x);
	// fprintf(stderr, "[w_j0.c] exit j0 2\n");
	if(_LIB_VERSION == _IEEE_ || isnan(x)) {
		fprintf(stderr, "[w_j0.c] enter j0 3\n");
		return z;
		// fprintf(stderr, "[w_j0.c] exit j0 3\n");
	}
	if(fabs(x)>X_TLOSS) {
		fprintf(stderr, "[w_j0.c] enter j0 4\n");
	        return __kernel_standard(x,x,34); 
		// fprintf(stderr, "[w_j0.c] exit j0 4\n");
	} else {
		fprintf(stderr, "[w_j0.c] enter j0 5\n");
	    return z;
	    // fprintf(stderr, "[w_j0.c] exit j0 5\n");
	}
#endif
}

#ifdef __STDC__
	double y0(double x)		
#else
	double y0(x)			
	double x;
#endif
{
#ifdef _IEEE_LIBM
	fprintf(stderr, "[w_j0.c] enter y0 1\n");
	return __ieee754_y0(x);
	// fprintf(stderr, "[w_j0.c] exit y0 1\n");
#else
	fprintf(stderr, "[w_j0.c] enter y0 2\n");
	double z;
	z = __ieee754_y0(x);
	// fprintf(stderr, "[w_j0.c] exit y0 2\n");
	if(_LIB_VERSION == _IEEE_ || isnan(x) ) {
		fprintf(stderr, "[w_j0.c] enter y0 3\n");
		return z;
		// fprintf(stderr, "[w_j0.c] exit y0 3\n");
	}
        if(x <= 0.0){
                if(x==0.0) {
                    fprintf(stderr, "[w_j0.c] enter y0 4\n");
                    
                    return __kernel_standard(x,x,8);
                    // fprintf(stderr, "[w_j0.c] exit y0 4\n");
                } else {
                    fprintf(stderr, "[w_j0.c] enter y0 5\n");
                    
                    return __kernel_standard(x,x,9);
                    // fprintf(stderr, "[w_j0.c] exit y0 5\n");
                }
        }
	if(x>X_TLOSS) {
		fprintf(stderr, "[w_j0.c] enter y0 6\n");
	        return __kernel_standard(x,x,35); 
		// fprintf(stderr, "[w_j0.c] exit y0 6\n");
	} else {
		fprintf(stderr, "[w_j0.c] enter y0 7\n");
	    return z;
	    // fprintf(stderr, "[w_j0.c] exit y0 7\n");
	}
#endif
}

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
	fprintf(stderr, "\n");

    if (argc != 2) {
		fprintf(stderr, "[w_j0.c] enter main 2\n");
        printf("Usage: %s <arg>\n", argv[0]);
        return 1;
        // fprintf(stderr, "[w_j0.c] exit main 2\n");
    }

	fprintf(stderr, "[w_j0.c] enter main 3\n");
    double a0 = atof(argv[1]);

    double r = j0(a0);
    printf("%f\n", r);

    return 0;
	// fprintf(stderr, "[w_j0.c] exit main 3\n");
}
// Total cost: 0.001574
// Total split cost: 0.000000, input tokens: 0, output tokens: 0, cache read tokens: 0, cache write tokens: 0, split chunks: [(0, 72)]
// Total instrumented cost: 0.001574, input tokens: 2514, output tokens: 814, cache read tokens: 0, cache write tokens: 0
