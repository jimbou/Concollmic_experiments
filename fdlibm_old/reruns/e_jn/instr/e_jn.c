#include <stdio.h>
#include "fdlibm.h"

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
	fprintf(stderr, "[e_jn.c] enter __ieee754_jn 1\n");
	int i,hx,ix,lx, sgn;
	double a, b, temp, di;
	double z, w;

    
	hx = __HI(x);
	ix = 0x7fffffff&hx;
	lx = __LO(x);
	// fprintf(stderr, "[e_jn.c] exit __ieee754_jn 1\n");
    
	if((ix|((unsigned)(lx|-lx))>>31)>0x7ff00000) return x+x;
	if(n<0){		
		fprintf(stderr, "[e_jn.c] enter __ieee754_jn 2\n");
		n = -n;
		x = -x;
		hx ^= 0x80000000;
		// fprintf(stderr, "[e_jn.c] exit __ieee754_jn 2\n");
	}
	fprintf(stderr, "[e_jn.c] enter __ieee754_jn 3\n");
	if(n==0) return(__ieee754_j0(x));
	if(n==1) return(__ieee754_j1(x));
	sgn = (n&1)&(hx>>31);	
	x = fabs(x);
	// fprintf(stderr, "[e_jn.c] exit __ieee754_jn 3\n");
	if((ix|lx)==0||ix>=0x7ff00000) 	
	    b = zero;
	else if((double)n<=x) {   
		fprintf(stderr, "[e_jn.c] enter __ieee754_jn 4\n");
		if(ix>=0x52D00000) { 
			fprintf(stderr, "[e_jn.c] enter __ieee754_jn 5\n");
			switch(n&3) {
			    case 0: fprintf(stderr, "\n"); temp =  cos(x)+sin(x); break;
			    // fprintf(stderr, "\n");
			    case 1: fprintf(stderr, "\n"); temp = -cos(x)+sin(x); break;
			    // fprintf(stderr, "\n");
			    case 2: fprintf(stderr, "\n"); temp = -cos(x)-sin(x); break;
			    // fprintf(stderr, "\n");
			    case 3: fprintf(stderr, "\n"); temp =  cos(x)-sin(x); break;
			    // fprintf(stderr, "\n");
			}
			b = invsqrtpi*temp/sqrt(x);
			// fprintf(stderr, "[e_jn.c] exit __ieee754_jn 5\n");
		} else {	
			fprintf(stderr, "[e_jn.c] enter __ieee754_jn 10\n");
		        a = __ieee754_j0(x);
		        b = __ieee754_j1(x);
		        for(i=1;i<n;i++){
				fprintf(stderr, "[e_jn.c] enter __ieee754_jn 11\n");
			    temp = b;
			    b = b*((double)(i+i)/x) - a; 
			    a = temp;
			    // fprintf(stderr, "[e_jn.c] exit __ieee754_jn 11\n");
		        }
			// fprintf(stderr, "[e_jn.c] exit __ieee754_jn 10\n");
		}
		// fprintf(stderr, "[e_jn.c] exit __ieee754_jn 4\n");
	} else {
		fprintf(stderr, "[e_jn.c] enter __ieee754_jn 12\n");
	    if(ix<0x3e100000) {	
			fprintf(stderr, "[e_jn.c] enter __ieee754_jn 13\n");
		if(n>33)	
		    b = zero;
		else {
			fprintf(stderr, "[e_jn.c] enter __ieee754_jn 14\n");
		    temp = x*0.5; b = temp;
		    for (a=one,i=2;i<=n;i++) {
				fprintf(stderr, "[e_jn.c] enter __ieee754_jn 15\n");
			a *= (double)i;		
			b *= temp;		
			// fprintf(stderr, "[e_jn.c] exit __ieee754_jn 15\n");
		    }
		    b = b/a;
			// fprintf(stderr, "[e_jn.c] exit __ieee754_jn 14\n");
		}
		// fprintf(stderr, "[e_jn.c] exit __ieee754_jn 13\n");
	    } else {
			fprintf(stderr, "[e_jn.c] enter __ieee754_jn 16\n");
		
		
	    
		double t,v;
		double q0,q1,h,tmp; int k,m;
		w  = (n+n)/(double)x; h = 2.0/(double)x;
		q0 = w;  z = w+h; q1 = w*z - 1.0; k=1;
		while(q1<1.0e9) {
			fprintf(stderr, "[e_jn.c] enter __ieee754_jn 17\n");
			k += 1; z += h;
			tmp = z*q1 - q0;
			q0 = q1;
			q1 = tmp;
			// fprintf(stderr, "[e_jn.c] exit __ieee754_jn 17\n");
		}
		m = n+n;
		for(t=zero, i = 2*(n+k); i>=m; i -= 2) {
			fprintf(stderr, "[e_jn.c] enter __ieee754_jn 18\n");
			t = one/(i/x-t);
			// fprintf(stderr, "[e_jn.c] exit __ieee754_jn 18\n");
		}
		a = t;
		b = one;
		
		tmp = n;
		v = two/x;
		tmp = tmp*__ieee754_log(fabs(v*tmp));
		if(tmp<7.09782712893383973096e+02) {
			fprintf(stderr, "[e_jn.c] enter __ieee754_jn 19\n");
	    	    for(i=n-1,di=(double)(i+i);i>0;i--){
					fprintf(stderr, "[e_jn.c] enter __ieee754_jn 20\n");
			        temp = b;
				b *= di;
				b  = b/x - a;
			        a = temp;
				di -= two;
				// fprintf(stderr, "[e_jn.c] exit __ieee754_jn 20\n");
	     	    }
			// fprintf(stderr, "[e_jn.c] exit __ieee754_jn 19\n");
		} else {
			fprintf(stderr, "[e_jn.c] enter __ieee754_jn 21\n");
	    	    for(i=n-1,di=(double)(i+i);i>0;i--){
					fprintf(stderr, "[e_jn.c] enter __ieee754_jn 22\n");
			        temp = b;
				b *= di;
				b  = b/x - a;
			        a = temp;
				di -= two;
			    
				if(b>1e100) {
					fprintf(stderr, "[e_jn.c] enter __ieee754_jn 23\n");
				    a /= b;
				    t /= b;
				    b  = one;
					// fprintf(stderr, "[e_jn.c] exit __ieee754_jn 23\n");
				}
				// fprintf(stderr, "[e_jn.c] exit __ieee754_jn 22\n");
	     	    }
			// fprintf(stderr, "[e_jn.c] exit __ieee754_jn 21\n");
		}
	    	b = (t*__ieee754_j0(x)/b);
		// fprintf(stderr, "[e_jn.c] exit __ieee754_jn 16\n");
	    }
		// fprintf(stderr, "[e_jn.c] exit __ieee754_jn 12\n");
	}
	fprintf(stderr, "[e_jn.c] enter __ieee754_jn 24\n");
	if(sgn==1) return -b; else return b;
	// fprintf(stderr, "[e_jn.c] exit __ieee754_jn 24\n");
}

