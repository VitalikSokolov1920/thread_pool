#include <stdio.h>
#include <unistd.h>

#include "thread_pool.h"

void* foo(void* arg) {
    fprintf(stdout, "foo(): start\n");

    thread_task_ctx_t* ctx = (thread_task_ctx_t*)arg;

    if (!ctx) {
        fprintf(stderr, "foo: ctx == NULL\n");

        return NULL;
    }

    int* i = (int*)ctx->data;

    fprintf(stdout, "foo: i = %d\n", *i);

    return NULL;
}

int main(int argc, char** argv) {
    thread_pool_t* pool = thread_pool_init();

    if (!pool) {
        fprintf(stderr, "Error during thread_pool_init\n");

        return -1;
    } else {
        fprintf(stdout, "Succes init pool\n");
    }

    for (int i = 0; i < 256; i++) {
        thread_task_t* task = thread_task_init(foo, &i, sizeof(i));

        if (!task) {
            fprintf(stderr, "Error during create task\n");
        }

        thread_pool_add_task(pool, task);

        sleep(1);
    }

    while (pool->queue->len) {
        fprintf(stdout, "Waiting...\n");
        sleep(1);
    }

    char c;

    scanf("%c", &c);
}