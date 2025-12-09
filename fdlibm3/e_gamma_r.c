
/* @(#)e_gamma_r.c 1.3 95/01/18 */
/*
 * ====================================================
 * Copyright (C) 1993 by Sun Microsystems, Inc. All rights reserved.
 *
 * Developed at SunSoft, a Sun Microsystems, Inc. business.
 * Permission to use, copy, modify, and distribute this
 * software is freely granted, provided that this notice 
 * is preserved.
 * ====================================================
 *
 */

/* __ieee754_gamma_r(x, signgamp)
 * Reentrant version of the logarithm of the Gamma function 
 * with user provide pointer for the sign of Gamma(x). 
 *
 * Method: See __ieee754_lgamma_r
 */

#include "fdlibm.h"

#ifdef __STDC__
	double __ieee754_gamma_r(double x, int *signgamp)
#else
	double __ieee754_gamma_r(x,signgamp)
	double x; int *signgamp;
#endif
{
	return __ieee754_lgamma_r(x,signgamp);
}


#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
    if (argc != 2) {
        printf("Usage: %s <x>\n", argv[0]);
        return 1;
    }

    double x = atof(argv[1]);
    int sign = 0;

    double r = __ieee754_gamma_r(x, &sign);

    printf("result = %f\n", r);
    printf("signgamp = %d\n", sign);

    return 0;
}
