#ifndef ARRAY_H
#define ARRAY_H

#include <stddef.h>

typedef struct {
    void* elems;
    size_t elem_size;
    size_t len;
} array_t;

array_t array_init(size_t size, size_t elem_size);

array_t* array_add(array_t* array, void* elem);

array_t* array_remove(array_t* array, size_t idx);

#endif