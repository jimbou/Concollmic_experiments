





#include "fdlibm.h"
#include <stdio.h>

#ifdef __STDC__
	double __ieee754_hypot(double x, double y)
#else
	double __ieee754_hypot(x,y)
	double x, y;
#endif
{
	fprintf(stderr, "\n");
	double a=x,b=y,t1,t2,y1,y2,w;
	int j,k,ha,hb;

	ha = __HI(x)&0x7fffffff;	
	hb = __HI(y)&0x7fffffff;	
	if(hb > ha) {
	    fprintf(stderr, "[e_hypot.c] enter __ieee754_hypot 2\n");
	    a=y;b=x;j=ha; ha=hb;hb=j;
	    // fprintf(stderr, "[e_hypot.c] exit __ieee754_hypot 2\n");
	} else {
	    fprintf(stderr, "[e_hypot.c] enter __ieee754_hypot 3\n");
	    a=x;b=y;
	    // fprintf(stderr, "[e_hypot.c] exit __ieee754_hypot 3\n");
	}
	__HI(a) = ha;	
	__HI(b) = hb;	
	if((ha-hb)>0x3c00000) {
	    fprintf(stderr, "[e_hypot.c] enter __ieee754_hypot 4\n");
	    return a+b;
	    // fprintf(stderr, "[e_hypot.c] exit __ieee754_hypot 4\n");
	}
	k=0;
	if(ha > 0x5f300000) {	
	   fprintf(stderr, "[e_hypot.c] enter __ieee754_hypot 5\n");
	   if(ha >= 0x7ff00000) {	
	       fprintf(stderr, "[e_hypot.c] enter __ieee754_hypot 6\n");
	       w = a+b;			
	       if(((ha&0xfffff)|__LO(a))==0) {
	           fprintf(stderr, "[e_hypot.c] enter __ieee754_hypot 7\n");
	           w = a;
	           // fprintf(stderr, "[e_hypot.c] exit __ieee754_hypot 7\n");
	       }
	       if(((hb^0x7ff00000)|__LO(b))==0) {
	           fprintf(stderr, "[e_hypot.c] enter __ieee754_hypot 8\n");
	           w = b;
	           // fprintf(stderr, "[e_hypot.c] exit __ieee754_hypot 8\n");
	       }
	       return w;
	       // fprintf(stderr, "[e_hypot.c] exit __ieee754_hypot 6\n");
	   }
	   // fprintf(stderr, "[e_hypot.c] exit __ieee754_hypot 5\n");
	   
	   fprintf(stderr, "[e_hypot.c] enter __ieee754_hypot 9\n");
	   ha -= 0x25800000; hb -= 0x25800000;	k += 600;
	   __HI(a) = ha;
	   __HI(b) = hb;
	   // fprintf(stderr, "[e_hypot.c] exit __ieee754_hypot 9\n");
	}
	if(hb < 0x20b00000) {	
	    fprintf(stderr, "[e_hypot.c] enter __ieee754_hypot 10\n");
	    if(hb <= 0x000fffff) {		
		fprintf(stderr, "[e_hypot.c] enter __ieee754_hypot 11\n");
		if((hb|(__LO(b)))==0) {
		    fprintf(stderr, "[e_hypot.c] enter __ieee754_hypot 12\n");
		    return a;
		    // fprintf(stderr, "[e_hypot.c] exit __ieee754_hypot 12\n");
		}
		t1=0;
		__HI(t1) = 0x7fd00000;	
		b *= t1;
		a *= t1;
		k -= 1022;
		// fprintf(stderr, "[e_hypot.c] exit __ieee754_hypot 11\n");
	    } else {		
	        fprintf(stderr, "[e_hypot.c] enter __ieee754_hypot 13\n");
	        ha += 0x25800000; 	
		hb += 0x25800000;	
		k -= 600;
	   	__HI(a) = ha;
	   	__HI(b) = hb;
		// fprintf(stderr, "[e_hypot.c] exit __ieee754_hypot 13\n");
	    }
	    // fprintf(stderr, "[e_hypot.c] exit __ieee754_hypot 10\n");
	}
    
	fprintf(stderr, "[e_hypot.c] enter __ieee754_hypot 14\n");
	w = a-b;
	if (w>b) {
	    fprintf(stderr, "[e_hypot.c] enter __ieee754_hypot 15\n");
	    t1 = 0;
	    __HI(t1) = ha;
	    t2 = a-t1;
	    w  = sqrt(t1*t1-(b*(-b)-t2*(a+t1)));
	    // fprintf(stderr, "[e_hypot.c] exit __ieee754_hypot 15\n");
	} else {
	    fprintf(stderr, "[e_hypot.c] enter __ieee754_hypot 16\n");
	    a  = a+a;
	    y1 = 0;
	    __HI(y1) = hb;
	    y2 = b - y1;
	    t1 = 0;
	    __HI(t1) = ha+0x00100000;
	    t2 = a - t1;
	    w  = sqrt(t1*y1-(w*(-w)-(t1*y2+t2*b)));
	    // fprintf(stderr, "[e_hypot.c] exit __ieee754_hypot 16\n");
	}
	// fprintf(stderr, "[e_hypot.c] exit __ieee754_hypot 14\n");
	if(k!=0) {
	    fprintf(stderr, "[e_hypot.c] enter __ieee754_hypot 17\n");
	    t1 = 1.0;
	    __HI(t1) += (k<<20);
	    return t1*w;
	    // fprintf(stderr, "[e_hypot.c] exit __ieee754_hypot 17\n");
	} else {
	    fprintf(stderr, "[e_hypot.c] enter __ieee754_hypot 18\n");
	    return w;
	    // fprintf(stderr, "[e_hypot.c] exit __ieee754_hypot 18\n");
	}
}

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
    fprintf(stderr, "[e_hypot.c] enter main 1\n");
    if (argc != 3) {
        fprintf(stderr, "[e_hypot.c] enter main 2\n");
        printf("Usage: %s <arg> <arg>\n", argv[0]);
        return 1;
        // fprintf(stderr, "[e_hypot.c] exit main 2\n");
    }
    // fprintf(stderr, "[e_hypot.c] exit main 1\n");

    fprintf(stderr, "[e_hypot.c] enter main 3\n");
    double a0 = atof(argv[1]);
    double a1 = atof(argv[2]);

    double r = __ieee754_hypot(a0, a1);
    printf("%f\n", r);

    return 0;
    // fprintf(stderr, "[e_hypot.c] exit main 3\n");
}
// Total cost: 0.006093
// Total split cost: 0.000000, input tokens: 0, output tokens: 0, cache read tokens: 0, cache write tokens: 0, split chunks: [(0, 95)]
// Total instrumented cost: 0.006093, input tokens: 7765, output tokens: 3633, cache read tokens: 0, cache write tokens: 0