#ifdef __STDC__
	double __ieee754_yn(int n, double x) 
#else
	double __ieee754_yn(n,x) 
	int n; double x;
#endif
{
	fprintf(stderr, "[e_jn.c] enter __ieee754_yn 1\n");
	int i,hx,ix,lx;
	int sign;
	double a, b, temp;

	hx = __HI(x);
	ix = 0x7fffffff&hx;
	lx = __LO(x);
	// fprintf(stderr, "[e_jn.c] exit __ieee754_yn 1\n");
    
	if((ix|((unsigned)(lx|-lx))>>31)>0x7ff00000) return x+x;
	if((ix|lx)==0) return -one/zero;
	if(hx<0) return zero/zero;
	sign = 1;
	if(n<0){
		fprintf(stderr, "[e_jn.c] enter __ieee754_yn 2\n");
		n = -n;
		sign = 1 - ((n&1)<<1);
		// fprintf(stderr, "[e_jn.c] exit __ieee754_yn 2\n");
	}
	fprintf(stderr, "[e_jn.c] enter __ieee754_yn 3\n");
	if(n==0) return(__ieee754_y0(x));
	if(n==1) return(sign*__ieee754_y1(x));
	if(ix==0x7ff00000) return zero;
	// fprintf(stderr, "[e_jn.c] exit __ieee754_yn 3\n");
	if(ix>=0x52D00000) { 
		fprintf(stderr, "[e_jn.c] enter __ieee754_yn 4\n");
		switch(n&3) {
		    case 0: fprintf(stderr, "\n"); temp =  sin(x)-cos(x); break;
		    // fprintf(stderr, "\n");
		    case 1: fprintf(stderr, "\n"); temp = -sin(x)-cos(x); break;
		    // fprintf(stderr, "\n");
		    case 2: fprintf(stderr, "\n"); temp = -sin(x)+cos(x); break;
		    // fprintf(stderr, "\n");
		    case 3: fprintf(stderr, "\n"); temp =  sin(x)+cos(x); break;
		    // fprintf(stderr, "\n");
		}
		b = invsqrtpi*temp/sqrt(x);
		// fprintf(stderr, "[e_jn.c] exit __ieee754_yn 4\n");
	} else {
		fprintf(stderr, "[e_jn.c] enter __ieee754_yn 9\n");
	    a = __ieee754_y0(x);
	    b = __ieee754_y1(x);
	
	    for(i=1;i<n&&(__HI(b) != 0xfff00000);i++){ 
			fprintf(stderr, "[e_jn.c] enter __ieee754_yn 10\n");
		temp = b;
		b = ((double)(i+i)/x)*b - a;
		a = temp;
		// fprintf(stderr, "[e_jn.c] exit __ieee754_yn 10\n");
	    }
		// fprintf(stderr, "[e_jn.c] exit __ieee754_yn 9\n");
	}
	fprintf(stderr, "[e_jn.c] enter __ieee754_yn 11\n");
	if(sign>0) return b; else return -b;
	// fprintf(stderr, "[e_jn.c] exit __ieee754_yn 11\n");
}

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
	fprintf(stderr, "\n");

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
    double r1 = __ieee754_yn(a0, a1);
    printf("%f\n", r);
    printf("%f\n", r1);

    return 0;
	// fprintf(stderr, "[e_jn.c] exit main 3\n");
}
// Total cost: 0.036002
// Total split cost: 0.000000, input tokens: 0, output tokens: 0, cache read tokens: 0, cache write tokens: 0, split chunks: [(0, 199)]
// Total instrumented cost: 0.036002, input tokens: 48068, output tokens: 20931, cache read tokens: 0, cache write tokens: 0
