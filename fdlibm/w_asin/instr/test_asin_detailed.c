
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <errno.h>

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

/* Try to access _LIB_VERSION if it's available */
extern int _LIB_VERSION;

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <number>\n", argv[0]);
        return 1;
    }
    
    double x = atof(argv[1]);
    
    /* Print debug info */
    printf("Input: %g\n", x);
    printf("fabs(x): %g\n", fabs(x));
    printf("isnan(x): %d\n", isnan(x));
    
    /* Try to access and print _LIB_VERSION */
    printf("_LIB_VERSION address: %p\n", &_LIB_VERSION);
    printf("_LIB_VERSION value: %d\n", _LIB_VERSION);
    
    /* Print the defined constants */
    printf("_IEEE_ = %d\n", _IEEE_);
    printf("_SVID_ = %d\n", _SVID_);
    printf("_XOPEN_ = %d\n", _XOPEN_);
    printf("_POSIX_ = %d\n", _POSIX_);
    
    /* Check the condition */
    if(_LIB_VERSION == _IEEE_ || isnan(x)) {
        printf("Condition is TRUE\n");
    } else {
        printf("Condition is FALSE\n");
    }
    
    /* Calculate asin */
    errno = 0;
    double result = asin(x);
    
    /* Print results */
    printf("asin(%g) = %g\n", x, result);
    printf("errno = %d\n", errno);
    
    return 0;
}
