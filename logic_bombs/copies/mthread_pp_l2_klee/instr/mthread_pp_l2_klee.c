#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>
#include "a_tester.h"

void* Inc(void* i){
    fprintf(stderr, "[mthread_pp_l2_klee.c] enter Inc 1\n");
    int count = 0;
    while (*((int *) i) > -1000 && count++ < 1000){
        fprintf(stderr, "[mthread_pp_l2_klee.c] enter Inc 2\n");
        ++ *((int*) i);
        // fprintf(stderr, "[mthread_pp_l2_klee.c] exit Inc 2\n");
    }
    // fprintf(stderr, "[mthread_pp_l2_klee.c] exit Inc 1\n");
}

void* Dec(void* i){
    fprintf(stderr, "[mthread_pp_l2_klee.c] enter Dec 1\n");
    int count = 0;
    while (*((int *) i) < 1000 && count++ < 1000){
        fprintf(stderr, "[mthread_pp_l2_klee.c] enter Dec 2\n");
        -- *((int*) i);
        // fprintf(stderr, "[mthread_pp_l2_klee.c] exit Dec 2\n");
    }
    // fprintf(stderr, "[mthread_pp_l2_klee.c] exit Dec 1\n");
}

int ThreadProp(int in){
    fprintf(stderr, "[mthread_pp_l2_klee.c] enter ThreadProp 1\n");
    pthread_t tid[10];
    pthread_create(&tid[0], NULL, Inc, (void *) &in); 
    pthread_create(&tid[1], NULL, Dec, (void *) &in); 
    pthread_create(&tid[2], NULL, Inc, (void *) &in); 
    pthread_create(&tid[3], NULL, Dec, (void *) &in); 
    pthread_create(&tid[4], NULL, Inc, (void *) &in); 
    pthread_create(&tid[5], NULL, Dec, (void *) &in); 
    pthread_create(&tid[6], NULL, Inc, (void *) &in); 
    pthread_create(&tid[7], NULL, Dec, (void *) &in); 
    pthread_create(&tid[8], NULL, Inc, (void *) &in); 
    pthread_create(&tid[9], NULL, Dec, (void *) &in); 
    pthread_join(tid[0], NULL); 
    pthread_join(tid[1], NULL); 
    pthread_join(tid[2], NULL); 
    pthread_join(tid[3], NULL); 
    pthread_join(tid[4], NULL); 
    pthread_join(tid[5], NULL); 
    pthread_join(tid[6], NULL); 
    pthread_join(tid[7], NULL); 
    pthread_join(tid[8], NULL); 
    pthread_join(tid[9], NULL); 
    return in;
    // fprintf(stderr, "[mthread_pp_l2_klee.c] exit ThreadProp 1\n");
}

// {"s":{"length": 4}}
int logic_bomb(char* s) {
    fprintf(stderr, "[mthread_pp_l2_klee.c] enter logic_bomb 1\n");
    int symvar = s[0] - 48;
    int i=ThreadProp(symvar+990);
    //printf("%d\n",i);
    if(i == 5999){
        fprintf(stderr, "[mthread_pp_l2_klee.c] enter logic_bomb 2\n");
        fprintf(stderr, "Logic bomb triggered\n");
        return 1;
        // fprintf(stderr, "[mthread_pp_l2_klee.c] exit logic_bomb 2\n");
    }
    fprintf(stderr, "[mthread_pp_l2_klee.c] enter logic_bomb 3\n");
    fprintf(stderr, "Path without the bomb\n");
    return 0;
    // fprintf(stderr, "[mthread_pp_l2_klee.c] exit logic_bomb 3\n");
    // fprintf(stderr, "[mthread_pp_l2_klee.c] exit logic_bomb 1\n");
}

int main(int argc, char **argv) {
    fprintf(stderr, "[mthread_pp_l2_klee.c] enter main 1\n");
    if (argc < 2) {
        fprintf(stderr, "[mthread_pp_l2_klee.c] enter main 2\n");
        fprintf(stderr, "Usage: %s <input-string>\n", argv[0]);
        return 2;
        // fprintf(stderr, "[mthread_pp_l2_klee.c] exit main 2\n");
    }
    fprintf(stderr, "[mthread_pp_l2_klee.c] enter main 3\n");
    char *s = argv[1];
    return logic_bomb(s);
    // fprintf(stderr, "[mthread_pp_l2_klee.c] exit main 3\n");
    // fprintf(stderr, "[mthread_pp_l2_klee.c] exit main 1\n");
}
// Total cost: 0.012913
// Total split cost: 0.000000, input tokens: 0, output tokens: 0, cache read tokens: 0, cache write tokens: 0, split chunks: [(0, 66)]
// Total instrumented cost: 0.012913, input tokens: 2709, output tokens: 870, cache read tokens: 0, cache write tokens: 0
