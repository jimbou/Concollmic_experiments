





#include "fdlibm.h"
#include <stdio.h>


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
        if(_LIB_VERSION == _IEEE_) {
            return y;
        }
        if(!finite(y)&&finite(x)) {
            if(floor(x)==x&&x<=0.0) {
                return __kernel_standard(x,x,15); 
            } else {
                return __kernel_standard(x,x,14); 
            }
        } else {
            return y;
        }
#endif
}             

#include <stdio.h>
#include <stdlib.h>

