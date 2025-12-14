
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <value>\n", argv[0]);
        return 1;
    }
    
    double x = atof(argv[1]);
    
    // Print information about the math library
    #ifdef _LIB_VERSION
        printf("_LIB_VERSION is defined\n");
        
        #ifdef _IEEE_
            printf("_IEEE_ is defined\n");
            if (_LIB_VERSION == _IEEE_)
                printf("_LIB_VERSION is set to _IEEE_\n");
        #endif
        
        #ifdef _SVID_
            printf("_SVID_ is defined\n");
            if (_LIB_VERSION == _SVID_)
                printf("_LIB_VERSION is set to _SVID_\n");
        #endif
        
        #ifdef _XOPEN_
            printf("_XOPEN_ is defined\n");
            if (_LIB_VERSION == _XOPEN_)
                printf("_LIB_VERSION is set to _XOPEN_\n");
        #endif
        
        #ifdef _POSIX_
            printf("_POSIX_ is defined\n");
            if (_LIB_VERSION == _POSIX_)
                printf("_LIB_VERSION is set to _POSIX_\n");
        #endif
    #else
        printf("_LIB_VERSION is not defined\n");
    #endif
    
    // Try to call gamma function
    double result;
    int signgam;
    
    printf("Calling gamma function with x = %f\n", x);
    
    #ifdef __GLIBC__
        printf("Using GLIBC version: %d.%d\n", __GLIBC__, __GLIBC_MINOR__);
        
        // Access internal __ieee754_gamma_r if possible
        #ifdef __ieee754_gamma_r
            result = __ieee754_gamma_r(x, &signgam);
            printf("__ieee754_gamma_r(%f) = %f, signgam = %d\n", x, result, signgam);
        #endif
        
        // Try standard tgamma
        result = tgamma(x);
        printf("tgamma(%f) = %f\n", x, result);
    #else
        printf("Not using GLIBC\n");
        
        // Try standard gamma function
        #ifdef gamma
            result = gamma(x);
            printf("gamma(%f) = %f\n", x, result);
        #endif
        
        // Try standard tgamma
        result = tgamma(x);
        printf("tgamma(%f) = %f\n", x, result);
    #endif
    
    return 0;
}
