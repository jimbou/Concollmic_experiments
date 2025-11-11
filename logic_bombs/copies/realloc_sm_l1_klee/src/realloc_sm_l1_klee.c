#include <stdio.h>
#include<stdio.h>
#include<stdlib.h>
#include"utils.h"
#include "a_tester.h"

// {"s":{"length": 4}}
int logic_bomb(char* s) {
    int symvar = s[0] - 48;
    int *array = (int *) malloc(sizeof(int) * 5);
    int k = 0;
    for (k=0; k<5; k++){
        array[k] = k;
    }
    array = (int *) realloc (array, sizeof(int) * 10);
    for (k=5; k<10; k++){
        array[k] = k;
    }
    if(array[symvar%10] == 7){
        fprintf(stderr, "Logic bomb triggered\n");
        return 1;
    }
    fprintf(stderr, "Path without the bomb\n");
    return 0;
}


int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <input-string>\n", argv[0]);
        return 2;
    }
    char *s = argv[1];
    return logic_bomb(s);
}