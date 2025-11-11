#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>
#include "a_tester.h"

void* Inc(void* i){
    fprintf(stderr, "[2thread_pp_l1_klee.c] enter Inc 1\n");
    ++ *((int*) i);
    // fprintf(stderr, "[2thread_pp_l1_klee.c] exit Inc 1\n");
}

void* Mult(void* i){
    fprintf(stderr, "[2thread_pp_l1_klee.c] enter Mult 1\n");
    *((int*) i) =  *((int*) i) * *((int*) i);
    // fprintf(stderr, "[2thread_pp_l1_klee.c] exit Mult 1\n");
}

int ThreadProp(int in){
    fprintf(stderr, "[2thread_pp_l1_klee.c] enter ThreadProp 1\n");
    pthread_t tid[2];
    int rc1 = pthread_create(&tid[0], NULL, Inc, (void *) &in); 
    int rc2 = pthread_create(&tid[1], NULL, Mult, (void *) &in); 
    rc1 = pthread_join(tid[0], NULL); 
    rc2 = pthread_join(tid[1], NULL); 
    int out = in;
    return out;
    // fprintf(stderr, "[2thread_pp_l1_klee.c] exit ThreadProp 1\n");
}

// {"s":{"length": 4}}
int logic_bomb(char* s) {
    fprintf(stderr, "[2thread_pp_l1_klee.c] enter logic_bomb 1\n");
    int symvar = s[0] - 48;
    int i=ThreadProp(symvar);
    // fprintf(stderr, "[2thread_pp_l1_klee.c] exit logic_bomb 1\n");

    if(i == 50){
        fprintf(stderr, "[2thread_pp_l1_klee.c] enter logic_bomb 2\n");
        fprintf(stderr, "Logic bomb triggered\n");
        return 1;
        // fprintf(stderr, "[2thread_pp_l1_klee.c] exit logic_bomb 2\n");
    }

    fprintf(stderr, "[2thread_pp_l1_klee.c] enter logic_bomb 3\n");
    fprintf(stderr, "Path without the bomb\n");
    return 0;
    // fprintf(stderr, "[2thread_pp_l1_klee.c] exit logic_bomb 3\n");
}



int main(int argc, char **argv) {
    fprintf(stderr, "[2thread_pp_l1_klee.c] enter main 1\n");
    if (argc < 2) {
        fprintf(stderr, "[2thread_pp_l1_klee.c] enter main 2\n");
        fprintf(stderr, "Usage: %s <input-string>\n", argv[0]);
        return 2;
        // fprintf(stderr, "[2thread_pp_l1_klee.c] exit main 2\n");
    }
    // fprintf(stderr, "[2thread_pp_l1_klee.c] exit main 1\n");

    fprintf(stderr, "[2thread_pp_l1_klee.c] enter main 3\n");
    char *s = argv[1];
    return logic_bomb(s);
    // fprintf(stderr, "[2thread_pp_l1_klee.c] exit main 3\n");
}
// Total cost: 0.008965
// Total split cost: 0.000000, input tokens: 0, output tokens: 0, cache read tokens: 0, cache write tokens: 0, split chunks: [(0, 46)]
// Total instrumented cost: 0.008965, input tokens: 2426, output tokens: 546, cache read tokens: 0, cache write tokens: 0
