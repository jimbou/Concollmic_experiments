
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <fenv.h>

extern int _LIB_VERSION;

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <value>\n", argv[0]);
        return 1;
    }
    
    double x = atof(argv[1]);
    int signgam = 0;
    double result;
    
    // Try to set _LIB_VERSION directly
    #ifdef _SVID_
        printf("Setting _LIB_VERSION to _SVID_\n");
        _LIB_VERSION = _SVID_;
    #endif
    
    // Print current _LIB_VERSION value if possible
    #ifdef _LIB_VERSION
        printf("_LIB_VERSION is defined\n");
        
        #ifdef _IEEE_
            if (_LIB_VERSION == _IEEE_)
                printf("_LIB_VERSION is set to _IEEE_\n");
        #endif
        
        #ifdef _SVID_
            if (_LIB_VERSION == _SVID_)
                printf("_LIB_VERSION is set to _SVID_\n");
        #endif
        
        #ifdef _XOPEN_
            if (_LIB_VERSION == _XOPEN_)
                printf("_LIB_VERSION is set to _XOPEN_\n");
        #endif
        
        #ifdef _POSIX_
            if (_LIB_VERSION == _POSIX_)
                printf("_LIB_VERSION is set to _POSIX_\n");
        #endif
    #else
        printf("_LIB_VERSION is not defined\n");
    #endif
    
    // Check if x is a non-positive integer
    if (x <= 0.0 && floor(x) == x) {
        printf("%f is a non-positive integer\n", x);
    } else {
        printf("%f is NOT a non-positive integer\n", x);
    }
    
    // Check if x is finite
    if (isfinite(x)) {
        printf("%f is finite\n", x);
    } else {
        printf("%f is not finite\n", x);
    }
    
    // Try to call gamma function
    printf("Calling tgamma function with x = %f\n", x);
    
    // Enable floating-point exceptions if possible
    #ifdef FE_INVALID
        printf("Enabling floating-point exceptions\n");
        feenableexcept(FE_INVALID | FE_DIVBYZERO | FE_OVERFLOW);
    #endif
    
    // Call tgamma
    result = tgamma(x);
    
    printf("tgamma(%f) = %f\n", x, result);
    
    // Check if result is finite
    if (isfinite(result)) {
        printf("Result is finite\n");
    } else {
        printf("Result is not finite\n");
        
        if (isinf(result)) {
            printf("Result is infinity\n");
        } else if (isnan(result)) {
            printf("Result is NaN\n");
        }
    }
    
    return 0;
}
