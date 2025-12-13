#include <stdio.h>
#include "fdlibm.h"

extern int signgam;

#ifdef __STDC__
	double lgamma(double x)
#else
	double lgamma(x)
	double x;
#endif
{
#ifdef _IEEE_LIBM
	fprintf(stderr, "[w_lgamma.c] enter lgamma 1\n");
	return __ieee754_lgamma_r(x,&signgam);
	// fprintf(stderr, "[w_lgamma.c] exit lgamma 1\n");
#else
	fprintf(stderr, "[w_lgamma.c] enter lgamma 2\n");
        double y;
        y = __ieee754_lgamma_r(x,&signgam);
        if(_LIB_VERSION == _IEEE_) {
	fprintf(stderr, "[w_lgamma.c] enter lgamma 3\n");
            return y;
            // fprintf(stderr, "[w_lgamma.c] exit lgamma 3\n");
        }
        if(!finite(y)&&finite(x)) {
            if(floor(x)==x&&x<=0.0) {
	fprintf(stderr, "[w_lgamma.c] enter lgamma 4\n");
                return __kernel_standard(x,x,15); 
                // fprintf(stderr, "[w_lgamma.c] exit lgamma 4\n");
            } else {
	fprintf(stderr, "[w_lgamma.c] enter lgamma 5\n");
                return __kernel_standard(x,x,14); 
                // fprintf(stderr, "[w_lgamma.c] exit lgamma 5\n");
            }
        } else {
	fprintf(stderr, "[w_lgamma.c] enter lgamma 6\n");
            return y;
            // fprintf(stderr, "[w_lgamma.c] exit lgamma 6\n");
        }
	// fprintf(stderr, "[w_lgamma.c] exit lgamma 2\n");
#endif
}

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
    fprintf(stderr, "\n");
    if (argc != 2) {
	fprintf(stderr, "[w_lgamma.c] enter main 2\n");
        printf("Usage: %s <arg>\n", argv[0]);
        return 1;
        // fprintf(stderr, "[w_lgamma.c] exit main 2\n");
    }

    fprintf(stderr, "[w_lgamma.c] enter main 3\n");
    double a0 = atof(argv[1]);

    double r = lgamma(a0);
    printf("%f\n", r);

    return 0;
    // fprintf(stderr, "[w_lgamma.c] exit main 3\n");
}
// Total cost: 0.001180
// Total split cost: 0.000000, input tokens: 0, output tokens: 0, cache read tokens: 0, cache write tokens: 0, split chunks: [(0, 50)]
// Total instrumented cost: 0.001180, input tokens: 2400, output tokens: 484, cache read tokens: 0, cache write tokens: 0
