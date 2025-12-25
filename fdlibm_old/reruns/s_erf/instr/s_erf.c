






#include "fdlibm.h"
#include <stdio.h>

#ifdef __STDC__
static const double
#else
static double
#endif
tiny	    = 1e-300,
half=  5.00000000000000000000e-01, 
one =  1.00000000000000000000e+00, 
two =  2.00000000000000000000e+00, 
	
erx =  8.45062911510467529297e-01, 

efx =  1.28379167095512586316e-01, 
efx8=  1.02703333676410069053e+00, 
pp0  =  1.28379167095512558561e-01, 
pp1  = -3.25042107247001499370e-01, 
pp2  = -2.84817495755985104766e-02, 
pp3  = -5.77027029648944159157e-03, 
pp4  = -2.37630166566501626084e-05, 
qq1  =  3.97917223959155352819e-01, 
qq2  =  6.50222499887672944485e-02, 
qq3  =  5.08130628187576562776e-03, 
qq4  =  1.32494738004321644526e-04, 
qq5  = -3.96022827877536812320e-06, 

pa0  = -2.36211856075265944077e-03, 
pa1  =  4.14856118683748331666e-01, 
pa2  = -3.72207876035701323847e-01, 
pa3  =  3.18346619901161753674e-01, 
pa4  = -1.10894694282396677476e-01, 
pa5  =  3.54783043256182359371e-02, 
pa6  = -2.16637559486879084300e-03, 
qa1  =  1.06420880400844228286e-01, 
qa2  =  5.40397917702171048937e-01, 
qa3  =  7.18286544141962662868e-02, 
qa4  =  1.26171219808761642112e-01, 
qa5  =  1.36370839120290507362e-02, 
qa6  =  1.19844998467991074170e-02, 

ra0  = -9.86494403484714822705e-03, 
ra1  = -6.93858572707181764372e-01, 
ra2  = -1.05586262253232909814e+01, 
ra3  = -6.23753324503260060396e+01, 
ra4  = -1.62396669462573470355e+02, 
ra5  = -1.84605092906711035994e+02, 
ra6  = -8.12874355063065934246e+01, 
ra7  = -9.81432934416914548592e+00, 
sa1  =  1.96512716674392571292e+01, 
sa2  =  1.37657754143519042600e+02, 
sa3  =  4.34565877475229228821e+02, 
sa4  =  6.45387271733267880336e+02, 
sa5  =  4.29008140027567833386e+02, 
sa6  =  1.08635005541779435134e+02, 
sa7  =  6.57024977031928170135e+00, 
sa8  = -6.04244152148580987438e-02, 

rb0  = -9.86494292470009928597e-03, 
rb1  = -7.99283237680523006574e-01, 
rb2  = -1.77579549177547519889e+01, 
rb3  = -1.60636384855821916062e+02, 
rb4  = -6.37566443368389627722e+02, 
rb5  = -1.02509513161107724954e+03, 
rb6  = -4.83519191608651397019e+02, 
sb1  =  3.03380607434824582924e+01, 
sb2  =  3.25792512996573918826e+02, 
sb3  =  1.53672958608443695994e+03, 
sb4  =  3.19985821950859553908e+03, 
sb5  =  2.55305040643316442583e+03, 
sb6  =  4.74528541206955367215e+02, 
sb7  = -2.24409524465858183362e+01; 

#ifdef __STDC__
	double erf(double x) 
#else
	double erf(x) 
	double x;
