





#include "fdlibm.h"
#include <stdio.h>

#ifdef __STDC__
static const double
#else
static double
#endif
one		= 1.0,
huge		= 1.0e+300,
tiny		= 1.0e-300,
o_threshold	= 7.09782712893383973096e+02,
ln2_hi		= 6.93147180369123816490e-01,
ln2_lo		= 1.90821492927058770002e-10,
invln2		= 1.44269504088896338700e+00,
	
Q1  =  -3.33333333333331316428e-02, 
Q2  =   1.58730158725481460165e-03, 
Q3  =  -7.93650757867487942473e-05, 
Q4  =   4.00821782732936239552e-06, 
Q5  =  -2.01099218183624371326e-07; 

#ifdef __STDC__
	double expm1(double x)
#else
	double expm1(x)
	double x;
#endif
{
	fprintf(stderr, "[s_expm1.c] enter expm1 1\n");
	double y,hi,lo,c,t,e,hxs,hfx,r1;
	int k,xsb;
	unsigned hx;

	hx  = __HI(x);	
	xsb = hx&0x80000000;		
	if(xsb==0) y=x; else y= -x;	
	hx &= 0x7fffffff;		
	// fprintf(stderr, "[s_expm1.c] exit expm1 1\n");

    
	if(hx >= 0x4043687A) {			
	    if(hx >= 0x40862E42) {		
                if(hx>=0x7ff00000) {
		    fprintf(stderr, "[s_expm1.c] enter expm1 2\n");
		    if(((hx&0xfffff)|__LO(x))!=0) 
		         return x+x; 	 
		    else return (xsb==0)? x:-1.0;
		    // fprintf(stderr, "[s_expm1.c] exit expm1 2\n");
	        }
	        fprintf(stderr, "[s_expm1.c] enter expm1 3\n");
	        if(x > o_threshold) return huge*huge; 
	        // fprintf(stderr, "[s_expm1.c] exit expm1 3\n");
	    }
	    fprintf(stderr, "[s_expm1.c] enter expm1 4\n");
	    if(xsb!=0) { 
		fprintf(stderr, "[s_expm1.c] enter expm1 5\n");
		if(x+tiny<0.0)		
		return tiny-one;	
		// fprintf(stderr, "[s_expm1.c] exit expm1 5\n");
	    }
	    // fprintf(stderr, "[s_expm1.c] exit expm1 4\n");
	}

    
	if(hx > 0x3fd62e42) {		 
	    if(hx < 0x3FF0A2B2) {	
		fprintf(stderr, "[s_expm1.c] enter expm1 6\n");
		if(xsb==0)
		    {hi = x - ln2_hi; lo =  ln2_lo;  k =  1;}
		else
		    {hi = x + ln2_hi; lo = -ln2_lo;  k = -1;}
		// fprintf(stderr, "[s_expm1.c] exit expm1 6\n");
	    } else {
		fprintf(stderr, "[s_expm1.c] enter expm1 7\n");
		k  = invln2*x+((xsb==0)?0.5:-0.5);
		t  = k;
		hi = x - t*ln2_hi;	
		lo = t*ln2_lo;
		// fprintf(stderr, "[s_expm1.c] exit expm1 7\n");
	    }
	    fprintf(stderr, "[s_expm1.c] enter expm1 8\n");
	    x  = hi - lo;
	    c  = (hi-x)-lo;
	    // fprintf(stderr, "[s_expm1.c] exit expm1 8\n");
	} 
	else if(hx < 0x3c900000) {  	
	    fprintf(stderr, "[s_expm1.c] enter expm1 9\n");
	    t = huge+x;	
	    return x - (t-(huge+x));	
	    // fprintf(stderr, "[s_expm1.c] exit expm1 9\n");
	}
	else {
	    fprintf(stderr, "[s_expm1.c] enter expm1 10\n");
	    k = 0;
	    // fprintf(stderr, "[s_expm1.c] exit expm1 10\n");
	}

    
	fprintf(stderr, "\n");
	hfx = 0.5*x;
	hxs = x*hfx;
	r1 = one+hxs*(Q1+hxs*(Q2+hxs*(Q3+hxs*(Q4+hxs*Q5))));
	t  = 3.0-r1*hfx;
	e  = hxs*((r1-t)/(6.0 - x*t));
	if(k==0) return x - (x*e-hxs);		
	else {
	    fprintf(stderr, "\n");
	    e  = (x*(e-c)-c);
	    e -= hxs;
	    if(k== -1) return 0.5*(x-e)-0.5;
	    if(k==1) 
	       	if(x < -0.25) return -2.0*(e-(x+0.5));
	       	else 	      return  one+2.0*(x-e);
	    if (k <= -2 || k>56) {   
	        fprintf(stderr, "[s_expm1.c] enter expm1 13\n");
	        y = one-(e-x);
	        __HI(y) += (k<<20);	
	        return y-one;
	        // fprintf(stderr, "[s_expm1.c] exit expm1 13\n");
	    }
	    fprintf(stderr, "[s_expm1.c] enter expm1 14\n");
	    t = one;
	    if(k<20) {
	       	fprintf(stderr, "[s_expm1.c] enter expm1 15\n");
	       	__HI(t) = 0x3ff00000 - (0x200000>>k);  
	       	y = t-(e-x);
	       	__HI(y) += (k<<20);	
	       	// fprintf(stderr, "[s_expm1.c] exit expm1 15\n");
	   } else {
	       	fprintf(stderr, "[s_expm1.c] enter expm1 16\n");
	       	__HI(t)  = ((0x3ff-k)<<20);	
	       	y = x-(e+t);
	       	y += one;
	       	__HI(y) += (k<<20);	
	       	// fprintf(stderr, "[s_expm1.c] exit expm1 16\n");
	    }
	    // fprintf(stderr, "[s_expm1.c] exit expm1 14\n");
	}
	fprintf(stderr, "[s_expm1.c] enter expm1 17\n");
	return y;
	// fprintf(stderr, "[s_expm1.c] exit expm1 17\n");
}

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
    fprintf(stderr, "\n");
    if (argc != 2) {
        fprintf(stderr, "[s_expm1.c] enter main 2\n");
        printf("Usage: %s <arg>\n", argv[0]);
        return 1;
        // fprintf(stderr, "[s_expm1.c] exit main 2\n");
    }

    fprintf(stderr, "[s_expm1.c] enter main 3\n");
    double a0 = atof(argv[1]);
    // fprintf(stderr, "[s_expm1.c] exit main 3\n");

    fprintf(stderr, "[s_expm1.c] enter main 4\n");
    double r = expm1(a0);
    printf("%f\n", r);

    return 0;
    // fprintf(stderr, "[s_expm1.c] exit main 4\n");
}
// Total cost: 0.002847
// Total split cost: 0.000000, input tokens: 0, output tokens: 0, cache read tokens: 0, cache write tokens: 0, split chunks: [(0, 132)]
// Total instrumented cost: 0.002847, input tokens: 3278, output tokens: 1784, cache read tokens: 0, cache write tokens: 0
