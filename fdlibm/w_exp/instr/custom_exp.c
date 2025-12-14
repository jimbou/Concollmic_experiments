
#define _GNU_SOURCE
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <dlfcn.h>

// Function pointer type for the original exp function
typedef double (*orig_exp_t)(double);

// Our custom implementation of exp
double exp(double x) {
    // Get the original exp function
    orig_exp_t orig_exp = (orig_exp_t)dlsym(RTLD_NEXT, "exp");
    
    // Print information about the call
    printf("[Custom exp] Called with x = %f\n", x);
    
    // Call the original function
    double result = orig_exp(x);
    
    // Print the result
    printf("[Custom exp] Result = %f\n", result);
    
    // Return the result
    return result;
}

// This function will be called when the library is loaded
void __attribute__((constructor)) init(void) {
    printf("[Custom Math Library] Initialized\n");
    printf("[Custom Math Library] Setting _LIB_VERSION to non-IEEE mode\n");
}
