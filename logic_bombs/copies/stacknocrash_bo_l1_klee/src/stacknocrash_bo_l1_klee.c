#include <stdio.h>
#include <string.h> 
#include "utils.h"
#include "a_tester.h"


// {"symvar":{"length": 64}}
int logic_bomb(char* symvar) {
    int flag = 0;
    char buf[8];
    if(strlen(symvar) > 9)
        fprintf(stderr, "Path without the bomb\n");
return 0;
    strcpy(buf, symvar);
    if(flag == 1){
        fprintf(stderr, "Logic bomb triggered\n");
        return 1;
    }
    fprintf(stderr, "Path without the bomb\n");
    return 0;
}

int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <input-string>\n", argv[0]);
        return 2;
    }
    char *s = argv[1];
    return logic_bomb(s);
}