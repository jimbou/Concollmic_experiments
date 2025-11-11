#include <stdio.h>
#include <string.h> 
#include "utils.h"
#include "a_tester.h"


// {"symvar":{"length": 64}}
int logic_bomb(char* symvar) {
    fprintf(stderr, "[stacknocrash_bo_l1_klee.c] enter logic_bomb 1\n");
    int flag = 0;
    char buf[8];
    // fprintf(stderr, "[stacknocrash_bo_l1_klee.c] exit logic_bomb 1\n");

    if(strlen(symvar) > 9) {
        fprintf(stderr, "[stacknocrash_bo_l1_klee.c] enter logic_bomb 2\n");
        fprintf(stderr, "Path without the bomb\n");
        return 0;
        // fprintf(stderr, "[stacknocrash_bo_l1_klee.c] exit logic_bomb 2\n");
    }

    fprintf(stderr, "[stacknocrash_bo_l1_klee.c] enter logic_bomb 3\n");
    strcpy(buf, symvar);
    // fprintf(stderr, "[stacknocrash_bo_l1_klee.c] exit logic_bomb 3\n");

    if(flag == 1) {
        fprintf(stderr, "[stacknocrash_bo_l1_klee.c] enter logic_bomb 4\n");
        fprintf(stderr, "Logic bomb triggered\n");
        return 1;
        // fprintf(stderr, "[stacknocrash_bo_l1_klee.c] exit logic_bomb 4\n");
    }

    fprintf(stderr, "[stacknocrash_bo_l1_klee.c] enter logic_bomb 5\n");
    fprintf(stderr, "Path without the bomb\n");
    return 0;
    // fprintf(stderr, "[stacknocrash_bo_l1_klee.c] exit logic_bomb 5\n");
}

int main(int argc, char **argv) {
    fprintf(stderr, "\n");
    if (argc < 2) {
        fprintf(stderr, "[stacknocrash_bo_l1_klee.c] enter main 2\n");
        fprintf(stderr, "Usage: %s <input-string>\n", argv[0]);
        return 2;
        // fprintf(stderr, "[stacknocrash_bo_l1_klee.c] exit main 2\n");
    }

    fprintf(stderr, "[stacknocrash_bo_l1_klee.c] enter main 3\n");
    char *s = argv[1];
    return logic_bomb(s);
    // fprintf(stderr, "[stacknocrash_bo_l1_klee.c] exit main 3\n");
}
// Total cost: 0.007278
// Total split cost: 0.000000, input tokens: 0, output tokens: 0, cache read tokens: 0, cache write tokens: 0, split chunks: [(0, 30)]
// Total instrumented cost: 0.007278, input tokens: 2319, output tokens: 404, cache read tokens: 0, cache write tokens: 0
