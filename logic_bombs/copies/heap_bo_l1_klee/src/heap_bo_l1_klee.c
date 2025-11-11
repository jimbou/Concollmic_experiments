#include <stdio.h>
#include <string.h> 
#include "utils.h"
#include "a_tester.h"

int n = 5;

// {"symvar":{"length": 256}}
int logic_bomb(char* symvar) {
    char *p, *q;
    p = (char*)malloc(16);
    q = (char*)malloc(16);
    strcpy(p, symvar);
    free(q);
    if (n != 5){
        free(p);
        fprintf(stderr, "Logic bomb triggered\n");
        return 1;
    }else {
        free(p);
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