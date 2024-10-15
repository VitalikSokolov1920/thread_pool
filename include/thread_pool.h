#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include "thread_task_list.h"

typedef struct {
    thread_task_list_t* queue;
    pthread_cond_t* cond;
} thread_pool_t;

typedef struct {
    pthread_cond_t* cond;
    pthread_mutex_t* cond_mutex;
    thread_pool_t* pool;
} thread_pool_arg_t;

thread_pool_t* thread_pool_init();

void* thread_pool_thread(void* arg);

int thread_pool_add_task(thread_pool_t* pool, thread_task_t* task);

void thread_pool_destroy(thread_pool_t* pool);

#endif