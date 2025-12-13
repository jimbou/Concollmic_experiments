#include <stdio.h>







#include "fdlibm.h"

#ifdef __STDC__
	double copysign(double x, double y)
#else
	double copysign(x,y)
	double x,y;
#endif
{
	fprintf(stderr, "[s_copysign.c] enter copysign 1\n");
	__HI(x) = (__HI(x)&0x7fffffff)|(__HI(y)&0x80000000);
        return x;
        // fprintf(stderr, "[s_copysign.c] exit copysign 1\n");
}

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
    fprintf(stderr, "\n");
    if (argc != 3) {
        fprintf(stderr, "[s_copysign.c] enter main 2\n");
        printf("Usage: %s <arg> <arg>\n", argv[0]);
        return 1;
        // fprintf(stderr, "[s_copysign.c] exit main 2\n");
    }

    fprintf(stderr, "[s_copysign.c] enter main 3\n");
    double a0 = atof(argv[1]);
    double a1 = atof(argv[2]);

    double r = copysign(a0, a1);
    printf("%f\n", r);

    return 0;
    // fprintf(stderr, "[s_copysign.c] exit main 3\n");
}
// Total cost: 0.000948
// Total split cost: 0.000000, input tokens: 0, output tokens: 0, cache read tokens: 0, cache write tokens: 0, split chunks: [(0, 37)]
// Total instrumented cost: 0.000948, input tokens: 2340, output tokens: 287, cache read tokens: 0, cache write tokens: 0
