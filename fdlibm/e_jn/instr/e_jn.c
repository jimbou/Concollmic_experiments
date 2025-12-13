





#include "fdlibm.h"
#include <stdio.h>

#ifdef __STDC__
static const double
#else
static double
#endif
invsqrtpi=  5.64189583547756279280e-01, 
two   =  2.00000000000000000000e+00, 
one   =  1.00000000000000000000e+00; 

static double zero  =  0.00000000000000000000e+00;

#ifdef __STDC__
	double __ieee754_jn(int n, double x)
#else
	double __ieee754_jn(n,x)
	int n; double x;
#endif
{
	fprintf(stderr, "\n");
	int i,hx,ix,lx, sgn;
	double a, b, temp, di;
	double z, w;

    
	hx = __HI(x);
	ix = 0x7fffffff&hx;
	lx = __LO(x);
    
	if((ix|((unsigned)(lx|-lx))>>31)>0x7ff00000) {
	fprintf(stderr, "[e_jn.c] enter __ieee754_jn 2\n");
	return x+x;
	// fprintf(stderr, "[e_jn.c] exit __ieee754_jn 2\n");
	}
	if(n<0){		
	fprintf(stderr, "[e_jn.c] enter __ieee754_jn 3\n");
		n = -n;
		x = -x;
		hx ^= 0x80000000;
	// fprintf(stderr, "[e_jn.c] exit __ieee754_jn 3\n");
	}
	if(n==0) {
	fprintf(stderr, "[e_jn.c] enter __ieee754_jn 4\n");
	return(__ieee754_j0(x));
	// fprintf(stderr, "[e_jn.c] exit __ieee754_jn 4\n");
	}
	if(n==1) {
	fprintf(stderr, "[e_jn.c] enter __ieee754_jn 5\n");
	return(__ieee754_j1(x));
	// fprintf(stderr, "[e_jn.c] exit __ieee754_jn 5\n");
	}
	fprintf(stderr, "[e_jn.c] enter __ieee754_jn 6\n");
	sgn = (n&1)&(hx>>31);	
	x = fabs(x);
	// fprintf(stderr, "[e_jn.c] exit __ieee754_jn 6\n");
	if((ix|lx)==0||ix>=0x7ff00000) 	
	{
	fprintf(stderr, "[e_jn.c] enter __ieee754_jn 7\n");
	    b = zero;
	// fprintf(stderr, "[e_jn.c] exit __ieee754_jn 7\n");
	}
	else if((double)n<=x) {   
		
	fprintf(stderr, "[e_jn.c] enter __ieee754_jn 8\n");
	    if(ix>=0x52D00000) { 
    
		switch(n&3) {
		    case 0: 
	fprintf(stderr, "[e_jn.c] enter __ieee754_jn 9\n");
	temp =  cos(x)+sin(x); 
	break;
	// fprintf(stderr, "[e_jn.c] exit __ieee754_jn 9\n");
		    case 1: 
	fprintf(stderr, "[e_jn.c] enter __ieee754_jn 10\n");
	temp = -cos(x)+sin(x); 
	break;
	// fprintf(stderr, "[e_jn.c] exit __ieee754_jn 10\n");
		    case 2: 
	fprintf(stderr, "[e_jn.c] enter __ieee754_jn 11\n");
	temp = -cos(x)-sin(x); 
	break;
	// fprintf(stderr, "[e_jn.c] exit __ieee754_jn 11\n");
		    case 3: 
	fprintf(stderr, "[e_jn.c] enter __ieee754_jn 12\n");
	temp =  cos(x)-sin(x); 
	break;
	// fprintf(stderr, "[e_jn.c] exit __ieee754_jn 12\n");
		}
	fprintf(stderr, "[e_jn.c] enter __ieee754_jn 13\n");
		b = invsqrtpi*temp/sqrt(x);
	// fprintf(stderr, "[e_jn.c] exit __ieee754_jn 13\n");
	    } else {	
	fprintf(stderr, "[e_jn.c] enter __ieee754_jn 14\n");
	        a = __ieee754_j0(x);
	        b = __ieee754_j1(x);
	        for(i=1;i<n;i++){
	fprintf(stderr, "[e_jn.c] enter __ieee754_jn 15\n");
		    temp = b;
		    b = b*((double)(i+i)/x) - a; 
		    a = temp;
	// fprintf(stderr, "[e_jn.c] exit __ieee754_jn 15\n");
	        }
	// fprintf(stderr, "[e_jn.c] exit __ieee754_jn 14\n");
	    }
	// fprintf(stderr, "[e_jn.c] exit __ieee754_jn 8\n");
	} else {
	fprintf(stderr, "[e_jn.c] enter __ieee754_jn 16\n");
	    if(ix<0x3e100000) {	
    
	fprintf(stderr, "[e_jn.c] enter __ieee754_jn 17\n");
		if(n>33)	
		{
	fprintf(stderr, "[e_jn.c] enter __ieee754_jn 18\n");
		    b = zero;
		// fprintf(stderr, "[e_jn.c] exit __ieee754_jn 18\n");
		}
		else {
	fprintf(stderr, "[e_jn.c] enter __ieee754_jn 19\n");
		    temp = x*0.5; b = temp;
		    for (a=one,i=2;i<=n;i++) {
	fprintf(stderr, "[e_jn.c] enter __ieee754_jn 20\n");
			a *= (double)i;		
			b *= temp;		
		// fprintf(stderr, "[e_jn.c] exit __ieee754_jn 20\n");
		    }
		    b = b/a;
		// fprintf(stderr, "[e_jn.c] exit __ieee754_jn 19\n");
		}
	// fprintf(stderr, "[e_jn.c] exit __ieee754_jn 17\n");
	    } else {
		
		
	    
	fprintf(stderr, "[e_jn.c] enter __ieee754_jn 21\n");
		double t,v;
		double q0,q1,h,tmp; int k,m;
		w  = (n+n)/(double)x; h = 2.0/(double)x;
		q0 = w;  z = w+h; q1 = w*z - 1.0; k=1;
		while(q1<1.0e9) {
	fprintf(stderr, "[e_jn.c] enter __ieee754_jn 22\n");
			k += 1; z += h;
			tmp = z*q1 - q0;
			q0 = q1;
			q1 = tmp;
		// fprintf(stderr, "[e_jn.c] exit __ieee754_jn 22\n");
		}
		m = n+n;
		for(t=zero, i = 2*(n+k); i>=m; i -= 2) {
	fprintf(stderr, "[e_jn.c] enter __ieee754_jn 23\n");
		t = one/(i/x-t);
		// fprintf(stderr, "[e_jn.c] exit __ieee754_jn 23\n");
		}
		a = t;
		b = one;
		
		tmp = n;
		v = two/x;
		tmp = tmp*__ieee754_log(fabs(v*tmp));
		if(tmp<7.09782712893383973096e+02) {
	fprintf(stderr, "[e_jn.c] enter __ieee754_jn 24\n");
	    	    for(i=n-1,di=(double)(i+i);i>0;i--){
	fprintf(stderr, "[e_jn.c] enter __ieee754_jn 25\n");
		        temp = b;
			b *= di;
			b  = b/x - a;
		        a = temp;
			di -= two;
		// fprintf(stderr, "[e_jn.c] exit __ieee754_jn 25\n");
	     	    }
		// fprintf(stderr, "[e_jn.c] exit __ieee754_jn 24\n");
		} else {
	fprintf(stderr, "[e_jn.c] enter __ieee754_jn 26\n");
	    	    for(i=n-1,di=(double)(i+i);i>0;i--){
	fprintf(stderr, "[e_jn.c] enter __ieee754_jn 27\n");
		        temp = b;
			b *= di;
			b  = b/x - a;
		        a = temp;
			di -= two;
		    
			if(b>1e100) {
	fprintf(stderr, "[e_jn.c] enter __ieee754_jn 28\n");
			    a /= b;
			    t /= b;
			    b  = one;
			// fprintf(stderr, "[e_jn.c] exit __ieee754_jn 28\n");
			}
		// fprintf(stderr, "[e_jn.c] exit __ieee754_jn 27\n");
	     	    }
		// fprintf(stderr, "[e_jn.c] exit __ieee754_jn 26\n");
		}
	fprintf(stderr, "[e_jn.c] enter __ieee754_jn 29\n");
	    	b = (t*__ieee754_j0(x)/b);
	// fprintf(stderr, "[e_jn.c] exit __ieee754_jn 29\n");
	    }
	// fprintf(stderr, "[e_jn.c] exit __ieee754_jn 21\n");
	}
	// fprintf(stderr, "[e_jn.c] exit __ieee754_jn 16\n");
	
	fprintf(stderr, "[e_jn.c] enter __ieee754_jn 30\n");
	if (sgn==1) {
	fprintf(stderr, "[e_jn.c] enter __ieee754_jn 31\n");
	return -b;
	// fprintf(stderr, "[e_jn.c] exit __ieee754_jn 31\n");
	} else {
	fprintf(stderr, "[e_jn.c] enter __ieee754_jn 32\n");
	return b;
	// fprintf(stderr, "[e_jn.c] exit __ieee754_jn 32\n");
	}
	// fprintf(stderr, "[e_jn.c] exit __ieee754_jn 30\n");
}