#endif
{
	fprintf(stderr, "[s_erf.c] enter erf 1\n");
	int hx,ix,i;
	double R,S,P,Q,s,y,z,r;
	hx = __HI(x);
	ix = hx&0x7fffffff;
	if(ix>=0x7ff00000) {		
	    fprintf(stderr, "[s_erf.c] enter erf 2\n");
	    i = ((unsigned)hx>>31)<<1;
	    return (double)(1-i)+one/x;
	    // fprintf(stderr, "[s_erf.c] exit erf 2\n");
	}
	// fprintf(stderr, "[s_erf.c] exit erf 1\n");

	if(ix < 0x3feb0000) {		
	    fprintf(stderr, "[s_erf.c] enter erf 3\n");
	    if(ix < 0x3e300000) { 	
	        fprintf(stderr, "[s_erf.c] enter erf 4\n");
	        if (ix < 0x00800000) 
		{
		    fprintf(stderr, "[s_erf.c] enter erf 5\n");
		    return 0.125*(8.0*x+efx8*x);
		    // fprintf(stderr, "[s_erf.c] exit erf 5\n");
		}
		// fprintf(stderr, "[s_erf.c] exit erf 4\n");
		fprintf(stderr, "[s_erf.c] enter erf 6\n");
		return x + efx*x;
		// fprintf(stderr, "[s_erf.c] exit erf 6\n");
	    }
	    // fprintf(stderr, "[s_erf.c] exit erf 3\n");
	    fprintf(stderr, "[s_erf.c] enter erf 7\n");
	    z = x*x;
	    r = pp0+z*(pp1+z*(pp2+z*(pp3+z*pp4)));
	    s = one+z*(qq1+z*(qq2+z*(qq3+z*(qq4+z*qq5))));
	    y = r/s;
	    return x + x*y;
	    // fprintf(stderr, "[s_erf.c] exit erf 7\n");
	}
	if(ix < 0x3ff40000) {		
	    fprintf(stderr, "[s_erf.c] enter erf 8\n");
	    s = fabs(x)-one;
	    P = pa0+s*(pa1+s*(pa2+s*(pa3+s*(pa4+s*(pa5+s*pa6)))));
	    Q = one+s*(qa1+s*(qa2+s*(qa3+s*(qa4+s*(qa5+s*qa6)))));
	    if(hx>=0) 
	    {
		fprintf(stderr, "[s_erf.c] enter erf 9\n");
		return erx + P/Q;
		// fprintf(stderr, "[s_erf.c] exit erf 9\n");
	    } 
	    else 
	    {
		fprintf(stderr, "[s_erf.c] enter erf 10\n");
		return -erx - P/Q;
		// fprintf(stderr, "[s_erf.c] exit erf 10\n");
	    }
	    // fprintf(stderr, "[s_erf.c] exit erf 8\n");
	}
	if (ix >= 0x40180000) {		
	    fprintf(stderr, "[s_erf.c] enter erf 11\n");
	    if(hx>=0) 
	    {
		fprintf(stderr, "[s_erf.c] enter erf 12\n");
		return one-tiny;
		// fprintf(stderr, "[s_erf.c] exit erf 12\n");
	    } 
	    else 
	    {
		fprintf(stderr, "[s_erf.c] enter erf 13\n");
		return tiny-one;
		// fprintf(stderr, "[s_erf.c] exit erf 13\n");
	    }
	    // fprintf(stderr, "[s_erf.c] exit erf 11\n");
	}
	fprintf(stderr, "[s_erf.c] enter erf 14\n");
	x = fabs(x);
 	s = one/(x*x);
	if(ix< 0x4006DB6E) {	
	    fprintf(stderr, "[s_erf.c] enter erf 15\n");
	    R=ra0+s*(ra1+s*(ra2+s*(ra3+s*(ra4+s*(
				ra5+s*(ra6+s*ra7))))));
	    S=one+s*(sa1+s*(sa2+s*(sa3+s*(sa4+s*(
				sa5+s*(sa6+s*(sa7+s*sa8)))))));
	    // fprintf(stderr, "[s_erf.c] exit erf 15\n");
	} else {	
	    fprintf(stderr, "[s_erf.c] enter erf 16\n");
	    R=rb0+s*(rb1+s*(rb2+s*(rb3+s*(ra4+s*(
				rb5+s*rb6)))));
	    S=one+s*(sb1+s*(sb2+s*(sb3+s*(sb4+s*(
				sb5+s*(sb6+s*sb7))))));
	    // fprintf(stderr, "[s_erf.c] exit erf 16\n");
	}
	fprintf(stderr, "[s_erf.c] enter erf 17\n");
	z  = x;  
	__LO(z) = 0;
	r  =  __ieee754_exp(-z*z-0.5625)*__ieee754_exp((z-x)*(z+x)+R/S);
	if(hx>=0) 
	{
	    fprintf(stderr, "[s_erf.c] enter erf 18\n");
	    return one-r/x;
	    // fprintf(stderr, "[s_erf.c] exit erf 18\n");
	} 
	else 
	{
	    fprintf(stderr, "[s_erf.c] enter erf 19\n");
	    return  r/x-one;
	    // fprintf(stderr, "[s_erf.c] exit erf 19\n");
	}
	// fprintf(stderr, "[s_erf.c] exit erf 17\n");
	// fprintf(stderr, "[s_erf.c] exit erf 14\n");
}

