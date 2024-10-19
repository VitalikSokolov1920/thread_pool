#include <stdio.h>
#include <unistd.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>

int main() {
    int sock = socket(AF_INET, SOCK_STREAM, 0);

    if (sock == -1) {
        perror("socket()");

        exit(EXIT_FAILURE);
    }

    struct sockaddr_in remote;

    remote.sin_port = htonl(4444);
    remote.sin_addr.s_addr = inet_addr("192.168.0.103");
    remote.sin_family = AF_INET;

    int ret = connect(sock, &remote, sizeof(remote));

    if (ret) {
        perror("connect()");

        exit(EXIT_FAILURE);
    }
}