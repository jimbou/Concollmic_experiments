#include <stdio.h>
/*
TOY:
*/
#include <string.h>
#include "utils.h"

#include "a_tester.h"

// {"s":{"length": 16}}
int logic_bomb(char* s) {
    fprintf(stderr, "[syscall_csv_klee.c] enter logic_bomb 1\n");
    if(s == NULL) {
        fprintf(stderr, "[syscall_csv_klee.c] enter logic_bomb 2\n");
        fprintf(stderr, "Path without the bomb\n");
        return 0;
        // fprintf(stderr, "[syscall_csv_klee.c] exit logic_bomb 2\n");
    }
    // fprintf(stderr, "[syscall_csv_klee.c] exit logic_bomb 1\n");

    fprintf(stderr, "[syscall_csv_klee.c] enter logic_bomb 3\n");
    if(s[0]=='\0') {
        fprintf(stderr, "[syscall_csv_klee.c] enter logic_bomb 4\n");
        fprintf(stderr, "Path without the bomb\n");
        return 0;
        // fprintf(stderr, "[syscall_csv_klee.c] exit logic_bomb 4\n");
    }
    // fprintf(stderr, "[syscall_csv_klee.c] exit logic_bomb 3\n");

    fprintf(stderr, "[syscall_csv_klee.c] enter logic_bomb 5\n");
    int trigger = -1;
    trigger = system(s);
    if(trigger == 0) {
        fprintf(stderr, "[syscall_csv_klee.c] enter logic_bomb 6\n");
        fprintf(stderr, "Logic bomb triggered\n");
        return 1;
        // fprintf(stderr, "[syscall_csv_klee.c] exit logic_bomb 6\n");
    } else {
        fprintf(stderr, "[syscall_csv_klee.c] enter logic_bomb 7\n");
        fprintf(stderr, "Path without the bomb\n");
        return 0;
        // fprintf(stderr, "[syscall_csv_klee.c] exit logic_bomb 7\n");
    }
    // fprintf(stderr, "[syscall_csv_klee.c] exit logic_bomb 5\n");
}

int main(int argc, char **argv) {
    fprintf(stderr, "[syscall_csv_klee.c] enter main 1\n");
    if (argc < 2) {
        fprintf(stderr, "[syscall_csv_klee.c] enter main 2\n");
        fprintf(stderr, "Usage: %s <input-string>\n", argv[0]);
        return 2;
        // fprintf(stderr, "[syscall_csv_klee.c] exit main 2\n");
    }
    // fprintf(stderr, "[syscall_csv_klee.c] exit main 1\n");

    fprintf(stderr, "[syscall_csv_klee.c] enter main 3\n");
    char *s = argv[1];
    return logic_bomb(s);
    // fprintf(stderr, "[syscall_csv_klee.c] exit main 3\n");
}
// Total cost: 0.008213
// Total split cost: 0.000000, input tokens: 0, output tokens: 0, cache read tokens: 0, cache write tokens: 0, split chunks: [(0, 38)]
// Total instrumented cost: 0.008213, input tokens: 2337, output tokens: 493, cache read tokens: 0, cache write tokens: 0
