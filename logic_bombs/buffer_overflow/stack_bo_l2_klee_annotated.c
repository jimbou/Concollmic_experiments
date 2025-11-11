#include <klee/klee.h>
#include <string.h> 
#include "utils.h"
#include "a_tester.h"

int trigger(){
    klee_assert(0 && "Logic bomb triggered");
}

// {"symvar":{"length": 128}}
int logic_bomb(char* symvar) {
    char buf[8];
    strcpy(buf, symvar);
    if(buf < 0)
        return trigger();
    klee_assert(1 && "Path without the bomb");
}

int main(int argc, char** argv) {
char symvar[129];
klee_make_symbolic(&symvar, sizeof(symvar), "symvar");
klee_assume(symvar[128]=='\0');
return logic_bomb(symvar);
}
