
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <errno.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <value>\n", argv[0]);
        return 1;
    }
    
    double x = atof(argv[1]);
    fprintf(stderr, "Input: %f\n", x);
    
    // Calculate atanh(x)
    errno = 0;
    double y = atanh(x);
    
    fprintf(stderr, "atanh(%f) = %f\n", x, y);
    fprintf(stderr, "errno = %d\n", errno);
    
    // Check if y >= 1.0 (our target branch condition)
    if (y >= 1.0) {
        fprintf(stderr, "TARGET BRANCH REACHED: y >= 1.0\n");
        // Do something that would only happen in this branch
        return 42;  // Special return code to indicate we reached the target branch
    } else {
        fprintf(stderr, "Not in target branch: y < 1.0\n");
        return 0;
    }
}
