#include <stdio.h>
#include <string.h> 
#include "utils.h"
#include "a_tester.h"

long f(long x){
    fprintf(stderr, "[7n+1_lo_l1_klee.c] enter f 1\n");
    if (x%2 == 0) {
        fprintf(stderr, "[7n+1_lo_l1_klee.c] enter f 2\n");
        return x/2;
        // fprintf(stderr, "[7n+1_lo_l1_klee.c] exit f 2\n");
    }
    else if (x%3 == 0) {
        fprintf(stderr, "[7n+1_lo_l1_klee.c] enter f 3\n");
        return x/3;
        // fprintf(stderr, "[7n+1_lo_l1_klee.c] exit f 3\n");
    }
    else if (x%5 == 0) {
        fprintf(stderr, "[7n+1_lo_l1_klee.c] enter f 4\n");
        return x/5;
        // fprintf(stderr, "[7n+1_lo_l1_klee.c] exit f 4\n");
    }
    else {
        fprintf(stderr, "[7n+1_lo_l1_klee.c] enter f 5\n");
        return 7*x + 1;
        // fprintf(stderr, "[7n+1_lo_l1_klee.c] exit f 5\n");
    }
    // fprintf(stderr, "[7n+1_lo_l1_klee.c] exit f 1\n");
}

// {"s":{"length": 4}}
int logic_bomb(char* s) {
    fprintf(stderr, "[7n+1_lo_l1_klee.c] enter logic_bomb 1\n");
    int symvar = s[0] - 48;
    symvar = symvar + 1104;
    long j = f(symvar);
    int loopcount = 1;
    // fprintf(stderr, "[7n+1_lo_l1_klee.c] exit logic_bomb 1\n");

    while(j != 1){
        fprintf(stderr, "[7n+1_lo_l1_klee.c] enter logic_bomb 2\n");
        j = f(j);
        loopcount ++;
        // fprintf(stderr, "[7n+1_lo_l1_klee.c] exit logic_bomb 2\n");
    }

    if(loopcount == 50){
        fprintf(stderr, "[7n+1_lo_l1_klee.c] enter logic_bomb 3\n");
        fprintf(stderr, "Logic bomb triggered\n");
        return 1;
        // fprintf(stderr, "[7n+1_lo_l1_klee.c] exit logic_bomb 3\n");
    } else {
        fprintf(stderr, "[7n+1_lo_l1_klee.c] enter logic_bomb 4\n");
        fprintf(stderr, "Path without the bomb\n");
        return 0;
        // fprintf(stderr, "[7n+1_lo_l1_klee.c] exit logic_bomb 4\n");
    }
}

int main(int argc, char **argv) {
    fprintf(stderr, "[7n+1_lo_l1_klee.c] enter main 1\n");
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <input-string>\n", argv[0]);
        return 2;
        // fprintf(stderr, "[7n+1_lo_l1_klee.c] exit main 1\n");
    }
    char *s = argv[1];
    fprintf(stderr, "[7n+1_lo_l1_klee.c] enter main 2\n");
    return logic_bomb(s);
    // fprintf(stderr, "[7n+1_lo_l1_klee.c] exit main 2\n");
}
// Total cost: 0.004404
// Total split cost: 0.000000, input tokens: 0, output tokens: 0, cache read tokens: 0, cache write tokens: 0, split chunks: [(0, 43)]
// Total instrumented cost: 0.004404, input tokens: 19326, output tokens: 3852, cache read tokens: 0, cache write tokens: 0
