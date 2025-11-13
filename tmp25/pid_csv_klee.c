#include <stdio.h>
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "a_tester.h"

// {"s":{"length": 4}}
int logic_bomb(char* s) {
   fprintf(stderr, "[pid_csv_klee.c] enter logic_bomb 1\n");
   int symvar = s[0] - 48;
   int pid = (int) getpid();
   // fprintf(stderr, "[pid_csv_klee.c] exit logic_bomb 1\n");
   if(pid%78 == symvar){
        fprintf(stderr, "[pid_csv_klee.c] enter logic_bomb 2\n");
        fprintf(stderr, "Logic bomb triggered\n");
        return 1;
        // fprintf(stderr, "[pid_csv_klee.c] exit logic_bomb 2\n");
   } else {
        fprintf(stderr, "[pid_csv_klee.c] enter logic_bomb 3\n");
        fprintf(stderr, "Path without the bomb\n");
        return 0;
        // fprintf(stderr, "[pid_csv_klee.c] exit logic_bomb 3\n");
    }
}

int main(int argc, char **argv) {
    fprintf(stderr, "\n");
    if (argc < 2) {
        fprintf(stderr, "[pid_csv_klee.c] enter main 2\n");
        fprintf(stderr, "Usage: %s <input-string>\n", argv[0]);
        return 2;
        // fprintf(stderr, "[pid_csv_klee.c] exit main 2\n");
    }
    fprintf(stderr, "[pid_csv_klee.c] enter main 3\n");
    char *s = argv[1];
    return logic_bomb(s);
    // fprintf(stderr, "[pid_csv_klee.c] exit main 3\n");
}
// Total cost: 0.001024
// Total split cost: 0.000000, input tokens: 0, output tokens: 0, cache read tokens: 0, cache write tokens: 0, split chunks: [(0, 29)]
// Total instrumented cost: 0.001024, input tokens: 2346, output tokens: 355, cache read tokens: 0, cache write tokens: 0
