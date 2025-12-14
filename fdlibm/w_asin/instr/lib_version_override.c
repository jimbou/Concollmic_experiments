
#include <stdio.h>
#include <math.h>

/* Define the possible _LIB_VERSION values if they're not already defined */
#ifndef _IEEE_
#define _IEEE_ 0
#endif
#ifndef _SVID_
#define _SVID_ 1
#endif
#ifndef _XOPEN_
#define _XOPEN_ 2
#endif
#ifndef _POSIX_
#define _POSIX_ 3
#endif

/* Try different approaches to override _LIB_VERSION */

/* Approach 1: Direct override if it's a global variable */
int _LIB_VERSION = _SVID_;  /* Set to non-IEEE value */

/* Approach 2: Override functions that might use _LIB_VERSION */
double asin(double x) {
    fprintf(stderr, "Custom asin called with x=%g\n", x);
    
    /* Call the original asin function */
    extern double __asin(double);
    double result = __asin(x);
    
    /* Print debug info about the result */
    fprintf(stderr, "Custom asin returning %g\n", result);
    return result;
}

/* Constructor function to run when library is loaded */
__attribute__((constructor))
void init(void) {
    fprintf(stderr, "Library initialized, setting _LIB_VERSION to _SVID_\n");
}
