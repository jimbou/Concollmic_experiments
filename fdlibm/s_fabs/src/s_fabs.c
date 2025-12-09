





#include "fdlibm.h"

#ifdef __STDC__
	double fabs(double x)
#else
	double fabs(x)
	double x;
#endif
{
	__HI(x) &= 0x7fffffff;
        return x;
}

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {

    if (argc != 2) {
        printf("Usage: %s <arg>\n", argv[0]);
        return 1;
    }

    double a0 = atof(argv[1]);

    double r = fabs(a0);
    printf("%f\n", r);

    return 0;
}
