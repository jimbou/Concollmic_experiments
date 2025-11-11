#include <stdio.h>
#include<stdio.h>
#include <stdlib.h>
#include "a_tester.h"

// {"s":{"length": 8}}
int logic_bomb(char* symvar) {
    fprintf(stderr, "[float4_fp_l2_klee.c] enter logic_bomb 1\n");
    float x = atof(symvar);
    x = x/-10000.0;
    // fprintf(stderr, "[float4_fp_l2_klee.c] exit logic_bomb 1\n");

    if(1024+x == 1024 && x>0){
        fprintf(stderr, "[float4_fp_l2_klee.c] enter logic_bomb 2\n");
        fprintf(stderr, "Logic bomb triggered\n");
        return 1;
        // fprintf(stderr, "[float4_fp_l2_klee.c] exit logic_bomb 2\n");
    } else {
        fprintf(stderr, "[float4_fp_l2_klee.c] enter logic_bomb 3\n");
        fprintf(stderr, "Path without the bomb\n");
        return 0;
        // fprintf(stderr, "[float4_fp_l2_klee.c] exit logic_bomb 3\n");
    }
}

int main(int argc, char **argv) {
    fprintf(stderr, "[float4_fp_l2_klee.c] enter main 1\n");
    if (argc < 2) {
        fprintf(stderr, "[float4_fp_l2_klee.c] enter main 2\n");
        fprintf(stderr, "Usage: %s <input-string>\n", argv[0]);
        return 2;
        // fprintf(stderr, "[float4_fp_l2_klee.c] exit main 2\n");
    }
    char *s = argv[1];
    // fprintf(stderr, "[float4_fp_l2_klee.c] exit main 1\n");

    fprintf(stderr, "[float4_fp_l2_klee.c] enter main 3\n");
    return logic_bomb(s);
    // fprintf(stderr, "[float4_fp_l2_klee.c] exit main 3\n");
}
// Total cost: 0.006560
// Total split cost: 0.000000, input tokens: 0, output tokens: 0, cache read tokens: 0, cache write tokens: 0, split chunks: [(0, 25)]
// Total instrumented cost: 0.006560, input tokens: 2296, output tokens: 338, cache read tokens: 0, cache write tokens: 0
