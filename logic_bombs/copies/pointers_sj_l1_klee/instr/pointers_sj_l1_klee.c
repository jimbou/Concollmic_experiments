#include <stdio.h>
#include <stdio.h>
#include "utils.h"

#include "a_tester.h"

int func0(){return 0;}
int func1(){return 1;}
int func2(){return 2;}
int func3(){return 3;}
int func4(){return 4;}
int func5(){return 5;}
int func6(){return 6;}

// {"s":{"length": 4}}
int logic_bomb(char* s) {
    fprintf(stderr, "[pointers_sj_l1_klee.c] enter logic_bomb 1\n");
    int (*f[7])() = {func0, func1, func2, func3, func4, func5, func6};
    int symvar = s[0] - 48;
    int ret = f[symvar%7](); 
    printf ("ret = %d\n", ret);
    if (ret == 5){
        fprintf(stderr, "[pointers_sj_l1_klee.c] enter logic_bomb 2\n");
        fprintf(stderr, "Logic bomb triggered\n");
        return 1;
        // fprintf(stderr, "[pointers_sj_l1_klee.c] exit logic_bomb 2\n");
    }
    fprintf(stderr, "[pointers_sj_l1_klee.c] enter logic_bomb 3\n");
    fprintf(stderr, "Path without the bomb\n");
    return 0;
    // fprintf(stderr, "[pointers_sj_l1_klee.c] exit logic_bomb 3\n");
    // fprintf(stderr, "[pointers_sj_l1_klee.c] exit logic_bomb 1\n");
}

int main(int argc, char **argv) {
    fprintf(stderr, "[pointers_sj_l1_klee.c] enter main 1\n");
    if (argc < 2) {
        fprintf(stderr, "[pointers_sj_l1_klee.c] enter main 2\n");
        fprintf(stderr, "Usage: %s <input-string>\n", argv[0]);
        return 2;
        // fprintf(stderr, "[pointers_sj_l1_klee.c] exit main 2\n");
    }
    fprintf(stderr, "[pointers_sj_l1_klee.c] enter main 3\n");
    char *s = argv[1];
    return logic_bomb(s);
    // fprintf(stderr, "[pointers_sj_l1_klee.c] exit main 3\n");
    // fprintf(stderr, "[pointers_sj_l1_klee.c] exit main 1\n");
}
// Total cost: 0.007760
// Total split cost: 0.000000, input tokens: 0, output tokens: 0, cache read tokens: 0, cache write tokens: 0, split chunks: [(0, 36)]
// Total instrumented cost: 0.007760, input tokens: 2392, output tokens: 434, cache read tokens: 0, cache write tokens: 0
