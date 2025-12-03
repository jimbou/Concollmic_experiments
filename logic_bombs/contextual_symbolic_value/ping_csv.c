#include <klee/klee.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip_icmp.h>
#include <arpa/inet.h>
#include <sys/select.h>


int ping_it(struct in_addr *dst)
{
    char cmd[128];
    snprintf(cmd, sizeof(cmd),
             "ping -c 1 -W 1 %s > /dev/null 2>&1",
             inet_ntoa(*dst));

    return system(cmd) == 0;
}

#include "a_tester.h"

// {"s":{"length": 32}}
int logic_bomb(char* s) {
    struct in_addr dst;

    if (inet_aton(s, &dst) == 0) {
        perror("inet_aton");
        //printf("%s isn't a valid IP address\n", s);
        klee_assert(0 && "Path without the bomb");
        return 0;
    }

    if (ping_it(&dst) == 1){
        klee_assert(0 && "Logic bomb triggered");
        return 0;
    }else{
        klee_assert(0 && "Path without the bomb");
        return 0;
    }
}


