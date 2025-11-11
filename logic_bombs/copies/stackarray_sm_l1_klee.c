#include <stdio.h>
#include<stdio.h>
#include"utils.h"
#include "a_tester.h"

// {"s":{"length": 4}}
int logic_bomb(char* s) {
    int symvar = s[0] - 48;
    int ary[] ={1,2,3,4,5};
    if(ary[symvar%5] == 5){
        fprintf(stderr, "Logic bomb triggered\n");
        return 1;
     }
    else {
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
