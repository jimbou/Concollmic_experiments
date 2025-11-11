#include <stdio.h>
#include <string.h> 
#include "utils.h"

#include "a_tester.h"

char* shell(const char* cmd)
{
    fprintf(stderr, "[echo_cp_l1_klee.c] enter shell 1\n");
    char* rs = "";
    FILE *f;
    f = popen(cmd, "r");
    char buf[1024];
    memset(buf,'\0',sizeof(buf));
    while(fgets(buf,1024-1,f)!=NULL)
    { 
       fprintf(stderr, "[echo_cp_l1_klee.c] enter shell 2\n");
       rs = buf;
       // fprintf(stderr, "[echo_cp_l1_klee.c] exit shell 2\n");
    }

    pclose(f);
    return rs;
    // fprintf(stderr, "[echo_cp_l1_klee.c] exit shell 1\n");
}

// {"s":{"length": 4}}
int logic_bomb(char* s) {
    fprintf(stderr, "[echo_cp_l1_klee.c] enter logic_bomb 1\n");
    int symvar = s[0] - 48;
    char cmd[256];
    sprintf(cmd, "echo %d\n", symvar); 
    char* rs = shell(cmd);
    // fprintf(stderr, "[echo_cp_l1_klee.c] exit logic_bomb 1\n");

    if(atoi(rs) == 7){
        fprintf(stderr, "[echo_cp_l1_klee.c] enter logic_bomb 2\n");
        fprintf(stderr, "Logic bomb triggered\n");
        return 1;
        // fprintf(stderr, "[echo_cp_l1_klee.c] exit logic_bomb 2\n");
    } else {
        fprintf(stderr, "[echo_cp_l1_klee.c] enter logic_bomb 3\n");
        fprintf(stderr, "Path without the bomb\n");
        return 0;
        // fprintf(stderr, "[echo_cp_l1_klee.c] exit logic_bomb 3\n");
    }
}

int main(int argc, char **argv) {
    fprintf(stderr, "[echo_cp_l1_klee.c] enter main 1\n");
    if (argc < 2) {
        fprintf(stderr, "[echo_cp_l1_klee.c] enter main 2\n");
        fprintf(stderr, "Usage: %s <input-string>\n", argv[0]);
        return 2;
        // fprintf(stderr, "[echo_cp_l1_klee.c] exit main 2\n");
    }
    char *s = argv[1];
    // fprintf(stderr, "[echo_cp_l1_klee.c] exit main 1\n");
    fprintf(stderr, "[echo_cp_l1_klee.c] enter main 3\n");
    return logic_bomb(s);
    // fprintf(stderr, "[echo_cp_l1_klee.c] exit main 3\n");
}
// Total cost: 0.008265
// Total split cost: 0.000000, input tokens: 0, output tokens: 0, cache read tokens: 0, cache write tokens: 0, split chunks: [(0, 46)]
// Total instrumented cost: 0.008265, input tokens: 2394, output tokens: 484, cache read tokens: 0, cache write tokens: 0
