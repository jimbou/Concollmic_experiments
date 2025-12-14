
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <errno.h>

extern char **environ;

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <number>\n", argv[0]);
        return 1;
    }
    
    double x = atof(argv[1]);
    printf("Input: %f\n", x);
    
    // Try various environment variables that might affect math library behavior
    putenv("MATH_ERRHANDLING=2");  // Set to MATH_ERREXCEPT
    putenv("POSIXLY_CORRECT=1");
    putenv("_SVID_SOURCE=1");
    putenv("_XOPEN_SOURCE=1");
    putenv("_LIB_VERSION=_SVID_");
    
    // Print all environment variables for debugging
    printf("Environment variables:\n");
    for (char **env = environ; *env != NULL; env++) {
        printf("  %s\n", *env);
    }
    
    // Call exp function
    double result = exp(x);
    
    printf("Result: %f\n", result);
    printf("Errno after exp: %d\n", errno);
    
    return 0;
}
