#include <stdio.h>
#include <string.h> 
#include "utils.h"

#include "a_tester.h"

// {"s":{"length": 16}}
int logic_bomb(char* s) {
    fprintf(stderr, "[file_csv_klee.c] enter logic_bomb 1\n");
    int trigger = 0;
    FILE *fp = fopen(s, "r");
    if(fp != NULL) {
        fprintf(stderr, "[file_csv_klee.c] enter logic_bomb 2\n");
        trigger = 1;
        fclose(fp);
        // fprintf(stderr, "[file_csv_klee.c] exit logic_bomb 2\n");
    }

    if(trigger) {
        fprintf(stderr, "[file_csv_klee.c] enter logic_bomb 3\n");
        fprintf(stderr, "Logic bomb triggered\n");
        return 1;
        // fprintf(stderr, "[file_csv_klee.c] exit logic_bomb 3\n");
    } else {
        fprintf(stderr, "[file_csv_klee.c] enter logic_bomb 4\n");
        fprintf(stderr, "Path without the bomb\n");
        return 0;
        // fprintf(stderr, "[file_csv_klee.c] exit logic_bomb 4\n");
    }
    // fprintf(stderr, "[file_csv_klee.c] exit logic_bomb 1\n");
}

int main(int argc, char **argv) {
    fprintf(stderr, "[file_csv_klee.c] enter main 1\n");
    if (argc < 2) {
        fprintf(stderr, "[file_csv_klee.c] enter main 2\n");
        fprintf(stderr, "Usage: %s <input-string>\n", argv[0]);
        return 2;
        // fprintf(stderr, "[file_csv_klee.c] exit main 2\n");
    }
    char *s = argv[1];
    return logic_bomb(s);
    // fprintf(stderr, "[file_csv_klee.c] exit main 1\n");
}
// Total cost: 0.006703
// Total split cost: 0.000000, input tokens: 0, output tokens: 0, cache read tokens: 0, cache write tokens: 0, split chunks: [(0, 32)]
// Total instrumented cost: 0.006703, input tokens: 2297, output tokens: 352, cache read tokens: 0, cache write tokens: 0
