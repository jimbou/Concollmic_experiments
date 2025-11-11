#include <stdio.h>
#include <string.h>

#include "utils.h"
#include "a_tester.h"

// {"s": {"length": 4}}
int logic_bomb(char* s) {
    int symvar = s[0] - 48;
    int j;
    char file[] = "tmp.covpro";
    int fd = open(file, O_CREAT | O_WRONLY | O_TRUNC, S_IRUSR | S_IWUSR);
    if(fd < 0)
    {
        exit(-1);             
    }
    write(fd, &symvar, sizeof symvar);
    close(fd);
    fd = open(file, O_RDONLY);
    read(fd, &j, sizeof j);
    close(fd);
    if(j == 7){
        fprintf(stderr, "Logic bomb triggered\n");
        return 1;
    } else{
        fprintf(stderr, "Path without the bomb\n");
        return 0;
    }
}

int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <input-string>\n", argv[0]);
        return 2;
    }
    char *s = argv[1];
    return logic_bomb(s);
}
