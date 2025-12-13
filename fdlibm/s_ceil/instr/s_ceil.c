





#include "fdlibm.h"
#include <stdio.h>

#ifdef __STDC__
static const double huge = 1.0e300;
#else
static double huge = 1.0e300;
#endif

#ifdef __STDC__
	double ceil(double x)
#else
	double ceil(x)
	double x;
#endif
{
	fprintf(stderr, "[s_ceil.c] enter ceil 1\n");
	int i0,i1,j0;
	unsigned i,j;
	i0 =  __HI(x);
	i1 =  __LO(x);
	j0 = ((i0>>20)&0x7ff)-0x3ff;
	// fprintf(stderr, "[s_ceil.c] exit ceil 1\n");
	if(j0<20) {
	    fprintf(stderr, "[s_ceil.c] enter ceil 2\n");
	    if(j0<0) { 	
		fprintf(stderr, "[s_ceil.c] enter ceil 3\n");
		if(huge+x>0.0) {
		    fprintf(stderr, "[s_ceil.c] enter ceil 4\n");
		    if(i0<0) {i0=0x80000000;i1=0;} 
		    else if((i0|i1)!=0) { i0=0x3ff00000;i1=0;}
		    // fprintf(stderr, "[s_ceil.c] exit ceil 4\n");
		}
		// fprintf(stderr, "[s_ceil.c] exit ceil 3\n");
	    } else {
		fprintf(stderr, "[s_ceil.c] enter ceil 5\n");
		i = (0x000fffff)>>j0;
		if(((i0&i)|i1)==0) {
		    fprintf(stderr, "[s_ceil.c] enter ceil 6\n");
		    return x; 
		    // fprintf(stderr, "[s_ceil.c] exit ceil 6\n");
		}
		if(huge+x>0.0) {	
		    fprintf(stderr, "[s_ceil.c] enter ceil 7\n");
		    if(i0>0) i0 += (0x00100000)>>j0;
		    i0 &= (~i); i1=0;
		    // fprintf(stderr, "[s_ceil.c] exit ceil 7\n");
		}
		// fprintf(stderr, "[s_ceil.c] exit ceil 5\n");
	    }
	    // fprintf(stderr, "[s_ceil.c] exit ceil 2\n");
	} else if (j0>51) {
	    fprintf(stderr, "[s_ceil.c] enter ceil 8\n");
	    if(j0==0x400) {
		fprintf(stderr, "[s_ceil.c] enter ceil 9\n");
		return x+x;	
		// fprintf(stderr, "[s_ceil.c] exit ceil 9\n");
	    } else {
		fprintf(stderr, "[s_ceil.c] enter ceil 10\n");
		return x;		
		// fprintf(stderr, "[s_ceil.c] exit ceil 10\n");
	    }
	    // fprintf(stderr, "[s_ceil.c] exit ceil 8\n");
	} else {
	    fprintf(stderr, "[s_ceil.c] enter ceil 11\n");
	    i = ((unsigned)(0xffffffff))>>(j0-20);
	    if((i1&i)==0) {
		fprintf(stderr, "[s_ceil.c] enter ceil 12\n");
		return x;	
		// fprintf(stderr, "[s_ceil.c] exit ceil 12\n");
	    }
	    if(huge+x>0.0) { 		
		fprintf(stderr, "[s_ceil.c] enter ceil 13\n");
		if(i0>0) {
		    fprintf(stderr, "[s_ceil.c] enter ceil 14\n");
		    if(j0==20) i0+=1; 
		    else {
			fprintf(stderr, "[s_ceil.c] enter ceil 15\n");
			j = i1 + (1<<(52-j0));
			if(j<i1) i0+=1;	
			i1 = j;
			// fprintf(stderr, "[s_ceil.c] exit ceil 15\n");
		    }
		    // fprintf(stderr, "[s_ceil.c] exit ceil 14\n");
		}
		i1 &= (~i);
		// fprintf(stderr, "[s_ceil.c] exit ceil 13\n");
	    }
	    // fprintf(stderr, "[s_ceil.c] exit ceil 11\n");
	}
	fprintf(stderr, "[s_ceil.c] enter ceil 16\n");
	__HI(x) = i0;
	__LO(x) = i1;
	return x;
	// fprintf(stderr, "[s_ceil.c] exit ceil 16\n");
}

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
    fprintf(stderr, "\n");
    if (argc != 2) {
        fprintf(stderr, "[s_ceil.c] enter main 2\n");
        printf("Usage: %s <arg>\n", argv[0]);
        return 1;
        // fprintf(stderr, "[s_ceil.c] exit main 2\n");
    }

    fprintf(stderr, "[s_ceil.c] enter main 3\n");
    double a0 = atof(argv[1]);

    double r = ceil(a0);
    printf("%f\n", r);

    return 0;
    // fprintf(stderr, "[s_ceil.c] exit main 3\n");
}
// Total cost: 0.001883
// Total split cost: 0.000000, input tokens: 0, output tokens: 0, cache read tokens: 0, cache write tokens: 0, split chunks: [(0, 80)]
// Total instrumented cost: 0.001883, input tokens: 2697, output tokens: 1050, cache read tokens: 0, cache write tokens: 0
