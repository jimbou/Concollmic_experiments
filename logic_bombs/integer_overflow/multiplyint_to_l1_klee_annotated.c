#include <klee/klee.h>
#include "a_tester.h"

// {"s":{"length": 4}}
int logic_bomb(char* s) {
    int symvar = s[0] - 48;
    if (254748364 * symvar < 0 && symvar > 0)
        klee_assert(0 && "Logic bomb triggered");
    else
        klee_assert(1 && "Path without the bomb");
}

int main(int argc, char** argv) {
char s[5];
klee_make_symbolic(&s, sizeof(s), "s");
klee_assume(s[4]=='\0');
return logic_bomb(s);
}
