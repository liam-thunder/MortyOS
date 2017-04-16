#ifndef LIST_H
#define LIST_H

#include "types.h"

typedef struct list_node {
    struct list_node *prev;
    struct list_node *next;
} list_node_t;

static inline void init_list_head(list_node_t *node);

static inline void list_add(list_node_t *new_node, list_node_t *head_node);

// add node between two node
static inline void __list_add(list_node_t* new_node, list_node_t* prev, list_node_t* next);

static inline list_node_t* list_next(list_node_t* node);

inline void init_list_head(list_node_t *node) {
    node->next = node->prev = node;
}

inline void list_add(list_node_t *new_node, list_node_t *head_node) {
    __list_add(new_node, head_node, head_node->next);
}

inline void __list_add(list_node_t* new_node, list_node_t* prev_node, list_node_t* next_node) {
    prev_node->next = next_node->prev = new_node;
    new_node->prev = prev_node;
    new_node->next = next_node;
}

inline list_node_t* list_next(list_node_t* node) {
    return node->next;
}

#endif