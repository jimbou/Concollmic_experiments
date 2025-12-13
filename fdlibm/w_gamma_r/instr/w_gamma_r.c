





#include "fdlibm.h"
#include <stdio.h>


#ifdef __STDC__
	double gamma_r(double x, int *signgamp) 
#else
	double gamma_r(x,signgamp)              
        double x; int *signgamp;
#endif
{
#ifdef _IEEE_LIBM
	fprintf(stderr, "[w_gamma_r.c] enter gamma_r 1\n");
	return __ieee754_gamma_r(x,signgamp);
	// fprintf(stderr, "[w_gamma_r.c] exit gamma_r 1\n");
#else
        fprintf(stderr, "[w_gamma_r.c] enter gamma_r 2\n");
        double y;
        y = __ieee754_gamma_r(x,signgamp);
        // fprintf(stderr, "[w_gamma_r.c] exit gamma_r 2\n");
        if(_LIB_VERSION == _IEEE_) {
            fprintf(stderr, "[w_gamma_r.c] enter gamma_r 3\n");
            return y;
            // fprintf(stderr, "[w_gamma_r.c] exit gamma_r 3\n");
        }
        fprintf(stderr, "[w_gamma_r.c] enter gamma_r 4\n");
        if(!finite(y)&&finite(x)) {
            fprintf(stderr, "[w_gamma_r.c] enter gamma_r 5\n");
            if(floor(x)==x&&x<=0.0) {
                fprintf(stderr, "[w_gamma_r.c] enter gamma_r 6\n");
                return __kernel_standard(x,x,41); 
                // fprintf(stderr, "[w_gamma_r.c] exit gamma_r 6\n");
            } else {
                fprintf(stderr, "[w_gamma_r.c] enter gamma_r 7\n");
                return __kernel_standard(x,x,40); 
                // fprintf(stderr, "[w_gamma_r.c] exit gamma_r 7\n");
            }
            // fprintf(stderr, "[w_gamma_r.c] exit gamma_r 5\n");
        } else {
            fprintf(stderr, "[w_gamma_r.c] enter gamma_r 8\n");
            return y;
            // fprintf(stderr, "[w_gamma_r.c] exit gamma_r 8\n");
        }
        // fprintf(stderr, "[w_gamma_r.c] exit gamma_r 4\n");
#endif
}             


#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
    fprintf(stderr, "[w_gamma_r.c] enter main 1\n");
    if (argc != 2) {
        fprintf(stderr, "[w_gamma_r.c] enter main 2\n");
        printf("Usage: %s <x>\n", argv[0]);
        return 1;
        // fprintf(stderr, "[w_gamma_r.c] exit main 2\n");
    }
    // fprintf(stderr, "[w_gamma_r.c] exit main 1\n");

    fprintf(stderr, "[w_gamma_r.c] enter main 3\n");
    double x = atof(argv[1]);
    int sign = 0;
    // fprintf(stderr, "[w_gamma_r.c] exit main 3\n");

    fprintf(stderr, "[w_gamma_r.c] enter main 4\n");
    double r = gamma_r(x, &sign);
    // fprintf(stderr, "[w_gamma_r.c] exit main 4\n");

    fprintf(stderr, "[w_gamma_r.c] enter main 5\n");
    printf("result = %f\n", r);
    printf("signgamp = %d\n", sign);
    // fprintf(stderr, "[w_gamma_r.c] exit main 5\n");

    fprintf(stderr, "[w_gamma_r.c] enter main 6\n");
    return 0;
    // fprintf(stderr, "[w_gamma_r.c] exit main 6\n");
}
// Total cost: 0.001408
// Total split cost: 0.000000, input tokens: 0, output tokens: 0, cache read tokens: 0, cache write tokens: 0, split chunks: [(0, 52)]
// Total instrumented cost: 0.001408, input tokens: 2445, output tokens: 680, cache read tokens: 0, cache write tokens: 0
