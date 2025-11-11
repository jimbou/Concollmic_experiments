#include <stdio.h>
#include <unistd.h>
#include "utils.h"
#include "a_tester.h"

// {"s":{"length": 2}}
int logic_bomb(char* s) {
    fprintf(stderr, "[forkpipe_pp_l1_klee.c] enter logic_bomb 1\n");
    int pid, fd[2];
    pipe(fd);
    if ((pid = fork()) == -1) {
        fprintf(stderr, "[forkpipe_pp_l1_klee.c] enter logic_bomb 2\n");
        fprintf(stderr, "Path without the bomb\n");
        // fprintf(stderr, "[forkpipe_pp_l1_klee.c] exit logic_bomb 2\n");
        return 0;
    }
    // fprintf(stderr, "[forkpipe_pp_l1_klee.c] exit logic_bomb 1\n");

    if (pid == 0) {
        fprintf(stderr, "[forkpipe_pp_l1_klee.c] enter logic_bomb 3\n");
        close(fd[0]);
        write(fd[1], s, sizeof(s));
        wait(NULL);
        exit(0);
        // fprintf(stderr, "[forkpipe_pp_l1_klee.c] exit logic_bomb 3\n");
    }
    else {
        fprintf(stderr, "[forkpipe_pp_l1_klee.c] enter logic_bomb 4\n");
        char content[8];
        close(fd[1]);
        read(fd[0], content, 8);
        if (strcmp(content, "7") == 0) {
            fprintf(stderr, "[forkpipe_pp_l1_klee.c] enter logic_bomb 5\n");
            fprintf(stderr, "Logic bomb triggered\n");
            // fprintf(stderr, "[forkpipe_pp_l1_klee.c] exit logic_bomb 5\n");
            return 1;
        }
        fprintf(stderr, "[forkpipe_pp_l1_klee.c] enter logic_bomb 6\n");
        fprintf(stderr, "Path without the bomb\n");
        // fprintf(stderr, "[forkpipe_pp_l1_klee.c] exit logic_bomb 6\n");
        return 0; 
        // fprintf(stderr, "[forkpipe_pp_l1_klee.c] exit logic_bomb 4\n");
    }
}

int main(int argc, char **argv) {
    fprintf(stderr, "[forkpipe_pp_l1_klee.c] enter main 1\n");
    if (argc < 2) {
        fprintf(stderr, "[forkpipe_pp_l1_klee.c] enter main 2\n");
        fprintf(stderr, "Usage: %s <input-string>\n", argv[0]);
        // fprintf(stderr, "[forkpipe_pp_l1_klee.c] exit main 2\n");
        return 2;
    }
    char *s = argv[1];
    // fprintf(stderr, "[forkpipe_pp_l1_klee.c] exit main 1\n");
    fprintf(stderr, "[forkpipe_pp_l1_klee.c] enter main 3\n");
    return logic_bomb(s);
    // fprintf(stderr, "[forkpipe_pp_l1_klee.c] exit main 3\n");
}
// Total cost: 0.008420
// Total split cost: 0.000000, input tokens: 0, output tokens: 0, cache read tokens: 0, cache write tokens: 0, split chunks: [(0, 39)]
// Total instrumented cost: 0.008420, input tokens: 2376, output tokens: 504, cache read tokens: 0, cache write tokens: 0
