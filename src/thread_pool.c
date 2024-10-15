#include <malloc.h>

#include "thread_pool.h"
#include "thread_task_list.h"

thread_pool_t* thread_pool_init() {
    thread_pool_t* pool = NULL;

    pool = (thread_pool_t*)malloc(sizeof(thread_pool_t));

    if (!pool) {
        return NULL;
    }

    pool->queue = thread_task_list_init();

    if (!pool->queue) {
        thread_pool_destroy(pool);

        return NULL;
    }

    pthread_cond_t* cond = (pthread_cond_t*)malloc(sizeof(pthread_cond_t));

    if (!cond) {
        thread_pool_destroy(pool);

        return NULL;
    }

    pthread_cond_init(cond, NULL);

    pool->cond = cond;

    return pool;
}

void* thread_pool_thread(void* arg) {
    thread_pool_arg_t* pool_arg = (thread_pool_arg_t*)arg;

    while(1) {
        pthread_mutex_lock(pool_arg->cond_mutex);
        pthread_cond_wait(pool_arg->cond, pool_arg->cond_mutex);
        pthread_mutex_unlock(pool_arg->cond_mutex);

        thread_task_t* task = thread_task_list_next_task(pool_arg->pool->queue);

        if (!task) {
            continue;
        }

        if (task->task_handler) {
            task->task_handler(task->task_ctx);
        }
    }

    return NULL;
}

void thread_pool_destroy(thread_pool_t* pool) {
    if (!pool) {
        return;
    }

    if (pool->queue) {
        thread_task_list_destroy(pool->queue);
    }

    if (pool->cond) {
        pthread_cond_destroy(pool->cond);
    }
}