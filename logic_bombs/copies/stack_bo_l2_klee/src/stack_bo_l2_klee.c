#include <stdio.h>
#include <string.h> 
#include "utils.h"
#include "a_tester.h"

int trigger(){
    fprintf(stderr, "Logic bomb triggered\n");
    return 1;
}

// {"symvar":{"length": 128}}
int logic_bomb(char* symvar) {
    char buf[8];
    strcpy(buf, symvar);
    if(buf < 0)
        return trigger();
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