#include <stdio.h>
#include<stdio.h>
#include <stdlib.h>
#include "a_tester.h"

// {"s":{"length": 8}}
int logic_bomb(char* symvar) {
    fprintf(stderr, "[float5_fp_l2_klee.c] enter logic_bomb 1\n");
    float x = atof(symvar);
    x = x/10.0;
    x = x + 0.1;
    x = x * x;
    // fprintf(stderr, "[float5_fp_l2_klee.c] exit logic_bomb 1\n");

    if (x > 0.1) {
        fprintf(stderr, "[float5_fp_l2_klee.c] enter logic_bomb 2\n");
        x -= x;
        // fprintf(stderr, "[float5_fp_l2_klee.c] exit logic_bomb 2\n");
    }

    if(x != 0.02) {
        fprintf(stderr, "[float5_fp_l2_klee.c] enter logic_bomb 3\n");
        x = x + 7.98;
        // fprintf(stderr, "[float5_fp_l2_klee.c] exit logic_bomb 3\n");

        if(x == 8) {
            fprintf(stderr, "[float5_fp_l2_klee.c] enter logic_bomb 4\n");
            fprintf(stderr, "Logic bomb triggered\n");
            return 1;
            // fprintf(stderr, "[float5_fp_l2_klee.c] exit logic_bomb 4\n");
        }
    }

    fprintf(stderr, "[float5_fp_l2_klee.c] enter logic_bomb 5\n");
    fprintf(stderr, "Path without the bomb\n");
    return 0;
    // fprintf(stderr, "[float5_fp_l2_klee.c] exit logic_bomb 5\n");
}

int main(int argc, char **argv) {
    fprintf(stderr, "[float5_fp_l2_klee.c] enter main 1\n");
    if (argc < 2) {
        fprintf(stderr, "[float5_fp_l2_klee.c] enter main 2\n");
        fprintf(stderr, "Usage: %s <input-string>\n", argv[0]);
        return 2;
        // fprintf(stderr, "[float5_fp_l2_klee.c] exit main 2\n");
    }
    char *s = argv[1];
    // fprintf(stderr, "[float5_fp_l2_klee.c] exit main 1\n");

    fprintf(stderr, "[float5_fp_l2_klee.c] enter main 3\n");
    return logic_bomb(s);
    // fprintf(stderr, "[float5_fp_l2_klee.c] exit main 3\n");
}
// Total cost: 0.007648
// Total split cost: 0.000000, input tokens: 0, output tokens: 0, cache read tokens: 0, cache write tokens: 0, split chunks: [(0, 32)]
// Total instrumented cost: 0.007648, input tokens: 2335, output tokens: 437, cache read tokens: 0, cache write tokens: 0
