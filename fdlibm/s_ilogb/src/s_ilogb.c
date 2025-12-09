





#include "fdlibm.h"

#ifdef __STDC__
	int ilogb(double x)
#else
	int ilogb(x)
	double x;
#endif
{
	int hx,lx,ix;

	hx  = (__HI(x))&0x7fffffff;	
	if(hx<0x00100000) {
	    lx = __LO(x);
	    if((hx|lx)==0) 
		return 0x80000001;	
	    else			
		if(hx==0) {
		    for (ix = -1043; lx>0; lx<<=1) ix -=1;
		} else {
		    for (ix = -1022,hx<<=11; hx>0; hx<<=1) ix -=1;
		}
	    return ix;
	}
	else if (hx<0x7ff00000) return (hx>>20)-1023;
	else return 0x7fffffff;
}

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {

    if (argc != 2) {
        printf("Usage: %s <arg>\n", argv[0]);
        return 1;
    }

    double a0 = atof(argv[1]);

    int r = ilogb(a0);
    printf("%f\n", r);

    return 0;
}
