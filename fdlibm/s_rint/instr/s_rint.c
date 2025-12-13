





#include "fdlibm.h"
#include <stdio.h>

#ifdef __STDC__
static const double
#else
static double 
#endif
TWO52[2]={
  4.50359962737049600000e+15, 
 -4.50359962737049600000e+15, 
};

#ifdef __STDC__
	double rint(double x)
#else
	double rint(x)
	double x;
#endif
{
	fprintf(stderr, "[s_rint.c] enter rint 1\n");
	int i0,j0,sx;
	unsigned i,i1;
	double w,t;
	i0 =  __HI(x);
	sx = (i0>>31)&1;
	i1 =  __LO(x);
	j0 = ((i0>>20)&0x7ff)-0x3ff;
	// fprintf(stderr, "[s_rint.c] exit rint 1\n");
	
	fprintf(stderr, "[s_rint.c] enter rint 2\n");
	if(j0<20) {
	    // fprintf(stderr, "[s_rint.c] exit rint 2\n");
	    fprintf(stderr, "[s_rint.c] enter rint 3\n");
	    if(j0<0) { 	
		fprintf(stderr, "[s_rint.c] enter rint 4\n");
		if(((i0&0x7fffffff)|i1)==0) {
		    fprintf(stderr, "[s_rint.c] enter rint 5\n");
		    return x;
		    // fprintf(stderr, "[s_rint.c] exit rint 5\n");
		}
		fprintf(stderr, "[s_rint.c] enter rint 6\n");
		i1 |= (i0&0x0fffff);
		i0 &= 0xfffe0000;
		i0 |= ((i1|-i1)>>12)&0x80000;
		__HI(x)=i0;
	        w = TWO52[sx]+x;
	        t =  w-TWO52[sx];
	        i0 = __HI(t);
	        __HI(t) = (i0&0x7fffffff)|(sx<<31);
	        return t;
	        // fprintf(stderr, "[s_rint.c] exit rint 6\n");
	    } else {
		fprintf(stderr, "[s_rint.c] enter rint 7\n");
		i = (0x000fffff)>>j0;
		if(((i0&i)|i1)==0) {
		    fprintf(stderr, "[s_rint.c] enter rint 8\n");
		    return x;
		    // fprintf(stderr, "[s_rint.c] exit rint 8\n");
		}
		fprintf(stderr, "[s_rint.c] enter rint 9\n");
		i>>=1;
		if(((i0&i)|i1)!=0) {
		    fprintf(stderr, "[s_rint.c] enter rint 10\n");
		    if(j0==19) i1 = 0x40000000; else
		    i0 = (i0&(~i))|((0x20000)>>j0);
		    // fprintf(stderr, "[s_rint.c] exit rint 10\n");
		}
		// fprintf(stderr, "[s_rint.c] exit rint 9\n");
	    }
	    // fprintf(stderr, "[s_rint.c] exit rint 7\n");
	    // fprintf(stderr, "[s_rint.c] exit rint 4\n");
	    // fprintf(stderr, "[s_rint.c] exit rint 3\n");
	} else if (j0>51) {
	    fprintf(stderr, "[s_rint.c] enter rint 11\n");
	    if(j0==0x400) {
		fprintf(stderr, "[s_rint.c] enter rint 12\n");
		return x+x;
		// fprintf(stderr, "[s_rint.c] exit rint 12\n");
	    } else {
		fprintf(stderr, "[s_rint.c] enter rint 13\n");
		return x;
		// fprintf(stderr, "[s_rint.c] exit rint 13\n");
	    }
	    // fprintf(stderr, "[s_rint.c] exit rint 11\n");
	} else {
	    fprintf(stderr, "[s_rint.c] enter rint 14\n");
	    i = ((unsigned)(0xffffffff))>>(j0-20);
	    if((i1&i)==0) {
		fprintf(stderr, "[s_rint.c] enter rint 15\n");
		return x;
		// fprintf(stderr, "[s_rint.c] exit rint 15\n");
	    }
	    fprintf(stderr, "[s_rint.c] enter rint 16\n");
	    i>>=1;
	    if((i1&i)!=0) {
		fprintf(stderr, "[s_rint.c] enter rint 17\n");
		i1 = (i1&(~i))|((0x40000000)>>(j0-20));
		// fprintf(stderr, "[s_rint.c] exit rint 17\n");
	    }
	    // fprintf(stderr, "[s_rint.c] exit rint 16\n");
	    // fprintf(stderr, "[s_rint.c] exit rint 14\n");
	}
	fprintf(stderr, "[s_rint.c] enter rint 18\n");
	__HI(x) = i0;
	__LO(x) = i1;
	w = TWO52[sx]+x;
	return w-TWO52[sx];
	// fprintf(stderr, "[s_rint.c] exit rint 18\n");
}

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
    fprintf(stderr, "[s_rint.c] enter main 1\n");
    if (argc != 2) {
        fprintf(stderr, "[s_rint.c] enter main 2\n");
        printf("Usage: %s <arg>\n", argv[0]);
        return 1;
        // fprintf(stderr, "[s_rint.c] exit main 2\n");
    }
    fprintf(stderr, "[s_rint.c] enter main 3\n");
    double a0 = atof(argv[1]);

    double r = rint(a0);
    printf("%f\n", r);

    return 0;
    // fprintf(stderr, "[s_rint.c] exit main 3\n");
    // fprintf(stderr, "[s_rint.c] exit main 1\n");
}
// Total cost: 0.011555
// Total split cost: 0.000000, input tokens: 0, output tokens: 0, cache read tokens: 0, cache write tokens: 0, split chunks: [(0, 85)]
// Total instrumented cost: 0.011555, input tokens: 19773, output tokens: 5651, cache read tokens: 0, cache write tokens: 0
