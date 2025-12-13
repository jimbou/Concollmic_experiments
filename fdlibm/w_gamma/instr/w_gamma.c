





#include "fdlibm.h"
#include <stdio.h>

extern int signgam;

#ifdef __STDC__
	double gamma(double x)
#else
	double gamma(x)
	double x;
#endif
{
fprintf(stderr, "[w_gamma.c] enter gamma 1\n");
#ifdef _IEEE_LIBM
fprintf(stderr, "[w_gamma.c] enter gamma 2\n");
	return __ieee754_gamma_r(x,&signgam);
	// fprintf(stderr, "[w_gamma.c] exit gamma 2\n");
#else
fprintf(stderr, "[w_gamma.c] enter gamma 3\n");
        double y;
        y = __ieee754_gamma_r(x,&signgam);
        if(_LIB_VERSION == _IEEE_) {
fprintf(stderr, "[w_gamma.c] enter gamma 4\n");
	return y;
	// fprintf(stderr, "[w_gamma.c] exit gamma 4\n");
}
        if(!finite(y)&&finite(x)) {
fprintf(stderr, "[w_gamma.c] enter gamma 5\n");
            if(floor(x)==x&&x<=0.0) {
fprintf(stderr, "[w_gamma.c] enter gamma 6\n");
                return __kernel_standard(x,x,41); 
		// fprintf(stderr, "[w_gamma.c] exit gamma 6\n");
}
            else {
fprintf(stderr, "[w_gamma.c] enter gamma 7\n");
                return __kernel_standard(x,x,40); 
		// fprintf(stderr, "[w_gamma.c] exit gamma 7\n");
}
	// fprintf(stderr, "[w_gamma.c] exit gamma 5\n");
} else {
fprintf(stderr, "[w_gamma.c] enter gamma 8\n");
            return y;
	// fprintf(stderr, "[w_gamma.c] exit gamma 8\n");
}
	// fprintf(stderr, "[w_gamma.c] exit gamma 3\n");
#endif
// fprintf(stderr, "[w_gamma.c] exit gamma 1\n");
}

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
fprintf(stderr, "\n");
    if (argc != 2) {
fprintf(stderr, "[w_gamma.c] enter main 2\n");
        printf("Usage: %s <arg>\n", argv[0]);
        return 1;
	// fprintf(stderr, "[w_gamma.c] exit main 2\n");
    }

fprintf(stderr, "[w_gamma.c] enter main 3\n");
    double a0 = atof(argv[1]);

    double r = gamma(a0);
    printf("%f\n", r);

    return 0;
	// fprintf(stderr, "[w_gamma.c] exit main 3\n");
}
// Total cost: 0.004525
// Total split cost: 0.000000, input tokens: 0, output tokens: 0, cache read tokens: 0, cache write tokens: 0, split chunks: [(0, 50)]
// Total instrumented cost: 0.004525, input tokens: 9201, output tokens: 1855, cache read tokens: 0, cache write tokens: 0
