#include <stdio.h>
#include <stdio.h>
#include "utils.h"

#define jmp(addr) asm("jmp *%0"::"r"(addr):)

#include "a_tester.h"

// {"s":{"length": 4}}
int logic_bomb(char* s) {
    int symvar = s[0] - 48;
    if (symvar%6 != 1 || symvar < 10|| symvar > 40 || symvar == 19)
	symvar = 13;
    long long addr = &&flag_0 + symvar;
    jmp(addr);
  flag_0:
    if (symvar > 0){
        symvar++;
        if(symvar == 0) {
            fprintf(stderr, "Logic bomb triggered\n");
            return 1;
        }
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