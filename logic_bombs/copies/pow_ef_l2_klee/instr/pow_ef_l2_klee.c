#include <stdio.h>
/*
TOY:
*/
#include <string.h> 
#include <math.h>
#include "utils.h"
#include "a_tester.h"

// {"s":{"length": 4}}
int logic_bomb(char* s) {
    fprintf(stderr, "[pow_ef_l2_klee.c] enter logic_bomb 1\n");
    int symvar = s[0] - 48;
    if(pow(symvar, 2) == 49){
        fprintf(stderr, "[pow_ef_l2_klee.c] enter logic_bomb 2\n");
        fprintf(stderr, "Logic bomb triggered\n");
        return 1;
        // fprintf(stderr, "[pow_ef_l2_klee.c] exit logic_bomb 2\n");
    }else{
        fprintf(stderr, "[pow_ef_l2_klee.c] enter logic_bomb 3\n");
        fprintf(stderr, "Path without the bomb\n");
        return 0;
        // fprintf(stderr, "[pow_ef_l2_klee.c] exit logic_bomb 3\n");
    }
    // fprintf(stderr, "[pow_ef_l2_klee.c] exit logic_bomb 1\n");
}

int main(int argc, char **argv) {
    fprintf(stderr, "\n");
    if (argc < 2) {
        fprintf(stderr, "[pow_ef_l2_klee.c] enter main 2\n");
        fprintf(stderr, "Usage: %s <input-string>\n", argv[0]);
        return 2;
        // fprintf(stderr, "[pow_ef_l2_klee.c] exit main 2\n");
    }
    fprintf(stderr, "[pow_ef_l2_klee.c] enter main 3\n");
    char *s = argv[1];
    return logic_bomb(s);
    // fprintf(stderr, "[pow_ef_l2_klee.c] exit main 3\n");
}
// Total cost: 0.006503
// Total split cost: 0.000000, input tokens: 0, output tokens: 0, cache read tokens: 0, cache write tokens: 0, split chunks: [(0, 29)]
// Total instrumented cost: 0.006503, input tokens: 2301, output tokens: 331, cache read tokens: 0, cache write tokens: 0
