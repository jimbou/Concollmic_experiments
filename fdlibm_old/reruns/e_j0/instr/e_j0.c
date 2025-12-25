





#include "fdlibm.h"
#include <stdio.h>

#ifdef __STDC__
static double pzero(double), qzero(double);
#else
static double pzero(), qzero();
#endif

#ifdef __STDC__
static const double 
#else
static double 
#endif
huge 	= 1e300,
one	= 1.0,
invsqrtpi=  5.64189583547756279280e-01, 
tpi      =  6.36619772367581382433e-01, 
 		
R02  =  1.56249999999999947958e-02, 
R03  = -1.89979294238854721751e-04, 
R04  =  1.82954049532700665670e-06, 
R05  = -4.61832688532103189199e-09, 
S01  =  1.56191029464890010492e-02, 
S02  =  1.16926784663337450260e-04, 
S03  =  5.13546550207318111446e-07, 
S04  =  1.16614003333790000205e-09; 

static double zero = 0.0;

#ifdef __STDC__
	double __ieee754_j0(double x) 
#else
	double __ieee754_j0(x) 
	double x;
#endif
{
	fprintf(stderr, "[e_j0.c] enter __ieee754_j0 1\n");
	double z, s,c,ss,cc,r,u,v;
	int hx,ix;

	hx = __HI(x);
	ix = hx&0x7fffffff;
	if(ix>=0x7ff00000) {
		fprintf(stderr, "[e_j0.c] enter __ieee754_j0 2\n");
		return one/(x*x);
		// fprintf(stderr, "[e_j0.c] exit __ieee754_j0 2\n");
	}
	fprintf(stderr, "[e_j0.c] enter __ieee754_j0 3\n");
	x = fabs(x);
	if(ix >= 0x40000000) {	
		fprintf(stderr, "[e_j0.c] enter __ieee754_j0 4\n");
		s = sin(x);
		c = cos(x);
		ss = s-c;
		cc = s+c;
		if(ix<0x7fe00000) {  
			fprintf(stderr, "[e_j0.c] enter __ieee754_j0 5\n");
		    z = -cos(x+x);
		    if ((s*c)<zero) {
				fprintf(stderr, "[e_j0.c] enter __ieee754_j0 6\n");
		    	cc = z/ss;
				// fprintf(stderr, "[e_j0.c] exit __ieee754_j0 6\n");
			}
		    else {
				fprintf(stderr, "[e_j0.c] enter __ieee754_j0 7\n");
		    	ss = z/cc;
				// fprintf(stderr, "[e_j0.c] exit __ieee754_j0 7\n");
			}
			// fprintf(stderr, "[e_j0.c] exit __ieee754_j0 5\n");
		}
	
		if(ix>0x48000000) {
			fprintf(stderr, "[e_j0.c] enter __ieee754_j0 8\n");
			z = (invsqrtpi*cc)/sqrt(x);
			// fprintf(stderr, "[e_j0.c] exit __ieee754_j0 8\n");
		}
		else {
			fprintf(stderr, "[e_j0.c] enter __ieee754_j0 9\n");
		    u = pzero(x); v = qzero(x);
		    z = invsqrtpi*(u*cc-v*ss)/sqrt(x);
			// fprintf(stderr, "[e_j0.c] exit __ieee754_j0 9\n");
		}
		fprintf(stderr, "[e_j0.c] enter __ieee754_j0 10\n");
		return z;
		// fprintf(stderr, "[e_j0.c] exit __ieee754_j0 10\n");
		// fprintf(stderr, "[e_j0.c] exit __ieee754_j0 4\n");
	}
	fprintf(stderr, "[e_j0.c] enter __ieee754_j0 11\n");
	if(ix<0x3f200000) {	
		fprintf(stderr, "[e_j0.c] enter __ieee754_j0 12\n");
	    if(huge+x>one) {	
			fprintf(stderr, "[e_j0.c] enter __ieee754_j0 13\n");
	        if(ix<0x3e400000) {
				fprintf(stderr, "[e_j0.c] enter __ieee754_j0 14\n");
	        	return one;
				// fprintf(stderr, "[e_j0.c] exit __ieee754_j0 14\n");
			}
	        else {
				fprintf(stderr, "[e_j0.c] enter __ieee754_j0 15\n");
	        	return one - 0.25*x*x;
				// fprintf(stderr, "[e_j0.c] exit __ieee754_j0 15\n");
			}
			// fprintf(stderr, "[e_j0.c] exit __ieee754_j0 13\n");
	    }
		// fprintf(stderr, "[e_j0.c] exit __ieee754_j0 12\n");
	}
	fprintf(stderr, "[e_j0.c] enter __ieee754_j0 16\n");
	z = x*x;
	r =  z*(R02+z*(R03+z*(R04+z*R05)));
	s =  one+z*(S01+z*(S02+z*(S03+z*S04)));
	if(ix < 0x3FF00000) {	
		fprintf(stderr, "[e_j0.c] enter __ieee754_j0 17\n");
	    return one + z*(-0.25+(r/s));
		// fprintf(stderr, "[e_j0.c] exit __ieee754_j0 17\n");
	} else {
		fprintf(stderr, "[e_j0.c] enter __ieee754_j0 18\n");
	    u = 0.5*x;
	    return((one+u)*(one-u)+z*(r/s));
		// fprintf(stderr, "[e_j0.c] exit __ieee754_j0 18\n");
	}
	// fprintf(stderr, "[e_j0.c] exit __ieee754_j0 16\n");
	// fprintf(stderr, "[e_j0.c] exit __ieee754_j0 11\n");
	// fprintf(stderr, "[e_j0.c] exit __ieee754_j0 3\n");
	// fprintf(stderr, "[e_j0.c] exit __ieee754_j0 1\n");
}

