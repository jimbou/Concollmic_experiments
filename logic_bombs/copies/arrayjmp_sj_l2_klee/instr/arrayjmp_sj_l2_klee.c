#include <stdio.h>
#include <stdio.h>
#include "utils.h"

#define jmp(addr) asm("jmp *%0"::"r"(addr):)

#include "a_tester.h"

// {"s":{"length": 4}}
int logic_bomb(char* s) {
    fprintf(stderr, "[arrayjmp_sj_l2_klee.c] enter logic_bomb 1\n");
    int symvar = s[0] - 48;
    int array[] = {
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
    10, 11, 12, 13, 14, 15, 16, 17, 18, 19,
    20, 21, 22, 23, 24, 25, 26, 27, 28, 29,
    30, 31, 32, 33, 34, 35, 36, 37, 38, 39,
    40, 41, 42, 43, 44, 45, 46, 47, 48, 49,
    50, 51, 52, 53, 54, 55, 56, 57, 58, 59,
    60, 61, 62, 63, 64, 65, 66, 67, 68, 69,
    70, 71, 72, 73, 74, 75, 76, 77, 78, 79,
    80, 81, 82, 83, 84, 85, 86, 87, 88, 89,
    90, 91, 92, 93, 94, 95, 96, 97, 98, 99,
    100, 101, 102, 103, 104, 105, 106, 107, 108, 109,
    110, 111, 112, 113, 114, 115, 116, 117, 118, 119,
    120, 121, 122, 123, 124, 125, 126, 127, 128, 129,
    130, 131, 132, 133, 134, 135, 136, 137, 138, 139,
    140, 141, 142, 143, 144, 145, 146, 147, 148, 149,
    150, 151, 152, 153, 154, 155, 156, 157, 158, 159,
    160, 161, 162, 163, 164, 165, 166, 167, 168, 169,
    170, 171, 172, 173, 174, 175, 176, 177, 178, 179,
    180, 181, 182, 183, 184, 185, 186, 187, 188, 189,
    190, 191, 192, 193, 194, 195, 196, 197, 198, 199,
    200, 201, 202, 203, 204, 205, 206, 207, 208, 209,
    210, 211, 212, 213, 214, 215, 216, 217, 218, 219,
    220, 221, 222, 223, 224, 225, 226, 227, 228, 229,
    230, 231, 232, 233, 234, 235, 236, 237, 238, 239,
    240, 241, 242, 243, 244, 245, 246, 247, 248, 249,
    250, 251, 252, 253, 254, 255
};
    long long addr = &&flag_0 + array[symvar%256];
    jmp(addr);
    // fprintf(stderr, "[arrayjmp_sj_l2_klee.c] exit logic_bomb 1\n");

  flag_0:
    fprintf(stderr, "[arrayjmp_sj_l2_klee.c] enter logic_bomb 2\n");
    if (symvar > 0){
        fprintf(stderr, "[arrayjmp_sj_l2_klee.c] enter logic_bomb 3\n");
        symvar++;
        // fprintf(stderr, "[arrayjmp_sj_l2_klee.c] exit logic_bomb 3\n");
        if(symvar == 0){
            fprintf(stderr, "[arrayjmp_sj_l2_klee.c] enter logic_bomb 4\n");
            fprintf(stderr, "Logic bomb triggered\n");
            return 1;
            // fprintf(stderr, "[arrayjmp_sj_l2_klee.c] exit logic_bomb 4\n");
        }
    }
    // fprintf(stderr, "[arrayjmp_sj_l2_klee.c] exit logic_bomb 2\n");
    fprintf(stderr, "[arrayjmp_sj_l2_klee.c] enter logic_bomb 5\n");
    fprintf(stderr, "Path without the bomb\n");
    return 0;
    // fprintf(stderr, "[arrayjmp_sj_l2_klee.c] exit logic_bomb 5\n");
}

int main(int argc, char **argv) {
    fprintf(stderr, "[arrayjmp_sj_l2_klee.c] enter main 1\n");
    if (argc < 2) {
        fprintf(stderr, "[arrayjmp_sj_l2_klee.c] enter main 2\n");
        fprintf(stderr, "Usage: %s <input-string>\n", argv[0]);
        return 2;
        // fprintf(stderr, "[arrayjmp_sj_l2_klee.c] exit main 2\n");
    }
    // fprintf(stderr, "[arrayjmp_sj_l2_klee.c] exit main 1\n");
    fprintf(stderr, "[arrayjmp_sj_l2_klee.c] enter main 3\n");
    char *s = argv[1];
    return logic_bomb(s);
    // fprintf(stderr, "[arrayjmp_sj_l2_klee.c] exit main 3\n");
}
// Total cost: 0.007992
// Total split cost: 0.000000, input tokens: 0, output tokens: 0, cache read tokens: 0, cache write tokens: 0, split chunks: [(0, 34)]
// Total instrumented cost: 0.007992, input tokens: 2369, output tokens: 463, cache read tokens: 0, cache write tokens: 0
