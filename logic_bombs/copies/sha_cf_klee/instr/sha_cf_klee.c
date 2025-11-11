#include <stdio.h>
/*
TOY:
*/
#include "sha1.h"
#include "utils.h"

#include "a_tester.h"

// {"s":{"length": 4}}
int logic_bomb(char* s) {
    fprintf(stderr, "[sha_cf_klee.c] enter logic_bomb 1\n");
    int symvar = s[0] - 48;
    int plaintext = symvar;
    unsigned cipher[5];
    cipher[0] = 0X902ba3cd;
    cipher[1] = 0Xa1883801;
    cipher[2] = 0X594b6e1b;
    cipher[3] = 0X452790cc;
    cipher[4] = 0X53948fda;
    // fprintf(stderr, "[sha_cf_klee.c] exit logic_bomb 1\n");

    if(SHA1_COMP(plaintext,cipher)==0){
        fprintf(stderr, "[sha_cf_klee.c] enter logic_bomb 2\n");
        fprintf(stderr, "Logic bomb triggered\n");
        return 1;
        // fprintf(stderr, "[sha_cf_klee.c] exit logic_bomb 2\n");
    }else{
        fprintf(stderr, "[sha_cf_klee.c] enter logic_bomb 3\n");
        fprintf(stderr, "Path without the bomb\n");
        return 0;
        // fprintf(stderr, "[sha_cf_klee.c] exit logic_bomb 3\n");
    }
}

int main(int argc, char **argv) {
    fprintf(stderr, "[sha_cf_klee.c] enter main 1\n");
    if (argc < 2) {
        fprintf(stderr, "[sha_cf_klee.c] enter main 2\n");
        fprintf(stderr, "Usage: %s <input-string>\n", argv[0]);
        return 2;
        // fprintf(stderr, "[sha_cf_klee.c] exit main 2\n");
    }
    char *s = argv[1];
    // fprintf(stderr, "[sha_cf_klee.c] exit main 1\n");
    fprintf(stderr, "[sha_cf_klee.c] enter main 3\n");
    return logic_bomb(s);
    // fprintf(stderr, "[sha_cf_klee.c] exit main 3\n");
}
// Total cost: 0.010143
// Total split cost: 0.000000, input tokens: 0, output tokens: 0, cache read tokens: 0, cache write tokens: 0, split chunks: [(0, 37)]
// Total instrumented cost: 0.010143, input tokens: 2373, output tokens: 421, cache read tokens: 0, cache write tokens: 0
