#ifndef ARRAY_H
#define ARRAY_H

#include <stddef.h>
#include <malloc.h>

#define CONVERT_ELEM_TO(elem, type) ((type)elem)

typedef struct {
    void* value;
    void* next;
} list_elem_t;

list_elem_t* list_elem_init(void* value);

void list_elem_destroy(list_elem_t* elem);

typedef struct {
    list_elem_t* head;
    list_elem_t* tail;
    size_t elem_size;
    size_t len;
} list_t;

list_t* list_init(size_t elem_size);

list_t* list_add(list_t* list, void* value);

static int has_next(list_t* list);

list_t* list_remove(list_t* list, size_t idx);

void list_destroy(list_t* list);

#endif