#include <stdio.h>
#include <string.h> 
#include "utils.h"
#include "a_tester.h"

long f(long x){
    if (x%2 == 0)
	return x/2;
    else if (x%3 == 0)
	return x/3;
    else
        return 3*x + 1;
}

// {"s":{"length": 4}}
int logic_bomb(char* s) {
    int symvar = s[0] - 48;
    symvar = symvar + 94;
    long j = f(symvar);
    int loopcount = 1;
    while(j != 1){
	j = f(j);
        loopcount ++;
    }
    if(loopcount == 25){
        fprintf(stderr, "Logic bomb triggered\n");
        return 1;
    } else {
        fprintf(stderr, "Path without the bomb\n");
        return 0;}
}

int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <input-string>\n", argv[0]);
        return 2;
    }
    char *s = argv[1];
    return logic_bomb(s);
}