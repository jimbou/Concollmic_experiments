#include <string.h> 
#include "../lib/utils.c"
#include "../include/a_tester.h"
#include <stdio.h>

int n = 5;

// {"symvar":{"length": 256}}
int logic_bomb(char* symvar) {
    fprintf(stderr, "[test.c] enter logic_bomb 1\n");
    char *p, *q;
    p = (char*)malloc(16);
    q = (char*)malloc(16);
    strcpy(p, symvar);
    free(q);
    // fprintf(stderr, "[test.c] exit logic_bomb 1\n");

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
}

int main(int argc, char** argv) {
    fprintf(stderr, "[test.c] enter main 1\n");
    if (argc != 2) {
        fprintf(stderr, "[test.c] enter main 2\n");
        printf("Usage: %s <input_string>\n", argv[0]);
        return 1;
        // fprintf(stderr, "[test.c] exit main 2\n");
    }
    // fprintf(stderr, "[test.c] exit main 1\n");

    fprintf(stderr, "[test.c] enter main 3\n");
    char* input = argv[1];
    int result = logic_bomb(input);
    // fprintf(stderr, "[test.c] exit main 3\n");

    if (result == BOMB_ENDING) {
        fprintf(stderr, "[test.c] enter main 4\n");
        printf("Bomb triggered!\n");
        // fprintf(stderr, "[test.c] exit main 4\n");
    } else {
        fprintf(stderr, "[test.c] enter main 5\n");
        printf("Normal execution.\n");
        // fprintf(stderr, "[test.c] exit main 5\n");
    }
    fprintf(stderr, "[test.c] enter main 6\n");
    return 0;
    // fprintf(stderr, "[test.c] exit main 6\n");
}   
    
// Total cost: 0.001658
// Total split cost: 0.000000, input tokens: 0, output tokens: 0, cache read tokens: 0, cache write tokens: 0, split chunks: [(0, 37)]
// Total instrumented cost: 0.001658, input tokens: 2320, output tokens: 456, cache read tokens: 0, cache write tokens: 0
