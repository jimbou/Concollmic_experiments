#include <stdio.h>
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
    fprintf(stderr, "[ping_csv_klee.c] enter ping_it 1\n");
    struct icmphdr icmp_hdr;
    struct sockaddr_in addr;
    int sequence = 0;
    int sock = socket(AF_INET,SOCK_DGRAM,IPPROTO_ICMP);
    if (sock < 0) {
        fprintf(stderr, "[ping_csv_klee.c] enter ping_it 2\n");
        perror("socket");
        return -1;
        // fprintf(stderr, "[ping_csv_klee.c] exit ping_it 2\n");
    }
    // fprintf(stderr, "[ping_csv_klee.c] exit ping_it 1\n");

    fprintf(stderr, "[ping_csv_klee.c] enter ping_it 3\n");
    memset(&addr, 0, sizeof addr);
    addr.sin_family = AF_INET;
    addr.sin_addr = *dst;

    memset(&icmp_hdr, 0, sizeof icmp_hdr);
    icmp_hdr.type = ICMP_ECHO;
    icmp_hdr.un.echo.id = 1234;//arbitrary id

    unsigned char data[2048];
    int rc;
    struct timeval timeout = {1, 0}; //wait max 3 seconds for a reply
    fd_set read_set;
    socklen_t slen;
    struct icmphdr rcv_hdr;

    icmp_hdr.un.echo.sequence = sequence++;
    memcpy(data, &icmp_hdr, sizeof icmp_hdr);
    memcpy(data + sizeof icmp_hdr, "hello", 5); //icmp payload
    rc = sendto(sock, data, sizeof icmp_hdr + 5,
                    0, (struct sockaddr*)&addr, sizeof addr);
    if (rc <= 0) {
        fprintf(stderr, "[ping_csv_klee.c] enter ping_it 4\n");
        perror("Sendto");
        // fprintf(stderr, "[ping_csv_klee.c] exit ping_it 4\n");
    }
    puts("Sent ICMP\n");
    // fprintf(stderr, "[ping_csv_klee.c] exit ping_it 3\n");

    fprintf(stderr, "[ping_csv_klee.c] enter ping_it 5\n");
    memset(&read_set, 0, sizeof read_set);
    FD_SET(sock, &read_set);

    //wait for a reply with a timeout
    rc = select(sock + 1, &read_set, NULL, NULL, &timeout);
    if (rc == 0) {
        fprintf(stderr, "[ping_csv_klee.c] enter ping_it 6\n");
        puts("Got no reply\n");
        return 0;
        // fprintf(stderr, "[ping_csv_klee.c] exit ping_it 6\n");
    } else if (rc < 0) {
        fprintf(stderr, "[ping_csv_klee.c] enter ping_it 7\n");
        perror("Select");
        return 0;
        // fprintf(stderr, "[ping_csv_klee.c] exit ping_it 7\n");
    }
    // fprintf(stderr, "[ping_csv_klee.c] exit ping_it 5\n");

    fprintf(stderr, "[ping_csv_klee.c] enter ping_it 8\n");
    //we don't care about the sender address in this example..
    slen = 0;
    rc = recvfrom(sock, data, sizeof data, 0, NULL, &slen);
    if (rc <= 0) {
        fprintf(stderr, "[ping_csv_klee.c] enter ping_it 9\n");
        perror("recvfrom");
        // fprintf(stderr, "[ping_csv_klee.c] exit ping_it 9\n");
    } else if (rc < sizeof rcv_hdr) {
        fprintf(stderr, "[ping_csv_klee.c] enter ping_it 10\n");
        //printf("Error, got short ICMP packet, %d bytes\n", rc);
        // fprintf(stderr, "[ping_csv_klee.c] exit ping_it 10\n");
    }
    memcpy(&rcv_hdr, data, sizeof rcv_hdr);
    if (rcv_hdr.type == ICMP_ECHOREPLY) {
        fprintf(stderr, "[ping_csv_klee.c] enter ping_it 11\n");
        //printf("ICMP Reply, id=0x%x, sequence =  0x%x\n",
                        //icmp_hdr.un.echo.id, icmp_hdr.un.echo.sequence);
        // fprintf(stderr, "[ping_csv_klee.c] exit ping_it 11\n");
    } else {
        fprintf(stderr, "[ping_csv_klee.c] enter ping_it 12\n");
        //printf("Got ICMP packet with type 0x%x ?!?\n", rcv_hdr.type);
        // fprintf(stderr, "[ping_csv_klee.c] exit ping_it 12\n");
    }
    // fprintf(stderr, "[ping_csv_klee.c] exit ping_it 8\n");
    return 1;
}

#include "a_tester.h"

// {"s":{"length": 32}}
int logic_bomb(char* s) {
    fprintf(stderr, "[ping_csv_klee.c] enter logic_bomb 1\n");
    struct in_addr dst;

    if (inet_aton(s, &dst) == 0) {
        fprintf(stderr, "[ping_csv_klee.c] enter logic_bomb 2\n");
        perror("inet_aton");
        //printf("%s isn't a valid IP address\n", s);
        fprintf(stderr, "Path without the bomb\n");
        return 0;
        // fprintf(stderr, "[ping_csv_klee.c] exit logic_bomb 2\n");
    }

    if (ping_it(&dst) == 1){
        fprintf(stderr, "[ping_csv_klee.c] enter logic_bomb 3\n");
        fprintf(stderr, "Logic bomb triggered\n");
        return 1;
        // fprintf(stderr, "[ping_csv_klee.c] exit logic_bomb 3\n");
    }else{
        fprintf(stderr, "[ping_csv_klee.c] enter logic_bomb 4\n");
        fprintf(stderr, "Path without the bomb\n");
        return 0;
        // fprintf(stderr, "[ping_csv_klee.c] exit logic_bomb 4\n");
    }
    // fprintf(stderr, "[ping_csv_klee.c] exit logic_bomb 1\n");
}

int main(int argc, char **argv) {
    fprintf(stderr, "\n");
    if (argc < 2) {
        fprintf(stderr, "[ping_csv_klee.c] enter main 2\n");
        fprintf(stderr, "Usage: %s <input-string>\n", argv[0]);
        return 2;
        // fprintf(stderr, "[ping_csv_klee.c] exit main 2\n");
    }
    fprintf(stderr, "[ping_csv_klee.c] enter main 3\n");
    char *s = argv[1];
    return logic_bomb(s);
    // fprintf(stderr, "[ping_csv_klee.c] exit main 3\n");
}
// Total cost: 0.085010
// Total split cost: 0.000000, input tokens: 0, output tokens: 0, cache read tokens: 0, cache write tokens: 0, split chunks: [(0, 110)]
// Total instrumented cost: 0.085010, input tokens: 20152, output tokens: 5207, cache read tokens: 0, cache write tokens: 0
