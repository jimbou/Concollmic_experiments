#include <stdio.h>
#include <string.h> 
#include "utils.h"
#include "a_tester.h"

int n = 5;

// {"symvar":{"length": 256}}
int logic_bomb(char* symvar) {
    fprintf(stderr, "[heap_bo_l1_klee.c] enter logic_bomb 1\n");
    char *p, *q;
    p = (char*)malloc(16);
    q = (char*)malloc(16);
    strcpy(p, symvar);
    free(q);
    // fprintf(stderr, "[heap_bo_l1_klee.c] exit logic_bomb 1\n");

    if (n != 5){
        fprintf(stderr, "[heap_bo_l1_klee.c] enter logic_bomb 2\n");
        free(p);
        fprintf(stderr, "Logic bomb triggered\n");
        return 1;
        // fprintf(stderr, "[heap_bo_l1_klee.c] exit logic_bomb 2\n");
    } else {
        fprintf(stderr, "[heap_bo_l1_klee.c] enter logic_bomb 3\n");
        free(p);
        fprintf(stderr, "Path without the bomb\n");
        return 0;
        // fprintf(stderr, "[heap_bo_l1_klee.c] exit logic_bomb 3\n");
    }
}

int main(int argc, char **argv) {
    fprintf(stderr, "[heap_bo_l1_klee.c] enter main 1\n");
    if (argc < 2) {
        fprintf(stderr, "[heap_bo_l1_klee.c] enter main 2\n");
        fprintf(stderr, "Usage: %s <input-string>\n", argv[0]);
        return 2;
        // fprintf(stderr, "[heap_bo_l1_klee.c] exit main 2\n");
    }
    char *s = argv[1];
    // fprintf(stderr, "[heap_bo_l1_klee.c] exit main 1\n");

    fprintf(stderr, "[heap_bo_l1_klee.c] enter main 3\n");
    return logic_bomb(s);
    // fprintf(stderr, "[heap_bo_l1_klee.c] exit main 3\n");
}
// Total cost: 0.006943
// Total split cost: 0.000000, input tokens: 0, output tokens: 0, cache read tokens: 0, cache write tokens: 0, split chunks: [(0, 33)]
// Total instrumented cost: 0.006943, input tokens: 2325, output tokens: 369, cache read tokens: 0, cache write tokens: 0
