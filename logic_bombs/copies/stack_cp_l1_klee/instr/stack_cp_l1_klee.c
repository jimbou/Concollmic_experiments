#include <stdio.h>
#include "a_tester.h"

//#define push(v) asm volatile ("push %0"::"m"(v))

// {"s":{"length": 4}}
int logic_bomb(char* s) {
    fprintf(stderr, "[stack_cp_l1_klee.c] enter logic_bomb 1\n");
    int symvar = s[0] - 48;
    int j;
    __asm__ __volatile__("push %0" :: "m"(symvar));
    __asm__ __volatile__("pop %0" :: "m"(j));
    // fprintf(stderr, "[stack_cp_l1_klee.c] exit logic_bomb 1\n");

    if(j == 7){
        fprintf(stderr, "[stack_cp_l1_klee.c] enter logic_bomb 2\n");
        fprintf(stderr, "Logic bomb triggered\n");
        return 1;
        // fprintf(stderr, "[stack_cp_l1_klee.c] exit logic_bomb 2\n");
    } else{
        fprintf(stderr, "[stack_cp_l1_klee.c] enter logic_bomb 3\n");
        fprintf(stderr, "Path without the bomb\n");
        return 0;
        // fprintf(stderr, "[stack_cp_l1_klee.c] exit logic_bomb 3\n");
    }
}

int main(int argc, char **argv) {
    fprintf(stderr, "[stack_cp_l1_klee.c] enter main 1\n");
    if (argc < 2) {
        fprintf(stderr, "[stack_cp_l1_klee.c] enter main 2\n");
        fprintf(stderr, "Usage: %s <input-string>\n", argv[0]);
        return 2;
        // fprintf(stderr, "[stack_cp_l1_klee.c] exit main 2\n");
    }
    char *s = argv[1];
    // fprintf(stderr, "[stack_cp_l1_klee.c] exit main 1\n");

    fprintf(stderr, "[stack_cp_l1_klee.c] enter main 3\n");
    return logic_bomb(s);
    // fprintf(stderr, "[stack_cp_l1_klee.c] exit main 3\n");
}
// Total cost: 0.006980
// Total split cost: 0.000000, input tokens: 0, output tokens: 0, cache read tokens: 0, cache write tokens: 0, split chunks: [(0, 28)]
// Total instrumented cost: 0.006980, input tokens: 2328, output tokens: 372, cache read tokens: 0, cache write tokens: 0
