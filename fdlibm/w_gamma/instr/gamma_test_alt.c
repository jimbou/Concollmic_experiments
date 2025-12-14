
    #include <stdio.h>
    #include <stdlib.h>
    #include <math.h>
    #include <fenv.h>
    
    int main(int argc, char *argv[]) {
        if (argc < 2) {
            printf("Usage: %s <value>\n", argv[0]);
            return 1;
        }
        
        double x = atof(argv[1]);
        double result;
        
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
        
        // Enable math error reporting
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
    