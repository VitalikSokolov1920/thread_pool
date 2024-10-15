#include <malloc.h>
#include <string.h>

#include "thread_task.h"

thread_task_ctx_t* thread_task_ctx_init(void* data, thread_task_ctx_data_len_t data_len) {
    thread_task_ctx_t* ctx = NULL;

    ctx = (thread_task_ctx_t*)malloc(sizeof(thread_task_ctx_t));

    if (!ctx) {
        return NULL;
    }

    ctx->data = malloc(data_len);

    if (!ctx->data) {
        thread_task_ctx_destroy(ctx);

        return NULL;
    }

    memcpy(ctx->data, data, data_len);

    return ctx;
}

void thread_task_ctx_destroy(thread_task_ctx_t* ctx) {
    if (!ctx) {
        return;
    }

    if (ctx->data) {
        free(ctx->data);
    }

    free(ctx);
}

thread_task_t* thread_task_init(thread_task_handler_t handler, void* data, thread_task_ctx_data_len_t data_len) {
    thread_task_t* task = NULL;

    task = (thread_task_t*)malloc(sizeof(thread_task_t));

    if (!task) {
        return NULL;
    }

    task->task_handler = handler;
    task->task_ctx = thread_task_ctx_init(data, data_len);

    if (!task->task_ctx) {
        thread_task_destoy(task);

        return NULL;
    }

    task->next = NULL;

    return task;
}

void thread_task_destoy(thread_task_t* task) {
    if (!task) {
        return;
    }

    if (task->task_ctx) {
        thread_task_ctx_destroy(task->task_ctx);
    }

    free(task);
}