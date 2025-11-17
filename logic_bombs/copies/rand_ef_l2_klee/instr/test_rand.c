
#include <stdio.h>
#include <stdlib.h>

int main() {
    srand((unsigned int)'T');
    int result = rand() % 100;
    printf("%d", result);
    return 0;
}
