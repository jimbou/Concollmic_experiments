






#include "fdlibm.h"
#include "stdio.h"


#ifdef __STDC__
	double pow(double x, double y)	
#else
	double pow(x,y)			
	double x,y;
#endif
{
#ifdef _IEEE_LIBM
	fprintf(stderr, "[w_pow.c] enter pow 1\n");
	return  __ieee754_pow(x,y);
	// fprintf(stderr, "[w_pow.c] exit pow 1\n");
#else
	fprintf(stderr, "\n");
	double z;
	z=__ieee754_pow(x,y);
	if(_LIB_VERSION == _IEEE_|| isnan(y)) {
	    fprintf(stderr, "[w_pow.c] enter pow 3\n");
	    return z;
	    // fprintf(stderr, "[w_pow.c] exit pow 3\n");
	}
	if(isnan(x)) {
	    if(y==0.0) {
	        fprintf(stderr, "[w_pow.c] enter pow 4\n");
	        return __kernel_standard(x,y,42); 
	        // fprintf(stderr, "[w_pow.c] exit pow 4\n");
	    } else {
		fprintf(stderr, "[w_pow.c] enter pow 5\n");
		return z;
		// fprintf(stderr, "[w_pow.c] exit pow 5\n");
	    }
	}
	if(x==0.0){ 
	    if(y==0.0) {
	        fprintf(stderr, "[w_pow.c] enter pow 6\n");
	        return __kernel_standard(x,y,20); 
	        // fprintf(stderr, "[w_pow.c] exit pow 6\n");
	    }
	    if(finite(y)&&y<0.0) {
	        fprintf(stderr, "[w_pow.c] enter pow 7\n");
	        return __kernel_standard(x,y,23); 
	        // fprintf(stderr, "[w_pow.c] exit pow 7\n");
	    }
	    fprintf(stderr, "[w_pow.c] enter pow 8\n");
	    return z;
	    // fprintf(stderr, "[w_pow.c] exit pow 8\n");
	}
	if(!finite(z)) {
	    if(finite(x)&&finite(y)) {
	        if(isnan(z)) {
	            fprintf(stderr, "[w_pow.c] enter pow 9\n");
	            return __kernel_standard(x,y,24); 
	            // fprintf(stderr, "[w_pow.c] exit pow 9\n");
	        } else {
	            fprintf(stderr, "[w_pow.c] enter pow 10\n");
	            return __kernel_standard(x,y,21); 
	            // fprintf(stderr, "[w_pow.c] exit pow 10\n");
	        }
	    }
	} 
	if(z==0.0&&finite(x)&&finite(y)) {
	    fprintf(stderr, "[w_pow.c] enter pow 11\n");
	    return __kernel_standard(x,y,22); 
	    // fprintf(stderr, "[w_pow.c] exit pow 11\n");
	}
	fprintf(stderr, "[w_pow.c] enter pow 12\n");
	return z;
	// fprintf(stderr, "[w_pow.c] exit pow 12\n");
#endif
}

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
    fprintf(stderr, "\n");
    if (argc != 3) {
        fprintf(stderr, "[w_pow.c] enter main 2\n");
        printf("Usage: %s <arg> <arg>\n", argv[0]);
        return 1;
        // fprintf(stderr, "[w_pow.c] exit main 2\n");
    }

    fprintf(stderr, "[w_pow.c] enter main 3\n");
    double a0 = atof(argv[1]);
    double a1 = atof(argv[2]);
    // fprintf(stderr, "[w_pow.c] exit main 3\n");

    fprintf(stderr, "[w_pow.c] enter main 4\n");
    double r = pow(a0, a1);
    printf("%f\n", r);
    // fprintf(stderr, "[w_pow.c] exit main 4\n");

    fprintf(stderr, "[w_pow.c] enter main 5\n");
    return 0;
    // fprintf(stderr, "[w_pow.c] exit main 5\n");
}
// Total cost: 0.001572
// Total split cost: 0.000000, input tokens: 0, output tokens: 0, cache read tokens: 0, cache write tokens: 0, split chunks: [(0, 68)]
// Total instrumented cost: 0.001572, input tokens: 2532, output tokens: 808, cache read tokens: 0, cache write tokens: 0
