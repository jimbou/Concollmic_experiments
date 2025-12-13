





#include "fdlibm.h"
#include <stdio.h>

#ifdef __STDC__
	int ilogb(double x)
#else
	int ilogb(x)
	double x;
#endif
{
	fprintf(stderr, "[s_ilogb.c] enter ilogb 1\n");
	int hx,lx,ix;

	hx  = (__HI(x))&0x7fffffff;	
	if(hx<0x00100000) {
	    fprintf(stderr, "[s_ilogb.c] enter ilogb 2\n");
	    lx = __LO(x);
	    if((hx|lx)==0) {
		fprintf(stderr, "[s_ilogb.c] enter ilogb 3\n");
		return 0x80000001;	
		// fprintf(stderr, "[s_ilogb.c] exit ilogb 3\n");
	    } else {
		fprintf(stderr, "[s_ilogb.c] enter ilogb 4\n");
		if(hx==0) {
		    fprintf(stderr, "[s_ilogb.c] enter ilogb 5\n");
		    for (ix = -1043; lx>0; lx<<=1) ix -=1;
		    // fprintf(stderr, "[s_ilogb.c] exit ilogb 5\n");
		} else {
		    fprintf(stderr, "[s_ilogb.c] enter ilogb 6\n");
		    for (ix = -1022,hx<<=11; hx>0; hx<<=1) ix -=1;
		    // fprintf(stderr, "[s_ilogb.c] exit ilogb 6\n");
		}
	    return ix;
	    // fprintf(stderr, "[s_ilogb.c] exit ilogb 4\n");
	    }
	    // fprintf(stderr, "[s_ilogb.c] exit ilogb 2\n");
	}
	else if (hx<0x7ff00000) {
	    fprintf(stderr, "[s_ilogb.c] enter ilogb 7\n");
	    return (hx>>20)-1023;
	    // fprintf(stderr, "[s_ilogb.c] exit ilogb 7\n");
	}
	else {
	    fprintf(stderr, "[s_ilogb.c] enter ilogb 8\n");
	    return 0x7fffffff;
	    // fprintf(stderr, "[s_ilogb.c] exit ilogb 8\n");
	}
	// fprintf(stderr, "[s_ilogb.c] exit ilogb 1\n");
}

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
    fprintf(stderr, "\n");
    if (argc != 2) {
        fprintf(stderr, "[s_ilogb.c] enter main 2\n");
        printf("Usage: %s <arg>\n", argv[0]);
        return 1;
        // fprintf(stderr, "[s_ilogb.c] exit main 2\n");
    }

    fprintf(stderr, "[s_ilogb.c] enter main 3\n");
    double a0 = atof(argv[1]);

    int r = ilogb(a0);
    printf("%f\n", r);

    return 0;
    // fprintf(stderr, "[s_ilogb.c] exit main 3\n");
}
// Total cost: 0.001354
// Total split cost: 0.000000, input tokens: 0, output tokens: 0, cache read tokens: 0, cache write tokens: 0, split chunks: [(0, 51)]
// Total instrumented cost: 0.001354, input tokens: 2460, output tokens: 627, cache read tokens: 0, cache write tokens: 0
