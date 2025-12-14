
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <fenv.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <value>\n", argv[0]);
        return 1;
    }
    
    double x = atof(argv[1]);
    
    // Check if x is a non-positive integer
    if (x <= 0.0 && floor(x) == x) {
        fprintf(stderr, "Input is a non-positive integer: %f\n", x);
    } else {
        fprintf(stderr, "Input is NOT a non-positive integer: %f\n", x);
        return 1;
    }
    
    // Call gamma function
    double result = tgamma(x);
    
    // Check if result is finite
    if (!isfinite(result)) {
        fprintf(stderr, "Result is not finite: %f\n", result);
        
        // Check if we're in the target branch
        if (floor(x) == x && x <= 0.0) {
            fprintf(stderr, "TARGET BRANCH REACHED: Non-positive integer input to gamma function\n");
            return 2;  // Special return code to indicate target branch reached
        }
    } else {
        fprintf(stderr, "Result is finite: %f\n", result);
    }
    
    return 0;
}
