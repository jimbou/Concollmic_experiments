#include <klee/klee.h>
/*
TOY:
*/
#include <string.h> 
#include <math.h>
#include "utils.h"
#include "a_tester.h"

// {"s":{"length": 3}}
int logic_bomb(char* symvar) {
    float i = atof(symvar);
    if(i - 7 == 0){
        klee_assert(0 && "Logic bomb triggered");
    }else{
        klee_assert(1 && "Path without the bomb");
    }
}

int main(int argc, char** argv) {
char symvar[1];
klee_make_symbolic(&symvar, sizeof(symvar), "symvar");
klee_assume(symvar[0]=='\0');
return logic_bomb(symvar);
}
