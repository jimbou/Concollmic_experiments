#include <stdio.h>
#include<stdio.h>
#include <stdlib.h>
#include "a_tester.h"

// {"s":{"length": 8}}
int logic_bomb(char* symvar) {
    float x = atof(symvar);
    x = x/10.0;
    x = x + 0.1;
    x = x * x;
    if (x > 0.1)
	x -= x;
    if(x != 0.02){
        x = x + 7.98;
        if(x == 8){
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