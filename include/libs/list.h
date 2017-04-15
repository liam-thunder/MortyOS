#ifndef LIST_H
#define LIST_H

#include "types.h"

typedef struct list_node {
    struct list_node *prev;
    struct list_node *next;
} list_node_t;

static inline void init_list_head(list_node_t *node) __attribute__((always_inline));


static inline void init_list_head(list_node_t *node) {
    node->next = node->prev = node;
}

#endif