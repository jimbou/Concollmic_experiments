





#include "fdlibm.h"
#include <stdio.h>

#ifdef __STDC__
static const double atanhi[] = {
#else
static double atanhi[] = {
#endif
  4.63647609000806093515e-01, 
  7.85398163397448278999e-01, 
  9.82793723247329054082e-01, 
  1.57079632679489655800e+00, 
};

#ifdef __STDC__
static const double atanlo[] = {
#else
static double atanlo[] = {
#endif
  2.26987774529616870924e-17, 
  3.06161699786838301793e-17, 
  1.39033110312309984516e-17, 
  6.12323399573676603587e-17, 
};

#ifdef __STDC__
static const double aT[] = {
#else
static double aT[] = {
#endif
  3.33333333333329318027e-01, 
 -1.99999999998764832476e-01, 
  1.42857142725034663711e-01, 
 -1.11111104054623557880e-01, 
  9.09088713343650656196e-02, 
 -7.69187620504482999495e-02, 
  6.66107313738753120669e-02, 
 -5.83357013379057348645e-02, 
  4.97687799461593236017e-02, 
 -3.65315727442169155270e-02, 
  1.62858201153657823623e-02, 
};

#ifdef __STDC__
	static const double 
#else
	static double 
#endif
one   = 1.0,
huge   = 1.0e300;

#ifdef __STDC__
	double atan(double x)
#else
	double atan(x)
	double x;
#endif
{
	fprintf(stderr, "[s_atan.c] enter atan 1\n");
	double w,s1,s2,z;
	int ix,hx,id;

	hx = __HI(x);
	ix = hx&0x7fffffff;
	// fprintf(stderr, "[s_atan.c] exit atan 1\n");
	
	if(ix>=0x44100000) {	
	fprintf(stderr, "[s_atan.c] enter atan 2\n");
	    if(ix>0x7ff00000||
		(ix==0x7ff00000&&(__LO(x)!=0)))
		return x+x;		
	    if(hx>0) return  atanhi[3]+atanlo[3];
	    else     return -atanhi[3]-atanlo[3];
	// fprintf(stderr, "[s_atan.c] exit atan 2\n");
	}
	
	if (ix < 0x3fdc0000) {	
	fprintf(stderr, "[s_atan.c] enter atan 3\n");
	    if (ix < 0x3e200000) {	
	fprintf(stderr, "[s_atan.c] enter atan 4\n");
		if(huge+x>one) return x;	
	// fprintf(stderr, "[s_atan.c] exit atan 4\n");
	    }
	    id = -1;
	// fprintf(stderr, "[s_atan.c] exit atan 3\n");
	} else {
	fprintf(stderr, "[s_atan.c] enter atan 5\n");
	x = fabs(x);
	// fprintf(stderr, "[s_atan.c] exit atan 5\n");
	
	if (ix < 0x3ff30000) {		
	fprintf(stderr, "[s_atan.c] enter atan 6\n");
	    if (ix < 0x3fe60000) {	
	fprintf(stderr, "[s_atan.c] enter atan 7\n");
		id = 0; x = (2.0*x-one)/(2.0+x); 
	// fprintf(stderr, "[s_atan.c] exit atan 7\n");
	    } else {			
	fprintf(stderr, "[s_atan.c] enter atan 8\n");
		id = 1; x  = (x-one)/(x+one); 
	// fprintf(stderr, "[s_atan.c] exit atan 8\n");
	    }
	// fprintf(stderr, "[s_atan.c] exit atan 6\n");
	} else {
	fprintf(stderr, "[s_atan.c] enter atan 9\n");
	    if (ix < 0x40038000) {	
	fprintf(stderr, "[s_atan.c] enter atan 10\n");
		id = 2; x  = (x-1.5)/(one+1.5*x);
	// fprintf(stderr, "[s_atan.c] exit atan 10\n");
	    } else {			
	fprintf(stderr, "[s_atan.c] enter atan 11\n");
		id = 3; x  = -1.0/x;
	// fprintf(stderr, "[s_atan.c] exit atan 11\n");
	    }
	// fprintf(stderr, "[s_atan.c] exit atan 9\n");
	}}
    
	fprintf(stderr, "[s_atan.c] enter atan 12\n");
	z = x*x;
	w = z*z;
    
	s1 = z*(aT[0]+w*(aT[2]+w*(aT[4]+w*(aT[6]+w*(aT[8]+w*aT[10])))));
	s2 = w*(aT[1]+w*(aT[3]+w*(aT[5]+w*(aT[7]+w*aT[9]))));
	// fprintf(stderr, "[s_atan.c] exit atan 12\n");
	
	if (id<0) {
	fprintf(stderr, "[s_atan.c] enter atan 13\n");
	return x - x*(s1+s2);
	// fprintf(stderr, "[s_atan.c] exit atan 13\n");
	}
	else {
	fprintf(stderr, "[s_atan.c] enter atan 14\n");
	    z = atanhi[id] - ((x*(s1+s2) - atanlo[id]) - x);
	    return (hx<0)? -z:z;
	// fprintf(stderr, "[s_atan.c] exit atan 14\n");
	}
}

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
	fprintf(stderr, "[s_atan.c] enter main 1\n");
    if (argc != 2) {
	fprintf(stderr, "[s_atan.c] enter main 2\n");
        printf("Usage: %s <arg>\n", argv[0]);
        return 1;
	// fprintf(stderr, "[s_atan.c] exit main 2\n");
    }

	fprintf(stderr, "[s_atan.c] enter main 3\n");
    double a0 = atof(argv[1]);

    double r = atan(a0);
    printf("%f\n", r);

    return 0;
	// fprintf(stderr, "[s_atan.c] exit main 3\n");
	// fprintf(stderr, "[s_atan.c] exit main 1\n");
}
// Total cost: 0.005755
// Total split cost: 0.000000, input tokens: 0, output tokens: 0, cache read tokens: 0, cache write tokens: 0, split chunks: [(0, 124)]
// Total instrumented cost: 0.005755, input tokens: 7946, output tokens: 3281, cache read tokens: 0, cache write tokens: 0