#ifdef __STDC__
static const double
#else
static double
#endif
u00  = -7.38042951086872317523e-02, 
u01  =  1.76666452509181115538e-01, 
u02  = -1.38185671945596898896e-02, 
u03  =  3.47453432093683650238e-04, 
u04  = -3.81407053724364161125e-06, 
u05  =  1.95590137035022920206e-08, 
u06  = -3.98205194132103398453e-11, 
v01  =  1.27304834834123699328e-02, 
v02  =  7.60068627350353253702e-05, 
v03  =  2.59150851840457805467e-07, 
v04  =  4.41110311332675467403e-10; 

#ifdef __STDC__
	double __ieee754_y0(double x) 
#else
	double __ieee754_y0(x) 
	double x;
#endif
{
	fprintf(stderr, "[e_j0.c] enter __ieee754_y0 1\n");
	double z, s,c,ss,cc,u,v;
	int hx,ix,lx;

        hx = __HI(x);
        ix = 0x7fffffff&hx;
        lx = __LO(x);
    
	if(ix>=0x7ff00000) {
		fprintf(stderr, "[e_j0.c] enter __ieee754_y0 2\n");
		return  one/(x+x*x);
		// fprintf(stderr, "[e_j0.c] exit __ieee754_y0 2\n");
	}
	fprintf(stderr, "[e_j0.c] enter __ieee754_y0 3\n");
        if((ix|lx)==0) {
		fprintf(stderr, "[e_j0.c] enter __ieee754_y0 4\n");
        	return -one/zero;
		// fprintf(stderr, "[e_j0.c] exit __ieee754_y0 4\n");
	}
	fprintf(stderr, "[e_j0.c] enter __ieee754_y0 5\n");
        if(hx<0) {
		fprintf(stderr, "[e_j0.c] enter __ieee754_y0 6\n");
        	return zero/zero;
		// fprintf(stderr, "[e_j0.c] exit __ieee754_y0 6\n");
	}
	fprintf(stderr, "[e_j0.c] enter __ieee754_y0 7\n");
        if(ix >= 0x40000000) {  
        
			fprintf(stderr, "[e_j0.c] enter __ieee754_y0 8\n");
                s = sin(x);
                c = cos(x);
                ss = s-c;
                cc = s+c;
	
                if(ix<0x7fe00000) {  
					fprintf(stderr, "[e_j0.c] enter __ieee754_y0 9\n");
                    z = -cos(x+x);
                    if ((s*c)<zero) {
						fprintf(stderr, "[e_j0.c] enter __ieee754_y0 10\n");
                    	cc = z/ss;
						// fprintf(stderr, "[e_j0.c] exit __ieee754_y0 10\n");
					}
                    else {
						fprintf(stderr, "[e_j0.c] enter __ieee754_y0 11\n");
                    	ss = z/cc;
						// fprintf(stderr, "[e_j0.c] exit __ieee754_y0 11\n");
					}
					// fprintf(stderr, "[e_j0.c] exit __ieee754_y0 9\n");
                }
                if(ix>0x48000000) {
					fprintf(stderr, "[e_j0.c] enter __ieee754_y0 12\n");
                	z = (invsqrtpi*ss)/sqrt(x);
					// fprintf(stderr, "[e_j0.c] exit __ieee754_y0 12\n");
				}
                else {
					fprintf(stderr, "[e_j0.c] enter __ieee754_y0 13\n");
                    u = pzero(x); v = qzero(x);
                    z = invsqrtpi*(u*ss+v*cc)/sqrt(x);
					// fprintf(stderr, "[e_j0.c] exit __ieee754_y0 13\n");
                }
				fprintf(stderr, "[e_j0.c] enter __ieee754_y0 14\n");
                return z;
				// fprintf(stderr, "[e_j0.c] exit __ieee754_y0 14\n");
				// fprintf(stderr, "[e_j0.c] exit __ieee754_y0 8\n");
		}
	fprintf(stderr, "[e_j0.c] enter __ieee754_y0 15\n");
	if(ix<=0x3e400000) {	
		fprintf(stderr, "[e_j0.c] enter __ieee754_y0 16\n");
	    return(u00 + tpi*__ieee754_log(x));
		// fprintf(stderr, "[e_j0.c] exit __ieee754_y0 16\n");
	}
	fprintf(stderr, "[e_j0.c] enter __ieee754_y0 17\n");
	z = x*x;
	u = u00+z*(u01+z*(u02+z*(u03+z*(u04+z*(u05+z*u06)))));
	v = one+z*(v01+z*(v02+z*(v03+z*v04)));
	fprintf(stderr, "[e_j0.c] enter __ieee754_y0 18\n");
	return(u/v + tpi*(__ieee754_j0(x)*__ieee754_log(x)));
	// fprintf(stderr, "[e_j0.c] exit __ieee754_y0 18\n");
	// fprintf(stderr, "[e_j0.c] exit __ieee754_y0 17\n");
	// fprintf(stderr, "[e_j0.c] exit __ieee754_y0 15\n");
	// fprintf(stderr, "[e_j0.c] exit __ieee754_y0 7\n");
	// fprintf(stderr, "[e_j0.c] exit __ieee754_y0 5\n");
	// fprintf(stderr, "[e_j0.c] exit __ieee754_y0 3\n");
	// fprintf(stderr, "[e_j0.c] exit __ieee754_y0 1\n");
}


