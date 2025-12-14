
#include <stdio.h>
#include <math.h>
#include <errno.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <value>\n", argv[0]);
        return 1;
    }
    
    double x = atof(argv[1]);
    printf("Input value: %f\n", x);
    
    #ifdef _LIB_VERSION
        printf("_LIB_VERSION is defined\n");
        #ifdef _SVID_
            printf("Setting _LIB_VERSION to _SVID_\n");
            _LIB_VERSION = _SVID_;
        #endif
    #else
        printf("_LIB_VERSION is not defined\n");
    #endif
    
    errno = 0;
    double result = atanh(x);
    
    printf("atanh(%f) = %f\n", x, result);
    printf("errno = %d\n", errno);
    
    return 0;
}
