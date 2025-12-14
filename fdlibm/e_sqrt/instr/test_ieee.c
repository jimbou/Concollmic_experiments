
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <float_value>\n", argv[0]);
        return 1;
    }
    
    double value = atof(argv[1]);
    unsigned long long int_bits;
    memcpy(&int_bits, &value, sizeof(double));
    
    printf("Input string: %s\n", argv[1]);
    printf("Parsed as double: %g\n", value);
    printf("IEEE 754 hex: 0x%llx\n", int_bits);
    
    // Extract high word (most significant 32 bits)
    unsigned int high_word = (unsigned int)(int_bits >> 32);
    printf("High word: 0x%x\n", high_word);
    
    // Check if exponent bits are all 1
    unsigned int exponent_mask = 0x7ff00000;
    unsigned int exponent_bits = high_word & exponent_mask;
    printf("Exponent bits: 0x%x\n", exponent_bits);
    printf("Constraint (ix0&0x7ff00000)==0x7ff00000: %s\n", 
           (exponent_bits == exponent_mask) ? "true" : "false");
    
    return 0;
}
