
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <number>\n", argv[0]);
        return 1;
    }
    
    double value = atof(argv[1]);
    
    printf("Input: %s\n", argv[1]);
    printf("Parsed value: %f\n", value);
    printf("Is NaN: %d\n", isnan(value));
    printf("Is Inf: %d\n", isinf(value));
    
    return 0;
}
