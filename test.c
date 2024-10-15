#include <stdio.h>
#include <unistd.h>

#include "thread_pool.h"

void* foo(void* arg) {
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
    }

    for (int i = 0; i < 10; i++) {
        thread_task_t* task = thread_task_init(foo, &i, sizeof(i));

        if (!task) {
            fprintf(stderr, "Error during create task\n");
        }

        thread_pool_add_task(pool, task);
    }

    sleep(5);
}