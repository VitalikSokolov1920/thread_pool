#include <malloc.h>

#include "thread_task_list.h"

thread_task_list_t* thread_task_list_init() {
    thread_task_list_t* list = NULL;

    list = (thread_task_list_t*)malloc(sizeof(thread_task_list_t));

    if (!list) {
        return NULL;
    }

    pthread_mutex_t* mutex = (pthread_mutex_t*)malloc(sizeof(pthread_mutex_t));

    pthread_mutex_init(mutex, NULL);

    list->mutex = mutex;
    list->head = list->tail = NULL;
    list->len = 0;
    
    return list;
}

void thread_task_list_destroy(thread_task_list_t* list) {
    if (!list) {
        return;
    }

    if (list->mutex) {
        pthread_mutex_destroy(list->mutex);

        free(list->mutex);
    }

    if (list->head) {
        for (thread_task_t* cur = list->head; cur;) {
            thread_task_list_t* forDel = cur;

            cur = cur->next;

            thread_task_destoy(forDel);
        }
    }

    free(list);
}