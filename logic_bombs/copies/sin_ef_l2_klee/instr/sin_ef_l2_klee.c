#include <stdio.h>
/*
TOY:
Solution: 30
*/
#include <string.h> 
#include <math.h>
#include "utils.h"

#define PI 3.14159265358979323846264338327

#include "a_tester.h"

// {"s":{"length": 4}}
int logic_bomb(char* s) {
    fprintf(stderr, "[sin_ef_l2_klee.c] enter logic_bomb 1\n");
    int symvar = s[0];
    float v = sin(symvar*PI/30);
    // fprintf(stderr, "[sin_ef_l2_klee.c] exit logic_bomb 1\n");

    if(v > 0.5){
        fprintf(stderr, "[sin_ef_l2_klee.c] enter logic_bomb 2\n");
        fprintf(stderr, "Logic bomb triggered\n");
        return 1;
        // fprintf(stderr, "[sin_ef_l2_klee.c] exit logic_bomb 2\n");
    }else{
        fprintf(stderr, "[sin_ef_l2_klee.c] enter logic_bomb 3\n");
        fprintf(stderr, "Path without the bomb\n");
        return 0;
        // fprintf(stderr, "[sin_ef_l2_klee.c] exit logic_bomb 3\n");
    }
}

int main(int argc, char **argv) {
    fprintf(stderr, "[sin_ef_l2_klee.c] enter main 1\n");
    if (argc < 2) {
        fprintf(stderr, "[sin_ef_l2_klee.c] enter main 2\n");
        fprintf(stderr, "Usage: %s <input-string>\n", argv[0]);
        return 2;
        // fprintf(stderr, "[sin_ef_l2_klee.c] exit main 2\n");
    }
    char *s = argv[1];
    // fprintf(stderr, "[sin_ef_l2_klee.c] exit main 1\n");

    fprintf(stderr, "[sin_ef_l2_klee.c] enter main 3\n");
    return logic_bomb(s);
    // fprintf(stderr, "[sin_ef_l2_klee.c] exit main 3\n");
}
// Total cost: 0.006935
// Total split cost: 0.000000, input tokens: 0, output tokens: 0, cache read tokens: 0, cache write tokens: 0, split chunks: [(0, 33)]
// Total instrumented cost: 0.006935, input tokens: 2326, output tokens: 368, cache read tokens: 0, cache write tokens: 0
