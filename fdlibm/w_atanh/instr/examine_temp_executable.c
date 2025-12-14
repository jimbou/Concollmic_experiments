
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <errno.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <value>\n", argv[0]);
        return 1;
    }
    
    double x = atof(argv[1]);
    printf("Input: %f\n", x);
    
    // Calculate atanh(x)
    errno = 0;
    double y = atanh(x);
    
    printf("atanh(%f) = %f\n", x, y);
    printf("errno = %d\n", errno);
    
    // Check if y >= 1.0 (our target branch condition)
    if (y >= 1.0) {
        printf("Target branch condition met: y >= 1.0\n");
    } else {
        printf("Target branch condition not met: y < 1.0\n");
    }
    
    return 0;
}
