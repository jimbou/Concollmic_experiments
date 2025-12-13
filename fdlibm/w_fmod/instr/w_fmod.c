#include <stdio.h>
#include "fdlibm.h"


#ifdef __STDC__
	double fmod(double x, double y)	
#else
	double fmod(x,y)		
	double x,y;
#endif
{
#ifdef _IEEE_LIBM
	fprintf(stderr, "[w_fmod.c] enter fmod 1\n");
	return __ieee754_fmod(x,y);
	// fprintf(stderr, "[w_fmod.c] exit fmod 1\n");
#else
	fprintf(stderr, "[w_fmod.c] enter fmod 2\n");
	double z;
	z = __ieee754_fmod(x,y);
	if(_LIB_VERSION == _IEEE_ ||isnan(y)||isnan(x)) {
		fprintf(stderr, "[w_fmod.c] enter fmod 3\n");
		return z;
		// fprintf(stderr, "[w_fmod.c] exit fmod 3\n");
	}
	if(y==0.0) {
		fprintf(stderr, "[w_fmod.c] enter fmod 4\n");
	        return __kernel_standard(x,y,27); 
		// fprintf(stderr, "[w_fmod.c] exit fmod 4\n");
	} else {
		fprintf(stderr, "[w_fmod.c] enter fmod 5\n");
	    return z;
		// fprintf(stderr, "[w_fmod.c] exit fmod 5\n");
	}
	// fprintf(stderr, "[w_fmod.c] exit fmod 2\n");
#endif
}

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
	fprintf(stderr, "\n");
    if (argc != 3) {
		fprintf(stderr, "[w_fmod.c] enter main 2\n");
        printf("Usage: %s <arg> <arg>\n", argv[0]);
        return 1;
		// fprintf(stderr, "[w_fmod.c] exit main 2\n");
    }

	fprintf(stderr, "[w_fmod.c] enter main 3\n");
    double a0 = atof(argv[1]);
    double a1 = atof(argv[2]);

    double r = fmod(a0, a1);
    printf("%f\n", r);

    return 0;
	// fprintf(stderr, "[w_fmod.c] exit main 3\n");
}
// Total cost: 0.001150
// Total split cost: 0.000000, input tokens: 0, output tokens: 0, cache read tokens: 0, cache write tokens: 0, split chunks: [(0, 47)]
// Total instrumented cost: 0.001150, input tokens: 2389, output tokens: 459, cache read tokens: 0, cache write tokens: 0
