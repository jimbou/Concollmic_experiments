





#include "fdlibm.h"
#include <stdio.h>

#ifdef __STDC__
static const double 
#else
static double 
#endif
one =  1.00000000000000000000e+00, 
C1  =  4.16666666666666019037e-02, 
C2  = -1.38888888888741095749e-03, 
C3  =  2.48015872894767294178e-05, 
C4  = -2.75573143513906633035e-07, 
C5  =  2.08757232129817482790e-09, 
C6  = -1.13596475577881948265e-11; 

#ifdef __STDC__
	double __kernel_cos(double x, double y)
#else
	double __kernel_cos(x, y)
	double x,y;
#endif
{
	fprintf(stderr, "[k_cos.c] enter __kernel_cos 1\n");
	double a,hz,z,r,qx;
	int ix;
	ix = __HI(x)&0x7fffffff;	
	// fprintf(stderr, "[k_cos.c] exit __kernel_cos 1\n");
	
	if(ix<0x3e400000) {			
	    fprintf(stderr, "[k_cos.c] enter __kernel_cos 2\n");
	    if(((int)x)==0) {
		fprintf(stderr, "[k_cos.c] enter __kernel_cos 3\n");
		return one;
		// fprintf(stderr, "[k_cos.c] exit __kernel_cos 3\n");
	    }
	    // fprintf(stderr, "[k_cos.c] exit __kernel_cos 2\n");
	}
	fprintf(stderr, "[k_cos.c] enter __kernel_cos 4\n");
	z  = x*x;
	r  = z*(C1+z*(C2+z*(C3+z*(C4+z*(C5+z*C6)))));
	// fprintf(stderr, "[k_cos.c] exit __kernel_cos 4\n");
	
	if(ix < 0x3FD33333) 			 
	{
	    fprintf(stderr, "[k_cos.c] enter __kernel_cos 5\n");
	    return one - (0.5*z - (z*r - x*y));
	    // fprintf(stderr, "[k_cos.c] exit __kernel_cos 5\n");
	}
	else {
	    fprintf(stderr, "\n");
	    if(ix > 0x3fe90000) {		
		fprintf(stderr, "[k_cos.c] enter __kernel_cos 7\n");
		qx = 0.28125;
		// fprintf(stderr, "[k_cos.c] exit __kernel_cos 7\n");
	    } else {
		fprintf(stderr, "[k_cos.c] enter __kernel_cos 8\n");
	        __HI(qx) = ix-0x00200000;	
	        __LO(qx) = 0;
		// fprintf(stderr, "[k_cos.c] exit __kernel_cos 8\n");
	    }
	    fprintf(stderr, "[k_cos.c] enter __kernel_cos 9\n");
	    hz = 0.5*z-qx;
	    a  = one-qx;
	    return a - (hz - (z*r-x*y));
	    // fprintf(stderr, "[k_cos.c] exit __kernel_cos 9\n");
	}
}

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
    fprintf(stderr, "[k_cos.c] enter main 1\n");
    if (argc != 3) {
	fprintf(stderr, "[k_cos.c] enter main 2\n");
        printf("Usage: %s <arg> <arg>\n", argv[0]);
        return 1;
        // fprintf(stderr, "[k_cos.c] exit main 2\n");
    }
    fprintf(stderr, "[k_cos.c] enter main 3\n");
    double a0 = atof(argv[1]);
    double a1 = atof(argv[2]);

    double r = __kernel_cos(a0, a1);
    printf("%f\n", r);
    // fprintf(stderr, "[k_cos.c] exit main 3\n");

    return 0;
    // fprintf(stderr, "[k_cos.c] exit main 1\n");
}
// Total cost: 0.009256
// Total split cost: 0.000000, input tokens: 0, output tokens: 0, cache read tokens: 0, cache write tokens: 0, split chunks: [(0, 69)]
// Total instrumented cost: 0.009256, input tokens: 17252, output tokens: 4180, cache read tokens: 0, cache write tokens: 0
