





#include "fdlibm.h"
#include <stdio.h>

#ifdef __STDC__
	double tan(double x)
#else
	double tan(x)
	double x;
#endif
{
	fprintf(stderr, "[s_tan.c] enter tan 1\n");
	double y[2],z=0.0;
	int n, ix;

    
	ix = __HI(x);

    
	ix &= 0x7fffffff;
	if(ix <= 0x3fe921fb) {
		fprintf(stderr, "[s_tan.c] enter tan 2\n");
		return __kernel_tan(x,z,1);
		// fprintf(stderr, "[s_tan.c] exit tan 2\n");
	}

    
	else if (ix>=0x7ff00000) {
		fprintf(stderr, "[s_tan.c] enter tan 3\n");
		return x-x;
		// fprintf(stderr, "[s_tan.c] exit tan 3\n");
	}

    
	else {
		fprintf(stderr, "[s_tan.c] enter tan 4\n");
	    n = __ieee754_rem_pio2(x,y);
	    return __kernel_tan(y[0],y[1],1-((n&1)<<1));
		// fprintf(stderr, "[s_tan.c] exit tan 4\n");
	}
	// fprintf(stderr, "[s_tan.c] exit tan 1\n");
}

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
	fprintf(stderr, "\n");
    if (argc != 2) {
		fprintf(stderr, "[s_tan.c] enter main 2\n");
        printf("Usage: %s <arg>\n", argv[0]);
        return 1;
		// fprintf(stderr, "[s_tan.c] exit main 2\n");
    }

	fprintf(stderr, "[s_tan.c] enter main 3\n");
    double a0 = atof(argv[1]);

    double r = tan(a0);
    printf("%f\n", r);

    return 0;
	// fprintf(stderr, "[s_tan.c] exit main 3\n");
}
// Total cost: 0.001125
// Total split cost: 0.000000, input tokens: 0, output tokens: 0, cache read tokens: 0, cache write tokens: 0, split chunks: [(0, 52)]
// Total instrumented cost: 0.001125, input tokens: 2398, output tokens: 434, cache read tokens: 0, cache write tokens: 0
