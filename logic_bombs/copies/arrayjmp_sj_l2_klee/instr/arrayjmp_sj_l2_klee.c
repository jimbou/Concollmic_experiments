#include <stdio.h>
#include <stdio.h>
#include "utils.h"

#define jmp(addr) asm("jmp *%0"::"r"(addr):)

#include "a_tester.h"

// {"s":{"length": 4}}
int logic_bomb(char* s) {
    fprintf(stderr, "[arrayjmp_sj_l2_klee.c] enter logic_bomb 1\n");
    int symvar = s[0] - 48;
    int array[] = {7,13,14,15,16,21,22,37,23,24};
    long long addr = &&flag_0 + array[symvar%10];
    jmp(addr);
    // fprintf(stderr, "[arrayjmp_sj_l2_klee.c] exit logic_bomb 1\n");

  flag_0:
    fprintf(stderr, "[arrayjmp_sj_l2_klee.c] enter logic_bomb 2\n");
    if (symvar > 0){
        fprintf(stderr, "[arrayjmp_sj_l2_klee.c] enter logic_bomb 3\n");
        symvar++;
        // fprintf(stderr, "[arrayjmp_sj_l2_klee.c] exit logic_bomb 3\n");
        if(symvar == 0){
            fprintf(stderr, "[arrayjmp_sj_l2_klee.c] enter logic_bomb 4\n");
            fprintf(stderr, "Logic bomb triggered\n");
            return 1;
            // fprintf(stderr, "[arrayjmp_sj_l2_klee.c] exit logic_bomb 4\n");
        }
    }
    // fprintf(stderr, "[arrayjmp_sj_l2_klee.c] exit logic_bomb 2\n");
    fprintf(stderr, "[arrayjmp_sj_l2_klee.c] enter logic_bomb 5\n");
    fprintf(stderr, "Path without the bomb\n");
    return 0;
    // fprintf(stderr, "[arrayjmp_sj_l2_klee.c] exit logic_bomb 5\n");
}

int main(int argc, char **argv) {
    fprintf(stderr, "[arrayjmp_sj_l2_klee.c] enter main 1\n");
    if (argc < 2) {
        fprintf(stderr, "[arrayjmp_sj_l2_klee.c] enter main 2\n");
        fprintf(stderr, "Usage: %s <input-string>\n", argv[0]);
        return 2;
        // fprintf(stderr, "[arrayjmp_sj_l2_klee.c] exit main 2\n");
    }
    // fprintf(stderr, "[arrayjmp_sj_l2_klee.c] exit main 1\n");
    fprintf(stderr, "[arrayjmp_sj_l2_klee.c] enter main 3\n");
    char *s = argv[1];
    return logic_bomb(s);
    // fprintf(stderr, "[arrayjmp_sj_l2_klee.c] exit main 3\n");
}
// Total cost: 0.007992
// Total split cost: 0.000000, input tokens: 0, output tokens: 0, cache read tokens: 0, cache write tokens: 0, split chunks: [(0, 34)]
// Total instrumented cost: 0.007992, input tokens: 2369, output tokens: 463, cache read tokens: 0, cache write tokens: 0
