





#include "fdlibm.h"

#ifdef __STDC__
	double logb(double x)
#else
	double logb(x)
	double x;
#endif
{
	int lx,ix;
	ix = (__HI(x))&0x7fffffff;	
	lx = __LO(x);			
	if((ix|lx)==0) return -1.0/fabs(x);
	if(ix>=0x7ff00000) return x*x;
	if((ix>>=20)==0) 			
		return -1022.0; 
	else
		return (double) (ix-1023); 
}

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {

    if (argc != 2) {
        printf("Usage: %s <arg>\n", argv[0]);
        return 1;
    }

    double a0 = atof(argv[1]);

    double r = logb(a0);
    printf("%f\n", r);

    return 0;
}
