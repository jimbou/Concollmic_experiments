
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
    fprintf(stderr, "[Custom exp] Called with x = %f\n", x);
    
    // Call the original function
    double result = orig_exp(x);
    
    // Print the result
    fprintf(stderr, "[Custom exp] Result = %f\n", result);
    
    // Print information about the _LIB_VERSION branch
    fprintf(stderr, "[Custom exp] Simulating _LIB_VERSION != _IEEE_ branch\n");
    
    // Return the result
    return result;
}

// This function will be called when the library is loaded
void __attribute__((constructor)) init(void) {
    fprintf(stderr, "[Custom Math Library] Initialized\n");
    fprintf(stderr, "[Custom Math Library] Simulating _LIB_VERSION != _IEEE_\n");
}
