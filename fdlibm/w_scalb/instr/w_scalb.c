





#include "fdlibm.h"

#include <errno.h>
#include <stdio.h>

#ifdef __STDC__
#ifdef _SCALB_INT
	double scalb(double x, int fn)		
#else
	double scalb(double x, double fn)	
#endif
#else
	double scalb(x,fn)			
#ifdef _SCALB_INT
	double x; int fn;
#else
	double x,fn;
#endif
#endif
{
	fprintf(stderr, "[w_scalb.c] enter scalb 1\n");
#ifdef _IEEE_LIBM
	return __ieee754_scalb(x,fn);
	// fprintf(stderr, "[w_scalb.c] exit scalb 1\n");
#else
	double z;
	z = __ieee754_scalb(x,fn);
	if(_LIB_VERSION == _IEEE_) {
		fprintf(stderr, "[w_scalb.c] enter scalb 2\n");
		return z;
		// fprintf(stderr, "[w_scalb.c] exit scalb 2\n");
	}
	if(!(finite(z)||isnan(z))&&finite(x)) {
		fprintf(stderr, "[w_scalb.c] enter scalb 3\n");
		return __kernel_standard(x,(double)fn,32); 
		// fprintf(stderr, "[w_scalb.c] exit scalb 3\n");
	}
	if(z==0.0&&z!=x) {
		fprintf(stderr, "[w_scalb.c] enter scalb 4\n");
		return __kernel_standard(x,(double)fn,33); 
		// fprintf(stderr, "[w_scalb.c] exit scalb 4\n");
	} 
#ifndef _SCALB_INT
	if(!finite(fn)) {
		fprintf(stderr, "[w_scalb.c] enter scalb 5\n");
		errno = ERANGE;
		// fprintf(stderr, "[w_scalb.c] exit scalb 5\n");
	}
#endif
	fprintf(stderr, "[w_scalb.c] enter scalb 6\n");
	return z;
	// fprintf(stderr, "[w_scalb.c] exit scalb 6\n");
#endif 
}

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
	fprintf(stderr, "\n");
    if (argc != 3) {
		fprintf(stderr, "[w_scalb.c] enter main 2\n");
        printf("Usage: %s <arg> <arg>\n", argv[0]);
        return 1;
		// fprintf(stderr, "[w_scalb.c] exit main 2\n");
    }

    fprintf(stderr, "[w_scalb.c] enter main 3\n");
    double a0 = atof(argv[1]);
    double a1 = atof(argv[2]);

    double r = scalb(a0, a1);
    printf("%f\n", r);

    return 0;
	// fprintf(stderr, "[w_scalb.c] exit main 3\n");
}
// Total cost: 0.001314
// Total split cost: 0.000000, input tokens: 0, output tokens: 0, cache read tokens: 0, cache write tokens: 0, split chunks: [(0, 62)]
// Total instrumented cost: 0.001314, input tokens: 2488, output tokens: 584, cache read tokens: 0, cache write tokens: 0
