
    #include <stdio.h>
    #include <stdlib.h>
    #include <math.h>
    
    int main(int argc, char *argv[]) {
        if (argc < 2) {
            printf("Usage: %s <float_value>\n", argv[0]);
            return 1;
        }
        
        double value = atof(argv[1]);
        
        printf("Input string: %s\n", argv[1]);
        printf("Parsed value: %f\n", value);
        printf("isnan: %d\n", isnan(value));
        printf("isinf: %d\n", isinf(value));
        
        return 0;
    }
    