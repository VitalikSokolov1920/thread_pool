#ifndef THREAD_SPECIALIZATION_H
#define THREAD_SPECIALIZATION_H

#include <stdatomic.h>

typedef struct {
    size_t spec_size; // количество потоков
    unsigned long spec_id; // id специализации
    atomic_uint available_threads; // количество свободных потоков в данный момент
    int* threads_pids;
} thread_specialization_t;

thread_specialization_t* thread_specialization_init(size_t spec_size);



void thread_specialization_destroy(thread_specialization_t* spec);

#endif