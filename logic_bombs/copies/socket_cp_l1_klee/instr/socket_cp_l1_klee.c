#include <stdio.h>
/*
TOY:
*/
#include "utils.h"
#include <unistd.h>
#include "a_tester.h"

int server(){
    fprintf(stderr, "\n");
    int server_sockfd,client_sockfd;
    int server_len,client_len;
    struct sockaddr_in server_address;
    struct sockaddr_in client_address;
    int i,btye;
    char char_recv,char_send;

    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_address.sin_port = 19991;
    server_len = sizeof(server_address);

    server_sockfd = socket(AF_INET,SOCK_STREAM,0);

    bind(server_sockfd,(struct sockaddr *)&server_address,server_len);

    listen(server_sockfd,5);
    //printf("server waiting for connect\n");

    client_len = sizeof(client_address);
    client_sockfd = accept(server_sockfd,(struct sockaddr *)&client_address,(socklen_t *)&client_len);

    if(btye = recv(client_sockfd,&char_recv,1,0) == -1) {
        fprintf(stderr, "[socket_cp_l1_klee.c] enter server 2\n");
        perror("recv");
        exit(EXIT_FAILURE);
        // fprintf(stderr, "[socket_cp_l1_klee.c] exit server 2\n");
    }
    //printf("receive from client is %c\n",char_recv);

    char_send = char_recv;
    if(btye = send(client_sockfd,&char_send,1,0) == -1) {
        fprintf(stderr, "[socket_cp_l1_klee.c] enter server 3\n");
        perror("send");
        exit(EXIT_FAILURE);
        // fprintf(stderr, "[socket_cp_l1_klee.c] exit server 3\n");
    }

    fprintf(stderr, "[socket_cp_l1_klee.c] enter server 4\n");
    close(client_sockfd);
    close(server_sockfd);
    // fprintf(stderr, "[socket_cp_l1_klee.c] exit server 4\n");
}

int client_send(char char_send){
    fprintf(stderr, "\n");
    //printf("client start\n");
    int sockfd;
    int len;
    struct sockaddr_in address;
    int result;
    int i,byte;
    char char_recv;
    if((sockfd = socket(AF_INET,SOCK_STREAM,0)) == -1) {
        fprintf(stderr, "[socket_cp_l1_klee.c] enter client_send 2\n");
        perror("socket");
        exit(-1);
        // fprintf(stderr, "[socket_cp_l1_klee.c] exit client_send 2\n");
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr("127.0.0.1");
    address.sin_port = 19991;
    len = sizeof(address);
    if((result = connect(sockfd,(struct sockaddr *)&address,len)) == -1) {
        fprintf(stderr, "[socket_cp_l1_klee.c] enter client_send 3\n");
        perror("connect");
        exit(-1);
        // fprintf(stderr, "[socket_cp_l1_klee.c] exit client_send 3\n");
    }

    if(byte = send(sockfd,&char_send,1,0) == -1) {
        fprintf(stderr, "[socket_cp_l1_klee.c] enter client_send 4\n");
        perror("send");
        exit(-1);
        // fprintf(stderr, "[socket_cp_l1_klee.c] exit client_send 4\n");
    }
    if(byte = recv(sockfd,&char_recv,1,0) == -1) {
        fprintf(stderr, "[socket_cp_l1_klee.c] enter client_send 5\n");
        perror("recv");
        exit(-1);
        // fprintf(stderr, "[socket_cp_l1_klee.c] exit client_send 5\n");
    }
    //printf("receive from server %c\n",char_recv);
    int ret = char_recv - 48;
    fprintf(stderr, "[socket_cp_l1_klee.c] enter client_send 6\n");
    close(sockfd);
    return ret;
    // fprintf(stderr, "[socket_cp_l1_klee.c] exit client_send 6\n");
}

// {"s":{"length": 16}}
int logic_bomb(char* s) {
    fprintf(stderr, "[socket_cp_l1_klee.c] enter logic_bomb 1\n");
    int pid1,status,i=0;
    pid1=fork();
    if(pid1 < 0){
        fprintf(stderr, "[socket_cp_l1_klee.c] enter logic_bomb 2\n");
        fprintf(stderr, "Path without the bomb\n");
        return 0;
        // fprintf(stderr, "[socket_cp_l1_klee.c] exit logic_bomb 2\n");
    }
    else if(pid1 == 0){
        fprintf(stderr, "[socket_cp_l1_klee.c] enter logic_bomb 3\n");
        server();
        waitpid(NULL);
        exit(0);
        // fprintf(stderr, "[socket_cp_l1_klee.c] exit logic_bomb 3\n");
    }else{
        fprintf(stderr, "[socket_cp_l1_klee.c] enter logic_bomb 4\n");
        sleep(5);
        i=client_send(s[0]);
        if(i == 7){
            fprintf(stderr, "[socket_cp_l1_klee.c] enter logic_bomb 5\n");
            fprintf(stderr, "Logic bomb triggered\n");
            return 1;
            // fprintf(stderr, "[socket_cp_l1_klee.c] exit logic_bomb 5\n");
         }else{
            fprintf(stderr, "[socket_cp_l1_klee.c] enter logic_bomb 6\n");
            fprintf(stderr, "Path without the bomb\n");
            return 0;
            // fprintf(stderr, "[socket_cp_l1_klee.c] exit logic_bomb 6\n");
         }
        // fprintf(stderr, "[socket_cp_l1_klee.c] exit logic_bomb 4\n");
    }
    // fprintf(stderr, "[socket_cp_l1_klee.c] exit logic_bomb 1\n");
}

int main(int argc, char **argv) {
    fprintf(stderr, "[socket_cp_l1_klee.c] enter main 1\n");
    if (argc < 2) {
        fprintf(stderr, "[socket_cp_l1_klee.c] enter main 2\n");
        fprintf(stderr, "Usage: %s <input-string>\n", argv[0]);
        return 2;
        // fprintf(stderr, "[socket_cp_l1_klee.c] exit main 2\n");
    }
    char *s = argv[1];
    fprintf(stderr, "[socket_cp_l1_klee.c] enter main 3\n");
    return logic_bomb(s);
    // fprintf(stderr, "[socket_cp_l1_klee.c] exit main 3\n");
    // fprintf(stderr, "[socket_cp_l1_klee.c] exit main 1\n");
}
// Total cost: 0.016865
// Total split cost: 0.000000, input tokens: 0, output tokens: 0, cache read tokens: 0, cache write tokens: 0, split chunks: [(0, 115)]
// Total instrumented cost: 0.016865, input tokens: 2878, output tokens: 1223, cache read tokens: 0, cache write tokens: 0
