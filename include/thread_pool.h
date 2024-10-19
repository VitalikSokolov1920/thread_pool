#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <stdatomic.h>

#include "thread_task_list.h"

#define POOL_SIZE 16

typedef struct {
    thread_task_list_t* queue;
    pthread_cond_t* cond;
    pthread_mutex_t* cond_mutex;
    atomic_int available_threads;
    pthread_t main_thread_pid;
} thread_pool_t;

thread_pool_t* thread_pool_init();

void* thread_pool_thread(void* arg);

void* thread_main_thread(void* arg);

static void thread_sigusr1_handler(int);

int thread_pool_add_task(thread_pool_t* pool, thread_task_t* task);

void thread_pool_destroy(thread_pool_t* pool);

#endif