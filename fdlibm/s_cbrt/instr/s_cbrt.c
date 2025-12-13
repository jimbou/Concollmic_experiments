



#include "fdlibm.h"
#include <stdio.h>


#ifdef __STDC__
static const unsigned 
#else
static unsigned 
#endif
	B1 = 715094163, 
	B2 = 696219795; 

#ifdef __STDC__
static const double
#else
static double
#endif
C =  5.42857142857142815906e-01, 
D = -7.05306122448979611050e-01, 
E =  1.41428571428571436819e+00, 
F =  1.60714285714285720630e+00, 
G =  3.57142857142857150787e-01; 

#ifdef __STDC__
	double cbrt(double x) 
#else
	double cbrt(x) 
	double x;
#endif
{
	fprintf(stderr, "\n");
	int	hx;
	double r,s,t=0.0,w;
	unsigned sign;


	hx = __HI(x);		
	sign=hx&0x80000000; 		
	hx  ^=sign;
	if(hx>=0x7ff00000) 
	{
		fprintf(stderr, "[s_cbrt.c] enter cbrt 2\n");
		return(x+x);
		// fprintf(stderr, "[s_cbrt.c] exit cbrt 2\n");
	}
	if((hx|__LO(x))==0) 
	{
		fprintf(stderr, "[s_cbrt.c] enter cbrt 3\n");
	    return(x);
		// fprintf(stderr, "[s_cbrt.c] exit cbrt 3\n");
	}

	__HI(x) = hx;	
    
	if(hx<0x00100000) 		
	{
		fprintf(stderr, "[s_cbrt.c] enter cbrt 4\n");
	  	__HI(t)=0x43500000; 		
	   	t*=x; __HI(t)=__HI(t)/3+B2;
		// fprintf(stderr, "[s_cbrt.c] exit cbrt 4\n");
	}
	else
	{
		fprintf(stderr, "[s_cbrt.c] enter cbrt 5\n");
	  	__HI(t)=hx/3+B1;
		// fprintf(stderr, "[s_cbrt.c] exit cbrt 5\n");
	}


    
	fprintf(stderr, "[s_cbrt.c] enter cbrt 6\n");
	r=t*t/x;
	s=C+r*t;
	t*=G+F/(s+E+D/s);	
	// fprintf(stderr, "[s_cbrt.c] exit cbrt 6\n");

     
	fprintf(stderr, "[s_cbrt.c] enter cbrt 7\n");
	__LO(t)=0; __HI(t)+=0x00000001;
	// fprintf(stderr, "[s_cbrt.c] exit cbrt 7\n");


    
	fprintf(stderr, "[s_cbrt.c] enter cbrt 8\n");
	s=t*t;		
	r=x/s;
	w=t+t;
	r=(r-t)/(w+r);	
	t=t+t*r;
	// fprintf(stderr, "[s_cbrt.c] exit cbrt 8\n");

    
	fprintf(stderr, "[s_cbrt.c] enter cbrt 9\n");
	__HI(t) |= sign;
	return(t);
	// fprintf(stderr, "[s_cbrt.c] exit cbrt 9\n");
}

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
	fprintf(stderr, "\n");
    if (argc != 2) {
		fprintf(stderr, "[s_cbrt.c] enter main 2\n");
        printf("Usage: %s <arg>\n", argv[0]);
        return 1;
		// fprintf(stderr, "[s_cbrt.c] exit main 2\n");
    }

	fprintf(stderr, "[s_cbrt.c] enter main 3\n");
    double a0 = atof(argv[1]);

    double r = cbrt(a0);
    printf("%f\n", r);

    return 0;
	// fprintf(stderr, "[s_cbrt.c] exit main 3\n");
}
// Total cost: 0.001653
// Total split cost: 0.000000, input tokens: 0, output tokens: 0, cache read tokens: 0, cache write tokens: 0, split chunks: [(0, 93)]
// Total instrumented cost: 0.001653, input tokens: 2660, output tokens: 850, cache read tokens: 0, cache write tokens: 0
