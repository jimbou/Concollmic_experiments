#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <math.h>
#include "fdlibm.h"

#ifdef __STDC__ 
    int matherr(struct exception *x)
#else 
    int matherr(x) 
    struct exception *x; 
#endif 
{ 
int n=0; 
if(x->arg1!=x->arg1) return 0; 
return n; 
}

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <double>\n", argv[0]);
        return 1;
    }

    errno = 0;
    double x = strtod(argv[1], NULL);

    

    struct exception ex;
    ex.type = 0;
    ex.name = "matherr_test";
    ex.arg1 = x;
    ex.arg2 = 0.0;
    ex.retval = 0.0;

    int r = matherr(&ex);

    printf("matherr returned %d\n", r);
    return 0;
}
