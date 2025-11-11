#include <stdio.h>
/*
TOY:
Solution: 30
*/
#include <string.h> 
#include <math.h>
#include "utils.h"

#define PI 3.14159265358979323846264338327

#include "a_tester.h"

// {"s":{"length": 4}}
int logic_bomb(char* s) {
    int symvar = s[0];
    float v = sin(symvar*PI/30);
    if(v > 0.5){
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
