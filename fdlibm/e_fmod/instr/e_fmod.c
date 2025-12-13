#include <stdio.h>

#include "fdlibm.h"

#ifdef __STDC__
static const double one = 1.0, Zero[] = {0.0, -0.0,};
#else
static double one = 1.0, Zero[] = {0.0, -0.0,};
#endif

#ifdef __STDC__
	double __ieee754_fmod(double x, double y)
#else
	double __ieee754_fmod(x,y)
	double x,y ;
#endif
{
	fprintf(stderr, "[e_fmod.c] enter __ieee754_fmod 1\n");
	int n,hx,hy,hz,ix,iy,sx,i;
	unsigned lx,ly,lz;

	hx = __HI(x);		
	lx = __LO(x);		
	hy = __HI(y);		
	ly = __LO(y);		
	sx = hx&0x80000000;		
	hx ^=sx;		
	hy &= 0x7fffffff;	
	// fprintf(stderr, "[e_fmod.c] exit __ieee754_fmod 1\n");

    
	if((hy|ly)==0||(hx>=0x7ff00000)||	
	  ((hy|((ly|-ly)>>31))>0x7ff00000))	
	{
		fprintf(stderr, "[e_fmod.c] enter __ieee754_fmod 2\n");
		return (x*y)/(x*y);
		// fprintf(stderr, "[e_fmod.c] exit __ieee754_fmod 2\n");
	}
	fprintf(stderr, "[e_fmod.c] enter __ieee754_fmod 3\n");
	if(hx<=hy) {
		fprintf(stderr, "[e_fmod.c] enter __ieee754_fmod 4\n");
	    if((hx<hy)||(lx<ly)) {
			fprintf(stderr, "[e_fmod.c] enter __ieee754_fmod 5\n");
			return x;
			// fprintf(stderr, "[e_fmod.c] exit __ieee754_fmod 5\n");
		}
		fprintf(stderr, "[e_fmod.c] enter __ieee754_fmod 6\n");
	    if(lx==ly) 
		{
			fprintf(stderr, "[e_fmod.c] enter __ieee754_fmod 7\n");
			return Zero[(unsigned)sx>>31];
			// fprintf(stderr, "[e_fmod.c] exit __ieee754_fmod 7\n");
		}
		// fprintf(stderr, "[e_fmod.c] exit __ieee754_fmod 6\n");
		// fprintf(stderr, "[e_fmod.c] exit __ieee754_fmod 4\n");
	}
	// fprintf(stderr, "[e_fmod.c] exit __ieee754_fmod 3\n");

    
	fprintf(stderr, "[e_fmod.c] enter __ieee754_fmod 8\n");
	if(hx<0x00100000) {	
		fprintf(stderr, "[e_fmod.c] enter __ieee754_fmod 9\n");
	    if(hx==0) {
			fprintf(stderr, "[e_fmod.c] enter __ieee754_fmod 10\n");
			for (ix = -1043, i=lx; i>0; i<<=1) ix -=1;
			// fprintf(stderr, "[e_fmod.c] exit __ieee754_fmod 10\n");
	    } else {
			fprintf(stderr, "[e_fmod.c] enter __ieee754_fmod 11\n");
			for (ix = -1022,i=(hx<<11); i>0; i<<=1) ix -=1;
			// fprintf(stderr, "[e_fmod.c] exit __ieee754_fmod 11\n");
	    }
		// fprintf(stderr, "[e_fmod.c] exit __ieee754_fmod 9\n");
	} else {
		fprintf(stderr, "[e_fmod.c] enter __ieee754_fmod 12\n");
		ix = (hx>>20)-1023;
		// fprintf(stderr, "[e_fmod.c] exit __ieee754_fmod 12\n");
	}
	// fprintf(stderr, "[e_fmod.c] exit __ieee754_fmod 8\n");

    
	fprintf(stderr, "[e_fmod.c] enter __ieee754_fmod 13\n");
	if(hy<0x00100000) {	
		fprintf(stderr, "[e_fmod.c] enter __ieee754_fmod 14\n");
	    if(hy==0) {
			fprintf(stderr, "[e_fmod.c] enter __ieee754_fmod 15\n");
			for (iy = -1043, i=ly; i>0; i<<=1) iy -=1;
			// fprintf(stderr, "[e_fmod.c] exit __ieee754_fmod 15\n");
	    } else {
			fprintf(stderr, "[e_fmod.c] enter __ieee754_fmod 16\n");
			for (iy = -1022,i=(hy<<11); i>0; i<<=1) iy -=1;
			// fprintf(stderr, "[e_fmod.c] exit __ieee754_fmod 16\n");
	    }
		// fprintf(stderr, "[e_fmod.c] exit __ieee754_fmod 14\n");
	} else {
		fprintf(stderr, "[e_fmod.c] enter __ieee754_fmod 17\n");
		iy = (hy>>20)-1023;
		// fprintf(stderr, "[e_fmod.c] exit __ieee754_fmod 17\n");
	}
	// fprintf(stderr, "[e_fmod.c] exit __ieee754_fmod 13\n");

    
	fprintf(stderr, "[e_fmod.c] enter __ieee754_fmod 18\n");
	if(ix >= -1022) 
	{
		fprintf(stderr, "[e_fmod.c] enter __ieee754_fmod 19\n");
	    hx = 0x00100000|(0x000fffff&hx);
		// fprintf(stderr, "[e_fmod.c] exit __ieee754_fmod 19\n");
	}
	else {		
		fprintf(stderr, "[e_fmod.c] enter __ieee754_fmod 20\n");
	    n = -1022-ix;
	    if(n<=31) {
			fprintf(stderr, "[e_fmod.c] enter __ieee754_fmod 21\n");
	        hx = (hx<<n)|(lx>>(32-n));
	        lx <<= n;
			// fprintf(stderr, "[e_fmod.c] exit __ieee754_fmod 21\n");
	    } else {
			fprintf(stderr, "[e_fmod.c] enter __ieee754_fmod 22\n");
			hx = lx<<(n-32);
			lx = 0;
			// fprintf(stderr, "[e_fmod.c] exit __ieee754_fmod 22\n");
	    }
		// fprintf(stderr, "[e_fmod.c] exit __ieee754_fmod 20\n");
	}
	// fprintf(stderr, "[e_fmod.c] exit __ieee754_fmod 18\n");
	fprintf(stderr, "[e_fmod.c] enter __ieee754_fmod 23\n");
	if(iy >= -1022) 
	{
		fprintf(stderr, "[e_fmod.c] enter __ieee754_fmod 24\n");
	    hy = 0x00100000|(0x000fffff&hy);
		// fprintf(stderr, "[e_fmod.c] exit __ieee754_fmod 24\n");
	}
	else {		
		fprintf(stderr, "[e_fmod.c] enter __ieee754_fmod 25\n");
	    n = -1022-iy;
	    if(n<=31) {
			fprintf(stderr, "[e_fmod.c] enter __ieee754_fmod 26\n");
	        hy = (hy<<n)|(ly>>(32-n));
	        ly <<= n;
			// fprintf(stderr, "[e_fmod.c] exit __ieee754_fmod 26\n");
	    } else {
			fprintf(stderr, "[e_fmod.c] enter __ieee754_fmod 27\n");
			hy = ly<<(n-32);
			ly = 0;
			// fprintf(stderr, "[e_fmod.c] exit __ieee754_fmod 27\n");
	    }
		// fprintf(stderr, "[e_fmod.c] exit __ieee754_fmod 25\n");
	}
	// fprintf(stderr, "[e_fmod.c] exit __ieee754_fmod 23\n");

    
	fprintf(stderr, "[e_fmod.c] enter __ieee754_fmod 28\n");
	n = ix - iy;
	while(n--) {
		fprintf(stderr, "[e_fmod.c] enter __ieee754_fmod 29\n");
	    hz=hx-hy;lz=lx-ly; if(lx<ly) hz -= 1;
	    if(hz<0){hx = hx+hx+(lx>>31); lx = lx+lx;}
	    else {
			fprintf(stderr, "[e_fmod.c] enter __ieee754_fmod 30\n");
	    	if((hz|lz)==0) 		
			{
				fprintf(stderr, "[e_fmod.c] enter __ieee754_fmod 31\n");
			    return Zero[(unsigned)sx>>31];
				// fprintf(stderr, "[e_fmod.c] exit __ieee754_fmod 31\n");
			}
	    	hx = hz+hz+(lz>>31); lx = lz+lz;
			// fprintf(stderr, "[e_fmod.c] exit __ieee754_fmod 30\n");
	    }
		// fprintf(stderr, "[e_fmod.c] exit __ieee754_fmod 29\n");
	}
	// fprintf(stderr, "[e_fmod.c] exit __ieee754_fmod 28\n");
	fprintf(stderr, "[e_fmod.c] enter __ieee754_fmod 32\n");
	hz=hx-hy;lz=lx-ly; if(lx<ly) hz -= 1;
	if(hz>=0) {hx=hz;lx=lz;}
	// fprintf(stderr, "[e_fmod.c] exit __ieee754_fmod 32\n");

    
	fprintf(stderr, "\n");
	if((hx|lx)==0) 			
	{
		fprintf(stderr, "[e_fmod.c] enter __ieee754_fmod 34\n");
	    return Zero[(unsigned)sx>>31];
		// fprintf(stderr, "[e_fmod.c] exit __ieee754_fmod 34\n");
	}
	fprintf(stderr, "[e_fmod.c] enter __ieee754_fmod 35\n");
	while(hx<0x00100000) {		
		fprintf(stderr, "[e_fmod.c] enter __ieee754_fmod 36\n");
	    hx = hx+hx+(lx>>31); lx = lx+lx;
	    iy -= 1;
		// fprintf(stderr, "[e_fmod.c] exit __ieee754_fmod 36\n");
	}
	// fprintf(stderr, "[e_fmod.c] exit __ieee754_fmod 35\n");
	fprintf(stderr, "\n");
	if(iy>= -1022) {	
		fprintf(stderr, "[e_fmod.c] enter __ieee754_fmod 38\n");
	    hx = ((hx-0x00100000)|((iy+1023)<<20));
	    __HI(x) = hx|sx;
	    __LO(x) = lx;
		// fprintf(stderr, "[e_fmod.c] exit __ieee754_fmod 38\n");
	} else {		
		fprintf(stderr, "[e_fmod.c] enter __ieee754_fmod 39\n");
	    n = -1022 - iy;
	    if(n<=20) {
			fprintf(stderr, "[e_fmod.c] enter __ieee754_fmod 40\n");
			lx = (lx>>n)|((unsigned)hx<<(32-n));
			hx >>= n;
			// fprintf(stderr, "[e_fmod.c] exit __ieee754_fmod 40\n");
	    } else if (n<=31) {
			fprintf(stderr, "[e_fmod.c] enter __ieee754_fmod 41\n");
			lx = (hx<<(32-n))|(lx>>n); hx = sx;
			// fprintf(stderr, "[e_fmod.c] exit __ieee754_fmod 41\n");
	    } else {
			fprintf(stderr, "[e_fmod.c] enter __ieee754_fmod 42\n");
			lx = hx>>(n-32); hx = sx;
			// fprintf(stderr, "[e_fmod.c] exit __ieee754_fmod 42\n");
	    }
	    __HI(x) = hx|sx;
	    __LO(x) = lx;
	    x *= one;		
		// fprintf(stderr, "[e_fmod.c] exit __ieee754_fmod 39\n");
	}
	fprintf(stderr, "[e_fmod.c] enter __ieee754_fmod 43\n");
	return x;
	// fprintf(stderr, "[e_fmod.c] exit __ieee754_fmod 43\n");
}

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
	fprintf(stderr, "\n");

    if (argc != 3) {
		fprintf(stderr, "[e_fmod.c] enter main 2\n");
        printf("Usage: %s <arg> <arg>\n", argv[0]);
        return 1;
		// fprintf(stderr, "[e_fmod.c] exit main 2\n");
    }

	fprintf(stderr, "[e_fmod.c] enter main 3\n");
    double a0 = atof(argv[1]);
    double a1 = atof(argv[2]);

    double r = __ieee754_fmod(a0, a1);
    printf("%f\n", r);

    return 0;
	// fprintf(stderr, "[e_fmod.c] exit main 3\n");
}
// Total cost: 0.004094
// Total split cost: 0.000000, input tokens: 0, output tokens: 0, cache read tokens: 0, cache write tokens: 0, split chunks: [(0, 146)]
// Total instrumented cost: 0.004094, input tokens: 3357, output tokens: 2898, cache read tokens: 0, cache write tokens: 0
