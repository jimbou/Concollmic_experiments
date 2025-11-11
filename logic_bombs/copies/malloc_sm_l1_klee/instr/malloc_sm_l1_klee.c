#include <stdio.h>
#include<stdio.h>
#include<stdlib.h>
#include"utils.h"
#include "a_tester.h"

// {"s":{"length": 4}}
int logic_bomb(char* s) {
    fprintf(stderr, "[malloc_sm_l1_klee.c] enter logic_bomb 1\n");
    int symvar = s[0] - 48;
    int *array = (int *) malloc(sizeof(int) * 10);
    int k = 0;
    // fprintf(stderr, "[malloc_sm_l1_klee.c] exit logic_bomb 1\n");

    for (k=0; k<10; k++){
        fprintf(stderr, "[malloc_sm_l1_klee.c] enter logic_bomb 2\n");
        array[k] = k;
        // fprintf(stderr, "[malloc_sm_l1_klee.c] exit logic_bomb 2\n");
    }

    if(array[symvar % 10] == 7){
        fprintf(stderr, "[malloc_sm_l1_klee.c] enter logic_bomb 3\n");
        fprintf(stderr, "Logic bomb triggered\n");
        return 1;
        // fprintf(stderr, "[malloc_sm_l1_klee.c] exit logic_bomb 3\n");
    }

    fprintf(stderr, "[malloc_sm_l1_klee.c] enter logic_bomb 4\n");
    fprintf(stderr, "Path without the bomb\n");
    return 0;
    // fprintf(stderr, "[malloc_sm_l1_klee.c] exit logic_bomb 4\n");
}

int main(int argc, char **argv) {
    fprintf(stderr, "[malloc_sm_l1_klee.c] enter main 1\n");
    if (argc < 2) {
        fprintf(stderr, "[malloc_sm_l1_klee.c] enter main 2\n");
        fprintf(stderr, "Usage: %s <input-string>\n", argv[0]);
        return 2;
        // fprintf(stderr, "[malloc_sm_l1_klee.c] exit main 2\n");
    }
    char *s = argv[1];
    // fprintf(stderr, "[malloc_sm_l1_klee.c] exit main 1\n");

    fprintf(stderr, "[malloc_sm_l1_klee.c] enter main 3\n");
    return logic_bomb(s);
    // fprintf(stderr, "[malloc_sm_l1_klee.c] exit main 3\n");
}
// Total cost: 0.007310
// Total split cost: 0.000000, input tokens: 0, output tokens: 0, cache read tokens: 0, cache write tokens: 0, split chunks: [(0, 30)]
// Total instrumented cost: 0.007310, input tokens: 2332, output tokens: 404, cache read tokens: 0, cache write tokens: 0
