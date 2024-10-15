#ifndef ARRAY_H
#define ARRAY_H

typedef unsigned int array_len_t;

typedef struct {
    void* elems;
    array_len_t len;
} array_t;

#endif