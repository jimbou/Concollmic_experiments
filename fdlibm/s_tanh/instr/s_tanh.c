





#include "fdlibm.h"
#include <stdio.h>

#ifdef __STDC__
static const double one=1.0, two=2.0, tiny = 1.0e-300;
#else
static double one=1.0, two=2.0, tiny = 1.0e-300;
#endif

#ifdef __STDC__
	double tanh(double x)
#else
	double tanh(x)
	double x;
#endif
{
	fprintf(stderr, "[s_tanh.c] enter tanh 1\n");
	double t,z;
	int jx,ix;

    
	jx = __HI(x);
	ix = jx&0x7fffffff;
	// fprintf(stderr, "[s_tanh.c] exit tanh 1\n");

    
	if(ix>=0x7ff00000) { 
	    fprintf(stderr, "[s_tanh.c] enter tanh 2\n");
	    if (jx>=0) {
		fprintf(stderr, "[s_tanh.c] enter tanh 3\n");
		return one/x+one;
		// fprintf(stderr, "[s_tanh.c] exit tanh 3\n");
	    } else {
		fprintf(stderr, "[s_tanh.c] enter tanh 4\n");
		return one/x-one;
		// fprintf(stderr, "[s_tanh.c] exit tanh 4\n");
	    }
	    // fprintf(stderr, "[s_tanh.c] exit tanh 2\n");
	}

    
	if (ix < 0x40360000) {		
	    fprintf(stderr, "[s_tanh.c] enter tanh 5\n");
	    if (ix<0x3c800000) {		
		fprintf(stderr, "[s_tanh.c] enter tanh 6\n");
		return x*(one+x);
		// fprintf(stderr, "[s_tanh.c] exit tanh 6\n");
	    }
	    if (ix>=0x3ff00000) {	
		fprintf(stderr, "[s_tanh.c] enter tanh 7\n");
		t = expm1(two*fabs(x));
		z = one - two/(t+two);
		// fprintf(stderr, "[s_tanh.c] exit tanh 7\n");
	    } else {
		fprintf(stderr, "[s_tanh.c] enter tanh 8\n");
	        t = expm1(-two*fabs(x));
	        z= -t/(t+two);
		// fprintf(stderr, "[s_tanh.c] exit tanh 8\n");
	    }
	    // fprintf(stderr, "[s_tanh.c] exit tanh 5\n");
    
	} else {
	    fprintf(stderr, "[s_tanh.c] enter tanh 9\n");
	    z = one - tiny;		
	    // fprintf(stderr, "[s_tanh.c] exit tanh 9\n");
	}
	fprintf(stderr, "[s_tanh.c] enter tanh 10\n");
	return (jx>=0)? z: -z;
	// fprintf(stderr, "[s_tanh.c] exit tanh 10\n");
}

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
    fprintf(stderr, "\n");
    if (argc != 2) {
        fprintf(stderr, "[s_tanh.c] enter main 2\n");
        printf("Usage: %s <arg>\n", argv[0]);
        return 1;
        // fprintf(stderr, "[s_tanh.c] exit main 2\n");
    }

    fprintf(stderr, "[s_tanh.c] enter main 3\n");
    double a0 = atof(argv[1]);

    double r = tanh(a0);
    printf("%f\n", r);

    return 0;
    // fprintf(stderr, "[s_tanh.c] exit main 3\n");
}
// Total cost: 0.001503
// Total split cost: 0.000000, input tokens: 0, output tokens: 0, cache read tokens: 0, cache write tokens: 0, split chunks: [(0, 69)]
// Total instrumented cost: 0.001503, input tokens: 2537, output tokens: 744, cache read tokens: 0, cache write tokens: 0
