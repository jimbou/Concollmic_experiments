
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <fenv.h>

int main() {
    // Try to print information about math error handling
    printf("MATH_ERRHANDLING: %d\n", MATH_ERRHANDLING);
    
    // Try to access _LIB_VERSION if available
    #ifdef _LIB_VERSION
        printf("_LIB_VERSION is defined\n");
        // Try to determine the current value
        if (_LIB_VERSION == _IEEE_) printf("_LIB_VERSION = _IEEE_\n");
        if (_LIB_VERSION == _SVID_) printf("_LIB_VERSION = _SVID_\n");
        if (_LIB_VERSION == _XOPEN_) printf("_LIB_VERSION = _XOPEN_\n");
        if (_LIB_VERSION == _POSIX_) printf("_LIB_VERSION = _POSIX_\n");
    #else
        printf("_LIB_VERSION is not directly accessible\n");
    #endif
    
    return 0;
}
