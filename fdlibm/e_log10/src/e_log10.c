





#include "fdlibm.h"

#ifdef __STDC__
static const double
#else
static double
#endif
two54      =  1.80143985094819840000e+16, 
ivln10     =  4.34294481903251816668e-01, 
log10_2hi  =  3.01029995663611771306e-01, 
log10_2lo  =  3.69423907715893078616e-13; 

static double zero   =  0.0;

#ifdef __STDC__
	double __ieee754_log10(double x)
#else
	double __ieee754_log10(x)
	double x;
#endif
{
	double y,z;
	int i,k,hx;
	unsigned lx;

	hx = __HI(x);	
	lx = __LO(x);	

        k=0;
        if (hx < 0x00100000) {                  
            if (((hx&0x7fffffff)|lx)==0)
                return -two54/zero;             
            if (hx<0) return (x-x)/zero;        
            k -= 54; x *= two54; 
            hx = __HI(x);                
        }
	if (hx >= 0x7ff00000) return x+x;
	k += (hx>>20)-1023;
	i  = ((unsigned)k&0x80000000)>>31;
        hx = (hx&0x000fffff)|((0x3ff-i)<<20);
        y  = (double)(k+i);
        __HI(x) = hx;
	z  = y*log10_2lo + ivln10*__ieee754_log(x);
	return  z+y*log10_2hi;
}

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {

    if (argc != 2) {
        printf("Usage: %s <arg>\n", argv[0]);
        return 1;
    }

    double a0 = atof(argv[1]);

    double r = __ieee754_log10(a0);
    printf("%f\n", r);

    return 0;
}
