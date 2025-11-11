#include <stdio.h>
#include <stdio.h>
#include "utils.h"

#include "a_tester.h"

int func0(){return 0;}
int func1(){return 1;}
int func2(){return 2;}
int func3(){return 3;}
int func4(){return 4;}
int func5(){return 5;}
int func6(){return 6;}

// {"s":{"length": 4}}
int logic_bomb(char* s) {
    int (*f[7])() = {func0, func1, func2, func3, func4, func5, func6};
    int symvar = s[0] - 48;
    int ret = f[symvar%7](); 
    printf ("ret = %d\n", ret);
    if (ret == 5){
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