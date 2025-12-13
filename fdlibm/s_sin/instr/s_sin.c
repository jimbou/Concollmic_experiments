





#include "fdlibm.h"
#include <stdio.h>

#ifdef __STDC__
	double sin(double x)
#else
	double sin(x)
	double x;
#endif
{
	fprintf(stderr, "[s_sin.c] enter sin 1\n");
	double y[2],z=0.0;
	int n, ix;

    
	ix = __HI(x);

    
	ix &= 0x7fffffff;
	if(ix <= 0x3fe921fb) {
		fprintf(stderr, "[s_sin.c] enter sin 2\n");
		return __kernel_sin(x,z,0);
		// fprintf(stderr, "[s_sin.c] exit sin 2\n");
	}

    
	else if (ix>=0x7ff00000) {
		fprintf(stderr, "[s_sin.c] enter sin 3\n");
		return x-x;
		// fprintf(stderr, "[s_sin.c] exit sin 3\n");
	}

    
	else {
		fprintf(stderr, "[s_sin.c] enter sin 4\n");
	    n = __ieee754_rem_pio2(x,y);
	    switch(n&3) {
		case 0: 
			fprintf(stderr, "[s_sin.c] enter sin 5\n");
			return  __kernel_sin(y[0],y[1],1);
			// fprintf(stderr, "[s_sin.c] exit sin 5\n");
		case 1: 
			fprintf(stderr, "[s_sin.c] enter sin 6\n");
			return  __kernel_cos(y[0],y[1]);
			// fprintf(stderr, "[s_sin.c] exit sin 6\n");
		case 2: 
			fprintf(stderr, "[s_sin.c] enter sin 7\n");
			return -__kernel_sin(y[0],y[1],1);
			// fprintf(stderr, "[s_sin.c] exit sin 7\n");
		default:
			fprintf(stderr, "[s_sin.c] enter sin 8\n");
			return -__kernel_cos(y[0],y[1]);
			// fprintf(stderr, "[s_sin.c] exit sin 8\n");
	    }
		// fprintf(stderr, "[s_sin.c] exit sin 4\n");
	}
	// fprintf(stderr, "[s_sin.c] exit sin 1\n");
}

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
	fprintf(stderr, "\n");

    if (argc != 2) {
		fprintf(stderr, "[s_sin.c] enter main 2\n");
        printf("Usage: %s <arg>\n", argv[0]);
        return 1;
		// fprintf(stderr, "[s_sin.c] exit main 2\n");
    }

	fprintf(stderr, "[s_sin.c] enter main 3\n");
    double a0 = atof(argv[1]);

    double r = sin(a0);
    printf("%f\n", r);

    return 0;
	// fprintf(stderr, "[s_sin.c] exit main 3\n");
}
// Total cost: 0.001337
// Total split cost: 0.000000, input tokens: 0, output tokens: 0, cache read tokens: 0, cache write tokens: 0, split chunks: [(0, 58)]
// Total instrumented cost: 0.001337, input tokens: 2459, output tokens: 612, cache read tokens: 0, cache write tokens: 0
