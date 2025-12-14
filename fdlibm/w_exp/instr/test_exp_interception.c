
    #include <stdio.h>
    #include <math.h>
    #include <stdlib.h>
    
    int main(int argc, char *argv[]) {
        if (argc != 2) {
            printf("Usage: %s <number>\n", argv[0]);
            return 1;
        }
        
        double x = atof(argv[1]);
        printf("Input: %f\n", x);
        
        // Call exp function
        double result = exp(x);
        
        printf("Result: %f\n", result);
        
        return 0;
    }
    