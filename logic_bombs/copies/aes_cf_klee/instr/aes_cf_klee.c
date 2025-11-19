#include <stdio.h>
/*
TOY:
*/
#include <inttypes.h>
#include "aes.h"
#include "utils.h"

void aes_print(uint8_t* str) {
    fprintf(stderr, "[aes_cf_klee.c] enter aes_print 1\n");
    unsigned char i;
    for(i = 0; i < 16; ++i)
        printf("%.2x", str[i]);
    printf("\n");
    // fprintf(stderr, "[aes_cf_klee.c] exit aes_print 1\n");
}

#include "a_tester.h"

// {"s":{"length": 32}}
int logic_bomb(char* s) {
    fprintf(stderr, "[aes_cf_klee.c] enter logic_bomb 1\n");
    if(strlen(s) != 32){
        fprintf(stderr, "[aes_cf_klee.c] enter logic_bomb 2\n");
        //printf("please input the 128-bit keys\n");
        fprintf(stderr, "Path without the bomb\n");
        return 0;
        // fprintf(stderr, "[aes_cf_klee.c] exit logic_bomb 2\n");
    }
    // fprintf(stderr, "[aes_cf_klee.c] exit logic_bomb 1\n");

    fprintf(stderr, "[aes_cf_klee.c] enter logic_bomb 3\n");
    uint8_t key[16];

    sscanf(s,
        "%2" SCNx8 "%2" SCNx8
        "%2" SCNx8 "%2" SCNx8
        "%2" SCNx8 "%2" SCNx8
        "%2" SCNx8 "%2" SCNx8
        "%2" SCNx8 "%2" SCNx8
        "%2" SCNx8 "%2" SCNx8
        "%2" SCNx8 "%2" SCNx8
        "%2" SCNx8 "%2" SCNx8,
        &key[0],&key[1],
        &key[2],&key[3],
        &key[4],&key[5],
        &key[6],&key[7],
        &key[8],&key[9],
        &key[10],&key[11],
        &key[12],&key[13],
        &key[14],&key[15]);

    //aes_print(key);

    uint8_t decodetext[16];
    uint8_t ciphertext[] = {0x3a, 0xd7, 0x7b, 0xb4, 0x0d, 0x7a, 0x36, 0x60, 0xa8, 0x9e, 0xca, 0xf3, 0x24, 0x66, 0xef, 0x97};
    uint8_t plaintext[] = {0x6b, 0xc1, 0xbe, 0xe2, 0x2e, 0x40, 0x9f, 0x96, 0xe9, 0x3d, 0x7e, 0x11, 0x73, 0x93, 0x17, 0x2a};

    AES128_ECB_decrypt(ciphertext, key, decodetext);

    //aes_print(decodetext);
    if(0 == memcmp((char*) plaintext, (char*) decodetext, 16)){
        fprintf(stderr, "[aes_cf_klee.c] enter logic_bomb 4\n");
        fprintf(stderr, "Logic bomb triggered\n");
        return 1;
        // fprintf(stderr, "[aes_cf_klee.c] exit logic_bomb 4\n");
    }else{
        fprintf(stderr, "[aes_cf_klee.c] enter logic_bomb 5\n");
        fprintf(stderr, "Path without the bomb\n");
        return 0;
        // fprintf(stderr, "[aes_cf_klee.c] exit logic_bomb 5\n");
    }
    // fprintf(stderr, "[aes_cf_klee.c] exit logic_bomb 3\n");
}

int main(int argc, char **argv) {
    fprintf(stderr, "[aes_cf_klee.c] enter main 1\n");
    if (argc < 2) {
        fprintf(stderr, "[aes_cf_klee.c] enter main 2\n");
        fprintf(stderr, "Usage: %s <input-string>\n", argv[0]);
        return 2;
        // fprintf(stderr, "[aes_cf_klee.c] exit main 2\n");
    }
    // fprintf(stderr, "[aes_cf_klee.c] exit main 1\n");

    fprintf(stderr, "[aes_cf_klee.c] enter main 3\n");
    char *s = argv[1];
    return logic_bomb(s);
    // fprintf(stderr, "[aes_cf_klee.c] exit main 3\n");
}
// Total cost: 0.014225
// Total split cost: 0.000000, input tokens: 0, output tokens: 0, cache read tokens: 0, cache write tokens: 0, split chunks: [(0, 72)]
// Total instrumented cost: 0.014225, input tokens: 2850, output tokens: 966, cache read tokens: 0, cache write tokens: 0
