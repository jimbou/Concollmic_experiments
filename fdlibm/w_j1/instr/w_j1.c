





#include "fdlibm.h"
#include "stdio.h"

#ifdef __STDC__
	double j1(double x)		
#else
	double j1(x)			
	double x;
#endif
{
#ifdef _IEEE_LIBM
	fprintf(stderr, "[w_j1.c] enter j1 1\n");
	return __ieee754_j1(x);
	// fprintf(stderr, "[w_j1.c] exit j1 1\n");
#else
	fprintf(stderr, "[w_j1.c] enter j1 2\n");
	double z;
	z = __ieee754_j1(x);
	// fprintf(stderr, "[w_j1.c] exit j1 2\n");
	if(_LIB_VERSION == _IEEE_ || isnan(x) )
	{
		fprintf(stderr, "[w_j1.c] enter j1 3\n");
		return z;
		// fprintf(stderr, "[w_j1.c] exit j1 3\n");
	}
	if(fabs(x)>X_TLOSS) {
		fprintf(stderr, "[w_j1.c] enter j1 4\n");
	        return __kernel_standard(x,x,36); 
		// fprintf(stderr, "[w_j1.c] exit j1 4\n");
	} else
	{
		fprintf(stderr, "[w_j1.c] enter j1 5\n");
	    return z;
		// fprintf(stderr, "[w_j1.c] exit j1 5\n");
	}
#endif
}

#ifdef __STDC__
	double y1(double x)		
#else
	double y1(x)			
	double x;
#endif
{
#ifdef _IEEE_LIBM
	fprintf(stderr, "[w_j1.c] enter y1 1\n");
	return __ieee754_y1(x);
	// fprintf(stderr, "[w_j1.c] exit y1 1\n");
#else
	fprintf(stderr, "[w_j1.c] enter y1 2\n");
	double z;
	z = __ieee754_y1(x);
	// fprintf(stderr, "[w_j1.c] exit y1 2\n");
	if(_LIB_VERSION == _IEEE_ || isnan(x) )
	{
		fprintf(stderr, "[w_j1.c] enter y1 3\n");
		return z;
		// fprintf(stderr, "[w_j1.c] exit y1 3\n");
	}
        if(x <= 0.0){
                if(x==0.0)
                {
                    fprintf(stderr, "[w_j1.c] enter y1 4\n");
                    
                    return __kernel_standard(x,x,10);
                    // fprintf(stderr, "[w_j1.c] exit y1 4\n");
                }
                else
                {
                    fprintf(stderr, "[w_j1.c] enter y1 5\n");
                    
                    return __kernel_standard(x,x,11);
                    // fprintf(stderr, "[w_j1.c] exit y1 5\n");
                }
        }
	if(x>X_TLOSS) {
		fprintf(stderr, "[w_j1.c] enter y1 6\n");
	        return __kernel_standard(x,x,37); 
		// fprintf(stderr, "[w_j1.c] exit y1 6\n");
	} else
	{
		fprintf(stderr, "[w_j1.c] enter y1 7\n");
	    return z;
		// fprintf(stderr, "[w_j1.c] exit y1 7\n");
	}
#endif
}

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
	fprintf(stderr, "\n");

    if (argc != 2) {
		fprintf(stderr, "[w_j1.c] enter main 2\n");
        printf("Usage: %s <arg>\n", argv[0]);
        return 1;
		// fprintf(stderr, "[w_j1.c] exit main 2\n");
    }

	fprintf(stderr, "[w_j1.c] enter main 3\n");
    double a0 = atof(argv[1]);

    double r = j1(a0);
    printf("%f\n", r);

    return 0;
	// fprintf(stderr, "[w_j1.c] exit main 3\n");
}
// Total cost: 0.001650
// Total split cost: 0.000000, input tokens: 0, output tokens: 0, cache read tokens: 0, cache write tokens: 0, split chunks: [(0, 73)]
// Total instrumented cost: 0.001650, input tokens: 2518, output tokens: 882, cache read tokens: 0, cache write tokens: 0
