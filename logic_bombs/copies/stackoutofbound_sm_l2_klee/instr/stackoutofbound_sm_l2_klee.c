#include <stdio.h>
#include <stdio.h>
#include "a_tester.h"

// {"s":{"length": 4}}
int logic_bomb(char* s) {
    fprintf(stderr, "[stackoutofbound_sm_l2_klee.c] enter logic_bomb 1\n");
    int symvar = s[0] - 48;
    int a[] = {1, 2, 3, 4, 5, 6};
    if (a[symvar]<0 || a[symvar] > 6){
        fprintf(stderr, "[stackoutofbound_sm_l2_klee.c] enter logic_bomb 2\n");
        fprintf(stderr, "Logic bomb triggered\n");
        return 1;
        // fprintf(stderr, "[stackoutofbound_sm_l2_klee.c] exit logic_bomb 2\n");
    }
    fprintf(stderr, "[stackoutofbound_sm_l2_klee.c] enter logic_bomb 3\n");
    fprintf(stderr, "Path without the bomb\n");
    return 0;
    // fprintf(stderr, "[stackoutofbound_sm_l2_klee.c] exit logic_bomb 3\n");
    // fprintf(stderr, "[stackoutofbound_sm_l2_klee.c] exit logic_bomb 1\n");
}

int main(int argc, char **argv) {
    fprintf(stderr, "[stackoutofbound_sm_l2_klee.c] enter main 1\n");
    if (argc < 2) {
        fprintf(stderr, "[stackoutofbound_sm_l2_klee.c] enter main 2\n");
        fprintf(stderr, "Usage: %s <input-string>\n", argv[0]);
        return 2;
        // fprintf(stderr, "[stackoutofbound_sm_l2_klee.c] exit main 2\n");
    }
    fprintf(stderr, "[stackoutofbound_sm_l2_klee.c] enter main 3\n");
    char *s = argv[1];
    return logic_bomb(s);
    // fprintf(stderr, "[stackoutofbound_sm_l2_klee.c] exit main 3\n");
    // fprintf(stderr, "[stackoutofbound_sm_l2_klee.c] exit main 1\n");
}
// Total cost: 0.006733
// Total split cost: 0.000000, input tokens: 0, output tokens: 0, cache read tokens: 0, cache write tokens: 0, split chunks: [(0, 24)]
// Total instrumented cost: 0.006733, input tokens: 2313, output tokens: 351, cache read tokens: 0, cache write tokens: 0
