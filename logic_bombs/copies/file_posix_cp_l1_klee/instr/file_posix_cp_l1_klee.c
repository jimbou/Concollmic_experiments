#include <stdio.h>
#include <string.h>

#include "utils.h"
#include "a_tester.h"

// {"s": {"length": 4}}
int logic_bomb(char* s) {
    fprintf(stderr, "[file_posix_cp_l1_klee.c] enter logic_bomb 1\n");
    int symvar = s[0] - 48;
    int j;
    char file[] = "tmp.covpro";
    int fd = open(file, O_CREAT | O_WRONLY | O_TRUNC, S_IRUSR | S_IWUSR);
    if(fd < 0)
    {
        fprintf(stderr, "[file_posix_cp_l1_klee.c] enter logic_bomb 2\n");
        exit(-1);             
        // fprintf(stderr, "[file_posix_cp_l1_klee.c] exit logic_bomb 2\n");
    }
    fprintf(stderr, "[file_posix_cp_l1_klee.c] enter logic_bomb 3\n");
    write(fd, &symvar, sizeof symvar);
    close(fd);
    fd = open(file, O_RDONLY);
    read(fd, &j, sizeof j);
    close(fd);
    // fprintf(stderr, "[file_posix_cp_l1_klee.c] exit logic_bomb 3\n");

    if(j == 7){
        fprintf(stderr, "[file_posix_cp_l1_klee.c] enter logic_bomb 4\n");
        fprintf(stderr, "Logic bomb triggered\n");
        return 1;
        // fprintf(stderr, "[file_posix_cp_l1_klee.c] exit logic_bomb 4\n");
    } else{
        fprintf(stderr, "[file_posix_cp_l1_klee.c] enter logic_bomb 5\n");
        fprintf(stderr, "Path without the bomb\n");
        return 0;
        // fprintf(stderr, "[file_posix_cp_l1_klee.c] exit logic_bomb 5\n");
    }
    // fprintf(stderr, "[file_posix_cp_l1_klee.c] exit logic_bomb 1\n");
}

int main(int argc, char **argv) {
    fprintf(stderr, "[file_posix_cp_l1_klee.c] enter main 1\n");
    if (argc < 2) {
        fprintf(stderr, "[file_posix_cp_l1_klee.c] enter main 2\n");
        fprintf(stderr, "Usage: %s <input-string>\n", argv[0]);
        return 2;
        // fprintf(stderr, "[file_posix_cp_l1_klee.c] exit main 2\n");
    }
    fprintf(stderr, "[file_posix_cp_l1_klee.c] enter main 3\n");
    char *s = argv[1];
    return logic_bomb(s);
    // fprintf(stderr, "[file_posix_cp_l1_klee.c] exit main 3\n");
    // fprintf(stderr, "[file_posix_cp_l1_klee.c] exit main 1\n");
}
// Total cost: 0.010735
// Total split cost: 0.000000, input tokens: 0, output tokens: 0, cache read tokens: 0, cache write tokens: 0, split chunks: [(0, 38)]
// Total instrumented cost: 0.010735, input tokens: 2382, output tokens: 478, cache read tokens: 0, cache write tokens: 0
