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
fprintf(stderr, "\n");
int n=0; 
if(x->arg1!=x->arg1) {
fprintf(stderr, "[s_matherr.c] enter matherr 2\n");
return 0;
// fprintf(stderr, "[s_matherr.c] exit matherr 2\n");
}
fprintf(stderr, "[s_matherr.c] enter matherr 3\n");
return n; 
// fprintf(stderr, "[s_matherr.c] exit matherr 3\n");
}

int main(int argc, char **argv) {
fprintf(stderr, "\n");
    if (argc != 2) {
fprintf(stderr, "[s_matherr.c] enter main 2\n");
        fprintf(stderr, "Usage: %s <double>\n", argv[0]);
        return 1;
// fprintf(stderr, "[s_matherr.c] exit main 2\n");
    }

fprintf(stderr, "[s_matherr.c] enter main 3\n");
    errno = 0;
    double x = strtod(argv[1], NULL);
    // fprintf(stderr, "[s_matherr.c] exit main 3\n");

    

fprintf(stderr, "[s_matherr.c] enter main 4\n");
    struct exception ex;
    ex.type = 0;
    ex.name = "matherr_test";
    ex.arg1 = x;
    ex.arg2 = 0.0;
    ex.retval = 0.0;
    // fprintf(stderr, "[s_matherr.c] exit main 4\n");

fprintf(stderr, "[s_matherr.c] enter main 5\n");
    int r = matherr(&ex);
    // fprintf(stderr, "[s_matherr.c] exit main 5\n");

fprintf(stderr, "[s_matherr.c] enter main 6\n");
    printf("matherr returned %d\n", r);
    return 0;
// fprintf(stderr, "[s_matherr.c] exit main 6\n");
}
// Total cost: 0.001146
// Total split cost: 0.000000, input tokens: 0, output tokens: 0, cache read tokens: 0, cache write tokens: 0, split chunks: [(0, 41)]
// Total instrumented cost: 0.001146, input tokens: 2398, output tokens: 453, cache read tokens: 0, cache write tokens: 0
