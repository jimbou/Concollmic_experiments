
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <errno.h>

#if defined(_SVID_SOURCE) || defined(_XOPEN_SOURCE)
#include <math.h>
struct exception {
    int type;
    char *name;
    double arg1;
    double arg2;
    double retval;
};

int matherr(struct exception *e) {
    printf("matherr called: type=%d, name=%s, arg1=%f\n", 
           e->type, e->name, e->arg1);
    return 1; /* Error has been handled */
}
#endif

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
