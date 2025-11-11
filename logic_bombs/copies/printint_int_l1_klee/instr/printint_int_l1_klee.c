#include <stdio.h>
/*
TOY:
Solution: 7
*/
#include <string.h> 
#include "utils.h"
#include "a_tester.h"

// {"s":{"length": 4}}
int logic_bomb(char* s) {
    fprintf(stderr, "[printint_int_l1_klee.c] enter logic_bomb 1\n");
    int symvar = s[0] - 48;
    int x = symvar + 190;
    printf("x = %d\n", x);
    // fprintf(stderr, "[printint_int_l1_klee.c] exit logic_bomb 1\n");

    if(x == 197){
        fprintf(stderr, "[printint_int_l1_klee.c] enter logic_bomb 2\n");
        fprintf(stderr, "Logic bomb triggered\n");
        return 1;
        // fprintf(stderr, "[printint_int_l1_klee.c] exit logic_bomb 2\n");
    }else{
        fprintf(stderr, "[printint_int_l1_klee.c] enter logic_bomb 3\n");
        fprintf(stderr, "Path without the bomb\n");
        return 0;
        // fprintf(stderr, "[printint_int_l1_klee.c] exit logic_bomb 3\n");
    }
}

int main(int argc, char **argv) {
    fprintf(stderr, "[printint_int_l1_klee.c] enter main 1\n");
    if (argc < 2) {
        fprintf(stderr, "[printint_int_l1_klee.c] enter main 2\n");
        fprintf(stderr, "Usage: %s <input-string>\n", argv[0]);
        return 2;
        // fprintf(stderr, "[printint_int_l1_klee.c] exit main 2\n");
    }
    char *s = argv[1];
    // fprintf(stderr, "[printint_int_l1_klee.c] exit main 1\n");

    fprintf(stderr, "[printint_int_l1_klee.c] enter main 3\n");
    return logic_bomb(s);
    // fprintf(stderr, "[printint_int_l1_klee.c] exit main 3\n");
}
// Total cost: 0.006810
// Total split cost: 0.000000, input tokens: 0, output tokens: 0, cache read tokens: 0, cache write tokens: 0, split chunks: [(0, 31)]
// Total instrumented cost: 0.006810, input tokens: 2316, output tokens: 358, cache read tokens: 0, cache write tokens: 0
