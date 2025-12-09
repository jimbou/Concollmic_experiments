
/* @(#)s_finite.c 1.3 95/01/18 */
/*
 * ====================================================
 * Copyright (C) 1993 by Sun Microsystems, Inc. All rights reserved.
 *
 * Developed at SunSoft, a Sun Microsystems, Inc. business.
 * Permission to use, copy, modify, and distribute this
 * software is freely granted, provided that this notice 
 * is preserved.
 * ====================================================
 */

/*
 * finite(x) returns 1 is x is finite, else 0;
 * no branching!
 */

#include "fdlibm.h"

#ifdef __STDC__
	int finite(double x)
#else
	int finite(x)
	double x;
#endif
{
	int hx; 
	hx = __HI(x);
	return  (unsigned)((hx&0x7fffffff)-0x7ff00000)>>31;
}

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {

    if (argc != 2) {
        printf("Usage: %s <arg>\n", argv[0]);
        return 1;
    }

    double a0 = atof(argv[1]);

    int r = finite(a0);
    printf("%f\n", r);

    return 0;
}
