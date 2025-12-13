





#include "fdlibm.h"
#include <stdio.h>

#ifdef __STDC__
static const double huge = 1.0e300;
#else
static double huge = 1.0e300;
#endif

#ifdef __STDC__
	double floor(double x)
#else
	double floor(x)
	double x;
#endif
{
	fprintf(stderr, "[s_floor.c] enter floor 1\n");
	int i0,i1,j0;
	unsigned i,j;
	i0 =  __HI(x);
	i1 =  __LO(x);
	j0 = ((i0>>20)&0x7ff)-0x3ff;
	// fprintf(stderr, "[s_floor.c] exit floor 1\n");
	if(j0<20) {
	    fprintf(stderr, "[s_floor.c] enter floor 2\n");
	    if(j0<0) { 	
		fprintf(stderr, "[s_floor.c] enter floor 3\n");
		if(huge+x>0.0) {
		    fprintf(stderr, "[s_floor.c] enter floor 4\n");
		    if(i0>=0) {i0=i1=0;} 
		    else if(((i0&0x7fffffff)|i1)!=0)
			{ i0=0xbff00000;i1=0;}
		    // fprintf(stderr, "[s_floor.c] exit floor 4\n");
		}
		// fprintf(stderr, "[s_floor.c] exit floor 3\n");
	    } else {
		fprintf(stderr, "[s_floor.c] enter floor 5\n");
		i = (0x000fffff)>>j0;
		if(((i0&i)|i1)==0) return x; 
		if(huge+x>0.0) {	
		    fprintf(stderr, "[s_floor.c] enter floor 6\n");
		    if(i0<0) i0 += (0x00100000)>>j0;
		    i0 &= (~i); i1=0;
		    // fprintf(stderr, "[s_floor.c] exit floor 6\n");
		}
		// fprintf(stderr, "[s_floor.c] exit floor 5\n");
	    }
	    // fprintf(stderr, "[s_floor.c] exit floor 2\n");
	} else if (j0>51) {
	    fprintf(stderr, "[s_floor.c] enter floor 7\n");
	    if(j0==0x400) return x+x;	
	    else return x;		
	    // fprintf(stderr, "[s_floor.c] exit floor 7\n");
	} else {
	    fprintf(stderr, "[s_floor.c] enter floor 8\n");
	    i = ((unsigned)(0xffffffff))>>(j0-20);
	    if((i1&i)==0) return x;	
	    if(huge+x>0.0) { 		
		fprintf(stderr, "[s_floor.c] enter floor 9\n");
		if(i0<0) {
		    fprintf(stderr, "[s_floor.c] enter floor 10\n");
		    if(j0==20) i0+=1; 
		    else {
			fprintf(stderr, "[s_floor.c] enter floor 11\n");
			j = i1+(1<<(52-j0));
			if(j<i1) i0 +=1 ; 	
			i1=j;
			// fprintf(stderr, "[s_floor.c] exit floor 11\n");
		    }
		    // fprintf(stderr, "[s_floor.c] exit floor 10\n");
		}
		i1 &= (~i);
		// fprintf(stderr, "[s_floor.c] exit floor 9\n");
	    }
	    // fprintf(stderr, "[s_floor.c] exit floor 8\n");
	}
	fprintf(stderr, "[s_floor.c] enter floor 12\n");
	__HI(x) = i0;
	__LO(x) = i1;
	return x;
	// fprintf(stderr, "[s_floor.c] exit floor 12\n");
}

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
    fprintf(stderr, "\n");
    if (argc != 2) {
        fprintf(stderr, "[s_floor.c] enter main 2\n");
        printf("Usage: %s <arg>\n", argv[0]);
        return 1;
        // fprintf(stderr, "[s_floor.c] exit main 2\n");
    }

    fprintf(stderr, "[s_floor.c] enter main 3\n");
    double a0 = atof(argv[1]);
    // fprintf(stderr, "[s_floor.c] exit main 3\n");

    fprintf(stderr, "[s_floor.c] enter main 4\n");
    double r = floor(a0);
    printf("%f\n", r);

    return 0;
    // fprintf(stderr, "[s_floor.c] exit main 4\n");
}
// Total cost: 0.001776
// Total split cost: 0.000000, input tokens: 0, output tokens: 0, cache read tokens: 0, cache write tokens: 0, split chunks: [(0, 81)]
// Total instrumented cost: 0.001776, input tokens: 2697, output tokens: 953, cache read tokens: 0, cache write tokens: 0
