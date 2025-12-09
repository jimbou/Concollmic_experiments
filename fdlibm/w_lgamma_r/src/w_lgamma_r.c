





#include "fdlibm.h"


#ifdef __STDC__
	double lgamma_r(double x, int *signgamp) 
#else
	double lgamma_r(x,signgamp)              
        double x; int *signgamp;
#endif
{
#ifdef _IEEE_LIBM
	return __ieee754_lgamma_r(x,signgamp);
#else
        double y;
        y = __ieee754_lgamma_r(x,signgamp);
        if(_LIB_VERSION == _IEEE_) return y;
        if(!finite(y)&&finite(x)) {
            if(floor(x)==x&&x<=0.0)
                return __kernel_standard(x,x,15); 
            else
                return __kernel_standard(x,x,14); 
        } else
            return y;
#endif
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

    double r = lgamma_r(x, &sign);

    printf("result = %f\n", r);
    printf("signgamp = %d\n", sign);

    return 0;
}
