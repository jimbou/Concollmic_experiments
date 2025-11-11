#include <stdio.h>
/*
*solution: 
*/
#include<stdio.h>
#include"utils.h"

#include "a_tester.h"

// {"s":{"length": 4}}
int logic_bomb(char* s) {
    int symvar = s[0] - 48;
    int i = symvar;
    int j = abs(i%11);
    int a[] = {(i+5)%11,(i+6)%11,(i+7)%11,(i+8)%11,(i+9)%11,(i+10)%11,i%11,(i+1)%11,(i+2)%11,(i+3)%11,(i+4)%11};
    int* p = &a[a[a[j]]];
    int* q = &a[a[a[a[a[*p]]]]];

    if(p == q){
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
