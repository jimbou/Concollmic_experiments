#include <stdio.h>
/*
TOY:
*/
#include <string.h> 
#include <math.h>
#include "utils.h"
#include "a_tester.h"

// {"s":{"length": 3}}
int logic_bomb(char* symvar) {
    float i = atof(symvar);
    if(i - 7 == 0){
        fprintf(stderr, "Logic bomb triggered\n");
        return 1;
    }else{
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
