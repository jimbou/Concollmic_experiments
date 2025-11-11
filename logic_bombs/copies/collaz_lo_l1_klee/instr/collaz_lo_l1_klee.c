#include <stdio.h>
#include <string.h> 
#include "utils.h"
#include "a_tester.h"

int f(int x){
    fprintf(stderr, "[collaz_lo_l1_klee.c] enter f 1\n");
    if (x%2 == 0) {
        fprintf(stderr, "[collaz_lo_l1_klee.c] enter f 2\n");
        return x/2;
        // fprintf(stderr, "[collaz_lo_l1_klee.c] exit f 2\n");
    }
    fprintf(stderr, "[collaz_lo_l1_klee.c] enter f 3\n");
    return 3*x + 1;
    // fprintf(stderr, "[collaz_lo_l1_klee.c] exit f 3\n");
    // fprintf(stderr, "[collaz_lo_l1_klee.c] exit f 1\n");
}

// {"s":{"length": 4}}
int logic_bomb(char* s) {
    fprintf(stderr, "[collaz_lo_l1_klee.c] enter logic_bomb 1\n");
    int symvar = s[0] - 48;
    symvar = symvar + 94;
    int j = f(symvar);
    int loopcount = 1;
    // fprintf(stderr, "[collaz_lo_l1_klee.c] exit logic_bomb 1\n");

    while(j != 1){
        fprintf(stderr, "[collaz_lo_l1_klee.c] enter logic_bomb 2\n");
        j = f(j);
        loopcount ++;
        // fprintf(stderr, "[collaz_lo_l1_klee.c] exit logic_bomb 2\n");
    }

    if(loopcount == 25){
        fprintf(stderr, "[collaz_lo_l1_klee.c] enter logic_bomb 3\n");
        fprintf(stderr, "Logic bomb triggered\n");
        return 1;
        // fprintf(stderr, "[collaz_lo_l1_klee.c] exit logic_bomb 3\n");
    } else {
        fprintf(stderr, "[collaz_lo_l1_klee.c] enter logic_bomb 4\n");
        fprintf(stderr, "Path without the bomb\n");
        return 0;
        // fprintf(stderr, "[collaz_lo_l1_klee.c] exit logic_bomb 4\n");
    }
}

int main(int argc, char **argv) {
    fprintf(stderr, "[collaz_lo_l1_klee.c] enter main 1\n");
    if (argc < 2) {
        fprintf(stderr, "[collaz_lo_l1_klee.c] enter main 2\n");
        fprintf(stderr, "Usage: %s <input-string>\n", argv[0]);
        return 2;
        // fprintf(stderr, "[collaz_lo_l1_klee.c] exit main 2\n");
    }
    char *s = argv[1];
    // fprintf(stderr, "[collaz_lo_l1_klee.c] exit main 1\n");
    fprintf(stderr, "[collaz_lo_l1_klee.c] enter main 3\n");
    return logic_bomb(s);
    // fprintf(stderr, "[collaz_lo_l1_klee.c] exit main 3\n");
}
// Total cost: 0.008358
// Total split cost: 0.000000, input tokens: 0, output tokens: 0, cache read tokens: 0, cache write tokens: 0, split chunks: [(0, 38)]
// Total instrumented cost: 0.008358, input tokens: 2359, output tokens: 502, cache read tokens: 0, cache write tokens: 0
