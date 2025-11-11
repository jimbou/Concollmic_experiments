#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>
#include "a_tester.h"

void* trigger(void* i){
    fprintf(stderr, "[paraloop_lo_l2_klee.c] enter trigger 1\n");
    sleep(5);
    ++ *((int*) i);
    // fprintf(stderr, "[paraloop_lo_l2_klee.c] exit trigger 1\n");
}

// {"s":{"length": 4}}
int logic_bomb(char* s) {
    fprintf(stderr, "[paraloop_lo_l2_klee.c] enter logic_bomb 1\n");
    int symvar = s[0] - 48;
    int i = symvar + 1;
    pthread_t tid;
    int rc = pthread_create(&tid, NULL, trigger, (void *) &symvar); 
    // fprintf(stderr, "[paraloop_lo_l2_klee.c] exit logic_bomb 1\n");

    while (symvar != i){
        fprintf(stderr, "[paraloop_lo_l2_klee.c] enter logic_bomb 2\n");
        sleep(1);
        symvar ++;
        i ++;
        // fprintf(stderr, "[paraloop_lo_l2_klee.c] exit logic_bomb 2\n");
    }

    fprintf(stderr, "[paraloop_lo_l2_klee.c] enter logic_bomb 3\n");
    rc = pthread_join(tid, NULL); 
    // fprintf(stderr, "[paraloop_lo_l2_klee.c] exit logic_bomb 3\n");

    if(symvar == 13){
        fprintf(stderr, "[paraloop_lo_l2_klee.c] enter logic_bomb 4\n");
        fprintf(stderr, "Logic bomb triggered\n");
        return 1;
        // fprintf(stderr, "[paraloop_lo_l2_klee.c] exit logic_bomb 4\n");
    }

    fprintf(stderr, "[paraloop_lo_l2_klee.c] enter logic_bomb 5\n");
    fprintf(stderr, "Path without the bomb\n");
    return 0;
    // fprintf(stderr, "[paraloop_lo_l2_klee.c] exit logic_bomb 5\n");
}

int main(int argc, char **argv) {
    fprintf(stderr, "[paraloop_lo_l2_klee.c] enter main 1\n");
    if (argc < 2) {
        fprintf(stderr, "[paraloop_lo_l2_klee.c] enter main 2\n");
        fprintf(stderr, "Usage: %s <input-string>\n", argv[0]);
        return 2;
        // fprintf(stderr, "[paraloop_lo_l2_klee.c] exit main 2\n");
    }
    char *s = argv[1];
    // fprintf(stderr, "[paraloop_lo_l2_klee.c] exit main 1\n");

    fprintf(stderr, "[paraloop_lo_l2_klee.c] enter main 3\n");
    return logic_bomb(s);
    // fprintf(stderr, "[paraloop_lo_l2_klee.c] exit main 3\n");
}
// Total cost: 0.008303
// Total split cost: 0.000000, input tokens: 0, output tokens: 0, cache read tokens: 0, cache write tokens: 0, split chunks: [(0, 39)]
// Total instrumented cost: 0.008303, input tokens: 2373, output tokens: 493, cache read tokens: 0, cache write tokens: 0
