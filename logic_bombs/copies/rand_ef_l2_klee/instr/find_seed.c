
#include <stdio.h>
#include <stdlib.h>

int main() {
    for (int seed = 0; seed < 256; seed++) {
        srand(seed);
        int r = rand() % 100;
        if (r == 77) {
            printf("Seed %d (ASCII: '%c') produces rand()%%100 = 77\n", seed, seed);
        }
    }
    return 0;
}
