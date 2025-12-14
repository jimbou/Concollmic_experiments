
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <errno.h>
#include <fenv.h>

// Define our own versions if not available
#ifndef _LIB_VERSION
typedef enum {
    _IEEE_ = -1,
    _SVID_ = 0,
    _XOPEN_ = 1,
    _POSIX_ = 2
} _LIB_VERSION_TYPE;
extern _LIB_VERSION_TYPE _LIB_VERSION;
#endif

// Function to print the current state of math error handling
void print_math_errhandling() {
    printf("math_errhandling = %d\n", math_errhandling);
    
    if (math_errhandling & MATH_ERRNO)
        printf("MATH_ERRNO is enabled\n");
    else
        printf("MATH_ERRNO is disabled\n");
        
    if (math_errhandling & MATH_ERREXCEPT)
        printf("MATH_ERREXCEPT is enabled\n");
    else
        printf("MATH_ERREXCEPT is disabled\n");
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <value>\n", argv[0]);
        return 1;
    }
    
    // Enable all floating point exceptions
    feenableexcept(FE_ALL_EXCEPT);
    
    double x = atof(argv[1]);
    printf("Input value: %f\n", x);
    
    // Print math error handling configuration
    print_math_errhandling();
    
    // Reset errno before the call
    errno = 0;
    
    // Call atanh and check the result
    printf("Calling atanh(%f)...\n", x);
    double result = atanh(x);
    
    printf("atanh(%f) = %f\n", x, result);
    printf("errno after call = %d\n", errno);
    
    // Check if errno indicates a domain error
    if (errno == EDOM)
        printf("Domain error detected (EDOM)\n");
    else if (errno == ERANGE)
        printf("Range error detected (ERANGE)\n");
    
    // Check floating point exceptions
    int fe_raised = fetestexcept(FE_ALL_EXCEPT);
    if (fe_raised & FE_DIVBYZERO)
        printf("FE_DIVBYZERO exception raised\n");
    if (fe_raised & FE_INVALID)
        printf("FE_INVALID exception raised\n");
    if (fe_raised & FE_OVERFLOW)
        printf("FE_OVERFLOW exception raised\n");
    if (fe_raised & FE_UNDERFLOW)
        printf("FE_UNDERFLOW exception raised\n");
    if (fe_raised & FE_INEXACT)
        printf("FE_INEXACT exception raised\n");
    
    return 0;
}
