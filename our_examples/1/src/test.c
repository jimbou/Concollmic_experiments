#include <string.h> 
#include "../lib/utils.c"
#include "../include/a_tester.h"

int n = 5;

// {"symvar":{"length": 256}}
int logic_bomb(char* symvar) {
    char *p, *q;
    p = (char*)malloc(16);
    q = (char*)malloc(16);
    strcpy(p, symvar);
    free(q);
    if (n != 5){
        free(p);
        return BOMB_ENDING;
    }else {
        free(p);
        return NORMAL_ENDING;
    }
}

int main(int argc, char** argv) {
    
    char* input = argv[1];
    int result = logic_bomb(input);
    if (result == BOMB_ENDING) {
        printf("Bomb triggered!\n");
    } else {
        printf("Normal execution.\n");
    }
    return 0;
}   
