





#include "fdlibm.h"

#ifdef __STDC__
	double copysign(double x, double y)
#else
	double copysign(x,y)
	double x,y;
#endif
{
	__HI(x) = (__HI(x)&0x7fffffff)|(__HI(y)&0x80000000);
        return x;
}

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {

    if (argc != 3) {
        printf("Usage: %s <arg> <arg>\n", argv[0]);
        return 1;
    }

    double a0 = atof(argv[1]);
    double a1 = atof(argv[2]);

    double r = copysign(a0, a1);
    printf("%f\n", r);

    return 0;
}
