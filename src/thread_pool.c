#include <malloc.h>
#include <signal.h>
#include <unistd.h>

#include "thread_pool.h"
#include "thread_task_list.h"

thread_pool_t* thread_pool_init() {
    // pool

    thread_pool_t* pool = NULL;

    pool = (thread_pool_t*)malloc(sizeof(thread_pool_t));

    if (!pool) {
        return NULL;
    }

    pool->queue = thread_task_list_init();

    // available_threads

    atomic_init(&pool->available_threads, POOL_SIZE);

    if (!pool->queue) {
        fprintf(stderr, "thread_pool_init(): Error during init queue\n");

        thread_pool_destroy(pool);

        return NULL;
    }

    // cond

    pthread_cond_t* cond = (pthread_cond_t*)malloc(sizeof(pthread_cond_t));

    if (!cond) {
        fprintf(stderr, "thread_pool_init(): Error during create pthread_cond_t\n");

        thread_pool_destroy(pool);

        return NULL;
    }

    pthread_cond_init(cond, NULL);

    pool->cond = cond;

    // cond_mutex

    pthread_mutex_t* cond_mutex = (pthread_mutex_t*)malloc(sizeof(pthread_mutex_t));

    if (!cond_mutex) {
        fprintf(stderr, "thread_pool_init(): Error during create pthread_mutex_t\n");

        thread_pool_destroy(pool);

        return NULL;
    }

    pthread_mutex_init(cond_mutex, NULL);

    pool->cond_mutex = cond_mutex;

    // attrs

    pthread_attr_t attrs = {0};

    pthread_attr_init(&attrs);

    pthread_create(&pool->main_thread_pid, &attrs, thread_main_thread, pool);

    pthread_t pid_list[POOL_SIZE] = { 0 };

    for (size_t i = 0; i < POOL_SIZE; i++) {
        pthread_t pid;
        pthread_create(&pid, &attrs, thread_pool_thread, pool);
        pid_list[i] = pid;
    }

    for (size_t i = 0; i < POOL_SIZE; i++) {
        printf("thread [%lu] pid: %lu\n", i, pid_list[i]);
    }

    pthread_attr_destroy(&attrs);

    return pool;
}

void* thread_pool_thread(void* arg) {
    thread_pool_t* pool = (thread_pool_t*)arg;

    while(1) {
        pthread_mutex_lock(pool->cond_mutex);
        pthread_cond_wait(pool->cond, pool->cond_mutex);
        pthread_mutex_unlock(pool->cond_mutex);

        if (thread_task_list_is_empty(pool->queue)) {
            continue;
        }

        thread_task_t* task = thread_task_list_next_task(pool->queue);

        if (!task) {
            continue;
        }

        fprintf(stdout, "thread_pool_thread(): starting execute task\n");

        void* res;

        if (task->task_handler) {
            res = task->task_handler(task->task_ctx);

            fprintf(stdout, "task_handler result %d\n", *((int*)res));
        }

        thread_task_destoy(task);
    }

    return NULL;
}

void* thread_main_thread(void* arg) {
    thread_pool_t* pool = (thread_pool_t*)arg;

    signal(SIGUSR1, thread_sigusr1_handler);

    if (!pool) {
        return NULL;
    }

    while (1) {
        if (pool->queue->len > 0) {
            if (pool->available_threads > 0) {
                pthread_cond_signal(pool->cond);
            } else {
                pause();

                pthread_cond_signal(pool->cond);
            }
        } else {
            pause();

            pthread_cond_signal(pool->cond);
        }
    }
}

static void thread_sigusr1_handler(int) {}

int thread_pool_add_task(thread_pool_t* pool, thread_task_t* task) {
    if (!thread_task_list_add_task(pool->queue, task)) {
        pthread_kill(pool->main_thread_pid, SIGUSR1);

        return 0;
    } else {
        fprintf(stderr, "Error during thread_task_list_add_task\n");

        return -1;
    }
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

        free(pool->cond);
    }

    if (pool->cond_mutex) {
        pthread_mutex_destroy(pool->cond_mutex);

        free(pool->cond_mutex);
    }
}