#include <stdio.h>
#include <string.h> 
#include "utils.h"
#include "a_tester.h"

int trigger(){
    fprintf(stderr, "[stack_bo_l2_klee.c] enter trigger 1\n");
    fprintf(stderr, "Logic bomb triggered\n");
    return 1;
    // fprintf(stderr, "[stack_bo_l2_klee.c] exit trigger 1\n");
}

// {"symvar":{"length": 128}}
int logic_bomb(char* symvar) {
    fprintf(stderr, "[stack_bo_l2_klee.c] enter logic_bomb 1\n");
    char buf[8];
    strcpy(buf, symvar);
    // fprintf(stderr, "[stack_bo_l2_klee.c] exit logic_bomb 1\n");

    if(buf < 0) {
        fprintf(stderr, "[stack_bo_l2_klee.c] enter logic_bomb 2\n");
        return trigger();
        // fprintf(stderr, "[stack_bo_l2_klee.c] exit logic_bomb 2\n");
    }

    fprintf(stderr, "[stack_bo_l2_klee.c] enter logic_bomb 3\n");
    fprintf(stderr, "Path without the bomb\n");
    return 0;
    // fprintf(stderr, "[stack_bo_l2_klee.c] exit logic_bomb 3\n");
}

int main(int argc, char **argv) {
    fprintf(stderr, "[stack_bo_l2_klee.c] enter main 1\n");
    if (argc < 2) {
        fprintf(stderr, "[stack_bo_l2_klee.c] enter main 2\n");
        fprintf(stderr, "Usage: %s <input-string>\n", argv[0]);
        return 2;
        // fprintf(stderr, "[stack_bo_l2_klee.c] exit main 2\n");
    }
    // fprintf(stderr, "[stack_bo_l2_klee.c] exit main 1\n");

    fprintf(stderr, "[stack_bo_l2_klee.c] enter main 3\n");
    char *s = argv[1];
    return logic_bomb(s);
    // fprintf(stderr, "[stack_bo_l2_klee.c] exit main 3\n");
}
// Total cost: 0.006715
// Total split cost: 0.000000, input tokens: 0, output tokens: 0, cache read tokens: 0, cache write tokens: 0, split chunks: [(0, 28)]
// Total instrumented cost: 0.006715, input tokens: 2286, output tokens: 356, cache read tokens: 0, cache write tokens: 0
