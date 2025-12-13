#include <stdio.h>
#include "fdlibm.h"

#ifdef __STDC__
static	const double	one	= 1.0, tiny=1.0e-300;
#else
static	double	one	= 1.0, tiny=1.0e-300;
#endif

#ifdef __STDC__
	double __ieee754_sqrt(double x)
#else
	double __ieee754_sqrt(x)
	double x;
#endif
{
	fprintf(stderr, "[e_sqrt.c] enter __ieee754_sqrt 1\n");
	double z;
	int 	sign = (int)0x80000000; 
	unsigned r,t1,s1,ix1,q1;
	int ix0,s0,q,m,t,i;

	ix0 = __HI(x);			
	ix1 = __LO(x);		
	// fprintf(stderr, "[e_sqrt.c] exit __ieee754_sqrt 1\n");

    
	if((ix0&0x7ff00000)==0x7ff00000) {	
		fprintf(stderr, "[e_sqrt.c] enter __ieee754_sqrt 2\n");		
	    return x*x+x;
	    // fprintf(stderr, "[e_sqrt.c] exit __ieee754_sqrt 2\n");		
	} 
    
	if(ix0<=0) {
		fprintf(stderr, "[e_sqrt.c] enter __ieee754_sqrt 3\n");
	    if(((ix0&(~sign))|ix1)==0) return x;
	    // fprintf(stderr, "[e_sqrt.c] exit __ieee754_sqrt 3\n");
	    else if(ix0<0)
	    {
		fprintf(stderr, "[e_sqrt.c] enter __ieee754_sqrt 4\n");
		return (x-x)/(x-x);
		// fprintf(stderr, "[e_sqrt.c] exit __ieee754_sqrt 4\n");		
	    }
	}
    
	m = (ix0>>20);
	if(m==0) {				
	    while(ix0==0) {
		fprintf(stderr, "[e_sqrt.c] enter __ieee754_sqrt 5\n");
		m -= 21;
		ix0 |= (ix1>>11); ix1 <<= 21;
		// fprintf(stderr, "[e_sqrt.c] exit __ieee754_sqrt 5\n");
	    }
	    for(i=0;(ix0&0x00100000)==0;i++) {
		fprintf(stderr, "[e_sqrt.c] enter __ieee754_sqrt 6\n");
		ix0<<=1;
		// fprintf(stderr, "[e_sqrt.c] exit __ieee754_sqrt 6\n");
	    }
	    fprintf(stderr, "[e_sqrt.c] enter __ieee754_sqrt 7\n");
	    m -= i-1;
	    ix0 |= (ix1>>(32-i));
	    ix1 <<= i;
	    // fprintf(stderr, "[e_sqrt.c] exit __ieee754_sqrt 7\n");
	}
	fprintf(stderr, "[e_sqrt.c] enter __ieee754_sqrt 8\n");
	m -= 1023;	
	ix0 = (ix0&0x000fffff)|0x00100000;
	if(m&1){	
		fprintf(stderr, "[e_sqrt.c] enter __ieee754_sqrt 9\n");
	    ix0 += ix0 + ((ix1&sign)>>31);
	    ix1 += ix1;
	    // fprintf(stderr, "[e_sqrt.c] exit __ieee754_sqrt 9\n");
	}
	m >>= 1;	
	// fprintf(stderr, "[e_sqrt.c] exit __ieee754_sqrt 8\n");

    
	fprintf(stderr, "[e_sqrt.c] enter __ieee754_sqrt 10\n");
	ix0 += ix0 + ((ix1&sign)>>31);
	ix1 += ix1;
	q = q1 = s0 = s1 = 0;	
	r = 0x00200000;		
	// fprintf(stderr, "[e_sqrt.c] exit __ieee754_sqrt 10\n");

	while(r!=0) {
	    fprintf(stderr, "[e_sqrt.c] enter __ieee754_sqrt 11\n");
	    t = s0+r; 
	    if(t<=ix0) { 
		fprintf(stderr, "[e_sqrt.c] enter __ieee754_sqrt 12\n");
		s0   = t+r; 
		ix0 -= t; 
		q   += r; 
		// fprintf(stderr, "[e_sqrt.c] exit __ieee754_sqrt 12\n");
	    } 
	    ix0 += ix0 + ((ix1&sign)>>31);
	    ix1 += ix1;
	    r>>=1;
	    // fprintf(stderr, "[e_sqrt.c] exit __ieee754_sqrt 11\n");
	}

	r = sign;
	while(r!=0) {
	    fprintf(stderr, "[e_sqrt.c] enter __ieee754_sqrt 13\n");
	    t1 = s1+r; 
	    t  = s0;
	    if((t<ix0)||((t==ix0)&&(t1<=ix1))) { 
		fprintf(stderr, "[e_sqrt.c] enter __ieee754_sqrt 14\n");
		s1  = t1+r;
		if(((t1&sign)==sign)&&(s1&sign)==0) s0 += 1;
		ix0 -= t;
		if (ix1 < t1) ix0 -= 1;
		ix1 -= t1;
		q1  += r;
		// fprintf(stderr, "[e_sqrt.c] exit __ieee754_sqrt 14\n");
	    }
	    ix0 += ix0 + ((ix1&sign)>>31);
	    ix1 += ix1;
	    r>>=1;
	    // fprintf(stderr, "[e_sqrt.c] exit __ieee754_sqrt 13\n");
	}

    
	if((ix0|ix1)!=0) {
	    fprintf(stderr, "[e_sqrt.c] enter __ieee754_sqrt 15\n");
	    z = one-tiny; 
	    if (z>=one) {
		fprintf(stderr, "[e_sqrt.c] enter __ieee754_sqrt 16\n");
	        z = one+tiny;
	        if (q1==(unsigned)0xffffffff) { q1=0; q += 1;}
		else if (z>one) {
		    fprintf(stderr, "[e_sqrt.c] enter __ieee754_sqrt 17\n");
		    if (q1==(unsigned)0xfffffffe) q+=1;
		    q1+=2; 
		    // fprintf(stderr, "[e_sqrt.c] exit __ieee754_sqrt 17\n");
		} else
		{
		    fprintf(stderr, "[e_sqrt.c] enter __ieee754_sqrt 18\n");
	            q1 += (q1&1);
		    // fprintf(stderr, "[e_sqrt.c] exit __ieee754_sqrt 18\n");
		}
		// fprintf(stderr, "[e_sqrt.c] exit __ieee754_sqrt 16\n");
	    }
	    // fprintf(stderr, "[e_sqrt.c] exit __ieee754_sqrt 15\n");
	}
	fprintf(stderr, "[e_sqrt.c] enter __ieee754_sqrt 19\n");
	ix0 = (q>>1)+0x3fe00000;
	ix1 =  q1>>1;
	if ((q&1)==1) ix1 |= sign;
	ix0 += (m <<20);
	__HI(z) = ix0;
	__LO(z) = ix1;
	return z;
	// fprintf(stderr, "[e_sqrt.c] exit __ieee754_sqrt 19\n");
}



#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
	fprintf(stderr, "\n");

    if (argc != 2) {
		fprintf(stderr, "[e_sqrt.c] enter main 2\n");
        printf("Usage: %s <arg>\n", argv[0]);
        return 1;
        // fprintf(stderr, "[e_sqrt.c] exit main 2\n");
    }

    fprintf(stderr, "[e_sqrt.c] enter main 3\n");
    double a0 = atof(argv[1]);

    double r = __ieee754_sqrt(a0);
    printf("%f\n", r);

    return 0;
    // fprintf(stderr, "[e_sqrt.c] exit main 3\n");
}
// Total cost: 0.005955
// Total split cost: 0.000000, input tokens: 0, output tokens: 0, cache read tokens: 0, cache write tokens: 0, split chunks: [(0, 134)]
// Total instrumented cost: 0.005955, input tokens: 7988, output tokens: 3453, cache read tokens: 0, cache write tokens: 0
