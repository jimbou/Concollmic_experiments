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
    int symvar = s[0] - 48;
    int j;
    char file[] = "tmp.covpro";
    FILE *fp = fopen(file, "ab+");
    if(fp == NULL)
    {
        //printf("Error!");   
        exit(1);             
    }
    fprintf(fp,"%d",symvar);
    fclose(fp);

    fp = fopen("tmp.covpro", "r");
    fscanf(fp,"%d",&j);
    fclose(fp);
    remove(file);
    if(j == 7){
        fprintf(stderr, "Logic bomb triggered\n");
        return 1;
    } else{
        fprintf(stderr, "Path without the bomb\n");
        return 0;
    }
}

int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <input-string>\n", argv[0]);
        return 2;
    }
    char *s = argv[1];
    return logic_bomb(s);
}