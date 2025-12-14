
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <math.h>

// Our replacement for j1 that calls y1 instead
double j1(double x) {
    fprintf(stderr, "[interposer] Intercepted j1(%f), calling y1 instead\n", x);
    
    // Call y1 with a non-positive value to trigger the target branch
    double result = y1(0.0);  // Use 0.0 to satisfy x <= 0.0
    
    fprintf(stderr, "[interposer] y1(0.0) returned %f\n", result);
    
    // Return the original result that j1 would have returned
    // This is to avoid affecting the program's behavior too much
    typedef double (*orig_j1_t)(double);
    orig_j1_t orig_j1 = (orig_j1_t)dlsym(RTLD_NEXT, "j1");
    if (orig_j1) {
        return orig_j1(x);
    } else {
        fprintf(stderr, "[interposer] Error finding original j1: %s\n", dlerror());
        return result;  // Fall back to the y1 result
    }
}
