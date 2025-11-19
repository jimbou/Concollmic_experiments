
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void logic_bomb(const char* s) {
    srand(s[0]);
    int r = rand() % 100;
    printf("Using seed '%c' (ASCII: %d), rand()%%100 = %d\n", s[0], s[0], r);
    if (r == 77) {
        printf("Target branch reached!\n");
    } else {
        printf("Target branch not reached.\n");
    }
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("Usage: %s <string>\n", argv[0]);
        return 1;
    }
    
    logic_bomb(argv[1]);
    return 0;
}
