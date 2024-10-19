#include <string.h>

#include "containers/list.h"

list_elem_t* list_elem_init(void* value) {
    list_elem_t* elem = (list_elem_t*)malloc(sizeof(list_elem_t));

    if (!elem) {
        return NULL;
    }

    elem->next = NULL;
    elem->value = value;

    return elem;
}

void list_elem_destroy(list_elem_t* elem) {
    if (!elem)
        return;

    free(elem);

    return;
}

list_t* list_init(size_t elem_size) {
    if (!elem_size) {
        return NULL;
    }

    list_t* list = (list_t*)malloc(sizeof(list_t));

    if (!list) {
        list_destroy(list);

        return NULL;
    }

    list->head = list->tail = NULL;
    list->len = 0;
    list->elem_size = elem_size;

    return list;
}

list_t* list_add(list_t* list, void* value) {
    if (list->tail) {
        list->tail->next = list_elem_init(value);
        list->tail = list->tail->next;
    } else {
        list->head = list_elem_init(value);
        list->tail = list->head;
    }

    if (!list->tail) {
        list_destroy(list);

        return NULL;
    }
    list->tail->next = NULL;
    list->len++;

    return list;
}

int has_next(list_t* list) {
    static list_elem_t* cur = NULL;

    if (!list) {
        if (cur) {
            cur = cur->next;

            return 1;
        }
        return 0;
    } else {
        cur = list->head;

        return cur ? 1 : 0;
    }
}

list_t* list_remove(list_t* list, size_t idx) {
    if (idx > list->len || !list->head) {
        return list;
    }

    list_elem_t* forDelete = NULL;

    if (!idx) {
        forDelete = list->head;

        list->head = list->head->next;

        list_elem_destroy(forDelete);
    }

    list_elem_t* prev = list->head;
    forDelete = prev->next;

    for (size_t i = 1; i < list->len; i++, forDelete = forDelete->next, prev = prev->next) {
        if (i == idx) {
            prev->next = forDelete->next;

            list_elem_destroy(forDelete);

            return list;
        }
    }

    return list;
}

void list_destroy(list_t* list) {
    if (!list) {
        return;
    }

    if (list->head) {
        list_elem_t* forDelete = NULL;
        while (list->head) {
            forDelete = list->head;
            list->head = list->head->next;

            list_elem_destroy(forDelete);
        }
    }

    free(list);
}