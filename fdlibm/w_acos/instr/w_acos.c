





#include "fdlibm.h"
#include "stdio.h"


#ifdef __STDC__
	double acos(double x)		
#else
	double acos(x)			
	double x;
#endif
{
#ifdef _IEEE_LIBM
	fprintf(stderr, "[w_acos.c] enter acos 1\n");
	return __ieee754_acos(x);
	// fprintf(stderr, "[w_acos.c] exit acos 1\n");
#else
	fprintf(stderr, "[w_acos.c] enter acos 2\n");
	double z;
	z = __ieee754_acos(x);
	// fprintf(stderr, "[w_acos.c] exit acos 2\n");
	if(_LIB_VERSION == _IEEE_ || isnan(x)) {
		fprintf(stderr, "[w_acos.c] enter acos 3\n");
		return z;
		// fprintf(stderr, "[w_acos.c] exit acos 3\n");
	}
	if(fabs(x)>1.0) {
		fprintf(stderr, "[w_acos.c] enter acos 4\n");
	        return __kernel_standard(x,x,1); 
		// fprintf(stderr, "[w_acos.c] exit acos 4\n");
	} else {
		fprintf(stderr, "[w_acos.c] enter acos 5\n");
	    return z;
		// fprintf(stderr, "[w_acos.c] exit acos 5\n");
	}
#endif
}

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
	fprintf(stderr, "\n");

    if (argc != 2) {
		fprintf(stderr, "[w_acos.c] enter main 2\n");
        printf("Usage: %s <arg>\n", argv[0]);
        return 1;
		// fprintf(stderr, "[w_acos.c] exit main 2\n");
    }

	fprintf(stderr, "[w_acos.c] enter main 3\n");
    double a0 = atof(argv[1]);

    double r = acos(a0);
    printf("%f\n", r);

    return 0;
	// fprintf(stderr, "[w_acos.c] exit main 3\n");
}
// Total cost: 0.001186
// Total split cost: 0.000000, input tokens: 0, output tokens: 0, cache read tokens: 0, cache write tokens: 0, split chunks: [(0, 46)]
// Total instrumented cost: 0.001186, input tokens: 2362, output tokens: 498, cache read tokens: 0, cache write tokens: 0
