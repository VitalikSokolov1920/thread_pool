#include <stdio.h>
#include <unistd.h>

#include "thread_pool.h"
#include "list.h"

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

int task_pool() {
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

void test_list() {
    list_t* list = list_init(sizeof(user));

    if (!list) {
        fprintf(stderr, "Error: list_init()");
        
        return;
    }

    user user1 = {.name = "user1", .age = 21};
    user user2 = {.name = "user2", .age = 22};
    user user3 = {.name = "user3", .age = 23};

    list_add(list, &user1);
    list_add(list, &user2);
    list_add(list, &user3);

    printf("list size %lu\n", list->len);

    for (list_elem_t* cur = list->head; cur; cur = cur->next) {
        user* u = cur->value;

        printf("User name: %s, age: %d\n", u->name, u->age);
    }
}

int main(int argc, char** argv) {
    test_list();
}