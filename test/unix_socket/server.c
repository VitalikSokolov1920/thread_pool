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

#include "../../include/thread_pool.h"

typedef struct {
    thread_pool_t* pool;
    int socket;
    struct sockaddr_un remote;
} process_conn_ctx_t;

void* process_conn(void* arg) {
    thread_task_ctx_t* ctx = (thread_task_ctx_t*)arg;

    process_conn_ctx_t* conn_ctx = (process_conn_ctx_t*)(ctx->data);

    printf("Socket: %d\n", conn_ctx->socket);

    return NULL;
}

void* server_thread(void* arg) {
    thread_task_ctx_t* ctx = (thread_task_ctx_t*)arg;

    int server_fd = socket(AF_UNIX, SOCK_STREAM, 0);

    if (server_fd == -1) {
        perror("socket():");

        return NULL;
    }

    int ret = 0;

    struct sockaddr_un adr;

    adr.sun_family = AF_UNIX;
    strcpy(adr.sun_path, "./unix_socket");

    if ((ret = bind(server_fd, &adr, sizeof(adr))) == -1) {
        perror("bind()");

        return NULL;
    }

    if ((ret = listen(server_fd, 5)) == -1) {
        perror("listen()");

        return NULL;
    }

    while (1) {
        struct sockaddr_un remote_adr;

        memset(&remote_adr, 0, sizeof(remote_adr));

        int sock = accept(server_fd, &remote_adr, sizeof(remote_adr));

        if (sock == -1) {
            perror("accept()");

            continue;
        }

        process_conn_ctx_t conn_ctx = {
            .pool = ctx->data,
            .socket = sock,
            .remote = remote_adr
        };

        thread_task_t* task = thread_task_init(process_conn, &conn_ctx, sizeof(process_conn_ctx_t));

        if (!task) {
            fprintf(stderr, "error: thread_task_init()\n");

            close(sock);

            continue;
        }

        thread_pool_add_task(ctx->data, task);
    }
}

int main(int argc, char** argv) {
    thread_pool_t* pool = thread_pool_init(1);

    if (!pool) {
        fprintf(stderr, "error: thread_pool_init()\n");

        exit(EXIT_FAILURE);
    }

    thread_task_t* task = thread_task_init(server_thread, pool, sizeof(pool));

    thread_pool_add_task(pool, task);

    scanf("Enter str: %s");
}