#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>
#include "a_tester.h"

void* Inc(void* i){
    fprintf(stderr, "[2thread_pp_l2_klee.c] enter Inc 1\n");
    int count = 0;
    while (*((int *) i) > -1000 && count++ < 1000){
        fprintf(stderr, "[2thread_pp_l2_klee.c] enter Inc 2\n");
        ++ *((int *) i);
        //printf("%d\n", *((int*) i));
        // fprintf(stderr, "[2thread_pp_l2_klee.c] exit Inc 2\n");
    }
    // fprintf(stderr, "[2thread_pp_l2_klee.c] exit Inc 1\n");
}

void* Dec(void* i){
    fprintf(stderr, "[2thread_pp_l2_klee.c] enter Dec 1\n");
    int count = 0;
    while (*((int *) i) <  1000 && count++ < 1000){
        fprintf(stderr, "[2thread_pp_l2_klee.c] enter Dec 2\n");
        -- *((int *) i);
        //printf("%d\n", *((int *) i));
        // fprintf(stderr, "[2thread_pp_l2_klee.c] exit Dec 2\n");
    }
    // fprintf(stderr, "[2thread_pp_l2_klee.c] exit Dec 1\n");
}

int ThreadProp(int in){
    fprintf(stderr, "[2thread_pp_l2_klee.c] enter ThreadProp 1\n");
    pthread_t tid[2];
    pthread_create(&tid[0], NULL, Inc, (void *) &in); 
    pthread_create(&tid[1], NULL, Dec, (void *) &in); 
    pthread_join(tid[0], NULL); 
    pthread_join(tid[1], NULL); 
    return in;
    // fprintf(stderr, "[2thread_pp_l2_klee.c] exit ThreadProp 1\n");
}

// {"s":{"length": 4}}
int logic_bomb(char* s) {
    fprintf(stderr, "[2thread_pp_l2_klee.c] enter logic_bomb 1\n");
    int symvar = s[0] - 48;
    int i=ThreadProp(symvar-909);
    //printf("%d\n",i);
    // fprintf(stderr, "[2thread_pp_l2_klee.c] exit logic_bomb 1\n");

    if(i == -1900){
        fprintf(stderr, "[2thread_pp_l2_klee.c] enter logic_bomb 2\n");
        fprintf(stderr, "Logic bomb triggered\n");
        return 1;
        // fprintf(stderr, "[2thread_pp_l2_klee.c] exit logic_bomb 2\n");
    }
    fprintf(stderr, "[2thread_pp_l2_klee.c] enter logic_bomb 3\n");
    fprintf(stderr, "Path without the bomb\n");
    return 0;
    // fprintf(stderr, "[2thread_pp_l2_klee.c] exit logic_bomb 3\n");
}

int main(int argc, char **argv) {
    fprintf(stderr, "[2thread_pp_l2_klee.c] enter main 1\n");
    if (argc < 2) {
        fprintf(stderr, "[2thread_pp_l2_klee.c] enter main 2\n");
        fprintf(stderr, "Usage: %s <input-string>\n", argv[0]);
        return 2;
        // fprintf(stderr, "[2thread_pp_l2_klee.c] exit main 2\n");
    }
    char *s = argv[1];
    return logic_bomb(s);
    // fprintf(stderr, "[2thread_pp_l2_klee.c] exit main 1\n");
}
// Total cost: 0.010000
// Total split cost: 0.000000, input tokens: 0, output tokens: 0, cache read tokens: 0, cache write tokens: 0, split chunks: [(0, 51)]
// Total instrumented cost: 0.010000, input tokens: 2496, output tokens: 632, cache read tokens: 0, cache write tokens: 0
