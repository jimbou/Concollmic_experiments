#include <stdio.h>
/*
TOY:
Result:
Triton: Pin is out of memory: MmapChecked

*/
#include <string.h> 
#include "utils.h"
#include "a_tester.h"

// {"s":{"length": 4}}
int logic_bomb(char* s) {
    fprintf(stderr, "[file_cp_l1_klee.c] enter logic_bomb 1\n");
    int symvar = s[0] - 48;
    int j;
    char file[] = "tmp.covpro";
    FILE *fp = fopen(file, "ab+");
    if(fp == NULL)
    {
        fprintf(stderr, "[file_cp_l1_klee.c] enter logic_bomb 2\n");
        //printf("Error!");   
        exit(1);             
        // fprintf(stderr, "[file_cp_l1_klee.c] exit logic_bomb 2\n");
    }
    fprintf(stderr, "[file_cp_l1_klee.c] enter logic_bomb 3\n");
    fprintf(fp,"%d",symvar);
    fclose(fp);
    // fprintf(stderr, "[file_cp_l1_klee.c] exit logic_bomb 3\n");

    fprintf(stderr, "[file_cp_l1_klee.c] enter logic_bomb 4\n");
    fp = fopen("tmp.covpro", "r");
    fscanf(fp,"%d",&j);
    fclose(fp);
    remove(file);
    // fprintf(stderr, "[file_cp_l1_klee.c] exit logic_bomb 4\n");

    if(j == 7){
        fprintf(stderr, "[file_cp_l1_klee.c] enter logic_bomb 5\n");
        fprintf(stderr, "Logic bomb triggered\n");
        return 1;
        // fprintf(stderr, "[file_cp_l1_klee.c] exit logic_bomb 5\n");
    } else{
        fprintf(stderr, "[file_cp_l1_klee.c] enter logic_bomb 6\n");
        fprintf(stderr, "Path without the bomb\n");
        return 0;
        // fprintf(stderr, "[file_cp_l1_klee.c] exit logic_bomb 6\n");
    }
    // fprintf(stderr, "[file_cp_l1_klee.c] exit logic_bomb 1\n");
}

int main(int argc, char **argv) {
    fprintf(stderr, "\n");
    if (argc < 2) {
        fprintf(stderr, "[file_cp_l1_klee.c] enter main 2\n");
        fprintf(stderr, "Usage: %s <input-string>\n", argv[0]);
        return 2;
        // fprintf(stderr, "[file_cp_l1_klee.c] exit main 2\n");
    }
    fprintf(stderr, "[file_cp_l1_klee.c] enter main 3\n");
    char *s = argv[1];
    return logic_bomb(s);
    // fprintf(stderr, "[file_cp_l1_klee.c] exit main 3\n");
}
// Total cost: 0.008528
// Total split cost: 0.000000, input tokens: 0, output tokens: 0, cache read tokens: 0, cache write tokens: 0, split chunks: [(0, 46)]
// Total instrumented cost: 0.008528, input tokens: 2395, output tokens: 510, cache read tokens: 0, cache write tokens: 0
