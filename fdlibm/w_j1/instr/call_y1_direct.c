
#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <math.h>

// Define the y1 function type
typedef double (*y1_func_t)(double);

int main() {
    // First, try to call y1 from the standard math library
    double result = y1(0.0);
    fprintf(stderr, "Standard math library y1(0.0) = %f\n", result);
    
    return 0;
}
