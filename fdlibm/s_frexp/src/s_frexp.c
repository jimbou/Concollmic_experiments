





#include "fdlibm.h"

#ifdef __STDC__
static const double
#else
static double
#endif
two54 =  1.80143985094819840000e+16; 

#ifdef __STDC__
	double frexp(double x, int *eptr)
#else
	double frexp(x, eptr)
	double x; int *eptr;
#endif
{
	int  hx, ix, lx;
	hx = __HI(x);
	ix = 0x7fffffff&hx;
	lx = __LO(x);
	*eptr = 0;
	if(ix>=0x7ff00000||((ix|lx)==0)) return x;	
	if (ix<0x00100000) {		
	    x *= two54;
	    hx = __HI(x);
	    ix = hx&0x7fffffff;
	    *eptr = -54;
	}
	*eptr += (ix>>20)-1022;
	hx = (hx&0x800fffff)|0x3fe00000;
	__HI(x) = hx;
	return x;
}

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
    if (argc != 2) {
        printf("Usage: %s <x>\n", argv[0]);
        return 1;
    }

    double x = atof(argv[1]);
    int e = 0;

    double r = frexp(x, &e);

    printf("result = %f\n", r);
    printf("exponent = %d\n", e);

    return 0;
}
