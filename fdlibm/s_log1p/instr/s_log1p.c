





#include "fdlibm.h"
#include <stdio.h>

#ifdef __STDC__
static const double
#else
static double
#endif
ln2_hi  =  6.93147180369123816490e-01,	
ln2_lo  =  1.90821492927058770002e-10,	
two54   =  1.80143985094819840000e+16,  
Lp1 = 6.666666666666735130e-01,  
Lp2 = 3.999999999940941908e-01,  
Lp3 = 2.857142874366239149e-01,  
Lp4 = 2.222219843214978396e-01,  
Lp5 = 1.818357216161805012e-01,  
Lp6 = 1.531383769920937332e-01,  
Lp7 = 1.479819860511658591e-01;  

static double zero = 0.0;

#ifdef __STDC__
	double log1p(double x)
#else
	double log1p(x)
	double x;
#endif
{
	fprintf(stderr, "[s_log1p.c] enter log1p 1\n");
	double hfsq,f,c,s,z,R,u;
	int k,hx,hu,ax;

	hx = __HI(x);		
	ax = hx&0x7fffffff;

	k = 1;
	if (hx < 0x3FDA827A) {			
	    if(ax>=0x3ff00000) {		
		fprintf(stderr, "[s_log1p.c] enter log1p 2\n");
		if(x==-1.0) return -two54/zero; 
		else return (x-x)/(x-x);	
		// fprintf(stderr, "[s_log1p.c] exit log1p 2\n");
	    }
	    if(ax<0x3e200000) {			
		fprintf(stderr, "[s_log1p.c] enter log1p 3\n");
		if(two54+x>zero			
	            &&ax<0x3c900000) 		
		    return x;
		else
		    return x - x*x*0.5;
		// fprintf(stderr, "[s_log1p.c] exit log1p 3\n");
	    }
	    fprintf(stderr, "[s_log1p.c] enter log1p 4\n");
	    if(hx>0||hx<=((int)0xbfd2bec3)) {
		k=0;f=x;hu=1;}	
	    // fprintf(stderr, "[s_log1p.c] exit log1p 4\n");
	} 
	fprintf(stderr, "[s_log1p.c] enter log1p 5\n");
	if (hx >= 0x7ff00000) return x+x;
	// fprintf(stderr, "[s_log1p.c] exit log1p 5\n");
	fprintf(stderr, "[s_log1p.c] enter log1p 6\n");
	if(k!=0) {
	    fprintf(stderr, "[s_log1p.c] enter log1p 7\n");
	    if(hx<0x43400000) {
		fprintf(stderr, "[s_log1p.c] enter log1p 8\n");
		u  = 1.0+x; 
	        hu = __HI(u);		
	        k  = (hu>>20)-1023;
	        c  = (k>0)? 1.0-(u-x):x-(u-1.0);
		c /= u;
		// fprintf(stderr, "[s_log1p.c] exit log1p 8\n");
	    } else {
		fprintf(stderr, "[s_log1p.c] enter log1p 9\n");
		u  = x;
	        hu = __HI(u);		
	        k  = (hu>>20)-1023;
		c  = 0;
		// fprintf(stderr, "[s_log1p.c] exit log1p 9\n");
	    }
	    fprintf(stderr, "[s_log1p.c] enter log1p 10\n");
	    hu &= 0x000fffff;
	    if(hu<0x6a09e) {
		fprintf(stderr, "[s_log1p.c] enter log1p 11\n");
	        __HI(u) = hu|0x3ff00000;	
		// fprintf(stderr, "[s_log1p.c] exit log1p 11\n");
	    } else {
		fprintf(stderr, "[s_log1p.c] enter log1p 12\n");
	        k += 1; 
	        __HI(u) = hu|0x3fe00000;	
	        hu = (0x00100000-hu)>>2;
		// fprintf(stderr, "[s_log1p.c] exit log1p 12\n");
	    }
	    fprintf(stderr, "[s_log1p.c] enter log1p 13\n");
	    f = u-1.0;
	    // fprintf(stderr, "[s_log1p.c] exit log1p 13\n");
	    // fprintf(stderr, "[s_log1p.c] exit log1p 10\n");
	}
	// fprintf(stderr, "[s_log1p.c] exit log1p 7\n");
	fprintf(stderr, "[s_log1p.c] enter log1p 14\n");
	hfsq=0.5*f*f;
	if(hu==0) {	
	    fprintf(stderr, "[s_log1p.c] enter log1p 15\n");
	    if(f==zero) if(k==0) return zero;  
			else {c += k*ln2_lo; return k*ln2_hi+c;}
	    fprintf(stderr, "[s_log1p.c] enter log1p 16\n");
	    R = hfsq*(1.0-0.66666666666666666*f);
	    if(k==0) return f-R; else
	    	     return k*ln2_hi-((R-(k*ln2_lo+c))-f);
	    // fprintf(stderr, "[s_log1p.c] exit log1p 16\n");
	    // fprintf(stderr, "[s_log1p.c] exit log1p 15\n");
	}
	fprintf(stderr, "[s_log1p.c] enter log1p 17\n");
 	s = f/(2.0+f); 
	z = s*s;
	R = z*(Lp1+z*(Lp2+z*(Lp3+z*(Lp4+z*(Lp5+z*(Lp6+z*Lp7))))));
	if(k==0) return f-(hfsq-s*(hfsq+R)); else
		 return k*ln2_hi-((hfsq-(s*(hfsq+R)+(k*ln2_lo+c)))-f);
	// fprintf(stderr, "[s_log1p.c] exit log1p 17\n");
	// fprintf(stderr, "[s_log1p.c] exit log1p 14\n");
	// fprintf(stderr, "[s_log1p.c] exit log1p 6\n");
	// fprintf(stderr, "[s_log1p.c] exit log1p 1\n");
}

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
    fprintf(stderr, "[s_log1p.c] enter main 1\n");
    if (argc != 2) {
        fprintf(stderr, "[s_log1p.c] enter main 2\n");
        printf("Usage: %s <arg>\n", argv[0]);
        return 1;
        // fprintf(stderr, "[s_log1p.c] exit main 2\n");
    }

    fprintf(stderr, "[s_log1p.c] enter main 3\n");
    double a0 = atof(argv[1]);

    double r = log1p(a0);
    printf("%f\n", r);

    return 0;
    // fprintf(stderr, "[s_log1p.c] exit main 3\n");
    // fprintf(stderr, "[s_log1p.c] exit main 1\n");
}
// Total cost: 0.026227
// Total split cost: 0.000000, input tokens: 0, output tokens: 0, cache read tokens: 0, cache write tokens: 0, split chunks: [(0, 111)]
// Total instrumented cost: 0.026227, input tokens: 40798, output tokens: 13829, cache read tokens: 0, cache write tokens: 0
