
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <number>\n", argv[0]);
        return 1;
    }
    
    double x = atof(argv[1]);
    double result = asin(x);
    
    printf("asin(%g) = %g\n", x, result);
    return 0;
}
