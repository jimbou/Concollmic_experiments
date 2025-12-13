





#include "fdlibm.h"
#include <stdio.h>

#ifdef __STDC__
static const double 
#else
static double 
#endif
tiny  = 1.0e-300,
zero  = 0.0,
pi_o_4  = 7.8539816339744827900E-01, 
pi_o_2  = 1.5707963267948965580E+00, 
pi      = 3.1415926535897931160E+00, 
pi_lo   = 1.2246467991473531772E-16; 

#ifdef __STDC__
	double __ieee754_atan2(double y, double x)
#else
	double __ieee754_atan2(y,x)
	double  y,x;
#endif
{  
	fprintf(stderr, "[e_atan2.c] enter __ieee754_atan2 1\n");
	double z;
	int k,m,hx,hy,ix,iy;
	unsigned lx,ly;

	hx = __HI(x); ix = hx&0x7fffffff;
	lx = __LO(x);
	hy = __HI(y); iy = hy&0x7fffffff;
	ly = __LO(y);
	// fprintf(stderr, "[e_atan2.c] exit __ieee754_atan2 1\n");
	if(((ix|((lx|-lx)>>31))>0x7ff00000)||
	   ((iy|((ly|-ly)>>31))>0x7ff00000))	
	{
		fprintf(stderr, "[e_atan2.c] enter __ieee754_atan2 2\n");
		return x+y;
		// fprintf(stderr, "[e_atan2.c] exit __ieee754_atan2 2\n");
	}
	fprintf(stderr, "\n");
	if((hx-0x3ff00000|lx)==0)
	{
		fprintf(stderr, "[e_atan2.c] enter __ieee754_atan2 4\n");
		return atan(y);
		// fprintf(stderr, "[e_atan2.c] exit __ieee754_atan2 4\n");
	}
	fprintf(stderr, "[e_atan2.c] enter __ieee754_atan2 5\n");
	m = ((hy>>31)&1)|((hx>>30)&2);	
	// fprintf(stderr, "[e_atan2.c] exit __ieee754_atan2 5\n");

    
	fprintf(stderr, "[e_atan2.c] enter __ieee754_atan2 6\n");
	if((iy|ly)==0)
	{
		fprintf(stderr, "[e_atan2.c] enter __ieee754_atan2 7\n");
		switch(m) {
		case 0: 
		case 1:
			fprintf(stderr, "[e_atan2.c] enter __ieee754_atan2 8\n");
			return y;
			// fprintf(stderr, "[e_atan2.c] exit __ieee754_atan2 8\n");
		case 2:
			fprintf(stderr, "[e_atan2.c] enter __ieee754_atan2 9\n");
			return  pi+tiny;
			// fprintf(stderr, "[e_atan2.c] exit __ieee754_atan2 9\n");
		case 3:
			fprintf(stderr, "[e_atan2.c] enter __ieee754_atan2 10\n");
			return -pi-tiny;
			// fprintf(stderr, "[e_atan2.c] exit __ieee754_atan2 10\n");
		}
		// fprintf(stderr, "[e_atan2.c] exit __ieee754_atan2 7\n");
	}
	// fprintf(stderr, "[e_atan2.c] exit __ieee754_atan2 6\n");
    
	fprintf(stderr, "[e_atan2.c] enter __ieee754_atan2 11\n");
	if((ix|lx)==0)
	{
		fprintf(stderr, "[e_atan2.c] enter __ieee754_atan2 12\n");
		return (hy<0)?  -pi_o_2-tiny: pi_o_2+tiny;
		// fprintf(stderr, "[e_atan2.c] exit __ieee754_atan2 12\n");
	}
	// fprintf(stderr, "[e_atan2.c] exit __ieee754_atan2 11\n");
	    
    
	fprintf(stderr, "[e_atan2.c] enter __ieee754_atan2 13\n");
	if(ix==0x7ff00000)
	{
		fprintf(stderr, "[e_atan2.c] enter __ieee754_atan2 14\n");
		if(iy==0x7ff00000)
		{
			fprintf(stderr, "[e_atan2.c] enter __ieee754_atan2 15\n");
			switch(m) {
			    case 0:
				fprintf(stderr, "[e_atan2.c] enter __ieee754_atan2 16\n");
				return  pi_o_4+tiny;
				// fprintf(stderr, "[e_atan2.c] exit __ieee754_atan2 16\n");
			    case 1:
				fprintf(stderr, "[e_atan2.c] enter __ieee754_atan2 17\n");
				return -pi_o_4-tiny;
				// fprintf(stderr, "[e_atan2.c] exit __ieee754_atan2 17\n");
			    case 2:
				fprintf(stderr, "[e_atan2.c] enter __ieee754_atan2 18\n");
				return  3.0*pi_o_4+tiny;
				// fprintf(stderr, "[e_atan2.c] exit __ieee754_atan2 18\n");
			    case 3:
				fprintf(stderr, "[e_atan2.c] enter __ieee754_atan2 19\n");
				return -3.0*pi_o_4-tiny;
				// fprintf(stderr, "[e_atan2.c] exit __ieee754_atan2 19\n");
			}
			// fprintf(stderr, "[e_atan2.c] exit __ieee754_atan2 15\n");
		}
		else
		{
			fprintf(stderr, "[e_atan2.c] enter __ieee754_atan2 20\n");
			switch(m) {
			    case 0:
				fprintf(stderr, "[e_atan2.c] enter __ieee754_atan2 21\n");
				return  zero  ;	
				// fprintf(stderr, "[e_atan2.c] exit __ieee754_atan2 21\n");
			    case 1:
				fprintf(stderr, "[e_atan2.c] enter __ieee754_atan2 22\n");
				return -zero  ;	
				// fprintf(stderr, "[e_atan2.c] exit __ieee754_atan2 22\n");
			    case 2:
				fprintf(stderr, "[e_atan2.c] enter __ieee754_atan2 23\n");
				return  pi+tiny  ;	
				// fprintf(stderr, "[e_atan2.c] exit __ieee754_atan2 23\n");
			    case 3:
				fprintf(stderr, "[e_atan2.c] enter __ieee754_atan2 24\n");
				return -pi-tiny  ;	
				// fprintf(stderr, "[e_atan2.c] exit __ieee754_atan2 24\n");
			}
			// fprintf(stderr, "[e_atan2.c] exit __ieee754_atan2 20\n");
		}
		// fprintf(stderr, "[e_atan2.c] exit __ieee754_atan2 14\n");
	}
	// fprintf(stderr, "[e_atan2.c] exit __ieee754_atan2 13\n");
    
	fprintf(stderr, "[e_atan2.c] enter __ieee754_atan2 25\n");
	if(iy==0x7ff00000)
	{
		fprintf(stderr, "[e_atan2.c] enter __ieee754_atan2 26\n");
		return (hy<0)? -pi_o_2-tiny: pi_o_2+tiny;
		// fprintf(stderr, "[e_atan2.c] exit __ieee754_atan2 26\n");
	}
	// fprintf(stderr, "[e_atan2.c] exit __ieee754_atan2 25\n");

    
	fprintf(stderr, "[e_atan2.c] enter __ieee754_atan2 27\n");
	k = (iy-ix)>>20;
	if(k > 60)
	{
		fprintf(stderr, "[e_atan2.c] enter __ieee754_atan2 28\n");
		z=pi_o_2+0.5*pi_lo;
		// fprintf(stderr, "[e_atan2.c] exit __ieee754_atan2 28\n");
	}
	else if(hx<0&&k<-60)
	{
		fprintf(stderr, "[e_atan2.c] enter __ieee754_atan2 29\n");
		z=0.0;
		// fprintf(stderr, "[e_atan2.c] exit __ieee754_atan2 29\n");
	}
	else
	{
		fprintf(stderr, "[e_atan2.c] enter __ieee754_atan2 30\n");
		z=atan(fabs(y/x));
		// fprintf(stderr, "[e_atan2.c] exit __ieee754_atan2 30\n");
	}
	// fprintf(stderr, "[e_atan2.c] exit __ieee754_atan2 27\n");
	
	fprintf(stderr, "[e_atan2.c] enter __ieee754_atan2 31\n");
	switch (m) {
	    case 0:
		fprintf(stderr, "[e_atan2.c] enter __ieee754_atan2 32\n");
		return       z  ;	
		// fprintf(stderr, "[e_atan2.c] exit __ieee754_atan2 32\n");
	    case 1:
		fprintf(stderr, "[e_atan2.c] enter __ieee754_atan2 33\n");
		__HI(z) ^= 0x80000000;
		return       z  ;	
		// fprintf(stderr, "[e_atan2.c] exit __ieee754_atan2 33\n");
	    case 2:
		fprintf(stderr, "[e_atan2.c] enter __ieee754_atan2 34\n");
		return  pi-(z-pi_lo);
		// fprintf(stderr, "[e_atan2.c] exit __ieee754_atan2 34\n");
	    default: 
	    	fprintf(stderr, "[e_atan2.c] enter __ieee754_atan2 35\n");
	    	return  (z-pi_lo)-pi;
		// fprintf(stderr, "[e_atan2.c] exit __ieee754_atan2 35\n");
	}
	// fprintf(stderr, "[e_atan2.c] exit __ieee754_atan2 31\n");
}

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {

	fprintf(stderr, "[e_atan2.c] enter main 1\n");
	if (argc != 3)
	{
		fprintf(stderr, "[e_atan2.c] enter main 2\n");
		printf("Usage: %s <arg> <arg>\n", argv[0]);
		return 1;
		// fprintf(stderr, "[e_atan2.c] exit main 2\n");
	}
	// fprintf(stderr, "[e_atan2.c] exit main 1\n");

	fprintf(stderr, "[e_atan2.c] enter main 3\n");
	double a0 = atof(argv[1]);
	double a1 = atof(argv[2]);

	double r = __ieee754_atan2(a0, a1);
	printf("%f\n", r);

	return 0;
	// fprintf(stderr, "[e_atan2.c] exit main 3\n");
}
// Total cost: 0.012267
// Total split cost: 0.000000, input tokens: 0, output tokens: 0, cache read tokens: 0, cache write tokens: 0, split chunks: [(0, 107)]
// Total instrumented cost: 0.012267, input tokens: 16294, output tokens: 7152, cache read tokens: 0, cache write tokens: 0
