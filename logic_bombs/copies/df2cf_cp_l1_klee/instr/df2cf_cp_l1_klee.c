#include <stdio.h>
/*
TOY:
*/
#include <string.h> 
#include "utils.h"

#include "a_tester.h"

int df2cf(char a)
{
    fprintf(stderr, "[df2cf_cp_l1_klee.c] enter df2cf 1\n");
    int b;
    switch(a){
      case 0:
        fprintf(stderr, "[df2cf_cp_l1_klee.c] enter df2cf 2\n");
        b = 0;
        break;
        // fprintf(stderr, "[df2cf_cp_l1_klee.c] exit df2cf 2\n");
      case 1:
        fprintf(stderr, "[df2cf_cp_l1_klee.c] enter df2cf 3\n");
        b = 1;
        break;
        // fprintf(stderr, "[df2cf_cp_l1_klee.c] exit df2cf 3\n");
      case 2:
        fprintf(stderr, "[df2cf_cp_l1_klee.c] enter df2cf 4\n");
        b = 2;
        break;
        // fprintf(stderr, "[df2cf_cp_l1_klee.c] exit df2cf 4\n");
      case 3:
        fprintf(stderr, "[df2cf_cp_l1_klee.c] enter df2cf 5\n");
        b = 3;
        break;
        // fprintf(stderr, "[df2cf_cp_l1_klee.c] exit df2cf 5\n");
      case 4:
        fprintf(stderr, "[df2cf_cp_l1_klee.c] enter df2cf 6\n");
        b = 4;
        break;
        // fprintf(stderr, "[df2cf_cp_l1_klee.c] exit df2cf 6\n");
      case 5:
        fprintf(stderr, "[df2cf_cp_l1_klee.c] enter df2cf 7\n");
        b = 5;
        break;
        // fprintf(stderr, "[df2cf_cp_l1_klee.c] exit df2cf 7\n");
      case 6:
        fprintf(stderr, "[df2cf_cp_l1_klee.c] enter df2cf 8\n");
        b = 6;
        break;
        // fprintf(stderr, "[df2cf_cp_l1_klee.c] exit df2cf 8\n");
      case 7:
        fprintf(stderr, "[df2cf_cp_l1_klee.c] enter df2cf 9\n");
        b = 7;
        break;
        // fprintf(stderr, "[df2cf_cp_l1_klee.c] exit df2cf 9\n");
      case 8:
        fprintf(stderr, "[df2cf_cp_l1_klee.c] enter df2cf 10\n");
        b = 8;
        break;
        // fprintf(stderr, "[df2cf_cp_l1_klee.c] exit df2cf 10\n");
      case 9:
        fprintf(stderr, "[df2cf_cp_l1_klee.c] enter df2cf 11\n");
        b = 9;
        break;
        // fprintf(stderr, "[df2cf_cp_l1_klee.c] exit df2cf 11\n");
      default:
        fprintf(stderr, "[df2cf_cp_l1_klee.c] enter df2cf 12\n");
        b = 0;
        break;
        // fprintf(stderr, "[df2cf_cp_l1_klee.c] exit df2cf 12\n");
    }
    return b;
    // fprintf(stderr, "[df2cf_cp_l1_klee.c] exit df2cf 1\n");
}

// {"s":{"length": 4}}
int logic_bomb(char* s) {
    fprintf(stderr, "[df2cf_cp_l1_klee.c] enter logic_bomb 1\n");
    int symvar = s[0] - 48;
    int a = df2cf(symvar%10);
    a++;
    int b = symvar + a;
    if(b == 15){
        fprintf(stderr, "[df2cf_cp_l1_klee.c] enter logic_bomb 2\n");
        fprintf(stderr, "Logic bomb triggered\n");
        return 1;
        // fprintf(stderr, "[df2cf_cp_l1_klee.c] exit logic_bomb 2\n");
    } else {
        fprintf(stderr, "[df2cf_cp_l1_klee.c] enter logic_bomb 3\n");
        fprintf(stderr, "Path without the bomb\n");
        return 0;
        // fprintf(stderr, "[df2cf_cp_l1_klee.c] exit logic_bomb 3\n");
    }
    // fprintf(stderr, "[df2cf_cp_l1_klee.c] exit logic_bomb 1\n");
}

int main(int argc, char **argv) {
    fprintf(stderr, "[df2cf_cp_l1_klee.c] enter main 1\n");
    if (argc < 2) {
        fprintf(stderr, "[df2cf_cp_l1_klee.c] enter main 2\n");
        fprintf(stderr, "Usage: %s <input-string>\n", argv[0]);
        return 2;
        // fprintf(stderr, "[df2cf_cp_l1_klee.c] exit main 2\n");
    }
    char *s = argv[1];
    return logic_bomb(s);
    // fprintf(stderr, "[df2cf_cp_l1_klee.c] exit main 1\n");
}
// Total cost: 0.011963
// Total split cost: 0.000000, input tokens: 0, output tokens: 0, cache read tokens: 0, cache write tokens: 0, split chunks: [(0, 73)]
// Total instrumented cost: 0.011963, input tokens: 2481, output tokens: 832, cache read tokens: 0, cache write tokens: 0
