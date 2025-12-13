





#include "fdlibm.h"
#include <stdio.h>

#ifdef __STDC__
	double cos(double x)
#else
	double cos(x)
	double x;
#endif
{
	fprintf(stderr, "[s_cos.c] enter cos 1\n");
	double y[2],z=0.0;
	int n, ix;

    
	ix = __HI(x);

    
	ix &= 0x7fffffff;
	if(ix <= 0x3fe921fb) {
		fprintf(stderr, "[s_cos.c] enter cos 2\n");
		return __kernel_cos(x,z);
		// fprintf(stderr, "[s_cos.c] exit cos 2\n");
	}

    
	else if (ix>=0x7ff00000) {
		fprintf(stderr, "[s_cos.c] enter cos 3\n");
		return x-x;
		// fprintf(stderr, "[s_cos.c] exit cos 3\n");
	}

    
	else {
		fprintf(stderr, "[s_cos.c] enter cos 4\n");
	    n = __ieee754_rem_pio2(x,y);
	    switch(n&3) {
		case 0: 
			fprintf(stderr, "[s_cos.c] enter cos 5\n");
			return  __kernel_cos(y[0],y[1]);
			// fprintf(stderr, "[s_cos.c] exit cos 5\n");
		case 1: 
			fprintf(stderr, "[s_cos.c] enter cos 6\n");
			return -__kernel_sin(y[0],y[1],1);
			// fprintf(stderr, "[s_cos.c] exit cos 6\n");
		case 2: 
			fprintf(stderr, "[s_cos.c] enter cos 7\n");
			return -__kernel_cos(y[0],y[1]);
			// fprintf(stderr, "[s_cos.c] exit cos 7\n");
		default:
			fprintf(stderr, "[s_cos.c] enter cos 8\n");
		        return  __kernel_sin(y[0],y[1],1);
			// fprintf(stderr, "[s_cos.c] exit cos 8\n");
	    }
		// fprintf(stderr, "[s_cos.c] exit cos 4\n");
	}
	// fprintf(stderr, "[s_cos.c] exit cos 1\n");
}

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
	fprintf(stderr, "[s_cos.c] enter main 1\n");

    if (argc != 2) {
		fprintf(stderr, "[s_cos.c] enter main 2\n");
        printf("Usage: %s <arg>\n", argv[0]);
        return 1;
		// fprintf(stderr, "[s_cos.c] exit main 2\n");
    }

	fprintf(stderr, "[s_cos.c] enter main 3\n");
    double a0 = atof(argv[1]);

    double r = cos(a0);
    printf("%f\n", r);

    return 0;
	// fprintf(stderr, "[s_cos.c] exit main 3\n");
	// fprintf(stderr, "[s_cos.c] exit main 1\n");
}
// Total cost: 0.003082
// Total split cost: 0.000000, input tokens: 0, output tokens: 0, cache read tokens: 0, cache write tokens: 0, split chunks: [(0, 58)]
// Total instrumented cost: 0.003082, input tokens: 5715, output tokens: 1399, cache read tokens: 0, cache write tokens: 0