#ifdef __STDC__
	double erfc(double x) 
#else
	double erfc(x) 
	double x;
#endif
{
	fprintf(stderr, "[s_erf.c] enter erfc 1\n");
	int hx,ix;
	double R,S,P,Q,s,y,z,r;
	hx = __HI(x);
	ix = hx&0x7fffffff;
	if(ix>=0x7ff00000) {			
	    fprintf(stderr, "[s_erf.c] enter erfc 2\n");
	    return (double)(((unsigned)hx>>31)<<1)+one/x;
	    // fprintf(stderr, "[s_erf.c] exit erfc 2\n");
	}
	// fprintf(stderr, "[s_erf.c] exit erfc 1\n");

	if(ix < 0x3feb0000) {		
	    fprintf(stderr, "[s_erf.c] enter erfc 3\n");
	    if(ix < 0x3c700000)  	
	    {
		fprintf(stderr, "[s_erf.c] enter erfc 4\n");
		return one-x;
		// fprintf(stderr, "[s_erf.c] exit erfc 4\n");
	    }
	    // fprintf(stderr, "[s_erf.c] exit erfc 3\n");
	    fprintf(stderr, "[s_erf.c] enter erfc 5\n");
	    z = x*x;
	    r = pp0+z*(pp1+z*(pp2+z*(pp3+z*pp4)));
	    s = one+z*(qq1+z*(qq2+z*(qq3+z*(qq4+z*qq5))));
	    y = r/s;
	    if(hx < 0x3fd00000) {  	
		fprintf(stderr, "[s_erf.c] enter erfc 6\n");
		return one-(x+x*y);
		// fprintf(stderr, "[s_erf.c] exit erfc 6\n");
	    } else {
		fprintf(stderr, "[s_erf.c] enter erfc 7\n");
		r = x*y;
		r += (x-half);
	        return half - r ;
		// fprintf(stderr, "[s_erf.c] exit erfc 7\n");
	    }
	    // fprintf(stderr, "[s_erf.c] exit erfc 5\n");
	}
	if(ix < 0x3ff40000) {		
	    fprintf(stderr, "[s_erf.c] enter erfc 8\n");
	    s = fabs(x)-one;
	    P = pa0+s*(pa1+s*(pa2+s*(pa3+s*(pa4+s*(pa5+s*pa6)))));
	    Q = one+s*(qa1+s*(qa2+s*(qa3+s*(qa4+s*(qa5+s*qa6)))));
	    if(hx>=0) {
		fprintf(stderr, "[s_erf.c] enter erfc 9\n");
	        z  = one-erx; return z - P/Q;
		// fprintf(stderr, "[s_erf.c] exit erfc 9\n");
	    } else {
		fprintf(stderr, "[s_erf.c] enter erfc 10\n");
		z = erx+P/Q; return one+z;
		// fprintf(stderr, "[s_erf.c] exit erfc 10\n");
	    }
	    // fprintf(stderr, "[s_erf.c] exit erfc 8\n");
	}
	if (ix < 0x403c0000) {		
	    fprintf(stderr, "[s_erf.c] enter erfc 11\n");
	    x = fabs(x);
 	    s = one/(x*x);
	    if(ix< 0x4006DB6D) {	
	        fprintf(stderr, "[s_erf.c] enter erfc 12\n");
	        R=ra0+s*(ra1+s*(ra2+s*(ra3+s*(ra4+s*(
				ra5+s*(ra6+s*ra7))))));
	        S=one+s*(sa1+s*(sa2+s*(sa3+s*(sa4+s*(
				sa5+s*(sa6+s*(sa7+s*sa8)))))));
	        // fprintf(stderr, "[s_erf.c] exit erfc 12\n");
	    } else {			
		fprintf(stderr, "[s_erf.c] enter erfc 13\n");
		if(hx<0&&ix>=0x40180000) 
		{
		    fprintf(stderr, "[s_erf.c] enter erfc 14\n");
		    return two-tiny;
		    // fprintf(stderr, "[s_erf.c] exit erfc 14\n");
		}
		// fprintf(stderr, "[s_erf.c] exit erfc 13\n");
		fprintf(stderr, "[s_erf.c] enter erfc 15\n");
	        R=rb0+s*(rb1+s*(rb2+s*(rb3+s*(ra4+s*(
				rb5+s*rb6)))));
	        S=one+s*(sb1+s*(sb2+s*(sb3+s*(sb4+s*(
				sb5+s*(sb6+s*sb7))))));
	        // fprintf(stderr, "[s_erf.c] exit erfc 15\n");
	    }
	    fprintf(stderr, "[s_erf.c] enter erfc 16\n");
	    z  = x;
	    __LO(z)  = 0;
	    r  =  __ieee754_exp(-z*z-0.5625)*
			__ieee754_exp((z-x)*(z+x)+R/S);
	    if(hx>0) 
	    {
		fprintf(stderr, "[s_erf.c] enter erfc 17\n");
		return r/x;
		// fprintf(stderr, "[s_erf.c] exit erfc 17\n");
	    } 
	    else 
	    {
		fprintf(stderr, "[s_erf.c] enter erfc 18\n");
		return two-r/x;
		// fprintf(stderr, "[s_erf.c] exit erfc 18\n");
	    }
	    // fprintf(stderr, "[s_erf.c] exit erfc 16\n");
	} else {
	    fprintf(stderr, "[s_erf.c] enter erfc 19\n");
	    if(hx>0) 
	    {
		fprintf(stderr, "[s_erf.c] enter erfc 20\n");
		return tiny*tiny;
		// fprintf(stderr, "[s_erf.c] exit erfc 20\n");
	    } 
	    else 
	    {
		fprintf(stderr, "[s_erf.c] enter erfc 21\n");
		return two-tiny;
		// fprintf(stderr, "[s_erf.c] exit erfc 21\n");
	    }
	    // fprintf(stderr, "[s_erf.c] exit erfc 19\n");
	}
	// fprintf(stderr, "[s_erf.c] exit erfc 11\n");
}

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
    fprintf(stderr, "[s_erf.c] enter main 1\n");

    if (argc != 2) {
        fprintf(stderr, "[s_erf.c] enter main 2\n");
        printf("Usage: %s <arg>\n", argv[0]);
        return 1;
        // fprintf(stderr, "[s_erf.c] exit main 2\n");
    }
    // fprintf(stderr, "[s_erf.c] exit main 1\n");

    fprintf(stderr, "[s_erf.c] enter main 3\n");
    double a0 = atof(argv[1]);
    // fprintf(stderr, "[s_erf.c] exit main 3\n");

    fprintf(stderr, "[s_erf.c] enter main 4\n");
    double r = erf(a0);
    // fprintf(stderr, "[s_erf.c] exit main 4\n");

    fprintf(stderr, "[s_erf.c] enter main 5\n");
    double r1 = erfc(a0);
    // fprintf(stderr, "[s_erf.c] exit main 5\n");

    fprintf(stderr, "[s_erf.c] enter main 6\n");
    printf("%f\n", r);
    // fprintf(stderr, "[s_erf.c] exit main 6\n");

    fprintf(stderr, "[s_erf.c] enter main 7\n");
    printf("%f\n", r1);
    // fprintf(stderr, "[s_erf.c] exit main 7\n");

    fprintf(stderr, "[s_erf.c] enter main 8\n");
    return 0;
    // fprintf(stderr, "[s_erf.c] exit main 8\n");
}
// Total cost: 0.012734
// Total split cost: 0.000000, input tokens: 0, output tokens: 0, cache read tokens: 0, cache write tokens: 0, split chunks: [(0, 221)]
// Total instrumented cost: 0.012734, input tokens: 13547, output tokens: 8251, cache read tokens: 0, cache write tokens: 0
