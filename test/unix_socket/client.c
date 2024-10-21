#include <stdio.h>
#include <unistd.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>

int main() {
    int sock = socket(AF_UNIX, SOCK_STREAM, 0);

    if (sock == -1) {
        perror("socket()");

        exit(EXIT_FAILURE);
    }

    struct sockaddr_un remote;

    memset(&remote, 0, sizeof(remote));

    remote.sun_family = AF_UNIX;
    strcpy(remote.sun_path, "./unix_socket");

    int ret = connect(sock, &remote, sizeof(remote));

    if (ret == -1) {
        perror("connect()");

        exit(EXIT_FAILURE);
    }
}