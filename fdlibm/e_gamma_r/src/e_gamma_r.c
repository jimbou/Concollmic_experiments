





#include "fdlibm.h"

#ifdef __STDC__
	double __ieee754_gamma_r(double x, int *signgamp)
#else
	double __ieee754_gamma_r(x,signgamp)
	double x; int *signgamp;
#endif
{
	return __ieee754_lgamma_r(x,signgamp);
}


#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
    if (argc != 2) {
        printf("Usage: %s <x>\n", argv[0]);
        return 1;
    }

    double x = atof(argv[1]);
    int sign = 0;

    double r = __ieee754_gamma_r(x, &sign);

    printf("result = %f\n", r);
    printf("signgamp = %d\n", sign);

    return 0;
}
