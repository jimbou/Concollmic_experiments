





#include "fdlibm.h"
#include <stdio.h>

#ifdef __STDC__
	double jn(int n, double x)	
#else
	double jn(n,x)			
	double x; int n;
#endif
{
fprintf(stderr, "[w_jn.c] enter jn 1\n");
#ifdef _IEEE_LIBM
	return __ieee754_jn(n,x);
	// fprintf(stderr, "[w_jn.c] exit jn 1\n");
#else
	double z;
	z = __ieee754_jn(n,x);
	if(_LIB_VERSION == _IEEE_ || isnan(x) ) {
fprintf(stderr, "[w_jn.c] enter jn 2\n");
	    return z;
	    // fprintf(stderr, "[w_jn.c] exit jn 2\n");
	}
	if(fabs(x)>X_TLOSS) {
fprintf(stderr, "[w_jn.c] enter jn 3\n");
	    return __kernel_standard((double)n,x,38); 
	    // fprintf(stderr, "[w_jn.c] exit jn 3\n");
	} else {
fprintf(stderr, "[w_jn.c] enter jn 4\n");
	    return z;
	    // fprintf(stderr, "[w_jn.c] exit jn 4\n");
	}
#endif
}

#ifdef __STDC__
	double yn(int n, double x)	
#else
	double yn(n,x)			
	double x; int n;
#endif
{
fprintf(stderr, "[w_jn.c] enter yn 1\n");
#ifdef _IEEE_LIBM
	return __ieee754_yn(n,x);
	// fprintf(stderr, "[w_jn.c] exit yn 1\n");
#else
	double z;
	z = __ieee754_yn(n,x);
	if(_LIB_VERSION == _IEEE_ || isnan(x) ) {
fprintf(stderr, "[w_jn.c] enter yn 2\n");
	    return z;
	    // fprintf(stderr, "[w_jn.c] exit yn 2\n");
	}
        if(x <= 0.0){
                if(x==0.0) {
fprintf(stderr, "[w_jn.c] enter yn 3\n");
                    
                    return __kernel_standard((double)n,x,12);
                    // fprintf(stderr, "[w_jn.c] exit yn 3\n");
                } else {
fprintf(stderr, "[w_jn.c] enter yn 4\n");
                    
                    return __kernel_standard((double)n,x,13);
                    // fprintf(stderr, "[w_jn.c] exit yn 4\n");
                }
        }
	if(x>X_TLOSS) {
fprintf(stderr, "[w_jn.c] enter yn 5\n");
	    return __kernel_standard((double)n,x,39); 
	    // fprintf(stderr, "[w_jn.c] exit yn 5\n");
	} else {
fprintf(stderr, "[w_jn.c] enter yn 6\n");
	    return z;
	    // fprintf(stderr, "[w_jn.c] exit yn 6\n");
	}
#endif
}

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
fprintf(stderr, "\n");

    if (argc != 3) {
fprintf(stderr, "[w_jn.c] enter main 2\n");
        printf("Usage: %s <arg> <arg>\n", argv[0]);
        return 1;
        // fprintf(stderr, "[w_jn.c] exit main 2\n");
    }

fprintf(stderr, "[w_jn.c] enter main 3\n");
    int a0 = atoi(argv[1]);
    double a1 = atof(argv[2]);

    double r = jn(a0, a1);
    printf("%f\n", r);

    return 0;
    // fprintf(stderr, "[w_jn.c] exit main 3\n");
}
// Total cost: 0.001569
// Total split cost: 0.000000, input tokens: 0, output tokens: 0, cache read tokens: 0, cache write tokens: 0, split chunks: [(0, 74)]
// Total instrumented cost: 0.001569, input tokens: 2559, output tokens: 798, cache read tokens: 0, cache write tokens: 0
