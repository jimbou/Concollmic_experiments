#include <string.h> 
#include <stdio.h>
#include "../lib/utils.c"
#include "../include/a_tester.h"

int n = 5;

// {"symvar":{"length": 256}}
int logic_bomb(char* symvar) {
    fprintf(stderr, "[test.c] enter logic_bomb 1\n");
    char *p, *q;
    p = (char*)malloc(16);
    q = (char*)malloc(16);
    strcpy(p, symvar);
    free(q);
    if (n != 5){
        fprintf(stderr, "[test.c] enter logic_bomb 2\n");
        free(p);
        return BOMB_ENDING;
        // fprintf(stderr, "[test.c] exit logic_bomb 2\n");
    }else {
        fprintf(stderr, "[test.c] enter logic_bomb 3\n");
        free(p);
        return NORMAL_ENDING;
        // fprintf(stderr, "[test.c] exit logic_bomb 3\n");
    }
    // fprintf(stderr, "[test.c] exit logic_bomb 1\n");
}

int main(int argc, char** argv) {
    fprintf(stderr, "[test.c] enter main 1\n");
    char* input = argv[1];
    int result = logic_bomb(input);
    if (result == BOMB_ENDING) {
        fprintf(stderr, "[test.c] enter main 2\n");
        printf("Bomb triggered!\n");
        // fprintf(stderr, "[test.c] exit main 2\n");
    } else {
        fprintf(stderr, "[test.c] enter main 3\n");
        printf("Normal execution.\n");
        // fprintf(stderr, "[test.c] exit main 3\n");
    }
    return 0;
    // fprintf(stderr, "[test.c] exit main 1\n");
}   
// Total cost: 0.000868
// Total split cost: 0.000000, input tokens: 0, output tokens: 0, cache read tokens: 0, cache write tokens: 0, split chunks: [(0, 33)]
// Total instrumented cost: 0.000868, input tokens: 2289, output tokens: 354, cache read tokens: 0, cache write tokens: 0
