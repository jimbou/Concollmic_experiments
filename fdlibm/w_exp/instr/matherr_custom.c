
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <errno.h>

// Define the matherr function to handle math errors in SVID mode
int matherr(struct exception *e) {
    printf("matherr called: type=%d, name=%s, arg1=%f\n", 
           e->type, e->name, e->arg1);
    // Set a custom return value
    e->retval = 42.0;
    // Return 1 to indicate we've handled the error
    return 1;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <number>\n", argv[0]);
        return 1;
    }
    
    double x = atof(argv[1]);
    printf("Input: %f\n", x);
    
    // Reset errno
    errno = 0;
    
    // Call exp function
    double result = exp(x);
    
    printf("Result: %f\n", result);
    printf("Errno after exp: %d\n", errno);
    
    return 0;
}