#ifdef __STDC__
static const double pR8[6] = { 
#else
static double pR8[6] = { 
#endif
  0.00000000000000000000e+00, 
 -7.03124999999900357484e-02, 
 -8.08167041275349795626e+00, 
 -2.57063105679704847262e+02, 
 -2.48521641009428822144e+03, 
 -5.25304380490729545272e+03, 
};
#ifdef __STDC__
static const double pS8[5] = {
#else
static double pS8[5] = {
#endif
  1.16534364619668181717e+02, 
  3.83374475364121826715e+03, 
  4.05978572648472545552e+04, 
  1.16752972564375915681e+05, 
  4.76277284146730962675e+04, 
};

#ifdef __STDC__
static const double pR5[6] = { 
#else
static double pR5[6] = { 
#endif
 -1.14125464691894502584e-11, 
 -7.03124940873599280078e-02, 
 -4.15961064470587782438e+00, 
 -6.76747652265167261021e+01, 
 -3.31231299649172967747e+02, 
 -3.46433388365604912451e+02, 
};
#ifdef __STDC__
static const double pS5[5] = {
#else
static double pS5[5] = {
#endif
  6.07539382692300335975e+01, 
  1.05125230595704579173e+03, 
  5.97897094333855784498e+03, 
  9.62544514357774460223e+03, 
  2.40605815922939109441e+03, 
};

#ifdef __STDC__
static const double pR3[6] = {
#else
static double pR3[6] = {
#endif
 -2.54704601771951915620e-09, 
 -7.03119616381481654654e-02, 
 -2.40903221549529611423e+00, 
 -2.19659774734883086467e+01, 
 -5.80791704701737572236e+01, 
 -3.14479470594888503854e+01, 
};
#ifdef __STDC__
static const double pS3[5] = {
#else
static double pS3[5] = {
#endif
  3.58560338055209726349e+01, 
  3.61513983050303863820e+02, 
  1.19360783792111533330e+03, 
  1.12799679856907414432e+03, 
  1.73580930813335754692e+02, 
};

#ifdef __STDC__
static const double pR2[6] = {
#else
static double pR2[6] = {
#endif
 -8.87534333032526411254e-08, 
 -7.03030995483624743247e-02, 
 -1.45073846780952986357e+00, 
 -7.63569613823527770791e+00, 
 -1.11931668860356747786e+01, 
 -3.23364579351335335033e+00, 
};
#ifdef __STDC__
static const double pS2[5] = {
#else
static double pS2[5] = {
#endif
  2.22202997532088808441e+01, 
  1.36206794218215208048e+02, 
  2.70470278658083486789e+02, 
  1.53875394208320329881e+02, 
  1.46576176948256193810e+01, 
};

#ifdef __STDC__
	static double pzero(double x)
#else
	static double pzero(x)
	double x;
#endif
{
	fprintf(stderr, "[e_j0.c] enter pzero 1\n");
#ifdef __STDC__
	const double *p,*q;
#else
	double *p,*q;
#endif
	double z,r,s;
	int ix;
	ix = 0x7fffffff&__HI(x);
	if(ix>=0x40200000)     {
		fprintf(stderr, "[e_j0.c] enter pzero 2\n");
		p = pR8; q= pS8;
		// fprintf(stderr, "[e_j0.c] exit pzero 2\n");
	}
	else if(ix>=0x40122E8B){
		fprintf(stderr, "[e_j0.c] enter pzero 3\n");
		p = pR5; q= pS5;
		// fprintf(stderr, "[e_j0.c] exit pzero 3\n");
	}
	else if(ix>=0x4006DB6D){
		fprintf(stderr, "[e_j0.c] enter pzero 4\n");
		p = pR3; q= pS3;
		// fprintf(stderr, "[e_j0.c] exit pzero 4\n");
	}
	else if(ix>=0x40000000){
		fprintf(stderr, "[e_j0.c] enter pzero 5\n");
		p = pR2; q= pS2;
		// fprintf(stderr, "[e_j0.c] exit pzero 5\n");
	}
	fprintf(stderr, "[e_j0.c] enter pzero 6\n");
	z = one/(x*x);
	r = p[0]+z*(p[1]+z*(p[2]+z*(p[3]+z*(p[4]+z*p[5]))));
	s = one+z*(q[0]+z*(q[1]+z*(q[2]+z*(q[3]+z*q[4]))));
	fprintf(stderr, "[e_j0.c] enter pzero 7\n");
	return one+ r/s;
	// fprintf(stderr, "[e_j0.c] exit pzero 7\n");
	// fprintf(stderr, "[e_j0.c] exit pzero 6\n");
	// fprintf(stderr, "[e_j0.c] exit pzero 1\n");
}
		


#ifdef __STDC__
static const double qR8[6] = { 
#else
static double qR8[6] = { 
#endif
  0.00000000000000000000e+00, 
  7.32421874999935051953e-02, 
  1.17682064682252693899e+01, 
  5.57673380256401856059e+02, 
  8.85919720756468632317e+03, 
  3.70146267776887834771e+04, 
};
#ifdef __STDC__
static const double qS8[6] = {
#else
static double qS8[6] = {
#endif
  1.63776026895689824414e+02, 
  8.09834494656449805916e+03, 
  1.42538291419120476348e+05, 
  8.03309257119514397345e+05, 
  8.40501579819060512818e+05, 
 -3.43899293537866615225e+05, 
};

#ifdef __STDC__
static const double qR5[6] = { 
#else
static double qR5[6] = { 
#endif
  1.84085963594515531381e-11, 
  7.32421766612684765896e-02, 
  5.83563508962056953777e+00, 
  1.35111577286449829671e+02, 
  1.02724376596164097464e+03, 
  1.98997785864605384631e+03, 
};
#ifdef __STDC__
static const double qS5[6] = {
#else
static double qS5[6] = {
#endif
  8.27766102236537761883e+01, 
  2.07781416421392987104e+03, 
  1.88472887785718085070e+04, 
  5.67511122894947329769e+04, 
  3.59767538425114471465e+04, 
 -5.35434275601944773371e+03, 
};

#ifdef __STDC__
static const double qR3[6] = {
#else
static double qR3[6] = {
#endif
  4.37741014089738620906e-09, 
  7.32411180042911447163e-02, 
  3.34423137516170720929e+00, 
  4.26218440745412650017e+01, 
  1.70808091340565596283e+02, 
  1.66733948696651168575e+02, 
};
#ifdef __STDC__
static const double qS3[6] = {
#else
static double qS3[6] = {
#endif
  4.87588729724587182091e+01, 
  7.09689221056606015736e+02, 
  3.70414822620111362994e+03, 
  6.46042516752568917582e+03, 
  2.51633368920368957333e+03, 
 -1.49247451836156386662e+02, 
};

#ifdef __STDC__
static const double qR2[6] = {
#else
static double qR2[6] = {
#endif
  1.50444444886983272379e-07, 
  7.32234265963079278272e-02, 
  1.99819174093815998816e+00, 
  1.44956029347885735348e+01, 
  3.16662317504781540833e+01, 
  1.62527075710929267416e+01, 
};
#ifdef __STDC__
static const double qS2[6] = {
#else
static double qS2[6] = {
#endif
  3.03655848355219184498e+01, 
  2.69348118608049844624e+02, 
  8.44783757595320139444e+02, 
  8.82935845112488550512e+02, 
  2.12666388511798828631e+02, 
 -5.31095493882666946917e+00, 
};

#ifdef __STDC__
	static double qzero(double x)
#else
	static double qzero(x)
	double x;
#endif
{
	fprintf(stderr, "[e_j0.c] enter qzero 1\n");
#ifdef __STDC__
	const double *p,*q;
#else
	double *p,*q;
#endif
	double s,r,z;
	int ix;
	ix = 0x7fffffff&__HI(x);
	if(ix>=0x40200000)     {
		fprintf(stderr, "[e_j0.c] enter qzero 2\n");
		p = qR8; q= qS8;
		// fprintf(stderr, "[e_j0.c] exit qzero 2\n");
	}
	else if(ix>=0x40122E8B){
		fprintf(stderr, "[e_j0.c] enter qzero 3\n");
		p = qR5; q= qS5;
		// fprintf(stderr, "[e_j0.c] exit qzero 3\n");
	}
	else if(ix>=0x4006DB6D){
		fprintf(stderr, "[e_j0.c] enter qzero 4\n");
		p = qR3; q= qS3;
		// fprintf(stderr, "[e_j0.c] exit qzero 4\n");
	}
	else if(ix>=0x40000000){
		fprintf(stderr, "[e_j0.c] enter qzero 5\n");
		p = qR2; q= qS2;
		// fprintf(stderr, "[e_j0.c] exit qzero 5\n");
	}
	fprintf(stderr, "[e_j0.c] enter qzero 6\n");
	z = one/(x*x);
	r = p[0]+z*(p[1]+z*(p[2]+z*(p[3]+z*(p[4]+z*p[5]))));
	s = one+z*(q[0]+z*(q[1]+z*(q[2]+z*(q[3]+z*(q[4]+z*q[5])))));
	fprintf(stderr, "[e_j0.c] enter qzero 7\n");
	return (-.125 + r/s)/x;
	// fprintf(stderr, "[e_j0.c] exit qzero 7\n");
	// fprintf(stderr, "[e_j0.c] exit qzero 6\n");
	// fprintf(stderr, "[e_j0.c] exit qzero 1\n");
}

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
	fprintf(stderr, "[e_j0.c] enter main 1\n");

    if (argc != 2) {
		fprintf(stderr, "[e_j0.c] enter main 2\n");
        printf("Usage: %s <arg>\n", argv[0]);
        return 1;
		// fprintf(stderr, "[e_j0.c] exit main 2\n");
    }

	fprintf(stderr, "[e_j0.c] enter main 3\n");
    double a0 = atof(argv[1]);

    double r = __ieee754_j0(a0);
    double r1 = __ieee754_y0(a0);
    printf("%f\n", r);
    printf("%f\n", r1);

	fprintf(stderr, "[e_j0.c] enter main 4\n");
    return 0;
	// fprintf(stderr, "[e_j0.c] exit main 4\n");
	// fprintf(stderr, "[e_j0.c] exit main 3\n");
	// fprintf(stderr, "[e_j0.c] exit main 1\n");
}
// Total cost: 0.048702
// Total split cost: 0.000000, input tokens: 0, output tokens: 0, cache read tokens: 0, cache write tokens: 0, split chunks: [(0, 414)]
// Total instrumented cost: 0.048702, input tokens: 64243, output tokens: 28506, cache read tokens: 0, cache write tokens: 0
