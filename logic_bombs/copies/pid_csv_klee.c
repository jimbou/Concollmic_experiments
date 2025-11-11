#include <stdio.h>
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "a_tester.h"

// {"s":{"length": 4}}
int logic_bomb(char* s) {
   int symvar = s[0] - 48;
   int pid = (int) getpid();
   if(pid%78 == symvar){
        fprintf(stderr, "Logic bomb triggered\n");
        return 1;
   } else {
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
