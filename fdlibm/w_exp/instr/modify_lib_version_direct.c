
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

// Try to access _LIB_VERSION directly
#if defined(_SVID_SOURCE) || defined(_XOPEN_SOURCE) || defined(_GNU_SOURCE) || defined(_DEFAULT_SOURCE)
extern int _LIB_VERSION;
extern int _IEEE_, _SVID_, _XOPEN_, _POSIX_;
#endif

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <number>\n", argv[0]);
        return 1;
    }
    
    double x = atof(argv[1]);
    printf("Input: %f\n", x);
    
    // Try to print and modify _LIB_VERSION if available
    #if defined(_SVID_SOURCE) || defined(_XOPEN_SOURCE) || defined(_GNU_SOURCE) || defined(_DEFAULT_SOURCE)
    printf("_LIB_VERSION before: %d\n", _LIB_VERSION);
    printf("_IEEE_=%d, _SVID_=%d, _XOPEN_=%d, _POSIX_=%d\n", _IEEE_, _SVID_, _XOPEN_, _POSIX_);
    
    // Try to modify it
    _LIB_VERSION = _SVID_;
    printf("_LIB_VERSION after setting to _SVID_: %d\n", _LIB_VERSION);
    #else
    printf("_LIB_VERSION not directly accessible\n");
    #endif
    
    // Call exp function
    double result = exp(x);
    printf("Result: %f\n", result);
    
    return 0;
}
