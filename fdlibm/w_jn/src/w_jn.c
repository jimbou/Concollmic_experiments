





#include "fdlibm.h"

#ifdef __STDC__
	double jn(int n, double x)	
#else
	double jn(n,x)			
	double x; int n;
#endif
{
#ifdef _IEEE_LIBM
	return __ieee754_jn(n,x);
#else
	double z;
	z = __ieee754_jn(n,x);
	if(_LIB_VERSION == _IEEE_ || isnan(x) ) return z;
	if(fabs(x)>X_TLOSS) {
	    return __kernel_standard((double)n,x,38); 
	} else
	    return z;
#endif
}

#ifdef __STDC__
	double yn(int n, double x)	
#else
	double yn(n,x)			
	double x; int n;
#endif
{
#ifdef _IEEE_LIBM
	return __ieee754_yn(n,x);
#else
	double z;
	z = __ieee754_yn(n,x);
	if(_LIB_VERSION == _IEEE_ || isnan(x) ) return z;
        if(x <= 0.0){
                if(x==0.0)
                    
                    return __kernel_standard((double)n,x,12);
                else
                    
                    return __kernel_standard((double)n,x,13);
        }
	if(x>X_TLOSS) {
	    return __kernel_standard((double)n,x,39); 
	} else
	    return z;
#endif
}

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {

    if (argc != 3) {
        printf("Usage: %s <arg> <arg>\n", argv[0]);
        return 1;
    }

    int a0 = atoi(argv[1]);
    double a1 = atof(argv[2]);

    double r = jn(a0, a1);
    printf("%f\n", r);

    return 0;
}