#ifdef __STDC__
	double __ieee754_yn(int n, double x) 
#else
	double __ieee754_yn(n,x) 
	int n; double x;
#endif
{
	fprintf(stderr, "\n");
	int i,hx,ix,lx;
	int sign;
	double a, b, temp;

	hx = __HI(x);
	ix = 0x7fffffff&hx;
	lx = __LO(x);
    
	if((ix|((unsigned)(lx|-lx))>>31)>0x7ff00000) {
	fprintf(stderr, "[e_jn.c] enter __ieee754_yn 2\n");
	return x+x;
	// fprintf(stderr, "[e_jn.c] exit __ieee754_yn 2\n");
	}
	if((ix|lx)==0) {
	fprintf(stderr, "[e_jn.c] enter __ieee754_yn 3\n");
	return -one/zero;
	// fprintf(stderr, "[e_jn.c] exit __ieee754_yn 3\n");
	}
	if(hx<0) {
	fprintf(stderr, "[e_jn.c] enter __ieee754_yn 4\n");
	return zero/zero;
	// fprintf(stderr, "[e_jn.c] exit __ieee754_yn 4\n");
	}
	fprintf(stderr, "[e_jn.c] enter __ieee754_yn 5\n");
	sign = 1;
	// fprintf(stderr, "[e_jn.c] exit __ieee754_yn 5\n");
	if(n<0){
	fprintf(stderr, "[e_jn.c] enter __ieee754_yn 6\n");
		n = -n;
		sign = 1 - ((n&1)<<1);
	// fprintf(stderr, "[e_jn.c] exit __ieee754_yn 6\n");
	}
	if(n==0) {
	fprintf(stderr, "[e_jn.c] enter __ieee754_yn 7\n");
	return(__ieee754_y0(x));
	// fprintf(stderr, "[e_jn.c] exit __ieee754_yn 7\n");
	}
	if(n==1) {
	fprintf(stderr, "[e_jn.c] enter __ieee754_yn 8\n");
	return(sign*__ieee754_y1(x));
	// fprintf(stderr, "[e_jn.c] exit __ieee754_yn 8\n");
	}
	if(ix==0x7ff00000) {
	fprintf(stderr, "[e_jn.c] enter __ieee754_yn 9\n");
	return zero;
	// fprintf(stderr, "[e_jn.c] exit __ieee754_yn 9\n");
	}
	if(ix>=0x52D00000) { 
    
	fprintf(stderr, "\n");
		switch(n&3) {
		    case 0: 
	fprintf(stderr, "[e_jn.c] enter __ieee754_yn 11\n");
	temp =  sin(x)-cos(x); 
	break;
	// fprintf(stderr, "[e_jn.c] exit __ieee754_yn 11\n");
		    case 1: 
	fprintf(stderr, "[e_jn.c] enter __ieee754_yn 12\n");
	temp = -sin(x)-cos(x); 
	break;
	// fprintf(stderr, "[e_jn.c] exit __ieee754_yn 12\n");
		    case 2: 
	fprintf(stderr, "[e_jn.c] enter __ieee754_yn 13\n");
	temp = -sin(x)+cos(x); 
	break;
	// fprintf(stderr, "[e_jn.c] exit __ieee754_yn 13\n");
		    case 3: 
	fprintf(stderr, "[e_jn.c] enter __ieee754_yn 14\n");
	temp =  sin(x)+cos(x); 
	break;
	// fprintf(stderr, "[e_jn.c] exit __ieee754_yn 14\n");
		}
	fprintf(stderr, "[e_jn.c] enter __ieee754_yn 15\n");
		b = invsqrtpi*temp/sqrt(x);
	// fprintf(stderr, "[e_jn.c] exit __ieee754_yn 15\n");
	} else {
	fprintf(stderr, "[e_jn.c] enter __ieee754_yn 16\n");
	    a = __ieee754_y0(x);
	    b = __ieee754_y1(x);
	
	    for(i=1;i<n&&(__HI(b) != 0xfff00000);i++){ 
	fprintf(stderr, "[e_jn.c] enter __ieee754_yn 17\n");
		temp = b;
		b = ((double)(i+i)/x)*b - a;
		a = temp;
	// fprintf(stderr, "[e_jn.c] exit __ieee754_yn 17\n");
	    }
	// fprintf(stderr, "[e_jn.c] exit __ieee754_yn 16\n");
	}
	fprintf(stderr, "[e_jn.c] enter __ieee754_yn 18\n");
	if(sign>0) {
	fprintf(stderr, "[e_jn.c] enter __ieee754_yn 19\n");
	return b;
	// fprintf(stderr, "[e_jn.c] exit __ieee754_yn 19\n");
	} else {
	fprintf(stderr, "[e_jn.c] enter __ieee754_yn 20\n");
	return -b;
	// fprintf(stderr, "[e_jn.c] exit __ieee754_yn 20\n");
	}
	// fprintf(stderr, "[e_jn.c] exit __ieee754_yn 18\n");
}

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {

    fprintf(stderr, "[e_jn.c] enter main 1\n");
    if (argc != 3) {
    fprintf(stderr, "[e_jn.c] enter main 2\n");
        printf("Usage: %s <arg> <arg>\n", argv[0]);
        return 1;
    // fprintf(stderr, "[e_jn.c] exit main 2\n");
    }

    fprintf(stderr, "[e_jn.c] enter main 3\n");
    int a0 = atoi(argv[1]);
    double a1 = atof(argv[2]);

    double r = __ieee754_jn(a0, a1);
    printf("%f\n", r);

    return 0;
    // fprintf(stderr, "[e_jn.c] exit main 3\n");
    // fprintf(stderr, "[e_jn.c] exit main 1\n");
}
// Total cost: 0.011067
// Total split cost: 0.000000, input tokens: 0, output tokens: 0, cache read tokens: 0, cache write tokens: 0, split chunks: [(0, 197)]
// Total instrumented cost: 0.011067, input tokens: 10996, output tokens: 7362, cache read tokens: 0, cache write tokens: 0
