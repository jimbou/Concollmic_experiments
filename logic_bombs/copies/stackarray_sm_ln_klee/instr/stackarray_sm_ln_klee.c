#include <stdio.h>
/*
*solution: 
*/
#include<stdio.h>
#include"utils.h"

#include "a_tester.h"

// {"s":{"length": 4}}
int logic_bomb(char* s) {
    fprintf(stderr, "[stackarray_sm_ln_klee.c] enter logic_bomb 1\n");
    int symvar = s[0] - 48;
    int i = symvar;
    int j = abs(i%11);
    int a[] = {(i+5)%11,(i+6)%11,(i+7)%11,(i+8)%11,(i+9)%11,(i+10)%11,i%11,(i+1)%11,(i+2)%11,(i+3)%11,(i+4)%11};
    int* p = &a[a[a[j]]];
    int* q = &a[a[a[a[a[*p]]]]];
    // fprintf(stderr, "[stackarray_sm_ln_klee.c] exit logic_bomb 1\n");

    if(p == q){
        fprintf(stderr, "[stackarray_sm_ln_klee.c] enter logic_bomb 2\n");
        fprintf(stderr, "Logic bomb triggered\n");
        return 1;
        // fprintf(stderr, "[stackarray_sm_ln_klee.c] exit logic_bomb 2\n");
     }
    else {
        fprintf(stderr, "[stackarray_sm_ln_klee.c] enter logic_bomb 3\n");
        fprintf(stderr, "Path without the bomb\n");
        return 0;
        // fprintf(stderr, "[stackarray_sm_ln_klee.c] exit logic_bomb 3\n");
    }
}

int main(int argc, char **argv) {
    fprintf(stderr, "[stackarray_sm_ln_klee.c] enter main 1\n");
    if (argc < 2) {
        fprintf(stderr, "[stackarray_sm_ln_klee.c] enter main 2\n");
        fprintf(stderr, "Usage: %s <input-string>\n", argv[0]);
        return 2;
        // fprintf(stderr, "[stackarray_sm_ln_klee.c] exit main 2\n");
    }
    char *s = argv[1];
    // fprintf(stderr, "[stackarray_sm_ln_klee.c] exit main 1\n");
    fprintf(stderr, "[stackarray_sm_ln_klee.c] enter main 3\n");
    return logic_bomb(s);
    // fprintf(stderr, "[stackarray_sm_ln_klee.c] exit main 3\n");
}
// Total cost: 0.010440
// Total split cost: 0.000000, input tokens: 0, output tokens: 0, cache read tokens: 0, cache write tokens: 0, split chunks: [(0, 36)]
// Total instrumented cost: 0.010440, input tokens: 2400, output tokens: 444, cache read tokens: 0, cache write tokens: 0
