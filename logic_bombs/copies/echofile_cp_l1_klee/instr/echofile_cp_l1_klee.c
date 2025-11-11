#include <stdio.h>
/*
TOY:
*/
#include <string.h> 
#include "utils.h"

#include "a_tester.h"

// {"s":{"length": 4}}
int logic_bomb(char* s) {
    fprintf(stderr, "[echofile_cp_l1_klee.c] enter logic_bomb 1\n");
    int symvar = s[0] - 48;
    int j;
    char file[] = "tmp.covpro";
    char cmd[256];
    sprintf(cmd, "echo %d > %s\n", symvar, file); 
    system(cmd);

    FILE *fp = stdin;
    fp = fopen(file, "r");
    fscanf(fp,"%d",&j);
    fclose(fp);
    remove(file);

    if(j == 7){
        fprintf(stderr, "[echofile_cp_l1_klee.c] enter logic_bomb 2\n");
        fprintf(stderr, "Logic bomb triggered\n");
        return 1;
        // fprintf(stderr, "[echofile_cp_l1_klee.c] exit logic_bomb 2\n");
    } else{
        fprintf(stderr, "[echofile_cp_l1_klee.c] enter logic_bomb 3\n");
        fprintf(stderr, "Path without the bomb\n");
        return 0;
        // fprintf(stderr, "[echofile_cp_l1_klee.c] exit logic_bomb 3\n");
    }
    // fprintf(stderr, "[echofile_cp_l1_klee.c] exit logic_bomb 1\n");
}

int main(int argc, char **argv) {
    fprintf(stderr, "[echofile_cp_l1_klee.c] enter main 1\n");
    if (argc < 2) {
        fprintf(stderr, "[echofile_cp_l1_klee.c] enter main 2\n");
        fprintf(stderr, "Usage: %s <input-string>\n", argv[0]);
        return 2;
        // fprintf(stderr, "[echofile_cp_l1_klee.c] exit main 2\n");
    }
    char *s = argv[1];
    return logic_bomb(s);
    // fprintf(stderr, "[echofile_cp_l1_klee.c] exit main 1\n");
}
// Total cost: 0.007205
// Total split cost: 0.000000, input tokens: 0, output tokens: 0, cache read tokens: 0, cache write tokens: 0, split chunks: [(0, 41)]
// Total instrumented cost: 0.007205, input tokens: 2366, output tokens: 385, cache read tokens: 0, cache write tokens: 0
