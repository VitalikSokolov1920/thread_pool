#ifndef THREAD_TASK_LIST_H
#define THREAD_TASK_LIST_H

#include <pthread.h>

#include "thread_task.h"

typedef unsigned int thread_task_list_len_t;

typedef struct {
    thread_task_t* head;
    thread_task_t* tail;
    thread_task_list_len_t len;
    pthread_mutex_t* mutex;
} thread_task_list_t;

thread_task_list_t* thread_task_list_init();

int add_task(thread_task_list_t* queue, thread_task_t* task);

thread_task_t* next(thread_task_list_t* queue);

void thread_task_list_destroy(thread_task_list_t* list);

#endif