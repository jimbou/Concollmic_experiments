





#include "fdlibm.h"
#include <stdio.h>

#ifdef __STDC__
static const double 
#else
static double 
#endif
one=  1.00000000000000000000e+00, 
pi =  3.14159265358979311600e+00, 
pio2_hi =  1.57079632679489655800e+00, 
pio2_lo =  6.12323399573676603587e-17, 
pS0 =  1.66666666666666657415e-01, 
pS1 = -3.25565818622400915405e-01, 
pS2 =  2.01212532134862925881e-01, 
pS3 = -4.00555345006794114027e-02, 
pS4 =  7.91534994289814532176e-04, 
pS5 =  3.47933107596021167570e-05, 
qS1 = -2.40339491173441421878e+00, 
qS2 =  2.02094576023350569471e+00, 
qS3 = -6.88283971605453293030e-01, 
qS4 =  7.70381505559019352791e-02; 

#ifdef __STDC__
	double __ieee754_acos(double x)
#else
	double __ieee754_acos(x)
	double x;
#endif
{
	fprintf(stderr, "[e_acos.c] enter __ieee754_acos 1\n");
	double z,p,q,r,w,s,c,df;
	int hx,ix;
	hx = __HI(x);
	ix = hx&0x7fffffff;
	// fprintf(stderr, "[e_acos.c] exit __ieee754_acos 1\n");
	if(ix>=0x3ff00000) {	
	    fprintf(stderr, "\n");
	    if(((ix-0x3ff00000)|__LO(x))==0) {	
		fprintf(stderr, "[e_acos.c] enter __ieee754_acos 3\n");
		if(hx>0) {
		    fprintf(stderr, "[e_acos.c] enter __ieee754_acos 4\n");
		    return 0.0;
		    // fprintf(stderr, "[e_acos.c] exit __ieee754_acos 4\n");
		} else {
		    fprintf(stderr, "[e_acos.c] enter __ieee754_acos 5\n");
		    return pi+2.0*pio2_lo;
		    // fprintf(stderr, "[e_acos.c] exit __ieee754_acos 5\n");
		}
		// fprintf(stderr, "[e_acos.c] exit __ieee754_acos 3\n");
	    }
	    fprintf(stderr, "[e_acos.c] enter __ieee754_acos 6\n");
	    return (x-x)/(x-x);
	    // fprintf(stderr, "[e_acos.c] exit __ieee754_acos 6\n");
	}
	if(ix<0x3fe00000) {	
	    fprintf(stderr, "\n");
	    if(ix<=0x3c600000) {
		fprintf(stderr, "[e_acos.c] enter __ieee754_acos 8\n");
		return pio2_hi+pio2_lo;
		// fprintf(stderr, "[e_acos.c] exit __ieee754_acos 8\n");
	    }
	    fprintf(stderr, "[e_acos.c] enter __ieee754_acos 9\n");
	    z = x*x;
	    p = z*(pS0+z*(pS1+z*(pS2+z*(pS3+z*(pS4+z*pS5)))));
	    q = one+z*(qS1+z*(qS2+z*(qS3+z*qS4)));
	    r = p/q;
	    return pio2_hi - (x - (pio2_lo-x*r));
	    // fprintf(stderr, "[e_acos.c] exit __ieee754_acos 9\n");
	} else  if (hx<0) {		
	    fprintf(stderr, "[e_acos.c] enter __ieee754_acos 10\n");
	    z = (one+x)*0.5;
	    p = z*(pS0+z*(pS1+z*(pS2+z*(pS3+z*(pS4+z*pS5)))));
	    q = one+z*(qS1+z*(qS2+z*(qS3+z*qS4)));
	    s = sqrt(z);
	    r = p/q;
	    w = r*s-pio2_lo;
	    return pi - 2.0*(s+w);
	    // fprintf(stderr, "[e_acos.c] exit __ieee754_acos 10\n");
	} else {			
	    fprintf(stderr, "[e_acos.c] enter __ieee754_acos 11\n");
	    z = (one-x)*0.5;
	    s = sqrt(z);
	    df = s;
	    __LO(df) = 0;
	    c  = (z-df*df)/(s+df);
	    p = z*(pS0+z*(pS1+z*(pS2+z*(pS3+z*(pS4+z*pS5)))));
	    q = one+z*(qS1+z*(qS2+z*(qS3+z*qS4)));
	    r = p/q;
	    w = r*s+c;
	    return 2.0*(df+w);
	    // fprintf(stderr, "[e_acos.c] exit __ieee754_acos 11\n");
	}
}

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
    fprintf(stderr, "\n");
    if (argc != 2) {
        fprintf(stderr, "[e_acos.c] enter main 2\n");
        printf("Usage: %s <arg>\n", argv[0]);
        return 1;
        // fprintf(stderr, "[e_acos.c] exit main 2\n");
    }
    fprintf(stderr, "[e_acos.c] enter main 3\n");
    double a0 = atof(argv[1]);

    double r = __ieee754_acos(a0);
    printf("%f\n", r);

    return 0;
    // fprintf(stderr, "[e_acos.c] exit main 3\n");
}
// Total cost: 0.002414
// Total split cost: 0.000000, input tokens: 0, output tokens: 0, cache read tokens: 0, cache write tokens: 0, split chunks: [(0, 92)]
// Total instrumented cost: 0.002414, input tokens: 3066, output tokens: 1442, cache read tokens: 0, cache write tokens: 0
