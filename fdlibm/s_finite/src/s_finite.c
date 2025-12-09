





#include "fdlibm.h"

#ifdef __STDC__
	int finite(double x)
#else
	int finite(x)
	double x;
#endif
{
	int hx; 
	hx = __HI(x);
	return  (unsigned)((hx&0x7fffffff)-0x7ff00000)>>31;
}

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {

    if (argc != 2) {
        printf("Usage: %s <arg>\n", argv[0]);
        return 1;
    }

    double a0 = atof(argv[1]);

    int r = finite(a0);
    printf("%f\n", r);

    return 0;
}
