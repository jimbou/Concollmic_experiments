





#include "fdlibm.h"
#include <stdio.h>

extern int signgam;

#ifdef __STDC__
	double gamma(double x)
#else
	double gamma(x)
	double x;
#endif
{
#ifdef _IEEE_LIBM
	return __ieee754_gamma_r(x,&signgam);
#else
        double y;
        y = __ieee754_gamma_r(x,&signgam);
        if(_LIB_VERSION == _IEEE_) {
	return y;
}
        if(!finite(y)&&finite(x)) {
            if(floor(x)==x&&x<=0.0) {
                return __kernel_standard(x,x,41); 
}
            else {
                return __kernel_standard(x,x,40); 
}
} else {
            return y;
}
#endif
}

#include <stdio.h>
#include <stdlib.h>

