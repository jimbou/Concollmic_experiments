#include <stdio.h>
#include<stdio.h>
#include <stdlib.h>
#include "a_tester.h"

// {"s":{"length": 4}}
int logic_bomb(char* s) {
    int symvar = s[0] - 48;
    float a = symvar/70.0;
    float b = 0.1;
    if(a != 0.1){
        if(a - b == 0) {
                fprintf(stderr, "Logic bomb triggered\n");
                return 1;
            }
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