#ifndef THREAD_TASK_H
#define THREAD_TASK_H

typedef unsigned int thread_task_ctx_data_len_t;

typedef struct {
    void* data;
    thread_task_ctx_data_len_t len;
} thread_task_ctx_t;

thread_task_ctx_t* thread_task_ctx_init(void* data, thread_task_ctx_data_len_t data_len);

void thread_task_ctx_destroy(thread_task_ctx_t* ctx);

typedef void*(*thread_task_handler_t)(void*); 

typedef struct {
    thread_task_handler_t task_handler;
    thread_task_ctx_t* task_ctx;
    thread_task_t* next;
} thread_task_t;

thread_task_t* thread_task_init(thread_task_handler_t handler, void* data, thread_task_ctx_data_len_t data_len);

void thread_task_destoy(thread_task_t* task);

#endif