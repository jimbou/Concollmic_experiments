#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "utils.h"
#include "a_tester.h"

// {"s":{"length": 2}}
int logic_bomb(char* symvar) {
    fprintf(stderr, "[forkshm_pp_l1_klee.c] enter logic_bomb 1\n");
    char *p_addr, *c_addr;
    int flag = 0;
    key_t shmid = shmget(IPC_PRIVATE, 1024, S_IRUSR|S_IWUSR);
    if (shmid < 0) {
        fprintf(stderr, "[forkshm_pp_l1_klee.c] enter logic_bomb 2\n");
        fprintf(stderr, "Path without the bomb\n");
        // fprintf(stderr, "[forkshm_pp_l1_klee.c] exit logic_bomb 2\n");
        return 0;
    }
    // fprintf(stderr, "[forkshm_pp_l1_klee.c] exit logic_bomb 1\n");

    fprintf(stderr, "[forkshm_pp_l1_klee.c] enter logic_bomb 3\n");
    pid_t pid = fork();
    if(pid == 0){
        fprintf(stderr, "[forkshm_pp_l1_klee.c] enter logic_bomb 4\n");
        p_addr = shmat(shmid,0,0);
        memset(p_addr,'/0',1024);
        strncpy(p_addr, symvar, 1024);
        exit(0);
        // fprintf(stderr, "[forkshm_pp_l1_klee.c] exit logic_bomb 4\n");
    }
    // fprintf(stderr, "[forkshm_pp_l1_klee.c] exit logic_bomb 3\n");

    if (pid > 0){
        fprintf(stderr, "[forkshm_pp_l1_klee.c] enter logic_bomb 5\n");
        sleep(1);
        c_addr = shmat(shmid,0,0);
        if(strcmp(c_addr, "7") == 0) {
            fprintf(stderr, "[forkshm_pp_l1_klee.c] enter logic_bomb 6\n");
            flag = 1;
            // fprintf(stderr, "[forkshm_pp_l1_klee.c] exit logic_bomb 6\n");
        }
        shmctl(shmid,IPC_RMID,0);
        if(flag == 1){
            fprintf(stderr, "[forkshm_pp_l1_klee.c] enter logic_bomb 7\n");
            fprintf(stderr, "Logic bomb triggered\n");
            return 1;
            // fprintf(stderr, "[forkshm_pp_l1_klee.c] exit logic_bomb 7\n");
        }
        fprintf(stderr, "[forkshm_pp_l1_klee.c] enter logic_bomb 8\n");
        fprintf(stderr, "Path without the bomb\n");
        return 0;
        // fprintf(stderr, "[forkshm_pp_l1_klee.c] exit logic_bomb 8\n");
        // fprintf(stderr, "[forkshm_pp_l1_klee.c] exit logic_bomb 5\n");
    }
}

int main(int argc, char **argv) {
    fprintf(stderr, "[forkshm_pp_l1_klee.c] enter main 1\n");
    if (argc < 2) {
        fprintf(stderr, "[forkshm_pp_l1_klee.c] enter main 2\n");
        fprintf(stderr, "Usage: %s <input-string>\n", argv[0]);
        return 2;
        // fprintf(stderr, "[forkshm_pp_l1_klee.c] exit main 2\n");
    }
    // fprintf(stderr, "[forkshm_pp_l1_klee.c] exit main 1\n");

    fprintf(stderr, "[forkshm_pp_l1_klee.c] enter main 3\n");
    char *s = argv[1];
    return logic_bomb(s);
    // fprintf(stderr, "[forkshm_pp_l1_klee.c] exit main 3\n");
}
// Total cost: 0.010398
// Total split cost: 0.000000, input tokens: 0, output tokens: 0, cache read tokens: 0, cache write tokens: 0, split chunks: [(0, 49)]
// Total instrumented cost: 0.010398, input tokens: 2487, output tokens: 674, cache read tokens: 0, cache write tokens: 0
