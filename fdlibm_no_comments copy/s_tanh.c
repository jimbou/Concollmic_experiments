





#include "fdlibm.h"
#include <stdio.h>

#ifdef __STDC__
static const double one=1.0, two=2.0, tiny = 1.0e-300;
#else
static double one=1.0, two=2.0, tiny = 1.0e-300;
#endif

#ifdef __STDC__
	double tanh(double x)
#else
	double tanh(x)
	double x;
#endif
{
	double t,z;
	int jx,ix;

    
	jx = __HI(x);
	ix = jx&0x7fffffff;

    
	if(ix>=0x7ff00000) { 
	    if (jx>=0) {
		return one/x+one;
	    } else {
		return one/x-one;
	    }
	}

    
	if (ix < 0x40360000) {		
	    if (ix<0x3c800000) {		
		return x*(one+x);
	    }
	    if (ix>=0x3ff00000) {	
		t = expm1(two*fabs(x));
		z = one - two/(t+two);
	    } else {
	        t = expm1(-two*fabs(x));
	        z= -t/(t+two);
	    }
    
	} else {
	    z = one - tiny;		
	}
	return (jx>=0)? z: -z;
}

#include <stdio.h>
#include <stdlib.h>

