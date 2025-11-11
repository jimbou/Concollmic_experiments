#include <stdio.h>
#include <stdio.h>
#include "utils.h"

#define jmp(addr) asm("jmp *%0"::"r"(addr):)

#include "a_tester.h"

// {"s":{"length": 4}}
int logic_bomb(char* s) {
    fprintf(stderr, "[jmp_sj_l1_klee.c] enter logic_bomb 1\n");
    int symvar = s[0] - 48;
    if (symvar%6 != 1 || symvar < 10|| symvar > 40 || symvar == 19)
    {
        fprintf(stderr, "[jmp_sj_l1_klee.c] enter logic_bomb 2\n");
        symvar = 13;
        // fprintf(stderr, "[jmp_sj_l1_klee.c] exit logic_bomb 2\n");
    }
    long long addr = &&flag_0 + symvar;
    jmp(addr);
  flag_0:
    fprintf(stderr, "[jmp_sj_l1_klee.c] enter logic_bomb 3\n");
    if (symvar > 0){
        fprintf(stderr, "[jmp_sj_l1_klee.c] enter logic_bomb 4\n");
        symvar++;
        // fprintf(stderr, "[jmp_sj_l1_klee.c] exit logic_bomb 4\n");
        if(symvar == 0) {
            fprintf(stderr, "[jmp_sj_l1_klee.c] enter logic_bomb 5\n");
            fprintf(stderr, "Logic bomb triggered\n");
            return 1;
            // fprintf(stderr, "[jmp_sj_l1_klee.c] exit logic_bomb 5\n");
        }
    }
    fprintf(stderr, "Path without the bomb\n");
    return 0;
    // fprintf(stderr, "[jmp_sj_l1_klee.c] exit logic_bomb 3\n");
    // fprintf(stderr, "[jmp_sj_l1_klee.c] exit logic_bomb 1\n");
}

int main(int argc, char **argv) {
    fprintf(stderr, "[jmp_sj_l1_klee.c] enter main 1\n");
    if (argc < 2) {
        fprintf(stderr, "[jmp_sj_l1_klee.c] enter main 2\n");
        fprintf(stderr, "Usage: %s <input-string>\n", argv[0]);
        return 2;
        // fprintf(stderr, "[jmp_sj_l1_klee.c] exit main 2\n");
    }
    char *s = argv[1];
    fprintf(stderr, "[jmp_sj_l1_klee.c] enter main 3\n");
    return logic_bomb(s);
    // fprintf(stderr, "[jmp_sj_l1_klee.c] exit main 3\n");
    // fprintf(stderr, "[jmp_sj_l1_klee.c] exit main 1\n");
}
// Total cost: 0.010675
// Total split cost: 0.000000, input tokens: 0, output tokens: 0, cache read tokens: 0, cache write tokens: 0, split chunks: [(0, 35)]
// Total instrumented cost: 0.010675, input tokens: 2374, output tokens: 474, cache read tokens: 0, cache write tokens: 0
